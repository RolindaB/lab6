/*
 * UART.h
 *
 * Created: 20/09/2024 10:54:06
 *  Author: asrol
 */ 


#ifndef UART_H_
#define UART_H_
#include <stdint.h> // Para uint8_t
#define F_CPU 16000000UL // Frecuencia del reloj del microcontrolador
#include <avr/io.h>
#include <stdint.h>

void initUART9600();
void writeUART(char Caracter);
void writeTextUART(char * Texto);


#endif /* UART_H_ */