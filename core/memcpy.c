#include <stdio.h>

void *memcpy(void *dst, void *src, size_t count)
{
	char *d = dst;
	char *s = src;

	while (count--)
		*d++ = *s++;

	return dst;
}
