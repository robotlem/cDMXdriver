# cDMXdriver
Driver to use a serial port on linux to output a dmx-signal
# About
- `struct dmxPort`: A struct in which all important data is stored. It is stored in shared memory.
- `initDMXPort(port)`: The function to generate a dmxPort struct (as shared memory). It configures the serial port, initializes the variables and returns a pointer to the generated dmx Port. The parameter specifies the port e.g. "/dev/ttyUSB0".
- `setMaxDMXChannel(dmx, maxChannel)`: Set the highest send DMX channel. You should use this to get a higher output frequency cf. img/.
- `setDMXChannel(dmx, channel, value)`: Set a channel (1-512) to a value (0-255).
- `getDMXChannel(dmx, channel)`: Get a channel (1-512).
- `getDMXOutputPointer(dmx)`: Get a pointer to the output array. Channels from 0 to 511.
- `startDMXOutput(dmx)`: Start sending the dmx signal (makes a fork and call 'dmxThreadFunction').
- `stopDMXOutput(dmx)`: Stops the active output.
- `closeDMXPort(dmx)`: Stops sending signals and closes the serial port.
# Notes
