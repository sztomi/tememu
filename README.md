# tememu == Tiny Embeddable MIPS Emulator

This project is hardly usable ATM. It is going to be a small&fast MIPS32 emulator at one point. 

# Status

Right now, only a handful of instructions are understood by the emulator. 
I'm aiming for 100% test coverage, so I'm progressing slowly but steadily. 

Right now the master branch is *very* unstable. Once I have a stable version, 
I'll stop blindly commiting into the master branch and only merge once a feature
is complete and the overall lib is usable.

First stable version is a version that emulates a MIPS version correctly (probably R2000).

Currently, the greatest program it can run is a fibonacci generator :).

# Building

Use premake to generate a suitable makefile/project file. 

## Build dependencies:

As Ubuntu/Debian package names, you can probably copy-paste these to install them on these distros):
libboost-dev 

## Other dependencies

  * googletest 1.6 from http://code.google.com/p/googletest
    (compile&place gtest and gtest_main static library files into thirdparty/lib)

## Building on *nix:

    premake4 gmake
    make config=release

or

    make config=debug

To run the unit tests, execute:

    ./build/release/test

from the root directory of the project. The unit tests use binary programs from ./testmips folder
(i.e. current working directory/testmips). It won't work if you cd into that folder.




