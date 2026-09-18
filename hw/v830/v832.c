#include "qemu/osdep.h"
#include "qemu/module.h"
#include "qemu/units.h"
#include "qapi/error.h"
#include "exec/memory.h"
#include "exec/address-spaces.h"
#include "sysemu/sysemu.h"
#include "hw/qdev-properties.h"
#include "hw/sysbus.h"
#include "qom/object.h"
#include "hw/misc/unimp.h"
#include "v832.h"

struct V832McuClass {
    /*< private >*/
    SysBusDeviceClass parent_class;

    /*< public >*/
    const char *cpu_type;

    size_t flash_size;
};
typedef struct V832MpuClass V832MpuClass;
DECLARE_CLASS_CHECKERS(V832MpuClass, V832_MPU,
        TYPE_V832_MPU)


// This functions sets up the device
static void v832_mpu_realize(DeviceState *dev, Error **errp)
{
    //We create a state for the microcontroller form the generic state
    V832MpuState *s = V832_MPU(dev);
    //And we create a class from the state
    const V832MpuClass *mc = V832_MPU_GET_CLASS(dev);

    // The V832 CPU was defined in the V832MpuState
    object_initialize_child(OBJECT(dev), "cpu", &s->cpu, mc->cpu_type);
    //Set the CPU object to realized
    object_property_set_bool(OBJECT(&s->cpu), "realized", true, &error_abort);

    //Init the flash memory region
    memory_region_init_rom(&s->flash, OBJECT(dev),
                           "flash", mc->flash_size, &error_fatal);
    //Here we set the start address of the memory region 
    memory_region_add_subregion(get_system_memory(),
                                0x00700000, &s->flash);
}

static void v832_mpu_class_init(ObjectClass *oc, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(oc);
    //Set the actuall setup function
    dc->realize = v832_mpu_realize;
    dc->user_creatable = false;
}

static void v832_mpu_class_init(ObjectClass *oc, void *data){

    V832MpuClass* v832mpu = V832_MPU_CLASS(oc);

    v832mpu->cpu_type = V830_CPU_TYPE_NAME("V832");
    v832mpu->flash_size = 16 * MiB;
}

static const TypeInfo v832_mpu_types[] = {
        {
                .name           = TYPE_V832_MPU,
                .parent         = TYPE_V832_MCU,
                .class_init     = v832_mpu_class_init,
        }, {
                .name           = TYPE_V832_MCU,
                .parent         = TYPE_SYS_BUS_DEVICE,
                .instance_size  = sizeof(V832McuState),
                .class_size     = sizeof(V832McuClass),
                .class_init     = v832_mcu_class_init,
                .abstract       = true,
        }
};

DEFINE_TYPES(v832_mpu_types)