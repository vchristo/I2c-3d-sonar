# I2c-3d-sonar
The Sonar 3D I2C has 5 ultrasonic sensors, 4 for the Horizontal plane and 1 for altitude.
 The objective is to prevent the drone from colliding with objects in the vicinity.
 The desired maximum distance of the drone from an object can be parameterized via I2C commands, the Arduino's PID_v1 library was used, data is returned via I2C.
