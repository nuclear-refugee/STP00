#include "serial_packet.h"

uint8_t unpack(serial_packet *sp,uint8_t *raw){
    sp->timeout_count = 0;
    switch (sp->status) {
        case Pst_HEADER:{
            if(*raw == P_HEADER) {
                sp->status = Pst_UID;
                sp->chksum = P_HEADER;
            }
            else return ERROR_HEADER;
            break;
        }
        case Pst_UID:{
            if(*raw == P_UID){
                sp->status = Pst_ADDR;
                sp->chksum += *raw;
            }
            else {
                sp->status = Pst_HEADER;
                return ERROR_UID;
            }
            break;
        }
        case Pst_ADDR:{
            sp->addr = *raw & 0x7F;
            sp->WR = *raw >> 7;
            sp->status = Pst_BYTES;
            sp->chksum += *raw;
            break;
        }
        case Pst_BYTES:{
            sp->bytes = *raw;
            sp->chksum += *raw;
            if(sp->WR)  // if "write" then receive data
                sp->status = Pst_DATA;
            else        // if "read" then receive checksum
                sp->status = Pst_CHKSUM;
            break;
        }
        case Pst_DATA:{
            sp->data = *raw;
            sp->status = Pst_CHKSUM;
            sp->chksum += *raw;
            break;
        }
        case Pst_CHKSUM:{
            if(*raw == sp->chksum)
                return Packet_OK;
            else return ERROR_CHKSUM;
            break;
        }
    }
    return WORKING;
}

uint8_t serial_packet_timeout_count(serial_packet * sp){
    if(sp->status){
        sp->timeout_count ++;
        if(sp->timeout_count > SERIAL_TIMEOUT){
            sp->status = Pst_HEADER;
            sp->timeout_count = 0;
            return 1;
        }
    }
    return 0;
}

void serial_packet_state_reset(serial_packet *sp)
{
    sp->status = Pst_HEADER;
}
