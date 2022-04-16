import sys
import time
from digi.xbee.devices import Raw802Device

# In order
# DELIMITER = '7E'
# LENGTH TWO BYTES (DATA BYTES + ADDRESS + NO_ACK = DATA + 8 + 1 = DATA + 9)
# TYPE = '00'
MAC1 = '0013A20041C72100'
MAC2 = '0013A20041C72075'
# OPTIONS = '01'  # DISABLE ACK
# CHECKSUM ONE BYTE

coordinator = Raw802Device("COM1", 9600) # The big boi bee that controls tbe other bees


# After sending to a bee, bee must reply and this checks that the correct bee replied and
# prints what it replied.
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
    net = coordinator.get_network()
    net.start_discovery_process()   # Starts looking for bees
    while net.is_discovery_running():   # Locks until looking it is done looking for bees
        time.sleep(0.5)
    node1 = net.get_device_by_64(MAC1)  # Get the specific bee
    if node1 is None:   # Make sure it is actually there
        print("Device with 64-bit addr: %s not found" % str(MAC1))
    node2 = net.get_device_by_64(MAC2)  # Get second bee
    if node2 is None: 
        print("Device with 64-bit addr: %s not found" % str(MAC2))
    return node1,node2  # Return the bees
def main():
    e1,e2 = get_nodes()
    msg = 'A'
    coordinator.send_data_async(e1,msg) # Sends msg to bee e1
    verify_reply(e1)
    
    msg = 'B'
    coordinator.send_data_async(e2,msg)  # Sends msg to bee e2
    verify_reply(e1)
    

if __name__ == '__main__':
    sys.exit(main())