#include<regx51.h>

void init_uart(); 
char recivevalue;
char correct[] = "correct";
char wrong[] = "wrong";
int counter = 0;
int i = 0;
int k = 0;

void main( ){
	init_uart();
	while(1);
}

void init_uart(void) {
	SCON=0x50;
	TMOD=0x20;
	TH1=0xe6;
	TR1=1;
	IE=0x90;
}

void UART_Isr(void) interrupt 4 {
	while(RI == 0);
	
	RI=0;
	recivevalue=SBUF;
	P2 = recivevalue;
	
	if(recivevalue == 'E') {
		counter ++;
	}
	if(recivevalue == '9' && counter == 1) {
		counter ++;
	}
	if(recivevalue == '4' && counter == 2) {
		counter ++;
	}
	if(recivevalue == '0' && counter == 3) {
		counter ++;
	}
	if(recivevalue == '8' && counter == 4) {
		counter ++;
	}
	if(recivevalue == '6' && counter == 5) {
		counter ++;
	}
	if(recivevalue == '0' && counter == 6) {
		counter ++;
	}
	if(recivevalue == '7' && counter == 7) {
		counter ++;
	}
	if(recivevalue == '6' && counter == 8) {
		counter ++;
	}
	if(recivevalue == 0x0D) {
		SBUF = 0x0A;
		while(TI==0);
		TI=0;
		
		if (counter == 9) {
			for(i = 0; i < sizeof(correct); i++) {
				SBUF=correct[i];
				while(TI==0);
				TI=0;
			}
		}
		else {
			for(k = 0; k < sizeof(wrong); k++) {
				SBUF=wrong[k];
				while(TI==0);
				TI=0;
			}
		}
		counter = 0;
	}
}