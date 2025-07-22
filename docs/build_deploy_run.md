# Building, Deploying, and Running the Flight Controller

## Prerequisites

Generate the dockcross helper script. This allows us to cross-compile on our
host machine.

```bash
cd resources
./install_beagle_compiler.sh
```

## Build+Deploy Instructions

Run the build script with the `-d` option to deploy:

```bash
./build/build.sh -d
```

## Running

1. SSH into the beaglebone blue
2. If it has been a while since the last run or the sensors are noisy, then calibrate first.

```bash
sudo rc_calibrate_accel
sudo rc_calibrate_gyro
```

3. Run the executable created in the previous steps

```bash
sudo ~/Drone/bin/Drone
```

4. If everything worked, you should see something along the lines of the
   following. The system will wait for a connection from a commanding client.
   If no such connection is made in 30 seconds, then the controller will abort.

```
Performing ESC wakeup
Initializing barometer
Initializing DMP
Waiting for sensors to settle
Measurement system successfully initialized.
Hardware init success.
Starting threads.
Executing idle command.
Network handler successfully initialized.
set timeout to 30
Command handler successfully initialized.
Found initial command: 90816
Command status: STATUS_EXECUTING
Command currently executing or waiting. Pausing for completion.
Updating state from buffer
Buffer is empty. Waiting for signal.
```

To connect to the drone with the commanding interface, please see: [commanding_interface.md](./commanding_interface.md).
