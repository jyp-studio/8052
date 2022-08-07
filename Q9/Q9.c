#include <regx51.h>

void main( ) {
	while(1) {
		if(P1 >256 / 2) //if P1 greater than 256/2
			P2 = 0; //LED on
		else
			P2 = 255; //LED off
	}
}