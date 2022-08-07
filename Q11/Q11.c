#include<regx51.h>
void delay (int);
void turn ();

code char one_phase[] = {0x01,0x02,0x04,0x08};
code char two_phase[] = {0x0c,0x06,0x03,0x09};
int dir , delay_time;

void main () {
	delay_time = 3000;
	dir = 1; 
	while(1) {
		turn();
	}
}

void turn() {
	int x;
	
	//one phase, turn right
	if(dir == 1)  { 
		for(x = 0; x < 4; x++) {
			P1 = one_phase[x];
			delay(delay_time);
		}
	}
	//two phase, turn left
	else {
		for(x = 0; x < 4; x++) {
			P1 = two_phase[x]; 
			delay(delay_time);
		}
	}
}

void delay(int t) {
	while(t--);
}
