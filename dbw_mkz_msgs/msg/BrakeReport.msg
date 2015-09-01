Header header

# Brake pedal
# Unitless, range 0.15 to 0.50
float32 pedal_input
float32 pedal_cmd
float32 pedal_output

# Brake On Off (BOO), brake lights
bool boo_input
bool boo_cmd
bool boo_output

# Status
bool enabled
bool driver

# Faults
bool fault_ch1
bool fault_ch2
bool fault_boo
bool fault_connector
