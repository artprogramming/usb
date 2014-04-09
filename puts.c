extern void putchar(char c);

int puts(char *s)
{
	int i = 0;
	while (*s) {
		putchar(*s++);
		i++;
	}
	//putchar('\n');
	return i+1;
}
