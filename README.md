## Project Overview

### Hardware

- Beaglebone Blue
  - Configured using part 1 of guide from [imfatant](https://github.com/imfatant/test)
  - Installed [librobotcontrol](https://beagleboard.org/librobotcontrol/index.html)

- Drone Kit
  - [DJI Flamewheel F450](https://www.dji.com/flame-wheel-arf/feature)


### Software

- Flight controller written in C
- [librobotcontrol](https://beagleboard.org/librobotcontrol/index.html) for hardware access
- Commanding interface written in JS using node (not yet implemented)


### Compilation

- Docker image / support script that handles all cross-compilation needs
    - Started with [dockcross](https://github.com/dockcross/dockcross) linux-armv7 toolchain as base
    - glib of dockcross image was newer than my board supported, so I extended the linux-armv7 image and installed an older cross-compiler
    - The `resources/` directory of this repo provides an easy way to get access to the cross-compiler image


### Work to go

## Hardware

- Securly mount board on drone frame, current mounting solution may be causing elevated noise in sensors
- Acquire a dedicated battery to power board, or solder a new connection from the main battery
- Configure board to run flight controller on boot

## Software

- Refine control algorithm.
  - Currently using single PID controller, may need to move to cascaded configuration.
  - PID input is using direct gyro/accel output. These likely need to be fused for better attitude estimation. Sensor fusion is implemented for altitude estimation, but may need refinement.
- Add support for setting configuration values via commanding interface (e.g. PID)
- Node.js commanding interface
