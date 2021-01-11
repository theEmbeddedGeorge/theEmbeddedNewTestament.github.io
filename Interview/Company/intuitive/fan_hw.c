#include "fan_hw.h"

#define MMAP(base_addr, offset) {printf("Map physical address %x with offset %x.\n", base_addr, offset);}
#define UMAP(base_addr) {printf("UMap physical address %x.\n", base_addr);}
#define READW(ptr, val) {printf("Read a word from a pointer address.\n");}
#define WRITEW(ptr, val) {printf("Write a word to a pointer address.\n");}

static int general_read_speed(uint32_t* value, Fan_hw* fan_self) {
    // read register
    void *ptr;

    MMAP(fan_self->RD_REG, 0x100);
    READW(ptr, value);
    UMAP(fan_self->RD_REG);

    return 0;
}

static int general_set_speed(uint8_t duty_cycle, Fan_hw* fan_self) {
    // read register
    void *ptr;
    uint32_t step;
    uint32_t value;

    /*
    * Simple algorithm to convert from duty cycles into PWM counts for 
    * our general fan 
    */
    step = (0xfffffff / 100);
    value = (uint32_t) duty_cycle * step;

    MMAP(fan_self->WRT_REG, 0x100);
    WRITEW(ptr, value);
    UMAP(fan_self->WRT_REG);

    return 0;
}
