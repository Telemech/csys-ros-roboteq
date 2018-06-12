# 50Hz feedback message for controls purposes
Header header

# Current flowing in the motors (A)
float32 motor_current

# Output stage, as a proportion of full (-1..1)
float32 motor_power

# Commanded and measured speed of the motors (rad/s)
# Position is reported in rads, and wraps around +-6M
float32 commanded_velocity
float32 measured_velocity
float32 measured_position

# Electrical power supply to the driver (V, A)
float32 supply_voltage
float32 supply_current

# Channel temperature of the FET as reported by the controller (deg. C)
int8 channel_temperature
