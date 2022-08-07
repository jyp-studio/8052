#include <regx51.h>

/*
principle:
ex: 10000 - 1   = 1111
		10000 - 10  = 1110
		10000 - 100 = 1100
Therefore, if we want get N LED light, we should use 2^8 - 2^N to get the result.
*/

void main( ) {
	while(1) {
		int N = 0; // N is the numbers of LED should light.
		int n = 1; // n = 2^N
		N = P1 / 28;
			for(; N > 0; N--)
				n *= 2;
			P2 = 256 - n;
	}
}
