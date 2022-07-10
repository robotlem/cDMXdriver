#include <stdio.h>
#include <unistd.h>
#include "cDMXdriver.h"

int main(int argc, char *argv[])
{

    // Generate a pointer to a dmxPort
    struct dmxPort *dmx1;

    // Open a serial port for DMX use
    // Store the returned pointer as dmx1
    dmx1 = initDMXPort("/dev/ttyUSB0");

    // Set the highest used channel to something between 1 and 512
    // A smaller amount of channels leads to a higher output frequency
    setMaxDMXChannel(dmx1, 100);

    // Start the outputing Process
    startDMXOutput(dmx1);

    sleep(2);

    // Set all channels to 255 = 100% with "setDMXChannel"
    for (int chan = 1; chan <= 512; chan++){
        setDMXChannel(dmx1, chan, 255);
    }

    sleep(2);

    // Set all channels to 128 = 50% with pointer to array
    unsigned char *universe1 = getDMXOutputPointer(dmx1);
    for (int chan = 0; chan < 512; chan++){
        universe1[chan] = 128;
    }

    sleep(2);

    // Stop sending signals and close the port
    closeDMXPort(dmx1);
    // !! Important to call "closeDMXPort" at the end to kill the child process and to close the port

    return 0;
}
