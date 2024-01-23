#ifndef _I2C_H_
#define _I2C_H_

void i2c_enable();

void i2c_write_read(int write_n,
                    int read_n,
                    char slave_address,
                    char* buffer_write,
                    void (*callback)(char*));

#endif // _I2C_H_
