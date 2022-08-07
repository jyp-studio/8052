#include <regx51.h> // include header file for 8051
#define TH0_init 0x06 //TH0_init= 256 - 250
#define TL0_init 0x06 //TL0_init = 256 - 250
#define Timer0_int_exe_time 2000 //the parameter can be changed

void delay(unsigned int t);
int counter;

void main( )
{
	TMOD = 0x02; //set timer0 to mode 2(8-bits and auto-reload)
	IP = 0x02; //timer0 interrupt has high priority
	IE = 0x82; //enable timer0 interrupt
	P2 = 0xff;
	TL0 = TL0_init; TH0 = TH0_init; //set TH0 & TL0
	TCON = 0x10; //enable timer0
	
	while (1)
	{
		delay(41679); // adjust this parameter to match timer
		P2_0 = ~P2_0; //inverse P2.0
	}
}

void timer0_interrupt(void) interrupt 1 // ‘interrupt 1’ is int vector of INT0
{
	counter++;
	if (counter == Timer0_int_exe_time) //250clock cycle * 2000 = 0.5 second
	{
		P2_1 = ~P2_1; //inverse P2.1
		counter = 0;
	}
}

//delay function
void delay(unsigned int t)
{
	for (; t>0; t--);
}