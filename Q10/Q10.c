#include <regx51.h>

void print_msg(char *);
void write(char, int);
void delay(unsigned int);
void main() 
{
	write(0x38, 0);
	write(0x0F, 0);
	write(0x06, 0);
	write(0x01, 0);
	write(0x80, 0);
	print_msg("Hello, world!");
	while (1);
}
void print_msg(char *msg)
{
	for (;*msg != '\0'; msg++)
		write(*msg, 1);
}
void write(char cmd, int rs_value)
{
	P1 = cmd;
	P3_0 = rs_value;
	P3_1 = 1;
	delay(100);
	P3_1 = 0;
}
void delay(unsigned int i)
{
	while (i--);
}