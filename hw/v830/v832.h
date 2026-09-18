#ifndef HW_V830_V832_H
#define HW_V830_V832_H
#include "target/v830/cpu.h"
#include "qom/object.h"
#include "hw/sysbus.h"
#define TYPE_V830_MPU "V830" //This will be the CPU
#define TYPE_V832_MPU "V832" //This will be a system on chip (SOC)

typedef struct V832MpuState V832MpuState;
DECLARE_INSTANCE_CHECKER(V832MpuState, V832_MPU, TYPE_V832_MPU)

//The Microcontroller needs a state
struct V832MpuState {
    /*< private >*/
    SysBusDevice parent_obj;

    /*< public >*/
    //This is a reference to the CPU logic. We implement it in part 2.
    V830ACPU cpu;
    //The flash that will contain the firmware.
    MemoryRegion flash;
};
#endif // HW_V830_V832_H