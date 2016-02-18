# One Line SDK Update

* Use this option if the SDK has already been installed.
* Paste the following into a terminal to update the SDK. This script will update the source code, install system dependencies, and build.
* ```wget -q -O - https://bitbucket.org/DataspeedInc/dbw_mkz_ros/raw/default/dbw_mkz/scripts/sdk_update.sh | sh```

# One Line SDK Install

* Use this option to install the SDK on a workstation that already has ROS installed.
* Paste the following into a terminal to install the SDK. This script will download the source code, install system dependencies, build, and setup your environment.
* ```wget -q -O - https://bitbucket.org/DataspeedInc/dbw_mkz_ros/raw/default/dbw_mkz/scripts/sdk_install.sh | sh```

# One Line ROS and SDK Install

* Use this option to install ROS Indigo and this SDK on a virgin workstation.
* This should ONLY be run on a fresh install of [Ubuntu 14.04 Desktop](http://releases.ubuntu.com/releases/14.04/).
* Paste the following into a terminal to install ROS Indigo and this SDK. This script will change some operating system parameters, install [ROS Indigo](http://wiki.ros.org/indigo/Installation/Ubuntu), and preform an SDK install.
* ```wget -q -O - https://bitbucket.org/DataspeedInc/dbw_mkz_ros/raw/default/dbw_mkz/scripts/ros_install.sh | sh```

# Manual install/update

* Setup workspace
  * ```sudo apt-get install python-wstool```
  * ```mkdir -p ~/dbw_ws/src && cd ~/dbw_ws && wstool init src```
  * ```wstool merge -t src https://bitbucket.org/DataspeedInc/dbw_mkz_ros/raw/default/dbw_mkz.rosinstall```
* Update workspace and resolve dependencies
  * ```wstool update -t src```
  * ```rosdep update && rosdep install --from-paths src --ignore-src```
* Install udev rules
  * ```sudo cp ~/dbw_ws/src/dataspeed_can/dataspeed_can_usb/90-DataspeedUsbCanToolRules.rules /etc/udev/rules.d/```
  * ```sudo udevadm control --reload-rules && sudo service udev restart && sudo udevadm trigger```
* Build workspace
  * ```catkin_make -DCMAKE_BUILD_TYPE=Release```

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
