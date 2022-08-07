#include<regx51.h>
void delay (int);
void turn ();
void reverse_interrupt();
void half_interrupt();

code char half_phase[2][8] = {{0x01,0x0c,0x02,0x06,0x04,0x03,0x08,0x09},
															{0x09,0x08,0x03,0x04,0x06,0x02,0x0c,0x01}};
code int d_time[2] = {1000, 500};
int dir , delay_time;

void main () {
	IE = 0x85;
	TCON = 0x05;
	delay_time = 1;
	dir = 1; 
	while(1) {
		turn();
	}
}

void reverse_interrupt(void) interrupt 0 {
	delay(100);
	dir ^= 1;
}

void half_interrupt(void) interrupt 2 {
	delay(100);
	delay_time ^= 1;
}

void turn() {
	int x;
	for(x = 0; x < 8; x++) {
		P1 = half_phase[dir][x];
		delay(d_time[delay_time]);
	}
}

void delay(int t) {
	while(t--);
}
