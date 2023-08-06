# KIUEdit
KIUEdit is a save editor for Kid Icarus: Uprising on the 3DS. The application runs natively on the hardware, and can edit hearts, hearts donated to either Palutena or Viridi, and can edit the stars, modifiers, and types of weapons.

## Building
To build you'll need a copy of [devkitPro](https://devkitpro.org/wiki/Getting_Started). After installing you can clone this using `git clone https://github.com/SchuyBlu/KIUEdit.git` and simply build by calling `make`.

## Testing
At this point you may have noticed that there are two separate makefiles. The makefile named `makefile.tests` is for testing, and does not generate a `.3dsx` file. In fact, this doesn't use the devkitPro toolchain at all, and runs on linux platforms (although it should run fine on msys2 or cygwin as well). As you may expect, because of this requirement the only code tested is code modular enough to be compiled on linux.

The [Unity](http://www.throwtheswitch.org/unity) framework is being used for testing, and a cloned repo of the source is expected in the project root in order to build. To build all tests you can call `make -f makefile.tests` and run as normal.
