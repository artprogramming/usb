CC	= sdcc

OBJS	= main.rel led.rel serial.rel putchar.rel puts.rel printf.rel \
	  d12.rel delay.rel usb.rel

all:usb.bin

usb.bin:usb.hex
	objcopy -I ihex -O binary usb.hex usb.bin

usb.hex:usb.ihx
	packihx usb.ihx > usb.hex

usb.ihx:$(OBJS)
	$(CC) -o usb.ihx $(OBJS)

%.rel:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY:clean
clean:
	-rm -f *.asm *.lst *.rel *.rst *.sym
	-rm -f *.bin *.hex *.ihx *.lk *.map *.mem

# Dependencies
main.rel:printf.h
led.rel:led.c led.h
serial:serial.c configs.h
putchar.rel:putchar.c
puts.rel:puts.c
printf.rel:printf.c
d12.rel:d12.c d12.h
delay.rel:delay.c
usb.rel:usb.c d12.h printf.h delay.h usb.h
