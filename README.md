# MRoboSub Flight Controller Firmware

## Commands
All commands must are in the format COMMAND:{ARGUMENT1}[,{ARGUMENT2}] and should be sent on their own lines over serial.

* HEARTBEAT:{NUMBER}
    Currently not implemented, reserved for automatic estop in case of ros network crash.
* POWER:{INDEX},{PERCENT}
    Used to apply power to motor, example `POWER:1,-25` will set motor index 1 to -25%. Index ranges from 0 to 7, and Percent ranges from -100 to 100
* ENABLE:{TRUE|FALSE}
    Used to enable or disable the flight controller, while disabled (default powerup state) all outputs are set to 0 and POWER commands will not afect outputs.
    When enabling are motors are commanded to 0 power.
    `ENABLE:TRUE` or `ENABLE:FALSE`
* ESTOP:
    This will permanently disable the outputs until the next full power cycle, designed to be a safety mechanism allowing ROS nodes to have no control over sub movement in case of failure.
    `ESTOP:`

## Web Interface
Website is accessible at `192.168.4.1` when connected to the `MROBOSUB_TEST_CONTROL:robo rocks` wifi network. This website allows for enabling and disabling control and setting motor power. The controller still responds to serial commands when using the website so all serial transmitters should be disabled for full website control.