# KIUEdit
KIUEdit is a save editor for Kid Icarus: Uprising on the 3DS. The application runs natively on the hardware, and can edit hearts, hearts donated to either Palutena or Viridi, and can edit the stars, modifiers, and types of weapons.

## Building
### Requirements
[Docker](https://www.docker.com/) is recommended to build the application within a controlled environment. However, it is also possible to build outside of a docker container. If you choose to do so, you will need to install [devkitpro](https://devkitpro.org/wiki/Getting_Started) and install `libctru`.

### Building Release
To build the release call `make release` from the root of the directory. **NOTE: This will not work without Docker installed and running.**

### Building within Docker Environment
Building and debugging with docker is fairly straightforward. You can run `./start_docker.sh`, which will build the required image and attach the docker image immediately. To see how to use the script please call `./start_docker.sh -h`.

You are also able to build the application by calling `make 3ds`. This will simply build the application, unlike `make release` which will build the application within a docker container.

## Testing
To run tests you'll need to install the  [Unity Test](https://github.com/ThrowTheSwitch/Unity) unit testing framework. You'll need a copy of it in the root directory of the project, which you should have running within a docker container. Please rename the testing framework directory to `unity` if it isn't already named so. You can run the following to run each test.
```
make test
```
The output to the terminal will only show failed tests and any output from the address sanitizer. You'll find a file containing all reports in `/build/test/results/` with a different text file for each source test file.

## Todo
The following features currently are incomplete:
- `make release`
- `make 3ds`
- `start_docker.sh` needs a way to remove the docker image and container. Likely will store image data through a JSON file.

## Issues
Currently most code in `test/Test_file.c` won't work as it relies on an input save file. I intend to write a script that will generate a mock save file, but as it stands I'm unwilling to add a genuine save file from a 3DS game to a github repo.

For the time being, `make 3ds` will fail, as there is no `src/main.c` file. This will change as development continues.
