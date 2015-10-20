#! /usr/bin/env python

signals=[
         '/pedal_input',
         '/pedal_cmd',
         '/pedal_output',
         '/enabled',
         '/driver'
         ]

def get_signal_names(topic):
    signal_names = []
    for sig in signals:
        signal_names.append(topic + sig)
    
    return signal_names

def modify_csv_row(msg, row, start_idx):
    row[start_idx:start_idx+len(signals)-1] = [msg.pedal_input,
                                               msg.pedal_cmd,
                                               msg.pedal_output,
                                               int(msg.enabled),
                                               int(msg.driver)
                                               ]
    return row
