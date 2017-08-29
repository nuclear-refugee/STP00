#include "../ASALIB/ASA_DEVICE.h"
#include "STP00.h"


int main() {
    char phase[4] = {15-(1<<PHA),15-(1<<PHB),15-(1<<PHC),15-(1<<PHD)};
    uint8_t i=0;
    PH_DDR = 15;
    while (1) {
        PH_PORT = phase[i];
        i = (i+1)%4;
        _delay_ms(10);
    }


    return 0;
}

void uart_init(/* arguments */) {

}
