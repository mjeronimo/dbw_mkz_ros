#! /bin/bash
current_dir=$(pwd)
rosparam load $current_dir/params.yaml
rosrun dbw_csv_tools bag_to_csv.py
