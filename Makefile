CC	= sdcc

SRCS	= hw/serial.c hw/led.c hw/key.c hw/d12.c \
	  core/delay.c core/putchar.c core/puts.c core/printf.c core/usb.c \
	  app/hid/desc_hid.c app/hid/hid.c app/hid/main.c app/hid/reportkey.c

OBJS	= $(SRCS:%.c=%.rel)

CFLAGS	= -I./include -I./include/usb -I./hw -I./core -I./app/hid

all:usb.ihx
usb.ihx:$(OBJS)
	$(CC) -o $@ $(OBJS)

%.rel:%.c
	$(CC) $(CFLAGS) -o $@ -c $<


.PHONY:clean
clean:
	-rm -f $(addsuffix .rel, $(basename $(OBJS)))
	-rm -f $(addsuffix .asm, $(basename $(OBJS)))
	-rm -f $(addsuffix .lst, $(basename $(OBJS)))
	-rm -f $(addsuffix .rst, $(basename $(OBJS)))
	-rm -f $(addsuffix .sym, $(basename $(OBJS)))
	-rm -f usb.ihx usb.map usb.lk usb.mem	


# Dependencies
hw/serial.rel:hw/serial.c include/configs.h
hw/led.rel:hw/led.c hw/led.h
hw/key.rel:hw/key.c core/delay.h
hw/d12.rel:hw/d12.c hw/d12.h
core/delay.rel:core/delay.c
core/putchar.rel:core/putchar.c
core/puts.rel:core/puts.c
core/printf.rel:core/printf.c
core/usb.rel:core/usb.c hw/d12.h include/printf.h core/delay.h \
	include/usb/ch9.h core/usb.h app/hid/hid.h include/types.h
app/hid/desc_hid.rel:app/hid/desc_hid.c include/usb/ch9.h app/hid/hid.h
app/hid/hid.rel:app/hid/hid.c include/usb/ch9.h app/hid/hid.h \
	hw/d12.h core/usb.h include/printf.h
app/hid/main.rel:app/hid/main.c include/printf.h hw/key.h core/delay.h
app/hid/reportkey.rel:app/hid/reportkey.c hw/d12.h hw/key.h include/usb/keycodes.h
