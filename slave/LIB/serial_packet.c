#include "serial_packet.h"

uint8_t unpack(serial_packet *sp,uint8_t *raw){
    switch (sp->status) {
        case Pst_HEADER:{
            if(*raw == P_HEADER) {
                sp->status = Pst_UID;
            }
            else return ERROR_HEADER;
            break;
        }
        case Pst_UID:{
            if(*raw == P_UID){
                sp->status = Pst_ADDR;
                sp->chksum = *raw;
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
            sp->status = Pst_DATA;
            sp->chksum += *raw;
            break;
        }
        //
        // case Pst_BYTES:{
        //     break;
        // }
        case Pst_DATA:{
            sp->data = *raw;
            sp->status = Pst_CHKSUM;
            sp->chksum += *raw;
            break;
        }
        case Pst_CHKSUM:{
            if(*raw == sp->chksum) {
                sp->status = Pst_HEADER;
                return Packet_OK;
            }
            else return ERROR_CHKSUM;
            break;
        }
    }
    return WORKING;
}
