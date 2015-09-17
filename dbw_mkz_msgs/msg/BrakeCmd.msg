# Brake pedal
# Options defined below
float32 pedal_cmd
uint8 pedal_cmd_type

# Brake On Off (BOO), brake lights
bool boo_cmd

# Enable. Also used to clear driver override
bool enable

uint8 CMD_NONE=0
uint8 CMD_PEDAL=1  # Unitless, range 0.15 to 0.50
uint8 CMD_TORQUE=2 # Nm, range 0 to 3250
