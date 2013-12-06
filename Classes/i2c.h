#ifndef I2C_H
#define I2C_H

#include "registry.h"

//defines
#define I2C_DELAY 0x0F  /* For delay I2C bus */
#define I2C_OK 0
#define I2C_BUSY -1
#define I2C_NO_SUCH_DEVICE -2
#define I2C_NO_ACK_RECIEVED -3

//'private' primitives
void I2C_delay(void);
void I2C_clock(void);
void I2C_ack(void);
void I2C_noack(void);
void I2C_start(void);
void I2C_stop(void);
void I2C_available(unsigned char *av);
void I2C_ack_check(unsigned char *ack);
void I2C_read(unsigned char *data);
void I2C_write(unsigned char data);

//primitives
char I2C_readFromID(unsigned char id, unsigned char addr, unsigned char *data);
char I2C_writeToID(unsigned char id, unsigned char addr, unsigned char data);
//vectorial r/w
char I2C_readFromID_v(unsigned char id, unsigned char addr, unsigned char *data, unsigned char size);
char I2C_writeToID_v(unsigned char id, unsigned char addr, unsigned char *data, unsigned char size);

char I2C_ping(unsigned char addr, unsigned char *response);
char I2C_scan_network(unsigned char *list, unsigned char *size);

#endif
