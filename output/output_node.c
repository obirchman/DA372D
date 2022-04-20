/*  Notes
    First byte 0x73, Two bytes = length(excluding last byte), One byte = command ID, Four bytes = address.
    9th byte - second last byte = data
*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#ifndef AVR_ATmega328P
#define AVR_ATmega328P
#endif
#define BAUD 9600
#define FIRST_DATA_BYTE 9
#define DATA_LEN (length - FIRST_DATA_BYTE)
#define REMAINING_BYTES (length+1)
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <util/setbaud.h>


void USART_init(){    //asynchronous, no parity, 1-stop bit, 8-bit frame
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    UCSR0B = _BV(UCSZ01) | _BV(UCSZ00);
    UCSR0B = _BV(RXEN0);
}
uint8_t USART_receive(){ // receive a single byte, polling.
    loop_until_bit_is_set(UCSR0A,RXC0);
    return UDR0;
}

void USART_send(uint8_t msg){   // send a single byte, polling.
    loop_until_bit_is_set(UCSR0A,UDRE0);
    UDR0 = msg;
}

void wait_for_delimiter(){  // The protocol starts every frame with 7E so ignore everything before that.
	while(!(USART_receive() == 0x7E));
}
void czech_checksum(){

}
uint8_t startReceive(){

    uint16_t length = USART_receive() << 8; // Receive two bytes that represents remaining bytes excluding checksum(1byte)
     length += USART_receive();             // We receive these separately so we can allocate a properly sized array for rest.

    uint8_t current_byte = 0;
    uint8_t frame[REMAINING_BYTES];
	uint8_t checksum = 0;

    while(current_byte < REMAINING_BYTES){  //receive all the bytes remaining in the frame
        frame[current_byte] = USART_receive();
		checksum += frame[current_byte++];  //Add values for checksum verification
    }
	if(checksum != 0xFF){   //The sum of all the bytes excluding delimiter and length should have lowest 8-bit = 0xFF
		return -1;
	}
    uint8_t data[DATA_LEN];
    for(int i = FIRST_DATA_BYTE; i < length; i++){ //iterates over all data bytes and adds them to a new array.
		data[i-FIRST_DATA_BYTE] = frame[i];
	}
	return 0;
} 

int main(void){
    USART_init();
    while(1){
        wait_for_delimiter();
        uint8_t ret = startReceive();   //TODO Suggestions 1. Pass a pointer that is initialized inside.
		send_verify(ret);               //                 2. Do length check before this function call and send an initalized pointer.
		/*                                                 
		    Light a led if the data received = command
		*/
	}
    return 0;
}