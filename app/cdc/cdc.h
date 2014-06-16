#ifndef __USB_CDC_H
#define __USB_CDC_H

#include <stdint.h>

enum {
	SET_LINE_CODING = 0x20,
	GET_LINE_CODING = 0x21,
	SET_CONTROL_LINE_STATE = 0x22,
};

struct line_coding {
	/* dwDTERate:	Baudrates, 9600 or 115200 */
	uint32_t dwDTERate;

	/* bCharForamt: 0 - 1	Stop bit
	 *		1 - 1.5 Stop bits
	 *		2 - 2	Stop bits
	 */
	uint8_t bCharFormat;

	/* bParityType:	0 - None
	 *		1 - Odd
	 *		2 - Even
	 *		3 - Mark
	 *		4 - Space
	 */
	uint8_t bParityType;

	/* bDataBits: 5, 6, 7, 8 or 16 */
	uint8_t bDataBits;
};

#endif /* __USB_CDC_H */
