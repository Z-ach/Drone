#!/usr/bin/env bash

# Get dir name where script lives
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

# Define dockcross compilation helper script, build/src dirs
DOCKCROSS="$DIR/beagle_armv7"
BUILD="build"
SRC="src"

# Move into build directory and clean cmake/make files
cd $DIR
rm -r CMake*
rm cmake*
rm Makefile

# Important - must cd to root proj dir, otherwise dockcross helper script
# will not have access to relevant directories
cd ..

# Run dockcross script with cmake, then make
$DOCKCROSS cmake -S$SRC -B$BUILD
$DOCKCROSS make -C$BUILD

if [ "$1" == "-d" ]; then
    # If deploy flag was set, rsync compiled binary to drone
    rsync bin/Drone bbb:~/Drone/bin/
fi
