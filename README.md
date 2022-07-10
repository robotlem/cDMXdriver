# cDMXdriver
Driver to use a serial port on linux to output a dmx-signal
# About
- struct dmxPort: A struct in which all important data is stored. It is stored in shared memory.
- initDMXPort(port): The function to generate a dmxPort struct (as shared memory). It configures the serial port, initializes the variables and returns a pointer to the generated dmx Port. The parameter specifies the port e.g. "/dev/ttyUSB0".
- setMaxDMXChannel(dmx, maxChannel): Set the highest send DMX channel. You should use this to get a higher output frequency cf. img/ .
- 
