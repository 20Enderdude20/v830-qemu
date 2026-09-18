#ifndef HW_V830_BOOT_H
#define HW_V830_BOOT_H

#include "hw/boards.h"
#include "cpu.h"

bool v830_load_firmware(v830CPU *cpu, MachineState *ms,
                         MemoryRegion *mr, const char *firmware);

#endif // HW_V830_BOOT_H