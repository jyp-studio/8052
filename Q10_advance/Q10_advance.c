#include <regx51.h>

void print_msg(char *);
void print_create(int, int);
void write(char, int);
void delay(unsigned int);
void create_self_def(int, int[]);

int smile[8] = {0x00, 0x0A, 0x0A, 0x00, 0x11, 0x0E, 0x00, 0x00};

void main() {
	write(0x38, 0);
	write(0x0F, 0);
	write(0x05, 0);
	write(0x01, 0);
	write(0x80, 0);
	print_msg("E94086076");
	
	create_self_def(0x40, smile);
	print_create(0xCF, 0x00);
	
	while (1);
}

void print_msg(char *msg) {
		for (;*msg != '\0'; msg++)
			write(*msg, 1);
}

void print_create(int location, int msg_address) {
	while(1) {
		write(location, 0); write(msg_address, 1);
		delay(8000);
	}
}

void create_self_def(int address, int message[]) {
	int i;
	write(address, 0);
	for (i = 0; i < 8; i++)
		write(message[i], 1);
}

void write(char cmd, int rs_value) {
	P1 = cmd;
	P3_0 = rs_value;
	P3_1 = 1;
	delay(100);
	P3_1 = 0;
}

void delay(unsigned int i) {
	while (i--);
}

