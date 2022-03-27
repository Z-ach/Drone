#!/usr/bin/env bash

# Load docker tar into image
docker build --platform linux/amd64 --pull -t zachkaufman/beagle_armv7 .

# Run and output dockcross helper script
docker run --platform linux/amd64 zachkaufman/beagle_armv7 > beagle_armv7

# Allow execute permissions
chmod +x beagle_armv7

# Move to build directory
mv beagle_armv7 ../build/
