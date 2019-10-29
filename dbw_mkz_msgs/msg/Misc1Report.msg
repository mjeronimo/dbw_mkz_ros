
Header header

# Turn Signal enumeration
TurnSignal turn_signal

# High beams
bool high_beam_headlights

# Front Windshield Wipers enumeration
Wiper wiper

# Ambient Light Sensor enumeration
AmbientLight ambient_light

# Outside air temperature
float32 outside_temperature # Degrees Celsius

# Buttons
bool btn_cc_on        # Cruise Control On
bool btn_cc_off       # Cruise Control Off
bool btn_cc_on_off    # Cruise Control On/Off Toggle
bool btn_cc_res       # Cruise Control Resume
bool btn_cc_cncl      # Cruise Control Cancel
bool btn_cc_res_cncl  # Cruise Control Resume/Cancel
bool btn_cc_res_inc   # Cruise Control Resume+
bool btn_cc_res_dec   # Cruise Control Resume-
bool btn_cc_set_inc   # Cruise Control Set+
bool btn_cc_set_dec   # Cruise Control Set-
bool btn_cc_gap_inc   # Cruise Control Gap+
bool btn_cc_gap_dec   # Cruise Control Gap-
bool btn_la_on_off    # Lane Assist On/Off Toggle
bool btn_ld_ok        # Left D-Pad OK
bool btn_ld_up        # Left D-Pad Up
bool btn_ld_down      # Left D-Pad Down
bool btn_ld_left      # Left D-Pad Left
bool btn_ld_right     # Left D-Pad Right
bool btn_r_ok         # Steering wheel button on the right side, Ok(Center)
bool btn_r_up         # Steering wheel button on the right side, Up
bool btn_r_down       # Steering wheel button on the right side, Down
bool btn_r_left       # Steering wheel button on the right side, Left
bool btn_r_right      # Steering wheel button on the right side, Right
bool btn_r_call_start # Steering wheel button on the right side, Call start
bool btn_r_call_end   # Steering wheel button on the right side, Call end
bool btn_r_speak      # Steering wheel button on the right side, Speak
bool btn_r_mute       # Steering wheel button on the right side, Mute
bool btn_r_vol_up     # Steering wheel button on the right side, Volume up
bool btn_r_vol_down   # Steering wheel button on the right side, Volume down

# Faults
bool fault_bus

# Doors
bool door_driver
bool door_passenger
bool door_rear_left
bool door_rear_right
bool door_hood
bool door_trunk

# Passenger seat
bool passenger_detect
bool passenger_airbag

# Seat Belts
bool buckle_driver
bool buckle_passenger
