#ifndef __SERIAL_PACKET_H__
#define __SERIAL_PACKET_H__

#include <inttypes.h>
#include "Reg_sys.h"
#include "fifo.h"
#include "device.h"
// #include <stdio.h>
#include "uart.h"

typedef struct serial_packet_format
{
    volatile uint8_t status;
    volatile uint8_t UID;
    volatile uint8_t bytes_c;
    volatile uint8_t WR;
    volatile uint8_t addr;
    volatile uint8_t * data;
    volatile uint8_t chksum;
    volatile uint8_t timeout_count;
    volatile uint8_t reg_index_max;
} serial_packet;

#define serial_packet_initial {0,0,0,0,0,0,0,0}

// Packet receiver status
#define Pst_HEADER 0
#define Pst_UID    1
#define Pst_ADDR   2
#define Pst_BYTES  3
#define Pst_DATA   4
#define Pst_CHKSUM 5
// TODO: change to enum

// Packet receiver(unpack) return value
#define WORKING          0
//#define HEADER_NOT_MATCH 1
//#define UID_NOT_MATCH    2
#define ERROR_CHKSUM     0xA3
#define ERROR_ADDR       0xA4
#define Packet_OK        0xA5
// TODO: enum

#define P_HEADER 0xF0
#define P_UID    0x12
#define P_ERROR  0xA0

#define SERIAL_TIMEOUT 200

uint8_t unpack(serial_packet *sp,REG_PTR_SIZE * reg,uint8_t *raw);
uint8_t serial_packet_timeout_count(serial_packet * sp);
void serial_packet_state_reset(serial_packet *sp);
void pack_respond(serial_packet *sp , REG_PTR_SIZE * reg , fifo_set * tx_fifo);

#endif
