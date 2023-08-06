# KIUEdit
KIUEdit is a save editor for Kid Icarus: Uprising on the 3DS. The application runs natively on the hardware, and can edit hearts, hearts donated to either Palutena or Viridi, and can edit the stars, modifiers, and types of weapons.

## Building
To build you'll need a copy of [devkitPro](https://devkitpro.org/wiki/Getting_Started). After installing you can clone this using `git clone https://github.com/SchuyBlu/KIUEdit.git` and simply build by calling `make 3ds`.

## Testing
At this point you may have noticed that there are two separate makefiles. The makefile named `Makefile` is for testing, and does not generate a `.3dsx` file, whereas `Makefile.3ds` is used for for generating the `.3dsx` executable. This is to allow for modular testing on linux systems, as the 3DS does not have any tools for testing code.

To run tests, all you need to do is run `make test` and all tests will be run. The output to the terminal will only show failed tests and memory leaks. You'll find a file containing all reports in `/build/test/results/` with a different text file for each test.

