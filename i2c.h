#ifndef _I2C_H_
#define _I2C_H_

void i2c_enable();

void i2c_write_read(int write,
                    int read,
                    char slave_address,
                    char* buffer_write,
                    void (*callback)(char*));

#endif // _I2C_H_
