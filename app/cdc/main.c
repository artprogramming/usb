extern void usb_irq(void) __interrupt 0;
extern int  usb_init(void);
extern void cdc_init(void);
extern void serial_init(void);

void main(void)
{
	serial_init();
	cdc_init();
	usb_init();

	while (1);
}
