#!/usr/bin/env bash

# To run at startup, add the line below to 'Startup Applications'
# gnome-terminal -e "/path/to/this/file/startup_joystick_rviz.bash"

# Source ~/.bashrc, the required workspace must be in your .bashrc file
source $HOME/dbw_ws/devel/setup.bash

# Launch
roslaunch dbw_mkz_joystick_demo joystick_demo.launch sys:=true rviz:=true

