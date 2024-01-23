import serial
import mouse

def to_signed_char(byte_val):
    v = int(byte_val)
    if byte_val > 127:
        v -= 256
    return v

def adjust(val):
    if abs(val) < 20:
        return val / 4
    ret = (abs(val) - 20)**2 / 20 + 5
    if val > 0:
        return ret
    return -ret

ser = serial.Serial("/dev/ttyACM0")
ser.reset_input_buffer()

while ser.is_open:
    s = ser.read(2)
    x_val = to_signed_char(s[0])
    y_val = to_signed_char(s[1])

    if abs(x_val) < 6:
        x_val = 0
    else:
        x_val = adjust(x_val)
    if abs(y_val) < 6:
        y_val = 0
    else:
        y_val = adjust(y_val)
    mouse.move(x_val, y_val, absolute=False, duration=0.002)
