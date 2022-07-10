#include "cDMXdriver.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <asm/termbits.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <sys/ioctl.h>

#include <sys/shm.h>

void initSerialDMXPort(int serial_port)
{

    struct termios2 tty;

    ioctl(serial_port, TCGETS2, &tty);

    // Config für DMX:
    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag |= CSTOPB;  // Set stop field, two stop bits used in communication
    tty.c_cflag &= ~CSIZE; // Clear all the size bits, then use one of the statements below
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    tty.c_cflag &= ~CBAUD;
    tty.c_cflag |= CBAUDEX;
    tty.c_ispeed = 250000; // 250000 baud for DMX
    tty.c_ospeed = 250000;

    ioctl(serial_port, TCSETS2, &tty);
}
void outputOneDMXCycle(struct dmxPort *dmx)
{
    ioctl(dmx->serial_port, TIOCSBRK);  // Generate the break
    ioctl(dmx->serial_port, TIOCCBRK);
    write(dmx->serial_port, dmx->output, dmx->maxOutput+1);
}
struct dmxPort * initDMXPort(char port[])
{
    struct dmxPort *shmDMXp;
    key_t shmKey = ftok(port,42);

    int shmid = shmget(shmKey,sizeof(struct dmxPort), 0644|IPC_CREAT);
    if (shmid == -1){
        printf("Shared memory create error");
        return NULL;
    }
    shmDMXp = shmat(shmid, NULL, 0);
    if (shmDMXp == (void *) -1) {
        perror("Shared memory attach error");
        return NULL;
    }

    shmDMXp->serial_port = open(port, O_RDWR);
    initSerialDMXPort(shmDMXp->serial_port);
    shmDMXp->maxOutput = 512;
    shmDMXp->activeOutput = 0;
    for (int z = 0; z <sizeof(shmDMXp->output);z++){
        shmDMXp->output[z] = 0;
    }

    return shmDMXp;
}
void closeDMXPort(struct dmxPort *dmx)
{
    stopDMXOutput(dmx);
    close(dmx->serial_port);
}
void setMaxDMXChannel(struct dmxPort *dmx, int maxChannnel)
{
    if (maxChannnel > 0 && maxChannnel <=512){
        dmx->maxOutput = maxChannnel;
    }
}
void setDMXChannel(struct dmxPort *dmx, int channel, unsigned int value)
{
    if(channel > 0 && channel < sizeof(dmx->output)){
        dmx->output[channel] = value;
    }
}
unsigned char getDMXChannel(struct dmxPort *dmx, int channel)
{
    return dmx->output[channel];
}
void dmxThreadFunction(struct dmxPort *dmx)
{
    while(dmx->activeOutput != 0){
        ioctl(dmx->serial_port, TIOCSBRK);
        ioctl(dmx->serial_port, TIOCCBRK);
        write(dmx->serial_port, dmx->output, dmx->maxOutput+1);
    }
    exit(0);
}
void startDMXOutput(struct dmxPort *dmx)
{
    if (dmx->activeOutput == 0){        // If not active jet...
        dmx->activeOutput = 1;          // start output
        pid_t pid = fork();
        if (pid < 0){
            printf("Error by forking");
        }
        else if (pid == 0){
            dmxThreadFunction(dmx);
        }
    }
}
void stopDMXOutput(struct dmxPort *dmx)
{
    dmx->activeOutput = 0;
}
unsigned char * getDMXOutputPointer(struct dmxPort *dmx)
{
    return &dmx->output[1];
}
