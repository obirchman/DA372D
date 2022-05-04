# DA372D
Xbee project with three Xbee devices. One coordinator and two end points with each end point connected to an atmega328p.

Requirements
Software on PC for interacting with end nodes.
A coordinator Xbee that communicates with two end nodes.
Two end node Xbee that are connected to an atmega328p each.
Two atmega328p that communicates over serial.

Our implementation
We are using python digi xbee module to create the software on the PC. Might be changed to Qt serialport implementation.
Then we are building a simple gui in Qt creator.
For one atmega328p we have a magnet contact and some resistors connected to the ADC so we can detect open, closed and broken circuit.
The other atmege328p has a simple buzzer that goes off if it's open as well as two LEDs to indicate whether it is closed or broken.
