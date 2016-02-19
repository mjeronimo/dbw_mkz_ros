#!/usr/bin/env bash

# To run at startup, add the line below to 'Startup Applications'
# gnome-terminal -e "/path/to/this/file/startup_dbw.bash"

# Source ~/.bashrc, the required workspace must be in your .bashrc file
source $HOME/dbw_ws/devel/setup.bash

# Launch
roslaunch dbw_mkz_can dbw.launch

