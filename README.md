# J.U.M.P.
JIAM Universal Measurement Probe: A measurement CNC device with swappable components

JUMP is designed as a measurement device which scans a 2D surface (namely thin film samples). It is built around an Arduino with two stepper motors, a linear motor, and a hot-swappable SPI measurement unit. Currently working is a K-type thermocouple on the MAX31855 chip, measuring the surface temperature of a material.

The Python script acts as a graphical user interface for the device, and it can both send and receive data. Given a command line argument for output, it will also produce a CSV file which can be converted into a 2D scatterplot or, eventually, an image.
