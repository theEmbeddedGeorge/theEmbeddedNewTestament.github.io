#define MMAP(base_addr, offset) // {printf("Map physical address %x with offset %x.\n", base_addr, offset);}
#define UMAP(base_addr) //{printf("UMap physical address %x.\n", base_addr);}
#define READW(ptr, val) //{printf("Read a word from a pointer address.\n");}
#define WRITEW(ptr, val) //{printf("Write a word to a pointer address.\n");}
#define WRITEB(ptr, val) //{printf("Write a byte to a pointer address.\n");}

#define MAX_FAN_NUM 20

typedef struct fan_hw Fan_hw;
typedef int (*read_speed_cb)(uint32_t* value, Fan_hw *fan_self);
typedef int (*set_speed_cb)(uint32_t duty_cycle, Fan_hw *fan_self);

struct fan_hw {
    char vendor[32];
    char model[32];
    uint32_t WRT_REG;
    uint32_t RD_REG;
    read_speed_cb read_spd;
    set_speed_cb set_spd;
};

int general_read_speed(uint32_t* value, Fan_hw* fan_self);
int general_set_speed(uint32_t duty_cycle, Fan_hw* fan_self);

/* Fan hardware instances */
Fan_hw general_fan_list[MAX_FAN_NUM] = {
    {"General_vendor", "0xa1", 0xffff8000, 0xffff4000, general_read_speed, general_set_speed},
    {"General_vendor", "0xa1", 0xffff8000, 0xffff4000, general_read_speed, general_set_speed},
    {"General_vendor", "0xb2", 0xffffa000, 0xffff2000, general_read_speed, general_set_speed},
    {"General_vendor", "0xb2", 0xffffa000, 0xffff2000, general_read_speed, general_set_speed},
    {"General_vendor", "0xc3", 0xffffe000, 0xffff6000, general_read_speed, general_set_speed},
    {"General_vendor", "0xc3", 0xffffe000, 0xffff6000, general_read_speed, general_set_speed},
    {"General_vendor", "0xd4", 0xffff8020, 0xffff4020, general_read_speed, general_set_speed},
    {"General_vendor", "0xd4", 0xffff8020, 0xffff4020, general_read_speed, general_set_speed},
    {"General_vendor", "0xe5", 0xffff8040, 0xffff1000, general_read_speed, general_set_speed},
    {"General_vendor", "0xe5", 0xffff8040, 0xffff1000, general_read_speed, general_set_speed},
    {"General_vendor", "0xa1", 0xffff8000, 0xffff4000, general_read_speed, general_set_speed},
    {"General_vendor", "0xa1", 0xffff8000, 0xffff4000, general_read_speed, general_set_speed},
    {"General_vendor", "0xb2", 0xffffa000, 0xffff2000, general_read_speed, general_set_speed},
    {"General_vendor", "0xb2", 0xffffa000, 0xffff2000, general_read_speed, general_set_speed},
    {"General_vendor", "0xc3", 0xffffe000, 0xffff6000, general_read_speed, general_set_speed},
    {"General_vendor", "0xc3", 0xffffe000, 0xffff6000, general_read_speed, general_set_speed},
    {"General_vendor", "0xd4", 0xffff8020, 0xffff4020, general_read_speed, general_set_speed},
    {"General_vendor", "0xd4", 0xffff8020, 0xffff4020, general_read_speed, general_set_speed},
    {"General_vendor", "0xe5", 0xffff8040, 0xffff1000, general_read_speed, general_set_speed},
    {"General_vendor", "0xe5", 0xffff8040, 0xffff1000, general_read_speed, general_set_speed},
};

int general_read_speed(uint32_t* value, Fan_hw* fan_self) {
    // read register
    void *ptr;

    MMAP(fan_self->RD_REG, 0x100);
    READW(ptr, value);
    UMAP(fan_self->RD_REG);

    return 0;
}

int general_set_speed(uint32_t duty_cycle, Fan_hw* fan_self) {
    // read register
    void *ptr;
    uint16_t step;
    uint16_t value;

    /*
    * Simple algorithm to convert from duty cycles into PWM counts for 
    * our general fan 
    */
    step = (0xffff / 100);
    value = (uint16_t)duty_cycle * step;

    MMAP(fan_self->WRT_REG, 0x100);
    WRITEW(ptr, value);
    UMAP(fan_self->WRT_REG);

    return 0;
}
