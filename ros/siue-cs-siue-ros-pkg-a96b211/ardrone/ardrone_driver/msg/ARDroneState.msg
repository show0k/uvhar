Header header

#The current altitude, in meters
float32 altitude

#Indicates whether the drone is airborne
bool is_airborne

#Percentage of battery life remaining. Note that because of the way the hardware reports it's battery life
# the drone could still be powered on, yet be reporting a battery percentage of 0. This means it has determined
# it does not have enough battery to fly, and it will not takeoff if commanded to.
int32 battery_percentage

#Battery voltage
float32 battery_voltage

#Indicates if the drone is in emergency mode
bool estop_state

#Current speed in meters/sec, calculated by the onboard IMU data fused with optical flow
# from the downward facing camera
ardrone_driver/Velocity vels
