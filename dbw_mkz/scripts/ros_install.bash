#! /bin/bash
MY_WORKSPACE=$HOME/dbw_ws

# Screen lock settings
echo 'Disabling screen lock...'
gsettings set org.gnome.desktop.screensaver lock-enabled false
gsettings set org.gnome.desktop.session idle-delay 0

# Disable suspend on lid close
echo 'Disabling suspend on lid close...'
gsettings set org.gnome.settings-daemon.plugins.power lid-close-ac-action nothing
gsettings set org.gnome.settings-daemon.plugins.power lid-close-battery-action nothing

# Remove unnecessary packages
echo 'Removing unnecessary packages...'
sudo apt-get update
sudo apt-get remove -y thunderbird transmission-gtk transmission-common unity-webapps-common brasero-common
sudo apt-get autoremove -y

# Disable error reporting and Amazon search results
gsettings set com.canonical.Unity.Lenses remote-content-search 'none'
sudo apt-get purge unity-webapps-common apport -y

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
echo "Installing SDK..."
bash <(wget -q -O - https://bitbucket.org/DataspeedInc/dbw_mkz_ros/raw/default/dbw_mkz/scripts/sdk_install.bash)

# Configure startup script
mkdir -p $HOME/.config/autostart
cp $MY_WORKSPACE/src/dbw_mkz_ros/dbw_mkz/scripts/joystick_demo.desktop $HOME/.config/autostart

### Misc fixes ###
# Fix launcher icons
echo 'Setting up launcher icons...'
gsettings set com.canonical.Unity.Launcher favorites "['application://nautilus.desktop', 'application://gnome-terminal.desktop']"

# List view in folders
echo 'Setting list view in folders...'
gsettings set org.gnome.nautilus.preferences default-folder-viewer 'list-view'

# Launch files open in gedit
echo 'Configuring launch files to open in gedit...'
xdg-mime default gedit.desktop application/xml

echo 'ROS Indigo install: Done'

