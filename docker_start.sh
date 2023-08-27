#!/bin/bash
# Author: Schuy
# Simple script to run docker building install process.
# NOTE: Way to check if git username and email are in use is kinda
#       hacky, but it works!

IMAGE_NAME="buster_slim"
CONTAINER_NAME="buster_slim"
USERNAME="user"
GIT_NAME="none"
GIT_EMAIL="none"
VOLUME_OPTION=""
REBUILD=false

help () {
	cat <<- xx
	Usage: $0 [OPTIONS]

	Options:
	 -h,				Echoes a help command.
	 -n <input>,			Name the container.
	 -t <input>,			Name the image.
	 -u <user>,			Sets the username for the user in the container.
	 -g <name>:<email>,		Sets a global git config name and email for the image.
	 -v <mount_path>,		Expects absolute path. Sets the path for the volume mount.
	xx
	exit 0
}


container_is_exited () {
	val=$( docker ps -a | grep -E "${CONTAINER_NAME}.*Exited" )
	if [ -z "${val}" ]; then
		return 1
	fi
	return 0
}


container_is_running () {
	val=$( docker ps | grep "${CONTAINER_NAME}" )
	if [ -z "${val}" ]; then
		return 1
	fi
	return 0
}

check_if_image_exists () {
	val=$( docker images | grep "${IMAGE_NAME}" )
	if [ -z "${val}" ]; then
		return 1
	fi
	return 0
}


restart_container () {
	# Attempt to stop, it doesn't matter if the container is running or not.
	docker stop ${CONTAINER_NAME} 2> /dev/null
	yes "y" | docker container prune > /dev/null

	# Reboot...
	docker run --name ${CONTAINER_NAME} ${VOLUME_OPTION} -dt ${IMAGE_NAME} > /dev/null
	if [ $? -eq 0 ]; then
		return 0
	fi
	return 1
}


attach_container () {
	# Restart container regardless if mount point is set.
	if ! [ -z "${VOLUME_OPTION}" ]; then
		echo "Restarting..."
		restart_container
	fi

	container_is_running
	if [ $? -eq 0 ]; then
		docker exec -it ${CONTAINER_NAME} /bin/zsh
		exit 0
	fi

	# Restart container if exited.
	restart_container
	if [ $? -eq 0 ]; then
		docker exec -it ${CONTAINER_NAME} /bin/zsh
		exit 0
	fi
	return 1
}


build_image () {
	# Clean any broken image
	yes "y" | docker image prune
	docker build \
		-t ${IMAGE_NAME} \
		--build-arg USERNAME=${USERNAME} \
		--build-arg GIT_NAME=${GIT_NAME} \
		--build-arg GIT_EMAIL=${GIT_EMAIL} \
		.
}


while getopts "hn:t:u:g:v:" arg
do
	case $arg in
		h)
			help
			;;
		n)
			CONTAINER_NAME=${OPTARG}
			;;
		t)
			IMAGE_NAME=${OPTARG}
			REBUILD=true
			;;
		u)
			USERNAME=${OPTARG}
			REBUILD=true
			;;
		g)
			GIT_NAME=$( echo ${OPTARG} | cut -d ":" -f 1 )
			GIT_EMAIL=$( echo ${OPTARG} | cut -d ":" -f 2 )
			REBUILD=true
			;;
		v)
			VOLUME_OPTION="-v ${OPTARG}:/home/${USERNAME}/workspace"
			;;
	esac
done

# Rebuild image if any of the optional flags that modify the build process have been passed.
if [ ${REBUILD} = true ]; then
	build_image
fi

# Check if the image exists. If not, build it.
check_if_image_exists
if [ $? -eq 1 ]; then
	build_image
fi

# Launch container.
attach_container

