#! /usr/bin/env python

signals=[
         '/steering_wheel_angle',
         '/steering_wheel_angle_cmd',
         '/steering_wheel_torque',
         '/speed',
         '/enabled',
         '/driver'
         ]

def get_signal_names(topic):
    signal_names = []
    for sig in signals:
        signal_names.append(topic + sig)
    
    return signal_names

def modify_csv_row(msg, row, start_idx):
    row[start_idx:start_idx+len(signals)-1] = [msg.steering_wheel_angle,
                                               msg.steering_wheel_angle_cmd,
                                               msg.steering_wheel_torque,
                                               msg.speed,
                                               int(msg.enabled),
                                               int(msg.driver)
                                               ]
    return row