#ifndef _ACCELEROMETER_H_
#define _ACCELEROMETER_H_

void startAccelerometer();

int checkDataAvailable();

signed int readX();
signed int readY();
signed int readZ();

#endif // _ACCELEROMETER_H_