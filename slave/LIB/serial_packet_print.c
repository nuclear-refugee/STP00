#include "serial_packet_print.h"

void serial_packet_chk(uint8_t *chk){
    switch(*chk)
    {
        case ERROR_HEADER:
            printf("ERROR HEADER\n");
            break;
        case ERROR_UID:
            printf("ERROR UID\n");
            break;
        case ERROR_CHKSUM:
            printf("ERROR_CHKSUM\n");
            break;
        case Packet_OK:
            printf("Packet OK\n");
            break;
    }
}

void serial_packet_stat(serial_packet *sp)
{
    printf("--- Information ---\n");
    printf("Status:");
    switch(sp->status){
        case Pst_HEADER:
            printf("HEADER\n");
            break;
        case Pst_UID:
            printf("UID\n");
            break;
        case Pst_BYTES:
            printf("BYTES\n");
            break;
        case Pst_ADDR:
            printf("ADDR\n");
            break;
        case Pst_DATA:
            printf("DATA\n");
            break;
        case Pst_CHKSUM:
            printf("CHKSUM\n");
            break;
    }
    // printf("Status : %u\n",sp->status);
    printf("WR     : %u\n",sp->WR);
    printf("ADDR   : %u\n",sp->addr);
    printf("BYTES  : %u\n",sp->bytes);
    printf("Data   : %u\n",sp->data);
    printf("Chksum : %u\n",sp->chksum);
    printf("--- END ---\n");
}
