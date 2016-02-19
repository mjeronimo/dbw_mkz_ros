#!/usr/bin/env sh
MY_WORKSPACE=$HOME/dbw_ws

# Update workspace
wstool merge -t $MY_WORKSPACE/src https://bitbucket.org/DataspeedInc/dbw_mkz_ros/raw/default/dbw_mkz.rosinstall
wstool update -t $MY_WORKSPACE/src

# Resolve dependencies
rosdep update && rosdep install -y -r --from-paths $MY_WORKSPACE/src --ignore-src

# Build workspace
cd $MY_WORKSPACE
catkin_make -DCMAKE_BUILD_TYPE=Release

echo 'SDK update: Done'

