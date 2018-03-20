#include "Reg_sys.h"

REG_PTR_SIZE * Register_info_initial(uint8_t index)
{
    return malloc( index * sizeof(REG_PTR_SIZE) );
}

void Register_info_fillin(REG_PTR_SIZE * st_ptr, void* reg_ptr, uint8_t bytes)
{
    st_ptr->ptr = reg_ptr;
    st_ptr->bytes = bytes;
}
