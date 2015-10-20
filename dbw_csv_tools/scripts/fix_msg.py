#! /usr/bin/env python

signals=[
         '/latitude',
         '/longitude',
         '/altitude',
         '/status'
         ]

def get_signal_names(topic):
    signal_names = []
    for sig in signals:
        signal_names.append(topic + sig)
    
    return signal_names

def modify_csv_row(msg, row, start_idx):
    row[start_idx:start_idx+len(signals)-1] = [msg.latitude,
                                               msg.longitude,
                                               msg.altitude,
                                               msg.status.status
                                               ]
    return row