#ifndef __SERIAL_PACKET_PRINT_H__
#define __SERIAL_PACKET_PRINT_H__

#include "serial_packet_jsb.h"
#include <stdio.h>
#include <inttypes.h>

void serial_packet_chk(uint8_t *chk);
void serial_packet_stat(serial_packet *sp);

#endif
