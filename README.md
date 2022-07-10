# cDMXdriver
Driver to use a serial port on linux to output a dmx-signal
# About
## Hardware
Use the TX pin of your serial port or usb to serial adapter (e.g. CP2102 or FT232RL) as a input for a RS485 transmitter (e.g.MAX485).
Connect the output of the RS485 transmitter to your DMX device.
#### example for MAX485:
- Pin 3, 8 at Vcc = 5V
- Pin 2, 8 at GND
- Pin 1 over 120Ohm at GND
- Pin 4 TX as input signal
- Pin 6 as DMX HOT (+)
- Pin 7 as DMX COLD (-)
## Functions
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
- I haven't tested the software with a real DMX device yet.
- I used a CP2102 USB to TTL adapter to test the software.
- Look in the img folder for oszi images.
- There are some ohter usefull functions. Take a look at cDMXdriver.h.
