#!/usr/bin/env sh
MY_WORKSPACE=$HOME/dbw_ws

# Install wstool
sudo apt-get install -y python-wstool

# Setup workspace
mkdir -p $MY_WORKSPACE/src
wstool init $MY_WORKSPACE/src
wstool merge -t $MY_WORKSPACE/src https://bitbucket.org/DataspeedInc/dbw_mkz_ros/raw/default/dbw_mkz.rosinstall

# Update workspace
wstool update -t $MY_WORKSPACE/src

# Install udev rules
sudo cp $MY_WORKSPACE/src/dataspeed_can/dataspeed_can_usb/90-DataspeedUsbCanToolRules.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules && sudo service udev restart && sudo udevadm trigger

# Setup environment
echo "source ~/dbw_ws/devel/setup.bash" >> ~/.bashrc

# Call SDK Update script
$MY_WORKSPACE/src/dbw_mkz_ros/dbw_mkz/scripts/sdk_update.sh

echo 'SDK install: Done'

