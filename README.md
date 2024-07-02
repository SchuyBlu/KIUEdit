# KIUEdit
KIUEdit is a save editor for Kid Icarus: Uprising on the 3DS. The application runs natively on the hardware, and can edit hearts, hearts donated to either Palutena or Viridi, and can edit the stars, modifiers, and types of weapons.

**NOTE:** *This project is unfinished, and as such I cannot currently guarantee that building 3dsx will work. Building tests will always work.*

## Building
### Requirements
[Docker](https://www.docker.com/) is recommended to build the application within a controlled environment. However, it is also possible to build outside of a docker container. If you choose to do so, you will need to install [devkitpro](https://devkitpro.org/wiki/Getting_Started) and install `libctru`.

### Building 3dsx

To build the 3dsx file call `make release`.

### Building with Docker
You can run `./run.sh release` to build the `.3dsx` file. This calls docker, so `sudo` may be required depending on your system configuration.

## Testing
To run tests you'll need to install the [Unity Test](https://github.com/ThrowTheSwitch/Unity) unit testing framework. Please place this in the project directory. Rename the testing framework directory to `unity` if it isn't already named so. You can run the following to run each test.

### With Make
```
make test
```

### With Docker
```
./run.sh test
```

The output to the terminal will only show failed tests and any output from the address sanitizer. You'll find a file containing all reports in `/build/test/results/` with a different text file for each source test file.

## Todo
The following features currently are incomplete:
- Add ability to modify data on 3DS.
- Dynamically create submenu tree when entering savefile.

The following are features that will potentially be added, but are not a priority:
- Add new weapons.
- Modify data shown in the vault.
- A simple GUI.
