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
#define FIRST_DATA_BYTE 5
#define DATA_LEN (length - FIRST_DATA_BYTE)
#define REMAINING_BYTES (length+1)
#define FRAME_BYTES 12+data_len
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <util/setbaud.h>
uint8_t CONTROLLER_ADDRESS[] = {0x00,0x13,0xA2,0x00,0x41,0xB7,0x75,0x70};


void USART_init(){    //asynchronous, no parity, 1-stop bit, 8-bit frame
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    UCSR0B = _BV(UCSZ01) | _BV(UCSZ00);
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);
}

uint8_t USART_receive(){ // receive a single byte, polling.
    loop_until_bit_is_set(UCSR0A,RXC0);
    return UDR0;
}

void USART_send(uint8_t msg){   // send a single byte, polling.
    // 7E 00 06 81 00 00 21 00 41 1C
    loop_until_bit_is_set(UCSR0A,UDRE0);
    UDR0 = msg;
}

void wait_for_delimiter(){  // The protocol starts every frame with 7E so ignore everything before that.
	while(!(USART_receive() == 0x7E));
    //PORTB = _BV(PINB0);
}

void czech_checksum(){

}

uint8_t get_length(){
    uint16_t length = USART_receive() << 8; // Receive two bytes that represents remaining bytes excluding checksum(1byte)
    length += USART_receive();             // We receive these separately so we can allocate a properly sized array for rest.
    return length;
}

uint8_t getByte(int16_t data, uint8_t byte){    
    return (0xFF) & data >> 8*byte;
}
void sendFrame(uint8_t *data, uint8_t data_len){
    uint8_t checksum = 0x00 + 0x00 + 0x01;
    
    USART_send(0x7E);
    USART_send(getByte(11+data_len, 1));
    USART_send(getByte(11+data_len, 0));
    USART_send(0x00);
    USART_send(0x00);
    
    for(int i = 0; i < 8; i++){
        USART_send(CONTROLLER_ADDRESS[i]);
        checksum += CONTROLLER_ADDRESS[i];
    }

    USART_send(0x01);
    
    for(uint8_t i = 0; i < data_len; i++)
    {
        USART_send(data[i]);
        checksum += data[i];
    }
    
    USART_send(0xFF - checksum);
    
}

uint8_t startReceive(uint8_t *data, uint16_t length){

    //uint16_t length = USART_receive() << 8; // Receive two bytes that represents remaining bytes excluding checksum(1byte)
    //length += USART_receive();             // We receive these separately so we can allocate a properly sized array for rest.

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
    
    

    for(int i = FIRST_DATA_BYTE; i < length; i++){ //iterates over all data bytes and adds them to a new array.
		data[i-FIRST_DATA_BYTE] = frame[i];
        
	}
	
    return 1;
} 

void send_verify(uint8_t ret){
    sendFrame(&ret, 1);
}

int main(void){
    
    USART_init();
    DDRB = _BV(PINB0);
    uint8_t retur = 0;
    while(1){
        /*
        uint8_t ch = USART_receive();
        if(ch == 'A'){
            PORTB = _BV(PINB0);
        }
        */
        wait_for_delimiter();
        uint16_t length = get_length();
        
        uint8_t data[DATA_LEN];
        uint8_t ret = startReceive(data,length);
        if(data[0] == 'A'){
            PORTB = _BV(PINB0);
        } 
        if(retur){
            retur = 0;
        }
        if(!retur){
            retur = 1;
        }
                               // 7E 00 0C 00 00 00 13 A2 00 41 B7 75 70 01 01 6B 

        //uint8_t sendMe[] = {0x7E,0x00,0x0C,0x00,0x00,0x00,0x13,0xA2,0x00,0x41,0xB7,0x75,0x70,0x01,0x01,0x6A};
        //USART_send
		send_verify(retur);
	}

    return 0;
}