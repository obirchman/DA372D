#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif
#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <util/setbaud.h>

void USART_init(){	//asynchronous, no parity, 1-stop bit, 8-bit frame
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	UCSR0B = _BV(UCSZ01) | _BV(UCSZ00);
	UCSR0B = _BV(TXEN0);
}
uint8_t USART_receive(){ // receive a single byte, polling.
	loop_until_bit_is_set(UCSR0A,RXC0);
	return UDR0;
}
void USART_send(uint8_t msg){   // send a single byte, polling.
	loop_until_bit_is_set(UCSR0A,UDRE0);
	UDR0 = msg;
}
int main(void){
	USART_init();
    while(1){
        uint8_t msg = 'A';
		USART_send(msg);
        _delay_ms(1000);
        msg = 'B';
        USART_send(msg);
        _delay_ms(1000);
	}
	return 0;
}
