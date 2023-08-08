# KIUEdit
KIUEdit is a save editor for Kid Icarus: Uprising on the 3DS. The application runs natively on the hardware, and can edit hearts, hearts donated to either Palutena or Viridi, and can edit the stars, modifiers, and types of weapons.

## Building
To build you'll need a copy of [devkitPro](https://devkitpro.org/wiki/Getting_Started). After installing, you can execute the following commands to build.
```
git clone https://github.com/SchuyBlu/KIUEdit.git
cd ./KIUEdit
make 3ds
```

## Testing
At this point you may have noticed that there are two separate makefiles. The makefile named `Makefile` is for testing, and does not generate a `.3dsx` file, whereas `Makefile.3ds` is used for for generating the `.3dsx` executable. This is to allow for modular testing on linux systems, as the 3DS does not have any tools for testing code.

To run tests you'll need to install the  [Unity Test](https://github.com/ThrowTheSwitch/Unity) unit testing framework. You'll need a copy of it in the root directory of the project. If your system is case sensitive, make sure you rename the `Unity` directory to `unity`. You can run the following to run each test.
```
make test
```
The output to the terminal will only show failed tests and any output from the address sanitizer. You'll find a file containing all reports in `/build/test/results/` with a different text file for each source test file.

