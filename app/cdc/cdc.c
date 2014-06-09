#include <stdint.h>

#include "printf.h"
#include "types.h"
#include "ch9.h"
#include "d12.h"
#include "cdc.h"
#include "usb.h"

//static struct cdc_line_coding this_line_coding = {115200, 0, 0, 8};
static uint8_t this_device_line_coding[] = {0x80, 0x25, 0x00, 0x00, 0x00, 0x00, 0x08};

static void cdc_set_line_coding(void);
static void cdc_get_line_coding(struct setup_packet *setup);
static void cdc_set_control_line_state(void);

extern uint8_t flag;
void cdc_class_request(struct setup_packet *setup)
{
	switch (setup->bRequest) {
	case CDC_SET_LINE_CODING:
		cdc_set_line_coding();
		break;
	case CDC_GET_LINE_CODING:
		cdc_get_line_coding(setup);
		break;
	case CDC_SET_CONTROL_LINE_STATE:
		cdc_set_control_line_state();
		break;
	default:
		break;
	}
}

//uint8_t buf[7];
static void cdc_set_line_coding(void)
{
	/*int i;
	printf("set line coding\n");
	for (i = 0; i < 7; i++) {
		this_device_line_coding[i] = 0x00;
	}
	d12_read_buffer(D12_EPINDEX_0_IN, (uint8_t *)&this_device_line_coding, 7);
	
	for (i = 0; i < 7; i++) {
		printf("%x ", this_device_line_coding[i]);
	}
	printf("\n");
	usb_send_zero_length_packet();*/
	flag = 1;
}

static void cdc_get_line_coding(struct setup_packet *setup)
{
	int bytes_send, i;

	printf("get line coding\n");
	bytes_send = MIN(sizeof(struct cdc_line_coding), setup->wLength);
	for (i = 0; i < 7; i++) {
		printf("sss %x ", this_device_line_coding[i]);
	}
	printf("\n");
	d12_write_buffer(D12_EPINDEX_0_IN, (uint8_t *)&this_device_line_coding, 7);
	printf("for for\n");
}

static void cdc_set_control_line_state(void)
{
	printf("set control line state\n");
	usb_send_zero_length_packet();
}

void cdc_init(void)
{
	usb_register_class_driver(cdc_class_request);
}
