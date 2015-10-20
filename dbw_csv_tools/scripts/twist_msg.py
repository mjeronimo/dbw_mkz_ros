#! /usr/bin/env python

signals=[
         '/linear/x',
         '/linear/y',
         '/linear/z',
         '/angular/x',
         '/angular/y',
         '/angular/z'
         ]

def get_signal_names(topic):
    signal_names = []
    for sig in signals:
        signal_names.append(topic + sig)
    
    return signal_names

def modify_csv_row(msg, row, start_idx):
    row[start_idx:start_idx+len(signals)-1] = [msg.linear.x,
                                               msg.linear.y,
                                               msg.linear.z,
                                               msg.angular.x,
                                               msg.angular.y,
                                               msg.angular.z
                                               ]
    return row