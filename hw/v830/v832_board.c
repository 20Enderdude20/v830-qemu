#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qapi/error.h"
#include "boot.h"
#include "qom/object.h"
#include "hw/boards.h"


#define TYPE_V832_BOARD_BASE_MACHINE MACHINE_TYPE_NAME("v832-board-base")
#define TYPE_V832_BOARD_MACHINE MACHINE_TYPE_NAME("v832-board")


struct v832BoardMachineState {
    MachineState parent_obj;
    v832MpuState mpu;
};
typedef struct v832BoardMachineState v832BoardMachineState;

struct v832BoardMachineClass {
    MachineClass parent_class;
};

DECLARE_OBJ_CHECKERS(v832BoardMachineState, v832BoardMachineClass,
        V832_BOARD_MACHINE, TYPE_V832_BOARD_MACHINE)

        
        //The generic MachineState is passed by QEMU
static void v832_board_init(MachineState *machine)
{
    //Make a specific MachineState out of the generic one
    v832BoardMachineState* m_state = V832_BOARD_MACHINE(machine);

    //We initialize the mocrocontroller that is part of the board
    object_initialize_child(OBJECT(machine), "mpu", &m_state->mpu, TYPE_V832_MPU);
    //And we connect it via QEMU's SYSBUS.
    sysbus_realize(SYS_BUS_DEVICE(&m_state->mpu), &error_abort);

    //Here we load the firmware file with a load function that we will implment in boot.c
    if (machine->firmware) {
        if (!v832_load_firmware(&m_state->mpu.cpu, machine,
                                 &m_state->mpu.flash, machine->firmware)) {
            exit(1);
        }
    }
}


//Generic Objectc is passed by QEMU
static void v832_board_class_init(ObjectClass *oc, void *data)
{
    //The generic machine class from object
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "NEC V832 Generic Board";
    mc->alias = "v832-board";
    
    //Notice that we tell QEMU what function is used to initialize our board here.
    mc->init = v832_board_init;
    mc->default_cpus = 1;
    mc->min_cpus = mc->default_cpus;
    mc->max_cpus = mc->default_cpus;
    // Our board does not have any media drive
    mc->no_floppy = 1;
    mc->no_cdrom = 1;
    //We also will not have threads
    mc->no_parallel = 1;
}



static const TypeInfo v832_board_machine_types[] = {
        {
                                //Notice that this is the TYPE that we defined above.
                .name           = TYPE_V832_BOARD_MACHINE,
                                //Our machine is a direct child of QEMU generic machine
                .parent         = TYPE_MACHINE,
                .instance_size  = sizeof(V832BoardMachineState),
                .class_size     = sizeof(V832BoardMachineClass),
                //We need to register the class init function 
                .class_init     = v832_board_class_init,
        }
};
DEFINE_TYPES(v832_board_machine_types)
