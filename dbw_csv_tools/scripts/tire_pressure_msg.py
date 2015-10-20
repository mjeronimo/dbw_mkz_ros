#! /usr/bin/env python

signals=[
         '/front_left',
         '/front_right',
         '/rear_left',
         '/rear_right'
         ]

def get_signal_names(topic):
    signal_names = []
    for sig in signals:
        signal_names.append(topic + sig)
    
    return signal_names

def modify_csv_row(msg, row, start_idx):
    row[start_idx:start_idx+len(signals)-1] = [msg.front_left,
                                               msg.front_right,
                                               msg.rear_left,
                                               msg.rear_right
                                               ]
    return row