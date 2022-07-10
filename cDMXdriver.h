#ifndef CDMXDRIVER_H_INCLUDED
#define CDMXDRIVER_H_INCLUDED

struct dmxPort{
    volatile int serial_port;
    unsigned char output[513];
    volatile int maxOutput;
    volatile unsigned char activeOutput;
};

void initSerialDMXPort(int serial_port);        // Configuration of the serial port

void outputOneDMXCycle(struct dmxPort *dmx);    // Output one dmx cycle; great for testing

struct dmxPort *initDMXPort(char port[]);       // Generate a configured dmxPort struct

void closeDMXPort(struct dmxPort *dmx);         // Close the dmx port

void setMaxDMXChannel(struct dmxPort *dmx, int maxChannnel);                // Set the maximal used channel

void setDMXChannel(struct dmxPort *dmx, int channel, unsigned int value);   // Set the value of a channel

unsigned char getDMXChannel(struct dmxPort *dmx, int channel);              // Get the value of a channel

void dmxThreadFunction(struct dmxPort *dmx);    // Function to output dmx

void startDMXOutput(struct dmxPort *dmx);       // Function to start dmx output (forks and calls dmxThreadFunction)

void stopDMXOutput(struct dmxPort *dmx);        // Function to stop dmx output

unsigned char *getDMXOutputPointer(struct dmxPort *dmx);                    // Get a pointer to the output-array


#endif
