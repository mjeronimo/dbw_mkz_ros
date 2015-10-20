#! /usr/bin/env python

def get_signal_names(topic):
    return [topic]

def modify_csv_row(msg, row, start_idx):
    row[start_idx] = int(msg.data)
    return row
    
