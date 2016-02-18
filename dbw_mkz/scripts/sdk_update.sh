#!/usr/bin/env sh
MY_WORKSPACE=$HOME/dbw_ws

# Update workspace
wstool update -t $MY_WORKSPACE/src

# Resolve dependencies
rosdep update && rosdep install --from-paths $MY_WORKSPACE/src --ignore-src

# Build workspace
cd $MY_WORKSPACE
catkin_make -DCMAKE_BUILD_TYPE=Release -j1

echo 'SDK update: Done'

