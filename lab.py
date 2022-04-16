import sys
import serial
import digi
# In order
DELIMITER = '7E'
# LENGTH TWO BYTES (DATA BYTES + ADDRESS + NO_ACK = DATA + 8 + 1 = DATA + 9)
TYPE = '00'
MAC1 = '0013A20041C72100'
MAC2 = '0013A20041C72075'
OPTIONS = '01'  # DISABLE ACK
# CHECKSUM ONE BYTE

def send_frame(data,receiver):
    data_len = (len(data) / 2 ) + 9 # Two digits = One byte and + 9 is because of static length stuff. See above
    data_hex = format(data_len)
    
    print(int(data_len[1],16))
    message_string = DELIMITER+TYPE+receiver+data+OPTIONS
    print(message_string)


def main():
    data = '01'
    send_frame(data,MAC1);
if __name__ == '__main__':
    sys.exit(main())