/*
 * atmgacontrol.c
 *
 * Created: 20/09/2024 10:52:48
 * Author : asrol
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include "UART/UART.h"
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#define BUTTON_UP     PD7  // D7 en Arduino (PD7 en ATmega328P)
#define BUTTON_DOWN   PB0  // D8 en Arduino (PB0 en ATmega328P)
#define BUTTON_LEFT   PB1  // D9 en Arduino (PB1 en ATmega328P)
#define BUTTON_RIGHT  PB2  // D10 en Arduino (PB2 en ATmega328P)
#define BUTTON_A      PB3  // D11 en Arduino (PB3 en ATmega328P)
#define BUTTON_B      PB4  // D12 en Arduino (PB4 en ATmega328P)
volatile char receivedChar = 0;    //Variable que almacena el valor del UART

void init_buttons() {
	// Configurar los pines como entradas con pull-up
	DDRD &= ~(1 << BUTTON_UP); // Configurar PD7 (D7) como entrada
	PORTD |= (1 << BUTTON_UP); // Activar pull-up interno en PD7 (D7)
	
	DDRB &= ~((1 << BUTTON_DOWN) | (1 << BUTTON_LEFT) |(1 << BUTTON_RIGHT) | (1 << BUTTON_A) | (1 << BUTTON_B)); // Configurar PB0-PB4 (D8-D12) como entradas
	PORTB |= (1 << BUTTON_DOWN) | (1 << BUTTON_LEFT) |(1 << BUTTON_RIGHT) | (1 << BUTTON_A) | (1 << BUTTON_B);  // Activar pull-up interno en PB0-PB4 (D8-D12)
}


void check_buttons() {
	// Comprobar si algún botón está presionado
	if (!(PIND & (1 << BUTTON_UP))) {
		_delay_ms(200);
		writeUART('U');
	}
	if (!(PINB & (1 << BUTTON_DOWN))) {
		_delay_ms(200);
		writeUART('D');
	}
	if (!(PINB & (1 << BUTTON_LEFT))) {
		_delay_ms(200);
		writeUART('L');
	}
	if (!(PINB & (1 << BUTTON_RIGHT))) {
		_delay_ms(200);
		writeUART('R');
	}
	if (!(PINB & (1 << BUTTON_A))) {
		writeUART('A');
	}
	if (!(PINB & (1 << BUTTON_B))) {
		_delay_ms(200);
		writeUART('B');
	}
	
}


int main(void) {
	 initUART9600(); // Inicializar UART a 9600 baud
	init_buttons();   // Inicializar los botones
	//sei();
	while (1) {
		check_buttons();  // Revisar el estado de los botones
		_delay_ms(100);
		
	}
	return 0;
}

ISR(USART_RX_vect)
{
	receivedChar = UDR0; // Almacena el carácter recibido
	
	while(!(UCSR0A & (1<<UDRE0)));    //Mientras haya caracteres
	
}