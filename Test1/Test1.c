#include<regx51.h>

void delay(unsigned int);
void print_msg(char *);
void write(char, int);
void init(); 
void timer0_interrupt();
void send_msg(char *);
int check(char *);
void int_0();
void int_1();
void create_self_def(int, int[]);
void print_create(int, int);


//UART vairables
bit receive_full = 1;
bit transmit_flag = 1;
char index = 0;
char rxbuf[9];
code char *start = "E94086076";
code char *LCD_start = "\r\nLCD on\r\n";
code char *close = "ShutdownN";
code char *LCD_close = "\r\nLCD off\r\n";
code char *level_0 = "bright000";
code char *LCD_lv0 = "\r\nscreen brightness dims\r\n";
code char *level_1 = "bright001";
code char *LCD_lv1 = "\r\nscreen brightness brighter\r\n";

int i = 0;

//LCD variables
int LCD_light_level;
int LCD_run_level = 1000;
int run_level = 0;
int LCD_PWM_counter;
int LCD_run_counter;
bit LCD_print = 0; //1: move LCD ;0: unmove LCD
bit LCD_PWM = 0; //1: turn on CONTR; 0: turn off CONTR
bit dir = 0;
bit screen_switch = 1;

//CGRAM variables
int smile[8] = {0x00, 0x0A, 0x0A, 0x00, 0x11, 0x0E, 0x00, 0x00};
int heart[8] = {0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00};

//main function
void main() {
	init();
	
	//close LCD
	P1 = 0xFF;
	P2 = 0xFF;
	
	//check UART msg
	while(1) {
		if (receive_full){
			receive_full = 0;
			
			//start up LCD
			if (check(start)) {
				send_msg(LCD_start);
				P1 = 0;
				delay(100000);
				init();
				LCD_PWM = 1;
				LCD_light_level = 2;
				
				create_self_def(0x40, smile);
				create_self_def(0x48, heart);
				print_create(0x80, 0x00);
				print_create(0x8F, 0x01);
				print_msg("                        E94086076");
			}
			//close LCD
			else if (check(close)) {
				send_msg(LCD_close);
				LCD_print = 0;
				LCD_PWM = 0;
				P1 = 0xFF;
				P2 = 0xFF;
			}
			//switch LCD PWM level
			else if (check(level_0)) {
				LCD_light_level--;
				send_msg(LCD_lv0);
			}
			else if (check(level_1)) {
				LCD_light_level++;
				send_msg(LCD_lv1);
			}
		}
	};
}

//init
void init() {
	//initial LCD
	write(0x38, 0);
	write(0x0F, 0);
	write(0x06, 0);
	write(0x01, 0);
	write(0x80, 0);
	
	//init timer
	IE = 0x97;
	IP = 0x02;
	TMOD = 0x22;
	TCON = 0x50;
	
	//init UART
	SCON = 0x50;
	TH1 = 0xE6;
	TR1 = 1;
}

//write to LCD
void write(char cmd, int rs_value) {
	P2 = cmd;
	P1_6 = rs_value;
	P1_7 = 1;
	delay(1000);
	P1_7 = 0;
}

//LCD print
void print_msg(char *msg) {
	for (;*msg != '\0'; msg++)
		write(*msg, 1);
}

//timer
void timer0_interrupt(void) interrupt 1 {
	//let LCD move right continuously
	if (LCD_print == 1) {
		LCD_run_counter++;
		if (LCD_run_counter >= LCD_run_level) {
			dir ? write(0x1C, 0) : write(0x18, 0);
			LCD_run_counter = 0;
		}
	}
	
	//PWM let LCD light correctly
	if (LCD_PWM == 1) {
		P1_3 = 0;
		if (!LCD_PWM_counter) {
			P1_3 = 1;
			LCD_PWM_counter = LCD_light_level;
		}
		LCD_PWM_counter--;
	}
}

//UART_interrupt
void UART_Isr(void) interrupt 4 {
	if(RI){
		RI = 0;
		rxbuf[index ++] = SBUF;
		index %= 9;
		if(!index)
			receive_full = 1;
	}
	else {
		TI = 0;
		transmit_flag = 1;
	}
}

//send msg back to computer
void send_msg(char *c){
	for(; *c!='\0'; c++){
		while(!transmit_flag);
		transmit_flag = 0;
		SBUF = *c;
	}
}

//8051 check msg
int check(char *c){
	for(i=0; i<9; i++) {
		if(*(c + i) != *(rxbuf + i))
			return 0;
	}
	return 1;
}

//INT0
void int_0(void) interrupt 0 {
	delay(20000);
	//stop moving screen
	LCD_print = 0;
	screen_switch ^= 1;
	screen_switch ? write(0x0F, 0) : write(0x08, 0);
	write(0x01, 0);
	print_msg("Best(4)                                 Worst(7)");
}

//INT1
void int_1(void) interrupt 2 {
	delay(20000);
	if (run_level == 0) {
		LCD_print = 0;
		write(0x01, 0);
		print_msg("Thanks!");
		run_level ++;
	}
	else if (run_level == 1) {
		dir = 0;
		LCD_print = 1;
		LCD_run_level = 1000;
		run_level ++;
	}
	else {
		LCD_run_level = 500;
		run_level = 0;
	}
}

//create words
void create_self_def(int address, int message[]) {
	int i;
	write(address, 0);
	for (i = 0; i < 8; i++)
		write(message[i], 1);
}

//print created words
void print_create(int location, int msg_address) {
	write(location, 0); write(msg_address, 1);
}

//delay
void delay(unsigned int t) {
	while(t--);
}