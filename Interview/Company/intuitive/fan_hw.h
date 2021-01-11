#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

#define MAX_FAN_NUM 10

typedef struct fan_hw Fan_hw;
typedef int (*read_speed_cb)(uint32_t* value, Fan_hw *fan_self);
typedef int (*set_speed_cb)(uint32_t spd, Fan_hw *fan_self);

struct fan_hw {
    char vendor[32];
    char model[32];
    uint32_t WRT_REG;
    uint32_t RD_REG;
    read_speed_cb read_spd;
    set_speed_cb set_spd;
};

static int general_read_speed(uint32_t* value, Fan_hw* fan_self);
static int general_set_speed(uint32_t duty_cycle, Fan_hw* fan_self);

Fan_hw general_fan_list[MAX_FAN_NUM] {
    {"General_vendor", "0xa1", 0xffff8000, 0xffff4000},
    {"General_vendor", "0xa1", 0xffff8000, 0xffff4000},
    {"General_vendor", "0xb2", 0xffffa000, 0xffff2000},
    {"General_vendor", "0xb2", 0xffffa000, 0xffff2000},
    {"General_vendor", "0xc3", 0xffffe000, 0xffff6000},
    {"General_vendor", "0xc3", 0xffffe000, 0xffff6000},
    {"General_vendor", "0xd4", 0xffff8020, 0xffff4020},
    {"General_vendor", "0xd4", 0xffff8020, 0xffff4020},
    {"General_vendor", "0xe5", 0xffff8040, 0xffff1000},
    {"General_vendor", "0xe5", 0xffff8040, 0xffff1000},
}