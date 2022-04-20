#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#ifndef AVR_ATmega328P
#define AVR_ATmega328P
#endif
#define BAUD 9600
#define DATA_START (length - 9)
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

void wait_for_delimiter(){
	while(!(USART_receive() == 0x7E));
}
void czech_checksum(){

}
uint8_t startReceive(){

    uint16_t length = USART_receive() << 8;
     length += USART_receive();

    uint8_t iterator = 0;
    uint8_t payload[length+1];
	uint8_t checksum = 0;

	//receive it all
    while(iterator < length + 1){
        payload[iterator] = USART_receive();
		checksum += payload[iterator++];
    }
	if(checksum != 0xFF){
		return -1;
	}
	// read data into new array
    uint8_t data[DATA_START];
    for(int i = 9; i < length; i++){
		data[i-9] = payload[i];
	}
	return 0;
} 

int main(void){
    USART_init();
    while(1){
        wait_for_delimiter();
        uint8_t ret = startReceive();
		send_verify(ret);
		/*
		Do programmy things
		*/
	}
    return 0;
}