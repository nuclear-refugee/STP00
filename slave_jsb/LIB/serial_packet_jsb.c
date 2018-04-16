#include "serial_packet_jsb.h"

uint8_t unpack(serial_packet *sp,REG_PTR_SIZE * reg,uint8_t *raw){
    sp->timeout_count = 0;
    switch (sp->status) {
        case Pst_HEADER:{
            if(*raw == P_HEADER) {
                sp->status = Pst_UID;
                sp->chksum = P_HEADER;
                sp->bytes_c = 0;
                free(sp->data);
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
                return Packet_OK;
            }
            else
            {
                sp->status = Pst_HEADER;
                return ERROR_CHKSUM;
            }
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

void pack_respond(serial_packet *sp , REG_PTR_SIZE * reg , fifo_set * tx_fifo )
{
    uint8_t trigger = 0 , i , data ,chksum;
    if( fifo_empty(tx_fifo) )
        trigger = 1;
    serial_packet_state_reset(sp);
    if(sp->WR)   // written
    {   // if ok, then return header as acknowledge
        for( i = 0 ; i != reg[sp->addr].bytes ; i++ )
        {
            *((uint8_t *) (reg[sp->addr].ptr + i )) = *( sp->data + i );
        }
        data = P_HEADER;
        fifo_push( tx_fifo , 1, &data );
    }
    else    // read
    {
        // return HEADER | Data | chksum
        chksum = P_HEADER;
        data = P_HEADER;
        fifo_push( tx_fifo , 1 , &data );
        fifo_push( tx_fifo , reg[sp->addr].bytes , ((uint8_t*)(reg[sp->addr].ptr)) );
        for( i = 0; i != reg[sp->addr].bytes ; i++ )
        {
            chksum += *((uint8_t *)(reg[sp->addr].ptr)+i);
        }
        fifo_push( tx_fifo, 1 , &chksum);
    }
    if(trigger)
    {   // if tx not working(tx stop when fifo empty), then trigger
        fifo_pop(tx_fifo,1,&data);
        // UCSRA |= 1 << TXC;
        USART_transmit( &data );
        trigger = 0;
    }
}

void serial_packet_state_reset(serial_packet *sp)
{
    sp->status = Pst_HEADER;
}
