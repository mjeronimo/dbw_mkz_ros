#! /usr/bin/env python

signals=[
         '/orientation/w',
         '/orientation/x',
         '/orientation/y',
         '/orientation/z',
         '/linear_acceleration/x',
         '/linear_acceleration/y',
         '/linear_acceleration/z',
         '/angular_velocity/x',
         '/angular_velocity/y',
         '/angular_velocity/z'
         ]

def get_signal_names(topic):
    signal_names = []
    for sig in signals:
        signal_names.append(topic + sig)
    
    return signal_names

def modify_csv_row(msg, row, start_idx):
    row[start_idx:start_idx+len(signals)-1] = [msg.orientation.w,
                                               msg.orientation.x,
                                               msg.orientation.y,
                                               msg.orientation.z,
                                               msg.linear_acceleration.x,
                                               msg.linear_acceleration.y,
                                               msg.linear_acceleration.z,
                                               msg.angular_velocity.x,
                                               msg.angular_velocity.y,
                                               msg.angular_velocity.z,
                                               ]
    return row