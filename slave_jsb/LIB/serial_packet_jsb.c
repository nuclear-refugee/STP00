#include "serial_packet_jsb.h"

uint8_t unpack(serial_packet *sp,REG_PTR_SIZE * reg,uint8_t *raw){
    sp->timeout_count = 0;
    switch (sp->status) {
        case Pst_HEADER:{
            if(*raw == P_HEADER) {
                sp->status = Pst_UID;
                sp->chksum = P_HEADER;
            }
            // else return HEADER_NOT_MATCH;
            break;
        }
        case Pst_UID:{
            if(*raw == P_UID){
                sp->status = Pst_ADDR;
                sp->chksum += *raw;
            }
            else {
                sp->status = Pst_HEADER;
                // return UID_NOT_MATCH;
            }
            break;
        }
        case Pst_ADDR:{
            sp->addr = *raw & 0x7F;
            sp->WR = *raw >> 7;
            sp->chksum += *raw;
            if(sp->addr >= sp->reg_index_max ){
                sp->status = Pst_HEADER;
                return ERROR_ADDR;
            }
            if(sp->WR){
                free(sp->data);
                // printf("specify reg[%u] length: %u\n",sp->addr,reg[sp->addr].bytes);
                sp->data = malloc(reg[sp->addr].bytes);
                sp->status = Pst_DATA;
            }
            else
                sp->status = Pst_CHKSUM;
            break;
        }
        case Pst_DATA:{
            *(sp->data+sp->bytes_c) = *raw;
            sp->bytes_c ++;
            if(sp->bytes_c == reg[sp->addr].bytes)
                sp->status = Pst_CHKSUM;
            sp->chksum += *raw;
            break;
        }
        case Pst_CHKSUM:{
            if(*raw == sp->chksum){
                for(int i = 0 ; i < reg[sp->addr].bytes ; i++ ){
                    *(uint8_t*)(reg[sp->addr].ptr+i) = *(sp->data+i);
                }
                return Packet_OK;
            }
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
