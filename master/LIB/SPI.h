#ifndef __SPI_H__
#define __SPI_H__

#include "rev_MEGA128.h"
#include <avr/io.h>

void SPI_master_set(void);
uint8_t SPI_swap(uint8_t* trans_data, uint8_t *rec_data);

#endif
