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
int main(void){
	USART_init();
	DDRB = _BV(PORTB0) | _BV(PORTB1);   // enable outputs
	while(1){
		uint8_t ch = USART_receive();   // get "command"
		if(ch == 'A'){
			PORTB ^= _BV(PORTB0);	// Toggle first LED
		}
		else if(ch == 'B'){
			PORTB ^= _BV(PORTB1);   // Toggle second LED
		}
	}
	return 0;
}
