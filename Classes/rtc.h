#include "i2c.h"

//RTC
#define DS1307_ID 0xD0 /* RTC ID */

#define RTC_read(addr) I2C_readFromID(DS1307_ID, addr)

#define RTC_write(s, m, h) do { I2C_writeToID(DS1307_ID, 0x00, s); I2C_writeToID(DS1307_ID, 0x01, m); I2C_writeToID(DS1307_ID, 0x02, h); } while (0)
