#ifndef __REG_SYS_H__
#define __REG_SYS_H__

#include <inttypes.h>
#include <stdlib.h>

typedef struct register_ptr_size{
    void*   ptr;
    uint8_t bytes;
} REG_PTR_SIZE;

REG_PTR_SIZE * Register_info_initial(uint8_t index);
void Register_info_fillin(REG_PTR_SIZE * st_ptr, void* reg_ptr, uint8_t bytes);

#endif
