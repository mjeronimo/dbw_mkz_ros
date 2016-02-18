#!/usr/bin/env sh

# Disable suspend on lid close
echo 'Disabling suspend on lid close...'
gsettings set org.gnome.settings-daemon.plugins.power lid-close-ac-action nothing
gsettings set org.gnome.settings-daemon.plugins.power lid-close-battery-action nothing

# Remove unnecessary packages
echo 'Removing unnecessary packages...'
sudo apt-get update
sudo apt-get remove -y thunderbird transmission-gtk transmission-common unity-webapps-common brasero-common
sudo apt-get autoremove -y

# Upgrade
echo 'Upgrading system...'
sudo apt-get dist-upgrade -y
sudo apt-get update
sudo apt-get dist-upgrade -y
sudo apt-get autoremove -y

# Install ROS Indigo
echo 'Installing ROS Indigo...'
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
sudo apt-key adv --keyserver hkp://pool.sks-keyservers.net --recv-key 0xB01FA116
sudo apt-get update
sudo apt-get install -y ros-indigo-desktop
sudo rosdep init

# Update rosdep rules
echo 'Updating rosdep rules...'
rosdep update

# Setup environment
echo "source /opt/ros/indigo/setup.bash" >> ~/.bashrc

# Install SDK
echo 'Installing SDK...'
wget -q -O - https://bitbucket.org/DataspeedInc/dbw_mkz_ros/raw/default/dbw_mkz/scripts/sdk_install.sh | sh

echo 'ROS Indigo install: Done'

