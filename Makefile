CC	= sdcc

APP	= APP_HID

SRCS	= hw/serial.c hw/led.c hw/key.c hw/d12.c \
	  core/delay.c core/putchar.c core/puts.c core/printf.c core/usb.c \

ifeq ($(APP), APP_HID)
SRCS	+= app/hid/desc.c app/hid/hid.c app/hid/main.c app/hid/reportkey.c
else ifeq ($(APP), APP_CDC)
SRCS	+= app/cdc/desc.c app/cdc/cdc.c app/cdc/main.c
endif

OBJS	= $(SRCS:%.c=%.rel)

CFLAGS	= -I./include -I./include/usb -I./hw -I./core

ifeq ($(APP), APP_HID)
CFLAGS	+= -I./app/hid
else ifeq ($(APP), APP_CDC)
CFLAGS	+= -I./app/cdc
endif

all:usb.bin
usb.bin:usb.hex
	objcopy -I ihex -O binary usb.hex usb.bin

usb.hex:usb.ihx
	packihx usb.ihx > usb.hex

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
	-rm -f usb.bin usb.hex usb.ihx usb.map usb.lk usb.mem	


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
	include/usb/ch9.h core/usb.h include/types.h
ifeq ($(APP), APP_HID)
app/hid/desc.rel:app/hid/desc.c include/types.h include/usb/ch9.h app/hid/hid.h
app/hid/hid.rel:app/hid/hid.c include/usb/ch9.h app/hid/hid.h \
	hw/d12.h core/usb.h
app/hid/main.rel:app/hid/main.c hw/key.h
app/hid/reportkey.rel:app/hid/reportkey.c hw/d12.h hw/key.h include/usb/keycodes.h
else ifeq ($(APP), APP_CDC)
app/cdc/cdc.rel:app/cdc/cdc.c include/types.h include/usb/ch9.h \
	hw/d12.h app/cdc/cdc.h core/usb.h
app/cdc/desc.rel:app/cdc/desc.c include/types.h include/usb/ch9.h
app/cdc/main.rel:app/cdc/main.c
endif
