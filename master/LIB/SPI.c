#include "SPI.h"
/** Initial setup for master**/
void SPI_master_set(void)
{
    SPI_DDR |= ((1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS));    // MOSI, SCK, SS output
    SPSR = 0b00000000;
    SPCR = 0b01010001;      // SPI enable, select as Master, CPOL and CPHA 0, SCK frequency: fosc/16
}

uint8_t SPI_swap(uint8_t* trans_data, uint8_t *rec_data)
{
    uint8_t count = 0;
    SPDR = *trans_data;
    while((!(SPSR & (1 << SPIF))))
    {
        count ++;
        if(count > 100)
        {
            return 1;
        }
    }
    *rec_data = SPDR;
    return 0;
}
