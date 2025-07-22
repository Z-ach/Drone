# Commanding Interface (Web UI)

## Overview

This web-based interface provides:

- Real-time telemetry visualization (including 3D orientation)
- Live updates of sensor readings and motor outputs
- Configuration control (PID tuning, throttle, etc.)

## Running the Interface

1. **Ensure the drone's flight controller is running**
   
   See [build_deploy_run.md](./build_deploy_run.md).

2. **Start the command server on your host machine**

   From the `cmd_interface` directory:

   ```bash
   node broker.js
   ```

3. Open `cmd_interface/index.html` in your browser.
4. Click "Connect to Server" to begin streaming telemetry.


