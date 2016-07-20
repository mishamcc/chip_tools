#ifndef USB_FUNCTIONS_H
#define USB_FUNCTIONS_H

#define SETUP_PPORT  1
#define UCFCORE_RESET 2
#define SET_LEDS     3
#define WRITE_PPORT  4
#define READ_PPORT   5
#define EXEC_SPI     6
#define JTAG_EXEC    7
#define SET_GPIO     8
#define GET_GPIO     9
#define SET_GPIO_DIR 10
#define GET_GPIO_DIR 11
#define GET_DESCRIPTOR 12
#define HS_ENABLE     13
#define MF_RESET      14
#define SETUP_SPI     15
#define LOCK_SPI      16 // lock SPort for the given number of milliseconds
#define MF_JTAG_EXEC  17 // 
#define MF_PUT_VECTOR 18
#define MF_GET_VECTOR 19
#define MF_DIR_VECTOR 20
#define WRITE_STRING_PPORT  21
#define READ_STRING_PPORT   22
#define EXEC_STRING         23
#define SPI_SLAVE_START     24

/** "Classic" board w/o gpif support */
#define BOARD_MF_PROTO 1
/** Same with 3 wires enabling GPIF */
#define BOARD_MF_PROTO_GPIF 2
/** MFTest board */
#define BOARD_MF_TEST 3
/// MF Proto board for JTAG debugging. Just direct MCU<->JTAG wiring */
#define BOARD_JTAG_DEBUG 4

// Board capabilities. Bit vector. LSB first.

#define BCAP_JTAG 1
#define BCAP_MF   2

#endif
