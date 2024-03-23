#!/bin/bash
# Simple script to call build tools within docker container.
# Author: Schuy

IMAGE_NAME="kiu"
CONTAINER_NAME="dev"
REBUILD=0

help() {
	cat <<- xx
	Usage: $0 [OPTIONS] COMMAND

	Options:
	 -h,		Echoes a help command. 
	 -r,		Forces a rebuild of the image.
	 -m,		Mount a container filesystem to /mnt/docker.
	 -u,		Unmount and stop a container filesystem from /mnt/docker.
	
	Commands:
	 release	Builds the 3dsx file.
	 test		Builds and runs all tests.
	 clean		Cleans up build files.
	xx
	exit 0
}

check_for_sshfs() {
	if [ -z "$( which sshfs )" ]; then
		echo "Error: Please install sshfs to mount container filesystem."
		exit 1
	fi
}

build_image() {
	yes "y" | docker image prune
	docker build -t "${IMAGE_NAME}" .
}

check_if_exists() {
	val=$( docker images | grep "${IMAGE_NAME}" )
	if [ -z "${val}" ]; then
		build_image
	fi
}

mount_container() {
	check_for_sshfs
	check_if_exists

	# Run and retrieve IP address.
	docker run --name ${CONTAINER_NAME} -dt ${IMAGE_NAME}
	cip=$( docker inspect --format='{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' ${CONTAINER_NAME} )

	# Create /mnt/docker if it doesn't exist
	if [ ! -d "/mnt/docker" ]; then
		mkdir /mnt/docker
	fi

	sshfs -o allow_other,default_permissions user@${cip}:/ /mnt/docker
}

unmount_container() {
	check_for_sshfs

	umount /mnt/docker
	docker stop ${CONTAINER_NAME} > /dev/null
	yes "y" | docker container prune > /dev/null
}

while getopts ":hrmu" arg
do
	case $arg in
		h)
			help
			;;
		r)
			REBUILD=1
			if [ -n "${OPTARG}" ]; then
				IMAGE_NAME="${OPTARG}"
			fi
			build_image
			;;
		m)
			if [ -n "${OPTARG}" ]; then
				CONTAINER_NAME="${OPTARG}"
			fi
			mount_container
			exit 0
			;;
		u)
			if [ -n "${OPTARG}" ]; then
				CONTAINER_NAME="${OPTARG}"
			fi
			unmount_container
			exit 0
			;;
	esac
done

check_if_exists

# Run image
docker run --name test -d -i -t -v ./:/home/workspace/ kiu /bin/bash > /dev/null

# Check arguments
if [ "$1" = "test" ]; then
	docker exec -w /home/workspace/ test bash -c 'make test'
elif [ "$1" = "clean" ]; then
	docker exec -w /home/workspace/ test bash -c "make clean"
elif [ "${REBUILD}" -eq 0 ]; then
	echo ${REBUILD}
	echo "Error: Not a valid argument. Please see ./build.sh -h"
fi

docker stop test > /dev/null
yes "y" | docker container prune > /dev/null

