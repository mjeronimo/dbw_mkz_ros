# Setup workspace
```
sudo apt-get install python-wstool
mkdir -p ~/dbw_ws/src
cd ~/dbw_ws
wstool init src
wstool merge -t src https://bitbucket.org/DataspeedInc/dbw_mkz_ros/raw/default/dbw_mkz.rosinstall
```

# Update workspace and resolve dependencies
```
wstool update -t src
rosdep update && rosdep install --from-paths src --ignore-src
```

# Install udev rules
```
sudo cp ~/dbw_ws/src/dataspeed_can/dataspeed_can_usb/90-DataspeedUsbCanToolRules.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules && sudo service udev restart && sudo udevadm trigger
```

# Build workspace
```
catkin_make -DCMAKE_BUILD_TYPE=Release
```

# Launch joystick demo
```
source ~/dbw_ws/devel/setup.bash
roslaunch dbw_mkz_joystick_demo joystick_demo.launch sys:=true
```

# Launch Drive-By-Wire system only
```
source ~/dbw_ws/devel/setup.bash
roslaunch dbw_mkz_can dbw.launch
```

# Launch RViz visualization
```
source ~/dbw_ws/devel/setup.bash
roslaunch dbw_mkz_description rviz.launch
```
