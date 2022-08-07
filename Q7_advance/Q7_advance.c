#include <regx51.h> // include header file for 8051
#define TH0_init 0x37
#define TL0_init 0x05
#define Timer0_int_exe_time 10 //the parameter can be changed

int counter = 0;

void main( )
{
	P3 = 0x00;
	TMOD = 0x09; //set timer0 to mode 1(16 bits timer)
	IP = 0x02;
	IE = 0x82; //enable timer1 $ INT1 interrupt
	P2 = 0xfe; //set P2.0 to low (light)
	TL0 = TL0_init; TH0 = TH0_init; //set TH1 & TL1
	TCON = 0x10; //enable timer0
	while(1);
}

void timer0_interrupt(void) interrupt 1 // ‘interrupt 1’ is int vector of INT0
{
	counter ++;
	if (counter == Timer0_int_exe_time) //250clock cycle * 2000 = 0.5 second
	{
		P2 = (P2 << 1) | (P2 >> 7);
		counter = 0;
		TL0 = TL0_init; 
		TH0 = TH0_init; //set TH0 & TL0
	}
}