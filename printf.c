#include <stdarg.h>

extern void putchar(char c);
extern int  puts(char *s);

void itoa(int num, char *str, int base)
{
	int i, sign = num;
	
	char tmp[16];
	
	const char *digits = "0123456789ABCDEF";
	
	if (sign < 0) {
		num = -num;
	}
	
	i = 0;
	
	if (num == 0) {
		tmp[i++] = '0';
	} else {
		while (num != 0) {
			tmp[i++] = digits[num % base];
			num /= base;
		}
	}
	
	if (sign < 0) {
		tmp[i++] = '-';
	}
	
	while (i--) {
		*str++ = tmp[i];
	}
	
	*str = '\0';
}

void printf(const char *fmt, ...)
{
	va_list args;
	char buf[16];
	
	int num, base;
	
	va_start(args, fmt);
	
	for (; *fmt; fmt++) {
		if (*fmt != '%') {
			putchar(*fmt);
			continue;
		}
		
		fmt++;
		base = 10;
		
		switch (*fmt) {
		case 'c':
			putchar((unsigned char)va_arg(args, int));
			continue;
		case 's':
			puts(va_arg(args, char *));
			continue;
		case 'o':
			base = 8;
			break;
		case 'd':
		case 'i':
			break;
		case 'x':
		case 'X':
			base = 16;
			break;
		default:
			continue;
		}
		
		num = va_arg(args, int);
		itoa(num, buf, base);
		puts(buf);
	}
	va_end(args);
}
