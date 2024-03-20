#!/bin/bash
# Simple script to call build tools within docker container.
# Author: Schuy

IMAGE_NAME="kiu"
CONTAINER_NAME="kiu_test"
REBUILD=0

help() {
	cat <<- xx
	Usage: $0 [OPTIONS] COMMAND

	Options:
	 -h,		Echoes a help command. 
	 -r,		Forces a rebuild of the image.
	
	Commands:
	 release	Builds the 3dsx file.
	 test		Builds and runs all tests.
	 clean		Cleans up build files.
	xx
}

check_if_exists() {
	val=$( docker images | grep "${IMAGE_NAME}" )
	if [ -z "${val}" ]; then
		return 1
	fi
	return 0
}

build_image() {
	yes "y" | docker image prune
	docker build -t ${IMAGE_NAME} .
}


while getopts "hr:" arg
do
	case $arg in
		h)
			help
			;;
		r)
			IMAGE_NAME=${OPTARG}
			build_image
			;;
	esac
done

# Run image
docker run --name test -d -i -t -v ./:/home/workspace/ kiu /bin/bash > /dev/null

# Check arguments
if [ "$1" = "test" ]; then
	docker exec -w /home/workspace/ test bash -c 'make test'
elif [ "$1" = "clean" ]; then
	docker exec -w /home/workspace/ test bash -c "make clean"
else
	echo "Error: Not a valid argument. Please see ./build.sh -h"
fi

docker stop test > /dev/null
yes "y" | docker container prune > /dev/null

