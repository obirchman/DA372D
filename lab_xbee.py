import sys
import time
from digi.xbee.devices import Raw802Device
from digi.xbee.devices import ZigBeeDevice
from digi.xbee.models.address import XBee64BitAddress

# In order. THIS MIGHT BE INACCURATE.
# DELIMITER = '7E'
# LENGTH TWO BYTES (DATA BYTES + ADDRESS + NO_ACK = DATA + 8 + 1 = DATA + 9)
# TYPE = '00'
COORD_MAC = '0013A20041B77570'
MAC1 = '0013A20041C72100'
MAC2 = '0013A20041C72075'
# OPTIONS = '01'  # DISABLE ACK
# CHECKSUM ONE BYTE


PORT = 'COM16'
BAUD = 9600
coordinator = Raw802Device(PORT, BAUD) # The big boi bee that controls the other bees


# A bee should verify it's message somehow
def verify_reply(node):
    msg_object = coordinator.read_data()
    if(msg_object is node):
        print('Node 1 replied %s.' % msg_object.data)
        return True
    else:
        print('Not working as intended')
        return False


# Discover the hive and the bees
def get_nodes():
    coordinator.open()  # Opens a connection with the big boi bee
    net = coordinator.get_network() # Get network and start looking for bees.
    net.start_discovery_process()   
    while net.is_discovery_running():
        time.sleep(0.5)
    
    node1 = net.get_device_by_64(XBee64BitAddress(bytes.fromhex(MAC1)))  # Get the end bee
    if node1 is None:   # Make sure it is actually there
        print("Device with 64-bit addr: %s not found" % str(MAC1))
    node2 = net.get_device_by_64(XBee64BitAddress(bytes.fromhex(MAC2)))  # Get second end bee
    if node2 is None: 
        print("Device with 64-bit addr: %s not found" % str(MAC2))
    return node1  # Return the bees


# A placeholder for generating data to send
def get_data():
    return bytes(41)


#TODO add a receive function
def main():
    e1 = get_nodes()
    msg = 0
    while(1):
        coordinator.send_data_async(e1,get_data()) # Sends msg to bee e1
        time.sleep(1)
        if(msg == 5):
            msg = 0
    return 0
    
    
if __name__ == '__main__':
    sys.exit(main())