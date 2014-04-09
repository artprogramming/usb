#include <at89x52.h>
#include <stdint.h>

#include "d12.h"

#define DATA_PORT	P0

#define IT_N		P3_2
#define RD_N		P3_7
#define WR_N		P3_6
#define A0_N		P3_5

#define sethigh(pin)	(pin = 1)
#define setlow(pin)	(pin = 0)
#define setdata(val)	(DATA_PORT = val)
#define getdata()	(DATA_PORT)

#define ENABLE		1
#define DISABLE		0

static void d12_write_command(uint8_t cmd)
{
	sethigh(A0_N);
	setlow(WR_N);
	setdata(cmd);
	sethigh(WR_N);
}

static void d12_write_byte(uint8_t val)
{
	setlow(A0_N);
	setlow(WR_N);
	setdata(val);
	sethigh(WR_N);
}

static uint8_t d12_read_byte(void)
{
	uint8_t val;

	setlow(A0_N);
	setlow(RD_N);
	val = getdata();
	sethigh(RD_N);

	return val;
}

uint16_t d12_read_id(void)
{
	uint16_t id;

	d12_write_command(D12_READ_CHIP_ID);
	id = d12_read_byte();
	id |=(uint16_t)d12_read_byte() << 8;

	return id;
}

void d12_init(void)
{
	EX0 = 1;
	IT0 = 0;

	d12_write_command(D12_SET_MODE);
	d12_write_byte(0x16);
	d12_write_byte(0x47);
}

void d12_disconnect(void)
{
	d12_write_command(D12_SET_MODE);
	d12_write_byte(0x06);
	d12_write_byte(0x47);
}

void d12_set_address_enable(uint8_t addr)
{
	d12_write_command(D12_SET_ADDRESS_ENABLE);
	d12_write_byte(addr | 0x80);
}

void d12_set_endpoint_enable(uint8_t en)
{
	d12_write_command(D12_SET_ENDPOINT_ENABLE);
	d12_write_byte(en ? ENABLE : DISABLE);
}

uint16_t d12_read_interrupt_register(void)
{
	uint16_t reg;

	d12_write_command(D12_READ_INTERRUPT_REGISTER);
	reg = d12_read_byte();
	reg |= (uint16_t)d12_read_byte() << 8;

	return reg;
}

void d12_select_endpoint(uint8_t ep)
{
	d12_write_command(D12_SELECT_ENDPOINT + ep);
}

uint8_t d12_read_endpoint_status(uint8_t ep)
{
	uint8_t status;

	d12_select_endpoint(ep);
	d12_write_command(D12_READ_ENDPOINT_STATUS + ep);
	status = d12_read_byte();

	return status;
}

uint8_t d12_read_last_transaction(uint8_t ep)
{
	uint8_t status;

	d12_select_endpoint(ep);
	d12_write_command(D12_READ_LAST_TRANSACTION + ep);
	status = d12_read_byte();

	return status;
}

uint8_t d12_read_buffer(uint8_t ep, uint8_t *buf, uint8_t size)
{
	uint8_t i, len;

	d12_select_endpoint(ep);
	d12_write_command(D12_READ_BUFFER);
	d12_read_byte();
	len = d12_read_byte();

	if (len > size) {
		len = size;
	}

	for (i = 0; i < len; i++) {
		*buf++ = d12_read_byte();
	}

	return len;
}

void d12_write_buffer(uint8_t ep, uint8_t *buf, uint8_t size)
{
	uint8_t i;

	d12_select_endpoint(ep);
	d12_write_command(D12_WRITE_BUFFER);
	d12_write_byte(0x00);
	d12_write_byte(size);

	for (i = 0; i < size; i++) {
		d12_write_byte(*buf++);
	}

	d12_validate_buffer();
}

void d12_clear_buffer(void)
{
	d12_write_command(D12_CLEAR_BUFFER);
}

void d12_validate_buffer(void)
{
	d12_write_command(D12_VALIDATE_BUFFER);
}

void d12_set_endpoint_status(uint8_t ep, uint8_t stall)
{
	d12_write_command(D12_SET_ENDPOINT_STATUS + ep);
	d12_write_byte(stall ? 1 : 0);
}

void d12_ack_setup(void)
{
	d12_select_endpoint(D12_EP0_IN);
	d12_write_command(D12_ACK_SETUP);
	d12_select_endpoint(D12_EP0_OUT);
	d12_write_command(D12_ACK_SETUP);
}

void d12_send_resume(void)
{
	d12_write_command(D12_SEND_RESUME);
}

uint16_t d12_read_frame_number(void)
{
	uint16_t num;

	d12_write_command(D12_READ_FRAME_NUMBER);
	num = d12_read_byte();
	num |= (uint16_t)d12_read_byte() << 8;

	return num;
}