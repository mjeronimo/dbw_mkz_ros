#! /usr/bin/env python

signals=[
         '/pedal_input',
         '/pedal_cmd',
         '/pedal_output',
         '/torque_input',
         '/torque_cmd',
         '/torque_output',
         '/boo_input',
         '/boo_cmd',
         '/boo_output',
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
                                               msg.torque_input,
                                               msg.torque_cmd,
                                               msg.torque_output,
                                               int(msg.boo_input),
                                               int(msg.boo_cmd),
                                               int(msg.boo_output),
                                               int(msg.enabled),
                                               int(msg.driver)
                                               ]
    return row
