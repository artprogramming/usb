#ifndef __D12_H
#define __D12_H

/* command codes */
#define D12_READ_CHIP_ID		0xFD

#define D12_SET_ADDRESS_ENABLE		0xD0
#define D12_SET_ENDPOINT_ENABLE		0xD8
#define D12_SET_MODE			0xF3
#define D12_SET_DMA			0xFB
#define D12_READ_INTERRUPT_REGISTER	0xF4
#define D12_SELECT_ENDPOINT		0x00
#define D12_READ_ENDPOINT_STATUS	0x80
#define D12_READ_LAST_TRANSACTION	0x40
#define D12_READ_BUFFER			0xF0
#define D12_WRITE_BUFFER		0xF0
#define D12_CLEAR_BUFFER		0xF2
#define D12_VALIDATE_BUFFER		0xFA
#define D12_SET_ENDPOINT_STATUS		0x40
#define D12_ACK_SETUP			0xF1
#define D12_SEND_RESUME			0xF6
#define D12_READ_FRAME_NUMBER		0xF5


/* endpoint index */
#define D12_EP0_OUT			0x00
#define D12_EP0_IN			0x01
#define D12_EP1_OUT			0x02
#define D12_EP1_IN			0x03
#define D12_EP2_OUT			0x04
#define D12_EP2_IN			0x05


/* chip id from internet */
#define D12_CHIP_ID			0x1012


/* mode configuration bits */
#define D12_MODE_CFG_NO_LAZYCLOCK	(1 << 1)
#define D12_MODE_CFG_CLOCK_RUNNING	(1 << 2)
#define D12_MODE_CFG_INTERRUPT		(1 << 3)
#define D12_MODE_CFG_SOFTCONNECT	(1 << 4)
#define D12_MODE_CFG_NONISO		(0x00 << 6)
#define D12_MODE_CFG_ISOOUT		(0x01 << 6)
#define D12_MODE_CFG_ISOIN		(0x02 << 6)
#define D12_MODE_CFG_ISOIO		(0x03 << 6)

/* mode clock division factor byte */
#define D12_MODE_CLK_24MHZ		1
#define D12_MODE_CLK_16MHZ		2
#define D12_MODE_CLK_12MHZ		3
#define D12_MODE_CLK_8MHZ		5
#define D12_MODE_CLK_6MHZ		7
#define D12_MODE_CLK_4MHZ		11

#define D12_MODE_CLK_DIV_MASK		0x0F

#define D12_MODE_CLK_SET_TO_ONE		(1 << 6)
#define D12_MODE_CLK_SOF_ONLY_INTERRUPT	(1 << 7)


/* dma configuration register */
#define D12_DMA_BURST_SINGAL_CYCLE	(0x00)
#define D12_DMA_BURST_4_CYCLE		(0x01)
#define D12_DMA_BURST_8_CYCLE		(0x02)
#define D12_DMA_BURST_16_CYCLE		(0x03)
#define D12_DMA_ENABLE			(1 << 2)
#define D12_DMA_DIR_WRITE		(1 << 3)
#define D12_DMA_DIR_READ		(0 << 3)
#define D12_DMA_AUTO_RELOAD		(1 << 4)
#define D12_DMA_INTERRUPT_PIN_MODE	(1 << 5)
#define D12_DMA_ENDPOINT_INDEX4_INTERRUPT_ENABLE (1 << 6)
#define D12_DMA_ENDPOINT_INDEX5_INTERRUPT_ENABLE (1 << 7)


/* interrupt register */
#define D12_INT_EP0_OUT			(0x01 << 0)
#define D12_INT_EP0_IN			(0x01 << 1)
#define D12_INT_EP1_OUT			(0x01 << 2)
#define D12_INT_EP1_IN			(0x01 << 3)
#define D12_INT_EP2_OUT			(0x01 << 4)
#define D12_INT_EP2_IN			(0x01 << 5)
#define D12_INT_BUS_RESET		(0x01 << 6)
#define D12_INT_SUSPEND			(0x01 << 7)
#define D12_INT_DMA_EOT			(0x01 << 8)


/* endpoint status */
#define D12_EPSTATUS_SETUP_PACKET	(1 << 2)
#define D12_EPSTATUS_BUFFER0_FULL	(1 << 5)
#define D12_EPSTATUS_BUFFER1_FULL	(1 << 6)
#define D12_EPSTATUE_ENDPOINT_STALLED	(1 << 7)


/* last transaction status */
#define D12_LAST_TRANS_DATA_SUCCESS	(1 << 0)
#define D12_LAST_TRANS_ERROR_CODE_MASK	(0x0F << 1)
#define D12_LAST_TRANS_SETUP_PACKET	(1 << 5)
#define D12_LAST_TRANS_DATA1_PACKET	(1 << 6)
#define D12_LAST_TRANS_PREVIOUS_STATUS_NOT_READ (1 << 7)


/* API */
extern uint16_t d12_read_id(void);
extern void d12_init(void);
extern void d12_disconnect(void);
extern void d12_set_address_enable(uint8_t addr);
extern void d12_set_endpoint_enable(uint8_t en);
extern uint16_t d12_read_interrupt_register(void);
extern void d12_select_endpoint(uint8_t ep);
extern uint8_t d12_read_endpoint_status(uint8_t ep);
extern uint8_t d12_read_last_transaction(uint8_t ep);
extern uint8_t d12_read_buffer(uint8_t ep, uint8_t *buf, uint8_t size);
extern void d12_write_buffer(uint8_t ep, uint8_t *buf, uint8_t size);
extern void d12_clear_buffer(void);
extern void d12_validate_buffer(void);
extern void d12_set_endpoint_status(uint8_t ep, uint8_t stall);
extern void d12_ack_setup(void);
extern void d12_send_resume(void);
extern uint16_t d12_read_frame_number(void);

#endif /* __D12_H */
