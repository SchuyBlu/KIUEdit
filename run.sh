#!/bin/bash
# Simple script to call build tools within docker container.
# Author: Schuy

IMAGE_NAME="kiu"
CONTAINER_NAME="dev_kiu"
REBUILD=0
MOUNTPOINT=${MOUNTPOINT:-$HOME/mnt/docker}

help() {
	cat <<- xx
	Usage: $0 [OPTIONS] COMMAND

	Options:
	 -h,		Echoes a help command. 
	 -r,		Forces a rebuild of the image.
	 -m,		Mount a container filesystem to /mnt/docker.
	 -u,		Unmount and stop a container filesystem from /mnt/docker.
	 -l,        Sends a 3dsx file to a 3DS.
	
	Commands:
	 release	Builds the 3dsx file.
	 test		Builds and runs all tests.
	 clean		Cleans up build files.
	xx
}

create_key() {
	if [ ! -f "./keys/id_rsa_shared" ]; then
		mkdir -p keys 2> /dev/null
		ssh-keygen -t ed25519 -f ./keys/id_rsa_shared -N "" -q
	fi
}

check_for_sshfs() {
	if [ -z "$( which sshfs )" ]; then
		echo "Error: Please install sshfs to mount container filesystem."
		exit 1
	fi
}

build_image() {
	create_key
	yes "y" | docker image prune
	docker build -t "${IMAGE_NAME}" .
}

check_if_exists() {
	# val=$( docker images | grep "$IMAGE_NAME" )
	# if [ -z "${val}" ]; then
	# 	build_image
	# fi
	if ! docker image inspect "${IMAGE_NAME}" > /dev/null; then
		build_image
	fi
}

kill_container_if_exists() {
	# if [ -n "$( docker ps | grep $CONTAINER_NAME )" ]; then
	# 	unmount_container
	# 	docker kill "$CONTAINER_NAME" > /dev/null 2> /dev/null
	# 	docker container rm "$CONTAINER_NAME" > /dev/null 2> /dev/null
	# 	docker container prune -f > /dev/null 2> /dev/null
	# fi
	if docker ps -aq -f "name=^${CONTAINER_NAME}$" > /dev/null 2>&1 && \
		[ -n "$( docker ps -aq -f "name=^${CONTAINER_NAME}$" )" ]; then
		unmount_container
		docker rm -f "${CONTAINER_NAME}" > /dev/null 2>&1 || true
		docker container prune -f > /dev/null 2>&1 || true
	fi
}

mount_container() {
	check_for_sshfs
	check_if_exists
	create_key
	kill_container_if_exists

	# Run and retrieve IP address.
	# docker run --name "$CONTAINER_NAME" -dt "$IMAGE_NAME"
	docker run --name "${CONTAINER_NAME}" -dt "${IMAGE_NAME}" > /dev/null 2>&1 ||  {
		echo "Error: failed to start container ${CONTAINER_NAME}"; exit 1;
	}

	# If the ip with key hasn't been registered, do so now
	cip=$( docker inspect --format='{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' "$CONTAINER_NAME" )
	if [ -z "${cip}" ]; then
		echo "Error: could not determine IP for ${CONTAINER_NAME}"; exit 1;
	fi

	# if [ -z "$( cat ~/.ssh/known_hosts | grep ${cip} )" ]; then
	# 	ssh-copy-id -i ./keys/id_rsa_shared user@${cip}
	# fi
	mkdir -p "$HOME/.ssh"
	ssh-keygen -R "${cip}" > /dev/null 2>&1 || true
	ssh-keyscan -H "${cip}" >> "$HOME/.ssh/known_hosts" 2> /dev/null || true

	# Create /mnt/docker if it doesn't exist
	if [ ! -d "/mnt/docker" ]; then
		mkdir /mnt/docker
	fi

	[ -d "$MOUNTPOINT" ] || mkdir -p "$MOUNTPOINT"

	# sshfs -o allow_other,default_permissions user@${cip}:/ /mnt/docker
	sshfs -o allow_other,default_permissions,IdentityFile=./keys/id_rsa_shared,StrictHostKeyChecking=no \
		"user@${cip}:/" "${MOUNTPOINT}"
	# sshfs -o allow_other,default_permissions,IdentityFile=./keys/id_rsa_shared,StrictHostKeyChecking=no \
	# 	"user@${cip}:/\" \"${MOUNTPOINT}\""
}

unmount_container() {
	check_for_sshfs

	# umount /mnt/docker 2> /dev/null
	# docker stop ${CONTAINER_NAME} > /dev/null
	if mount | grep -q " on ${MOUNTPOINT} "; then
		umount "${MOUNTPOINT}" 2> /dev/null || fusermount -u "${MOUNTPOINT}" 2> /dev/null || true
	fi
	docker stop "${CONTAINER_NAME}" > /dev/null 2>&1 || true
	yes "y" | docker container prune > /dev/null
}

while getopts ":hrmul" arg
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
		l) 
			if [ -n "${OPTARG}" ]; then
				CONTAINER_NAME="${OPTARG}"
			fi
			${MOUNTPOINT}/opt/devkitpro/tools/bin/3dslink ./editor.3dsx -a 192.168.0.246
	esac
done

check_if_exists

# Run image
# docker run --name test -d -i -t -v ./:/home/workspace/ kiu /bin/bash > /dev/null
docker run --name test -d -i -t -v ./:/home/workspace "${IMAGE_NAME}" /bin/bash > /dev/null

# Check arguments
if [ "$1" = "test" ]; then
	docker exec -w /home/workspace/ test bash -c 'make test'
elif [ "$1" = "clean" ]; then
	docker exec -w /home/workspace/ test bash -c "make clean"
elif [ "$1" = "release" ]; then
	docker exec -w /home/workspace/ test bash -c "make release"
fi

docker kill test > /dev/null
yes "y" | docker container prune > /dev/null

