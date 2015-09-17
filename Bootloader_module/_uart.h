#include "MKL46Z4.h"                    // Device header
#include "stdio.h"
void Uart0_init(uint32_t  baud);
char uart0_getchar(void);
void uart0_putchar (char ch);
int uart0_getchar_present (void);
int fputc(int c, FILE *f);
int fgetc(FILE *f);
