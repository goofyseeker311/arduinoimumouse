# USB Arduino IMU mouse
Using an integrated IMU on Arduino Nano 33 IOT as an USB mouse. Mouse X-Y moving and lift-off detection is working. Assumes that Arduino is attached to a weight slab that keeps the circuit board level on a surface or desktop.

Current implementation does not have any mouse buttons or wheel functionality. IMU accelerometer position algorithm should be updated to more proper model. Current IMU acceleration sensor 10 seconds (1000 samples average at 104Hz) running average is calculated to threshold any acceleration at a minimum.
