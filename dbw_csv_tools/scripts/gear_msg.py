#! /usr/bin/env python

signals=[
         '/state',
         '/cmd',
         '/driver'
         ]

def get_signal_names(topic):
    signal_names = []
    for sig in signals:
        signal_names.append(topic + sig)
    
    return signal_names

def modify_csv_row(msg, row, start_idx):
    row[start_idx:start_idx+len(signals)-1] = [msg.state.gear,
                                               msg.cmd.gear,
                                               int(msg.driver)
                                               ]
    return row