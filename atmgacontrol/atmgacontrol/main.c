#define F_CPU 16000000UL
#include <avr/io.h>
#include "UART/UART.h"
#include <stdint.h>
#include <avr/interrupt.h>

#define BUTTON_UP     PD7  // D7 en Arduino (PD7 en ATmega328P)
#define BUTTON_DOWN   PB0  // D8 en Arduino (PB0 en ATmega328P)
#define BUTTON_LEFT   PB1  // D9 en Arduino (PB1 en ATmega328P)
#define BUTTON_RIGHT  PB2  // D10 en Arduino (PB2 en ATmega328P)
#define BUTTON_A      PB3  // D11 en Arduino (PB3 en ATmega328P)
#define BUTTON_B      PB4  // D12 en Arduino (PB4 en ATmega328P)

volatile uint8_t debounce_counter = 0;  // Contador para el antirrebote
volatile uint8_t button_up_state = 0;    // Estado del bot�n UP
volatile uint8_t button_down_state = 0;  // Estado del bot�n DOWN
volatile uint8_t button_left_state = 0;  // Estado del bot�n LEFT
volatile uint8_t button_right_state = 0; // Estado del bot�n RIGHT
volatile uint8_t button_a_state = 0;     // Estado del bot�n A
volatile uint8_t button_b_state = 0;     // Estado del bot�n B
volatile char receivedChar = '\0';        // Car�cter recibido por UART

void init_buttons() {
	// Configurar los pines como entradas con pull-up
	DDRD &= ~(1 << BUTTON_UP); // Configurar PD7 como entrada
	PORTD |= (1 << BUTTON_UP); // Activar pull-up interno en PD7

	DDRB &= ~((1 << BUTTON_DOWN) | (1 << BUTTON_LEFT) | (1 << BUTTON_RIGHT) | (1 << BUTTON_A) | (1 << BUTTON_B)); // Configurar PB0-PB4 como entradas
	PORTB |= (1 << BUTTON_DOWN) | (1 << BUTTON_LEFT) | (1 << BUTTON_RIGHT) | (1 << BUTTON_A) | (1 << BUTTON_B);  // Activar pull-up interno
}

void check_buttons() {
	// Verificar el antirrebote
	if (debounce_counter > 0) {
		debounce_counter++;
	}

	// Esperar 20 ms para estabilizaci�n
	if (debounce_counter >= 20) {
		debounce_counter = 0;

		// Verificar el estado del bot�n UP
		if (!(PIND & (1 << BUTTON_UP)) && (button_up_state == 0)) {
			writeUART('U'); // Enviar U por UART
			button_up_state = 1; // Actualiza el estado del bot�n UP
			} else if (PIND & (1 << BUTTON_UP)) {
			button_up_state = 0; // Restablece el estado si se suelta el bot�n UP
		}

		// Verificar el estado del bot�n DOWN
		if (!(PINB & (1 << BUTTON_DOWN)) && (button_down_state == 0)) {
			writeUART('D'); // Enviar D por UART
			button_down_state = 1; // Actualiza el estado del bot�n DOWN
			} else if (PINB & (1 << BUTTON_DOWN)) {
			button_down_state = 0; // Restablece el estado si se suelta el bot�n DOWN
		}

		// Verificar el estado del bot�n LEFT
		if (!(PINB & (1 << BUTTON_LEFT)) && (button_left_state == 0)) {
			writeUART('L'); // Enviar L por UART
			button_left_state = 1; // Actualiza el estado del bot�n LEFT
			} else if (PINB & (1 << BUTTON_LEFT)) {
			button_left_state = 0; // Restablece el estado si se suelta el bot�n LEFT
		}

		// Verificar el estado del bot�n RIGHT
		if (!(PINB & (1 << BUTTON_RIGHT)) && (button_right_state == 0)) {
			writeUART('R'); // Enviar R por UART
			button_right_state = 1; // Actualiza el estado del bot�n RIGHT
			} else if (PINB & (1 << BUTTON_RIGHT)) {
			button_right_state = 0; // Restablece el estado si se suelta el bot�n RIGHT
		}

		// Verificar el estado del bot�n A
		if (!(PINB & (1 << BUTTON_A)) && (button_a_state == 0)) {
			writeUART('A'); // Enviar A por UART
			button_a_state = 1; // Actualiza el estado del bot�n A
			} else if (PINB & (1 << BUTTON_A)) {
			button_a_state = 0; // Restablece el estado si se suelta el bot�n A
		}

		// Verificar el estado del bot�n B
		if (!(PINB & (1 << BUTTON_B)) && (button_b_state == 0)) {
			writeUART('B'); // Enviar B por UART
			button_b_state = 1; // Actualiza el estado del bot�n B
			} else if (PINB & (1 << BUTTON_B)) {
			button_b_state = 0; // Restablece el estado si se suelta el bot�n B
		}
	}
}

// Inicializa Timer1
void initTimer1(void) {
	// Configura Timer1 para generar una interrupci�n cada 1 ms
	TCCR1A = 0; // Modo normal
	TCCR1B = (1 << WGM12) | (1 << CS11); // Modo CTC y prescaler de 8
	OCR1A = 1999; // Valor para 1 ms (16MHz / 8 / 2000)
	TIMSK1 = (1 << OCIE1A); // Habilitar la interrupci�n de comparaci�n
}

int main(void) {
	initUART9600();  // Inicializar UART a 9600 baud
	init_buttons();  // Inicializar los botones
	initTimer1();    // Inicializar Timer1
	sei();           // Habilitar interrupciones globales

	while (1) {
		check_buttons(); // Llamar a la funci�n para verificar los botones
	}
	return 0;
}

// ISR de recepci�n UART
ISR(USART_RX_vect) {
	receivedChar = UDR0;  // Almacena el car�cter recibido
	while (!(UCSR0A & (1 << UDRE0)));  // Mientras haya caracteres en el buffer
}

// ISR del Timer1
ISR(TIMER1_COMPA_vect) {
	debounce_counter++; // Incrementar el contador de antirrebote
}
