#! /usr/bin/env python

signals=[
         '/turn_signal',
         '/high_beam_headlights',
         '/wiper',
         '/ambient_light',
         '/btn_cc_on_off',
         '/btn_cc_res_cncl',
         '/btn_cc_set_inc',
         '/btn_cc_set_dec',
         '/btn_cc_gap_inc',
         '/btn_cc_gap_dec',
         '/btn_la_on_off',
         ]

def get_signal_names(topic):
    signal_names = []
    for sig in signals:
        signal_names.append(topic + sig)
    
    return signal_names

def modify_csv_row(msg, row, start_idx):
    row[start_idx:start_idx+len(signals)-1] = [msg.turn_signal.turn_signal,
                                               int(msg.high_beam_headlights),
                                               msg.wiper.wiper,
                                               msg.ambient_light.ambient_light,
                                               int(msg.btn_cc_on_off),
                                               int(msg.btn_cc_res_cncl),
                                               int(msg.btn_cc_set_inc),
                                               int(msg.btn_cc_set_dec),
                                               int(msg.btn_cc_gap_inc),
                                               int(msg.btn_cc_gap_dec),
                                               int(msg.btn_la_on_off)
                                               ]
    return row