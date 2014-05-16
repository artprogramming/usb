#include <stdint.h>

#include "printf.h"
#include "types.h"
#include "ch9.h"
#include "d12.h"
#include "cdc.h"
#include "usb.h"

static struct cdc_line_coding this_line_coding = {115200, 0, 0, 8};

static void cdc_set_line_coding(void);
static void cdc_get_line_coding(struct setup_packet *setup);
static void cdc_set_control_line_state(void);

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

static void cdc_set_line_coding(void)
{
}

static void cdc_get_line_coding(struct setup_packet *setup)
{
	int bytes_send;

	bytes_send = MIN(sizeof(struct cdc_line_coding), setup->wLength);
	d12_write_buffer(D12_EPINDEX_0_IN, &this_line_coding, bytes_send);
}

static void cdc_set_control_line_state(void)
{
	usb_send_zero_length_packet();
}
