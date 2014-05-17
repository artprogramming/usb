#include <at89x52.h>

void putchar(char c)
{
	SBUF = c;
	while (!TI);
	TI = 0;

	if (c == '\n') {
		putchar('\r');
	}
}
