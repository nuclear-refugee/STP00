#ifndef __SERIAL_PACKET_H__
#define __SERIAL_PACKET_H__

#include <inttypes.h>

typedef struct serial_packet_format
{
    volatile uint8_t status;
    //volatile uint8_t Header_count;
    volatile uint8_t WR;
    volatile uint8_t addr;
    volatile uint8_t data;
    volatile uint8_t chksum;
} serial_packet;

#define serial_packet_initial {0,0,0,0,0}

// Packet receiver status
#define Pst_HEADER 0
#define Pst_UID    1
#define Pst_ADDR   2
//#define Pst_BYTES  3
#define Pst_DATA   4
#define Pst_CHKSUM 5

// Packet receiver return value
#define WORKING      0
#define ERROR_HEADER 1
#define ERROR_UID    2
#define ERROR_CHKSUM 3
#define Packet_OK    4

#define P_HEADER 0xAA
#define P_UID 0x12

uint8_t unpack(serial_packet *sp,uint8_t *raw);

#endif
