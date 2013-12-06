#include "i2c.h"

#define EXPERIMENTAL 0

void I2C_delay_t(unsigned char d) {
#if defined EXPERIMENTAL && EXPERIMENTAL = 1
	for (unsigned char i = 0; i < d; i++);
#else
	unsigned char i;
	for (i = 0; i < d; i++);
#endif
	return;
}

void I2C_delay(void) {
#if defined EXPERIMENTAL && EXPERIMENTAL = 1
	for (unsigned char i = 0; i < I2C_DELAY; i++);
#else
	unsigned char i;
	for (i = 0; i < I2C_DELAY; i++);
#endif
	return;
}

void I2C_clock(void) {
	I2C_SCL = 1;
	I2C_delay();
	I2C_SCL = 0;
	I2C_delay();
	return;
}

void I2C_ack(void) {
	I2C_SDA = 0;
	I2C_delay();
	return;
}

void I2C_noack(void) {
	I2C_SDA = 1;
	I2C_delay();
	return;
}

void I2C_start(void) {
	I2C_SDA = 0;
	I2C_delay();
	I2C_SCL = 0;
	I2C_delay();
	return;
}

void I2C_stop(void) {
	I2C_SDA = 0;
	I2C_delay();
	I2C_SCL = 1;
	I2C_delay();
	I2C_SDA = 1;
	I2C_delay();
	return;
}

void I2C_available(unsigned char *av) {
	unsigned char ack1 = I2C_SDA & I2C_SCL;
	I2C_delay();
	unsigned char ack2 = I2C_SDA & I2C_SCL;
	I2C_delay();
	unsigned char ack3 = I2C_SDA & I2C_SCL;
	I2C_delay();
	*ack = ack1 & ack2 & ack3;
	return;
}

void I2C_ack_check(unsigned char *ack) {
	I2C_SDA = 1;
	I2C_delay();
	*ack = !I2C_SDA;
	return;
}

void I2C_read(unsigned char *data) {
	unsigned char dat = 0x00;
#if defined EXPERIMENTAL && EXPERIMENTAL = 1
	for (unsigned char i = 0; i < 8; i++) {
		I2C_SCL = 1;
		I2C_delay();
		dat |= I2C_SDA << (7 - i);
		I2C_SCL = 0;
		I2C_delay();
	}
#else
	unsigned char i;
	for (i = 0; i < 8; i++) {
		I2C_SCL = 1;
		I2C_delay();
		dat = (dat << 1) | I2C_SDA;
		I2C_SCL = 0;
		I2C_delay();
	}
#endif
	*data = dat;
	return;
}

void I2C_write(unsigned char data) {
#if defined EXPERIMENTAL && EXPERIMENTAL = 1
	for (unsigned char i = 0; i < 8; i++) {
		I2C_SDA = (data >> (7 - i)) & 0x01;
		I2C_clock();
	}
#else
	unsigned char data_bit;
	unsigned char i;
	for (i = 0; i < 8; i++) {
		data_bit = data & 0x80;
		if (data_bit == 0x80)
			I2C_SDA = 1;
		else
			I2C_SDA = 0;

		unsigned char j;
		for (j = 0; j < 10; j++);	//Pequeña espera
		I2C_SCL = 1;
		for (j = 0; j < 10; j++);	//Pequeña espera
		I2C_SCL = 0;
		data = data << 1;
	}
#endif
	I2C_SDA = 1;
	I2C_delay();
	I2C_SCL = 1;
	I2C_delay();
#if defined EXPERIMENTAL && EXPERIMENTAL = 1
#else
	data_bit = I2C_SDA;
#endif
	I2C_SCL = 0;
	I2C_delay();
	return;
}

char I2C_readDataFromID(unsigned char id, unsigned char addr, unsigned char *data) {
	unsigned char ack;
	I2C_available(&ack);
	if (ack == 0) {
		return I2C_BUSY;
	}

	I2C_start();

	I2C_write(id);
	I2C_ack_check(&ack);
	if (ack == 0) {
		I2C_stop();
		return I2C_NO_ACK_RECIEVED;
	}

	I2C_write(addr);
	I2C_ack_check(&ack);
	if (ack == 0) {
		I2C_stop();
		return I2C_NO_ACK_RECIEVED;
	}

	I2C_start();

	I2C_write(id+1); // read mode
	I2C_ack_check(&ack);
	if (ack == 0) {
		I2C_stop();
		return I2C_NO_ACK_RECIEVED;
	}

	I2C_read(data);
	I2C_ack();

	I2C_stop();

	return I2C_OK;
}

char I2C_writeDataToID(unsigned char id, unsigned char addr, unsigned char data) {
	unsigned char ack;
	I2C_available(&ack);
	if (ack == 0) {
		return I2C_BUSY;
	}

	I2C_start();

	I2C_write(id);
	I2C_ack_check(&ack);
	if (ack == 0) {
		I2C_stop();
		return I2C_NO_ACK_RECIEVED;
	}

	I2C_write(addr);
	I2C_ack_check(&ack);
	if (ack == 0) {
		I2C_stop();
		return I2C_NO_ACK_RECIEVED;
	}

	I2C_write(data);
	I2C_ack_check(&ack);
	if (ack == 0) {
		I2C_stop();
		return I2C_NO_ACK_RECIEVED;
	}

	I2C_stop();

	return I2C_OK;
}

char I2C_readDataFromID_v(unsigned char id, unsigned char addr, unsigned char *data, unsigned char size) {
	unsigned char ack;
	I2C_available(&ack);
	if (ack == 0) {
		return I2C_BUY;
	}

	I2C_start();

	I2C_write(id);
	I2C_ack_check(&ack);
	if (ack == 0) {
		I2C_stop();
		return I2C_NO_ACK_RECIEVED;
	}

	I2C_write(addr);
	I2C_ack_check(&ack);
	if (ack == 0) {
		I2C_stop();
		return I2C_NO_ACK_RECIEVED;
	}

	I2C_start();

	I2C_write(id+1); // read mode
	I2C_ack_check(&ack);
	if (ack == 0) {
		I2C_stop();
		return I2C_NO_ACK_RECIEVED;
	}

	for (unsigned char i = 0; i < size; i++) {
		I2C_read(&(data[i]));
		if (i + 1 != size) I2C_ack():I2C_noack;
	}

	I2C_stop();

	return I2C_OK;
}

char I2C_writeDataToID_v(unsigned char id, unsigned char addr, unsigned char *data, unsigned char size) {
	unsigned char ack;
	I2C_available(&ack);
	if (ack == 0) {
		return I2C_BUSY;
	}

	I2C_start();

	I2C_write(id);
	I2C_ack_check(&ack);
	if (ack == 0) {
		I2C_stop();
		return I2C_NO_ACK_RECIEVED;
	}

	I2C_write(addr);
	I2C_ack_check(&ack);
	if (ack == 0) {
		I2C_stop();
		return I2C_NO_ACK_RECIEVED;
	}

	for (unsigned char i = 0; i < size; i++) {
		I2C_write(data[i]);
		I2C_ack_check(&ack);
		if (ack == 0) {
			I2C_stop();
			return I2C_NO_ACK_RECIEVED;
		}
	}

	I2C_stop();

	return I2C_OK;
}

int I2C_ping(unsigned char addr, unsigned char *response) {
	*response = 0;

	unsigned char ack;
	I2C_available(&ack);
	if (ack == 0) {
		return I2C_BUSY;
	}

	I2C_start();

	I2C_write(id);
	I2C_ack_check(&ack);
	if (ack == 0) {
		I2C_stop();
		return I2C_NO_ACK_RECIEVED;
	}

	I2C_stop();

	*response = 1;

	return I2C_OK;
}

int I2C_scan_network(unsigned char *list, unsigned char *size) {

}