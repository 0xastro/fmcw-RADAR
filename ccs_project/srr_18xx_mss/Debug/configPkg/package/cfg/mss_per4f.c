/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-E10
 */

#define __nested__
#define __config__

#include <xdc/std.h>

/*
 * ======== GENERATED SECTIONS ========
 *
 *     MODULE INCLUDES
 *
 *     <module-name> INTERNALS
 *     <module-name> INHERITS
 *     <module-name> VTABLE
 *     <module-name> PATCH TABLE
 *     <module-name> DECLARATIONS
 *     <module-name> OBJECT OFFSETS
 *     <module-name> TEMPLATES
 *     <module-name> INITIALIZERS
 *     <module-name> FUNCTION STUBS
 *     <module-name> PROXY BODY
 *     <module-name> OBJECT DESCRIPTOR
 *     <module-name> VIRTUAL FUNCTIONS
 *     <module-name> SYSTEM FUNCTIONS
 *     <module-name> PRAGMAS
 *
 *     INITIALIZATION ENTRY POINT
 *     PROGRAM GLOBALS
 *     CLINK DIRECTIVES
 */


/*
 * ======== MODULE INCLUDES ========
 */

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/family/arm/IntrinsicsSupport.h>
#include <ti/sysbios/family/arm/TaskSupport.h>
#include <ti/sysbios/family/arm/exc/Exception.h>
#include <ti/sysbios/family/arm/v7r/tms570/Core.h>
#include <ti/sysbios/family/arm/v7r/vim/Hwi.h>
#include <ti/sysbios/gates/GateHwi.h>
#include <ti/sysbios/gates/GateMutex.h>
#include <ti/sysbios/hal/Cache.h>
#include <ti/sysbios/hal/CacheNull.h>
#include <ti/sysbios/hal/Core.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/sysbios/heaps/HeapMem.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Idle.h>
#include <ti/sysbios/knl/Intrinsics.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/timers/rti/Timer.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Core.h>
#include <xdc/runtime/Defaults.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Gate.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Main.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/Registry.h>
#include <xdc/runtime/Startup.h>
#include <xdc/runtime/SysStd.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Text.h>

/* suppress 'type qualifier is meaningless on cast type' warning */
#ifdef __ti__
#pragma diag_suppress 193
#endif
#ifdef __IAR_SYSTEMS_ICC__
#pragma diag_suppress=Pe191
#endif

/*
 * ======== ti.sysbios.BIOS INTERNALS ========
 */


/*
 * ======== ti.sysbios.BIOS_RtsGateProxy INTERNALS ========
 */

/* Module__ */
typedef struct ti_sysbios_BIOS_RtsGateProxy_Module__ {
    xdc_runtime_Types_Link link;
} ti_sysbios_BIOS_RtsGateProxy_Module__;

/* Module__root__V */
extern ti_sysbios_BIOS_RtsGateProxy_Module__ ti_sysbios_BIOS_RtsGateProxy_Module__root__V;

/* @@@ ti_sysbios_knl_Queue_Object__ */
typedef struct ti_sysbios_knl_Queue_Object__ {
    ti_sysbios_knl_Queue_Elem elem;
} ti_sysbios_knl_Queue_Object__;

/* @@@ ti_sysbios_knl_Queue_Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    ti_sysbios_knl_Queue_Object__ obj;
} ti_sysbios_knl_Queue_Object2__;

/* @@@ ti_sysbios_knl_Semaphore_Object__ */
typedef struct ti_sysbios_knl_Semaphore_Object__ {
    ti_sysbios_knl_Event_Handle event;
    xdc_UInt eventId;
    ti_sysbios_knl_Semaphore_Mode mode;
    volatile xdc_UInt16 count;
    ti_sysbios_knl_Queue_Object__ Object_field_pendQ;
} ti_sysbios_knl_Semaphore_Object__;

/* @@@ ti_sysbios_knl_Semaphore_Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    ti_sysbios_knl_Semaphore_Object__ obj;
} ti_sysbios_knl_Semaphore_Object2__;

/* Object__ */
typedef struct ti_sysbios_gates_GateMutex_Object__ {
    const ti_sysbios_gates_GateMutex_Fxns__ *__fxns;
    ti_sysbios_knl_Task_Handle owner;
    ti_sysbios_knl_Semaphore_Object__ Object_field_sem;
} ti_sysbios_gates_GateMutex_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    ti_sysbios_gates_GateMutex_Object__ obj;
} ti_sysbios_gates_GateMutex_Object2__;

/* Object */
typedef ti_sysbios_gates_GateMutex_Object__ ti_sysbios_BIOS_RtsGateProxy_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    ti_sysbios_BIOS_RtsGateProxy_Object__ obj;
} ti_sysbios_BIOS_RtsGateProxy_Object2__;


/*
 * ======== ti.sysbios.family.arm.IntrinsicsSupport INTERNALS ========
 */


/*
 * ======== ti.sysbios.family.arm.TaskSupport INTERNALS ========
 */


/*
 * ======== ti.sysbios.family.arm.exc.Exception INTERNALS ========
 */


/*
 * ======== ti.sysbios.family.arm.v7r.tms570.Core INTERNALS ========
 */


/*
 * ======== ti.sysbios.family.arm.v7r.vim.Hwi INTERNALS ========
 */

/* Module__ */
typedef struct ti_sysbios_family_arm_v7r_vim_Hwi_Module__ {
    xdc_runtime_Types_Link link;
} ti_sysbios_family_arm_v7r_vim_Hwi_Module__;

/* Module__root__V */
extern ti_sysbios_family_arm_v7r_vim_Hwi_Module__ ti_sysbios_family_arm_v7r_vim_Hwi_Module__root__V;

/* Object__ */
typedef struct ti_sysbios_family_arm_v7r_vim_Hwi_Object__ {
    const ti_sysbios_family_arm_v7r_vim_Hwi_Fxns__ *__fxns;
    ti_sysbios_family_arm_v7r_vim_Hwi_Type type;
    xdc_UArg arg;
    ti_sysbios_family_arm_v7r_vim_Hwi_FuncPtr fxn;
    xdc_Int intNum;
    ti_sysbios_family_arm_v7r_vim_Hwi_Irp irp;
    __TA_ti_sysbios_family_arm_v7r_vim_Hwi_Instance_State__hookEnv hookEnv;
    __TA_ti_sysbios_family_arm_v7r_vim_Hwi_Instance_State__disableMask disableMask;
} ti_sysbios_family_arm_v7r_vim_Hwi_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    ti_sysbios_family_arm_v7r_vim_Hwi_Object__ obj;
} ti_sysbios_family_arm_v7r_vim_Hwi_Object2__;


/*
 * ======== ti.sysbios.gates.GateHwi INTERNALS ========
 */

/* Module__ */
typedef struct ti_sysbios_gates_GateHwi_Module__ {
    xdc_runtime_Types_Link link;
} ti_sysbios_gates_GateHwi_Module__;

/* Module__root__V */
extern ti_sysbios_gates_GateHwi_Module__ ti_sysbios_gates_GateHwi_Module__root__V;

/* Object__ */
typedef struct ti_sysbios_gates_GateHwi_Object__ {
    const ti_sysbios_gates_GateHwi_Fxns__ *__fxns;
} ti_sysbios_gates_GateHwi_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    ti_sysbios_gates_GateHwi_Object__ obj;
} ti_sysbios_gates_GateHwi_Object2__;


/*
 * ======== ti.sysbios.gates.GateMutex INTERNALS ========
 */

/* Module__ */
typedef struct ti_sysbios_gates_GateMutex_Module__ {
    xdc_runtime_Types_Link link;
} ti_sysbios_gates_GateMutex_Module__;

/* Module__root__V */
extern ti_sysbios_gates_GateMutex_Module__ ti_sysbios_gates_GateMutex_Module__root__V;

/* <-- ti_sysbios_gates_GateMutex_Object */


/*
 * ======== ti.sysbios.hal.Cache INTERNALS ========
 */


/*
 * ======== ti.sysbios.hal.CacheNull INTERNALS ========
 */


/*
 * ======== ti.sysbios.hal.Cache_CacheProxy INTERNALS ========
 */


/*
 * ======== ti.sysbios.hal.Core INTERNALS ========
 */


/*
 * ======== ti.sysbios.hal.Core_CoreProxy INTERNALS ========
 */


/*
 * ======== ti.sysbios.hal.Hwi INTERNALS ========
 */

/* Module__ */
typedef struct ti_sysbios_hal_Hwi_Module__ {
    xdc_runtime_Types_Link link;
} ti_sysbios_hal_Hwi_Module__;

/* Module__root__V */
extern ti_sysbios_hal_Hwi_Module__ ti_sysbios_hal_Hwi_Module__root__V;

/* Object__ */
typedef struct ti_sysbios_hal_Hwi_Object__ {
    const ti_sysbios_hal_Hwi_Fxns__ *__fxns;
    ti_sysbios_hal_Hwi_HwiProxy_Handle pi;
} ti_sysbios_hal_Hwi_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    ti_sysbios_hal_Hwi_Object__ obj;
} ti_sysbios_hal_Hwi_Object2__;


/*
 * ======== ti.sysbios.hal.Hwi_HwiProxy INTERNALS ========
 */

/* Module__ */
typedef struct ti_sysbios_hal_Hwi_HwiProxy_Module__ {
    xdc_runtime_Types_Link link;
} ti_sysbios_hal_Hwi_HwiProxy_Module__;

/* Module__root__V */
extern ti_sysbios_hal_Hwi_HwiProxy_Module__ ti_sysbios_hal_Hwi_HwiProxy_Module__root__V;

/* <-- ti_sysbios_family_arm_v7r_vim_Hwi_Object */

/* Object */
typedef ti_sysbios_family_arm_v7r_vim_Hwi_Object__ ti_sysbios_hal_Hwi_HwiProxy_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    ti_sysbios_hal_Hwi_HwiProxy_Object__ obj;
} ti_sysbios_hal_Hwi_HwiProxy_Object2__;


/*
 * ======== ti.sysbios.heaps.HeapBuf INTERNALS ========
 */

/* Module__ */
typedef struct ti_sysbios_heaps_HeapBuf_Module__ {
    xdc_runtime_Types_Link link;
} ti_sysbios_heaps_HeapBuf_Module__;

/* Module__root__V */
extern ti_sysbios_heaps_HeapBuf_Module__ ti_sysbios_heaps_HeapBuf_Module__root__V;

/* <-- ti_sysbios_knl_Queue_Object */

/* Object__ */
typedef struct ti_sysbios_heaps_HeapBuf_Object__ {
    const ti_sysbios_heaps_HeapBuf_Fxns__ *__fxns;
    xdc_SizeT blockSize;
    xdc_SizeT align;
    xdc_UInt numBlocks;
    xdc_runtime_Memory_Size bufSize;
    __TA_ti_sysbios_heaps_HeapBuf_Instance_State__buf buf;
    xdc_UInt numFreeBlocks;
    xdc_UInt minFreeBlocks;
    ti_sysbios_knl_Queue_Object__ Object_field_freeList;
} ti_sysbios_heaps_HeapBuf_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    ti_sysbios_heaps_HeapBuf_Object__ obj;
} ti_sysbios_heaps_HeapBuf_Object2__;


/*
 * ======== ti.sysbios.heaps.HeapMem INTERNALS ========
 */

/* Module__ */
typedef struct ti_sysbios_heaps_HeapMem_Module__ {
    xdc_runtime_Types_Link link;
} ti_sysbios_heaps_HeapMem_Module__;

/* Module__root__V */
extern ti_sysbios_heaps_HeapMem_Module__ ti_sysbios_heaps_HeapMem_Module__root__V;

/* Object__ */
typedef struct ti_sysbios_heaps_HeapMem_Object__ {
    const ti_sysbios_heaps_HeapMem_Fxns__ *__fxns;
    xdc_runtime_Memory_Size align;
    __TA_ti_sysbios_heaps_HeapMem_Instance_State__buf buf;
    ti_sysbios_heaps_HeapMem_Header head;
    xdc_SizeT minBlockAlign;
} ti_sysbios_heaps_HeapMem_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    ti_sysbios_heaps_HeapMem_Object__ obj;
} ti_sysbios_heaps_HeapMem_Object2__;


/*
 * ======== ti.sysbios.heaps.HeapMem_Module_GateProxy INTERNALS ========
 */

/* Module__ */
typedef struct ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__ {
    xdc_runtime_Types_Link link;
} ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__;

/* Module__root__V */
extern ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__ ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__root__V;

/* <-- ti_sysbios_gates_GateMutex_Object */

/* Object */
typedef ti_sysbios_gates_GateMutex_Object__ ti_sysbios_heaps_HeapMem_Module_GateProxy_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    ti_sysbios_heaps_HeapMem_Module_GateProxy_Object__ obj;
} ti_sysbios_heaps_HeapMem_Module_GateProxy_Object2__;


/*
 * ======== ti.sysbios.knl.Clock INTERNALS ========
 */

/* Module__ */
typedef struct ti_sysbios_knl_Clock_Module__ {
    xdc_runtime_Types_Link link;
} ti_sysbios_knl_Clock_Module__;

/* Module__root__V */
extern ti_sysbios_knl_Clock_Module__ ti_sysbios_knl_Clock_Module__root__V;

/* <-- ti_sysbios_knl_Queue_Object */

/* Object__ */
typedef struct ti_sysbios_knl_Clock_Object__ {
    ti_sysbios_knl_Queue_Elem elem;
    xdc_UInt32 timeout;
    xdc_UInt32 currTimeout;
    xdc_UInt32 period;
    volatile xdc_Bool active;
    ti_sysbios_knl_Clock_FuncPtr fxn;
    xdc_UArg arg;
} ti_sysbios_knl_Clock_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    ti_sysbios_knl_Clock_Object__ obj;
} ti_sysbios_knl_Clock_Object2__;


/*
 * ======== ti.sysbios.knl.Clock_TimerProxy INTERNALS ========
 */

/* Module__ */
typedef struct ti_sysbios_knl_Clock_TimerProxy_Module__ {
    xdc_runtime_Types_Link link;
} ti_sysbios_knl_Clock_TimerProxy_Module__;

/* Module__root__V */
extern ti_sysbios_knl_Clock_TimerProxy_Module__ ti_sysbios_knl_Clock_TimerProxy_Module__root__V;

/* Object__ */
typedef struct ti_sysbios_timers_rti_Timer_Object__ {
    const ti_sysbios_timers_rti_Timer_Fxns__ *__fxns;
    xdc_Bool staticInst;
    xdc_Int id;
    ti_sysbios_interfaces_ITimer_RunMode runMode;
    ti_sysbios_interfaces_ITimer_StartMode startMode;
    xdc_UInt prescale;
    xdc_UInt period;
    ti_sysbios_interfaces_ITimer_PeriodType periodType;
    xdc_UInt intNum;
    xdc_UArg arg;
    ti_sysbios_hal_Hwi_FuncPtr tickFxn;
    xdc_runtime_Types_FreqHz extFreq;
    ti_sysbios_hal_Hwi_Handle hwi;
    xdc_Bool createHwi;
} ti_sysbios_timers_rti_Timer_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    ti_sysbios_timers_rti_Timer_Object__ obj;
} ti_sysbios_timers_rti_Timer_Object2__;

/* Object */
typedef ti_sysbios_timers_rti_Timer_Object__ ti_sysbios_knl_Clock_TimerProxy_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    ti_sysbios_knl_Clock_TimerProxy_Object__ obj;
} ti_sysbios_knl_Clock_TimerProxy_Object2__;


/*
 * ======== ti.sysbios.knl.Event INTERNALS ========
 */

/* Module__ */
typedef struct ti_sysbios_knl_Event_Module__ {
    xdc_runtime_Types_Link link;
} ti_sysbios_knl_Event_Module__;

/* Module__root__V */
extern ti_sysbios_knl_Event_Module__ ti_sysbios_knl_Event_Module__root__V;

/* <-- ti_sysbios_knl_Queue_Object */

/* Object__ */
typedef struct ti_sysbios_knl_Event_Object__ {
    volatile xdc_UInt postedEvents;
    ti_sysbios_knl_Queue_Object__ Object_field_pendQ;
} ti_sysbios_knl_Event_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    ti_sysbios_knl_Event_Object__ obj;
} ti_sysbios_knl_Event_Object2__;


/*
 * ======== ti.sysbios.knl.Idle INTERNALS ========
 */


/*
 * ======== ti.sysbios.knl.Intrinsics INTERNALS ========
 */


/*
 * ======== ti.sysbios.knl.Intrinsics_SupportProxy INTERNALS ========
 */


/*
 * ======== ti.sysbios.knl.Queue INTERNALS ========
 */

/* Module__ */
typedef struct ti_sysbios_knl_Queue_Module__ {
    xdc_runtime_Types_Link link;
} ti_sysbios_knl_Queue_Module__;

/* Module__root__V */
extern ti_sysbios_knl_Queue_Module__ ti_sysbios_knl_Queue_Module__root__V;

/* <-- ti_sysbios_knl_Queue_Object */


/*
 * ======== ti.sysbios.knl.Semaphore INTERNALS ========
 */

/* Module__ */
typedef struct ti_sysbios_knl_Semaphore_Module__ {
    xdc_runtime_Types_Link link;
} ti_sysbios_knl_Semaphore_Module__;

/* Module__root__V */
extern ti_sysbios_knl_Semaphore_Module__ ti_sysbios_knl_Semaphore_Module__root__V;

/* <-- ti_sysbios_knl_Semaphore_Object */


/*
 * ======== ti.sysbios.knl.Swi INTERNALS ========
 */

/* Module__ */
typedef struct ti_sysbios_knl_Swi_Module__ {
    xdc_runtime_Types_Link link;
} ti_sysbios_knl_Swi_Module__;

/* Module__root__V */
extern ti_sysbios_knl_Swi_Module__ ti_sysbios_knl_Swi_Module__root__V;

/* Object__ */
typedef struct ti_sysbios_knl_Swi_Object__ {
    ti_sysbios_knl_Queue_Elem qElem;
    ti_sysbios_knl_Swi_FuncPtr fxn;
    xdc_UArg arg0;
    xdc_UArg arg1;
    xdc_UInt priority;
    xdc_UInt mask;
    xdc_Bool posted;
    xdc_UInt initTrigger;
    xdc_UInt trigger;
    ti_sysbios_knl_Queue_Handle readyQ;
    __TA_ti_sysbios_knl_Swi_Instance_State__hookEnv hookEnv;
} ti_sysbios_knl_Swi_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    ti_sysbios_knl_Swi_Object__ obj;
} ti_sysbios_knl_Swi_Object2__;


/*
 * ======== ti.sysbios.knl.Task INTERNALS ========
 */

/* Module__ */
typedef struct ti_sysbios_knl_Task_Module__ {
    xdc_runtime_Types_Link link;
} ti_sysbios_knl_Task_Module__;

/* Module__root__V */
extern ti_sysbios_knl_Task_Module__ ti_sysbios_knl_Task_Module__root__V;

/* <-- ti_sysbios_knl_Queue_Object */

/* Object__ */
typedef struct ti_sysbios_knl_Task_Object__ {
    ti_sysbios_knl_Queue_Elem qElem;
    volatile xdc_Int priority;
    xdc_UInt mask;
    xdc_Ptr context;
    ti_sysbios_knl_Task_Mode mode;
    ti_sysbios_knl_Task_PendElem *pendElem;
    xdc_SizeT stackSize;
    __TA_ti_sysbios_knl_Task_Instance_State__stack stack;
    xdc_runtime_IHeap_Handle stackHeap;
    ti_sysbios_knl_Task_FuncPtr fxn;
    xdc_UArg arg0;
    xdc_UArg arg1;
    xdc_Ptr env;
    __TA_ti_sysbios_knl_Task_Instance_State__hookEnv hookEnv;
    xdc_Bool vitalTaskFlag;
    ti_sysbios_knl_Queue_Handle readyQ;
    xdc_UInt curCoreId;
    xdc_UInt affinity;
    xdc_Bool privileged;
    xdc_Ptr domain;
    xdc_UInt32 checkValue;
    xdc_Ptr tls;
} ti_sysbios_knl_Task_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    ti_sysbios_knl_Task_Object__ obj;
} ti_sysbios_knl_Task_Object2__;


/*
 * ======== ti.sysbios.knl.Task_SupportProxy INTERNALS ========
 */


/*
 * ======== ti.sysbios.timers.rti.Timer INTERNALS ========
 */

/* Module__ */
typedef struct ti_sysbios_timers_rti_Timer_Module__ {
    xdc_runtime_Types_Link link;
} ti_sysbios_timers_rti_Timer_Module__;

/* Module__root__V */
extern ti_sysbios_timers_rti_Timer_Module__ ti_sysbios_timers_rti_Timer_Module__root__V;

/* <-- ti_sysbios_timers_rti_Timer_Object */


/*
 * ======== xdc.runtime.Assert INTERNALS ========
 */


/*
 * ======== xdc.runtime.Core INTERNALS ========
 */


/*
 * ======== xdc.runtime.Defaults INTERNALS ========
 */


/*
 * ======== xdc.runtime.Diags INTERNALS ========
 */


/*
 * ======== xdc.runtime.Error INTERNALS ========
 */


/*
 * ======== xdc.runtime.Gate INTERNALS ========
 */


/*
 * ======== xdc.runtime.Log INTERNALS ========
 */


/*
 * ======== xdc.runtime.Main INTERNALS ========
 */


/*
 * ======== xdc.runtime.Main_Module_GateProxy INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_Main_Module_GateProxy_Module__ {
    xdc_runtime_Types_Link link;
} xdc_runtime_Main_Module_GateProxy_Module__;

/* Module__root__V */
extern xdc_runtime_Main_Module_GateProxy_Module__ xdc_runtime_Main_Module_GateProxy_Module__root__V;

/* <-- ti_sysbios_gates_GateHwi_Object */

/* Object */
typedef ti_sysbios_gates_GateHwi_Object__ xdc_runtime_Main_Module_GateProxy_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_Main_Module_GateProxy_Object__ obj;
} xdc_runtime_Main_Module_GateProxy_Object2__;


/*
 * ======== xdc.runtime.Memory INTERNALS ========
 */


/*
 * ======== xdc.runtime.Memory_HeapProxy INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_Memory_HeapProxy_Module__ {
    xdc_runtime_Types_Link link;
} xdc_runtime_Memory_HeapProxy_Module__;

/* Module__root__V */
extern xdc_runtime_Memory_HeapProxy_Module__ xdc_runtime_Memory_HeapProxy_Module__root__V;

/* <-- ti_sysbios_heaps_HeapMem_Object */

/* Object */
typedef ti_sysbios_heaps_HeapMem_Object__ xdc_runtime_Memory_HeapProxy_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_Memory_HeapProxy_Object__ obj;
} xdc_runtime_Memory_HeapProxy_Object2__;


/*
 * ======== xdc.runtime.Registry INTERNALS ========
 */


/*
 * ======== xdc.runtime.Startup INTERNALS ========
 */


/*
 * ======== xdc.runtime.SysStd INTERNALS ========
 */


/*
 * ======== xdc.runtime.System INTERNALS ========
 */


/*
 * ======== xdc.runtime.System_Module_GateProxy INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_System_Module_GateProxy_Module__ {
    xdc_runtime_Types_Link link;
} xdc_runtime_System_Module_GateProxy_Module__;

/* Module__root__V */
extern xdc_runtime_System_Module_GateProxy_Module__ xdc_runtime_System_Module_GateProxy_Module__root__V;

/* <-- ti_sysbios_gates_GateHwi_Object */

/* Object */
typedef ti_sysbios_gates_GateHwi_Object__ xdc_runtime_System_Module_GateProxy_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_System_Module_GateProxy_Object__ obj;
} xdc_runtime_System_Module_GateProxy_Object2__;


/*
 * ======== xdc.runtime.System_SupportProxy INTERNALS ========
 */


/*
 * ======== xdc.runtime.Text INTERNALS ========
 */


/*
 * ======== INHERITS ========
 */

#pragma DATA_SECTION(xdc_runtime_IHeap_Interface__BASE__C, ".const:xdc_runtime_IHeap_Interface__BASE__C");
__FAR__ const xdc_runtime_Types_Base xdc_runtime_IHeap_Interface__BASE__C = {&xdc_runtime_IModule_Interface__BASE__C};

#pragma DATA_SECTION(ti_sysbios_interfaces_ICache_Interface__BASE__C, ".const:ti_sysbios_interfaces_ICache_Interface__BASE__C");
__FAR__ const xdc_runtime_Types_Base ti_sysbios_interfaces_ICache_Interface__BASE__C = {&xdc_runtime_IModule_Interface__BASE__C};

#pragma DATA_SECTION(xdc_runtime_ISystemSupport_Interface__BASE__C, ".const:xdc_runtime_ISystemSupport_Interface__BASE__C");
__FAR__ const xdc_runtime_Types_Base xdc_runtime_ISystemSupport_Interface__BASE__C = {&xdc_runtime_IModule_Interface__BASE__C};

#pragma DATA_SECTION(xdc_runtime_IGateProvider_Interface__BASE__C, ".const:xdc_runtime_IGateProvider_Interface__BASE__C");
__FAR__ const xdc_runtime_Types_Base xdc_runtime_IGateProvider_Interface__BASE__C = {&xdc_runtime_IModule_Interface__BASE__C};

#pragma DATA_SECTION(xdc_runtime_IModule_Interface__BASE__C, ".const:xdc_runtime_IModule_Interface__BASE__C");
__FAR__ const xdc_runtime_Types_Base xdc_runtime_IModule_Interface__BASE__C = {0};


/*
 * ======== ti.sysbios.gates.GateHwi VTABLE ========
 */

/* Module__FXNS__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateHwi_Module__FXNS__C, ".const:ti_sysbios_gates_GateHwi_Module__FXNS__C");
const ti_sysbios_gates_GateHwi_Fxns__ ti_sysbios_gates_GateHwi_Module__FXNS__C = {
    &xdc_runtime_IGateProvider_Interface__BASE__C, /* __base */
    &ti_sysbios_gates_GateHwi_Module__FXNS__C.__sfxns, /* __sysp */
    ti_sysbios_gates_GateHwi_query__E,
    ti_sysbios_gates_GateHwi_enter__E,
    ti_sysbios_gates_GateHwi_leave__E,
    {
        ti_sysbios_gates_GateHwi_Object__create__S,
        ti_sysbios_gates_GateHwi_Object__delete__S,
        ti_sysbios_gates_GateHwi_Handle__label__S,
        0x8028, /* __mid */
    } /* __sfxns */
};


/*
 * ======== ti.sysbios.gates.GateMutex VTABLE ========
 */

/* Module__FXNS__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateMutex_Module__FXNS__C, ".const:ti_sysbios_gates_GateMutex_Module__FXNS__C");
const ti_sysbios_gates_GateMutex_Fxns__ ti_sysbios_gates_GateMutex_Module__FXNS__C = {
    &xdc_runtime_IGateProvider_Interface__BASE__C, /* __base */
    &ti_sysbios_gates_GateMutex_Module__FXNS__C.__sfxns, /* __sysp */
    ti_sysbios_gates_GateMutex_query__E,
    ti_sysbios_gates_GateMutex_enter__E,
    ti_sysbios_gates_GateMutex_leave__E,
    {
        ti_sysbios_gates_GateMutex_Object__create__S,
        ti_sysbios_gates_GateMutex_Object__delete__S,
        ti_sysbios_gates_GateMutex_Handle__label__S,
        0x8029, /* __mid */
    } /* __sfxns */
};


/*
 * ======== ti.sysbios.hal.CacheNull VTABLE ========
 */

/* Module__FXNS__C */
#pragma DATA_SECTION(ti_sysbios_hal_CacheNull_Module__FXNS__C, ".const:ti_sysbios_hal_CacheNull_Module__FXNS__C");
const ti_sysbios_hal_CacheNull_Fxns__ ti_sysbios_hal_CacheNull_Module__FXNS__C = {
    &ti_sysbios_interfaces_ICache_Interface__BASE__C, /* __base */
    &ti_sysbios_hal_CacheNull_Module__FXNS__C.__sfxns, /* __sysp */
    ti_sysbios_hal_CacheNull_enable__E,
    ti_sysbios_hal_CacheNull_disable__E,
    ti_sysbios_hal_CacheNull_inv__E,
    ti_sysbios_hal_CacheNull_wb__E,
    ti_sysbios_hal_CacheNull_wbInv__E,
    ti_sysbios_hal_CacheNull_wbAll__E,
    ti_sysbios_hal_CacheNull_wbInvAll__E,
    ti_sysbios_hal_CacheNull_wait__E,
    {
        NULL, /* __create */
        NULL, /* __delete */
        NULL, /* __label */
        0x802c, /* __mid */
    } /* __sfxns */
};


/*
 * ======== ti.sysbios.heaps.HeapBuf VTABLE ========
 */

/* Module__FXNS__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_Module__FXNS__C, ".const:ti_sysbios_heaps_HeapBuf_Module__FXNS__C");
const ti_sysbios_heaps_HeapBuf_Fxns__ ti_sysbios_heaps_HeapBuf_Module__FXNS__C = {
    &xdc_runtime_IHeap_Interface__BASE__C, /* __base */
    &ti_sysbios_heaps_HeapBuf_Module__FXNS__C.__sfxns, /* __sysp */
    ti_sysbios_heaps_HeapBuf_alloc__E,
    ti_sysbios_heaps_HeapBuf_free__E,
    ti_sysbios_heaps_HeapBuf_isBlocking__E,
    ti_sysbios_heaps_HeapBuf_getStats__E,
    {
        ti_sysbios_heaps_HeapBuf_Object__create__S,
        ti_sysbios_heaps_HeapBuf_Object__delete__S,
        ti_sysbios_heaps_HeapBuf_Handle__label__S,
        0x8020, /* __mid */
    } /* __sfxns */
};


/*
 * ======== ti.sysbios.heaps.HeapMem VTABLE ========
 */

/* Module__FXNS__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_Module__FXNS__C, ".const:ti_sysbios_heaps_HeapMem_Module__FXNS__C");
const ti_sysbios_heaps_HeapMem_Fxns__ ti_sysbios_heaps_HeapMem_Module__FXNS__C = {
    &xdc_runtime_IHeap_Interface__BASE__C, /* __base */
    &ti_sysbios_heaps_HeapMem_Module__FXNS__C.__sfxns, /* __sysp */
    ti_sysbios_heaps_HeapMem_alloc__E,
    ti_sysbios_heaps_HeapMem_free__E,
    ti_sysbios_heaps_HeapMem_isBlocking__E,
    ti_sysbios_heaps_HeapMem_getStats__E,
    {
        ti_sysbios_heaps_HeapMem_Object__create__S,
        ti_sysbios_heaps_HeapMem_Object__delete__S,
        ti_sysbios_heaps_HeapMem_Handle__label__S,
        0x8021, /* __mid */
    } /* __sfxns */
};


/*
 * ======== xdc.runtime.SysStd VTABLE ========
 */

/* Module__FXNS__C */
#pragma DATA_SECTION(xdc_runtime_SysStd_Module__FXNS__C, ".const:xdc_runtime_SysStd_Module__FXNS__C");
const xdc_runtime_SysStd_Fxns__ xdc_runtime_SysStd_Module__FXNS__C = {
    &xdc_runtime_ISystemSupport_Interface__BASE__C, /* __base */
    &xdc_runtime_SysStd_Module__FXNS__C.__sfxns, /* __sysp */
    xdc_runtime_SysStd_abort__E,
    xdc_runtime_SysStd_exit__E,
    xdc_runtime_SysStd_flush__E,
    xdc_runtime_SysStd_putch__E,
    xdc_runtime_SysStd_ready__E,
    {
        NULL, /* __create */
        NULL, /* __delete */
        NULL, /* __label */
        0x800e, /* __mid */
    } /* __sfxns */
};


/*
 * ======== ti.sysbios.BIOS DECLARATIONS ========
 */

/* Module_State__ */
typedef struct ti_sysbios_BIOS_Module_State__ {
    xdc_runtime_Types_FreqHz cpuFreq;
    xdc_UInt rtsGateCount;
    xdc_IArg rtsGateKey;
    ti_sysbios_BIOS_RtsGateProxy_Handle rtsGate;
    ti_sysbios_BIOS_ThreadType threadType;
    __TA_ti_sysbios_BIOS_Module_State__smpThreadType smpThreadType;
    volatile ti_sysbios_BIOS_StartFuncPtr startFunc;
    volatile ti_sysbios_BIOS_ExitFuncPtr exitFunc;
} ti_sysbios_BIOS_Module_State__;

/* --> ti_sysbios_BIOS_startFunc */
extern xdc_Void ti_sysbios_BIOS_startFunc(xdc_Void);

/* --> ti_sysbios_BIOS_exitFunc */
extern xdc_Void ti_sysbios_BIOS_exitFunc(xdc_Int);

/* Module__state__V */
ti_sysbios_BIOS_Module_State__ ti_sysbios_BIOS_Module__state__V;


/*
 * ======== ti.sysbios.BIOS_RtsGateProxy DECLARATIONS ========
 */


/*
 * ======== ti.sysbios.family.arm.IntrinsicsSupport DECLARATIONS ========
 */


/*
 * ======== ti.sysbios.family.arm.TaskSupport DECLARATIONS ========
 */


/*
 * ======== ti.sysbios.family.arm.exc.Exception DECLARATIONS ========
 */

/* Module_State__ */
typedef struct ti_sysbios_family_arm_exc_Exception_Module_State__ {
    __TA_ti_sysbios_family_arm_exc_Exception_Module_State__excActive excActive;
    __TA_ti_sysbios_family_arm_exc_Exception_Module_State__excContext excContext;
    __TA_ti_sysbios_family_arm_exc_Exception_Module_State__excStackBuffers excStackBuffers;
    __TA_ti_sysbios_family_arm_exc_Exception_Module_State__excStack excStack;
    xdc_SizeT excStackSize;
} ti_sysbios_family_arm_exc_Exception_Module_State__;

/* --> ti_sysbios_family_arm_exc_Exception_Module_State_0_excActive__A */
__T1_ti_sysbios_family_arm_exc_Exception_Module_State__excActive ti_sysbios_family_arm_exc_Exception_Module_State_0_excActive__A[1];

/* --> ti_sysbios_family_arm_exc_Exception_Module_State_0_excContext__A */
__T1_ti_sysbios_family_arm_exc_Exception_Module_State__excContext ti_sysbios_family_arm_exc_Exception_Module_State_0_excContext__A[1];

/* --> ti_sysbios_family_arm_exc_Exception_Module_State_0_excStackBuffers__A */
__T1_ti_sysbios_family_arm_exc_Exception_Module_State__excStackBuffers ti_sysbios_family_arm_exc_Exception_Module_State_0_excStackBuffers__A[1];
#ifdef __IAR_SYSTEMS_ICC__
    #pragma data_alignment=8
#endif

/* --> ti_sysbios_family_arm_exc_Exception_Module_State_0_excStack_0__A */
__T1_ti_sysbios_family_arm_exc_Exception_Module_State__excStack ti_sysbios_family_arm_exc_Exception_Module_State_0_excStack_0__A[4096];
#ifdef __ti__align
    #pragma DATA_ALIGN(ti_sysbios_family_arm_exc_Exception_Module_State_0_excStack_0__A, 8);
#endif
#ifdef __GNUC__
#ifndef __TI_COMPILER_VERSION__
__T1_ti_sysbios_family_arm_exc_Exception_Module_State__excStack ti_sysbios_family_arm_exc_Exception_Module_State_0_excStack_0__A[4096] __attribute__ ((aligned(8)));
#endif
#endif

/* --> ti_sysbios_family_arm_exc_Exception_Module_State_0_excStack__A */
__T2_ti_sysbios_family_arm_exc_Exception_Module_State__excStack ti_sysbios_family_arm_exc_Exception_Module_State_0_excStack__A[1];

/* Module__state__V */
ti_sysbios_family_arm_exc_Exception_Module_State__ ti_sysbios_family_arm_exc_Exception_Module__state__V;

/* --> ti_sysbios_family_arm_exc_Exception_excHookFuncs__A */
const __T1_ti_sysbios_family_arm_exc_Exception_excHookFuncs ti_sysbios_family_arm_exc_Exception_excHookFuncs__A[1];


/*
 * ======== ti.sysbios.family.arm.v7r.tms570.Core DECLARATIONS ========
 */


/*
 * ======== ti.sysbios.family.arm.v7r.vim.Hwi DECLARATIONS ========
 */

/* Object__table__V */
ti_sysbios_family_arm_v7r_vim_Hwi_Object__ ti_sysbios_family_arm_v7r_vim_Hwi_Object__table__V[1];

/* Module_State__ */
typedef struct ti_sysbios_family_arm_v7r_vim_Hwi_Module_State__ {
    xdc_Char *taskSP;
    xdc_Char *isrStack;
    xdc_Ptr isrStackBase;
    xdc_Ptr isrStackSize;
    __TA_ti_sysbios_family_arm_v7r_vim_Hwi_Module_State__fiqStack fiqStack;
    xdc_SizeT fiqStackSize;
    xdc_UInt *vimRam;
    __TA_ti_sysbios_family_arm_v7r_vim_Hwi_Module_State__dispatchTable dispatchTable;
    __TA_ti_sysbios_family_arm_v7r_vim_Hwi_Module_State__zeroLatencyFIQMask zeroLatencyFIQMask;
    xdc_UInt spuriousInts;
} ti_sysbios_family_arm_v7r_vim_Hwi_Module_State__;

/* --> __TI_STACK_BASE */
extern void* __TI_STACK_BASE;

/* --> __TI_STACK_SIZE */
extern void* __TI_STACK_SIZE;
#ifdef __IAR_SYSTEMS_ICC__
    #pragma data_alignment=8
#endif

/* --> ti_sysbios_family_arm_v7r_vim_Hwi_Module_State_0_fiqStack__A */
__T1_ti_sysbios_family_arm_v7r_vim_Hwi_Module_State__fiqStack ti_sysbios_family_arm_v7r_vim_Hwi_Module_State_0_fiqStack__A[2048];
#ifdef __ti__sect
    #pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_Module_State_0_fiqStack__A, ".myFiqStack");
#endif
#if defined(__GNUC__) && !(defined(__MACH__) && defined(__APPLE__))
#ifndef __TI_COMPILER_VERSION__
__T1_ti_sysbios_family_arm_v7r_vim_Hwi_Module_State__fiqStack ti_sysbios_family_arm_v7r_vim_Hwi_Module_State_0_fiqStack__A[2048] __attribute__ ((section(".myFiqStack")));
#endif
#endif
#ifdef __ti__align
    #pragma DATA_ALIGN(ti_sysbios_family_arm_v7r_vim_Hwi_Module_State_0_fiqStack__A, 8);
#endif
#ifdef __GNUC__
#ifndef __TI_COMPILER_VERSION__
__T1_ti_sysbios_family_arm_v7r_vim_Hwi_Module_State__fiqStack ti_sysbios_family_arm_v7r_vim_Hwi_Module_State_0_fiqStack__A[2048] __attribute__ ((aligned(8)));
#endif
#endif

/* --> ti_sysbios_family_arm_v7r_vim_Hwi_Module_State_0_dispatchTable__A */
__T1_ti_sysbios_family_arm_v7r_vim_Hwi_Module_State__dispatchTable ti_sysbios_family_arm_v7r_vim_Hwi_Module_State_0_dispatchTable__A[128];

/* Module__state__V */
ti_sysbios_family_arm_v7r_vim_Hwi_Module_State__ ti_sysbios_family_arm_v7r_vim_Hwi_Module__state__V;

/* --> ti_sysbios_family_arm_v7r_vim_Hwi_channelMap__A */
const __T1_ti_sysbios_family_arm_v7r_vim_Hwi_channelMap ti_sysbios_family_arm_v7r_vim_Hwi_channelMap__A[128];

/* --> ti_sysbios_family_arm_v7r_vim_Hwi_intReqEnaSet__A */
const __T1_ti_sysbios_family_arm_v7r_vim_Hwi_intReqEnaSet ti_sysbios_family_arm_v7r_vim_Hwi_intReqEnaSet__A[4];

/* --> ti_sysbios_family_arm_v7r_vim_Hwi_wakeEnaSet__A */
const __T1_ti_sysbios_family_arm_v7r_vim_Hwi_wakeEnaSet ti_sysbios_family_arm_v7r_vim_Hwi_wakeEnaSet__A[4];

/* --> ti_sysbios_knl_Swi_disable__E */
extern xdc_UInt ti_sysbios_knl_Swi_disable__E(xdc_Void);

/* --> ti_sysbios_knl_Swi_restoreHwi__E */
extern xdc_Void ti_sysbios_knl_Swi_restoreHwi__E(xdc_UInt);

/* --> ti_sysbios_knl_Task_disable__E */
extern xdc_UInt ti_sysbios_knl_Task_disable__E(xdc_Void);

/* --> ti_sysbios_knl_Task_restoreHwi__E */
extern xdc_Void ti_sysbios_knl_Task_restoreHwi__E(xdc_UInt);


/*
 * ======== ti.sysbios.gates.GateHwi DECLARATIONS ========
 */

/* Object__table__V */
ti_sysbios_gates_GateHwi_Object__ ti_sysbios_gates_GateHwi_Object__table__V[1];


/*
 * ======== ti.sysbios.gates.GateMutex DECLARATIONS ========
 */

/* Object__table__V */
ti_sysbios_gates_GateMutex_Object__ ti_sysbios_gates_GateMutex_Object__table__V[2];


/*
 * ======== ti.sysbios.hal.Cache DECLARATIONS ========
 */


/*
 * ======== ti.sysbios.hal.CacheNull DECLARATIONS ========
 */


/*
 * ======== ti.sysbios.hal.Cache_CacheProxy DECLARATIONS ========
 */


/*
 * ======== ti.sysbios.hal.Core DECLARATIONS ========
 */


/*
 * ======== ti.sysbios.hal.Core_CoreProxy DECLARATIONS ========
 */


/*
 * ======== ti.sysbios.hal.Hwi DECLARATIONS ========
 */

/* Object__table__V */
ti_sysbios_hal_Hwi_Object__ ti_sysbios_hal_Hwi_Object__table__V[1];


/*
 * ======== ti.sysbios.hal.Hwi_HwiProxy DECLARATIONS ========
 */


/*
 * ======== ti.sysbios.heaps.HeapBuf DECLARATIONS ========
 */

/* Module_State__ */
typedef struct ti_sysbios_heaps_HeapBuf_Module_State__ {
    __TA_ti_sysbios_heaps_HeapBuf_Module_State__constructedHeaps constructedHeaps;
} ti_sysbios_heaps_HeapBuf_Module_State__;

/* Module__state__V */
ti_sysbios_heaps_HeapBuf_Module_State__ ti_sysbios_heaps_HeapBuf_Module__state__V;


/*
 * ======== ti.sysbios.heaps.HeapMem DECLARATIONS ========
 */

#ifdef __IAR_SYSTEMS_ICC__
    #pragma data_alignment=8
#endif
/* --> ti_sysbios_heaps_HeapMem_Instance_State_0_buf__A */
__T1_ti_sysbios_heaps_HeapMem_Instance_State__buf ti_sysbios_heaps_HeapMem_Instance_State_0_buf__A[32768];
#ifdef __ti__sect
    #pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_Instance_State_0_buf__A, "systemHeap");
#endif
#if defined(__GNUC__) && !(defined(__MACH__) && defined(__APPLE__))
#ifndef __TI_COMPILER_VERSION__
__T1_ti_sysbios_heaps_HeapMem_Instance_State__buf ti_sysbios_heaps_HeapMem_Instance_State_0_buf__A[32768] __attribute__ ((section("systemHeap")));
#endif
#endif
#ifdef __ti__align
    #pragma DATA_ALIGN(ti_sysbios_heaps_HeapMem_Instance_State_0_buf__A, 8);
#endif
#ifdef __GNUC__
#ifndef __TI_COMPILER_VERSION__
__T1_ti_sysbios_heaps_HeapMem_Instance_State__buf ti_sysbios_heaps_HeapMem_Instance_State_0_buf__A[32768] __attribute__ ((aligned(8)));
#endif
#endif

/* Object__table__V */
ti_sysbios_heaps_HeapMem_Object__ ti_sysbios_heaps_HeapMem_Object__table__V[1];


/*
 * ======== ti.sysbios.heaps.HeapMem_Module_GateProxy DECLARATIONS ========
 */


/*
 * ======== ti.sysbios.knl.Clock DECLARATIONS ========
 */

/* Module_State__ */
typedef struct ti_sysbios_knl_Clock_Module_State__ {
    volatile xdc_UInt32 ticks;
    xdc_UInt swiCount;
    ti_sysbios_knl_Clock_TimerProxy_Handle timer;
    ti_sysbios_knl_Swi_Handle swi;
    volatile xdc_UInt numTickSkip;
    xdc_UInt32 nextScheduledTick;
    xdc_UInt32 maxSkippable;
    xdc_Bool inWorkFunc;
    volatile xdc_Bool startDuringWorkFunc;
    xdc_Bool ticking;
    ti_sysbios_knl_Queue_Object__ Object_field_clockQ;
} ti_sysbios_knl_Clock_Module_State__;

/* Module__state__V */
ti_sysbios_knl_Clock_Module_State__ ti_sysbios_knl_Clock_Module__state__V;

/* --> ti_sysbios_knl_Clock_doTick__I */
extern xdc_Void ti_sysbios_knl_Clock_doTick__I(xdc_UArg);


/*
 * ======== ti.sysbios.knl.Clock_TimerProxy DECLARATIONS ========
 */


/*
 * ======== ti.sysbios.knl.Event DECLARATIONS ========
 */


/*
 * ======== ti.sysbios.knl.Idle DECLARATIONS ========
 */

/* --> ti_sysbios_hal_Hwi_checkStack */
extern xdc_Void ti_sysbios_hal_Hwi_checkStack(xdc_Void);

/* --> ti_sysbios_knl_Idle_funcList__A */
const __T1_ti_sysbios_knl_Idle_funcList ti_sysbios_knl_Idle_funcList__A[1];

/* --> ti_sysbios_knl_Idle_coreList__A */
const __T1_ti_sysbios_knl_Idle_coreList ti_sysbios_knl_Idle_coreList__A[1];


/*
 * ======== ti.sysbios.knl.Intrinsics DECLARATIONS ========
 */


/*
 * ======== ti.sysbios.knl.Intrinsics_SupportProxy DECLARATIONS ========
 */


/*
 * ======== ti.sysbios.knl.Queue DECLARATIONS ========
 */


/*
 * ======== ti.sysbios.knl.Semaphore DECLARATIONS ========
 */


/*
 * ======== ti.sysbios.knl.Swi DECLARATIONS ========
 */

/* Object__table__V */
ti_sysbios_knl_Swi_Object__ ti_sysbios_knl_Swi_Object__table__V[1];

/* Module_State__ */
typedef struct ti_sysbios_knl_Swi_Module_State__ {
    volatile xdc_Bool locked;
    xdc_UInt curSet;
    xdc_UInt curTrigger;
    ti_sysbios_knl_Swi_Handle curSwi;
    ti_sysbios_knl_Queue_Handle curQ;
    __TA_ti_sysbios_knl_Swi_Module_State__readyQ readyQ;
    __TA_ti_sysbios_knl_Swi_Module_State__constructedSwis constructedSwis;
} ti_sysbios_knl_Swi_Module_State__;

/* --> ti_sysbios_knl_Swi_Module_State_0_readyQ__A */
__T1_ti_sysbios_knl_Swi_Module_State__readyQ ti_sysbios_knl_Swi_Module_State_0_readyQ__A[16];

/* Module__state__V */
ti_sysbios_knl_Swi_Module_State__ ti_sysbios_knl_Swi_Module__state__V;

/* --> ti_sysbios_knl_Task_disable__E */
extern xdc_UInt ti_sysbios_knl_Task_disable__E(xdc_Void);

/* --> ti_sysbios_knl_Task_restore__E */
extern xdc_Void ti_sysbios_knl_Task_restore__E(xdc_UInt);


/*
 * ======== ti.sysbios.knl.Task DECLARATIONS ========
 */

#ifdef __IAR_SYSTEMS_ICC__
    #pragma data_alignment=8
#endif
/* --> ti_sysbios_knl_Task_Instance_State_0_stack__A */
__T1_ti_sysbios_knl_Task_Instance_State__stack ti_sysbios_knl_Task_Instance_State_0_stack__A[2048];
#ifdef __ti__sect
    #pragma DATA_SECTION(ti_sysbios_knl_Task_Instance_State_0_stack__A, ".bss:taskStackSection");
#endif
#if defined(__GNUC__) && !(defined(__MACH__) && defined(__APPLE__))
#ifndef __TI_COMPILER_VERSION__
__T1_ti_sysbios_knl_Task_Instance_State__stack ti_sysbios_knl_Task_Instance_State_0_stack__A[2048] __attribute__ ((section(".bss:taskStackSection")));
#endif
#endif
#ifdef __ti__align
    #pragma DATA_ALIGN(ti_sysbios_knl_Task_Instance_State_0_stack__A, 8);
#endif
#ifdef __GNUC__
#ifndef __TI_COMPILER_VERSION__
__T1_ti_sysbios_knl_Task_Instance_State__stack ti_sysbios_knl_Task_Instance_State_0_stack__A[2048] __attribute__ ((aligned(8)));
#endif
#endif

/* Object__table__V */
ti_sysbios_knl_Task_Object__ ti_sysbios_knl_Task_Object__table__V[1];

/* Module_State__ */
typedef struct ti_sysbios_knl_Task_Module_State__ {
    volatile xdc_Bool locked;
    volatile xdc_UInt curSet;
    volatile xdc_Bool workFlag;
    xdc_UInt vitalTasks;
    ti_sysbios_knl_Task_Handle curTask;
    ti_sysbios_knl_Queue_Handle curQ;
    __TA_ti_sysbios_knl_Task_Module_State__readyQ readyQ;
    __TA_ti_sysbios_knl_Task_Module_State__smpCurSet smpCurSet;
    __TA_ti_sysbios_knl_Task_Module_State__smpCurMask smpCurMask;
    __TA_ti_sysbios_knl_Task_Module_State__smpCurTask smpCurTask;
    __TA_ti_sysbios_knl_Task_Module_State__smpReadyQ smpReadyQ;
    __TA_ti_sysbios_knl_Task_Module_State__idleTask idleTask;
    __TA_ti_sysbios_knl_Task_Module_State__constructedTasks constructedTasks;
    xdc_Bool curTaskPrivileged;
    ti_sysbios_knl_Queue_Object__ Object_field_inactiveQ;
    ti_sysbios_knl_Queue_Object__ Object_field_terminatedQ;
} ti_sysbios_knl_Task_Module_State__;

/* --> ti_sysbios_knl_Task_Module_State_0_readyQ__A */
__T1_ti_sysbios_knl_Task_Module_State__readyQ ti_sysbios_knl_Task_Module_State_0_readyQ__A[16];

/* --> ti_sysbios_knl_Task_Module_State_0_idleTask__A */
__T1_ti_sysbios_knl_Task_Module_State__idleTask ti_sysbios_knl_Task_Module_State_0_idleTask__A[1];

/* Module__state__V */
ti_sysbios_knl_Task_Module_State__ ti_sysbios_knl_Task_Module__state__V;


/*
 * ======== ti.sysbios.knl.Task_SupportProxy DECLARATIONS ========
 */


/*
 * ======== ti.sysbios.timers.rti.Timer DECLARATIONS ========
 */

/* Object__table__V */
ti_sysbios_timers_rti_Timer_Object__ ti_sysbios_timers_rti_Timer_Object__table__V[1];

/* Module_State__ */
typedef struct ti_sysbios_timers_rti_Timer_Module_State__ {
    xdc_UInt availMask;
    __TA_ti_sysbios_timers_rti_Timer_Module_State__device device;
    __TA_ti_sysbios_timers_rti_Timer_Module_State__intFreqs intFreqs;
    __TA_ti_sysbios_timers_rti_Timer_Module_State__handles handles;
} ti_sysbios_timers_rti_Timer_Module_State__;

/* --> ti_sysbios_timers_rti_Timer_Module_State_0_device__A */
__T1_ti_sysbios_timers_rti_Timer_Module_State__device ti_sysbios_timers_rti_Timer_Module_State_0_device__A[2];

/* --> ti_sysbios_timers_rti_Timer_Module_State_0_intFreqs__A */
__T1_ti_sysbios_timers_rti_Timer_Module_State__intFreqs ti_sysbios_timers_rti_Timer_Module_State_0_intFreqs__A[2];

/* --> ti_sysbios_timers_rti_Timer_Module_State_0_handles__A */
__T1_ti_sysbios_timers_rti_Timer_Module_State__handles ti_sysbios_timers_rti_Timer_Module_State_0_handles__A[2];

/* Module__state__V */
ti_sysbios_timers_rti_Timer_Module_State__ ti_sysbios_timers_rti_Timer_Module__state__V;


/*
 * ======== xdc.runtime.Assert DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.Core DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.Defaults DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.Diags DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.Error DECLARATIONS ========
 */

/* Module_State__ */
typedef struct xdc_runtime_Error_Module_State__ {
    xdc_UInt16 count;
} xdc_runtime_Error_Module_State__;

/* Module__state__V */
xdc_runtime_Error_Module_State__ xdc_runtime_Error_Module__state__V;


/*
 * ======== xdc.runtime.Gate DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.Log DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.Main DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.Main_Module_GateProxy DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.Memory DECLARATIONS ========
 */

/* Module_State__ */
typedef struct xdc_runtime_Memory_Module_State__ {
    xdc_SizeT maxDefaultTypeAlign;
} xdc_runtime_Memory_Module_State__;

/* Module__state__V */
xdc_runtime_Memory_Module_State__ xdc_runtime_Memory_Module__state__V;


/*
 * ======== xdc.runtime.Memory_HeapProxy DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.Registry DECLARATIONS ========
 */

/* Module_State__ */
typedef struct xdc_runtime_Registry_Module_State__ {
    xdc_runtime_Registry_Desc *listHead;
    xdc_runtime_Types_ModuleId curId;
} xdc_runtime_Registry_Module_State__;

/* Module__state__V */
xdc_runtime_Registry_Module_State__ xdc_runtime_Registry_Module__state__V;


/*
 * ======== xdc.runtime.Startup DECLARATIONS ========
 */

/* Module_State__ */
typedef struct xdc_runtime_Startup_Module_State__ {
    xdc_Int *stateTab;
    xdc_Bool execFlag;
    xdc_Bool rtsDoneFlag;
} xdc_runtime_Startup_Module_State__;

/* Module__state__V */
xdc_runtime_Startup_Module_State__ xdc_runtime_Startup_Module__state__V;

/* --> ti_sysbios_hal_Hwi_initStack */
extern xdc_Void ti_sysbios_hal_Hwi_initStack(xdc_Void);

/* --> xdc_runtime_Startup_firstFxns__A */
const __T1_xdc_runtime_Startup_firstFxns xdc_runtime_Startup_firstFxns__A[2];

/* --> xdc_runtime_System_Module_startup__E */
extern xdc_Int xdc_runtime_System_Module_startup__E(xdc_Int);

/* --> ti_sysbios_knl_Clock_Module_startup__E */
extern xdc_Int ti_sysbios_knl_Clock_Module_startup__E(xdc_Int);

/* --> ti_sysbios_knl_Swi_Module_startup__E */
extern xdc_Int ti_sysbios_knl_Swi_Module_startup__E(xdc_Int);

/* --> ti_sysbios_knl_Task_Module_startup__E */
extern xdc_Int ti_sysbios_knl_Task_Module_startup__E(xdc_Int);

/* --> ti_sysbios_heaps_HeapBuf_Module_startup__E */
extern xdc_Int ti_sysbios_heaps_HeapBuf_Module_startup__E(xdc_Int);

/* --> ti_sysbios_family_arm_v7r_vim_Hwi_Module_startup__E */
extern xdc_Int ti_sysbios_family_arm_v7r_vim_Hwi_Module_startup__E(xdc_Int);

/* --> ti_sysbios_family_arm_exc_Exception_Module_startup__E */
extern xdc_Int ti_sysbios_family_arm_exc_Exception_Module_startup__E(xdc_Int);

/* --> ti_sysbios_hal_Hwi_Module_startup__E */
extern xdc_Int ti_sysbios_hal_Hwi_Module_startup__E(xdc_Int);

/* --> ti_sysbios_family_arm_v7r_tms570_Core_Module_startup__E */
extern xdc_Int ti_sysbios_family_arm_v7r_tms570_Core_Module_startup__E(xdc_Int);

/* --> ti_sysbios_timers_rti_Timer_Module_startup__E */
extern xdc_Int ti_sysbios_timers_rti_Timer_Module_startup__E(xdc_Int);

/* --> xdc_runtime_Startup_sfxnTab__A */
const __T1_xdc_runtime_Startup_sfxnTab xdc_runtime_Startup_sfxnTab__A[10];

/* --> xdc_runtime_Startup_sfxnRts__A */
const __T1_xdc_runtime_Startup_sfxnRts xdc_runtime_Startup_sfxnRts__A[10];


/*
 * ======== xdc.runtime.SysStd DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.System DECLARATIONS ========
 */

/* Module_State__ */
typedef struct xdc_runtime_System_Module_State__ {
    __TA_xdc_runtime_System_Module_State__atexitHandlers atexitHandlers;
    xdc_Int numAtexitHandlers;
} xdc_runtime_System_Module_State__;

/* --> xdc_runtime_System_Module_State_0_atexitHandlers__A */
__T1_xdc_runtime_System_Module_State__atexitHandlers xdc_runtime_System_Module_State_0_atexitHandlers__A[8];

/* Module__state__V */
xdc_runtime_System_Module_State__ xdc_runtime_System_Module__state__V;

/* --> xdc_runtime_System_printfExtend__I */
extern xdc_Int xdc_runtime_System_printfExtend__I(xdc_Char**,xdc_CString*,xdc_VaList*,xdc_runtime_System_ParseData*);


/*
 * ======== xdc.runtime.System_Module_GateProxy DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.System_SupportProxy DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.Text DECLARATIONS ========
 */

/* Module_State__ */
typedef struct xdc_runtime_Text_Module_State__ {
    xdc_CPtr charBase;
    xdc_CPtr nodeBase;
} xdc_runtime_Text_Module_State__;

/* Module__state__V */
xdc_runtime_Text_Module_State__ xdc_runtime_Text_Module__state__V;

/* --> xdc_runtime_Text_charTab__A */
const __T1_xdc_runtime_Text_charTab xdc_runtime_Text_charTab__A[6894];

/* --> xdc_runtime_Text_nodeTab__A */
const __T1_xdc_runtime_Text_nodeTab xdc_runtime_Text_nodeTab__A[52];


/*
 * ======== OBJECT OFFSETS ========
 */

#pragma DATA_SECTION(ti_sysbios_gates_GateMutex_Instance_State_sem__O, ".const:ti_sysbios_gates_GateMutex_Instance_State_sem__O");
__FAR__ const xdc_SizeT ti_sysbios_gates_GateMutex_Instance_State_sem__O = offsetof(ti_sysbios_gates_GateMutex_Object__,Object_field_sem);


/*
 * ======== OBJECT OFFSETS ========
 */

#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_Instance_State_freeList__O, ".const:ti_sysbios_heaps_HeapBuf_Instance_State_freeList__O");
__FAR__ const xdc_SizeT ti_sysbios_heaps_HeapBuf_Instance_State_freeList__O = offsetof(ti_sysbios_heaps_HeapBuf_Object__,Object_field_freeList);


/*
 * ======== OBJECT OFFSETS ========
 */

#pragma DATA_SECTION(ti_sysbios_knl_Clock_Module_State_clockQ__O, ".const:ti_sysbios_knl_Clock_Module_State_clockQ__O");
__FAR__ const xdc_SizeT ti_sysbios_knl_Clock_Module_State_clockQ__O = offsetof(ti_sysbios_knl_Clock_Module_State__,Object_field_clockQ);


/*
 * ======== OBJECT OFFSETS ========
 */

#pragma DATA_SECTION(ti_sysbios_knl_Event_Instance_State_pendQ__O, ".const:ti_sysbios_knl_Event_Instance_State_pendQ__O");
__FAR__ const xdc_SizeT ti_sysbios_knl_Event_Instance_State_pendQ__O = offsetof(ti_sysbios_knl_Event_Object__,Object_field_pendQ);


/*
 * ======== OBJECT OFFSETS ========
 */

#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_Instance_State_pendQ__O, ".const:ti_sysbios_knl_Semaphore_Instance_State_pendQ__O");
__FAR__ const xdc_SizeT ti_sysbios_knl_Semaphore_Instance_State_pendQ__O = offsetof(ti_sysbios_knl_Semaphore_Object__,Object_field_pendQ);


/*
 * ======== OBJECT OFFSETS ========
 */

#pragma DATA_SECTION(ti_sysbios_knl_Task_Module_State_inactiveQ__O, ".const:ti_sysbios_knl_Task_Module_State_inactiveQ__O");
__FAR__ const xdc_SizeT ti_sysbios_knl_Task_Module_State_inactiveQ__O = offsetof(ti_sysbios_knl_Task_Module_State__,Object_field_inactiveQ);
#pragma DATA_SECTION(ti_sysbios_knl_Task_Module_State_terminatedQ__O, ".const:ti_sysbios_knl_Task_Module_State_terminatedQ__O");
__FAR__ const xdc_SizeT ti_sysbios_knl_Task_Module_State_terminatedQ__O = offsetof(ti_sysbios_knl_Task_Module_State__,Object_field_terminatedQ);


/*
 * ======== xdc.cfg.Program TEMPLATE ========
 */

/*
 *  ======== __ASM__ ========
 *  Define absolute path prefix for this executable's
 *  configuration generated files.
 */
xdc__META(__ASM__, "@(#)__ASM__ = C:/Users/astro/workspace_v9_2/srr_18xx_mss/Debug/configPkg/package/cfg/mss_per4f");

/*
 *  ======== __ISA__ ========
 *  Define the ISA of this executable.  This symbol is used by platform
 *  specific "exec" commands that support more than one ISA; e.g., gdb
 */
xdc__META(__ISA__, "@(#)__ISA__ = v7R");

/*
 *  ======== __PLAT__ ========
 *  Define the name of the platform that can run this executable.  This
 *  symbol is used by platform independent "exec" commands
 */
xdc__META(__PLAT__, "@(#)__PLAT__ = ti.platforms.cortexR");

/*
 *  ======== __TARG__ ========
 *  Define the name of the target used to build this executable.
 */
xdc__META(__TARG__, "@(#)__TARG__ = ti.targets.arm.elf.R4F");

/*
 *  ======== __TRDR__ ========
 *  Define the name of the class that can read/parse this executable.
 */
xdc__META(__TRDR__, "@(#)__TRDR__ = ti.targets.omf.elf.Elf32");


/*
 * ======== xdc.cfg.SourceDir TEMPLATE ========
 */



/*
 * ======== xdc.runtime.Diags TEMPLATE ========
 */



/*
 * ======== xdc.runtime.Error TEMPLATE ========
 */

xdc_runtime_Error_Block xdc_runtime_Error_IgnoreBlock = {
    ~0, /* unused */
    {
        {0, 0} /* data */
    },
    0,  /* id */
    0,  /* msg */
    {
        0,  /* mod */
        0,  /* file */
        0   /* line */
    }
};

/*
 * ======== xdc.runtime.Startup TEMPLATE ========
 */

/*
 *  ======== MODULE STARTUP DONE FUNCTIONS ========
 */
xdc_Bool xdc_runtime_System_Module__startupDone__F(void); /* keep GCC prototype warning quiet */
xdc_Bool xdc_runtime_System_Module__startupDone__F(void) {
    return (&xdc_runtime_Startup_Module__state__V)->stateTab == 0 || (&xdc_runtime_Startup_Module__state__V)->stateTab[0] < 0;
}
xdc_Bool ti_sysbios_knl_Clock_Module__startupDone__F(void); /* keep GCC prototype warning quiet */
xdc_Bool ti_sysbios_knl_Clock_Module__startupDone__F(void) {
    return (&xdc_runtime_Startup_Module__state__V)->stateTab == 0 || (&xdc_runtime_Startup_Module__state__V)->stateTab[1] < 0;
}
xdc_Bool ti_sysbios_knl_Swi_Module__startupDone__F(void); /* keep GCC prototype warning quiet */
xdc_Bool ti_sysbios_knl_Swi_Module__startupDone__F(void) {
    return (&xdc_runtime_Startup_Module__state__V)->stateTab == 0 || (&xdc_runtime_Startup_Module__state__V)->stateTab[2] < 0;
}
xdc_Bool ti_sysbios_knl_Task_Module__startupDone__F(void); /* keep GCC prototype warning quiet */
xdc_Bool ti_sysbios_knl_Task_Module__startupDone__F(void) {
    return (&xdc_runtime_Startup_Module__state__V)->stateTab == 0 || (&xdc_runtime_Startup_Module__state__V)->stateTab[3] < 0;
}
xdc_Bool ti_sysbios_heaps_HeapBuf_Module__startupDone__F(void); /* keep GCC prototype warning quiet */
xdc_Bool ti_sysbios_heaps_HeapBuf_Module__startupDone__F(void) {
    return (&xdc_runtime_Startup_Module__state__V)->stateTab == 0 || (&xdc_runtime_Startup_Module__state__V)->stateTab[4] < 0;
}
xdc_Bool ti_sysbios_family_arm_v7r_vim_Hwi_Module__startupDone__F(void); /* keep GCC prototype warning quiet */
xdc_Bool ti_sysbios_family_arm_v7r_vim_Hwi_Module__startupDone__F(void) {
    return (&xdc_runtime_Startup_Module__state__V)->stateTab == 0 || (&xdc_runtime_Startup_Module__state__V)->stateTab[5] < 0;
}
xdc_Bool ti_sysbios_family_arm_exc_Exception_Module__startupDone__F(void); /* keep GCC prototype warning quiet */
xdc_Bool ti_sysbios_family_arm_exc_Exception_Module__startupDone__F(void) {
    return (&xdc_runtime_Startup_Module__state__V)->stateTab == 0 || (&xdc_runtime_Startup_Module__state__V)->stateTab[6] < 0;
}
xdc_Bool ti_sysbios_hal_Hwi_Module__startupDone__F(void); /* keep GCC prototype warning quiet */
xdc_Bool ti_sysbios_hal_Hwi_Module__startupDone__F(void) {
    return (&xdc_runtime_Startup_Module__state__V)->stateTab == 0 || (&xdc_runtime_Startup_Module__state__V)->stateTab[7] < 0;
}
xdc_Bool ti_sysbios_family_arm_v7r_tms570_Core_Module__startupDone__F(void); /* keep GCC prototype warning quiet */
xdc_Bool ti_sysbios_family_arm_v7r_tms570_Core_Module__startupDone__F(void) {
    return (&xdc_runtime_Startup_Module__state__V)->stateTab == 0 || (&xdc_runtime_Startup_Module__state__V)->stateTab[8] < 0;
}
xdc_Bool ti_sysbios_timers_rti_Timer_Module__startupDone__F(void); /* keep GCC prototype warning quiet */
xdc_Bool ti_sysbios_timers_rti_Timer_Module__startupDone__F(void) {
    return (&xdc_runtime_Startup_Module__state__V)->stateTab == 0 || (&xdc_runtime_Startup_Module__state__V)->stateTab[9] < 0;
}


/*
 * Startup_exec__I is an internal entry point called by target/platform
 * boot code. Boot code is not brought into a partial-link assembly. So,
 * without this pragma, whole program optimizers would otherwise optimize-out
 * this function.
 */
#ifdef __ti__
#pragma FUNC_EXT_CALLED(xdc_runtime_Startup_exec__I);
#endif

#ifdef __GNUC__
#if __GNUC__ >= 4
#ifdef __llvm__
xdc_Void xdc_runtime_Startup_exec__I(void) __attribute__ ((used));
#else
xdc_Void xdc_runtime_Startup_exec__I(void) __attribute__ ((externally_visible));
#endif
#endif
#endif

/*
 *  ======== xdc_runtime_Startup_exec__I ========
 *  Initialize all used modules that have startup functions
 */
xdc_Void xdc_runtime_Startup_exec__I(void)
{
    xdc_Int state[10];
    xdc_runtime_Startup_startModsFxn__C(state, 10);
}

/*
 * ======== xdc.runtime.Reset TEMPLATE ========
 */

/*
 * Startup_reset__I is an internal entry point called by target/platform
 * boot code. Boot code is not brought into a partial-link assembly. So,
 * without this pragma, whole program optimizers would otherwise optimize-out
 * this function.
 */
#ifdef __ti__
#pragma FUNC_EXT_CALLED(xdc_runtime_Startup_reset__I);
#endif

#ifdef __GNUC__
#if __GNUC__ >= 4
#ifdef __llvm__
xdc_Void xdc_runtime_Startup_reset__I(void) __attribute__ ((used));
#else
xdc_Void xdc_runtime_Startup_reset__I(void) __attribute__ ((externally_visible));
#endif
#endif
#endif


/*
 *  ======== xdc_runtime_Startup_reset__I ========
 *  This function is called by bootstrap initialization code as early as
 *  possible in the startup process.  This function calls all functions in
 *  the Reset.fxns array _as well as_ Startup.resetFxn (if it's non-NULL)
 */
xdc_Void xdc_runtime_Startup_reset__I(void)
{
}

/*
 * ======== xdc.runtime.System TEMPLATE ========
 */


#include <xdc/std.h>
#include <limits.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/Text.h>
#include <xdc/runtime/Assert.h>

#include <xdc/runtime/System.h>
#include <xdc/runtime/System__internal.h>

/*
 *  ======== System_printfExtend__I ========
 *  This function processes optional extended formats of printf.
 *  
 *  It returns the number of characters added to the result.
 *
 *  Precision (maximum string length) is not supported for %$S.
 *
 *  Right-justified (which is default) minimum width is not supported
 *  for %$L, %$S, or %$F.
 */
xdc_Int xdc_runtime_System_printfExtend__I(xdc_Char **pbuf, xdc_CString *pfmt, 
    xdc_VaList *pva, xdc_runtime_System_ParseData *parse)
{
    xdc_CString fmt = *pfmt;
    xdc_Int     res;
    xdc_Char    c;
    xdc_Bool    found = FALSE;
    
    /* 
     * Create a local variable 'va' to ensure that the dereference of
     * pva only occurs once.
     */
    va_list va = *pva;
    
    res = 0;

    c = *fmt++;
    *pfmt = *pfmt + 1;

    
    if (c == '$') {
        c = *fmt++;
        *pfmt = *pfmt + 1;
        
        if (c == 'L') {
            xdc_runtime_Types_Label *lab = parse->aFlag ? 
                (xdc_runtime_Types_Label *)xdc_iargToPtr(va_arg(va, xdc_IArg)) :
                (xdc_runtime_Types_Label *)va_arg(va, void *);
            
            /* 
             * Call Text_putLab to write out the label, taking the precision 
             * into account.
             */
            res = xdc_runtime_Text_putLab(lab, pbuf, parse->precis);
            
            /*
             * Set the length to 0 to indicate to 'doPrint' that nothing should
             * be copied from parse.ptr.
             */
            parse->len = 0;
            
            /* Update the minimum width field. */
            parse->width -= res;
            
            found = TRUE;
        }
        
        if (c == 'F') {
            xdc_runtime_Types_Site site;
            
            /* Retrieve the file name string from the argument list */
            site.file = parse->aFlag ? (xdc_Char *) xdc_iargToPtr(va_arg(va, xdc_IArg)) :
                                       (xdc_Char *) va_arg(va, xdc_Char *);
            
            /* Retrieve the line number from the argument list. */
            site.line = parse->aFlag ? (xdc_Int) va_arg(va, xdc_IArg) : 
                                       (xdc_Int) va_arg(va, xdc_Int);
            
            /* 
             * Omit the 'mod' field, set it to 0.
             * '0' is a safe sentinel value - the IDs for named modules are 
             * 0x8000 and higher, and the IDs for unnamed modules go from 0x1
             * to 0x7fff.
             */
            site.mod = 0;
            
            /* 
             * Call putSite to format the file and line number.
             * If a precision was specified, it will be used as the maximum
             * string lengrth.
             */
            res = xdc_runtime_Text_putSite(&site, pbuf, parse->precis);
            
            /*
             * Set the length to 0 to indicate to 'doPrint' that nothing should
             * be copied from parse.ptr.
             */
            parse->len = 0;
       
            /* Update the minimum width field */
            parse->width -= res;
            
            found = TRUE;
        }
        
        if (c == 'S') {
            /* Retrieve the format string from the argument list */
            parse->ptr = parse->aFlag ? 
                (xdc_Char *) xdc_iargToPtr(va_arg(va, xdc_IArg)) :
                (xdc_Char *) va_arg(va, xdc_Char *);
            
            /* Update pva before passing it to doPrint. */
            *pva = va;
            
            /* Perform the recursive format. System_doPrint does not advance
             * the buffer pointer, so it has to be done explicitly.
             */
            res = xdc_runtime_System_doPrint__I(*pbuf, parse->precis, 
                                                parse->ptr, pva, parse->aFlag);

            if (*pbuf) {
                *pbuf += res;
            }
            
            /* Update the temporary variable with any changes to *pva */
            va = *pva;
            
            /*
             * Set the length to 0 to indicate to 'doPrint' that nothing should
             * be copied from parse.ptr
             */
            parse->len = 0;

            /* Update the minimum width field */
            parse->width -= res;
            
            /* Indicate that we were able to interpret the specifier */
            found = TRUE;
        }
        
    }

    if (c == 'f') {
        /* support arguments _after_ optional float support */
        if (parse->aFlag) {
            (void)va_arg(va, xdc_IArg);
        }
        else {
            (void)va_arg(va, double);
        }
    }    

    if (found == FALSE) {
        /* other character (like %) copy to output */
        *(parse->ptr) = c;
        parse->len = 1;
    }

    /*
     * Before returning, we must update the value of pva. We use a label here
     * so that all return points will go through this update.
     * The 'goto end' is here to ensure that there is always a reference to the
     * label (to avoid the compiler complaining). 
     */
    goto end; 
end:
    *pva = va;
    return (res);
}

/*
 * ======== xdc.runtime.Text TEMPLATE ========
 */


/*
 *  ======== xdc_runtime_Text_visitRope__I ========
 *  This function is indirectly called within Text.c through
 *  the visitRopeFxn configuration parameter of xdc.runtime.Text.
 */
void xdc_runtime_Text_visitRope__I(xdc_runtime_Text_RopeId rope,
    xdc_Fxn visFxn, xdc_Ptr visState)
{
    xdc_CString stack[7];
    xdc_runtime_Text_visitRope2__I(rope, visFxn, visState, stack);
}


/*
 * ======== ti.sysbios.BIOS TEMPLATE ========
 */


Void ti_sysbios_BIOS_atExitFunc__I(Int);

extern Void ti_sysbios_BIOS_registerRTSLock();
extern Void ti_sysbios_timers_rti_Timer_startup__E();

Void ti_sysbios_BIOS_startFunc__I()
{
    /*
     *  Check if XDC runtime startup functions have been called. If not, then
     *  call BIOS_linkedWithIncorrectBootLibrary() function. This function
     *  will spin forever.
     */
    if (xdc_runtime_Startup_rtsDone() != TRUE) {
        ti_sysbios_BIOS_linkedWithIncorrectBootLibrary();
    }

    xdc_runtime_System_atexit(
        (xdc_runtime_System_AtexitHandler)ti_sysbios_BIOS_atExitFunc__I);
    ti_sysbios_BIOS_registerRTSLock();
    ti_sysbios_timers_rti_Timer_startup__E();
    ti_sysbios_hal_Hwi_startup();
    ti_sysbios_knl_Swi_startup();
    ti_sysbios_knl_Task_startup();
}

#include <_lock.h>

Void ti_sysbios_BIOS_atExitFunc__I(Int notused)
{
    ti_sysbios_knl_Swi_disable();
    ti_sysbios_knl_Task_disable();
    ti_sysbios_BIOS_setThreadType(ti_sysbios_BIOS_ThreadType_Main);

    if ((&ti_sysbios_BIOS_Module__state__V)->rtsGate != NULL) {
        _register_lock(_nop);
        _register_unlock(_nop);
    }
}


/*
 *  ======== BIOS_rtsLock ========
 *  Called by rts _lock() function
 */
Void ti_sysbios_BIOS_rtsLock()
{
    IArg key;

    key = ti_sysbios_BIOS_RtsGateProxy_enter((&ti_sysbios_BIOS_Module__state__V)->rtsGate);
    if ((&ti_sysbios_BIOS_Module__state__V)->rtsGateCount == 0) {
        (&ti_sysbios_BIOS_Module__state__V)->rtsGateKey = key;
    }
    /* Increment need not be atomic */
    (&ti_sysbios_BIOS_Module__state__V)->rtsGateCount++;
}

/*
 *  ======== BIOS_rtsUnLock ========
 *  Called by rts _unlock() function
 */
Void ti_sysbios_BIOS_rtsUnlock()
{
    xdc_runtime_Assert_isTrue(((&ti_sysbios_BIOS_Module__state__V)->rtsGateCount), NULL);

    (&ti_sysbios_BIOS_Module__state__V)->rtsGateCount--;

    if ((&ti_sysbios_BIOS_Module__state__V)->rtsGateCount == 0) {
        ti_sysbios_BIOS_RtsGateProxy_leave((&ti_sysbios_BIOS_Module__state__V)->rtsGate, (&ti_sysbios_BIOS_Module__state__V)->rtsGateKey);
    }
}

/*
 *  ======== BIOS_nullFunc ========
 */
Void ti_sysbios_BIOS_nullFunc__I()
{
}

/*
 *  ======== BIOS_registerRTSLock ========
 */
Void ti_sysbios_BIOS_registerRTSLock(Void)
{
    if ((&ti_sysbios_BIOS_Module__state__V)->rtsGate != NULL) {
        _register_lock(ti_sysbios_BIOS_rtsLock);
        _register_unlock(ti_sysbios_BIOS_rtsUnlock);
    }
}

/*
 *  ======== BIOS_removeRTSLock ========
 */
Void ti_sysbios_BIOS_removeRTSLock(Void)
{
    if ((&ti_sysbios_BIOS_Module__state__V)->rtsGate != NULL) {
        _register_lock(ti_sysbios_BIOS_nullFunc);
        _register_unlock(ti_sysbios_BIOS_nullFunc);
    }
}

/*
 *  ======== BIOS_exitFunc ========
 */
Void ti_sysbios_BIOS_exitFunc(Int stat)
{
    /* remove the RTS lock */
    ti_sysbios_BIOS_removeRTSLock();

    /* force thread type to 'Main' */
    ti_sysbios_BIOS_setThreadType(ti_sysbios_BIOS_ThreadType_Main);

    xdc_runtime_System_exit(stat);
}

/*
 *  ======== BIOS_errorRaiseHook ========
 */
Void ti_sysbios_BIOS_errorRaiseHook(xdc_runtime_Error_Block *eb)
{
    /*
     * If this is an Assert thread, defang Gate threadtype check
     */
    if (eb->id == xdc_runtime_Assert_E_assertFailed) {
        /* remove the RTS lock */
        ti_sysbios_BIOS_removeRTSLock();
        /* force thread type to 'Main' */
        ti_sysbios_BIOS_setThreadType(ti_sysbios_BIOS_ThreadType_Main);
    }

    /* Call the installed Error.raiseHook */
    ti_sysbios_BIOS_installedErrorHook(eb);
}

/*
 * ======== ti.sysbios.Build TEMPLATE ========
 */


/*
 * ======== ti.sysbios.knl.Clock TEMPLATE ========
 */

Void ti_sysbios_knl_Clock_doTick__I(UArg arg)
{
    /* update system time */
    (&ti_sysbios_knl_Clock_Module__state__V)->ticks++;

    ti_sysbios_knl_Clock_logTick__E();

    if (!ti_sysbios_knl_Queue_empty(ti_sysbios_knl_Clock_Module_State_clockQ())) {
        (&ti_sysbios_knl_Clock_Module__state__V)->swiCount++;

        ti_sysbios_knl_Swi_post((&ti_sysbios_knl_Clock_Module__state__V)->swi);
    }
}

/*
 * ======== ti.sysbios.knl.Task TEMPLATE ========
 */




/*
 * ======== ti.sysbios.family.arm.v7r.vim.Hwi TEMPLATE ========
 */

#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_vectors, ".vecs");

extern Void _c_int00();
extern Void ti_sysbios_family_arm_exc_Exception_excHandlerAsm__I();
extern Void ti_sysbios_family_arm_exc_Exception_excHandlerAsm__I();
extern Void ti_sysbios_family_arm_exc_Exception_excHandlerAsm__I();
extern Void ti_sysbios_family_arm_exc_Exception_excHandlerDataAsm__I();
extern Void ti_sysbios_family_arm_exc_Exception_excHandlerAsm__I();
extern Void ti_sysbios_family_arm_v7r_vim_Hwi_dispatchIRQ__I();

/*
 * The FIQ offset register is placed at an offset of #-0x1B0 w.r.t. the FIQ
 * vector table entry such that the vector table entry can directly reference
 * it.
 */
const UInt32 ti_sysbios_family_arm_v7r_vim_Hwi_vectors[] = {
    (UInt32)(0xE59FF018),                       /* ldr  pc, resetFunc         */
    (UInt32)(0xE59FF018),                       /* ldr  pc, undefInstFunc     */
    (UInt32)(0xE59FF018),                       /* ldr  pc, swiFunc           */
    (UInt32)(0xE59FF018),                       /* ldr  pc, prefetchAbortFunc */
    (UInt32)(0xE59FF018),                       /* ldr  pc, dataAbortFunc     */
    (UInt32)(0xE59FF018),                       /* ldr  pc, reservedFunc      */
    (UInt32)(0xE59FF018),                       /* ldr  pc, irqFunc           */
    (UInt32)(0xE51FF1B0),                       /* ldr  pc, [pc, #-0x1b0]     */
    (UInt32)(&_c_int00),          /* resetFunc                  */
    (UInt32)(&ti_sysbios_family_arm_exc_Exception_excHandlerAsm__I),  /* undefInstFunc              */
    (UInt32)(&ti_sysbios_family_arm_exc_Exception_excHandlerAsm__I),            /* swiFunc                    */
    (UInt32)(&ti_sysbios_family_arm_exc_Exception_excHandlerAsm__I),  /* prefetchAbortFunc          */
    (UInt32)(&ti_sysbios_family_arm_exc_Exception_excHandlerDataAsm__I),      /* dataAbortFunc              */
    (UInt32)(&ti_sysbios_family_arm_exc_Exception_excHandlerAsm__I),       /* reservedFunc               */
    (UInt32)(&ti_sysbios_family_arm_v7r_vim_Hwi_dispatchIRQ__I)             /* irqFunc                    */
};

/*
 * ======== ti.sysbios.rts.MemAlloc TEMPLATE ========
 */



#if defined(__ti__)

#pragma FUNC_EXT_CALLED(malloc);
#pragma FUNC_EXT_CALLED(memalign);
#pragma FUNC_EXT_CALLED(free);
#pragma FUNC_EXT_CALLED(calloc);
#pragma FUNC_EXT_CALLED(realloc);

#define ATTRIBUTE

#elif defined(__IAR_SYSTEMS_ICC__)

#define ATTRIBUTE

#else

#define ATTRIBUTE __attribute__ ((used))

#endif


#include <xdc/std.h>

#include <xdc/runtime/Memory.h>
#include <xdc/runtime/Error.h>

#include <string.h>

#if defined(__GNUC__) && !defined(__ti__)

#include <reent.h>

#endif


/*
 * Header is a union to make sure that the size is a power of 2.
 *
 * On the MSP430 small model (MSP430X), size_t is 2 bytes, which makes
 * the size of this struct 6 bytes.
 */
typedef union Header {
    struct {
        Ptr   actualBuf;
        SizeT size;
    } header;
    UArg pad[2];	/* 4 words on 28L, 8 bytes on most others */
} Header;



/*
 *  ======== ti_sysbios_rts_MemAlloc_alloc ========
 */
static Void *ti_sysbios_rts_MemAlloc_alloc(SizeT size)
{
    Header *packet;
    xdc_runtime_Error_Block eb;

    if (size == 0) {
        return (NULL);
    }

    xdc_runtime_Error_init(&eb);

    packet = (Header *)xdc_runtime_Memory_alloc(NULL,
        (SizeT)(size + sizeof(Header)), 0, &eb);

    if (packet == NULL) {
        return (NULL);
    }

    packet->header.actualBuf = (Ptr)packet;
    packet->header.size = size + sizeof(Header);

    return (packet + 1);
}

/*
 *  ======== malloc ========
 */
Void ATTRIBUTE *malloc(SizeT size)
{
    return (ti_sysbios_rts_MemAlloc_alloc(size));
}

/*
 *  ======== memalign ========
 *  mirrors the memalign() function from the TI run-time library
 */
Void ATTRIBUTE *memalign(SizeT alignment, SizeT size)
{
    Header                      *packet;
    Void                        *tmp;
    xdc_runtime_Error_Block     eb;

    if (alignment < sizeof(Header)) {
        alignment = sizeof(Header);
    }

    /*
     * return NULL if size is 0, or alignment is not a power of 2
     */
    if (size == 0 || (alignment & (alignment - 1))) {
        return (NULL);
    }

    xdc_runtime_Error_init(&eb);

    /*
     * Allocate 'align + size' so that we have room for the 'packet'
     * and can return an aligned buffer.
     */
    tmp = xdc_runtime_Memory_alloc(NULL, alignment + size, alignment, &eb);

    if (tmp == NULL) {
        return (NULL);
    }

    packet = (Header *)((char *)tmp + alignment - sizeof(Header));

    packet->header.actualBuf = tmp;
    packet->header.size = size + sizeof(Header);

    return (packet + 1);
}

/*
 *  ======== calloc ========
 */
Void ATTRIBUTE *calloc(SizeT nmemb, SizeT size)
{
    SizeT       nbytes;
    Ptr         retval;

    nbytes = nmemb * size;

    /* return NULL if there's an overflow */
    if (nmemb && size != (nbytes / nmemb)) {
        return (NULL);
    }

    retval = ti_sysbios_rts_MemAlloc_alloc(nbytes);
    if (retval != NULL) {
        (Void)memset(retval, (Int)'\0', nbytes);
    }

    return (retval);
}

/*
 *  ======== free ========
 */
Void ATTRIBUTE free(Void *ptr)
{
    Header      *packet;

    if (ptr != NULL) {
        packet = ((Header *)ptr) - 1;

        xdc_runtime_Memory_free(NULL, (Ptr)packet->header.actualBuf,
            (packet->header.size +
            ((char*)packet - (char*)packet->header.actualBuf)));
    }
}

/*
 *  ======== realloc ========
 */
Void ATTRIBUTE *realloc(Void *ptr, SizeT size)
{
    Ptr         retval;
    Header      *packet;
    SizeT       oldSize;

    if (ptr == NULL) {
        retval = malloc(size);
    }
    else if (size == 0) {
        free(ptr);
        retval = NULL;
    }
    else {
        packet = (Header *)ptr - 1;
        retval = malloc(size);
        if (retval != NULL) {
            oldSize = packet->header.size - sizeof(Header);
            (Void)memcpy(retval, ptr, (size < oldSize) ? size : oldSize);
            free(ptr);
        }
    }

    return (retval);
}

#if defined(__GNUC__) && !defined(__ti__)

/*
 *  ======== _malloc_r ========
 */
Void ATTRIBUTE *_malloc_r(struct _reent *rptr, SizeT size)
{
    return malloc(size);
}

/*
 *  ======== _calloc_r ========
 */
Void ATTRIBUTE *_calloc_r(struct _reent *rptr, SizeT nmemb, SizeT size)
{
    return calloc(nmemb, size);
}

/*
 *  ======== _free_r ========
 */
Void ATTRIBUTE _free_r(struct _reent *rptr, Void *ptr)
{
    free(ptr);
}

/*
 *  ======== _realloc_r ========
 */
Void ATTRIBUTE *_realloc_r(struct _reent *rptr, Void *ptr, SizeT size)
{
    return realloc(ptr, size);
}

#endif


/*
 * ======== ti.sysbios.family.arm.v7r.tms570.Core TEMPLATE ========
 */


#define IMM_FLAG_REG                  0xFFF7AC18
#define IMM_WORD1_REG                 0xFFF7AC84
#define IMM_REG_RW32(X)               (*(volatile UInt32*)(X))


extern Void _c_int00();

Void ti_sysbios_family_arm_v7r_tms570_Core_resetC__I()
{
}

/*
 * ======== ti.sysbios.BIOS INITIALIZERS ========
 */

/* Module__state__V */
#if defined (__ICCARM__)
#pragma location = ".data_ti_sysbios_BIOS_Module__state__V"
#endif
#if defined(__llvm__) && defined(__ti__)
ti_sysbios_BIOS_Module_State__ ti_sysbios_BIOS_Module__state__V __attribute__ ((section(".data:ti_sysbios_BIOS_Module__state__V")));
#elif defined(__GNUC__) && !(defined(__MACH__) && defined(__APPLE__))
#ifndef __TI_COMPILER_VERSION__
ti_sysbios_BIOS_Module_State__ ti_sysbios_BIOS_Module__state__V __attribute__ ((section(".data_ti_sysbios_BIOS_Module__state__V")));
#endif
#endif
ti_sysbios_BIOS_Module_State__ ti_sysbios_BIOS_Module__state__V = {
    {
        (xdc_Bits32)0x0,  /* hi */
        (xdc_Bits32)0xbebc200,  /* lo */
    },  /* cpuFreq */
    (xdc_UInt)0x0,  /* rtsGateCount */
    ((xdc_IArg)(0x0)),  /* rtsGateKey */
    (ti_sysbios_BIOS_RtsGateProxy_Handle)&ti_sysbios_gates_GateMutex_Object__table__V[1],  /* rtsGate */
    ti_sysbios_BIOS_ThreadType_Main,  /* threadType */
    ((void*)0),  /* smpThreadType */
    ((xdc_Void(*)(xdc_Void))((xdc_Fxn)ti_sysbios_BIOS_startFunc)),  /* startFunc */
    ((xdc_Void(*)(xdc_Int))((xdc_Fxn)ti_sysbios_BIOS_exitFunc)),  /* exitFunc */
};

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_Module__diagsEnabled__C, ".const:ti_sysbios_BIOS_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_BIOS_Module__diagsEnabled ti_sysbios_BIOS_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_Module__diagsIncluded__C, ".const:ti_sysbios_BIOS_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_BIOS_Module__diagsIncluded ti_sysbios_BIOS_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_Module__diagsMask__C, ".const:ti_sysbios_BIOS_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_BIOS_Module__diagsMask ti_sysbios_BIOS_Module__diagsMask__C = ((CT__ti_sysbios_BIOS_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_Module__gateObj__C, ".const:ti_sysbios_BIOS_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_BIOS_Module__gateObj ti_sysbios_BIOS_Module__gateObj__C = ((CT__ti_sysbios_BIOS_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_Module__gatePrms__C, ".const:ti_sysbios_BIOS_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_BIOS_Module__gatePrms ti_sysbios_BIOS_Module__gatePrms__C = ((CT__ti_sysbios_BIOS_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_Module__id__C, ".const:ti_sysbios_BIOS_Module__id__C");
__FAR__ const CT__ti_sysbios_BIOS_Module__id ti_sysbios_BIOS_Module__id__C = (xdc_Bits16)0x8015;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_Module__loggerDefined__C, ".const:ti_sysbios_BIOS_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_BIOS_Module__loggerDefined ti_sysbios_BIOS_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_Module__loggerObj__C, ".const:ti_sysbios_BIOS_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_BIOS_Module__loggerObj ti_sysbios_BIOS_Module__loggerObj__C = ((CT__ti_sysbios_BIOS_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_Module__loggerFxn0__C, ".const:ti_sysbios_BIOS_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_BIOS_Module__loggerFxn0 ti_sysbios_BIOS_Module__loggerFxn0__C = ((CT__ti_sysbios_BIOS_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_Module__loggerFxn1__C, ".const:ti_sysbios_BIOS_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_BIOS_Module__loggerFxn1 ti_sysbios_BIOS_Module__loggerFxn1__C = ((CT__ti_sysbios_BIOS_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_Module__loggerFxn2__C, ".const:ti_sysbios_BIOS_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_BIOS_Module__loggerFxn2 ti_sysbios_BIOS_Module__loggerFxn2__C = ((CT__ti_sysbios_BIOS_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_Module__loggerFxn4__C, ".const:ti_sysbios_BIOS_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_BIOS_Module__loggerFxn4 ti_sysbios_BIOS_Module__loggerFxn4__C = ((CT__ti_sysbios_BIOS_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_Module__loggerFxn8__C, ".const:ti_sysbios_BIOS_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_BIOS_Module__loggerFxn8 ti_sysbios_BIOS_Module__loggerFxn8__C = ((CT__ti_sysbios_BIOS_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_Object__count__C, ".const:ti_sysbios_BIOS_Object__count__C");
__FAR__ const CT__ti_sysbios_BIOS_Object__count ti_sysbios_BIOS_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_Object__heap__C, ".const:ti_sysbios_BIOS_Object__heap__C");
__FAR__ const CT__ti_sysbios_BIOS_Object__heap ti_sysbios_BIOS_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_Object__sizeof__C, ".const:ti_sysbios_BIOS_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_BIOS_Object__sizeof ti_sysbios_BIOS_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_Object__table__C, ".const:ti_sysbios_BIOS_Object__table__C");
__FAR__ const CT__ti_sysbios_BIOS_Object__table ti_sysbios_BIOS_Object__table__C = 0;

/* smpEnabled__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_smpEnabled__C, ".const:ti_sysbios_BIOS_smpEnabled__C");
__FAR__ const CT__ti_sysbios_BIOS_smpEnabled ti_sysbios_BIOS_smpEnabled__C = 0;

/* mpeEnabled__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_mpeEnabled__C, ".const:ti_sysbios_BIOS_mpeEnabled__C");
__FAR__ const CT__ti_sysbios_BIOS_mpeEnabled ti_sysbios_BIOS_mpeEnabled__C = 0;

/* cpuFreq__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_cpuFreq__C, ".const:ti_sysbios_BIOS_cpuFreq__C");
__FAR__ const CT__ti_sysbios_BIOS_cpuFreq ti_sysbios_BIOS_cpuFreq__C = {
    (xdc_Bits32)0x0,  /* hi */
    (xdc_Bits32)0xbebc200,  /* lo */
};

/* runtimeCreatesEnabled__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_runtimeCreatesEnabled__C, ".const:ti_sysbios_BIOS_runtimeCreatesEnabled__C");
__FAR__ const CT__ti_sysbios_BIOS_runtimeCreatesEnabled ti_sysbios_BIOS_runtimeCreatesEnabled__C = 1;

/* taskEnabled__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_taskEnabled__C, ".const:ti_sysbios_BIOS_taskEnabled__C");
__FAR__ const CT__ti_sysbios_BIOS_taskEnabled ti_sysbios_BIOS_taskEnabled__C = 1;

/* swiEnabled__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_swiEnabled__C, ".const:ti_sysbios_BIOS_swiEnabled__C");
__FAR__ const CT__ti_sysbios_BIOS_swiEnabled ti_sysbios_BIOS_swiEnabled__C = 1;

/* clockEnabled__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_clockEnabled__C, ".const:ti_sysbios_BIOS_clockEnabled__C");
__FAR__ const CT__ti_sysbios_BIOS_clockEnabled ti_sysbios_BIOS_clockEnabled__C = 1;

/* defaultKernelHeapInstance__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_defaultKernelHeapInstance__C, ".const:ti_sysbios_BIOS_defaultKernelHeapInstance__C");
__FAR__ const CT__ti_sysbios_BIOS_defaultKernelHeapInstance ti_sysbios_BIOS_defaultKernelHeapInstance__C = 0;

/* kernelHeapSize__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_kernelHeapSize__C, ".const:ti_sysbios_BIOS_kernelHeapSize__C");
__FAR__ const CT__ti_sysbios_BIOS_kernelHeapSize ti_sysbios_BIOS_kernelHeapSize__C = (xdc_SizeT)0x1000;

/* kernelHeapSection__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_kernelHeapSection__C, ".const:ti_sysbios_BIOS_kernelHeapSection__C");
__FAR__ const CT__ti_sysbios_BIOS_kernelHeapSection ti_sysbios_BIOS_kernelHeapSection__C = ".kernel_heap";

/* heapSize__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_heapSize__C, ".const:ti_sysbios_BIOS_heapSize__C");
__FAR__ const CT__ti_sysbios_BIOS_heapSize ti_sysbios_BIOS_heapSize__C = (xdc_SizeT)0x1000;

/* heapSection__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_heapSection__C, ".const:ti_sysbios_BIOS_heapSection__C");
__FAR__ const CT__ti_sysbios_BIOS_heapSection ti_sysbios_BIOS_heapSection__C = 0;

/* heapTrackEnabled__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_heapTrackEnabled__C, ".const:ti_sysbios_BIOS_heapTrackEnabled__C");
__FAR__ const CT__ti_sysbios_BIOS_heapTrackEnabled ti_sysbios_BIOS_heapTrackEnabled__C = 0;

/* setupSecureContext__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_setupSecureContext__C, ".const:ti_sysbios_BIOS_setupSecureContext__C");
__FAR__ const CT__ti_sysbios_BIOS_setupSecureContext ti_sysbios_BIOS_setupSecureContext__C = 0;

/* useSK__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_useSK__C, ".const:ti_sysbios_BIOS_useSK__C");
__FAR__ const CT__ti_sysbios_BIOS_useSK ti_sysbios_BIOS_useSK__C = 0;

/* installedErrorHook__C */
#pragma DATA_SECTION(ti_sysbios_BIOS_installedErrorHook__C, ".const:ti_sysbios_BIOS_installedErrorHook__C");
__FAR__ const CT__ti_sysbios_BIOS_installedErrorHook ti_sysbios_BIOS_installedErrorHook__C = ((CT__ti_sysbios_BIOS_installedErrorHook)((xdc_Fxn)xdc_runtime_Error_print__E));


/*
 * ======== ti.sysbios.BIOS_RtsGateProxy INITIALIZERS ========
 */


/*
 * ======== ti.sysbios.family.arm.IntrinsicsSupport INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_IntrinsicsSupport_Module__diagsEnabled__C, ".const:ti_sysbios_family_arm_IntrinsicsSupport_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_family_arm_IntrinsicsSupport_Module__diagsEnabled ti_sysbios_family_arm_IntrinsicsSupport_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_IntrinsicsSupport_Module__diagsIncluded__C, ".const:ti_sysbios_family_arm_IntrinsicsSupport_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_family_arm_IntrinsicsSupport_Module__diagsIncluded ti_sysbios_family_arm_IntrinsicsSupport_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_IntrinsicsSupport_Module__diagsMask__C, ".const:ti_sysbios_family_arm_IntrinsicsSupport_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_family_arm_IntrinsicsSupport_Module__diagsMask ti_sysbios_family_arm_IntrinsicsSupport_Module__diagsMask__C = ((CT__ti_sysbios_family_arm_IntrinsicsSupport_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_IntrinsicsSupport_Module__gateObj__C, ".const:ti_sysbios_family_arm_IntrinsicsSupport_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_family_arm_IntrinsicsSupport_Module__gateObj ti_sysbios_family_arm_IntrinsicsSupport_Module__gateObj__C = ((CT__ti_sysbios_family_arm_IntrinsicsSupport_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_IntrinsicsSupport_Module__gatePrms__C, ".const:ti_sysbios_family_arm_IntrinsicsSupport_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_family_arm_IntrinsicsSupport_Module__gatePrms ti_sysbios_family_arm_IntrinsicsSupport_Module__gatePrms__C = ((CT__ti_sysbios_family_arm_IntrinsicsSupport_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_IntrinsicsSupport_Module__id__C, ".const:ti_sysbios_family_arm_IntrinsicsSupport_Module__id__C");
__FAR__ const CT__ti_sysbios_family_arm_IntrinsicsSupport_Module__id ti_sysbios_family_arm_IntrinsicsSupport_Module__id__C = (xdc_Bits16)0x8013;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerDefined__C, ".const:ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerDefined ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerObj__C, ".const:ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerObj ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerObj__C = ((CT__ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn0__C, ".const:ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn0 ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn0__C = ((CT__ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn1__C, ".const:ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn1 ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn1__C = ((CT__ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn2__C, ".const:ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn2 ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn2__C = ((CT__ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn4__C, ".const:ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn4 ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn4__C = ((CT__ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn8__C, ".const:ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn8 ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn8__C = ((CT__ti_sysbios_family_arm_IntrinsicsSupport_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_IntrinsicsSupport_Object__count__C, ".const:ti_sysbios_family_arm_IntrinsicsSupport_Object__count__C");
__FAR__ const CT__ti_sysbios_family_arm_IntrinsicsSupport_Object__count ti_sysbios_family_arm_IntrinsicsSupport_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_IntrinsicsSupport_Object__heap__C, ".const:ti_sysbios_family_arm_IntrinsicsSupport_Object__heap__C");
__FAR__ const CT__ti_sysbios_family_arm_IntrinsicsSupport_Object__heap ti_sysbios_family_arm_IntrinsicsSupport_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_IntrinsicsSupport_Object__sizeof__C, ".const:ti_sysbios_family_arm_IntrinsicsSupport_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_family_arm_IntrinsicsSupport_Object__sizeof ti_sysbios_family_arm_IntrinsicsSupport_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_IntrinsicsSupport_Object__table__C, ".const:ti_sysbios_family_arm_IntrinsicsSupport_Object__table__C");
__FAR__ const CT__ti_sysbios_family_arm_IntrinsicsSupport_Object__table ti_sysbios_family_arm_IntrinsicsSupport_Object__table__C = 0;


/*
 * ======== ti.sysbios.family.arm.TaskSupport INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_TaskSupport_Module__diagsEnabled__C, ".const:ti_sysbios_family_arm_TaskSupport_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_family_arm_TaskSupport_Module__diagsEnabled ti_sysbios_family_arm_TaskSupport_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_TaskSupport_Module__diagsIncluded__C, ".const:ti_sysbios_family_arm_TaskSupport_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_family_arm_TaskSupport_Module__diagsIncluded ti_sysbios_family_arm_TaskSupport_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_TaskSupport_Module__diagsMask__C, ".const:ti_sysbios_family_arm_TaskSupport_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_family_arm_TaskSupport_Module__diagsMask ti_sysbios_family_arm_TaskSupport_Module__diagsMask__C = ((CT__ti_sysbios_family_arm_TaskSupport_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_TaskSupport_Module__gateObj__C, ".const:ti_sysbios_family_arm_TaskSupport_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_family_arm_TaskSupport_Module__gateObj ti_sysbios_family_arm_TaskSupport_Module__gateObj__C = ((CT__ti_sysbios_family_arm_TaskSupport_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_TaskSupport_Module__gatePrms__C, ".const:ti_sysbios_family_arm_TaskSupport_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_family_arm_TaskSupport_Module__gatePrms ti_sysbios_family_arm_TaskSupport_Module__gatePrms__C = ((CT__ti_sysbios_family_arm_TaskSupport_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_TaskSupport_Module__id__C, ".const:ti_sysbios_family_arm_TaskSupport_Module__id__C");
__FAR__ const CT__ti_sysbios_family_arm_TaskSupport_Module__id ti_sysbios_family_arm_TaskSupport_Module__id__C = (xdc_Bits16)0x8014;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_TaskSupport_Module__loggerDefined__C, ".const:ti_sysbios_family_arm_TaskSupport_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_family_arm_TaskSupport_Module__loggerDefined ti_sysbios_family_arm_TaskSupport_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_TaskSupport_Module__loggerObj__C, ".const:ti_sysbios_family_arm_TaskSupport_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_family_arm_TaskSupport_Module__loggerObj ti_sysbios_family_arm_TaskSupport_Module__loggerObj__C = ((CT__ti_sysbios_family_arm_TaskSupport_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_TaskSupport_Module__loggerFxn0__C, ".const:ti_sysbios_family_arm_TaskSupport_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_family_arm_TaskSupport_Module__loggerFxn0 ti_sysbios_family_arm_TaskSupport_Module__loggerFxn0__C = ((CT__ti_sysbios_family_arm_TaskSupport_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_TaskSupport_Module__loggerFxn1__C, ".const:ti_sysbios_family_arm_TaskSupport_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_family_arm_TaskSupport_Module__loggerFxn1 ti_sysbios_family_arm_TaskSupport_Module__loggerFxn1__C = ((CT__ti_sysbios_family_arm_TaskSupport_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_TaskSupport_Module__loggerFxn2__C, ".const:ti_sysbios_family_arm_TaskSupport_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_family_arm_TaskSupport_Module__loggerFxn2 ti_sysbios_family_arm_TaskSupport_Module__loggerFxn2__C = ((CT__ti_sysbios_family_arm_TaskSupport_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_TaskSupport_Module__loggerFxn4__C, ".const:ti_sysbios_family_arm_TaskSupport_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_family_arm_TaskSupport_Module__loggerFxn4 ti_sysbios_family_arm_TaskSupport_Module__loggerFxn4__C = ((CT__ti_sysbios_family_arm_TaskSupport_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_TaskSupport_Module__loggerFxn8__C, ".const:ti_sysbios_family_arm_TaskSupport_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_family_arm_TaskSupport_Module__loggerFxn8 ti_sysbios_family_arm_TaskSupport_Module__loggerFxn8__C = ((CT__ti_sysbios_family_arm_TaskSupport_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_TaskSupport_Object__count__C, ".const:ti_sysbios_family_arm_TaskSupport_Object__count__C");
__FAR__ const CT__ti_sysbios_family_arm_TaskSupport_Object__count ti_sysbios_family_arm_TaskSupport_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_TaskSupport_Object__heap__C, ".const:ti_sysbios_family_arm_TaskSupport_Object__heap__C");
__FAR__ const CT__ti_sysbios_family_arm_TaskSupport_Object__heap ti_sysbios_family_arm_TaskSupport_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_TaskSupport_Object__sizeof__C, ".const:ti_sysbios_family_arm_TaskSupport_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_family_arm_TaskSupport_Object__sizeof ti_sysbios_family_arm_TaskSupport_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_TaskSupport_Object__table__C, ".const:ti_sysbios_family_arm_TaskSupport_Object__table__C");
__FAR__ const CT__ti_sysbios_family_arm_TaskSupport_Object__table ti_sysbios_family_arm_TaskSupport_Object__table__C = 0;

/* defaultStackSize__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_TaskSupport_defaultStackSize__C, ".const:ti_sysbios_family_arm_TaskSupport_defaultStackSize__C");
__FAR__ const CT__ti_sysbios_family_arm_TaskSupport_defaultStackSize ti_sysbios_family_arm_TaskSupport_defaultStackSize__C = (xdc_SizeT)0x800;

/* stackAlignment__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_TaskSupport_stackAlignment__C, ".const:ti_sysbios_family_arm_TaskSupport_stackAlignment__C");
__FAR__ const CT__ti_sysbios_family_arm_TaskSupport_stackAlignment ti_sysbios_family_arm_TaskSupport_stackAlignment__C = (xdc_UInt)0x8;


/*
 * ======== ti.sysbios.family.arm.exc.Exception INITIALIZERS ========
 */

/* --> ti_sysbios_family_arm_exc_Exception_Module_State_0_excActive__A */
__T1_ti_sysbios_family_arm_exc_Exception_Module_State__excActive ti_sysbios_family_arm_exc_Exception_Module_State_0_excActive__A[1] = {
    0,  /* [0] */
};

/* --> ti_sysbios_family_arm_exc_Exception_Module_State_0_excContext__A */
__T1_ti_sysbios_family_arm_exc_Exception_Module_State__excContext ti_sysbios_family_arm_exc_Exception_Module_State_0_excContext__A[1] = {
    ((ti_sysbios_family_arm_exc_Exception_ExcContext*)0),  /* [0] */
};

/* --> ti_sysbios_family_arm_exc_Exception_Module_State_0_excStackBuffers__A */
__T1_ti_sysbios_family_arm_exc_Exception_Module_State__excStackBuffers ti_sysbios_family_arm_exc_Exception_Module_State_0_excStackBuffers__A[1] = {
    ((xdc_Ptr)0),  /* [0] */
};

/* --> ti_sysbios_family_arm_exc_Exception_Module_State_0_excStack_0__A */
__T1_ti_sysbios_family_arm_exc_Exception_Module_State__excStack ti_sysbios_family_arm_exc_Exception_Module_State_0_excStack_0__A[4096];

/* --> ti_sysbios_family_arm_exc_Exception_Module_State_0_excStack__A */
__T2_ti_sysbios_family_arm_exc_Exception_Module_State__excStack ti_sysbios_family_arm_exc_Exception_Module_State_0_excStack__A[1] = {
    ((void*)ti_sysbios_family_arm_exc_Exception_Module_State_0_excStack_0__A),  /* [0] */
};

/* Module__state__V */
#if defined (__ICCARM__)
#pragma location = ".data_ti_sysbios_family_arm_exc_Exception_Module__state__V"
#endif
#if defined(__llvm__) && defined(__ti__)
ti_sysbios_family_arm_exc_Exception_Module_State__ ti_sysbios_family_arm_exc_Exception_Module__state__V __attribute__ ((section(".data:ti_sysbios_family_arm_exc_Exception_Module__state__V")));
#elif defined(__GNUC__) && !(defined(__MACH__) && defined(__APPLE__))
#ifndef __TI_COMPILER_VERSION__
ti_sysbios_family_arm_exc_Exception_Module_State__ ti_sysbios_family_arm_exc_Exception_Module__state__V __attribute__ ((section(".data_ti_sysbios_family_arm_exc_Exception_Module__state__V")));
#endif
#endif
ti_sysbios_family_arm_exc_Exception_Module_State__ ti_sysbios_family_arm_exc_Exception_Module__state__V = {
    ((void*)ti_sysbios_family_arm_exc_Exception_Module_State_0_excActive__A),  /* excActive */
    ((void*)ti_sysbios_family_arm_exc_Exception_Module_State_0_excContext__A),  /* excContext */
    ((void*)ti_sysbios_family_arm_exc_Exception_Module_State_0_excStackBuffers__A),  /* excStackBuffers */
    ((void*)ti_sysbios_family_arm_exc_Exception_Module_State_0_excStack__A),  /* excStack */
    (xdc_SizeT)0x1000,  /* excStackSize */
};

/* --> ti_sysbios_family_arm_exc_Exception_excHookFuncs__A */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_excHookFuncs__A, ".const:ti_sysbios_family_arm_exc_Exception_excHookFuncs__A");
const __T1_ti_sysbios_family_arm_exc_Exception_excHookFuncs ti_sysbios_family_arm_exc_Exception_excHookFuncs__A[1] = {
    ((xdc_Void(*)(ti_sysbios_family_arm_exc_Exception_ExcContext*))0),  /* [0] */
};

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_Module__diagsEnabled__C, ".const:ti_sysbios_family_arm_exc_Exception_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__diagsEnabled ti_sysbios_family_arm_exc_Exception_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_Module__diagsIncluded__C, ".const:ti_sysbios_family_arm_exc_Exception_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__diagsIncluded ti_sysbios_family_arm_exc_Exception_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_Module__diagsMask__C, ".const:ti_sysbios_family_arm_exc_Exception_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__diagsMask ti_sysbios_family_arm_exc_Exception_Module__diagsMask__C = ((CT__ti_sysbios_family_arm_exc_Exception_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_Module__gateObj__C, ".const:ti_sysbios_family_arm_exc_Exception_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__gateObj ti_sysbios_family_arm_exc_Exception_Module__gateObj__C = ((CT__ti_sysbios_family_arm_exc_Exception_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_Module__gatePrms__C, ".const:ti_sysbios_family_arm_exc_Exception_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__gatePrms ti_sysbios_family_arm_exc_Exception_Module__gatePrms__C = ((CT__ti_sysbios_family_arm_exc_Exception_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_Module__id__C, ".const:ti_sysbios_family_arm_exc_Exception_Module__id__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__id ti_sysbios_family_arm_exc_Exception_Module__id__C = (xdc_Bits16)0x8026;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_Module__loggerDefined__C, ".const:ti_sysbios_family_arm_exc_Exception_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__loggerDefined ti_sysbios_family_arm_exc_Exception_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_Module__loggerObj__C, ".const:ti_sysbios_family_arm_exc_Exception_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__loggerObj ti_sysbios_family_arm_exc_Exception_Module__loggerObj__C = ((CT__ti_sysbios_family_arm_exc_Exception_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_Module__loggerFxn0__C, ".const:ti_sysbios_family_arm_exc_Exception_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__loggerFxn0 ti_sysbios_family_arm_exc_Exception_Module__loggerFxn0__C = ((CT__ti_sysbios_family_arm_exc_Exception_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_Module__loggerFxn1__C, ".const:ti_sysbios_family_arm_exc_Exception_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__loggerFxn1 ti_sysbios_family_arm_exc_Exception_Module__loggerFxn1__C = ((CT__ti_sysbios_family_arm_exc_Exception_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_Module__loggerFxn2__C, ".const:ti_sysbios_family_arm_exc_Exception_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__loggerFxn2 ti_sysbios_family_arm_exc_Exception_Module__loggerFxn2__C = ((CT__ti_sysbios_family_arm_exc_Exception_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_Module__loggerFxn4__C, ".const:ti_sysbios_family_arm_exc_Exception_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__loggerFxn4 ti_sysbios_family_arm_exc_Exception_Module__loggerFxn4__C = ((CT__ti_sysbios_family_arm_exc_Exception_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_Module__loggerFxn8__C, ".const:ti_sysbios_family_arm_exc_Exception_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__loggerFxn8 ti_sysbios_family_arm_exc_Exception_Module__loggerFxn8__C = ((CT__ti_sysbios_family_arm_exc_Exception_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_Object__count__C, ".const:ti_sysbios_family_arm_exc_Exception_Object__count__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Object__count ti_sysbios_family_arm_exc_Exception_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_Object__heap__C, ".const:ti_sysbios_family_arm_exc_Exception_Object__heap__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Object__heap ti_sysbios_family_arm_exc_Exception_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_Object__sizeof__C, ".const:ti_sysbios_family_arm_exc_Exception_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Object__sizeof ti_sysbios_family_arm_exc_Exception_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_Object__table__C, ".const:ti_sysbios_family_arm_exc_Exception_Object__table__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Object__table ti_sysbios_family_arm_exc_Exception_Object__table__C = 0;

/* E_swi__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_E_swi__C, ".const:ti_sysbios_family_arm_exc_Exception_E_swi__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_E_swi ti_sysbios_family_arm_exc_Exception_E_swi__C = (((xdc_runtime_Error_Id)4814) << 16 | 0);

/* E_prefetchAbort__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_E_prefetchAbort__C, ".const:ti_sysbios_family_arm_exc_Exception_E_prefetchAbort__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_E_prefetchAbort ti_sysbios_family_arm_exc_Exception_E_prefetchAbort__C = (((xdc_runtime_Error_Id)4847) << 16 | 0);

/* E_dataAbort__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_E_dataAbort__C, ".const:ti_sysbios_family_arm_exc_Exception_E_dataAbort__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_E_dataAbort ti_sysbios_family_arm_exc_Exception_E_dataAbort__C = (((xdc_runtime_Error_Id)4890) << 16 | 0);

/* E_undefinedInstruction__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_E_undefinedInstruction__C, ".const:ti_sysbios_family_arm_exc_Exception_E_undefinedInstruction__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_E_undefinedInstruction ti_sysbios_family_arm_exc_Exception_E_undefinedInstruction__C = (((xdc_runtime_Error_Id)4929) << 16 | 0);

/* enableDecode__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_enableDecode__C, ".const:ti_sysbios_family_arm_exc_Exception_enableDecode__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_enableDecode ti_sysbios_family_arm_exc_Exception_enableDecode__C = 1;

/* excHookFunc__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_excHookFunc__C, ".const:ti_sysbios_family_arm_exc_Exception_excHookFunc__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_excHookFunc ti_sysbios_family_arm_exc_Exception_excHookFunc__C = ((CT__ti_sysbios_family_arm_exc_Exception_excHookFunc)0);

/* excHookFuncs__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_exc_Exception_excHookFuncs__C, ".const:ti_sysbios_family_arm_exc_Exception_excHookFuncs__C");
__FAR__ const CT__ti_sysbios_family_arm_exc_Exception_excHookFuncs ti_sysbios_family_arm_exc_Exception_excHookFuncs__C = ((CT__ti_sysbios_family_arm_exc_Exception_excHookFuncs)ti_sysbios_family_arm_exc_Exception_excHookFuncs__A);


/*
 * ======== ti.sysbios.family.arm.v7r.tms570.Core INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_tms570_Core_Module__diagsEnabled__C, ".const:ti_sysbios_family_arm_v7r_tms570_Core_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_tms570_Core_Module__diagsEnabled ti_sysbios_family_arm_v7r_tms570_Core_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_tms570_Core_Module__diagsIncluded__C, ".const:ti_sysbios_family_arm_v7r_tms570_Core_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_tms570_Core_Module__diagsIncluded ti_sysbios_family_arm_v7r_tms570_Core_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_tms570_Core_Module__diagsMask__C, ".const:ti_sysbios_family_arm_v7r_tms570_Core_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_tms570_Core_Module__diagsMask ti_sysbios_family_arm_v7r_tms570_Core_Module__diagsMask__C = ((CT__ti_sysbios_family_arm_v7r_tms570_Core_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_tms570_Core_Module__gateObj__C, ".const:ti_sysbios_family_arm_v7r_tms570_Core_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_tms570_Core_Module__gateObj ti_sysbios_family_arm_v7r_tms570_Core_Module__gateObj__C = ((CT__ti_sysbios_family_arm_v7r_tms570_Core_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_tms570_Core_Module__gatePrms__C, ".const:ti_sysbios_family_arm_v7r_tms570_Core_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_tms570_Core_Module__gatePrms ti_sysbios_family_arm_v7r_tms570_Core_Module__gatePrms__C = ((CT__ti_sysbios_family_arm_v7r_tms570_Core_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_tms570_Core_Module__id__C, ".const:ti_sysbios_family_arm_v7r_tms570_Core_Module__id__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_tms570_Core_Module__id ti_sysbios_family_arm_v7r_tms570_Core_Module__id__C = (xdc_Bits16)0x8030;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerDefined__C, ".const:ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerDefined ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerObj__C, ".const:ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerObj ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerObj__C = ((CT__ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn0__C, ".const:ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn0 ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn0__C = ((CT__ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn1__C, ".const:ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn1 ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn1__C = ((CT__ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn2__C, ".const:ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn2 ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn2__C = ((CT__ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn4__C, ".const:ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn4 ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn4__C = ((CT__ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn8__C, ".const:ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn8 ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn8__C = ((CT__ti_sysbios_family_arm_v7r_tms570_Core_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_tms570_Core_Object__count__C, ".const:ti_sysbios_family_arm_v7r_tms570_Core_Object__count__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_tms570_Core_Object__count ti_sysbios_family_arm_v7r_tms570_Core_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_tms570_Core_Object__heap__C, ".const:ti_sysbios_family_arm_v7r_tms570_Core_Object__heap__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_tms570_Core_Object__heap ti_sysbios_family_arm_v7r_tms570_Core_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_tms570_Core_Object__sizeof__C, ".const:ti_sysbios_family_arm_v7r_tms570_Core_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_tms570_Core_Object__sizeof ti_sysbios_family_arm_v7r_tms570_Core_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_tms570_Core_Object__table__C, ".const:ti_sysbios_family_arm_v7r_tms570_Core_Object__table__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_tms570_Core_Object__table ti_sysbios_family_arm_v7r_tms570_Core_Object__table__C = 0;

/* numCores__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_tms570_Core_numCores__C, ".const:ti_sysbios_family_arm_v7r_tms570_Core_numCores__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_tms570_Core_numCores ti_sysbios_family_arm_v7r_tms570_Core_numCores__C = (xdc_UInt)0x1;

/* E_mismatchedIds__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_tms570_Core_E_mismatchedIds__C, ".const:ti_sysbios_family_arm_v7r_tms570_Core_E_mismatchedIds__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_tms570_Core_E_mismatchedIds ti_sysbios_family_arm_v7r_tms570_Core_E_mismatchedIds__C = (((xdc_runtime_Error_Id)5058) << 16 | 0);

/* id__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_tms570_Core_id__C, ".const:ti_sysbios_family_arm_v7r_tms570_Core_id__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_tms570_Core_id ti_sysbios_family_arm_v7r_tms570_Core_id__C = (xdc_UInt)0x0;


/*
 * ======== ti.sysbios.family.arm.v7r.vim.Hwi INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_family_arm_v7r_vim_Hwi_Object__DESC__C;

/* Object__PARAMS__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_Object__PARAMS__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_Object__PARAMS__C");
__FAR__ const ti_sysbios_family_arm_v7r_vim_Hwi_Params ti_sysbios_family_arm_v7r_vim_Hwi_Object__PARAMS__C = {
    sizeof (ti_sysbios_family_arm_v7r_vim_Hwi_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&ti_sysbios_family_arm_v7r_vim_Hwi_Object__PARAMS__C.__iprms, /* instance */
    ti_sysbios_interfaces_IHwi_MaskingOption_LOWER,  /* maskSetting */
    ((xdc_UArg)(0x0)),  /* arg */
    1,  /* enableInt */
    (xdc_Int)(-0x0 - 1),  /* eventId */
    (xdc_Int)(-0x0 - 1),  /* priority */
    ti_sysbios_family_arm_v7r_vim_Hwi_Type_IRQ,  /* type */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
ti_sysbios_family_arm_v7r_vim_Hwi_Module__ ti_sysbios_family_arm_v7r_vim_Hwi_Module__root__V = {
    {&ti_sysbios_family_arm_v7r_vim_Hwi_Module__root__V.link,  /* link.next */
    &ti_sysbios_family_arm_v7r_vim_Hwi_Module__root__V.link},  /* link.prev */
};

/* Object__table__V */
ti_sysbios_family_arm_v7r_vim_Hwi_Object__ ti_sysbios_family_arm_v7r_vim_Hwi_Object__table__V[1] = {
    {/* instance#0 */
        0,
        ti_sysbios_family_arm_v7r_vim_Hwi_Type_IRQ,  /* type */
        ((xdc_UArg)((void*)(ti_sysbios_timers_rti_Timer_Handle)&ti_sysbios_timers_rti_Timer_Object__table__V[0])),  /* arg */
        ((xdc_Void(*)(xdc_UArg))((xdc_Fxn)ti_sysbios_timers_rti_Timer_periodicStub__E)),  /* fxn */
        (xdc_Int)0x2,  /* intNum */
        ((xdc_UArg)0),  /* irp */
        ((void*)0),  /* hookEnv */
        {
            (xdc_UInt)0x4,  /* [0] */
            (xdc_UInt)0x0,  /* [1] */
            (xdc_UInt)0x0,  /* [2] */
            (xdc_UInt)0x0,  /* [3] */
        },  /* disableMask */
    },
};

/* --> ti_sysbios_family_arm_v7r_vim_Hwi_Module_State_0_fiqStack__A */
__T1_ti_sysbios_family_arm_v7r_vim_Hwi_Module_State__fiqStack ti_sysbios_family_arm_v7r_vim_Hwi_Module_State_0_fiqStack__A[2048];

/* --> ti_sysbios_family_arm_v7r_vim_Hwi_Module_State_0_dispatchTable__A */
__T1_ti_sysbios_family_arm_v7r_vim_Hwi_Module_State__dispatchTable ti_sysbios_family_arm_v7r_vim_Hwi_Module_State_0_dispatchTable__A[128] = {
    0,  /* [0] */
    0,  /* [1] */
    (ti_sysbios_family_arm_v7r_vim_Hwi_Handle)&ti_sysbios_family_arm_v7r_vim_Hwi_Object__table__V[0],  /* [2] */
    0,  /* [3] */
    0,  /* [4] */
    0,  /* [5] */
    0,  /* [6] */
    0,  /* [7] */
    0,  /* [8] */
    0,  /* [9] */
    0,  /* [10] */
    0,  /* [11] */
    0,  /* [12] */
    0,  /* [13] */
    0,  /* [14] */
    0,  /* [15] */
    0,  /* [16] */
    0,  /* [17] */
    0,  /* [18] */
    0,  /* [19] */
    0,  /* [20] */
    0,  /* [21] */
    0,  /* [22] */
    0,  /* [23] */
    0,  /* [24] */
    0,  /* [25] */
    0,  /* [26] */
    0,  /* [27] */
    0,  /* [28] */
    0,  /* [29] */
    0,  /* [30] */
    0,  /* [31] */
    0,  /* [32] */
    0,  /* [33] */
    0,  /* [34] */
    0,  /* [35] */
    0,  /* [36] */
    0,  /* [37] */
    0,  /* [38] */
    0,  /* [39] */
    0,  /* [40] */
    0,  /* [41] */
    0,  /* [42] */
    0,  /* [43] */
    0,  /* [44] */
    0,  /* [45] */
    0,  /* [46] */
    0,  /* [47] */
    0,  /* [48] */
    0,  /* [49] */
    0,  /* [50] */
    0,  /* [51] */
    0,  /* [52] */
    0,  /* [53] */
    0,  /* [54] */
    0,  /* [55] */
    0,  /* [56] */
    0,  /* [57] */
    0,  /* [58] */
    0,  /* [59] */
    0,  /* [60] */
    0,  /* [61] */
    0,  /* [62] */
    0,  /* [63] */
    0,  /* [64] */
    0,  /* [65] */
    0,  /* [66] */
    0,  /* [67] */
    0,  /* [68] */
    0,  /* [69] */
    0,  /* [70] */
    0,  /* [71] */
    0,  /* [72] */
    0,  /* [73] */
    0,  /* [74] */
    0,  /* [75] */
    0,  /* [76] */
    0,  /* [77] */
    0,  /* [78] */
    0,  /* [79] */
    0,  /* [80] */
    0,  /* [81] */
    0,  /* [82] */
    0,  /* [83] */
    0,  /* [84] */
    0,  /* [85] */
    0,  /* [86] */
    0,  /* [87] */
    0,  /* [88] */
    0,  /* [89] */
    0,  /* [90] */
    0,  /* [91] */
    0,  /* [92] */
    0,  /* [93] */
    0,  /* [94] */
    0,  /* [95] */
    0,  /* [96] */
    0,  /* [97] */
    0,  /* [98] */
    0,  /* [99] */
    0,  /* [100] */
    0,  /* [101] */
    0,  /* [102] */
    0,  /* [103] */
    0,  /* [104] */
    0,  /* [105] */
    0,  /* [106] */
    0,  /* [107] */
    0,  /* [108] */
    0,  /* [109] */
    0,  /* [110] */
    0,  /* [111] */
    0,  /* [112] */
    0,  /* [113] */
    0,  /* [114] */
    0,  /* [115] */
    0,  /* [116] */
    0,  /* [117] */
    0,  /* [118] */
    0,  /* [119] */
    0,  /* [120] */
    0,  /* [121] */
    0,  /* [122] */
    0,  /* [123] */
    0,  /* [124] */
    0,  /* [125] */
    0,  /* [126] */
    0,  /* [127] */
};

/* Module__state__V */
#if defined (__ICCARM__)
#pragma location = ".data_ti_sysbios_family_arm_v7r_vim_Hwi_Module__state__V"
#endif
#if defined(__llvm__) && defined(__ti__)
ti_sysbios_family_arm_v7r_vim_Hwi_Module_State__ ti_sysbios_family_arm_v7r_vim_Hwi_Module__state__V __attribute__ ((section(".data:ti_sysbios_family_arm_v7r_vim_Hwi_Module__state__V")));
#elif defined(__GNUC__) && !(defined(__MACH__) && defined(__APPLE__))
#ifndef __TI_COMPILER_VERSION__
ti_sysbios_family_arm_v7r_vim_Hwi_Module_State__ ti_sysbios_family_arm_v7r_vim_Hwi_Module__state__V __attribute__ ((section(".data_ti_sysbios_family_arm_v7r_vim_Hwi_Module__state__V")));
#endif
#endif
ti_sysbios_family_arm_v7r_vim_Hwi_Module_State__ ti_sysbios_family_arm_v7r_vim_Hwi_Module__state__V = {
    ((xdc_Char*)0),  /* taskSP */
    ((xdc_Char*)0),  /* isrStack */
    ((xdc_Ptr)((void*)&__TI_STACK_BASE)),  /* isrStackBase */
    ((xdc_Ptr)((void*)&__TI_STACK_SIZE)),  /* isrStackSize */
    ((void*)ti_sysbios_family_arm_v7r_vim_Hwi_Module_State_0_fiqStack__A),  /* fiqStack */
    (xdc_SizeT)0x800,  /* fiqStackSize */
    ((xdc_UInt*)(0xfff82000)),  /* vimRam */
    ((void*)ti_sysbios_family_arm_v7r_vim_Hwi_Module_State_0_dispatchTable__A),  /* dispatchTable */
    {
        (xdc_UInt)0xffffffff,  /* [0] */
        (xdc_UInt)0xffffffff,  /* [1] */
        (xdc_UInt)0xffffffff,  /* [2] */
        (xdc_UInt)0xffffffff,  /* [3] */
    },  /* zeroLatencyFIQMask */
    (xdc_UInt)0x0,  /* spuriousInts */
};

/* --> ti_sysbios_family_arm_v7r_vim_Hwi_channelMap__A */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_channelMap__A, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_channelMap__A");
const __T1_ti_sysbios_family_arm_v7r_vim_Hwi_channelMap ti_sysbios_family_arm_v7r_vim_Hwi_channelMap__A[128] = {
    (xdc_UInt8)0x0,  /* [0] */
    (xdc_UInt8)0x1,  /* [1] */
    (xdc_UInt8)0x2,  /* [2] */
    (xdc_UInt8)0x3,  /* [3] */
    (xdc_UInt8)0x4,  /* [4] */
    (xdc_UInt8)0x5,  /* [5] */
    (xdc_UInt8)0x6,  /* [6] */
    (xdc_UInt8)0x7,  /* [7] */
    (xdc_UInt8)0x8,  /* [8] */
    (xdc_UInt8)0x9,  /* [9] */
    (xdc_UInt8)0xa,  /* [10] */
    (xdc_UInt8)0xb,  /* [11] */
    (xdc_UInt8)0xc,  /* [12] */
    (xdc_UInt8)0xd,  /* [13] */
    (xdc_UInt8)0xe,  /* [14] */
    (xdc_UInt8)0xf,  /* [15] */
    (xdc_UInt8)0x10,  /* [16] */
    (xdc_UInt8)0x11,  /* [17] */
    (xdc_UInt8)0x12,  /* [18] */
    (xdc_UInt8)0x13,  /* [19] */
    (xdc_UInt8)0x14,  /* [20] */
    (xdc_UInt8)0x15,  /* [21] */
    (xdc_UInt8)0x16,  /* [22] */
    (xdc_UInt8)0x17,  /* [23] */
    (xdc_UInt8)0x18,  /* [24] */
    (xdc_UInt8)0x19,  /* [25] */
    (xdc_UInt8)0x1a,  /* [26] */
    (xdc_UInt8)0x1b,  /* [27] */
    (xdc_UInt8)0x1c,  /* [28] */
    (xdc_UInt8)0x1d,  /* [29] */
    (xdc_UInt8)0x1e,  /* [30] */
    (xdc_UInt8)0x1f,  /* [31] */
    (xdc_UInt8)0x20,  /* [32] */
    (xdc_UInt8)0x21,  /* [33] */
    (xdc_UInt8)0x22,  /* [34] */
    (xdc_UInt8)0x23,  /* [35] */
    (xdc_UInt8)0x24,  /* [36] */
    (xdc_UInt8)0x25,  /* [37] */
    (xdc_UInt8)0x26,  /* [38] */
    (xdc_UInt8)0x27,  /* [39] */
    (xdc_UInt8)0x28,  /* [40] */
    (xdc_UInt8)0x29,  /* [41] */
    (xdc_UInt8)0x2a,  /* [42] */
    (xdc_UInt8)0x2b,  /* [43] */
    (xdc_UInt8)0x2c,  /* [44] */
    (xdc_UInt8)0x2d,  /* [45] */
    (xdc_UInt8)0x2e,  /* [46] */
    (xdc_UInt8)0x2f,  /* [47] */
    (xdc_UInt8)0x30,  /* [48] */
    (xdc_UInt8)0x31,  /* [49] */
    (xdc_UInt8)0x32,  /* [50] */
    (xdc_UInt8)0x33,  /* [51] */
    (xdc_UInt8)0x34,  /* [52] */
    (xdc_UInt8)0x35,  /* [53] */
    (xdc_UInt8)0x36,  /* [54] */
    (xdc_UInt8)0x37,  /* [55] */
    (xdc_UInt8)0x38,  /* [56] */
    (xdc_UInt8)0x39,  /* [57] */
    (xdc_UInt8)0x3a,  /* [58] */
    (xdc_UInt8)0x3b,  /* [59] */
    (xdc_UInt8)0x3c,  /* [60] */
    (xdc_UInt8)0x3d,  /* [61] */
    (xdc_UInt8)0x3e,  /* [62] */
    (xdc_UInt8)0x3f,  /* [63] */
    (xdc_UInt8)0x40,  /* [64] */
    (xdc_UInt8)0x41,  /* [65] */
    (xdc_UInt8)0x42,  /* [66] */
    (xdc_UInt8)0x43,  /* [67] */
    (xdc_UInt8)0x44,  /* [68] */
    (xdc_UInt8)0x45,  /* [69] */
    (xdc_UInt8)0x46,  /* [70] */
    (xdc_UInt8)0x47,  /* [71] */
    (xdc_UInt8)0x48,  /* [72] */
    (xdc_UInt8)0x49,  /* [73] */
    (xdc_UInt8)0x4a,  /* [74] */
    (xdc_UInt8)0x4b,  /* [75] */
    (xdc_UInt8)0x4c,  /* [76] */
    (xdc_UInt8)0x4d,  /* [77] */
    (xdc_UInt8)0x4e,  /* [78] */
    (xdc_UInt8)0x4f,  /* [79] */
    (xdc_UInt8)0x50,  /* [80] */
    (xdc_UInt8)0x51,  /* [81] */
    (xdc_UInt8)0x52,  /* [82] */
    (xdc_UInt8)0x53,  /* [83] */
    (xdc_UInt8)0x54,  /* [84] */
    (xdc_UInt8)0x55,  /* [85] */
    (xdc_UInt8)0x56,  /* [86] */
    (xdc_UInt8)0x57,  /* [87] */
    (xdc_UInt8)0x58,  /* [88] */
    (xdc_UInt8)0x59,  /* [89] */
    (xdc_UInt8)0x5a,  /* [90] */
    (xdc_UInt8)0x5b,  /* [91] */
    (xdc_UInt8)0x5c,  /* [92] */
    (xdc_UInt8)0x5d,  /* [93] */
    (xdc_UInt8)0x5e,  /* [94] */
    (xdc_UInt8)0x5f,  /* [95] */
    (xdc_UInt8)0x60,  /* [96] */
    (xdc_UInt8)0x61,  /* [97] */
    (xdc_UInt8)0x62,  /* [98] */
    (xdc_UInt8)0x63,  /* [99] */
    (xdc_UInt8)0x64,  /* [100] */
    (xdc_UInt8)0x65,  /* [101] */
    (xdc_UInt8)0x66,  /* [102] */
    (xdc_UInt8)0x67,  /* [103] */
    (xdc_UInt8)0x68,  /* [104] */
    (xdc_UInt8)0x69,  /* [105] */
    (xdc_UInt8)0x6a,  /* [106] */
    (xdc_UInt8)0x6b,  /* [107] */
    (xdc_UInt8)0x6c,  /* [108] */
    (xdc_UInt8)0x6d,  /* [109] */
    (xdc_UInt8)0x6e,  /* [110] */
    (xdc_UInt8)0x6f,  /* [111] */
    (xdc_UInt8)0x70,  /* [112] */
    (xdc_UInt8)0x71,  /* [113] */
    (xdc_UInt8)0x72,  /* [114] */
    (xdc_UInt8)0x73,  /* [115] */
    (xdc_UInt8)0x74,  /* [116] */
    (xdc_UInt8)0x75,  /* [117] */
    (xdc_UInt8)0x76,  /* [118] */
    (xdc_UInt8)0x77,  /* [119] */
    (xdc_UInt8)0x78,  /* [120] */
    (xdc_UInt8)0x79,  /* [121] */
    (xdc_UInt8)0x7a,  /* [122] */
    (xdc_UInt8)0x7b,  /* [123] */
    (xdc_UInt8)0x7c,  /* [124] */
    (xdc_UInt8)0x7d,  /* [125] */
    (xdc_UInt8)0x7e,  /* [126] */
    (xdc_UInt8)0x7f,  /* [127] */
};

/* --> ti_sysbios_family_arm_v7r_vim_Hwi_intReqEnaSet__A */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_intReqEnaSet__A, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_intReqEnaSet__A");
const __T1_ti_sysbios_family_arm_v7r_vim_Hwi_intReqEnaSet ti_sysbios_family_arm_v7r_vim_Hwi_intReqEnaSet__A[4] = {
    (xdc_UInt32)0x4,  /* [0] */
    (xdc_UInt32)0x0,  /* [1] */
    (xdc_UInt32)0x0,  /* [2] */
    (xdc_UInt32)0x0,  /* [3] */
};

/* --> ti_sysbios_family_arm_v7r_vim_Hwi_wakeEnaSet__A */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_wakeEnaSet__A, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_wakeEnaSet__A");
const __T1_ti_sysbios_family_arm_v7r_vim_Hwi_wakeEnaSet ti_sysbios_family_arm_v7r_vim_Hwi_wakeEnaSet__A[4] = {
    (xdc_UInt32)0xffffffff,  /* [0] */
    (xdc_UInt32)0xffffffff,  /* [1] */
    (xdc_UInt32)0xffffffff,  /* [2] */
    (xdc_UInt32)0xffffffff,  /* [3] */
};

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsEnabled__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsEnabled ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsIncluded__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsIncluded ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsMask__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsMask ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsMask__C = ((CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_Module__gateObj__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__gateObj ti_sysbios_family_arm_v7r_vim_Hwi_Module__gateObj__C = ((CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_Module__gatePrms__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__gatePrms ti_sysbios_family_arm_v7r_vim_Hwi_Module__gatePrms__C = ((CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_Module__id__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_Module__id__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__id ti_sysbios_family_arm_v7r_vim_Hwi_Module__id__C = (xdc_Bits16)0x8024;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerDefined__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerDefined ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerObj__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerObj ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerObj__C = ((CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn0__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn0 ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn0__C = ((CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn1__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn1 ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn1__C = ((CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn2__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn2 ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn2__C = ((CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn4__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn4 ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn4__C = ((CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn8__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn8 ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn8__C = ((CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_Object__count__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_Object__count__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_Object__count ti_sysbios_family_arm_v7r_vim_Hwi_Object__count__C = 1;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_Object__heap__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_Object__heap__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_Object__heap ti_sysbios_family_arm_v7r_vim_Hwi_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_Object__sizeof__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_Object__sizeof ti_sysbios_family_arm_v7r_vim_Hwi_Object__sizeof__C = sizeof(ti_sysbios_family_arm_v7r_vim_Hwi_Object__);

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_Object__table__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_Object__table__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_Object__table ti_sysbios_family_arm_v7r_vim_Hwi_Object__table__C = ti_sysbios_family_arm_v7r_vim_Hwi_Object__table__V;

/* dispatcherAutoNestingSupport__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_dispatcherAutoNestingSupport__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_dispatcherAutoNestingSupport__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_dispatcherAutoNestingSupport ti_sysbios_family_arm_v7r_vim_Hwi_dispatcherAutoNestingSupport__C = 1;

/* dispatcherSwiSupport__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_dispatcherSwiSupport__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_dispatcherSwiSupport__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_dispatcherSwiSupport ti_sysbios_family_arm_v7r_vim_Hwi_dispatcherSwiSupport__C = 1;

/* dispatcherTaskSupport__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_dispatcherTaskSupport__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_dispatcherTaskSupport__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_dispatcherTaskSupport ti_sysbios_family_arm_v7r_vim_Hwi_dispatcherTaskSupport__C = 1;

/* dispatcherIrpTrackingSupport__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_dispatcherIrpTrackingSupport__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_dispatcherIrpTrackingSupport__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_dispatcherIrpTrackingSupport ti_sysbios_family_arm_v7r_vim_Hwi_dispatcherIrpTrackingSupport__C = 1;

/* NUM_INTERRUPTS__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_NUM_INTERRUPTS__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_NUM_INTERRUPTS__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_NUM_INTERRUPTS ti_sysbios_family_arm_v7r_vim_Hwi_NUM_INTERRUPTS__C = (xdc_UInt)0x80;

/* core0VectorTableAddress__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_core0VectorTableAddress__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_core0VectorTableAddress__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_core0VectorTableAddress ti_sysbios_family_arm_v7r_vim_Hwi_core0VectorTableAddress__C = ((CT__ti_sysbios_family_arm_v7r_vim_Hwi_core0VectorTableAddress)(0x0));

/* core1VectorTableAddress__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_core1VectorTableAddress__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_core1VectorTableAddress__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_core1VectorTableAddress ti_sysbios_family_arm_v7r_vim_Hwi_core1VectorTableAddress__C = ((CT__ti_sysbios_family_arm_v7r_vim_Hwi_core1VectorTableAddress)(0x0));

/* phantomFunc__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_phantomFunc__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_phantomFunc__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_phantomFunc ti_sysbios_family_arm_v7r_vim_Hwi_phantomFunc__C = ((CT__ti_sysbios_family_arm_v7r_vim_Hwi_phantomFunc)((xdc_Fxn)ti_sysbios_family_arm_v7r_vim_Hwi_phantomIntHandler__I));

/* fiqStack__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_fiqStack__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_fiqStack__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_fiqStack ti_sysbios_family_arm_v7r_vim_Hwi_fiqStack__C = ((CT__ti_sysbios_family_arm_v7r_vim_Hwi_fiqStack)0);

/* errataInitEsm__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_errataInitEsm__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_errataInitEsm__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_errataInitEsm ti_sysbios_family_arm_v7r_vim_Hwi_errataInitEsm__C = 1;

/* resetVIM__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_resetVIM__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_resetVIM__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_resetVIM ti_sysbios_family_arm_v7r_vim_Hwi_resetVIM__C = 1;

/* A_badChannelId__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_A_badChannelId__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_A_badChannelId__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_A_badChannelId ti_sysbios_family_arm_v7r_vim_Hwi_A_badChannelId__C = (((xdc_runtime_Assert_Id)2825) << 16 | 16);

/* E_alreadyDefined__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_E_alreadyDefined__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_E_alreadyDefined__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_E_alreadyDefined ti_sysbios_family_arm_v7r_vim_Hwi_E_alreadyDefined__C = (((xdc_runtime_Error_Id)4570) << 16 | 0);

/* E_badIntNum__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_E_badIntNum__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_E_badIntNum__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_E_badIntNum ti_sysbios_family_arm_v7r_vim_Hwi_E_badIntNum__C = (((xdc_runtime_Error_Id)4618) << 16 | 0);

/* E_undefined__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_E_undefined__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_E_undefined__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_E_undefined ti_sysbios_family_arm_v7r_vim_Hwi_E_undefined__C = (((xdc_runtime_Error_Id)4658) << 16 | 0);

/* E_unsupportedMaskingOption__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_E_unsupportedMaskingOption__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_E_unsupportedMaskingOption__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_E_unsupportedMaskingOption ti_sysbios_family_arm_v7r_vim_Hwi_E_unsupportedMaskingOption__C = (((xdc_runtime_Error_Id)4697) << 16 | 0);

/* E_phantomInterrupt__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_E_phantomInterrupt__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_E_phantomInterrupt__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_E_phantomInterrupt ti_sysbios_family_arm_v7r_vim_Hwi_E_phantomInterrupt__C = (((xdc_runtime_Error_Id)4760) << 16 | 0);

/* LM_begin__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_LM_begin__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_LM_begin__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_LM_begin ti_sysbios_family_arm_v7r_vim_Hwi_LM_begin__C = (((xdc_runtime_Log_Event)6429) << 16 | 768);

/* LD_end__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_LD_end__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_LD_end__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_LD_end ti_sysbios_family_arm_v7r_vim_Hwi_LD_end__C = (((xdc_runtime_Log_Event)6499) << 16 | 512);

/* channelMap__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_channelMap__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_channelMap__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_channelMap ti_sysbios_family_arm_v7r_vim_Hwi_channelMap__C = ((CT__ti_sysbios_family_arm_v7r_vim_Hwi_channelMap)ti_sysbios_family_arm_v7r_vim_Hwi_channelMap__A);

/* intReqEnaSet__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_intReqEnaSet__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_intReqEnaSet__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_intReqEnaSet ti_sysbios_family_arm_v7r_vim_Hwi_intReqEnaSet__C = ((CT__ti_sysbios_family_arm_v7r_vim_Hwi_intReqEnaSet)ti_sysbios_family_arm_v7r_vim_Hwi_intReqEnaSet__A);

/* wakeEnaSet__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_wakeEnaSet__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_wakeEnaSet__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_wakeEnaSet ti_sysbios_family_arm_v7r_vim_Hwi_wakeEnaSet__C = ((CT__ti_sysbios_family_arm_v7r_vim_Hwi_wakeEnaSet)ti_sysbios_family_arm_v7r_vim_Hwi_wakeEnaSet__A);

/* swiDisable__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_swiDisable__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_swiDisable__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_swiDisable ti_sysbios_family_arm_v7r_vim_Hwi_swiDisable__C = ((CT__ti_sysbios_family_arm_v7r_vim_Hwi_swiDisable)((xdc_Fxn)ti_sysbios_knl_Swi_disable__E));

/* swiRestoreHwi__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_swiRestoreHwi__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_swiRestoreHwi__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_swiRestoreHwi ti_sysbios_family_arm_v7r_vim_Hwi_swiRestoreHwi__C = ((CT__ti_sysbios_family_arm_v7r_vim_Hwi_swiRestoreHwi)((xdc_Fxn)ti_sysbios_knl_Swi_restoreHwi__E));

/* taskDisable__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_taskDisable__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_taskDisable__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_taskDisable ti_sysbios_family_arm_v7r_vim_Hwi_taskDisable__C = ((CT__ti_sysbios_family_arm_v7r_vim_Hwi_taskDisable)((xdc_Fxn)ti_sysbios_knl_Task_disable__E));

/* taskRestoreHwi__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_taskRestoreHwi__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_taskRestoreHwi__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_taskRestoreHwi ti_sysbios_family_arm_v7r_vim_Hwi_taskRestoreHwi__C = ((CT__ti_sysbios_family_arm_v7r_vim_Hwi_taskRestoreHwi)((xdc_Fxn)ti_sysbios_knl_Task_restoreHwi__E));

/* hooks__C */
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_hooks__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_hooks__C");
__FAR__ const CT__ti_sysbios_family_arm_v7r_vim_Hwi_hooks ti_sysbios_family_arm_v7r_vim_Hwi_hooks__C = {0, 0};


/*
 * ======== ti.sysbios.gates.GateHwi INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_gates_GateHwi_Object__DESC__C;

/* Object__PARAMS__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateHwi_Object__PARAMS__C, ".const:ti_sysbios_gates_GateHwi_Object__PARAMS__C");
__FAR__ const ti_sysbios_gates_GateHwi_Params ti_sysbios_gates_GateHwi_Object__PARAMS__C = {
    sizeof (ti_sysbios_gates_GateHwi_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&ti_sysbios_gates_GateHwi_Object__PARAMS__C.__iprms, /* instance */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
ti_sysbios_gates_GateHwi_Module__ ti_sysbios_gates_GateHwi_Module__root__V = {
    {&ti_sysbios_gates_GateHwi_Module__root__V.link,  /* link.next */
    &ti_sysbios_gates_GateHwi_Module__root__V.link},  /* link.prev */
};

/* Object__table__V */
ti_sysbios_gates_GateHwi_Object__ ti_sysbios_gates_GateHwi_Object__table__V[1] = {
    {/* instance#0 */
        &ti_sysbios_gates_GateHwi_Module__FXNS__C,
    },
};

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateHwi_Module__diagsEnabled__C, ".const:ti_sysbios_gates_GateHwi_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_gates_GateHwi_Module__diagsEnabled ti_sysbios_gates_GateHwi_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateHwi_Module__diagsIncluded__C, ".const:ti_sysbios_gates_GateHwi_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_gates_GateHwi_Module__diagsIncluded ti_sysbios_gates_GateHwi_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateHwi_Module__diagsMask__C, ".const:ti_sysbios_gates_GateHwi_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_gates_GateHwi_Module__diagsMask ti_sysbios_gates_GateHwi_Module__diagsMask__C = ((CT__ti_sysbios_gates_GateHwi_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateHwi_Module__gateObj__C, ".const:ti_sysbios_gates_GateHwi_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_gates_GateHwi_Module__gateObj ti_sysbios_gates_GateHwi_Module__gateObj__C = ((CT__ti_sysbios_gates_GateHwi_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateHwi_Module__gatePrms__C, ".const:ti_sysbios_gates_GateHwi_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_gates_GateHwi_Module__gatePrms ti_sysbios_gates_GateHwi_Module__gatePrms__C = ((CT__ti_sysbios_gates_GateHwi_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateHwi_Module__id__C, ".const:ti_sysbios_gates_GateHwi_Module__id__C");
__FAR__ const CT__ti_sysbios_gates_GateHwi_Module__id ti_sysbios_gates_GateHwi_Module__id__C = (xdc_Bits16)0x8028;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateHwi_Module__loggerDefined__C, ".const:ti_sysbios_gates_GateHwi_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_gates_GateHwi_Module__loggerDefined ti_sysbios_gates_GateHwi_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateHwi_Module__loggerObj__C, ".const:ti_sysbios_gates_GateHwi_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_gates_GateHwi_Module__loggerObj ti_sysbios_gates_GateHwi_Module__loggerObj__C = ((CT__ti_sysbios_gates_GateHwi_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateHwi_Module__loggerFxn0__C, ".const:ti_sysbios_gates_GateHwi_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_gates_GateHwi_Module__loggerFxn0 ti_sysbios_gates_GateHwi_Module__loggerFxn0__C = ((CT__ti_sysbios_gates_GateHwi_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateHwi_Module__loggerFxn1__C, ".const:ti_sysbios_gates_GateHwi_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_gates_GateHwi_Module__loggerFxn1 ti_sysbios_gates_GateHwi_Module__loggerFxn1__C = ((CT__ti_sysbios_gates_GateHwi_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateHwi_Module__loggerFxn2__C, ".const:ti_sysbios_gates_GateHwi_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_gates_GateHwi_Module__loggerFxn2 ti_sysbios_gates_GateHwi_Module__loggerFxn2__C = ((CT__ti_sysbios_gates_GateHwi_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateHwi_Module__loggerFxn4__C, ".const:ti_sysbios_gates_GateHwi_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_gates_GateHwi_Module__loggerFxn4 ti_sysbios_gates_GateHwi_Module__loggerFxn4__C = ((CT__ti_sysbios_gates_GateHwi_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateHwi_Module__loggerFxn8__C, ".const:ti_sysbios_gates_GateHwi_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_gates_GateHwi_Module__loggerFxn8 ti_sysbios_gates_GateHwi_Module__loggerFxn8__C = ((CT__ti_sysbios_gates_GateHwi_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateHwi_Object__count__C, ".const:ti_sysbios_gates_GateHwi_Object__count__C");
__FAR__ const CT__ti_sysbios_gates_GateHwi_Object__count ti_sysbios_gates_GateHwi_Object__count__C = 1;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateHwi_Object__heap__C, ".const:ti_sysbios_gates_GateHwi_Object__heap__C");
__FAR__ const CT__ti_sysbios_gates_GateHwi_Object__heap ti_sysbios_gates_GateHwi_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateHwi_Object__sizeof__C, ".const:ti_sysbios_gates_GateHwi_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_gates_GateHwi_Object__sizeof ti_sysbios_gates_GateHwi_Object__sizeof__C = sizeof(ti_sysbios_gates_GateHwi_Object__);

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateHwi_Object__table__C, ".const:ti_sysbios_gates_GateHwi_Object__table__C");
__FAR__ const CT__ti_sysbios_gates_GateHwi_Object__table ti_sysbios_gates_GateHwi_Object__table__C = ti_sysbios_gates_GateHwi_Object__table__V;


/*
 * ======== ti.sysbios.gates.GateMutex INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_gates_GateMutex_Object__DESC__C;

/* Object__PARAMS__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateMutex_Object__PARAMS__C, ".const:ti_sysbios_gates_GateMutex_Object__PARAMS__C");
__FAR__ const ti_sysbios_gates_GateMutex_Params ti_sysbios_gates_GateMutex_Object__PARAMS__C = {
    sizeof (ti_sysbios_gates_GateMutex_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&ti_sysbios_gates_GateMutex_Object__PARAMS__C.__iprms, /* instance */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
ti_sysbios_gates_GateMutex_Module__ ti_sysbios_gates_GateMutex_Module__root__V = {
    {&ti_sysbios_gates_GateMutex_Module__root__V.link,  /* link.next */
    &ti_sysbios_gates_GateMutex_Module__root__V.link},  /* link.prev */
};

/* Object__table__V */
ti_sysbios_gates_GateMutex_Object__ ti_sysbios_gates_GateMutex_Object__table__V[2] = {
    {/* instance#0 */
        &ti_sysbios_gates_GateMutex_Module__FXNS__C,
        0,  /* owner */
        {
            0,  /* event */
            (xdc_UInt)0x1,  /* eventId */
            ti_sysbios_knl_Semaphore_Mode_COUNTING,  /* mode */
            (xdc_UInt16)0x1,  /* count */
            {
                {
                    ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_gates_GateMutex_Object__table__V[0].Object_field_sem.Object_field_pendQ.elem)),  /* next */
                    ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_gates_GateMutex_Object__table__V[0].Object_field_sem.Object_field_pendQ.elem)),  /* prev */
                },  /* elem */
            },  /* Object_field_pendQ */
        },  /* Object_field_sem */
    },
    {/* instance#1 */
        &ti_sysbios_gates_GateMutex_Module__FXNS__C,
        0,  /* owner */
        {
            0,  /* event */
            (xdc_UInt)0x1,  /* eventId */
            ti_sysbios_knl_Semaphore_Mode_COUNTING,  /* mode */
            (xdc_UInt16)0x1,  /* count */
            {
                {
                    ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_gates_GateMutex_Object__table__V[1].Object_field_sem.Object_field_pendQ.elem)),  /* next */
                    ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_gates_GateMutex_Object__table__V[1].Object_field_sem.Object_field_pendQ.elem)),  /* prev */
                },  /* elem */
            },  /* Object_field_pendQ */
        },  /* Object_field_sem */
    },
};

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateMutex_Module__diagsEnabled__C, ".const:ti_sysbios_gates_GateMutex_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_gates_GateMutex_Module__diagsEnabled ti_sysbios_gates_GateMutex_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateMutex_Module__diagsIncluded__C, ".const:ti_sysbios_gates_GateMutex_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_gates_GateMutex_Module__diagsIncluded ti_sysbios_gates_GateMutex_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateMutex_Module__diagsMask__C, ".const:ti_sysbios_gates_GateMutex_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_gates_GateMutex_Module__diagsMask ti_sysbios_gates_GateMutex_Module__diagsMask__C = ((CT__ti_sysbios_gates_GateMutex_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateMutex_Module__gateObj__C, ".const:ti_sysbios_gates_GateMutex_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_gates_GateMutex_Module__gateObj ti_sysbios_gates_GateMutex_Module__gateObj__C = ((CT__ti_sysbios_gates_GateMutex_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateMutex_Module__gatePrms__C, ".const:ti_sysbios_gates_GateMutex_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_gates_GateMutex_Module__gatePrms ti_sysbios_gates_GateMutex_Module__gatePrms__C = ((CT__ti_sysbios_gates_GateMutex_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateMutex_Module__id__C, ".const:ti_sysbios_gates_GateMutex_Module__id__C");
__FAR__ const CT__ti_sysbios_gates_GateMutex_Module__id ti_sysbios_gates_GateMutex_Module__id__C = (xdc_Bits16)0x8029;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateMutex_Module__loggerDefined__C, ".const:ti_sysbios_gates_GateMutex_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_gates_GateMutex_Module__loggerDefined ti_sysbios_gates_GateMutex_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateMutex_Module__loggerObj__C, ".const:ti_sysbios_gates_GateMutex_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_gates_GateMutex_Module__loggerObj ti_sysbios_gates_GateMutex_Module__loggerObj__C = ((CT__ti_sysbios_gates_GateMutex_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateMutex_Module__loggerFxn0__C, ".const:ti_sysbios_gates_GateMutex_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_gates_GateMutex_Module__loggerFxn0 ti_sysbios_gates_GateMutex_Module__loggerFxn0__C = ((CT__ti_sysbios_gates_GateMutex_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateMutex_Module__loggerFxn1__C, ".const:ti_sysbios_gates_GateMutex_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_gates_GateMutex_Module__loggerFxn1 ti_sysbios_gates_GateMutex_Module__loggerFxn1__C = ((CT__ti_sysbios_gates_GateMutex_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateMutex_Module__loggerFxn2__C, ".const:ti_sysbios_gates_GateMutex_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_gates_GateMutex_Module__loggerFxn2 ti_sysbios_gates_GateMutex_Module__loggerFxn2__C = ((CT__ti_sysbios_gates_GateMutex_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateMutex_Module__loggerFxn4__C, ".const:ti_sysbios_gates_GateMutex_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_gates_GateMutex_Module__loggerFxn4 ti_sysbios_gates_GateMutex_Module__loggerFxn4__C = ((CT__ti_sysbios_gates_GateMutex_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateMutex_Module__loggerFxn8__C, ".const:ti_sysbios_gates_GateMutex_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_gates_GateMutex_Module__loggerFxn8 ti_sysbios_gates_GateMutex_Module__loggerFxn8__C = ((CT__ti_sysbios_gates_GateMutex_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateMutex_Object__count__C, ".const:ti_sysbios_gates_GateMutex_Object__count__C");
__FAR__ const CT__ti_sysbios_gates_GateMutex_Object__count ti_sysbios_gates_GateMutex_Object__count__C = 2;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateMutex_Object__heap__C, ".const:ti_sysbios_gates_GateMutex_Object__heap__C");
__FAR__ const CT__ti_sysbios_gates_GateMutex_Object__heap ti_sysbios_gates_GateMutex_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateMutex_Object__sizeof__C, ".const:ti_sysbios_gates_GateMutex_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_gates_GateMutex_Object__sizeof ti_sysbios_gates_GateMutex_Object__sizeof__C = sizeof(ti_sysbios_gates_GateMutex_Object__);

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateMutex_Object__table__C, ".const:ti_sysbios_gates_GateMutex_Object__table__C");
__FAR__ const CT__ti_sysbios_gates_GateMutex_Object__table ti_sysbios_gates_GateMutex_Object__table__C = ti_sysbios_gates_GateMutex_Object__table__V;

/* A_badContext__C */
#pragma DATA_SECTION(ti_sysbios_gates_GateMutex_A_badContext__C, ".const:ti_sysbios_gates_GateMutex_A_badContext__C");
__FAR__ const CT__ti_sysbios_gates_GateMutex_A_badContext ti_sysbios_gates_GateMutex_A_badContext__C = (((xdc_runtime_Assert_Id)3349) << 16 | 16);


/*
 * ======== ti.sysbios.hal.Cache INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_hal_Cache_Module__diagsEnabled__C, ".const:ti_sysbios_hal_Cache_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_hal_Cache_Module__diagsEnabled ti_sysbios_hal_Cache_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_hal_Cache_Module__diagsIncluded__C, ".const:ti_sysbios_hal_Cache_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_hal_Cache_Module__diagsIncluded ti_sysbios_hal_Cache_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_hal_Cache_Module__diagsMask__C, ".const:ti_sysbios_hal_Cache_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_hal_Cache_Module__diagsMask ti_sysbios_hal_Cache_Module__diagsMask__C = ((CT__ti_sysbios_hal_Cache_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_hal_Cache_Module__gateObj__C, ".const:ti_sysbios_hal_Cache_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_hal_Cache_Module__gateObj ti_sysbios_hal_Cache_Module__gateObj__C = ((CT__ti_sysbios_hal_Cache_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_hal_Cache_Module__gatePrms__C, ".const:ti_sysbios_hal_Cache_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_hal_Cache_Module__gatePrms ti_sysbios_hal_Cache_Module__gatePrms__C = ((CT__ti_sysbios_hal_Cache_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_hal_Cache_Module__id__C, ".const:ti_sysbios_hal_Cache_Module__id__C");
__FAR__ const CT__ti_sysbios_hal_Cache_Module__id ti_sysbios_hal_Cache_Module__id__C = (xdc_Bits16)0x802b;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_hal_Cache_Module__loggerDefined__C, ".const:ti_sysbios_hal_Cache_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_hal_Cache_Module__loggerDefined ti_sysbios_hal_Cache_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_hal_Cache_Module__loggerObj__C, ".const:ti_sysbios_hal_Cache_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_hal_Cache_Module__loggerObj ti_sysbios_hal_Cache_Module__loggerObj__C = ((CT__ti_sysbios_hal_Cache_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_hal_Cache_Module__loggerFxn0__C, ".const:ti_sysbios_hal_Cache_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_hal_Cache_Module__loggerFxn0 ti_sysbios_hal_Cache_Module__loggerFxn0__C = ((CT__ti_sysbios_hal_Cache_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_hal_Cache_Module__loggerFxn1__C, ".const:ti_sysbios_hal_Cache_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_hal_Cache_Module__loggerFxn1 ti_sysbios_hal_Cache_Module__loggerFxn1__C = ((CT__ti_sysbios_hal_Cache_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_hal_Cache_Module__loggerFxn2__C, ".const:ti_sysbios_hal_Cache_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_hal_Cache_Module__loggerFxn2 ti_sysbios_hal_Cache_Module__loggerFxn2__C = ((CT__ti_sysbios_hal_Cache_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_hal_Cache_Module__loggerFxn4__C, ".const:ti_sysbios_hal_Cache_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_hal_Cache_Module__loggerFxn4 ti_sysbios_hal_Cache_Module__loggerFxn4__C = ((CT__ti_sysbios_hal_Cache_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_hal_Cache_Module__loggerFxn8__C, ".const:ti_sysbios_hal_Cache_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_hal_Cache_Module__loggerFxn8 ti_sysbios_hal_Cache_Module__loggerFxn8__C = ((CT__ti_sysbios_hal_Cache_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_hal_Cache_Object__count__C, ".const:ti_sysbios_hal_Cache_Object__count__C");
__FAR__ const CT__ti_sysbios_hal_Cache_Object__count ti_sysbios_hal_Cache_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_hal_Cache_Object__heap__C, ".const:ti_sysbios_hal_Cache_Object__heap__C");
__FAR__ const CT__ti_sysbios_hal_Cache_Object__heap ti_sysbios_hal_Cache_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_hal_Cache_Object__sizeof__C, ".const:ti_sysbios_hal_Cache_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_hal_Cache_Object__sizeof ti_sysbios_hal_Cache_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_hal_Cache_Object__table__C, ".const:ti_sysbios_hal_Cache_Object__table__C");
__FAR__ const CT__ti_sysbios_hal_Cache_Object__table ti_sysbios_hal_Cache_Object__table__C = 0;


/*
 * ======== ti.sysbios.hal.CacheNull INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_hal_CacheNull_Module__diagsEnabled__C, ".const:ti_sysbios_hal_CacheNull_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_hal_CacheNull_Module__diagsEnabled ti_sysbios_hal_CacheNull_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_hal_CacheNull_Module__diagsIncluded__C, ".const:ti_sysbios_hal_CacheNull_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_hal_CacheNull_Module__diagsIncluded ti_sysbios_hal_CacheNull_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_hal_CacheNull_Module__diagsMask__C, ".const:ti_sysbios_hal_CacheNull_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_hal_CacheNull_Module__diagsMask ti_sysbios_hal_CacheNull_Module__diagsMask__C = ((CT__ti_sysbios_hal_CacheNull_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_hal_CacheNull_Module__gateObj__C, ".const:ti_sysbios_hal_CacheNull_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_hal_CacheNull_Module__gateObj ti_sysbios_hal_CacheNull_Module__gateObj__C = ((CT__ti_sysbios_hal_CacheNull_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_hal_CacheNull_Module__gatePrms__C, ".const:ti_sysbios_hal_CacheNull_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_hal_CacheNull_Module__gatePrms ti_sysbios_hal_CacheNull_Module__gatePrms__C = ((CT__ti_sysbios_hal_CacheNull_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_hal_CacheNull_Module__id__C, ".const:ti_sysbios_hal_CacheNull_Module__id__C");
__FAR__ const CT__ti_sysbios_hal_CacheNull_Module__id ti_sysbios_hal_CacheNull_Module__id__C = (xdc_Bits16)0x802c;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_hal_CacheNull_Module__loggerDefined__C, ".const:ti_sysbios_hal_CacheNull_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_hal_CacheNull_Module__loggerDefined ti_sysbios_hal_CacheNull_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_hal_CacheNull_Module__loggerObj__C, ".const:ti_sysbios_hal_CacheNull_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_hal_CacheNull_Module__loggerObj ti_sysbios_hal_CacheNull_Module__loggerObj__C = ((CT__ti_sysbios_hal_CacheNull_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_hal_CacheNull_Module__loggerFxn0__C, ".const:ti_sysbios_hal_CacheNull_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_hal_CacheNull_Module__loggerFxn0 ti_sysbios_hal_CacheNull_Module__loggerFxn0__C = ((CT__ti_sysbios_hal_CacheNull_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_hal_CacheNull_Module__loggerFxn1__C, ".const:ti_sysbios_hal_CacheNull_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_hal_CacheNull_Module__loggerFxn1 ti_sysbios_hal_CacheNull_Module__loggerFxn1__C = ((CT__ti_sysbios_hal_CacheNull_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_hal_CacheNull_Module__loggerFxn2__C, ".const:ti_sysbios_hal_CacheNull_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_hal_CacheNull_Module__loggerFxn2 ti_sysbios_hal_CacheNull_Module__loggerFxn2__C = ((CT__ti_sysbios_hal_CacheNull_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_hal_CacheNull_Module__loggerFxn4__C, ".const:ti_sysbios_hal_CacheNull_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_hal_CacheNull_Module__loggerFxn4 ti_sysbios_hal_CacheNull_Module__loggerFxn4__C = ((CT__ti_sysbios_hal_CacheNull_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_hal_CacheNull_Module__loggerFxn8__C, ".const:ti_sysbios_hal_CacheNull_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_hal_CacheNull_Module__loggerFxn8 ti_sysbios_hal_CacheNull_Module__loggerFxn8__C = ((CT__ti_sysbios_hal_CacheNull_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_hal_CacheNull_Object__count__C, ".const:ti_sysbios_hal_CacheNull_Object__count__C");
__FAR__ const CT__ti_sysbios_hal_CacheNull_Object__count ti_sysbios_hal_CacheNull_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_hal_CacheNull_Object__heap__C, ".const:ti_sysbios_hal_CacheNull_Object__heap__C");
__FAR__ const CT__ti_sysbios_hal_CacheNull_Object__heap ti_sysbios_hal_CacheNull_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_hal_CacheNull_Object__sizeof__C, ".const:ti_sysbios_hal_CacheNull_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_hal_CacheNull_Object__sizeof ti_sysbios_hal_CacheNull_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_hal_CacheNull_Object__table__C, ".const:ti_sysbios_hal_CacheNull_Object__table__C");
__FAR__ const CT__ti_sysbios_hal_CacheNull_Object__table ti_sysbios_hal_CacheNull_Object__table__C = 0;


/*
 * ======== ti.sysbios.hal.Cache_CacheProxy INITIALIZERS ========
 */


/*
 * ======== ti.sysbios.hal.Core INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_hal_Core_Module__diagsEnabled__C, ".const:ti_sysbios_hal_Core_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_hal_Core_Module__diagsEnabled ti_sysbios_hal_Core_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_hal_Core_Module__diagsIncluded__C, ".const:ti_sysbios_hal_Core_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_hal_Core_Module__diagsIncluded ti_sysbios_hal_Core_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_hal_Core_Module__diagsMask__C, ".const:ti_sysbios_hal_Core_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_hal_Core_Module__diagsMask ti_sysbios_hal_Core_Module__diagsMask__C = ((CT__ti_sysbios_hal_Core_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_hal_Core_Module__gateObj__C, ".const:ti_sysbios_hal_Core_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_hal_Core_Module__gateObj ti_sysbios_hal_Core_Module__gateObj__C = ((CT__ti_sysbios_hal_Core_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_hal_Core_Module__gatePrms__C, ".const:ti_sysbios_hal_Core_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_hal_Core_Module__gatePrms ti_sysbios_hal_Core_Module__gatePrms__C = ((CT__ti_sysbios_hal_Core_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_hal_Core_Module__id__C, ".const:ti_sysbios_hal_Core_Module__id__C");
__FAR__ const CT__ti_sysbios_hal_Core_Module__id ti_sysbios_hal_Core_Module__id__C = (xdc_Bits16)0x802d;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_hal_Core_Module__loggerDefined__C, ".const:ti_sysbios_hal_Core_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_hal_Core_Module__loggerDefined ti_sysbios_hal_Core_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_hal_Core_Module__loggerObj__C, ".const:ti_sysbios_hal_Core_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_hal_Core_Module__loggerObj ti_sysbios_hal_Core_Module__loggerObj__C = ((CT__ti_sysbios_hal_Core_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_hal_Core_Module__loggerFxn0__C, ".const:ti_sysbios_hal_Core_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_hal_Core_Module__loggerFxn0 ti_sysbios_hal_Core_Module__loggerFxn0__C = ((CT__ti_sysbios_hal_Core_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_hal_Core_Module__loggerFxn1__C, ".const:ti_sysbios_hal_Core_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_hal_Core_Module__loggerFxn1 ti_sysbios_hal_Core_Module__loggerFxn1__C = ((CT__ti_sysbios_hal_Core_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_hal_Core_Module__loggerFxn2__C, ".const:ti_sysbios_hal_Core_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_hal_Core_Module__loggerFxn2 ti_sysbios_hal_Core_Module__loggerFxn2__C = ((CT__ti_sysbios_hal_Core_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_hal_Core_Module__loggerFxn4__C, ".const:ti_sysbios_hal_Core_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_hal_Core_Module__loggerFxn4 ti_sysbios_hal_Core_Module__loggerFxn4__C = ((CT__ti_sysbios_hal_Core_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_hal_Core_Module__loggerFxn8__C, ".const:ti_sysbios_hal_Core_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_hal_Core_Module__loggerFxn8 ti_sysbios_hal_Core_Module__loggerFxn8__C = ((CT__ti_sysbios_hal_Core_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_hal_Core_Object__count__C, ".const:ti_sysbios_hal_Core_Object__count__C");
__FAR__ const CT__ti_sysbios_hal_Core_Object__count ti_sysbios_hal_Core_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_hal_Core_Object__heap__C, ".const:ti_sysbios_hal_Core_Object__heap__C");
__FAR__ const CT__ti_sysbios_hal_Core_Object__heap ti_sysbios_hal_Core_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_hal_Core_Object__sizeof__C, ".const:ti_sysbios_hal_Core_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_hal_Core_Object__sizeof ti_sysbios_hal_Core_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_hal_Core_Object__table__C, ".const:ti_sysbios_hal_Core_Object__table__C");
__FAR__ const CT__ti_sysbios_hal_Core_Object__table ti_sysbios_hal_Core_Object__table__C = 0;

/* numCores__C */
#pragma DATA_SECTION(ti_sysbios_hal_Core_numCores__C, ".const:ti_sysbios_hal_Core_numCores__C");
__FAR__ const CT__ti_sysbios_hal_Core_numCores ti_sysbios_hal_Core_numCores__C = (xdc_UInt)0x1;


/*
 * ======== ti.sysbios.hal.Core_CoreProxy INITIALIZERS ========
 */


/*
 * ======== ti.sysbios.hal.Hwi INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_hal_Hwi_Object__DESC__C;

/* Object__PARAMS__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_Object__PARAMS__C, ".const:ti_sysbios_hal_Hwi_Object__PARAMS__C");
__FAR__ const ti_sysbios_hal_Hwi_Params ti_sysbios_hal_Hwi_Object__PARAMS__C = {
    sizeof (ti_sysbios_hal_Hwi_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&ti_sysbios_hal_Hwi_Object__PARAMS__C.__iprms, /* instance */
    ti_sysbios_interfaces_IHwi_MaskingOption_LOWER,  /* maskSetting */
    ((xdc_UArg)(0x0)),  /* arg */
    1,  /* enableInt */
    (xdc_Int)(-0x0 - 1),  /* eventId */
    (xdc_Int)(-0x0 - 1),  /* priority */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
ti_sysbios_hal_Hwi_Module__ ti_sysbios_hal_Hwi_Module__root__V = {
    {&ti_sysbios_hal_Hwi_Module__root__V.link,  /* link.next */
    &ti_sysbios_hal_Hwi_Module__root__V.link},  /* link.prev */
};

/* Object__table__V */
ti_sysbios_hal_Hwi_Object__ ti_sysbios_hal_Hwi_Object__table__V[1] = {
    {/* instance#0 */
        0,
        (ti_sysbios_hal_Hwi_HwiProxy_Handle)&ti_sysbios_family_arm_v7r_vim_Hwi_Object__table__V[0],  /* pi */
    },
};

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_Module__diagsEnabled__C, ".const:ti_sysbios_hal_Hwi_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_hal_Hwi_Module__diagsEnabled ti_sysbios_hal_Hwi_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_Module__diagsIncluded__C, ".const:ti_sysbios_hal_Hwi_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_hal_Hwi_Module__diagsIncluded ti_sysbios_hal_Hwi_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_Module__diagsMask__C, ".const:ti_sysbios_hal_Hwi_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_hal_Hwi_Module__diagsMask ti_sysbios_hal_Hwi_Module__diagsMask__C = ((CT__ti_sysbios_hal_Hwi_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_Module__gateObj__C, ".const:ti_sysbios_hal_Hwi_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_hal_Hwi_Module__gateObj ti_sysbios_hal_Hwi_Module__gateObj__C = ((CT__ti_sysbios_hal_Hwi_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_Module__gatePrms__C, ".const:ti_sysbios_hal_Hwi_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_hal_Hwi_Module__gatePrms ti_sysbios_hal_Hwi_Module__gatePrms__C = ((CT__ti_sysbios_hal_Hwi_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_Module__id__C, ".const:ti_sysbios_hal_Hwi_Module__id__C");
__FAR__ const CT__ti_sysbios_hal_Hwi_Module__id ti_sysbios_hal_Hwi_Module__id__C = (xdc_Bits16)0x802e;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_Module__loggerDefined__C, ".const:ti_sysbios_hal_Hwi_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_hal_Hwi_Module__loggerDefined ti_sysbios_hal_Hwi_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_Module__loggerObj__C, ".const:ti_sysbios_hal_Hwi_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_hal_Hwi_Module__loggerObj ti_sysbios_hal_Hwi_Module__loggerObj__C = ((CT__ti_sysbios_hal_Hwi_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_Module__loggerFxn0__C, ".const:ti_sysbios_hal_Hwi_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_hal_Hwi_Module__loggerFxn0 ti_sysbios_hal_Hwi_Module__loggerFxn0__C = ((CT__ti_sysbios_hal_Hwi_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_Module__loggerFxn1__C, ".const:ti_sysbios_hal_Hwi_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_hal_Hwi_Module__loggerFxn1 ti_sysbios_hal_Hwi_Module__loggerFxn1__C = ((CT__ti_sysbios_hal_Hwi_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_Module__loggerFxn2__C, ".const:ti_sysbios_hal_Hwi_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_hal_Hwi_Module__loggerFxn2 ti_sysbios_hal_Hwi_Module__loggerFxn2__C = ((CT__ti_sysbios_hal_Hwi_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_Module__loggerFxn4__C, ".const:ti_sysbios_hal_Hwi_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_hal_Hwi_Module__loggerFxn4 ti_sysbios_hal_Hwi_Module__loggerFxn4__C = ((CT__ti_sysbios_hal_Hwi_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_Module__loggerFxn8__C, ".const:ti_sysbios_hal_Hwi_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_hal_Hwi_Module__loggerFxn8 ti_sysbios_hal_Hwi_Module__loggerFxn8__C = ((CT__ti_sysbios_hal_Hwi_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_Object__count__C, ".const:ti_sysbios_hal_Hwi_Object__count__C");
__FAR__ const CT__ti_sysbios_hal_Hwi_Object__count ti_sysbios_hal_Hwi_Object__count__C = 1;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_Object__heap__C, ".const:ti_sysbios_hal_Hwi_Object__heap__C");
__FAR__ const CT__ti_sysbios_hal_Hwi_Object__heap ti_sysbios_hal_Hwi_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_Object__sizeof__C, ".const:ti_sysbios_hal_Hwi_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_hal_Hwi_Object__sizeof ti_sysbios_hal_Hwi_Object__sizeof__C = sizeof(ti_sysbios_hal_Hwi_Object__);

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_Object__table__C, ".const:ti_sysbios_hal_Hwi_Object__table__C");
__FAR__ const CT__ti_sysbios_hal_Hwi_Object__table ti_sysbios_hal_Hwi_Object__table__C = ti_sysbios_hal_Hwi_Object__table__V;

/* dispatcherAutoNestingSupport__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_dispatcherAutoNestingSupport__C, ".const:ti_sysbios_hal_Hwi_dispatcherAutoNestingSupport__C");
__FAR__ const CT__ti_sysbios_hal_Hwi_dispatcherAutoNestingSupport ti_sysbios_hal_Hwi_dispatcherAutoNestingSupport__C = 1;

/* dispatcherSwiSupport__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_dispatcherSwiSupport__C, ".const:ti_sysbios_hal_Hwi_dispatcherSwiSupport__C");
__FAR__ const CT__ti_sysbios_hal_Hwi_dispatcherSwiSupport ti_sysbios_hal_Hwi_dispatcherSwiSupport__C = 1;

/* dispatcherTaskSupport__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_dispatcherTaskSupport__C, ".const:ti_sysbios_hal_Hwi_dispatcherTaskSupport__C");
__FAR__ const CT__ti_sysbios_hal_Hwi_dispatcherTaskSupport ti_sysbios_hal_Hwi_dispatcherTaskSupport__C = 1;

/* dispatcherIrpTrackingSupport__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_dispatcherIrpTrackingSupport__C, ".const:ti_sysbios_hal_Hwi_dispatcherIrpTrackingSupport__C");
__FAR__ const CT__ti_sysbios_hal_Hwi_dispatcherIrpTrackingSupport ti_sysbios_hal_Hwi_dispatcherIrpTrackingSupport__C = 1;

/* E_stackOverflow__C */
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_E_stackOverflow__C, ".const:ti_sysbios_hal_Hwi_E_stackOverflow__C");
__FAR__ const CT__ti_sysbios_hal_Hwi_E_stackOverflow ti_sysbios_hal_Hwi_E_stackOverflow__C = (((xdc_runtime_Error_Id)5021) << 16 | 0);


/*
 * ======== ti.sysbios.hal.Hwi_HwiProxy INITIALIZERS ========
 */


/*
 * ======== ti.sysbios.heaps.HeapBuf INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_heaps_HeapBuf_Object__DESC__C;

/* Object__PARAMS__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_Object__PARAMS__C, ".const:ti_sysbios_heaps_HeapBuf_Object__PARAMS__C");
__FAR__ const ti_sysbios_heaps_HeapBuf_Params ti_sysbios_heaps_HeapBuf_Object__PARAMS__C = {
    sizeof (ti_sysbios_heaps_HeapBuf_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&ti_sysbios_heaps_HeapBuf_Object__PARAMS__C.__iprms, /* instance */
    (xdc_SizeT)0x0,  /* align */
    (xdc_UInt)0x0,  /* numBlocks */
    (xdc_SizeT)0x0,  /* blockSize */
    ((xdc_UArg)(0x0)),  /* bufSize */
    ((xdc_Ptr)(0x0)),  /* buf */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
ti_sysbios_heaps_HeapBuf_Module__ ti_sysbios_heaps_HeapBuf_Module__root__V = {
    {&ti_sysbios_heaps_HeapBuf_Module__root__V.link,  /* link.next */
    &ti_sysbios_heaps_HeapBuf_Module__root__V.link},  /* link.prev */
};

/* Module__state__V */
#if defined (__ICCARM__)
#pragma location = ".data_ti_sysbios_heaps_HeapBuf_Module__state__V"
#endif
#if defined(__llvm__) && defined(__ti__)
ti_sysbios_heaps_HeapBuf_Module_State__ ti_sysbios_heaps_HeapBuf_Module__state__V __attribute__ ((section(".data:ti_sysbios_heaps_HeapBuf_Module__state__V")));
#elif defined(__GNUC__) && !(defined(__MACH__) && defined(__APPLE__))
#ifndef __TI_COMPILER_VERSION__
ti_sysbios_heaps_HeapBuf_Module_State__ ti_sysbios_heaps_HeapBuf_Module__state__V __attribute__ ((section(".data_ti_sysbios_heaps_HeapBuf_Module__state__V")));
#endif
#endif
ti_sysbios_heaps_HeapBuf_Module_State__ ti_sysbios_heaps_HeapBuf_Module__state__V = {
    ((void*)0),  /* constructedHeaps */
};

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_Module__diagsEnabled__C, ".const:ti_sysbios_heaps_HeapBuf_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_Module__diagsEnabled ti_sysbios_heaps_HeapBuf_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_Module__diagsIncluded__C, ".const:ti_sysbios_heaps_HeapBuf_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_Module__diagsIncluded ti_sysbios_heaps_HeapBuf_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_Module__diagsMask__C, ".const:ti_sysbios_heaps_HeapBuf_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_Module__diagsMask ti_sysbios_heaps_HeapBuf_Module__diagsMask__C = ((CT__ti_sysbios_heaps_HeapBuf_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_Module__gateObj__C, ".const:ti_sysbios_heaps_HeapBuf_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_Module__gateObj ti_sysbios_heaps_HeapBuf_Module__gateObj__C = ((CT__ti_sysbios_heaps_HeapBuf_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_Module__gatePrms__C, ".const:ti_sysbios_heaps_HeapBuf_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_Module__gatePrms ti_sysbios_heaps_HeapBuf_Module__gatePrms__C = ((CT__ti_sysbios_heaps_HeapBuf_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_Module__id__C, ".const:ti_sysbios_heaps_HeapBuf_Module__id__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_Module__id ti_sysbios_heaps_HeapBuf_Module__id__C = (xdc_Bits16)0x8020;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_Module__loggerDefined__C, ".const:ti_sysbios_heaps_HeapBuf_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_Module__loggerDefined ti_sysbios_heaps_HeapBuf_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_Module__loggerObj__C, ".const:ti_sysbios_heaps_HeapBuf_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_Module__loggerObj ti_sysbios_heaps_HeapBuf_Module__loggerObj__C = ((CT__ti_sysbios_heaps_HeapBuf_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_Module__loggerFxn0__C, ".const:ti_sysbios_heaps_HeapBuf_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_Module__loggerFxn0 ti_sysbios_heaps_HeapBuf_Module__loggerFxn0__C = ((CT__ti_sysbios_heaps_HeapBuf_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_Module__loggerFxn1__C, ".const:ti_sysbios_heaps_HeapBuf_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_Module__loggerFxn1 ti_sysbios_heaps_HeapBuf_Module__loggerFxn1__C = ((CT__ti_sysbios_heaps_HeapBuf_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_Module__loggerFxn2__C, ".const:ti_sysbios_heaps_HeapBuf_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_Module__loggerFxn2 ti_sysbios_heaps_HeapBuf_Module__loggerFxn2__C = ((CT__ti_sysbios_heaps_HeapBuf_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_Module__loggerFxn4__C, ".const:ti_sysbios_heaps_HeapBuf_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_Module__loggerFxn4 ti_sysbios_heaps_HeapBuf_Module__loggerFxn4__C = ((CT__ti_sysbios_heaps_HeapBuf_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_Module__loggerFxn8__C, ".const:ti_sysbios_heaps_HeapBuf_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_Module__loggerFxn8 ti_sysbios_heaps_HeapBuf_Module__loggerFxn8__C = ((CT__ti_sysbios_heaps_HeapBuf_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_Object__count__C, ".const:ti_sysbios_heaps_HeapBuf_Object__count__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_Object__count ti_sysbios_heaps_HeapBuf_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_Object__heap__C, ".const:ti_sysbios_heaps_HeapBuf_Object__heap__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_Object__heap ti_sysbios_heaps_HeapBuf_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_Object__sizeof__C, ".const:ti_sysbios_heaps_HeapBuf_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_Object__sizeof ti_sysbios_heaps_HeapBuf_Object__sizeof__C = sizeof(ti_sysbios_heaps_HeapBuf_Object__);

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_Object__table__C, ".const:ti_sysbios_heaps_HeapBuf_Object__table__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_Object__table ti_sysbios_heaps_HeapBuf_Object__table__C = 0;

/* A_nullBuf__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_A_nullBuf__C, ".const:ti_sysbios_heaps_HeapBuf_A_nullBuf__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_A_nullBuf ti_sysbios_heaps_HeapBuf_A_nullBuf__C = (((xdc_runtime_Assert_Id)1995) << 16 | 16);

/* A_bufAlign__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_A_bufAlign__C, ".const:ti_sysbios_heaps_HeapBuf_A_bufAlign__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_A_bufAlign ti_sysbios_heaps_HeapBuf_A_bufAlign__C = (((xdc_runtime_Assert_Id)2024) << 16 | 16);

/* A_invalidAlign__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_A_invalidAlign__C, ".const:ti_sysbios_heaps_HeapBuf_A_invalidAlign__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_A_invalidAlign ti_sysbios_heaps_HeapBuf_A_invalidAlign__C = (((xdc_runtime_Assert_Id)2049) << 16 | 16);

/* A_invalidRequestedAlign__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_A_invalidRequestedAlign__C, ".const:ti_sysbios_heaps_HeapBuf_A_invalidRequestedAlign__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_A_invalidRequestedAlign ti_sysbios_heaps_HeapBuf_A_invalidRequestedAlign__C = (((xdc_runtime_Assert_Id)2139) << 16 | 16);

/* A_invalidBlockSize__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_A_invalidBlockSize__C, ".const:ti_sysbios_heaps_HeapBuf_A_invalidBlockSize__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_A_invalidBlockSize ti_sysbios_heaps_HeapBuf_A_invalidBlockSize__C = (((xdc_runtime_Assert_Id)2228) << 16 | 16);

/* A_zeroBlocks__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_A_zeroBlocks__C, ".const:ti_sysbios_heaps_HeapBuf_A_zeroBlocks__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_A_zeroBlocks ti_sysbios_heaps_HeapBuf_A_zeroBlocks__C = (((xdc_runtime_Assert_Id)2288) << 16 | 16);

/* A_zeroBufSize__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_A_zeroBufSize__C, ".const:ti_sysbios_heaps_HeapBuf_A_zeroBufSize__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_A_zeroBufSize ti_sysbios_heaps_HeapBuf_A_zeroBufSize__C = (((xdc_runtime_Assert_Id)2313) << 16 | 16);

/* A_invalidBufSize__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_A_invalidBufSize__C, ".const:ti_sysbios_heaps_HeapBuf_A_invalidBufSize__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_A_invalidBufSize ti_sysbios_heaps_HeapBuf_A_invalidBufSize__C = (((xdc_runtime_Assert_Id)2336) << 16 | 16);

/* A_noBlocksToFree__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_A_noBlocksToFree__C, ".const:ti_sysbios_heaps_HeapBuf_A_noBlocksToFree__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_A_noBlocksToFree ti_sysbios_heaps_HeapBuf_A_noBlocksToFree__C = (((xdc_runtime_Assert_Id)2394) << 16 | 16);

/* A_invalidFree__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_A_invalidFree__C, ".const:ti_sysbios_heaps_HeapBuf_A_invalidFree__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_A_invalidFree ti_sysbios_heaps_HeapBuf_A_invalidFree__C = (((xdc_runtime_Assert_Id)2454) << 16 | 16);

/* E_size__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_E_size__C, ".const:ti_sysbios_heaps_HeapBuf_E_size__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_E_size ti_sysbios_heaps_HeapBuf_E_size__C = (((xdc_runtime_Error_Id)4486) << 16 | 0);

/* trackMaxAllocs__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_trackMaxAllocs__C, ".const:ti_sysbios_heaps_HeapBuf_trackMaxAllocs__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_trackMaxAllocs ti_sysbios_heaps_HeapBuf_trackMaxAllocs__C = 0;

/* numConstructedHeaps__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_numConstructedHeaps__C, ".const:ti_sysbios_heaps_HeapBuf_numConstructedHeaps__C");
__FAR__ const CT__ti_sysbios_heaps_HeapBuf_numConstructedHeaps ti_sysbios_heaps_HeapBuf_numConstructedHeaps__C = (xdc_UInt)0x0;


/*
 * ======== ti.sysbios.heaps.HeapMem INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_heaps_HeapMem_Object__DESC__C;

/* Object__PARAMS__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_Object__PARAMS__C, ".const:ti_sysbios_heaps_HeapMem_Object__PARAMS__C");
__FAR__ const ti_sysbios_heaps_HeapMem_Params ti_sysbios_heaps_HeapMem_Object__PARAMS__C = {
    sizeof (ti_sysbios_heaps_HeapMem_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&ti_sysbios_heaps_HeapMem_Object__PARAMS__C.__iprms, /* instance */
    (xdc_SizeT)0x0,  /* minBlockAlign */
    ((xdc_Ptr)(0x0)),  /* buf */
    ((xdc_UArg)(0x0)),  /* size */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* --> ti_sysbios_heaps_HeapMem_Instance_State_0_buf__A */
__T1_ti_sysbios_heaps_HeapMem_Instance_State__buf ti_sysbios_heaps_HeapMem_Instance_State_0_buf__A[32768];

/* Module__root__V */
ti_sysbios_heaps_HeapMem_Module__ ti_sysbios_heaps_HeapMem_Module__root__V = {
    {&ti_sysbios_heaps_HeapMem_Module__root__V.link,  /* link.next */
    &ti_sysbios_heaps_HeapMem_Module__root__V.link},  /* link.prev */
};

/* Object__table__V */
ti_sysbios_heaps_HeapMem_Object__ ti_sysbios_heaps_HeapMem_Object__table__V[1] = {
    {/* instance#0 */
        &ti_sysbios_heaps_HeapMem_Module__FXNS__C,
        ((xdc_UArg)(0x8)),  /* align */
        ((void*)ti_sysbios_heaps_HeapMem_Instance_State_0_buf__A),  /* buf */
        {
            ((ti_sysbios_heaps_HeapMem_Header*)0),  /* next */
            ((xdc_UArg)(0x8000)),  /* size */
        },  /* head */
        (xdc_SizeT)0x8,  /* minBlockAlign */
    },
};

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_Module__diagsEnabled__C, ".const:ti_sysbios_heaps_HeapMem_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_Module__diagsEnabled ti_sysbios_heaps_HeapMem_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_Module__diagsIncluded__C, ".const:ti_sysbios_heaps_HeapMem_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_Module__diagsIncluded ti_sysbios_heaps_HeapMem_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_Module__diagsMask__C, ".const:ti_sysbios_heaps_HeapMem_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_Module__diagsMask ti_sysbios_heaps_HeapMem_Module__diagsMask__C = ((CT__ti_sysbios_heaps_HeapMem_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_Module__gateObj__C, ".const:ti_sysbios_heaps_HeapMem_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_Module__gateObj ti_sysbios_heaps_HeapMem_Module__gateObj__C = ((CT__ti_sysbios_heaps_HeapMem_Module__gateObj)((const void*)(xdc_runtime_IGateProvider_Handle)&ti_sysbios_gates_GateMutex_Object__table__V[0]));

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_Module__gatePrms__C, ".const:ti_sysbios_heaps_HeapMem_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_Module__gatePrms ti_sysbios_heaps_HeapMem_Module__gatePrms__C = ((CT__ti_sysbios_heaps_HeapMem_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_Module__id__C, ".const:ti_sysbios_heaps_HeapMem_Module__id__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_Module__id ti_sysbios_heaps_HeapMem_Module__id__C = (xdc_Bits16)0x8021;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_Module__loggerDefined__C, ".const:ti_sysbios_heaps_HeapMem_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_Module__loggerDefined ti_sysbios_heaps_HeapMem_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_Module__loggerObj__C, ".const:ti_sysbios_heaps_HeapMem_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_Module__loggerObj ti_sysbios_heaps_HeapMem_Module__loggerObj__C = ((CT__ti_sysbios_heaps_HeapMem_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_Module__loggerFxn0__C, ".const:ti_sysbios_heaps_HeapMem_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_Module__loggerFxn0 ti_sysbios_heaps_HeapMem_Module__loggerFxn0__C = ((CT__ti_sysbios_heaps_HeapMem_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_Module__loggerFxn1__C, ".const:ti_sysbios_heaps_HeapMem_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_Module__loggerFxn1 ti_sysbios_heaps_HeapMem_Module__loggerFxn1__C = ((CT__ti_sysbios_heaps_HeapMem_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_Module__loggerFxn2__C, ".const:ti_sysbios_heaps_HeapMem_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_Module__loggerFxn2 ti_sysbios_heaps_HeapMem_Module__loggerFxn2__C = ((CT__ti_sysbios_heaps_HeapMem_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_Module__loggerFxn4__C, ".const:ti_sysbios_heaps_HeapMem_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_Module__loggerFxn4 ti_sysbios_heaps_HeapMem_Module__loggerFxn4__C = ((CT__ti_sysbios_heaps_HeapMem_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_Module__loggerFxn8__C, ".const:ti_sysbios_heaps_HeapMem_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_Module__loggerFxn8 ti_sysbios_heaps_HeapMem_Module__loggerFxn8__C = ((CT__ti_sysbios_heaps_HeapMem_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_Object__count__C, ".const:ti_sysbios_heaps_HeapMem_Object__count__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_Object__count ti_sysbios_heaps_HeapMem_Object__count__C = 1;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_Object__heap__C, ".const:ti_sysbios_heaps_HeapMem_Object__heap__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_Object__heap ti_sysbios_heaps_HeapMem_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_Object__sizeof__C, ".const:ti_sysbios_heaps_HeapMem_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_Object__sizeof ti_sysbios_heaps_HeapMem_Object__sizeof__C = sizeof(ti_sysbios_heaps_HeapMem_Object__);

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_Object__table__C, ".const:ti_sysbios_heaps_HeapMem_Object__table__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_Object__table ti_sysbios_heaps_HeapMem_Object__table__C = ti_sysbios_heaps_HeapMem_Object__table__V;

/* A_zeroBlock__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_A_zeroBlock__C, ".const:ti_sysbios_heaps_HeapMem_A_zeroBlock__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_A_zeroBlock ti_sysbios_heaps_HeapMem_A_zeroBlock__C = (((xdc_runtime_Assert_Id)2482) << 16 | 16);

/* A_heapSize__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_A_heapSize__C, ".const:ti_sysbios_heaps_HeapMem_A_heapSize__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_A_heapSize ti_sysbios_heaps_HeapMem_A_heapSize__C = (((xdc_runtime_Assert_Id)2518) << 16 | 16);

/* A_align__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_A_align__C, ".const:ti_sysbios_heaps_HeapMem_A_align__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_A_align ti_sysbios_heaps_HeapMem_A_align__C = (((xdc_runtime_Assert_Id)2563) << 16 | 16);

/* E_memory__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_E_memory__C, ".const:ti_sysbios_heaps_HeapMem_E_memory__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_E_memory ti_sysbios_heaps_HeapMem_E_memory__C = (((xdc_runtime_Error_Id)4534) << 16 | 0);

/* A_invalidFree__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_A_invalidFree__C, ".const:ti_sysbios_heaps_HeapMem_A_invalidFree__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_A_invalidFree ti_sysbios_heaps_HeapMem_A_invalidFree__C = (((xdc_runtime_Assert_Id)2454) << 16 | 16);

/* primaryHeapBaseAddr__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_primaryHeapBaseAddr__C, ".const:ti_sysbios_heaps_HeapMem_primaryHeapBaseAddr__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_primaryHeapBaseAddr ti_sysbios_heaps_HeapMem_primaryHeapBaseAddr__C = ((CT__ti_sysbios_heaps_HeapMem_primaryHeapBaseAddr)0);

/* primaryHeapEndAddr__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_primaryHeapEndAddr__C, ".const:ti_sysbios_heaps_HeapMem_primaryHeapEndAddr__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_primaryHeapEndAddr ti_sysbios_heaps_HeapMem_primaryHeapEndAddr__C = ((CT__ti_sysbios_heaps_HeapMem_primaryHeapEndAddr)0);

/* reqAlign__C */
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_reqAlign__C, ".const:ti_sysbios_heaps_HeapMem_reqAlign__C");
__FAR__ const CT__ti_sysbios_heaps_HeapMem_reqAlign ti_sysbios_heaps_HeapMem_reqAlign__C = (xdc_SizeT)0x8;


/*
 * ======== ti.sysbios.heaps.HeapMem_Module_GateProxy INITIALIZERS ========
 */


/*
 * ======== ti.sysbios.knl.Clock INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_knl_Clock_Object__DESC__C;

/* Object__PARAMS__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_Object__PARAMS__C, ".const:ti_sysbios_knl_Clock_Object__PARAMS__C");
__FAR__ const ti_sysbios_knl_Clock_Params ti_sysbios_knl_Clock_Object__PARAMS__C = {
    sizeof (ti_sysbios_knl_Clock_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&ti_sysbios_knl_Clock_Object__PARAMS__C.__iprms, /* instance */
    0,  /* startFlag */
    (xdc_UInt32)0x0,  /* period */
    ((xdc_UArg)0),  /* arg */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
ti_sysbios_knl_Clock_Module__ ti_sysbios_knl_Clock_Module__root__V = {
    {&ti_sysbios_knl_Clock_Module__root__V.link,  /* link.next */
    &ti_sysbios_knl_Clock_Module__root__V.link},  /* link.prev */
};

/* Module__state__V */
#if defined (__ICCARM__)
#pragma location = ".data_ti_sysbios_knl_Clock_Module__state__V"
#endif
#if defined(__llvm__) && defined(__ti__)
ti_sysbios_knl_Clock_Module_State__ ti_sysbios_knl_Clock_Module__state__V __attribute__ ((section(".data:ti_sysbios_knl_Clock_Module__state__V")));
#elif defined(__GNUC__) && !(defined(__MACH__) && defined(__APPLE__))
#ifndef __TI_COMPILER_VERSION__
ti_sysbios_knl_Clock_Module_State__ ti_sysbios_knl_Clock_Module__state__V __attribute__ ((section(".data_ti_sysbios_knl_Clock_Module__state__V")));
#endif
#endif
ti_sysbios_knl_Clock_Module_State__ ti_sysbios_knl_Clock_Module__state__V = {
    (xdc_UInt32)0x0,  /* ticks */
    (xdc_UInt)0x0,  /* swiCount */
    (ti_sysbios_knl_Clock_TimerProxy_Handle)&ti_sysbios_timers_rti_Timer_Object__table__V[0],  /* timer */
    (ti_sysbios_knl_Swi_Handle)&ti_sysbios_knl_Swi_Object__table__V[0],  /* swi */
    (xdc_UInt)0x1,  /* numTickSkip */
    (xdc_UInt32)0x1,  /* nextScheduledTick */
    (xdc_UInt32)0x0,  /* maxSkippable */
    0,  /* inWorkFunc */
    0,  /* startDuringWorkFunc */
    0,  /* ticking */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Clock_Module__state__V.Object_field_clockQ.elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Clock_Module__state__V.Object_field_clockQ.elem)),  /* prev */
        },  /* elem */
    },  /* Object_field_clockQ */
};

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_Module__diagsEnabled__C, ".const:ti_sysbios_knl_Clock_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_knl_Clock_Module__diagsEnabled ti_sysbios_knl_Clock_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_Module__diagsIncluded__C, ".const:ti_sysbios_knl_Clock_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_knl_Clock_Module__diagsIncluded ti_sysbios_knl_Clock_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_Module__diagsMask__C, ".const:ti_sysbios_knl_Clock_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_knl_Clock_Module__diagsMask ti_sysbios_knl_Clock_Module__diagsMask__C = ((CT__ti_sysbios_knl_Clock_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_Module__gateObj__C, ".const:ti_sysbios_knl_Clock_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_knl_Clock_Module__gateObj ti_sysbios_knl_Clock_Module__gateObj__C = ((CT__ti_sysbios_knl_Clock_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_Module__gatePrms__C, ".const:ti_sysbios_knl_Clock_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_knl_Clock_Module__gatePrms ti_sysbios_knl_Clock_Module__gatePrms__C = ((CT__ti_sysbios_knl_Clock_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_Module__id__C, ".const:ti_sysbios_knl_Clock_Module__id__C");
__FAR__ const CT__ti_sysbios_knl_Clock_Module__id ti_sysbios_knl_Clock_Module__id__C = (xdc_Bits16)0x8017;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_Module__loggerDefined__C, ".const:ti_sysbios_knl_Clock_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_knl_Clock_Module__loggerDefined ti_sysbios_knl_Clock_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_Module__loggerObj__C, ".const:ti_sysbios_knl_Clock_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_knl_Clock_Module__loggerObj ti_sysbios_knl_Clock_Module__loggerObj__C = ((CT__ti_sysbios_knl_Clock_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_Module__loggerFxn0__C, ".const:ti_sysbios_knl_Clock_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_knl_Clock_Module__loggerFxn0 ti_sysbios_knl_Clock_Module__loggerFxn0__C = ((CT__ti_sysbios_knl_Clock_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_Module__loggerFxn1__C, ".const:ti_sysbios_knl_Clock_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_knl_Clock_Module__loggerFxn1 ti_sysbios_knl_Clock_Module__loggerFxn1__C = ((CT__ti_sysbios_knl_Clock_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_Module__loggerFxn2__C, ".const:ti_sysbios_knl_Clock_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_knl_Clock_Module__loggerFxn2 ti_sysbios_knl_Clock_Module__loggerFxn2__C = ((CT__ti_sysbios_knl_Clock_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_Module__loggerFxn4__C, ".const:ti_sysbios_knl_Clock_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_knl_Clock_Module__loggerFxn4 ti_sysbios_knl_Clock_Module__loggerFxn4__C = ((CT__ti_sysbios_knl_Clock_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_Module__loggerFxn8__C, ".const:ti_sysbios_knl_Clock_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_knl_Clock_Module__loggerFxn8 ti_sysbios_knl_Clock_Module__loggerFxn8__C = ((CT__ti_sysbios_knl_Clock_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_Object__count__C, ".const:ti_sysbios_knl_Clock_Object__count__C");
__FAR__ const CT__ti_sysbios_knl_Clock_Object__count ti_sysbios_knl_Clock_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_Object__heap__C, ".const:ti_sysbios_knl_Clock_Object__heap__C");
__FAR__ const CT__ti_sysbios_knl_Clock_Object__heap ti_sysbios_knl_Clock_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_Object__sizeof__C, ".const:ti_sysbios_knl_Clock_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_knl_Clock_Object__sizeof ti_sysbios_knl_Clock_Object__sizeof__C = sizeof(ti_sysbios_knl_Clock_Object__);

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_Object__table__C, ".const:ti_sysbios_knl_Clock_Object__table__C");
__FAR__ const CT__ti_sysbios_knl_Clock_Object__table ti_sysbios_knl_Clock_Object__table__C = 0;

/* LW_delayed__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_LW_delayed__C, ".const:ti_sysbios_knl_Clock_LW_delayed__C");
__FAR__ const CT__ti_sysbios_knl_Clock_LW_delayed ti_sysbios_knl_Clock_LW_delayed__C = (((xdc_runtime_Log_Event)5484) << 16 | 1024);

/* LM_tick__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_LM_tick__C, ".const:ti_sysbios_knl_Clock_LM_tick__C");
__FAR__ const CT__ti_sysbios_knl_Clock_LM_tick ti_sysbios_knl_Clock_LM_tick__C = (((xdc_runtime_Log_Event)5506) << 16 | 768);

/* LM_begin__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_LM_begin__C, ".const:ti_sysbios_knl_Clock_LM_begin__C");
__FAR__ const CT__ti_sysbios_knl_Clock_LM_begin ti_sysbios_knl_Clock_LM_begin__C = (((xdc_runtime_Log_Event)5524) << 16 | 768);

/* A_clockDisabled__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_A_clockDisabled__C, ".const:ti_sysbios_knl_Clock_A_clockDisabled__C");
__FAR__ const CT__ti_sysbios_knl_Clock_A_clockDisabled ti_sysbios_knl_Clock_A_clockDisabled__C = (((xdc_runtime_Assert_Id)615) << 16 | 16);

/* A_badThreadType__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_A_badThreadType__C, ".const:ti_sysbios_knl_Clock_A_badThreadType__C");
__FAR__ const CT__ti_sysbios_knl_Clock_A_badThreadType ti_sysbios_knl_Clock_A_badThreadType__C = (((xdc_runtime_Assert_Id)696) << 16 | 16);

/* serviceMargin__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_serviceMargin__C, ".const:ti_sysbios_knl_Clock_serviceMargin__C");
__FAR__ const CT__ti_sysbios_knl_Clock_serviceMargin ti_sysbios_knl_Clock_serviceMargin__C = (xdc_UInt32)0x0;

/* tickSource__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_tickSource__C, ".const:ti_sysbios_knl_Clock_tickSource__C");
__FAR__ const CT__ti_sysbios_knl_Clock_tickSource ti_sysbios_knl_Clock_tickSource__C = ti_sysbios_knl_Clock_TickSource_TIMER;

/* tickMode__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_tickMode__C, ".const:ti_sysbios_knl_Clock_tickMode__C");
__FAR__ const CT__ti_sysbios_knl_Clock_tickMode ti_sysbios_knl_Clock_tickMode__C = ti_sysbios_knl_Clock_TickMode_PERIODIC;

/* timerId__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_timerId__C, ".const:ti_sysbios_knl_Clock_timerId__C");
__FAR__ const CT__ti_sysbios_knl_Clock_timerId ti_sysbios_knl_Clock_timerId__C = (xdc_UInt)(-0x0 - 1);

/* tickPeriod__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_tickPeriod__C, ".const:ti_sysbios_knl_Clock_tickPeriod__C");
__FAR__ const CT__ti_sysbios_knl_Clock_tickPeriod ti_sysbios_knl_Clock_tickPeriod__C = (xdc_UInt32)0x3e8;

/* doTickFunc__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_doTickFunc__C, ".const:ti_sysbios_knl_Clock_doTickFunc__C");
__FAR__ const CT__ti_sysbios_knl_Clock_doTickFunc ti_sysbios_knl_Clock_doTickFunc__C = ((CT__ti_sysbios_knl_Clock_doTickFunc)((xdc_Fxn)ti_sysbios_knl_Clock_doTick__I));

/* triggerClock__C */
#pragma DATA_SECTION(ti_sysbios_knl_Clock_triggerClock__C, ".const:ti_sysbios_knl_Clock_triggerClock__C");
__FAR__ const CT__ti_sysbios_knl_Clock_triggerClock ti_sysbios_knl_Clock_triggerClock__C = 0;


/*
 * ======== ti.sysbios.knl.Clock_TimerProxy INITIALIZERS ========
 */


/*
 * ======== ti.sysbios.knl.Event INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_knl_Event_Object__DESC__C;

/* Object__PARAMS__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_Object__PARAMS__C, ".const:ti_sysbios_knl_Event_Object__PARAMS__C");
__FAR__ const ti_sysbios_knl_Event_Params ti_sysbios_knl_Event_Object__PARAMS__C = {
    sizeof (ti_sysbios_knl_Event_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&ti_sysbios_knl_Event_Object__PARAMS__C.__iprms, /* instance */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
ti_sysbios_knl_Event_Module__ ti_sysbios_knl_Event_Module__root__V = {
    {&ti_sysbios_knl_Event_Module__root__V.link,  /* link.next */
    &ti_sysbios_knl_Event_Module__root__V.link},  /* link.prev */
};

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_Module__diagsEnabled__C, ".const:ti_sysbios_knl_Event_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_knl_Event_Module__diagsEnabled ti_sysbios_knl_Event_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_Module__diagsIncluded__C, ".const:ti_sysbios_knl_Event_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_knl_Event_Module__diagsIncluded ti_sysbios_knl_Event_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_Module__diagsMask__C, ".const:ti_sysbios_knl_Event_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_knl_Event_Module__diagsMask ti_sysbios_knl_Event_Module__diagsMask__C = ((CT__ti_sysbios_knl_Event_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_Module__gateObj__C, ".const:ti_sysbios_knl_Event_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_knl_Event_Module__gateObj ti_sysbios_knl_Event_Module__gateObj__C = ((CT__ti_sysbios_knl_Event_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_Module__gatePrms__C, ".const:ti_sysbios_knl_Event_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_knl_Event_Module__gatePrms ti_sysbios_knl_Event_Module__gatePrms__C = ((CT__ti_sysbios_knl_Event_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_Module__id__C, ".const:ti_sysbios_knl_Event_Module__id__C");
__FAR__ const CT__ti_sysbios_knl_Event_Module__id ti_sysbios_knl_Event_Module__id__C = (xdc_Bits16)0x801a;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_Module__loggerDefined__C, ".const:ti_sysbios_knl_Event_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_knl_Event_Module__loggerDefined ti_sysbios_knl_Event_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_Module__loggerObj__C, ".const:ti_sysbios_knl_Event_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_knl_Event_Module__loggerObj ti_sysbios_knl_Event_Module__loggerObj__C = ((CT__ti_sysbios_knl_Event_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_Module__loggerFxn0__C, ".const:ti_sysbios_knl_Event_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_knl_Event_Module__loggerFxn0 ti_sysbios_knl_Event_Module__loggerFxn0__C = ((CT__ti_sysbios_knl_Event_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_Module__loggerFxn1__C, ".const:ti_sysbios_knl_Event_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_knl_Event_Module__loggerFxn1 ti_sysbios_knl_Event_Module__loggerFxn1__C = ((CT__ti_sysbios_knl_Event_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_Module__loggerFxn2__C, ".const:ti_sysbios_knl_Event_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_knl_Event_Module__loggerFxn2 ti_sysbios_knl_Event_Module__loggerFxn2__C = ((CT__ti_sysbios_knl_Event_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_Module__loggerFxn4__C, ".const:ti_sysbios_knl_Event_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_knl_Event_Module__loggerFxn4 ti_sysbios_knl_Event_Module__loggerFxn4__C = ((CT__ti_sysbios_knl_Event_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_Module__loggerFxn8__C, ".const:ti_sysbios_knl_Event_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_knl_Event_Module__loggerFxn8 ti_sysbios_knl_Event_Module__loggerFxn8__C = ((CT__ti_sysbios_knl_Event_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_Object__count__C, ".const:ti_sysbios_knl_Event_Object__count__C");
__FAR__ const CT__ti_sysbios_knl_Event_Object__count ti_sysbios_knl_Event_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_Object__heap__C, ".const:ti_sysbios_knl_Event_Object__heap__C");
__FAR__ const CT__ti_sysbios_knl_Event_Object__heap ti_sysbios_knl_Event_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_Object__sizeof__C, ".const:ti_sysbios_knl_Event_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_knl_Event_Object__sizeof ti_sysbios_knl_Event_Object__sizeof__C = sizeof(ti_sysbios_knl_Event_Object__);

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_Object__table__C, ".const:ti_sysbios_knl_Event_Object__table__C");
__FAR__ const CT__ti_sysbios_knl_Event_Object__table ti_sysbios_knl_Event_Object__table__C = 0;

/* LM_post__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_LM_post__C, ".const:ti_sysbios_knl_Event_LM_post__C");
__FAR__ const CT__ti_sysbios_knl_Event_LM_post ti_sysbios_knl_Event_LM_post__C = (((xdc_runtime_Log_Event)5556) << 16 | 768);

/* LM_pend__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_LM_pend__C, ".const:ti_sysbios_knl_Event_LM_pend__C");
__FAR__ const CT__ti_sysbios_knl_Event_LM_pend ti_sysbios_knl_Event_LM_pend__C = (((xdc_runtime_Log_Event)5610) << 16 | 768);

/* A_nullEventMasks__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_A_nullEventMasks__C, ".const:ti_sysbios_knl_Event_A_nullEventMasks__C");
__FAR__ const CT__ti_sysbios_knl_Event_A_nullEventMasks ti_sysbios_knl_Event_A_nullEventMasks__C = (((xdc_runtime_Assert_Id)766) << 16 | 16);

/* A_nullEventId__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_A_nullEventId__C, ".const:ti_sysbios_knl_Event_A_nullEventId__C");
__FAR__ const CT__ti_sysbios_knl_Event_A_nullEventId ti_sysbios_knl_Event_A_nullEventId__C = (((xdc_runtime_Assert_Id)813) << 16 | 16);

/* A_eventInUse__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_A_eventInUse__C, ".const:ti_sysbios_knl_Event_A_eventInUse__C");
__FAR__ const CT__ti_sysbios_knl_Event_A_eventInUse ti_sysbios_knl_Event_A_eventInUse__C = (((xdc_runtime_Assert_Id)852) << 16 | 16);

/* A_badContext__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_A_badContext__C, ".const:ti_sysbios_knl_Event_A_badContext__C");
__FAR__ const CT__ti_sysbios_knl_Event_A_badContext ti_sysbios_knl_Event_A_badContext__C = (((xdc_runtime_Assert_Id)895) << 16 | 16);

/* A_pendTaskDisabled__C */
#pragma DATA_SECTION(ti_sysbios_knl_Event_A_pendTaskDisabled__C, ".const:ti_sysbios_knl_Event_A_pendTaskDisabled__C");
__FAR__ const CT__ti_sysbios_knl_Event_A_pendTaskDisabled ti_sysbios_knl_Event_A_pendTaskDisabled__C = (((xdc_runtime_Assert_Id)958) << 16 | 16);


/*
 * ======== ti.sysbios.knl.Idle INITIALIZERS ========
 */

/* --> ti_sysbios_knl_Idle_funcList__A */
#pragma DATA_SECTION(ti_sysbios_knl_Idle_funcList__A, ".const:ti_sysbios_knl_Idle_funcList__A");
const __T1_ti_sysbios_knl_Idle_funcList ti_sysbios_knl_Idle_funcList__A[1] = {
    ((xdc_Void(*)(xdc_Void))((xdc_Fxn)ti_sysbios_hal_Hwi_checkStack)),  /* [0] */
};

/* --> ti_sysbios_knl_Idle_coreList__A */
#pragma DATA_SECTION(ti_sysbios_knl_Idle_coreList__A, ".const:ti_sysbios_knl_Idle_coreList__A");
const __T1_ti_sysbios_knl_Idle_coreList ti_sysbios_knl_Idle_coreList__A[1] = {
    (xdc_UInt)0x0,  /* [0] */
};

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_knl_Idle_Module__diagsEnabled__C, ".const:ti_sysbios_knl_Idle_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_knl_Idle_Module__diagsEnabled ti_sysbios_knl_Idle_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_knl_Idle_Module__diagsIncluded__C, ".const:ti_sysbios_knl_Idle_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_knl_Idle_Module__diagsIncluded ti_sysbios_knl_Idle_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_knl_Idle_Module__diagsMask__C, ".const:ti_sysbios_knl_Idle_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_knl_Idle_Module__diagsMask ti_sysbios_knl_Idle_Module__diagsMask__C = ((CT__ti_sysbios_knl_Idle_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_knl_Idle_Module__gateObj__C, ".const:ti_sysbios_knl_Idle_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_knl_Idle_Module__gateObj ti_sysbios_knl_Idle_Module__gateObj__C = ((CT__ti_sysbios_knl_Idle_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_knl_Idle_Module__gatePrms__C, ".const:ti_sysbios_knl_Idle_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_knl_Idle_Module__gatePrms ti_sysbios_knl_Idle_Module__gatePrms__C = ((CT__ti_sysbios_knl_Idle_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_knl_Idle_Module__id__C, ".const:ti_sysbios_knl_Idle_Module__id__C");
__FAR__ const CT__ti_sysbios_knl_Idle_Module__id ti_sysbios_knl_Idle_Module__id__C = (xdc_Bits16)0x8018;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_knl_Idle_Module__loggerDefined__C, ".const:ti_sysbios_knl_Idle_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_knl_Idle_Module__loggerDefined ti_sysbios_knl_Idle_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_knl_Idle_Module__loggerObj__C, ".const:ti_sysbios_knl_Idle_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_knl_Idle_Module__loggerObj ti_sysbios_knl_Idle_Module__loggerObj__C = ((CT__ti_sysbios_knl_Idle_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_knl_Idle_Module__loggerFxn0__C, ".const:ti_sysbios_knl_Idle_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_knl_Idle_Module__loggerFxn0 ti_sysbios_knl_Idle_Module__loggerFxn0__C = ((CT__ti_sysbios_knl_Idle_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_knl_Idle_Module__loggerFxn1__C, ".const:ti_sysbios_knl_Idle_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_knl_Idle_Module__loggerFxn1 ti_sysbios_knl_Idle_Module__loggerFxn1__C = ((CT__ti_sysbios_knl_Idle_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_knl_Idle_Module__loggerFxn2__C, ".const:ti_sysbios_knl_Idle_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_knl_Idle_Module__loggerFxn2 ti_sysbios_knl_Idle_Module__loggerFxn2__C = ((CT__ti_sysbios_knl_Idle_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_knl_Idle_Module__loggerFxn4__C, ".const:ti_sysbios_knl_Idle_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_knl_Idle_Module__loggerFxn4 ti_sysbios_knl_Idle_Module__loggerFxn4__C = ((CT__ti_sysbios_knl_Idle_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_knl_Idle_Module__loggerFxn8__C, ".const:ti_sysbios_knl_Idle_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_knl_Idle_Module__loggerFxn8 ti_sysbios_knl_Idle_Module__loggerFxn8__C = ((CT__ti_sysbios_knl_Idle_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_knl_Idle_Object__count__C, ".const:ti_sysbios_knl_Idle_Object__count__C");
__FAR__ const CT__ti_sysbios_knl_Idle_Object__count ti_sysbios_knl_Idle_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_knl_Idle_Object__heap__C, ".const:ti_sysbios_knl_Idle_Object__heap__C");
__FAR__ const CT__ti_sysbios_knl_Idle_Object__heap ti_sysbios_knl_Idle_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_knl_Idle_Object__sizeof__C, ".const:ti_sysbios_knl_Idle_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_knl_Idle_Object__sizeof ti_sysbios_knl_Idle_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_knl_Idle_Object__table__C, ".const:ti_sysbios_knl_Idle_Object__table__C");
__FAR__ const CT__ti_sysbios_knl_Idle_Object__table ti_sysbios_knl_Idle_Object__table__C = 0;

/* funcList__C */
#pragma DATA_SECTION(ti_sysbios_knl_Idle_funcList__C, ".const:ti_sysbios_knl_Idle_funcList__C");
__FAR__ const CT__ti_sysbios_knl_Idle_funcList ti_sysbios_knl_Idle_funcList__C = {1, ((__T1_ti_sysbios_knl_Idle_funcList *)ti_sysbios_knl_Idle_funcList__A)};

/* coreList__C */
#pragma DATA_SECTION(ti_sysbios_knl_Idle_coreList__C, ".const:ti_sysbios_knl_Idle_coreList__C");
__FAR__ const CT__ti_sysbios_knl_Idle_coreList ti_sysbios_knl_Idle_coreList__C = {1, ((__T1_ti_sysbios_knl_Idle_coreList *)ti_sysbios_knl_Idle_coreList__A)};


/*
 * ======== ti.sysbios.knl.Intrinsics INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_knl_Intrinsics_Module__diagsEnabled__C, ".const:ti_sysbios_knl_Intrinsics_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_knl_Intrinsics_Module__diagsEnabled ti_sysbios_knl_Intrinsics_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_knl_Intrinsics_Module__diagsIncluded__C, ".const:ti_sysbios_knl_Intrinsics_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_knl_Intrinsics_Module__diagsIncluded ti_sysbios_knl_Intrinsics_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_knl_Intrinsics_Module__diagsMask__C, ".const:ti_sysbios_knl_Intrinsics_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_knl_Intrinsics_Module__diagsMask ti_sysbios_knl_Intrinsics_Module__diagsMask__C = ((CT__ti_sysbios_knl_Intrinsics_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_knl_Intrinsics_Module__gateObj__C, ".const:ti_sysbios_knl_Intrinsics_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_knl_Intrinsics_Module__gateObj ti_sysbios_knl_Intrinsics_Module__gateObj__C = ((CT__ti_sysbios_knl_Intrinsics_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_knl_Intrinsics_Module__gatePrms__C, ".const:ti_sysbios_knl_Intrinsics_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_knl_Intrinsics_Module__gatePrms ti_sysbios_knl_Intrinsics_Module__gatePrms__C = ((CT__ti_sysbios_knl_Intrinsics_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_knl_Intrinsics_Module__id__C, ".const:ti_sysbios_knl_Intrinsics_Module__id__C");
__FAR__ const CT__ti_sysbios_knl_Intrinsics_Module__id ti_sysbios_knl_Intrinsics_Module__id__C = (xdc_Bits16)0x8019;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_knl_Intrinsics_Module__loggerDefined__C, ".const:ti_sysbios_knl_Intrinsics_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_knl_Intrinsics_Module__loggerDefined ti_sysbios_knl_Intrinsics_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_knl_Intrinsics_Module__loggerObj__C, ".const:ti_sysbios_knl_Intrinsics_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_knl_Intrinsics_Module__loggerObj ti_sysbios_knl_Intrinsics_Module__loggerObj__C = ((CT__ti_sysbios_knl_Intrinsics_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_knl_Intrinsics_Module__loggerFxn0__C, ".const:ti_sysbios_knl_Intrinsics_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_knl_Intrinsics_Module__loggerFxn0 ti_sysbios_knl_Intrinsics_Module__loggerFxn0__C = ((CT__ti_sysbios_knl_Intrinsics_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_knl_Intrinsics_Module__loggerFxn1__C, ".const:ti_sysbios_knl_Intrinsics_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_knl_Intrinsics_Module__loggerFxn1 ti_sysbios_knl_Intrinsics_Module__loggerFxn1__C = ((CT__ti_sysbios_knl_Intrinsics_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_knl_Intrinsics_Module__loggerFxn2__C, ".const:ti_sysbios_knl_Intrinsics_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_knl_Intrinsics_Module__loggerFxn2 ti_sysbios_knl_Intrinsics_Module__loggerFxn2__C = ((CT__ti_sysbios_knl_Intrinsics_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_knl_Intrinsics_Module__loggerFxn4__C, ".const:ti_sysbios_knl_Intrinsics_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_knl_Intrinsics_Module__loggerFxn4 ti_sysbios_knl_Intrinsics_Module__loggerFxn4__C = ((CT__ti_sysbios_knl_Intrinsics_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_knl_Intrinsics_Module__loggerFxn8__C, ".const:ti_sysbios_knl_Intrinsics_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_knl_Intrinsics_Module__loggerFxn8 ti_sysbios_knl_Intrinsics_Module__loggerFxn8__C = ((CT__ti_sysbios_knl_Intrinsics_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_knl_Intrinsics_Object__count__C, ".const:ti_sysbios_knl_Intrinsics_Object__count__C");
__FAR__ const CT__ti_sysbios_knl_Intrinsics_Object__count ti_sysbios_knl_Intrinsics_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_knl_Intrinsics_Object__heap__C, ".const:ti_sysbios_knl_Intrinsics_Object__heap__C");
__FAR__ const CT__ti_sysbios_knl_Intrinsics_Object__heap ti_sysbios_knl_Intrinsics_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_knl_Intrinsics_Object__sizeof__C, ".const:ti_sysbios_knl_Intrinsics_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_knl_Intrinsics_Object__sizeof ti_sysbios_knl_Intrinsics_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_knl_Intrinsics_Object__table__C, ".const:ti_sysbios_knl_Intrinsics_Object__table__C");
__FAR__ const CT__ti_sysbios_knl_Intrinsics_Object__table ti_sysbios_knl_Intrinsics_Object__table__C = 0;


/*
 * ======== ti.sysbios.knl.Intrinsics_SupportProxy INITIALIZERS ========
 */


/*
 * ======== ti.sysbios.knl.Queue INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_knl_Queue_Object__DESC__C;

/* Object__PARAMS__C */
#pragma DATA_SECTION(ti_sysbios_knl_Queue_Object__PARAMS__C, ".const:ti_sysbios_knl_Queue_Object__PARAMS__C");
__FAR__ const ti_sysbios_knl_Queue_Params ti_sysbios_knl_Queue_Object__PARAMS__C = {
    sizeof (ti_sysbios_knl_Queue_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&ti_sysbios_knl_Queue_Object__PARAMS__C.__iprms, /* instance */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
ti_sysbios_knl_Queue_Module__ ti_sysbios_knl_Queue_Module__root__V = {
    {&ti_sysbios_knl_Queue_Module__root__V.link,  /* link.next */
    &ti_sysbios_knl_Queue_Module__root__V.link},  /* link.prev */
};

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_knl_Queue_Module__diagsEnabled__C, ".const:ti_sysbios_knl_Queue_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_knl_Queue_Module__diagsEnabled ti_sysbios_knl_Queue_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_knl_Queue_Module__diagsIncluded__C, ".const:ti_sysbios_knl_Queue_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_knl_Queue_Module__diagsIncluded ti_sysbios_knl_Queue_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_knl_Queue_Module__diagsMask__C, ".const:ti_sysbios_knl_Queue_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_knl_Queue_Module__diagsMask ti_sysbios_knl_Queue_Module__diagsMask__C = ((CT__ti_sysbios_knl_Queue_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_knl_Queue_Module__gateObj__C, ".const:ti_sysbios_knl_Queue_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_knl_Queue_Module__gateObj ti_sysbios_knl_Queue_Module__gateObj__C = ((CT__ti_sysbios_knl_Queue_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_knl_Queue_Module__gatePrms__C, ".const:ti_sysbios_knl_Queue_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_knl_Queue_Module__gatePrms ti_sysbios_knl_Queue_Module__gatePrms__C = ((CT__ti_sysbios_knl_Queue_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_knl_Queue_Module__id__C, ".const:ti_sysbios_knl_Queue_Module__id__C");
__FAR__ const CT__ti_sysbios_knl_Queue_Module__id ti_sysbios_knl_Queue_Module__id__C = (xdc_Bits16)0x801b;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_knl_Queue_Module__loggerDefined__C, ".const:ti_sysbios_knl_Queue_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_knl_Queue_Module__loggerDefined ti_sysbios_knl_Queue_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_knl_Queue_Module__loggerObj__C, ".const:ti_sysbios_knl_Queue_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_knl_Queue_Module__loggerObj ti_sysbios_knl_Queue_Module__loggerObj__C = ((CT__ti_sysbios_knl_Queue_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_knl_Queue_Module__loggerFxn0__C, ".const:ti_sysbios_knl_Queue_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_knl_Queue_Module__loggerFxn0 ti_sysbios_knl_Queue_Module__loggerFxn0__C = ((CT__ti_sysbios_knl_Queue_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_knl_Queue_Module__loggerFxn1__C, ".const:ti_sysbios_knl_Queue_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_knl_Queue_Module__loggerFxn1 ti_sysbios_knl_Queue_Module__loggerFxn1__C = ((CT__ti_sysbios_knl_Queue_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_knl_Queue_Module__loggerFxn2__C, ".const:ti_sysbios_knl_Queue_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_knl_Queue_Module__loggerFxn2 ti_sysbios_knl_Queue_Module__loggerFxn2__C = ((CT__ti_sysbios_knl_Queue_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_knl_Queue_Module__loggerFxn4__C, ".const:ti_sysbios_knl_Queue_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_knl_Queue_Module__loggerFxn4 ti_sysbios_knl_Queue_Module__loggerFxn4__C = ((CT__ti_sysbios_knl_Queue_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_knl_Queue_Module__loggerFxn8__C, ".const:ti_sysbios_knl_Queue_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_knl_Queue_Module__loggerFxn8 ti_sysbios_knl_Queue_Module__loggerFxn8__C = ((CT__ti_sysbios_knl_Queue_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_knl_Queue_Object__count__C, ".const:ti_sysbios_knl_Queue_Object__count__C");
__FAR__ const CT__ti_sysbios_knl_Queue_Object__count ti_sysbios_knl_Queue_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_knl_Queue_Object__heap__C, ".const:ti_sysbios_knl_Queue_Object__heap__C");
__FAR__ const CT__ti_sysbios_knl_Queue_Object__heap ti_sysbios_knl_Queue_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_knl_Queue_Object__sizeof__C, ".const:ti_sysbios_knl_Queue_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_knl_Queue_Object__sizeof ti_sysbios_knl_Queue_Object__sizeof__C = sizeof(ti_sysbios_knl_Queue_Object__);

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_knl_Queue_Object__table__C, ".const:ti_sysbios_knl_Queue_Object__table__C");
__FAR__ const CT__ti_sysbios_knl_Queue_Object__table ti_sysbios_knl_Queue_Object__table__C = 0;


/*
 * ======== ti.sysbios.knl.Semaphore INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_knl_Semaphore_Object__DESC__C;

/* Object__PARAMS__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_Object__PARAMS__C, ".const:ti_sysbios_knl_Semaphore_Object__PARAMS__C");
__FAR__ const ti_sysbios_knl_Semaphore_Params ti_sysbios_knl_Semaphore_Object__PARAMS__C = {
    sizeof (ti_sysbios_knl_Semaphore_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&ti_sysbios_knl_Semaphore_Object__PARAMS__C.__iprms, /* instance */
    0,  /* event */
    (xdc_UInt)0x1,  /* eventId */
    ti_sysbios_knl_Semaphore_Mode_COUNTING,  /* mode */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
ti_sysbios_knl_Semaphore_Module__ ti_sysbios_knl_Semaphore_Module__root__V = {
    {&ti_sysbios_knl_Semaphore_Module__root__V.link,  /* link.next */
    &ti_sysbios_knl_Semaphore_Module__root__V.link},  /* link.prev */
};

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_Module__diagsEnabled__C, ".const:ti_sysbios_knl_Semaphore_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_Module__diagsEnabled ti_sysbios_knl_Semaphore_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_Module__diagsIncluded__C, ".const:ti_sysbios_knl_Semaphore_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_Module__diagsIncluded ti_sysbios_knl_Semaphore_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_Module__diagsMask__C, ".const:ti_sysbios_knl_Semaphore_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_Module__diagsMask ti_sysbios_knl_Semaphore_Module__diagsMask__C = ((CT__ti_sysbios_knl_Semaphore_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_Module__gateObj__C, ".const:ti_sysbios_knl_Semaphore_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_Module__gateObj ti_sysbios_knl_Semaphore_Module__gateObj__C = ((CT__ti_sysbios_knl_Semaphore_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_Module__gatePrms__C, ".const:ti_sysbios_knl_Semaphore_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_Module__gatePrms ti_sysbios_knl_Semaphore_Module__gatePrms__C = ((CT__ti_sysbios_knl_Semaphore_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_Module__id__C, ".const:ti_sysbios_knl_Semaphore_Module__id__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_Module__id ti_sysbios_knl_Semaphore_Module__id__C = (xdc_Bits16)0x801c;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_Module__loggerDefined__C, ".const:ti_sysbios_knl_Semaphore_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_Module__loggerDefined ti_sysbios_knl_Semaphore_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_Module__loggerObj__C, ".const:ti_sysbios_knl_Semaphore_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_Module__loggerObj ti_sysbios_knl_Semaphore_Module__loggerObj__C = ((CT__ti_sysbios_knl_Semaphore_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_Module__loggerFxn0__C, ".const:ti_sysbios_knl_Semaphore_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_Module__loggerFxn0 ti_sysbios_knl_Semaphore_Module__loggerFxn0__C = ((CT__ti_sysbios_knl_Semaphore_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_Module__loggerFxn1__C, ".const:ti_sysbios_knl_Semaphore_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_Module__loggerFxn1 ti_sysbios_knl_Semaphore_Module__loggerFxn1__C = ((CT__ti_sysbios_knl_Semaphore_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_Module__loggerFxn2__C, ".const:ti_sysbios_knl_Semaphore_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_Module__loggerFxn2 ti_sysbios_knl_Semaphore_Module__loggerFxn2__C = ((CT__ti_sysbios_knl_Semaphore_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_Module__loggerFxn4__C, ".const:ti_sysbios_knl_Semaphore_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_Module__loggerFxn4 ti_sysbios_knl_Semaphore_Module__loggerFxn4__C = ((CT__ti_sysbios_knl_Semaphore_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_Module__loggerFxn8__C, ".const:ti_sysbios_knl_Semaphore_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_Module__loggerFxn8 ti_sysbios_knl_Semaphore_Module__loggerFxn8__C = ((CT__ti_sysbios_knl_Semaphore_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_Object__count__C, ".const:ti_sysbios_knl_Semaphore_Object__count__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_Object__count ti_sysbios_knl_Semaphore_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_Object__heap__C, ".const:ti_sysbios_knl_Semaphore_Object__heap__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_Object__heap ti_sysbios_knl_Semaphore_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_Object__sizeof__C, ".const:ti_sysbios_knl_Semaphore_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_Object__sizeof ti_sysbios_knl_Semaphore_Object__sizeof__C = sizeof(ti_sysbios_knl_Semaphore_Object__);

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_Object__table__C, ".const:ti_sysbios_knl_Semaphore_Object__table__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_Object__table ti_sysbios_knl_Semaphore_Object__table__C = 0;

/* LM_post__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_LM_post__C, ".const:ti_sysbios_knl_Semaphore_LM_post__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_LM_post ti_sysbios_knl_Semaphore_LM_post__C = (((xdc_runtime_Log_Event)5691) << 16 | 768);

/* LM_pend__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_LM_pend__C, ".const:ti_sysbios_knl_Semaphore_LM_pend__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_LM_pend ti_sysbios_knl_Semaphore_LM_pend__C = (((xdc_runtime_Log_Event)5721) << 16 | 768);

/* A_noEvents__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_A_noEvents__C, ".const:ti_sysbios_knl_Semaphore_A_noEvents__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_A_noEvents ti_sysbios_knl_Semaphore_A_noEvents__C = (((xdc_runtime_Assert_Id)1106) << 16 | 16);

/* A_invTimeout__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_A_invTimeout__C, ".const:ti_sysbios_knl_Semaphore_A_invTimeout__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_A_invTimeout ti_sysbios_knl_Semaphore_A_invTimeout__C = (((xdc_runtime_Assert_Id)1161) << 16 | 16);

/* A_badContext__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_A_badContext__C, ".const:ti_sysbios_knl_Semaphore_A_badContext__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_A_badContext ti_sysbios_knl_Semaphore_A_badContext__C = (((xdc_runtime_Assert_Id)895) << 16 | 16);

/* A_overflow__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_A_overflow__C, ".const:ti_sysbios_knl_Semaphore_A_overflow__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_A_overflow ti_sysbios_knl_Semaphore_A_overflow__C = (((xdc_runtime_Assert_Id)1226) << 16 | 16);

/* A_pendTaskDisabled__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_A_pendTaskDisabled__C, ".const:ti_sysbios_knl_Semaphore_A_pendTaskDisabled__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_A_pendTaskDisabled ti_sysbios_knl_Semaphore_A_pendTaskDisabled__C = (((xdc_runtime_Assert_Id)1280) << 16 | 16);

/* E_objectNotInKernelSpace__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_E_objectNotInKernelSpace__C, ".const:ti_sysbios_knl_Semaphore_E_objectNotInKernelSpace__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_E_objectNotInKernelSpace ti_sysbios_knl_Semaphore_E_objectNotInKernelSpace__C = (((xdc_runtime_Error_Id)4068) << 16 | 0);

/* supportsEvents__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_supportsEvents__C, ".const:ti_sysbios_knl_Semaphore_supportsEvents__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_supportsEvents ti_sysbios_knl_Semaphore_supportsEvents__C = 0;

/* supportsPriority__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_supportsPriority__C, ".const:ti_sysbios_knl_Semaphore_supportsPriority__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_supportsPriority ti_sysbios_knl_Semaphore_supportsPriority__C = 1;

/* eventPost__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_eventPost__C, ".const:ti_sysbios_knl_Semaphore_eventPost__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_eventPost ti_sysbios_knl_Semaphore_eventPost__C = ((CT__ti_sysbios_knl_Semaphore_eventPost)0);

/* eventSync__C */
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_eventSync__C, ".const:ti_sysbios_knl_Semaphore_eventSync__C");
__FAR__ const CT__ti_sysbios_knl_Semaphore_eventSync ti_sysbios_knl_Semaphore_eventSync__C = ((CT__ti_sysbios_knl_Semaphore_eventSync)0);


/*
 * ======== ti.sysbios.knl.Swi INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_knl_Swi_Object__DESC__C;

/* Object__PARAMS__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_Object__PARAMS__C, ".const:ti_sysbios_knl_Swi_Object__PARAMS__C");
__FAR__ const ti_sysbios_knl_Swi_Params ti_sysbios_knl_Swi_Object__PARAMS__C = {
    sizeof (ti_sysbios_knl_Swi_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&ti_sysbios_knl_Swi_Object__PARAMS__C.__iprms, /* instance */
    ((xdc_UArg)(0x0)),  /* arg0 */
    ((xdc_UArg)(0x0)),  /* arg1 */
    (xdc_UInt)(-0x0 - 1),  /* priority */
    (xdc_UInt)0x0,  /* trigger */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
ti_sysbios_knl_Swi_Module__ ti_sysbios_knl_Swi_Module__root__V = {
    {&ti_sysbios_knl_Swi_Module__root__V.link,  /* link.next */
    &ti_sysbios_knl_Swi_Module__root__V.link},  /* link.prev */
};

/* Object__table__V */
ti_sysbios_knl_Swi_Object__ ti_sysbios_knl_Swi_Object__table__V[1] = {
    {/* instance#0 */
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Object__table__V[0].qElem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Object__table__V[0].qElem)),  /* prev */
        },  /* qElem */
        ((xdc_Void(*)(xdc_UArg,xdc_UArg))((xdc_Fxn)ti_sysbios_knl_Clock_workFunc__E)),  /* fxn */
        ((xdc_UArg)(0x0)),  /* arg0 */
        ((xdc_UArg)(0x0)),  /* arg1 */
        (xdc_UInt)0xf,  /* priority */
        (xdc_UInt)0x8000,  /* mask */
        0,  /* posted */
        (xdc_UInt)0x0,  /* initTrigger */
        (xdc_UInt)0x0,  /* trigger */
        (ti_sysbios_knl_Queue_Handle)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[15],  /* readyQ */
        ((void*)0),  /* hookEnv */
    },
};

/* --> ti_sysbios_knl_Swi_Module_State_0_readyQ__A */
__T1_ti_sysbios_knl_Swi_Module_State__readyQ ti_sysbios_knl_Swi_Module_State_0_readyQ__A[16] = {
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[0].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[0].elem)),  /* prev */
        },  /* elem */
    },  /* [0] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[1].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[1].elem)),  /* prev */
        },  /* elem */
    },  /* [1] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[2].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[2].elem)),  /* prev */
        },  /* elem */
    },  /* [2] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[3].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[3].elem)),  /* prev */
        },  /* elem */
    },  /* [3] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[4].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[4].elem)),  /* prev */
        },  /* elem */
    },  /* [4] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[5].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[5].elem)),  /* prev */
        },  /* elem */
    },  /* [5] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[6].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[6].elem)),  /* prev */
        },  /* elem */
    },  /* [6] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[7].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[7].elem)),  /* prev */
        },  /* elem */
    },  /* [7] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[8].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[8].elem)),  /* prev */
        },  /* elem */
    },  /* [8] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[9].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[9].elem)),  /* prev */
        },  /* elem */
    },  /* [9] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[10].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[10].elem)),  /* prev */
        },  /* elem */
    },  /* [10] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[11].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[11].elem)),  /* prev */
        },  /* elem */
    },  /* [11] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[12].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[12].elem)),  /* prev */
        },  /* elem */
    },  /* [12] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[13].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[13].elem)),  /* prev */
        },  /* elem */
    },  /* [13] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[14].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[14].elem)),  /* prev */
        },  /* elem */
    },  /* [14] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[15].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Swi_Module_State_0_readyQ__A[15].elem)),  /* prev */
        },  /* elem */
    },  /* [15] */
};

/* Module__state__V */
#if defined (__ICCARM__)
#pragma location = ".data_ti_sysbios_knl_Swi_Module__state__V"
#endif
#if defined(__llvm__) && defined(__ti__)
ti_sysbios_knl_Swi_Module_State__ ti_sysbios_knl_Swi_Module__state__V __attribute__ ((section(".data:ti_sysbios_knl_Swi_Module__state__V")));
#elif defined(__GNUC__) && !(defined(__MACH__) && defined(__APPLE__))
#ifndef __TI_COMPILER_VERSION__
ti_sysbios_knl_Swi_Module_State__ ti_sysbios_knl_Swi_Module__state__V __attribute__ ((section(".data_ti_sysbios_knl_Swi_Module__state__V")));
#endif
#endif
ti_sysbios_knl_Swi_Module_State__ ti_sysbios_knl_Swi_Module__state__V = {
    1,  /* locked */
    (xdc_UInt)0x0,  /* curSet */
    (xdc_UInt)0x0,  /* curTrigger */
    0,  /* curSwi */
    0,  /* curQ */
    ((void*)ti_sysbios_knl_Swi_Module_State_0_readyQ__A),  /* readyQ */
    ((void*)0),  /* constructedSwis */
};

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_Module__diagsEnabled__C, ".const:ti_sysbios_knl_Swi_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_knl_Swi_Module__diagsEnabled ti_sysbios_knl_Swi_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_Module__diagsIncluded__C, ".const:ti_sysbios_knl_Swi_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_knl_Swi_Module__diagsIncluded ti_sysbios_knl_Swi_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_Module__diagsMask__C, ".const:ti_sysbios_knl_Swi_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_knl_Swi_Module__diagsMask ti_sysbios_knl_Swi_Module__diagsMask__C = ((CT__ti_sysbios_knl_Swi_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_Module__gateObj__C, ".const:ti_sysbios_knl_Swi_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_knl_Swi_Module__gateObj ti_sysbios_knl_Swi_Module__gateObj__C = ((CT__ti_sysbios_knl_Swi_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_Module__gatePrms__C, ".const:ti_sysbios_knl_Swi_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_knl_Swi_Module__gatePrms ti_sysbios_knl_Swi_Module__gatePrms__C = ((CT__ti_sysbios_knl_Swi_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_Module__id__C, ".const:ti_sysbios_knl_Swi_Module__id__C");
__FAR__ const CT__ti_sysbios_knl_Swi_Module__id ti_sysbios_knl_Swi_Module__id__C = (xdc_Bits16)0x801d;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_Module__loggerDefined__C, ".const:ti_sysbios_knl_Swi_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_knl_Swi_Module__loggerDefined ti_sysbios_knl_Swi_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_Module__loggerObj__C, ".const:ti_sysbios_knl_Swi_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_knl_Swi_Module__loggerObj ti_sysbios_knl_Swi_Module__loggerObj__C = ((CT__ti_sysbios_knl_Swi_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_Module__loggerFxn0__C, ".const:ti_sysbios_knl_Swi_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_knl_Swi_Module__loggerFxn0 ti_sysbios_knl_Swi_Module__loggerFxn0__C = ((CT__ti_sysbios_knl_Swi_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_Module__loggerFxn1__C, ".const:ti_sysbios_knl_Swi_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_knl_Swi_Module__loggerFxn1 ti_sysbios_knl_Swi_Module__loggerFxn1__C = ((CT__ti_sysbios_knl_Swi_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_Module__loggerFxn2__C, ".const:ti_sysbios_knl_Swi_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_knl_Swi_Module__loggerFxn2 ti_sysbios_knl_Swi_Module__loggerFxn2__C = ((CT__ti_sysbios_knl_Swi_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_Module__loggerFxn4__C, ".const:ti_sysbios_knl_Swi_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_knl_Swi_Module__loggerFxn4 ti_sysbios_knl_Swi_Module__loggerFxn4__C = ((CT__ti_sysbios_knl_Swi_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_Module__loggerFxn8__C, ".const:ti_sysbios_knl_Swi_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_knl_Swi_Module__loggerFxn8 ti_sysbios_knl_Swi_Module__loggerFxn8__C = ((CT__ti_sysbios_knl_Swi_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_Object__count__C, ".const:ti_sysbios_knl_Swi_Object__count__C");
__FAR__ const CT__ti_sysbios_knl_Swi_Object__count ti_sysbios_knl_Swi_Object__count__C = 1;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_Object__heap__C, ".const:ti_sysbios_knl_Swi_Object__heap__C");
__FAR__ const CT__ti_sysbios_knl_Swi_Object__heap ti_sysbios_knl_Swi_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_Object__sizeof__C, ".const:ti_sysbios_knl_Swi_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_knl_Swi_Object__sizeof ti_sysbios_knl_Swi_Object__sizeof__C = sizeof(ti_sysbios_knl_Swi_Object__);

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_Object__table__C, ".const:ti_sysbios_knl_Swi_Object__table__C");
__FAR__ const CT__ti_sysbios_knl_Swi_Object__table ti_sysbios_knl_Swi_Object__table__C = ti_sysbios_knl_Swi_Object__table__V;

/* LM_begin__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_LM_begin__C, ".const:ti_sysbios_knl_Swi_LM_begin__C");
__FAR__ const CT__ti_sysbios_knl_Swi_LM_begin ti_sysbios_knl_Swi_LM_begin__C = (((xdc_runtime_Log_Event)5764) << 16 | 768);

/* LD_end__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_LD_end__C, ".const:ti_sysbios_knl_Swi_LD_end__C");
__FAR__ const CT__ti_sysbios_knl_Swi_LD_end ti_sysbios_knl_Swi_LD_end__C = (((xdc_runtime_Log_Event)5811) << 16 | 512);

/* LM_post__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_LM_post__C, ".const:ti_sysbios_knl_Swi_LM_post__C");
__FAR__ const CT__ti_sysbios_knl_Swi_LM_post ti_sysbios_knl_Swi_LM_post__C = (((xdc_runtime_Log_Event)5829) << 16 | 768);

/* A_swiDisabled__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_A_swiDisabled__C, ".const:ti_sysbios_knl_Swi_A_swiDisabled__C");
__FAR__ const CT__ti_sysbios_knl_Swi_A_swiDisabled ti_sysbios_knl_Swi_A_swiDisabled__C = (((xdc_runtime_Assert_Id)1374) << 16 | 16);

/* A_badPriority__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_A_badPriority__C, ".const:ti_sysbios_knl_Swi_A_badPriority__C");
__FAR__ const CT__ti_sysbios_knl_Swi_A_badPriority ti_sysbios_knl_Swi_A_badPriority__C = (((xdc_runtime_Assert_Id)1431) << 16 | 16);

/* numPriorities__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_numPriorities__C, ".const:ti_sysbios_knl_Swi_numPriorities__C");
__FAR__ const CT__ti_sysbios_knl_Swi_numPriorities ti_sysbios_knl_Swi_numPriorities__C = (xdc_UInt)0x10;

/* hooks__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_hooks__C, ".const:ti_sysbios_knl_Swi_hooks__C");
__FAR__ const CT__ti_sysbios_knl_Swi_hooks ti_sysbios_knl_Swi_hooks__C = {0, 0};

/* taskDisable__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_taskDisable__C, ".const:ti_sysbios_knl_Swi_taskDisable__C");
__FAR__ const CT__ti_sysbios_knl_Swi_taskDisable ti_sysbios_knl_Swi_taskDisable__C = ((CT__ti_sysbios_knl_Swi_taskDisable)((xdc_Fxn)ti_sysbios_knl_Task_disable__E));

/* taskRestore__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_taskRestore__C, ".const:ti_sysbios_knl_Swi_taskRestore__C");
__FAR__ const CT__ti_sysbios_knl_Swi_taskRestore ti_sysbios_knl_Swi_taskRestore__C = ((CT__ti_sysbios_knl_Swi_taskRestore)((xdc_Fxn)ti_sysbios_knl_Task_restore__E));

/* numConstructedSwis__C */
#pragma DATA_SECTION(ti_sysbios_knl_Swi_numConstructedSwis__C, ".const:ti_sysbios_knl_Swi_numConstructedSwis__C");
__FAR__ const CT__ti_sysbios_knl_Swi_numConstructedSwis ti_sysbios_knl_Swi_numConstructedSwis__C = (xdc_UInt)0x0;


/*
 * ======== ti.sysbios.knl.Task INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_knl_Task_Object__DESC__C;

/* Object__PARAMS__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_Object__PARAMS__C, ".const:ti_sysbios_knl_Task_Object__PARAMS__C");
__FAR__ const ti_sysbios_knl_Task_Params ti_sysbios_knl_Task_Object__PARAMS__C = {
    sizeof (ti_sysbios_knl_Task_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&ti_sysbios_knl_Task_Object__PARAMS__C.__iprms, /* instance */
    ((xdc_UArg)(0x0)),  /* arg0 */
    ((xdc_UArg)(0x0)),  /* arg1 */
    (xdc_Int)0x1,  /* priority */
    ((xdc_Ptr)0),  /* stack */
    (xdc_SizeT)0x0,  /* stackSize */
    0,  /* stackHeap */
    ((xdc_Ptr)0),  /* env */
    1,  /* vitalTaskFlag */
    (xdc_UInt)0x0,  /* affinity */
    1,  /* privileged */
    ((xdc_Ptr)0),  /* domain */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* --> ti_sysbios_knl_Task_Instance_State_0_stack__A */
__T1_ti_sysbios_knl_Task_Instance_State__stack ti_sysbios_knl_Task_Instance_State_0_stack__A[2048];

/* Module__root__V */
ti_sysbios_knl_Task_Module__ ti_sysbios_knl_Task_Module__root__V = {
    {&ti_sysbios_knl_Task_Module__root__V.link,  /* link.next */
    &ti_sysbios_knl_Task_Module__root__V.link},  /* link.prev */
};

/* Object__table__V */
ti_sysbios_knl_Task_Object__ ti_sysbios_knl_Task_Object__table__V[1] = {
    {/* instance#0 */
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Object__table__V[0].qElem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Object__table__V[0].qElem)),  /* prev */
        },  /* qElem */
        (xdc_Int)0x0,  /* priority */
        (xdc_UInt)0x1,  /* mask */
        ((xdc_Ptr)0),  /* context */
        ti_sysbios_knl_Task_Mode_INACTIVE,  /* mode */
        ((ti_sysbios_knl_Task_PendElem*)0),  /* pendElem */
        (xdc_SizeT)0x800,  /* stackSize */
        ((void*)ti_sysbios_knl_Task_Instance_State_0_stack__A),  /* stack */
        0,  /* stackHeap */
        ((xdc_Void(*)(xdc_UArg,xdc_UArg))((xdc_Fxn)ti_sysbios_knl_Idle_loop__E)),  /* fxn */
        ((xdc_UArg)(0x0)),  /* arg0 */
        ((xdc_UArg)(0x0)),  /* arg1 */
        ((xdc_Ptr)0),  /* env */
        ((void*)0),  /* hookEnv */
        1,  /* vitalTaskFlag */
        0,  /* readyQ */
        (xdc_UInt)0x0,  /* curCoreId */
        (xdc_UInt)0x0,  /* affinity */
        1,  /* privileged */
        ((xdc_Ptr)0),  /* domain */
        (xdc_UInt32)0x0,  /* checkValue */
        ((xdc_Ptr)0),  /* tls */
    },
};

/* --> ti_sysbios_knl_Task_Module_State_0_readyQ__A */
__T1_ti_sysbios_knl_Task_Module_State__readyQ ti_sysbios_knl_Task_Module_State_0_readyQ__A[16] = {
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[0].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[0].elem)),  /* prev */
        },  /* elem */
    },  /* [0] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[1].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[1].elem)),  /* prev */
        },  /* elem */
    },  /* [1] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[2].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[2].elem)),  /* prev */
        },  /* elem */
    },  /* [2] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[3].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[3].elem)),  /* prev */
        },  /* elem */
    },  /* [3] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[4].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[4].elem)),  /* prev */
        },  /* elem */
    },  /* [4] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[5].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[5].elem)),  /* prev */
        },  /* elem */
    },  /* [5] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[6].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[6].elem)),  /* prev */
        },  /* elem */
    },  /* [6] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[7].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[7].elem)),  /* prev */
        },  /* elem */
    },  /* [7] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[8].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[8].elem)),  /* prev */
        },  /* elem */
    },  /* [8] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[9].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[9].elem)),  /* prev */
        },  /* elem */
    },  /* [9] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[10].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[10].elem)),  /* prev */
        },  /* elem */
    },  /* [10] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[11].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[11].elem)),  /* prev */
        },  /* elem */
    },  /* [11] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[12].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[12].elem)),  /* prev */
        },  /* elem */
    },  /* [12] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[13].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[13].elem)),  /* prev */
        },  /* elem */
    },  /* [13] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[14].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[14].elem)),  /* prev */
        },  /* elem */
    },  /* [14] */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[15].elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module_State_0_readyQ__A[15].elem)),  /* prev */
        },  /* elem */
    },  /* [15] */
};

/* --> ti_sysbios_knl_Task_Module_State_0_idleTask__A */
__T1_ti_sysbios_knl_Task_Module_State__idleTask ti_sysbios_knl_Task_Module_State_0_idleTask__A[1] = {
    (ti_sysbios_knl_Task_Handle)&ti_sysbios_knl_Task_Object__table__V[0],  /* [0] */
};

/* Module__state__V */
#if defined (__ICCARM__)
#pragma location = ".data_ti_sysbios_knl_Task_Module__state__V"
#endif
#if defined(__llvm__) && defined(__ti__)
ti_sysbios_knl_Task_Module_State__ ti_sysbios_knl_Task_Module__state__V __attribute__ ((section(".data:ti_sysbios_knl_Task_Module__state__V")));
#elif defined(__GNUC__) && !(defined(__MACH__) && defined(__APPLE__))
#ifndef __TI_COMPILER_VERSION__
ti_sysbios_knl_Task_Module_State__ ti_sysbios_knl_Task_Module__state__V __attribute__ ((section(".data_ti_sysbios_knl_Task_Module__state__V")));
#endif
#endif
ti_sysbios_knl_Task_Module_State__ ti_sysbios_knl_Task_Module__state__V = {
    1,  /* locked */
    (xdc_UInt)0x0,  /* curSet */
    0,  /* workFlag */
    (xdc_UInt)0x1,  /* vitalTasks */
    0,  /* curTask */
    0,  /* curQ */
    ((void*)ti_sysbios_knl_Task_Module_State_0_readyQ__A),  /* readyQ */
    ((void*)0),  /* smpCurSet */
    ((void*)0),  /* smpCurMask */
    ((void*)0),  /* smpCurTask */
    ((void*)0),  /* smpReadyQ */
    ((void*)ti_sysbios_knl_Task_Module_State_0_idleTask__A),  /* idleTask */
    ((void*)0),  /* constructedTasks */
    1,  /* curTaskPrivileged */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module__state__V.Object_field_inactiveQ.elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module__state__V.Object_field_inactiveQ.elem)),  /* prev */
        },  /* elem */
    },  /* Object_field_inactiveQ */
    {
        {
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module__state__V.Object_field_terminatedQ.elem)),  /* next */
            ((ti_sysbios_knl_Queue_Elem*)((void*)&ti_sysbios_knl_Task_Module__state__V.Object_field_terminatedQ.elem)),  /* prev */
        },  /* elem */
    },  /* Object_field_terminatedQ */
};

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_Module__diagsEnabled__C, ".const:ti_sysbios_knl_Task_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_knl_Task_Module__diagsEnabled ti_sysbios_knl_Task_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_Module__diagsIncluded__C, ".const:ti_sysbios_knl_Task_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_knl_Task_Module__diagsIncluded ti_sysbios_knl_Task_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_Module__diagsMask__C, ".const:ti_sysbios_knl_Task_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_knl_Task_Module__diagsMask ti_sysbios_knl_Task_Module__diagsMask__C = ((CT__ti_sysbios_knl_Task_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_Module__gateObj__C, ".const:ti_sysbios_knl_Task_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_knl_Task_Module__gateObj ti_sysbios_knl_Task_Module__gateObj__C = ((CT__ti_sysbios_knl_Task_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_Module__gatePrms__C, ".const:ti_sysbios_knl_Task_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_knl_Task_Module__gatePrms ti_sysbios_knl_Task_Module__gatePrms__C = ((CT__ti_sysbios_knl_Task_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_Module__id__C, ".const:ti_sysbios_knl_Task_Module__id__C");
__FAR__ const CT__ti_sysbios_knl_Task_Module__id ti_sysbios_knl_Task_Module__id__C = (xdc_Bits16)0x801e;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_Module__loggerDefined__C, ".const:ti_sysbios_knl_Task_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_knl_Task_Module__loggerDefined ti_sysbios_knl_Task_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_Module__loggerObj__C, ".const:ti_sysbios_knl_Task_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_knl_Task_Module__loggerObj ti_sysbios_knl_Task_Module__loggerObj__C = ((CT__ti_sysbios_knl_Task_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_Module__loggerFxn0__C, ".const:ti_sysbios_knl_Task_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_knl_Task_Module__loggerFxn0 ti_sysbios_knl_Task_Module__loggerFxn0__C = ((CT__ti_sysbios_knl_Task_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_Module__loggerFxn1__C, ".const:ti_sysbios_knl_Task_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_knl_Task_Module__loggerFxn1 ti_sysbios_knl_Task_Module__loggerFxn1__C = ((CT__ti_sysbios_knl_Task_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_Module__loggerFxn2__C, ".const:ti_sysbios_knl_Task_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_knl_Task_Module__loggerFxn2 ti_sysbios_knl_Task_Module__loggerFxn2__C = ((CT__ti_sysbios_knl_Task_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_Module__loggerFxn4__C, ".const:ti_sysbios_knl_Task_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_knl_Task_Module__loggerFxn4 ti_sysbios_knl_Task_Module__loggerFxn4__C = ((CT__ti_sysbios_knl_Task_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_Module__loggerFxn8__C, ".const:ti_sysbios_knl_Task_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_knl_Task_Module__loggerFxn8 ti_sysbios_knl_Task_Module__loggerFxn8__C = ((CT__ti_sysbios_knl_Task_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_Object__count__C, ".const:ti_sysbios_knl_Task_Object__count__C");
__FAR__ const CT__ti_sysbios_knl_Task_Object__count ti_sysbios_knl_Task_Object__count__C = 1;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_Object__heap__C, ".const:ti_sysbios_knl_Task_Object__heap__C");
__FAR__ const CT__ti_sysbios_knl_Task_Object__heap ti_sysbios_knl_Task_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_Object__sizeof__C, ".const:ti_sysbios_knl_Task_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_knl_Task_Object__sizeof ti_sysbios_knl_Task_Object__sizeof__C = sizeof(ti_sysbios_knl_Task_Object__);

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_Object__table__C, ".const:ti_sysbios_knl_Task_Object__table__C");
__FAR__ const CT__ti_sysbios_knl_Task_Object__table ti_sysbios_knl_Task_Object__table__C = ti_sysbios_knl_Task_Object__table__V;

/* LM_switch__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_LM_switch__C, ".const:ti_sysbios_knl_Task_LM_switch__C");
__FAR__ const CT__ti_sysbios_knl_Task_LM_switch ti_sysbios_knl_Task_LM_switch__C = (((xdc_runtime_Log_Event)5869) << 16 | 768);

/* LM_sleep__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_LM_sleep__C, ".const:ti_sysbios_knl_Task_LM_sleep__C");
__FAR__ const CT__ti_sysbios_knl_Task_LM_sleep ti_sysbios_knl_Task_LM_sleep__C = (((xdc_runtime_Log_Event)5937) << 16 | 768);

/* LD_ready__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_LD_ready__C, ".const:ti_sysbios_knl_Task_LD_ready__C");
__FAR__ const CT__ti_sysbios_knl_Task_LD_ready ti_sysbios_knl_Task_LD_ready__C = (((xdc_runtime_Log_Event)5982) << 16 | 512);

/* LD_block__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_LD_block__C, ".const:ti_sysbios_knl_Task_LD_block__C");
__FAR__ const CT__ti_sysbios_knl_Task_LD_block ti_sysbios_knl_Task_LD_block__C = (((xdc_runtime_Log_Event)6023) << 16 | 512);

/* LM_yield__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_LM_yield__C, ".const:ti_sysbios_knl_Task_LM_yield__C");
__FAR__ const CT__ti_sysbios_knl_Task_LM_yield ti_sysbios_knl_Task_LM_yield__C = (((xdc_runtime_Log_Event)6055) << 16 | 768);

/* LM_setPri__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_LM_setPri__C, ".const:ti_sysbios_knl_Task_LM_setPri__C");
__FAR__ const CT__ti_sysbios_knl_Task_LM_setPri ti_sysbios_knl_Task_LM_setPri__C = (((xdc_runtime_Log_Event)6103) << 16 | 768);

/* LD_exit__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_LD_exit__C, ".const:ti_sysbios_knl_Task_LD_exit__C");
__FAR__ const CT__ti_sysbios_knl_Task_LD_exit ti_sysbios_knl_Task_LD_exit__C = (((xdc_runtime_Log_Event)6159) << 16 | 512);

/* LM_setAffinity__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_LM_setAffinity__C, ".const:ti_sysbios_knl_Task_LM_setAffinity__C");
__FAR__ const CT__ti_sysbios_knl_Task_LM_setAffinity ti_sysbios_knl_Task_LM_setAffinity__C = (((xdc_runtime_Log_Event)6190) << 16 | 768);

/* LM_schedule__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_LM_schedule__C, ".const:ti_sysbios_knl_Task_LM_schedule__C");
__FAR__ const CT__ti_sysbios_knl_Task_LM_schedule ti_sysbios_knl_Task_LM_schedule__C = (((xdc_runtime_Log_Event)6273) << 16 | 1024);

/* LM_noWork__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_LM_noWork__C, ".const:ti_sysbios_knl_Task_LM_noWork__C");
__FAR__ const CT__ti_sysbios_knl_Task_LM_noWork ti_sysbios_knl_Task_LM_noWork__C = (((xdc_runtime_Log_Event)6359) << 16 | 1024);

/* E_stackOverflow__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_E_stackOverflow__C, ".const:ti_sysbios_knl_Task_E_stackOverflow__C");
__FAR__ const CT__ti_sysbios_knl_Task_E_stackOverflow ti_sysbios_knl_Task_E_stackOverflow__C = (((xdc_runtime_Error_Id)4147) << 16 | 0);

/* E_spOutOfBounds__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_E_spOutOfBounds__C, ".const:ti_sysbios_knl_Task_E_spOutOfBounds__C");
__FAR__ const CT__ti_sysbios_knl_Task_E_spOutOfBounds ti_sysbios_knl_Task_E_spOutOfBounds__C = (((xdc_runtime_Error_Id)4190) << 16 | 0);

/* E_deleteNotAllowed__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_E_deleteNotAllowed__C, ".const:ti_sysbios_knl_Task_E_deleteNotAllowed__C");
__FAR__ const CT__ti_sysbios_knl_Task_E_deleteNotAllowed ti_sysbios_knl_Task_E_deleteNotAllowed__C = (((xdc_runtime_Error_Id)4241) << 16 | 0);

/* E_moduleStateCheckFailed__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_E_moduleStateCheckFailed__C, ".const:ti_sysbios_knl_Task_E_moduleStateCheckFailed__C");
__FAR__ const CT__ti_sysbios_knl_Task_E_moduleStateCheckFailed ti_sysbios_knl_Task_E_moduleStateCheckFailed__C = (((xdc_runtime_Error_Id)4272) << 16 | 0);

/* E_objectCheckFailed__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_E_objectCheckFailed__C, ".const:ti_sysbios_knl_Task_E_objectCheckFailed__C");
__FAR__ const CT__ti_sysbios_knl_Task_E_objectCheckFailed ti_sysbios_knl_Task_E_objectCheckFailed__C = (((xdc_runtime_Error_Id)4345) << 16 | 0);

/* E_objectNotInKernelSpace__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_E_objectNotInKernelSpace__C, ".const:ti_sysbios_knl_Task_E_objectNotInKernelSpace__C");
__FAR__ const CT__ti_sysbios_knl_Task_E_objectNotInKernelSpace ti_sysbios_knl_Task_E_objectNotInKernelSpace__C = (((xdc_runtime_Error_Id)4412) << 16 | 0);

/* A_badThreadType__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_A_badThreadType__C, ".const:ti_sysbios_knl_Task_A_badThreadType__C");
__FAR__ const CT__ti_sysbios_knl_Task_A_badThreadType ti_sysbios_knl_Task_A_badThreadType__C = (((xdc_runtime_Assert_Id)1480) << 16 | 16);

/* A_badTaskState__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_A_badTaskState__C, ".const:ti_sysbios_knl_Task_A_badTaskState__C");
__FAR__ const CT__ti_sysbios_knl_Task_A_badTaskState ti_sysbios_knl_Task_A_badTaskState__C = (((xdc_runtime_Assert_Id)1549) << 16 | 16);

/* A_noPendElem__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_A_noPendElem__C, ".const:ti_sysbios_knl_Task_A_noPendElem__C");
__FAR__ const CT__ti_sysbios_knl_Task_A_noPendElem ti_sysbios_knl_Task_A_noPendElem__C = (((xdc_runtime_Assert_Id)1603) << 16 | 16);

/* A_taskDisabled__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_A_taskDisabled__C, ".const:ti_sysbios_knl_Task_A_taskDisabled__C");
__FAR__ const CT__ti_sysbios_knl_Task_A_taskDisabled ti_sysbios_knl_Task_A_taskDisabled__C = (((xdc_runtime_Assert_Id)1657) << 16 | 16);

/* A_badPriority__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_A_badPriority__C, ".const:ti_sysbios_knl_Task_A_badPriority__C");
__FAR__ const CT__ti_sysbios_knl_Task_A_badPriority ti_sysbios_knl_Task_A_badPriority__C = (((xdc_runtime_Assert_Id)1720) << 16 | 16);

/* A_badTimeout__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_A_badTimeout__C, ".const:ti_sysbios_knl_Task_A_badTimeout__C");
__FAR__ const CT__ti_sysbios_knl_Task_A_badTimeout ti_sysbios_knl_Task_A_badTimeout__C = (((xdc_runtime_Assert_Id)1770) << 16 | 16);

/* A_badAffinity__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_A_badAffinity__C, ".const:ti_sysbios_knl_Task_A_badAffinity__C");
__FAR__ const CT__ti_sysbios_knl_Task_A_badAffinity ti_sysbios_knl_Task_A_badAffinity__C = (((xdc_runtime_Assert_Id)1805) << 16 | 16);

/* A_sleepTaskDisabled__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_A_sleepTaskDisabled__C, ".const:ti_sysbios_knl_Task_A_sleepTaskDisabled__C");
__FAR__ const CT__ti_sysbios_knl_Task_A_sleepTaskDisabled ti_sysbios_knl_Task_A_sleepTaskDisabled__C = (((xdc_runtime_Assert_Id)1838) << 16 | 16);

/* A_invalidCoreId__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_A_invalidCoreId__C, ".const:ti_sysbios_knl_Task_A_invalidCoreId__C");
__FAR__ const CT__ti_sysbios_knl_Task_A_invalidCoreId ti_sysbios_knl_Task_A_invalidCoreId__C = (((xdc_runtime_Assert_Id)1922) << 16 | 16);

/* numPriorities__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_numPriorities__C, ".const:ti_sysbios_knl_Task_numPriorities__C");
__FAR__ const CT__ti_sysbios_knl_Task_numPriorities ti_sysbios_knl_Task_numPriorities__C = (xdc_UInt)0x10;

/* defaultStackSize__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_defaultStackSize__C, ".const:ti_sysbios_knl_Task_defaultStackSize__C");
__FAR__ const CT__ti_sysbios_knl_Task_defaultStackSize ti_sysbios_knl_Task_defaultStackSize__C = (xdc_SizeT)0x800;

/* defaultStackHeap__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_defaultStackHeap__C, ".const:ti_sysbios_knl_Task_defaultStackHeap__C");
__FAR__ const CT__ti_sysbios_knl_Task_defaultStackHeap ti_sysbios_knl_Task_defaultStackHeap__C = 0;

/* allBlockedFunc__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_allBlockedFunc__C, ".const:ti_sysbios_knl_Task_allBlockedFunc__C");
__FAR__ const CT__ti_sysbios_knl_Task_allBlockedFunc ti_sysbios_knl_Task_allBlockedFunc__C = ((CT__ti_sysbios_knl_Task_allBlockedFunc)0);

/* initStackFlag__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_initStackFlag__C, ".const:ti_sysbios_knl_Task_initStackFlag__C");
__FAR__ const CT__ti_sysbios_knl_Task_initStackFlag ti_sysbios_knl_Task_initStackFlag__C = 1;

/* checkStackFlag__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_checkStackFlag__C, ".const:ti_sysbios_knl_Task_checkStackFlag__C");
__FAR__ const CT__ti_sysbios_knl_Task_checkStackFlag ti_sysbios_knl_Task_checkStackFlag__C = 1;

/* deleteTerminatedTasks__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_deleteTerminatedTasks__C, ".const:ti_sysbios_knl_Task_deleteTerminatedTasks__C");
__FAR__ const CT__ti_sysbios_knl_Task_deleteTerminatedTasks ti_sysbios_knl_Task_deleteTerminatedTasks__C = 0;

/* hooks__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_hooks__C, ".const:ti_sysbios_knl_Task_hooks__C");
__FAR__ const CT__ti_sysbios_knl_Task_hooks ti_sysbios_knl_Task_hooks__C = {0, 0};

/* moduleStateCheckFxn__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_moduleStateCheckFxn__C, ".const:ti_sysbios_knl_Task_moduleStateCheckFxn__C");
__FAR__ const CT__ti_sysbios_knl_Task_moduleStateCheckFxn ti_sysbios_knl_Task_moduleStateCheckFxn__C = ((CT__ti_sysbios_knl_Task_moduleStateCheckFxn)((xdc_Fxn)ti_sysbios_knl_Task_moduleStateCheck__I));

/* moduleStateCheckValueFxn__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_moduleStateCheckValueFxn__C, ".const:ti_sysbios_knl_Task_moduleStateCheckValueFxn__C");
__FAR__ const CT__ti_sysbios_knl_Task_moduleStateCheckValueFxn ti_sysbios_knl_Task_moduleStateCheckValueFxn__C = ((CT__ti_sysbios_knl_Task_moduleStateCheckValueFxn)((xdc_Fxn)ti_sysbios_knl_Task_getModuleStateCheckValue__I));

/* moduleStateCheckFlag__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_moduleStateCheckFlag__C, ".const:ti_sysbios_knl_Task_moduleStateCheckFlag__C");
__FAR__ const CT__ti_sysbios_knl_Task_moduleStateCheckFlag ti_sysbios_knl_Task_moduleStateCheckFlag__C = 0;

/* objectCheckFxn__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_objectCheckFxn__C, ".const:ti_sysbios_knl_Task_objectCheckFxn__C");
__FAR__ const CT__ti_sysbios_knl_Task_objectCheckFxn ti_sysbios_knl_Task_objectCheckFxn__C = ((CT__ti_sysbios_knl_Task_objectCheckFxn)((xdc_Fxn)ti_sysbios_knl_Task_objectCheck__I));

/* objectCheckValueFxn__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_objectCheckValueFxn__C, ".const:ti_sysbios_knl_Task_objectCheckValueFxn__C");
__FAR__ const CT__ti_sysbios_knl_Task_objectCheckValueFxn ti_sysbios_knl_Task_objectCheckValueFxn__C = ((CT__ti_sysbios_knl_Task_objectCheckValueFxn)((xdc_Fxn)ti_sysbios_knl_Task_getObjectCheckValue__I));

/* objectCheckFlag__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_objectCheckFlag__C, ".const:ti_sysbios_knl_Task_objectCheckFlag__C");
__FAR__ const CT__ti_sysbios_knl_Task_objectCheckFlag ti_sysbios_knl_Task_objectCheckFlag__C = 0;

/* numConstructedTasks__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_numConstructedTasks__C, ".const:ti_sysbios_knl_Task_numConstructedTasks__C");
__FAR__ const CT__ti_sysbios_knl_Task_numConstructedTasks ti_sysbios_knl_Task_numConstructedTasks__C = (xdc_UInt)0x0;

/* startupHookFunc__C */
#pragma DATA_SECTION(ti_sysbios_knl_Task_startupHookFunc__C, ".const:ti_sysbios_knl_Task_startupHookFunc__C");
__FAR__ const CT__ti_sysbios_knl_Task_startupHookFunc ti_sysbios_knl_Task_startupHookFunc__C = ((CT__ti_sysbios_knl_Task_startupHookFunc)0);


/*
 * ======== ti.sysbios.knl.Task_SupportProxy INITIALIZERS ========
 */


/*
 * ======== ti.sysbios.timers.rti.Timer INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_timers_rti_Timer_Object__DESC__C;

/* Object__PARAMS__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_Object__PARAMS__C, ".const:ti_sysbios_timers_rti_Timer_Object__PARAMS__C");
__FAR__ const ti_sysbios_timers_rti_Timer_Params ti_sysbios_timers_rti_Timer_Object__PARAMS__C = {
    sizeof (ti_sysbios_timers_rti_Timer_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&ti_sysbios_timers_rti_Timer_Object__PARAMS__C.__iprms, /* instance */
    ti_sysbios_interfaces_ITimer_RunMode_CONTINUOUS,  /* runMode */
    ti_sysbios_interfaces_ITimer_StartMode_AUTO,  /* startMode */
    ((xdc_UArg)0),  /* arg */
    (xdc_UInt32)0x0,  /* period */
    ti_sysbios_interfaces_ITimer_PeriodType_MICROSECS,  /* periodType */
    {
        (xdc_Bits32)0x0,  /* hi */
        (xdc_Bits32)0x0,  /* lo */
    },  /* extFreq */
    1,  /* createHwi */
    ((ti_sysbios_hal_Hwi_Params*)0),  /* hwiParams */
    (xdc_UInt8)0x1,  /* prescale */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
ti_sysbios_timers_rti_Timer_Module__ ti_sysbios_timers_rti_Timer_Module__root__V = {
    {&ti_sysbios_timers_rti_Timer_Module__root__V.link,  /* link.next */
    &ti_sysbios_timers_rti_Timer_Module__root__V.link},  /* link.prev */
};

/* Object__table__V */
ti_sysbios_timers_rti_Timer_Object__ ti_sysbios_timers_rti_Timer_Object__table__V[1] = {
    {/* instance#0 */
        0,
        1,  /* staticInst */
        (xdc_Int)0x0,  /* id */
        ti_sysbios_interfaces_ITimer_RunMode_CONTINUOUS,  /* runMode */
        ti_sysbios_interfaces_ITimer_StartMode_AUTO,  /* startMode */
        (xdc_UInt)0x1,  /* prescale */
        (xdc_UInt)0x3e8,  /* period */
        ti_sysbios_interfaces_ITimer_PeriodType_MICROSECS,  /* periodType */
        (xdc_UInt)0x2,  /* intNum */
        ((xdc_UArg)0),  /* arg */
        ((xdc_Void(*)(xdc_UArg))((xdc_Fxn)ti_sysbios_knl_Clock_doTick__I)),  /* tickFxn */
        {
            (xdc_Bits32)0x0,  /* hi */
            (xdc_Bits32)0x0,  /* lo */
        },  /* extFreq */
        (ti_sysbios_hal_Hwi_Handle)&ti_sysbios_hal_Hwi_Object__table__V[0],  /* hwi */
        1,  /* createHwi */
    },
};

/* --> ti_sysbios_timers_rti_Timer_Module_State_0_device__A */
__T1_ti_sysbios_timers_rti_Timer_Module_State__device ti_sysbios_timers_rti_Timer_Module_State_0_device__A[2] = {
    {
        (xdc_Int)0x2,  /* intNum */
        (xdc_Int)(-0x0 - 1),  /* eventId */
        ((xdc_Ptr)((void*)0xfffffc00)),  /* baseAddr */
    },  /* [0] */
    {
        (xdc_Int)0x3,  /* intNum */
        (xdc_Int)(-0x0 - 1),  /* eventId */
        ((xdc_Ptr)((void*)0xfffffc00)),  /* baseAddr */
    },  /* [1] */
};

/* --> ti_sysbios_timers_rti_Timer_Module_State_0_intFreqs__A */
__T1_ti_sysbios_timers_rti_Timer_Module_State__intFreqs ti_sysbios_timers_rti_Timer_Module_State_0_intFreqs__A[2] = {
    {
        (xdc_Bits32)0x0,  /* hi */
        (xdc_Bits32)0xbebc200,  /* lo */
    },  /* [0] */
    {
        (xdc_Bits32)0x0,  /* hi */
        (xdc_Bits32)0xbebc200,  /* lo */
    },  /* [1] */
};

/* --> ti_sysbios_timers_rti_Timer_Module_State_0_handles__A */
__T1_ti_sysbios_timers_rti_Timer_Module_State__handles ti_sysbios_timers_rti_Timer_Module_State_0_handles__A[2] = {
    (ti_sysbios_timers_rti_Timer_Handle)&ti_sysbios_timers_rti_Timer_Object__table__V[0],  /* [0] */
    0,  /* [1] */
};

/* Module__state__V */
#if defined (__ICCARM__)
#pragma location = ".data_ti_sysbios_timers_rti_Timer_Module__state__V"
#endif
#if defined(__llvm__) && defined(__ti__)
ti_sysbios_timers_rti_Timer_Module_State__ ti_sysbios_timers_rti_Timer_Module__state__V __attribute__ ((section(".data:ti_sysbios_timers_rti_Timer_Module__state__V")));
#elif defined(__GNUC__) && !(defined(__MACH__) && defined(__APPLE__))
#ifndef __TI_COMPILER_VERSION__
ti_sysbios_timers_rti_Timer_Module_State__ ti_sysbios_timers_rti_Timer_Module__state__V __attribute__ ((section(".data_ti_sysbios_timers_rti_Timer_Module__state__V")));
#endif
#endif
ti_sysbios_timers_rti_Timer_Module_State__ ti_sysbios_timers_rti_Timer_Module__state__V = {
    (xdc_UInt)0x2,  /* availMask */
    ((void*)ti_sysbios_timers_rti_Timer_Module_State_0_device__A),  /* device */
    ((void*)ti_sysbios_timers_rti_Timer_Module_State_0_intFreqs__A),  /* intFreqs */
    ((void*)ti_sysbios_timers_rti_Timer_Module_State_0_handles__A),  /* handles */
};

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_Module__diagsEnabled__C, ".const:ti_sysbios_timers_rti_Timer_Module__diagsEnabled__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__diagsEnabled ti_sysbios_timers_rti_Timer_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_Module__diagsIncluded__C, ".const:ti_sysbios_timers_rti_Timer_Module__diagsIncluded__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__diagsIncluded ti_sysbios_timers_rti_Timer_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_Module__diagsMask__C, ".const:ti_sysbios_timers_rti_Timer_Module__diagsMask__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__diagsMask ti_sysbios_timers_rti_Timer_Module__diagsMask__C = ((CT__ti_sysbios_timers_rti_Timer_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_Module__gateObj__C, ".const:ti_sysbios_timers_rti_Timer_Module__gateObj__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__gateObj ti_sysbios_timers_rti_Timer_Module__gateObj__C = ((CT__ti_sysbios_timers_rti_Timer_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_Module__gatePrms__C, ".const:ti_sysbios_timers_rti_Timer_Module__gatePrms__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__gatePrms ti_sysbios_timers_rti_Timer_Module__gatePrms__C = ((CT__ti_sysbios_timers_rti_Timer_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_Module__id__C, ".const:ti_sysbios_timers_rti_Timer_Module__id__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__id ti_sysbios_timers_rti_Timer_Module__id__C = (xdc_Bits16)0x8033;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_Module__loggerDefined__C, ".const:ti_sysbios_timers_rti_Timer_Module__loggerDefined__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__loggerDefined ti_sysbios_timers_rti_Timer_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_Module__loggerObj__C, ".const:ti_sysbios_timers_rti_Timer_Module__loggerObj__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__loggerObj ti_sysbios_timers_rti_Timer_Module__loggerObj__C = ((CT__ti_sysbios_timers_rti_Timer_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_Module__loggerFxn0__C, ".const:ti_sysbios_timers_rti_Timer_Module__loggerFxn0__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn0 ti_sysbios_timers_rti_Timer_Module__loggerFxn0__C = ((CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_Module__loggerFxn1__C, ".const:ti_sysbios_timers_rti_Timer_Module__loggerFxn1__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn1 ti_sysbios_timers_rti_Timer_Module__loggerFxn1__C = ((CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_Module__loggerFxn2__C, ".const:ti_sysbios_timers_rti_Timer_Module__loggerFxn2__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn2 ti_sysbios_timers_rti_Timer_Module__loggerFxn2__C = ((CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_Module__loggerFxn4__C, ".const:ti_sysbios_timers_rti_Timer_Module__loggerFxn4__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn4 ti_sysbios_timers_rti_Timer_Module__loggerFxn4__C = ((CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_Module__loggerFxn8__C, ".const:ti_sysbios_timers_rti_Timer_Module__loggerFxn8__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn8 ti_sysbios_timers_rti_Timer_Module__loggerFxn8__C = ((CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_Object__count__C, ".const:ti_sysbios_timers_rti_Timer_Object__count__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_Object__count ti_sysbios_timers_rti_Timer_Object__count__C = 1;

/* Object__heap__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_Object__heap__C, ".const:ti_sysbios_timers_rti_Timer_Object__heap__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_Object__heap ti_sysbios_timers_rti_Timer_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_Object__sizeof__C, ".const:ti_sysbios_timers_rti_Timer_Object__sizeof__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_Object__sizeof ti_sysbios_timers_rti_Timer_Object__sizeof__C = sizeof(ti_sysbios_timers_rti_Timer_Object__);

/* Object__table__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_Object__table__C, ".const:ti_sysbios_timers_rti_Timer_Object__table__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_Object__table ti_sysbios_timers_rti_Timer_Object__table__C = ti_sysbios_timers_rti_Timer_Object__table__V;

/* A_invalidTimer__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_A_invalidTimer__C, ".const:ti_sysbios_timers_rti_Timer_A_invalidTimer__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_A_invalidTimer ti_sysbios_timers_rti_Timer_A_invalidTimer__C = (((xdc_runtime_Assert_Id)3548) << 16 | 16);

/* E_invalidTimer__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_E_invalidTimer__C, ".const:ti_sysbios_timers_rti_Timer_E_invalidTimer__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_E_invalidTimer ti_sysbios_timers_rti_Timer_E_invalidTimer__C = (((xdc_runtime_Error_Id)5123) << 16 | 0);

/* E_notAvailable__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_E_notAvailable__C, ".const:ti_sysbios_timers_rti_Timer_E_notAvailable__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_E_notAvailable ti_sysbios_timers_rti_Timer_E_notAvailable__C = (((xdc_runtime_Error_Id)5159) << 16 | 0);

/* E_invalidHwiMask__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_E_invalidHwiMask__C, ".const:ti_sysbios_timers_rti_Timer_E_invalidHwiMask__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_E_invalidHwiMask ti_sysbios_timers_rti_Timer_E_invalidHwiMask__C = (((xdc_runtime_Error_Id)5198) << 16 | 0);

/* E_cannotSupport__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_E_cannotSupport__C, ".const:ti_sysbios_timers_rti_Timer_E_cannotSupport__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_E_cannotSupport ti_sysbios_timers_rti_Timer_E_cannotSupport__C = (((xdc_runtime_Error_Id)5250) << 16 | 0);

/* anyMask__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_anyMask__C, ".const:ti_sysbios_timers_rti_Timer_anyMask__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_anyMask ti_sysbios_timers_rti_Timer_anyMask__C = (xdc_UInt)0x3;

/* continueOnSuspend__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_continueOnSuspend__C, ".const:ti_sysbios_timers_rti_Timer_continueOnSuspend__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_continueOnSuspend ti_sysbios_timers_rti_Timer_continueOnSuspend__C = 0;

/* startupNeeded__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_startupNeeded__C, ".const:ti_sysbios_timers_rti_Timer_startupNeeded__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_startupNeeded ti_sysbios_timers_rti_Timer_startupNeeded__C = (xdc_UInt)0x1;

/* numTimerDevices__C */
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_numTimerDevices__C, ".const:ti_sysbios_timers_rti_Timer_numTimerDevices__C");
__FAR__ const CT__ti_sysbios_timers_rti_Timer_numTimerDevices ti_sysbios_timers_rti_Timer_numTimerDevices__C = (xdc_Int)0x2;


/*
 * ======== xdc.runtime.Assert INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(xdc_runtime_Assert_Module__diagsEnabled__C, ".const:xdc_runtime_Assert_Module__diagsEnabled__C");
__FAR__ const CT__xdc_runtime_Assert_Module__diagsEnabled xdc_runtime_Assert_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(xdc_runtime_Assert_Module__diagsIncluded__C, ".const:xdc_runtime_Assert_Module__diagsIncluded__C");
__FAR__ const CT__xdc_runtime_Assert_Module__diagsIncluded xdc_runtime_Assert_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
#pragma DATA_SECTION(xdc_runtime_Assert_Module__diagsMask__C, ".const:xdc_runtime_Assert_Module__diagsMask__C");
__FAR__ const CT__xdc_runtime_Assert_Module__diagsMask xdc_runtime_Assert_Module__diagsMask__C = ((CT__xdc_runtime_Assert_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(xdc_runtime_Assert_Module__gateObj__C, ".const:xdc_runtime_Assert_Module__gateObj__C");
__FAR__ const CT__xdc_runtime_Assert_Module__gateObj xdc_runtime_Assert_Module__gateObj__C = ((CT__xdc_runtime_Assert_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(xdc_runtime_Assert_Module__gatePrms__C, ".const:xdc_runtime_Assert_Module__gatePrms__C");
__FAR__ const CT__xdc_runtime_Assert_Module__gatePrms xdc_runtime_Assert_Module__gatePrms__C = ((CT__xdc_runtime_Assert_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(xdc_runtime_Assert_Module__id__C, ".const:xdc_runtime_Assert_Module__id__C");
__FAR__ const CT__xdc_runtime_Assert_Module__id xdc_runtime_Assert_Module__id__C = (xdc_Bits16)0x8002;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(xdc_runtime_Assert_Module__loggerDefined__C, ".const:xdc_runtime_Assert_Module__loggerDefined__C");
__FAR__ const CT__xdc_runtime_Assert_Module__loggerDefined xdc_runtime_Assert_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(xdc_runtime_Assert_Module__loggerObj__C, ".const:xdc_runtime_Assert_Module__loggerObj__C");
__FAR__ const CT__xdc_runtime_Assert_Module__loggerObj xdc_runtime_Assert_Module__loggerObj__C = ((CT__xdc_runtime_Assert_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(xdc_runtime_Assert_Module__loggerFxn0__C, ".const:xdc_runtime_Assert_Module__loggerFxn0__C");
__FAR__ const CT__xdc_runtime_Assert_Module__loggerFxn0 xdc_runtime_Assert_Module__loggerFxn0__C = ((CT__xdc_runtime_Assert_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(xdc_runtime_Assert_Module__loggerFxn1__C, ".const:xdc_runtime_Assert_Module__loggerFxn1__C");
__FAR__ const CT__xdc_runtime_Assert_Module__loggerFxn1 xdc_runtime_Assert_Module__loggerFxn1__C = ((CT__xdc_runtime_Assert_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(xdc_runtime_Assert_Module__loggerFxn2__C, ".const:xdc_runtime_Assert_Module__loggerFxn2__C");
__FAR__ const CT__xdc_runtime_Assert_Module__loggerFxn2 xdc_runtime_Assert_Module__loggerFxn2__C = ((CT__xdc_runtime_Assert_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(xdc_runtime_Assert_Module__loggerFxn4__C, ".const:xdc_runtime_Assert_Module__loggerFxn4__C");
__FAR__ const CT__xdc_runtime_Assert_Module__loggerFxn4 xdc_runtime_Assert_Module__loggerFxn4__C = ((CT__xdc_runtime_Assert_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(xdc_runtime_Assert_Module__loggerFxn8__C, ".const:xdc_runtime_Assert_Module__loggerFxn8__C");
__FAR__ const CT__xdc_runtime_Assert_Module__loggerFxn8 xdc_runtime_Assert_Module__loggerFxn8__C = ((CT__xdc_runtime_Assert_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(xdc_runtime_Assert_Object__count__C, ".const:xdc_runtime_Assert_Object__count__C");
__FAR__ const CT__xdc_runtime_Assert_Object__count xdc_runtime_Assert_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(xdc_runtime_Assert_Object__heap__C, ".const:xdc_runtime_Assert_Object__heap__C");
__FAR__ const CT__xdc_runtime_Assert_Object__heap xdc_runtime_Assert_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(xdc_runtime_Assert_Object__sizeof__C, ".const:xdc_runtime_Assert_Object__sizeof__C");
__FAR__ const CT__xdc_runtime_Assert_Object__sizeof xdc_runtime_Assert_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(xdc_runtime_Assert_Object__table__C, ".const:xdc_runtime_Assert_Object__table__C");
__FAR__ const CT__xdc_runtime_Assert_Object__table xdc_runtime_Assert_Object__table__C = 0;

/* E_assertFailed__C */
#pragma DATA_SECTION(xdc_runtime_Assert_E_assertFailed__C, ".const:xdc_runtime_Assert_E_assertFailed__C");
__FAR__ const CT__xdc_runtime_Assert_E_assertFailed xdc_runtime_Assert_E_assertFailed__C = (((xdc_runtime_Error_Id)3828) << 16 | 0);


/*
 * ======== xdc.runtime.Core INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(xdc_runtime_Core_Module__diagsEnabled__C, ".const:xdc_runtime_Core_Module__diagsEnabled__C");
__FAR__ const CT__xdc_runtime_Core_Module__diagsEnabled xdc_runtime_Core_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(xdc_runtime_Core_Module__diagsIncluded__C, ".const:xdc_runtime_Core_Module__diagsIncluded__C");
__FAR__ const CT__xdc_runtime_Core_Module__diagsIncluded xdc_runtime_Core_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
#pragma DATA_SECTION(xdc_runtime_Core_Module__diagsMask__C, ".const:xdc_runtime_Core_Module__diagsMask__C");
__FAR__ const CT__xdc_runtime_Core_Module__diagsMask xdc_runtime_Core_Module__diagsMask__C = ((CT__xdc_runtime_Core_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(xdc_runtime_Core_Module__gateObj__C, ".const:xdc_runtime_Core_Module__gateObj__C");
__FAR__ const CT__xdc_runtime_Core_Module__gateObj xdc_runtime_Core_Module__gateObj__C = ((CT__xdc_runtime_Core_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(xdc_runtime_Core_Module__gatePrms__C, ".const:xdc_runtime_Core_Module__gatePrms__C");
__FAR__ const CT__xdc_runtime_Core_Module__gatePrms xdc_runtime_Core_Module__gatePrms__C = ((CT__xdc_runtime_Core_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(xdc_runtime_Core_Module__id__C, ".const:xdc_runtime_Core_Module__id__C");
__FAR__ const CT__xdc_runtime_Core_Module__id xdc_runtime_Core_Module__id__C = (xdc_Bits16)0x8003;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(xdc_runtime_Core_Module__loggerDefined__C, ".const:xdc_runtime_Core_Module__loggerDefined__C");
__FAR__ const CT__xdc_runtime_Core_Module__loggerDefined xdc_runtime_Core_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(xdc_runtime_Core_Module__loggerObj__C, ".const:xdc_runtime_Core_Module__loggerObj__C");
__FAR__ const CT__xdc_runtime_Core_Module__loggerObj xdc_runtime_Core_Module__loggerObj__C = ((CT__xdc_runtime_Core_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(xdc_runtime_Core_Module__loggerFxn0__C, ".const:xdc_runtime_Core_Module__loggerFxn0__C");
__FAR__ const CT__xdc_runtime_Core_Module__loggerFxn0 xdc_runtime_Core_Module__loggerFxn0__C = ((CT__xdc_runtime_Core_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(xdc_runtime_Core_Module__loggerFxn1__C, ".const:xdc_runtime_Core_Module__loggerFxn1__C");
__FAR__ const CT__xdc_runtime_Core_Module__loggerFxn1 xdc_runtime_Core_Module__loggerFxn1__C = ((CT__xdc_runtime_Core_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(xdc_runtime_Core_Module__loggerFxn2__C, ".const:xdc_runtime_Core_Module__loggerFxn2__C");
__FAR__ const CT__xdc_runtime_Core_Module__loggerFxn2 xdc_runtime_Core_Module__loggerFxn2__C = ((CT__xdc_runtime_Core_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(xdc_runtime_Core_Module__loggerFxn4__C, ".const:xdc_runtime_Core_Module__loggerFxn4__C");
__FAR__ const CT__xdc_runtime_Core_Module__loggerFxn4 xdc_runtime_Core_Module__loggerFxn4__C = ((CT__xdc_runtime_Core_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(xdc_runtime_Core_Module__loggerFxn8__C, ".const:xdc_runtime_Core_Module__loggerFxn8__C");
__FAR__ const CT__xdc_runtime_Core_Module__loggerFxn8 xdc_runtime_Core_Module__loggerFxn8__C = ((CT__xdc_runtime_Core_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(xdc_runtime_Core_Object__count__C, ".const:xdc_runtime_Core_Object__count__C");
__FAR__ const CT__xdc_runtime_Core_Object__count xdc_runtime_Core_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(xdc_runtime_Core_Object__heap__C, ".const:xdc_runtime_Core_Object__heap__C");
__FAR__ const CT__xdc_runtime_Core_Object__heap xdc_runtime_Core_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(xdc_runtime_Core_Object__sizeof__C, ".const:xdc_runtime_Core_Object__sizeof__C");
__FAR__ const CT__xdc_runtime_Core_Object__sizeof xdc_runtime_Core_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(xdc_runtime_Core_Object__table__C, ".const:xdc_runtime_Core_Object__table__C");
__FAR__ const CT__xdc_runtime_Core_Object__table xdc_runtime_Core_Object__table__C = 0;

/* A_initializedParams__C */
#pragma DATA_SECTION(xdc_runtime_Core_A_initializedParams__C, ".const:xdc_runtime_Core_A_initializedParams__C");
__FAR__ const CT__xdc_runtime_Core_A_initializedParams xdc_runtime_Core_A_initializedParams__C = (((xdc_runtime_Assert_Id)1) << 16 | 16);


/*
 * ======== xdc.runtime.Defaults INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(xdc_runtime_Defaults_Module__diagsEnabled__C, ".const:xdc_runtime_Defaults_Module__diagsEnabled__C");
__FAR__ const CT__xdc_runtime_Defaults_Module__diagsEnabled xdc_runtime_Defaults_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(xdc_runtime_Defaults_Module__diagsIncluded__C, ".const:xdc_runtime_Defaults_Module__diagsIncluded__C");
__FAR__ const CT__xdc_runtime_Defaults_Module__diagsIncluded xdc_runtime_Defaults_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(xdc_runtime_Defaults_Module__diagsMask__C, ".const:xdc_runtime_Defaults_Module__diagsMask__C");
__FAR__ const CT__xdc_runtime_Defaults_Module__diagsMask xdc_runtime_Defaults_Module__diagsMask__C = ((CT__xdc_runtime_Defaults_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(xdc_runtime_Defaults_Module__gateObj__C, ".const:xdc_runtime_Defaults_Module__gateObj__C");
__FAR__ const CT__xdc_runtime_Defaults_Module__gateObj xdc_runtime_Defaults_Module__gateObj__C = ((CT__xdc_runtime_Defaults_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(xdc_runtime_Defaults_Module__gatePrms__C, ".const:xdc_runtime_Defaults_Module__gatePrms__C");
__FAR__ const CT__xdc_runtime_Defaults_Module__gatePrms xdc_runtime_Defaults_Module__gatePrms__C = ((CT__xdc_runtime_Defaults_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(xdc_runtime_Defaults_Module__id__C, ".const:xdc_runtime_Defaults_Module__id__C");
__FAR__ const CT__xdc_runtime_Defaults_Module__id xdc_runtime_Defaults_Module__id__C = (xdc_Bits16)0x8004;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(xdc_runtime_Defaults_Module__loggerDefined__C, ".const:xdc_runtime_Defaults_Module__loggerDefined__C");
__FAR__ const CT__xdc_runtime_Defaults_Module__loggerDefined xdc_runtime_Defaults_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(xdc_runtime_Defaults_Module__loggerObj__C, ".const:xdc_runtime_Defaults_Module__loggerObj__C");
__FAR__ const CT__xdc_runtime_Defaults_Module__loggerObj xdc_runtime_Defaults_Module__loggerObj__C = ((CT__xdc_runtime_Defaults_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(xdc_runtime_Defaults_Module__loggerFxn0__C, ".const:xdc_runtime_Defaults_Module__loggerFxn0__C");
__FAR__ const CT__xdc_runtime_Defaults_Module__loggerFxn0 xdc_runtime_Defaults_Module__loggerFxn0__C = ((CT__xdc_runtime_Defaults_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(xdc_runtime_Defaults_Module__loggerFxn1__C, ".const:xdc_runtime_Defaults_Module__loggerFxn1__C");
__FAR__ const CT__xdc_runtime_Defaults_Module__loggerFxn1 xdc_runtime_Defaults_Module__loggerFxn1__C = ((CT__xdc_runtime_Defaults_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(xdc_runtime_Defaults_Module__loggerFxn2__C, ".const:xdc_runtime_Defaults_Module__loggerFxn2__C");
__FAR__ const CT__xdc_runtime_Defaults_Module__loggerFxn2 xdc_runtime_Defaults_Module__loggerFxn2__C = ((CT__xdc_runtime_Defaults_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(xdc_runtime_Defaults_Module__loggerFxn4__C, ".const:xdc_runtime_Defaults_Module__loggerFxn4__C");
__FAR__ const CT__xdc_runtime_Defaults_Module__loggerFxn4 xdc_runtime_Defaults_Module__loggerFxn4__C = ((CT__xdc_runtime_Defaults_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(xdc_runtime_Defaults_Module__loggerFxn8__C, ".const:xdc_runtime_Defaults_Module__loggerFxn8__C");
__FAR__ const CT__xdc_runtime_Defaults_Module__loggerFxn8 xdc_runtime_Defaults_Module__loggerFxn8__C = ((CT__xdc_runtime_Defaults_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(xdc_runtime_Defaults_Object__count__C, ".const:xdc_runtime_Defaults_Object__count__C");
__FAR__ const CT__xdc_runtime_Defaults_Object__count xdc_runtime_Defaults_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(xdc_runtime_Defaults_Object__heap__C, ".const:xdc_runtime_Defaults_Object__heap__C");
__FAR__ const CT__xdc_runtime_Defaults_Object__heap xdc_runtime_Defaults_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(xdc_runtime_Defaults_Object__sizeof__C, ".const:xdc_runtime_Defaults_Object__sizeof__C");
__FAR__ const CT__xdc_runtime_Defaults_Object__sizeof xdc_runtime_Defaults_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(xdc_runtime_Defaults_Object__table__C, ".const:xdc_runtime_Defaults_Object__table__C");
__FAR__ const CT__xdc_runtime_Defaults_Object__table xdc_runtime_Defaults_Object__table__C = 0;


/*
 * ======== xdc.runtime.Diags INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(xdc_runtime_Diags_Module__diagsEnabled__C, ".const:xdc_runtime_Diags_Module__diagsEnabled__C");
__FAR__ const CT__xdc_runtime_Diags_Module__diagsEnabled xdc_runtime_Diags_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(xdc_runtime_Diags_Module__diagsIncluded__C, ".const:xdc_runtime_Diags_Module__diagsIncluded__C");
__FAR__ const CT__xdc_runtime_Diags_Module__diagsIncluded xdc_runtime_Diags_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
#pragma DATA_SECTION(xdc_runtime_Diags_Module__diagsMask__C, ".const:xdc_runtime_Diags_Module__diagsMask__C");
__FAR__ const CT__xdc_runtime_Diags_Module__diagsMask xdc_runtime_Diags_Module__diagsMask__C = ((CT__xdc_runtime_Diags_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(xdc_runtime_Diags_Module__gateObj__C, ".const:xdc_runtime_Diags_Module__gateObj__C");
__FAR__ const CT__xdc_runtime_Diags_Module__gateObj xdc_runtime_Diags_Module__gateObj__C = ((CT__xdc_runtime_Diags_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(xdc_runtime_Diags_Module__gatePrms__C, ".const:xdc_runtime_Diags_Module__gatePrms__C");
__FAR__ const CT__xdc_runtime_Diags_Module__gatePrms xdc_runtime_Diags_Module__gatePrms__C = ((CT__xdc_runtime_Diags_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(xdc_runtime_Diags_Module__id__C, ".const:xdc_runtime_Diags_Module__id__C");
__FAR__ const CT__xdc_runtime_Diags_Module__id xdc_runtime_Diags_Module__id__C = (xdc_Bits16)0x8005;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(xdc_runtime_Diags_Module__loggerDefined__C, ".const:xdc_runtime_Diags_Module__loggerDefined__C");
__FAR__ const CT__xdc_runtime_Diags_Module__loggerDefined xdc_runtime_Diags_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(xdc_runtime_Diags_Module__loggerObj__C, ".const:xdc_runtime_Diags_Module__loggerObj__C");
__FAR__ const CT__xdc_runtime_Diags_Module__loggerObj xdc_runtime_Diags_Module__loggerObj__C = ((CT__xdc_runtime_Diags_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(xdc_runtime_Diags_Module__loggerFxn0__C, ".const:xdc_runtime_Diags_Module__loggerFxn0__C");
__FAR__ const CT__xdc_runtime_Diags_Module__loggerFxn0 xdc_runtime_Diags_Module__loggerFxn0__C = ((CT__xdc_runtime_Diags_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(xdc_runtime_Diags_Module__loggerFxn1__C, ".const:xdc_runtime_Diags_Module__loggerFxn1__C");
__FAR__ const CT__xdc_runtime_Diags_Module__loggerFxn1 xdc_runtime_Diags_Module__loggerFxn1__C = ((CT__xdc_runtime_Diags_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(xdc_runtime_Diags_Module__loggerFxn2__C, ".const:xdc_runtime_Diags_Module__loggerFxn2__C");
__FAR__ const CT__xdc_runtime_Diags_Module__loggerFxn2 xdc_runtime_Diags_Module__loggerFxn2__C = ((CT__xdc_runtime_Diags_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(xdc_runtime_Diags_Module__loggerFxn4__C, ".const:xdc_runtime_Diags_Module__loggerFxn4__C");
__FAR__ const CT__xdc_runtime_Diags_Module__loggerFxn4 xdc_runtime_Diags_Module__loggerFxn4__C = ((CT__xdc_runtime_Diags_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(xdc_runtime_Diags_Module__loggerFxn8__C, ".const:xdc_runtime_Diags_Module__loggerFxn8__C");
__FAR__ const CT__xdc_runtime_Diags_Module__loggerFxn8 xdc_runtime_Diags_Module__loggerFxn8__C = ((CT__xdc_runtime_Diags_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(xdc_runtime_Diags_Object__count__C, ".const:xdc_runtime_Diags_Object__count__C");
__FAR__ const CT__xdc_runtime_Diags_Object__count xdc_runtime_Diags_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(xdc_runtime_Diags_Object__heap__C, ".const:xdc_runtime_Diags_Object__heap__C");
__FAR__ const CT__xdc_runtime_Diags_Object__heap xdc_runtime_Diags_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(xdc_runtime_Diags_Object__sizeof__C, ".const:xdc_runtime_Diags_Object__sizeof__C");
__FAR__ const CT__xdc_runtime_Diags_Object__sizeof xdc_runtime_Diags_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(xdc_runtime_Diags_Object__table__C, ".const:xdc_runtime_Diags_Object__table__C");
__FAR__ const CT__xdc_runtime_Diags_Object__table xdc_runtime_Diags_Object__table__C = 0;

/* setMaskEnabled__C */
#pragma DATA_SECTION(xdc_runtime_Diags_setMaskEnabled__C, ".const:xdc_runtime_Diags_setMaskEnabled__C");
__FAR__ const CT__xdc_runtime_Diags_setMaskEnabled xdc_runtime_Diags_setMaskEnabled__C = 0;

/* dictBase__C */
#pragma DATA_SECTION(xdc_runtime_Diags_dictBase__C, ".const:xdc_runtime_Diags_dictBase__C");
__FAR__ const CT__xdc_runtime_Diags_dictBase xdc_runtime_Diags_dictBase__C = ((CT__xdc_runtime_Diags_dictBase)0);


/*
 * ======== xdc.runtime.Error INITIALIZERS ========
 */

/* Module__state__V */
#if defined (__ICCARM__)
#pragma location = ".data_xdc_runtime_Error_Module__state__V"
#endif
#if defined(__llvm__) && defined(__ti__)
xdc_runtime_Error_Module_State__ xdc_runtime_Error_Module__state__V __attribute__ ((section(".data:xdc_runtime_Error_Module__state__V")));
#elif defined(__GNUC__) && !(defined(__MACH__) && defined(__APPLE__))
#ifndef __TI_COMPILER_VERSION__
xdc_runtime_Error_Module_State__ xdc_runtime_Error_Module__state__V __attribute__ ((section(".data_xdc_runtime_Error_Module__state__V")));
#endif
#endif
xdc_runtime_Error_Module_State__ xdc_runtime_Error_Module__state__V = {
    (xdc_UInt16)0x0,  /* count */
};

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(xdc_runtime_Error_Module__diagsEnabled__C, ".const:xdc_runtime_Error_Module__diagsEnabled__C");
__FAR__ const CT__xdc_runtime_Error_Module__diagsEnabled xdc_runtime_Error_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(xdc_runtime_Error_Module__diagsIncluded__C, ".const:xdc_runtime_Error_Module__diagsIncluded__C");
__FAR__ const CT__xdc_runtime_Error_Module__diagsIncluded xdc_runtime_Error_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(xdc_runtime_Error_Module__diagsMask__C, ".const:xdc_runtime_Error_Module__diagsMask__C");
__FAR__ const CT__xdc_runtime_Error_Module__diagsMask xdc_runtime_Error_Module__diagsMask__C = ((CT__xdc_runtime_Error_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(xdc_runtime_Error_Module__gateObj__C, ".const:xdc_runtime_Error_Module__gateObj__C");
__FAR__ const CT__xdc_runtime_Error_Module__gateObj xdc_runtime_Error_Module__gateObj__C = ((CT__xdc_runtime_Error_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(xdc_runtime_Error_Module__gatePrms__C, ".const:xdc_runtime_Error_Module__gatePrms__C");
__FAR__ const CT__xdc_runtime_Error_Module__gatePrms xdc_runtime_Error_Module__gatePrms__C = ((CT__xdc_runtime_Error_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(xdc_runtime_Error_Module__id__C, ".const:xdc_runtime_Error_Module__id__C");
__FAR__ const CT__xdc_runtime_Error_Module__id xdc_runtime_Error_Module__id__C = (xdc_Bits16)0x8006;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(xdc_runtime_Error_Module__loggerDefined__C, ".const:xdc_runtime_Error_Module__loggerDefined__C");
__FAR__ const CT__xdc_runtime_Error_Module__loggerDefined xdc_runtime_Error_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(xdc_runtime_Error_Module__loggerObj__C, ".const:xdc_runtime_Error_Module__loggerObj__C");
__FAR__ const CT__xdc_runtime_Error_Module__loggerObj xdc_runtime_Error_Module__loggerObj__C = ((CT__xdc_runtime_Error_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(xdc_runtime_Error_Module__loggerFxn0__C, ".const:xdc_runtime_Error_Module__loggerFxn0__C");
__FAR__ const CT__xdc_runtime_Error_Module__loggerFxn0 xdc_runtime_Error_Module__loggerFxn0__C = ((CT__xdc_runtime_Error_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(xdc_runtime_Error_Module__loggerFxn1__C, ".const:xdc_runtime_Error_Module__loggerFxn1__C");
__FAR__ const CT__xdc_runtime_Error_Module__loggerFxn1 xdc_runtime_Error_Module__loggerFxn1__C = ((CT__xdc_runtime_Error_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(xdc_runtime_Error_Module__loggerFxn2__C, ".const:xdc_runtime_Error_Module__loggerFxn2__C");
__FAR__ const CT__xdc_runtime_Error_Module__loggerFxn2 xdc_runtime_Error_Module__loggerFxn2__C = ((CT__xdc_runtime_Error_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(xdc_runtime_Error_Module__loggerFxn4__C, ".const:xdc_runtime_Error_Module__loggerFxn4__C");
__FAR__ const CT__xdc_runtime_Error_Module__loggerFxn4 xdc_runtime_Error_Module__loggerFxn4__C = ((CT__xdc_runtime_Error_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(xdc_runtime_Error_Module__loggerFxn8__C, ".const:xdc_runtime_Error_Module__loggerFxn8__C");
__FAR__ const CT__xdc_runtime_Error_Module__loggerFxn8 xdc_runtime_Error_Module__loggerFxn8__C = ((CT__xdc_runtime_Error_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(xdc_runtime_Error_Object__count__C, ".const:xdc_runtime_Error_Object__count__C");
__FAR__ const CT__xdc_runtime_Error_Object__count xdc_runtime_Error_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(xdc_runtime_Error_Object__heap__C, ".const:xdc_runtime_Error_Object__heap__C");
__FAR__ const CT__xdc_runtime_Error_Object__heap xdc_runtime_Error_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(xdc_runtime_Error_Object__sizeof__C, ".const:xdc_runtime_Error_Object__sizeof__C");
__FAR__ const CT__xdc_runtime_Error_Object__sizeof xdc_runtime_Error_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(xdc_runtime_Error_Object__table__C, ".const:xdc_runtime_Error_Object__table__C");
__FAR__ const CT__xdc_runtime_Error_Object__table xdc_runtime_Error_Object__table__C = 0;

/* policyFxn__C */
#pragma DATA_SECTION(xdc_runtime_Error_policyFxn__C, ".const:xdc_runtime_Error_policyFxn__C");
__FAR__ const CT__xdc_runtime_Error_policyFxn xdc_runtime_Error_policyFxn__C = ((CT__xdc_runtime_Error_policyFxn)((xdc_Fxn)xdc_runtime_Error_policyDefault__E));

/* E_generic__C */
#pragma DATA_SECTION(xdc_runtime_Error_E_generic__C, ".const:xdc_runtime_Error_E_generic__C");
__FAR__ const CT__xdc_runtime_Error_E_generic xdc_runtime_Error_E_generic__C = (((xdc_runtime_Error_Id)3850) << 16 | 0);

/* E_memory__C */
#pragma DATA_SECTION(xdc_runtime_Error_E_memory__C, ".const:xdc_runtime_Error_E_memory__C");
__FAR__ const CT__xdc_runtime_Error_E_memory xdc_runtime_Error_E_memory__C = (((xdc_runtime_Error_Id)3854) << 16 | 0);

/* E_msgCode__C */
#pragma DATA_SECTION(xdc_runtime_Error_E_msgCode__C, ".const:xdc_runtime_Error_E_msgCode__C");
__FAR__ const CT__xdc_runtime_Error_E_msgCode xdc_runtime_Error_E_msgCode__C = (((xdc_runtime_Error_Id)3888) << 16 | 0);

/* policy__C */
#pragma DATA_SECTION(xdc_runtime_Error_policy__C, ".const:xdc_runtime_Error_policy__C");
__FAR__ const CT__xdc_runtime_Error_policy xdc_runtime_Error_policy__C = xdc_runtime_Error_UNWIND;

/* raiseHook__C */
#pragma DATA_SECTION(xdc_runtime_Error_raiseHook__C, ".const:xdc_runtime_Error_raiseHook__C");
__FAR__ const CT__xdc_runtime_Error_raiseHook xdc_runtime_Error_raiseHook__C = ((CT__xdc_runtime_Error_raiseHook)((xdc_Fxn)ti_sysbios_BIOS_errorRaiseHook__I));

/* maxDepth__C */
#pragma DATA_SECTION(xdc_runtime_Error_maxDepth__C, ".const:xdc_runtime_Error_maxDepth__C");
__FAR__ const CT__xdc_runtime_Error_maxDepth xdc_runtime_Error_maxDepth__C = (xdc_UInt16)0x10;


/*
 * ======== xdc.runtime.Gate INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(xdc_runtime_Gate_Module__diagsEnabled__C, ".const:xdc_runtime_Gate_Module__diagsEnabled__C");
__FAR__ const CT__xdc_runtime_Gate_Module__diagsEnabled xdc_runtime_Gate_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(xdc_runtime_Gate_Module__diagsIncluded__C, ".const:xdc_runtime_Gate_Module__diagsIncluded__C");
__FAR__ const CT__xdc_runtime_Gate_Module__diagsIncluded xdc_runtime_Gate_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
#pragma DATA_SECTION(xdc_runtime_Gate_Module__diagsMask__C, ".const:xdc_runtime_Gate_Module__diagsMask__C");
__FAR__ const CT__xdc_runtime_Gate_Module__diagsMask xdc_runtime_Gate_Module__diagsMask__C = ((CT__xdc_runtime_Gate_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(xdc_runtime_Gate_Module__gateObj__C, ".const:xdc_runtime_Gate_Module__gateObj__C");
__FAR__ const CT__xdc_runtime_Gate_Module__gateObj xdc_runtime_Gate_Module__gateObj__C = ((CT__xdc_runtime_Gate_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(xdc_runtime_Gate_Module__gatePrms__C, ".const:xdc_runtime_Gate_Module__gatePrms__C");
__FAR__ const CT__xdc_runtime_Gate_Module__gatePrms xdc_runtime_Gate_Module__gatePrms__C = ((CT__xdc_runtime_Gate_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(xdc_runtime_Gate_Module__id__C, ".const:xdc_runtime_Gate_Module__id__C");
__FAR__ const CT__xdc_runtime_Gate_Module__id xdc_runtime_Gate_Module__id__C = (xdc_Bits16)0x8007;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(xdc_runtime_Gate_Module__loggerDefined__C, ".const:xdc_runtime_Gate_Module__loggerDefined__C");
__FAR__ const CT__xdc_runtime_Gate_Module__loggerDefined xdc_runtime_Gate_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(xdc_runtime_Gate_Module__loggerObj__C, ".const:xdc_runtime_Gate_Module__loggerObj__C");
__FAR__ const CT__xdc_runtime_Gate_Module__loggerObj xdc_runtime_Gate_Module__loggerObj__C = ((CT__xdc_runtime_Gate_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(xdc_runtime_Gate_Module__loggerFxn0__C, ".const:xdc_runtime_Gate_Module__loggerFxn0__C");
__FAR__ const CT__xdc_runtime_Gate_Module__loggerFxn0 xdc_runtime_Gate_Module__loggerFxn0__C = ((CT__xdc_runtime_Gate_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(xdc_runtime_Gate_Module__loggerFxn1__C, ".const:xdc_runtime_Gate_Module__loggerFxn1__C");
__FAR__ const CT__xdc_runtime_Gate_Module__loggerFxn1 xdc_runtime_Gate_Module__loggerFxn1__C = ((CT__xdc_runtime_Gate_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(xdc_runtime_Gate_Module__loggerFxn2__C, ".const:xdc_runtime_Gate_Module__loggerFxn2__C");
__FAR__ const CT__xdc_runtime_Gate_Module__loggerFxn2 xdc_runtime_Gate_Module__loggerFxn2__C = ((CT__xdc_runtime_Gate_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(xdc_runtime_Gate_Module__loggerFxn4__C, ".const:xdc_runtime_Gate_Module__loggerFxn4__C");
__FAR__ const CT__xdc_runtime_Gate_Module__loggerFxn4 xdc_runtime_Gate_Module__loggerFxn4__C = ((CT__xdc_runtime_Gate_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(xdc_runtime_Gate_Module__loggerFxn8__C, ".const:xdc_runtime_Gate_Module__loggerFxn8__C");
__FAR__ const CT__xdc_runtime_Gate_Module__loggerFxn8 xdc_runtime_Gate_Module__loggerFxn8__C = ((CT__xdc_runtime_Gate_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(xdc_runtime_Gate_Object__count__C, ".const:xdc_runtime_Gate_Object__count__C");
__FAR__ const CT__xdc_runtime_Gate_Object__count xdc_runtime_Gate_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(xdc_runtime_Gate_Object__heap__C, ".const:xdc_runtime_Gate_Object__heap__C");
__FAR__ const CT__xdc_runtime_Gate_Object__heap xdc_runtime_Gate_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(xdc_runtime_Gate_Object__sizeof__C, ".const:xdc_runtime_Gate_Object__sizeof__C");
__FAR__ const CT__xdc_runtime_Gate_Object__sizeof xdc_runtime_Gate_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(xdc_runtime_Gate_Object__table__C, ".const:xdc_runtime_Gate_Object__table__C");
__FAR__ const CT__xdc_runtime_Gate_Object__table xdc_runtime_Gate_Object__table__C = 0;


/*
 * ======== xdc.runtime.Log INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(xdc_runtime_Log_Module__diagsEnabled__C, ".const:xdc_runtime_Log_Module__diagsEnabled__C");
__FAR__ const CT__xdc_runtime_Log_Module__diagsEnabled xdc_runtime_Log_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(xdc_runtime_Log_Module__diagsIncluded__C, ".const:xdc_runtime_Log_Module__diagsIncluded__C");
__FAR__ const CT__xdc_runtime_Log_Module__diagsIncluded xdc_runtime_Log_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
#pragma DATA_SECTION(xdc_runtime_Log_Module__diagsMask__C, ".const:xdc_runtime_Log_Module__diagsMask__C");
__FAR__ const CT__xdc_runtime_Log_Module__diagsMask xdc_runtime_Log_Module__diagsMask__C = ((CT__xdc_runtime_Log_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(xdc_runtime_Log_Module__gateObj__C, ".const:xdc_runtime_Log_Module__gateObj__C");
__FAR__ const CT__xdc_runtime_Log_Module__gateObj xdc_runtime_Log_Module__gateObj__C = ((CT__xdc_runtime_Log_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(xdc_runtime_Log_Module__gatePrms__C, ".const:xdc_runtime_Log_Module__gatePrms__C");
__FAR__ const CT__xdc_runtime_Log_Module__gatePrms xdc_runtime_Log_Module__gatePrms__C = ((CT__xdc_runtime_Log_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(xdc_runtime_Log_Module__id__C, ".const:xdc_runtime_Log_Module__id__C");
__FAR__ const CT__xdc_runtime_Log_Module__id xdc_runtime_Log_Module__id__C = (xdc_Bits16)0x8008;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(xdc_runtime_Log_Module__loggerDefined__C, ".const:xdc_runtime_Log_Module__loggerDefined__C");
__FAR__ const CT__xdc_runtime_Log_Module__loggerDefined xdc_runtime_Log_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(xdc_runtime_Log_Module__loggerObj__C, ".const:xdc_runtime_Log_Module__loggerObj__C");
__FAR__ const CT__xdc_runtime_Log_Module__loggerObj xdc_runtime_Log_Module__loggerObj__C = ((CT__xdc_runtime_Log_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(xdc_runtime_Log_Module__loggerFxn0__C, ".const:xdc_runtime_Log_Module__loggerFxn0__C");
__FAR__ const CT__xdc_runtime_Log_Module__loggerFxn0 xdc_runtime_Log_Module__loggerFxn0__C = ((CT__xdc_runtime_Log_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(xdc_runtime_Log_Module__loggerFxn1__C, ".const:xdc_runtime_Log_Module__loggerFxn1__C");
__FAR__ const CT__xdc_runtime_Log_Module__loggerFxn1 xdc_runtime_Log_Module__loggerFxn1__C = ((CT__xdc_runtime_Log_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(xdc_runtime_Log_Module__loggerFxn2__C, ".const:xdc_runtime_Log_Module__loggerFxn2__C");
__FAR__ const CT__xdc_runtime_Log_Module__loggerFxn2 xdc_runtime_Log_Module__loggerFxn2__C = ((CT__xdc_runtime_Log_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(xdc_runtime_Log_Module__loggerFxn4__C, ".const:xdc_runtime_Log_Module__loggerFxn4__C");
__FAR__ const CT__xdc_runtime_Log_Module__loggerFxn4 xdc_runtime_Log_Module__loggerFxn4__C = ((CT__xdc_runtime_Log_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(xdc_runtime_Log_Module__loggerFxn8__C, ".const:xdc_runtime_Log_Module__loggerFxn8__C");
__FAR__ const CT__xdc_runtime_Log_Module__loggerFxn8 xdc_runtime_Log_Module__loggerFxn8__C = ((CT__xdc_runtime_Log_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(xdc_runtime_Log_Object__count__C, ".const:xdc_runtime_Log_Object__count__C");
__FAR__ const CT__xdc_runtime_Log_Object__count xdc_runtime_Log_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(xdc_runtime_Log_Object__heap__C, ".const:xdc_runtime_Log_Object__heap__C");
__FAR__ const CT__xdc_runtime_Log_Object__heap xdc_runtime_Log_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(xdc_runtime_Log_Object__sizeof__C, ".const:xdc_runtime_Log_Object__sizeof__C");
__FAR__ const CT__xdc_runtime_Log_Object__sizeof xdc_runtime_Log_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(xdc_runtime_Log_Object__table__C, ".const:xdc_runtime_Log_Object__table__C");
__FAR__ const CT__xdc_runtime_Log_Object__table xdc_runtime_Log_Object__table__C = 0;

/* L_construct__C */
#pragma DATA_SECTION(xdc_runtime_Log_L_construct__C, ".const:xdc_runtime_Log_L_construct__C");
__FAR__ const CT__xdc_runtime_Log_L_construct xdc_runtime_Log_L_construct__C = (((xdc_runtime_Log_Event)5308) << 16 | 4);

/* L_create__C */
#pragma DATA_SECTION(xdc_runtime_Log_L_create__C, ".const:xdc_runtime_Log_L_create__C");
__FAR__ const CT__xdc_runtime_Log_L_create xdc_runtime_Log_L_create__C = (((xdc_runtime_Log_Event)5332) << 16 | 4);

/* L_destruct__C */
#pragma DATA_SECTION(xdc_runtime_Log_L_destruct__C, ".const:xdc_runtime_Log_L_destruct__C");
__FAR__ const CT__xdc_runtime_Log_L_destruct xdc_runtime_Log_L_destruct__C = (((xdc_runtime_Log_Event)5353) << 16 | 4);

/* L_delete__C */
#pragma DATA_SECTION(xdc_runtime_Log_L_delete__C, ".const:xdc_runtime_Log_L_delete__C");
__FAR__ const CT__xdc_runtime_Log_L_delete xdc_runtime_Log_L_delete__C = (((xdc_runtime_Log_Event)5372) << 16 | 4);

/* L_error__C */
#pragma DATA_SECTION(xdc_runtime_Log_L_error__C, ".const:xdc_runtime_Log_L_error__C");
__FAR__ const CT__xdc_runtime_Log_L_error xdc_runtime_Log_L_error__C = (((xdc_runtime_Log_Event)5389) << 16 | 192);

/* L_warning__C */
#pragma DATA_SECTION(xdc_runtime_Log_L_warning__C, ".const:xdc_runtime_Log_L_warning__C");
__FAR__ const CT__xdc_runtime_Log_L_warning xdc_runtime_Log_L_warning__C = (((xdc_runtime_Log_Event)5403) << 16 | 224);

/* L_info__C */
#pragma DATA_SECTION(xdc_runtime_Log_L_info__C, ".const:xdc_runtime_Log_L_info__C");
__FAR__ const CT__xdc_runtime_Log_L_info xdc_runtime_Log_L_info__C = (((xdc_runtime_Log_Event)5419) << 16 | 16384);

/* L_start__C */
#pragma DATA_SECTION(xdc_runtime_Log_L_start__C, ".const:xdc_runtime_Log_L_start__C");
__FAR__ const CT__xdc_runtime_Log_L_start xdc_runtime_Log_L_start__C = (((xdc_runtime_Log_Event)5426) << 16 | 32768);

/* L_stop__C */
#pragma DATA_SECTION(xdc_runtime_Log_L_stop__C, ".const:xdc_runtime_Log_L_stop__C");
__FAR__ const CT__xdc_runtime_Log_L_stop xdc_runtime_Log_L_stop__C = (((xdc_runtime_Log_Event)5437) << 16 | 32768);

/* L_startInstance__C */
#pragma DATA_SECTION(xdc_runtime_Log_L_startInstance__C, ".const:xdc_runtime_Log_L_startInstance__C");
__FAR__ const CT__xdc_runtime_Log_L_startInstance xdc_runtime_Log_L_startInstance__C = (((xdc_runtime_Log_Event)5447) << 16 | 32768);

/* L_stopInstance__C */
#pragma DATA_SECTION(xdc_runtime_Log_L_stopInstance__C, ".const:xdc_runtime_Log_L_stopInstance__C");
__FAR__ const CT__xdc_runtime_Log_L_stopInstance xdc_runtime_Log_L_stopInstance__C = (((xdc_runtime_Log_Event)5466) << 16 | 32768);


/*
 * ======== xdc.runtime.Main INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(xdc_runtime_Main_Module__diagsEnabled__C, ".const:xdc_runtime_Main_Module__diagsEnabled__C");
__FAR__ const CT__xdc_runtime_Main_Module__diagsEnabled xdc_runtime_Main_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(xdc_runtime_Main_Module__diagsIncluded__C, ".const:xdc_runtime_Main_Module__diagsIncluded__C");
__FAR__ const CT__xdc_runtime_Main_Module__diagsIncluded xdc_runtime_Main_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(xdc_runtime_Main_Module__diagsMask__C, ".const:xdc_runtime_Main_Module__diagsMask__C");
__FAR__ const CT__xdc_runtime_Main_Module__diagsMask xdc_runtime_Main_Module__diagsMask__C = ((CT__xdc_runtime_Main_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(xdc_runtime_Main_Module__gateObj__C, ".const:xdc_runtime_Main_Module__gateObj__C");
__FAR__ const CT__xdc_runtime_Main_Module__gateObj xdc_runtime_Main_Module__gateObj__C = ((CT__xdc_runtime_Main_Module__gateObj)((const void*)(xdc_runtime_IGateProvider_Handle)&ti_sysbios_gates_GateHwi_Object__table__V[0]));

/* Module__gatePrms__C */
#pragma DATA_SECTION(xdc_runtime_Main_Module__gatePrms__C, ".const:xdc_runtime_Main_Module__gatePrms__C");
__FAR__ const CT__xdc_runtime_Main_Module__gatePrms xdc_runtime_Main_Module__gatePrms__C = ((CT__xdc_runtime_Main_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(xdc_runtime_Main_Module__id__C, ".const:xdc_runtime_Main_Module__id__C");
__FAR__ const CT__xdc_runtime_Main_Module__id xdc_runtime_Main_Module__id__C = (xdc_Bits16)0x8009;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(xdc_runtime_Main_Module__loggerDefined__C, ".const:xdc_runtime_Main_Module__loggerDefined__C");
__FAR__ const CT__xdc_runtime_Main_Module__loggerDefined xdc_runtime_Main_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(xdc_runtime_Main_Module__loggerObj__C, ".const:xdc_runtime_Main_Module__loggerObj__C");
__FAR__ const CT__xdc_runtime_Main_Module__loggerObj xdc_runtime_Main_Module__loggerObj__C = ((CT__xdc_runtime_Main_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(xdc_runtime_Main_Module__loggerFxn0__C, ".const:xdc_runtime_Main_Module__loggerFxn0__C");
__FAR__ const CT__xdc_runtime_Main_Module__loggerFxn0 xdc_runtime_Main_Module__loggerFxn0__C = ((CT__xdc_runtime_Main_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(xdc_runtime_Main_Module__loggerFxn1__C, ".const:xdc_runtime_Main_Module__loggerFxn1__C");
__FAR__ const CT__xdc_runtime_Main_Module__loggerFxn1 xdc_runtime_Main_Module__loggerFxn1__C = ((CT__xdc_runtime_Main_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(xdc_runtime_Main_Module__loggerFxn2__C, ".const:xdc_runtime_Main_Module__loggerFxn2__C");
__FAR__ const CT__xdc_runtime_Main_Module__loggerFxn2 xdc_runtime_Main_Module__loggerFxn2__C = ((CT__xdc_runtime_Main_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(xdc_runtime_Main_Module__loggerFxn4__C, ".const:xdc_runtime_Main_Module__loggerFxn4__C");
__FAR__ const CT__xdc_runtime_Main_Module__loggerFxn4 xdc_runtime_Main_Module__loggerFxn4__C = ((CT__xdc_runtime_Main_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(xdc_runtime_Main_Module__loggerFxn8__C, ".const:xdc_runtime_Main_Module__loggerFxn8__C");
__FAR__ const CT__xdc_runtime_Main_Module__loggerFxn8 xdc_runtime_Main_Module__loggerFxn8__C = ((CT__xdc_runtime_Main_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(xdc_runtime_Main_Object__count__C, ".const:xdc_runtime_Main_Object__count__C");
__FAR__ const CT__xdc_runtime_Main_Object__count xdc_runtime_Main_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(xdc_runtime_Main_Object__heap__C, ".const:xdc_runtime_Main_Object__heap__C");
__FAR__ const CT__xdc_runtime_Main_Object__heap xdc_runtime_Main_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(xdc_runtime_Main_Object__sizeof__C, ".const:xdc_runtime_Main_Object__sizeof__C");
__FAR__ const CT__xdc_runtime_Main_Object__sizeof xdc_runtime_Main_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(xdc_runtime_Main_Object__table__C, ".const:xdc_runtime_Main_Object__table__C");
__FAR__ const CT__xdc_runtime_Main_Object__table xdc_runtime_Main_Object__table__C = 0;


/*
 * ======== xdc.runtime.Main_Module_GateProxy INITIALIZERS ========
 */


/*
 * ======== xdc.runtime.Memory INITIALIZERS ========
 */

/* Module__state__V */
#if defined (__ICCARM__)
#pragma location = ".data_xdc_runtime_Memory_Module__state__V"
#endif
#if defined(__llvm__) && defined(__ti__)
xdc_runtime_Memory_Module_State__ xdc_runtime_Memory_Module__state__V __attribute__ ((section(".data:xdc_runtime_Memory_Module__state__V")));
#elif defined(__GNUC__) && !(defined(__MACH__) && defined(__APPLE__))
#ifndef __TI_COMPILER_VERSION__
xdc_runtime_Memory_Module_State__ xdc_runtime_Memory_Module__state__V __attribute__ ((section(".data_xdc_runtime_Memory_Module__state__V")));
#endif
#endif
xdc_runtime_Memory_Module_State__ xdc_runtime_Memory_Module__state__V = {
    (xdc_SizeT)0x8,  /* maxDefaultTypeAlign */
};

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(xdc_runtime_Memory_Module__diagsEnabled__C, ".const:xdc_runtime_Memory_Module__diagsEnabled__C");
__FAR__ const CT__xdc_runtime_Memory_Module__diagsEnabled xdc_runtime_Memory_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(xdc_runtime_Memory_Module__diagsIncluded__C, ".const:xdc_runtime_Memory_Module__diagsIncluded__C");
__FAR__ const CT__xdc_runtime_Memory_Module__diagsIncluded xdc_runtime_Memory_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
#pragma DATA_SECTION(xdc_runtime_Memory_Module__diagsMask__C, ".const:xdc_runtime_Memory_Module__diagsMask__C");
__FAR__ const CT__xdc_runtime_Memory_Module__diagsMask xdc_runtime_Memory_Module__diagsMask__C = ((CT__xdc_runtime_Memory_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(xdc_runtime_Memory_Module__gateObj__C, ".const:xdc_runtime_Memory_Module__gateObj__C");
__FAR__ const CT__xdc_runtime_Memory_Module__gateObj xdc_runtime_Memory_Module__gateObj__C = ((CT__xdc_runtime_Memory_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(xdc_runtime_Memory_Module__gatePrms__C, ".const:xdc_runtime_Memory_Module__gatePrms__C");
__FAR__ const CT__xdc_runtime_Memory_Module__gatePrms xdc_runtime_Memory_Module__gatePrms__C = ((CT__xdc_runtime_Memory_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(xdc_runtime_Memory_Module__id__C, ".const:xdc_runtime_Memory_Module__id__C");
__FAR__ const CT__xdc_runtime_Memory_Module__id xdc_runtime_Memory_Module__id__C = (xdc_Bits16)0x800a;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(xdc_runtime_Memory_Module__loggerDefined__C, ".const:xdc_runtime_Memory_Module__loggerDefined__C");
__FAR__ const CT__xdc_runtime_Memory_Module__loggerDefined xdc_runtime_Memory_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(xdc_runtime_Memory_Module__loggerObj__C, ".const:xdc_runtime_Memory_Module__loggerObj__C");
__FAR__ const CT__xdc_runtime_Memory_Module__loggerObj xdc_runtime_Memory_Module__loggerObj__C = ((CT__xdc_runtime_Memory_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(xdc_runtime_Memory_Module__loggerFxn0__C, ".const:xdc_runtime_Memory_Module__loggerFxn0__C");
__FAR__ const CT__xdc_runtime_Memory_Module__loggerFxn0 xdc_runtime_Memory_Module__loggerFxn0__C = ((CT__xdc_runtime_Memory_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(xdc_runtime_Memory_Module__loggerFxn1__C, ".const:xdc_runtime_Memory_Module__loggerFxn1__C");
__FAR__ const CT__xdc_runtime_Memory_Module__loggerFxn1 xdc_runtime_Memory_Module__loggerFxn1__C = ((CT__xdc_runtime_Memory_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(xdc_runtime_Memory_Module__loggerFxn2__C, ".const:xdc_runtime_Memory_Module__loggerFxn2__C");
__FAR__ const CT__xdc_runtime_Memory_Module__loggerFxn2 xdc_runtime_Memory_Module__loggerFxn2__C = ((CT__xdc_runtime_Memory_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(xdc_runtime_Memory_Module__loggerFxn4__C, ".const:xdc_runtime_Memory_Module__loggerFxn4__C");
__FAR__ const CT__xdc_runtime_Memory_Module__loggerFxn4 xdc_runtime_Memory_Module__loggerFxn4__C = ((CT__xdc_runtime_Memory_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(xdc_runtime_Memory_Module__loggerFxn8__C, ".const:xdc_runtime_Memory_Module__loggerFxn8__C");
__FAR__ const CT__xdc_runtime_Memory_Module__loggerFxn8 xdc_runtime_Memory_Module__loggerFxn8__C = ((CT__xdc_runtime_Memory_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(xdc_runtime_Memory_Object__count__C, ".const:xdc_runtime_Memory_Object__count__C");
__FAR__ const CT__xdc_runtime_Memory_Object__count xdc_runtime_Memory_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(xdc_runtime_Memory_Object__heap__C, ".const:xdc_runtime_Memory_Object__heap__C");
__FAR__ const CT__xdc_runtime_Memory_Object__heap xdc_runtime_Memory_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(xdc_runtime_Memory_Object__sizeof__C, ".const:xdc_runtime_Memory_Object__sizeof__C");
__FAR__ const CT__xdc_runtime_Memory_Object__sizeof xdc_runtime_Memory_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(xdc_runtime_Memory_Object__table__C, ".const:xdc_runtime_Memory_Object__table__C");
__FAR__ const CT__xdc_runtime_Memory_Object__table xdc_runtime_Memory_Object__table__C = 0;

/* defaultHeapInstance__C */
#pragma DATA_SECTION(xdc_runtime_Memory_defaultHeapInstance__C, ".const:xdc_runtime_Memory_defaultHeapInstance__C");
__FAR__ const CT__xdc_runtime_Memory_defaultHeapInstance xdc_runtime_Memory_defaultHeapInstance__C = (xdc_runtime_IHeap_Handle)&ti_sysbios_heaps_HeapMem_Object__table__V[0];


/*
 * ======== xdc.runtime.Memory_HeapProxy INITIALIZERS ========
 */


/*
 * ======== xdc.runtime.Registry INITIALIZERS ========
 */

/* Module__state__V */
#if defined (__ICCARM__)
#pragma location = ".data_xdc_runtime_Registry_Module__state__V"
#endif
#if defined(__llvm__) && defined(__ti__)
xdc_runtime_Registry_Module_State__ xdc_runtime_Registry_Module__state__V __attribute__ ((section(".data:xdc_runtime_Registry_Module__state__V")));
#elif defined(__GNUC__) && !(defined(__MACH__) && defined(__APPLE__))
#ifndef __TI_COMPILER_VERSION__
xdc_runtime_Registry_Module_State__ xdc_runtime_Registry_Module__state__V __attribute__ ((section(".data_xdc_runtime_Registry_Module__state__V")));
#endif
#endif
xdc_runtime_Registry_Module_State__ xdc_runtime_Registry_Module__state__V = {
    ((xdc_runtime_Types_RegDesc*)0),  /* listHead */
    (xdc_Bits16)0x7fff,  /* curId */
};

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(xdc_runtime_Registry_Module__diagsEnabled__C, ".const:xdc_runtime_Registry_Module__diagsEnabled__C");
__FAR__ const CT__xdc_runtime_Registry_Module__diagsEnabled xdc_runtime_Registry_Module__diagsEnabled__C = (xdc_Bits32)0x90;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(xdc_runtime_Registry_Module__diagsIncluded__C, ".const:xdc_runtime_Registry_Module__diagsIncluded__C");
__FAR__ const CT__xdc_runtime_Registry_Module__diagsIncluded xdc_runtime_Registry_Module__diagsIncluded__C = (xdc_Bits32)0x90;

/* Module__diagsMask__C */
#pragma DATA_SECTION(xdc_runtime_Registry_Module__diagsMask__C, ".const:xdc_runtime_Registry_Module__diagsMask__C");
__FAR__ const CT__xdc_runtime_Registry_Module__diagsMask xdc_runtime_Registry_Module__diagsMask__C = ((CT__xdc_runtime_Registry_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(xdc_runtime_Registry_Module__gateObj__C, ".const:xdc_runtime_Registry_Module__gateObj__C");
__FAR__ const CT__xdc_runtime_Registry_Module__gateObj xdc_runtime_Registry_Module__gateObj__C = ((CT__xdc_runtime_Registry_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(xdc_runtime_Registry_Module__gatePrms__C, ".const:xdc_runtime_Registry_Module__gatePrms__C");
__FAR__ const CT__xdc_runtime_Registry_Module__gatePrms xdc_runtime_Registry_Module__gatePrms__C = ((CT__xdc_runtime_Registry_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(xdc_runtime_Registry_Module__id__C, ".const:xdc_runtime_Registry_Module__id__C");
__FAR__ const CT__xdc_runtime_Registry_Module__id xdc_runtime_Registry_Module__id__C = (xdc_Bits16)0x800b;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(xdc_runtime_Registry_Module__loggerDefined__C, ".const:xdc_runtime_Registry_Module__loggerDefined__C");
__FAR__ const CT__xdc_runtime_Registry_Module__loggerDefined xdc_runtime_Registry_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(xdc_runtime_Registry_Module__loggerObj__C, ".const:xdc_runtime_Registry_Module__loggerObj__C");
__FAR__ const CT__xdc_runtime_Registry_Module__loggerObj xdc_runtime_Registry_Module__loggerObj__C = ((CT__xdc_runtime_Registry_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(xdc_runtime_Registry_Module__loggerFxn0__C, ".const:xdc_runtime_Registry_Module__loggerFxn0__C");
__FAR__ const CT__xdc_runtime_Registry_Module__loggerFxn0 xdc_runtime_Registry_Module__loggerFxn0__C = ((CT__xdc_runtime_Registry_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(xdc_runtime_Registry_Module__loggerFxn1__C, ".const:xdc_runtime_Registry_Module__loggerFxn1__C");
__FAR__ const CT__xdc_runtime_Registry_Module__loggerFxn1 xdc_runtime_Registry_Module__loggerFxn1__C = ((CT__xdc_runtime_Registry_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(xdc_runtime_Registry_Module__loggerFxn2__C, ".const:xdc_runtime_Registry_Module__loggerFxn2__C");
__FAR__ const CT__xdc_runtime_Registry_Module__loggerFxn2 xdc_runtime_Registry_Module__loggerFxn2__C = ((CT__xdc_runtime_Registry_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(xdc_runtime_Registry_Module__loggerFxn4__C, ".const:xdc_runtime_Registry_Module__loggerFxn4__C");
__FAR__ const CT__xdc_runtime_Registry_Module__loggerFxn4 xdc_runtime_Registry_Module__loggerFxn4__C = ((CT__xdc_runtime_Registry_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(xdc_runtime_Registry_Module__loggerFxn8__C, ".const:xdc_runtime_Registry_Module__loggerFxn8__C");
__FAR__ const CT__xdc_runtime_Registry_Module__loggerFxn8 xdc_runtime_Registry_Module__loggerFxn8__C = ((CT__xdc_runtime_Registry_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(xdc_runtime_Registry_Object__count__C, ".const:xdc_runtime_Registry_Object__count__C");
__FAR__ const CT__xdc_runtime_Registry_Object__count xdc_runtime_Registry_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(xdc_runtime_Registry_Object__heap__C, ".const:xdc_runtime_Registry_Object__heap__C");
__FAR__ const CT__xdc_runtime_Registry_Object__heap xdc_runtime_Registry_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(xdc_runtime_Registry_Object__sizeof__C, ".const:xdc_runtime_Registry_Object__sizeof__C");
__FAR__ const CT__xdc_runtime_Registry_Object__sizeof xdc_runtime_Registry_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(xdc_runtime_Registry_Object__table__C, ".const:xdc_runtime_Registry_Object__table__C");
__FAR__ const CT__xdc_runtime_Registry_Object__table xdc_runtime_Registry_Object__table__C = 0;


/*
 * ======== xdc.runtime.Startup INITIALIZERS ========
 */

/* Module__state__V */
#if defined (__ICCARM__)
#pragma location = ".data_xdc_runtime_Startup_Module__state__V"
#endif
#if defined(__llvm__) && defined(__ti__)
xdc_runtime_Startup_Module_State__ xdc_runtime_Startup_Module__state__V __attribute__ ((section(".data:xdc_runtime_Startup_Module__state__V")));
#elif defined(__GNUC__) && !(defined(__MACH__) && defined(__APPLE__))
#ifndef __TI_COMPILER_VERSION__
xdc_runtime_Startup_Module_State__ xdc_runtime_Startup_Module__state__V __attribute__ ((section(".data_xdc_runtime_Startup_Module__state__V")));
#endif
#endif
xdc_runtime_Startup_Module_State__ xdc_runtime_Startup_Module__state__V = {
    ((xdc_Int*)0),  /* stateTab */
    0,  /* execFlag */
    0,  /* rtsDoneFlag */
};

/* --> xdc_runtime_Startup_firstFxns__A */
#pragma DATA_SECTION(xdc_runtime_Startup_firstFxns__A, ".const:xdc_runtime_Startup_firstFxns__A");
const __T1_xdc_runtime_Startup_firstFxns xdc_runtime_Startup_firstFxns__A[2] = {
    ((xdc_Void(*)(xdc_Void))((xdc_Fxn)ti_sysbios_heaps_HeapMem_init__I)),  /* [0] */
    ((xdc_Void(*)(xdc_Void))((xdc_Fxn)ti_sysbios_hal_Hwi_initStack)),  /* [1] */
};

/* --> xdc_runtime_Startup_sfxnTab__A */
#pragma DATA_SECTION(xdc_runtime_Startup_sfxnTab__A, ".const:xdc_runtime_Startup_sfxnTab__A");
const __T1_xdc_runtime_Startup_sfxnTab xdc_runtime_Startup_sfxnTab__A[10] = {
    ((xdc_Int(*)(xdc_Int))((xdc_Fxn)xdc_runtime_System_Module_startup__E)),  /* [0] */
    ((xdc_Int(*)(xdc_Int))((xdc_Fxn)ti_sysbios_knl_Clock_Module_startup__E)),  /* [1] */
    ((xdc_Int(*)(xdc_Int))((xdc_Fxn)ti_sysbios_knl_Swi_Module_startup__E)),  /* [2] */
    ((xdc_Int(*)(xdc_Int))((xdc_Fxn)ti_sysbios_knl_Task_Module_startup__E)),  /* [3] */
    ((xdc_Int(*)(xdc_Int))((xdc_Fxn)ti_sysbios_heaps_HeapBuf_Module_startup__E)),  /* [4] */
    ((xdc_Int(*)(xdc_Int))((xdc_Fxn)ti_sysbios_family_arm_v7r_vim_Hwi_Module_startup__E)),  /* [5] */
    ((xdc_Int(*)(xdc_Int))((xdc_Fxn)ti_sysbios_family_arm_exc_Exception_Module_startup__E)),  /* [6] */
    ((xdc_Int(*)(xdc_Int))((xdc_Fxn)ti_sysbios_hal_Hwi_Module_startup__E)),  /* [7] */
    ((xdc_Int(*)(xdc_Int))((xdc_Fxn)ti_sysbios_family_arm_v7r_tms570_Core_Module_startup__E)),  /* [8] */
    ((xdc_Int(*)(xdc_Int))((xdc_Fxn)ti_sysbios_timers_rti_Timer_Module_startup__E)),  /* [9] */
};

/* --> xdc_runtime_Startup_sfxnRts__A */
#pragma DATA_SECTION(xdc_runtime_Startup_sfxnRts__A, ".const:xdc_runtime_Startup_sfxnRts__A");
const __T1_xdc_runtime_Startup_sfxnRts xdc_runtime_Startup_sfxnRts__A[10] = {
    1,  /* [0] */
    0,  /* [1] */
    0,  /* [2] */
    0,  /* [3] */
    1,  /* [4] */
    0,  /* [5] */
    0,  /* [6] */
    0,  /* [7] */
    0,  /* [8] */
    0,  /* [9] */
};

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(xdc_runtime_Startup_Module__diagsEnabled__C, ".const:xdc_runtime_Startup_Module__diagsEnabled__C");
__FAR__ const CT__xdc_runtime_Startup_Module__diagsEnabled xdc_runtime_Startup_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(xdc_runtime_Startup_Module__diagsIncluded__C, ".const:xdc_runtime_Startup_Module__diagsIncluded__C");
__FAR__ const CT__xdc_runtime_Startup_Module__diagsIncluded xdc_runtime_Startup_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
#pragma DATA_SECTION(xdc_runtime_Startup_Module__diagsMask__C, ".const:xdc_runtime_Startup_Module__diagsMask__C");
__FAR__ const CT__xdc_runtime_Startup_Module__diagsMask xdc_runtime_Startup_Module__diagsMask__C = ((CT__xdc_runtime_Startup_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(xdc_runtime_Startup_Module__gateObj__C, ".const:xdc_runtime_Startup_Module__gateObj__C");
__FAR__ const CT__xdc_runtime_Startup_Module__gateObj xdc_runtime_Startup_Module__gateObj__C = ((CT__xdc_runtime_Startup_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(xdc_runtime_Startup_Module__gatePrms__C, ".const:xdc_runtime_Startup_Module__gatePrms__C");
__FAR__ const CT__xdc_runtime_Startup_Module__gatePrms xdc_runtime_Startup_Module__gatePrms__C = ((CT__xdc_runtime_Startup_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(xdc_runtime_Startup_Module__id__C, ".const:xdc_runtime_Startup_Module__id__C");
__FAR__ const CT__xdc_runtime_Startup_Module__id xdc_runtime_Startup_Module__id__C = (xdc_Bits16)0x800c;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(xdc_runtime_Startup_Module__loggerDefined__C, ".const:xdc_runtime_Startup_Module__loggerDefined__C");
__FAR__ const CT__xdc_runtime_Startup_Module__loggerDefined xdc_runtime_Startup_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(xdc_runtime_Startup_Module__loggerObj__C, ".const:xdc_runtime_Startup_Module__loggerObj__C");
__FAR__ const CT__xdc_runtime_Startup_Module__loggerObj xdc_runtime_Startup_Module__loggerObj__C = ((CT__xdc_runtime_Startup_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(xdc_runtime_Startup_Module__loggerFxn0__C, ".const:xdc_runtime_Startup_Module__loggerFxn0__C");
__FAR__ const CT__xdc_runtime_Startup_Module__loggerFxn0 xdc_runtime_Startup_Module__loggerFxn0__C = ((CT__xdc_runtime_Startup_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(xdc_runtime_Startup_Module__loggerFxn1__C, ".const:xdc_runtime_Startup_Module__loggerFxn1__C");
__FAR__ const CT__xdc_runtime_Startup_Module__loggerFxn1 xdc_runtime_Startup_Module__loggerFxn1__C = ((CT__xdc_runtime_Startup_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(xdc_runtime_Startup_Module__loggerFxn2__C, ".const:xdc_runtime_Startup_Module__loggerFxn2__C");
__FAR__ const CT__xdc_runtime_Startup_Module__loggerFxn2 xdc_runtime_Startup_Module__loggerFxn2__C = ((CT__xdc_runtime_Startup_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(xdc_runtime_Startup_Module__loggerFxn4__C, ".const:xdc_runtime_Startup_Module__loggerFxn4__C");
__FAR__ const CT__xdc_runtime_Startup_Module__loggerFxn4 xdc_runtime_Startup_Module__loggerFxn4__C = ((CT__xdc_runtime_Startup_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(xdc_runtime_Startup_Module__loggerFxn8__C, ".const:xdc_runtime_Startup_Module__loggerFxn8__C");
__FAR__ const CT__xdc_runtime_Startup_Module__loggerFxn8 xdc_runtime_Startup_Module__loggerFxn8__C = ((CT__xdc_runtime_Startup_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(xdc_runtime_Startup_Object__count__C, ".const:xdc_runtime_Startup_Object__count__C");
__FAR__ const CT__xdc_runtime_Startup_Object__count xdc_runtime_Startup_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(xdc_runtime_Startup_Object__heap__C, ".const:xdc_runtime_Startup_Object__heap__C");
__FAR__ const CT__xdc_runtime_Startup_Object__heap xdc_runtime_Startup_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(xdc_runtime_Startup_Object__sizeof__C, ".const:xdc_runtime_Startup_Object__sizeof__C");
__FAR__ const CT__xdc_runtime_Startup_Object__sizeof xdc_runtime_Startup_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(xdc_runtime_Startup_Object__table__C, ".const:xdc_runtime_Startup_Object__table__C");
__FAR__ const CT__xdc_runtime_Startup_Object__table xdc_runtime_Startup_Object__table__C = 0;

/* maxPasses__C */
#pragma DATA_SECTION(xdc_runtime_Startup_maxPasses__C, ".const:xdc_runtime_Startup_maxPasses__C");
__FAR__ const CT__xdc_runtime_Startup_maxPasses xdc_runtime_Startup_maxPasses__C = (xdc_Int)0x20;

/* firstFxns__C */
#pragma DATA_SECTION(xdc_runtime_Startup_firstFxns__C, ".const:xdc_runtime_Startup_firstFxns__C");
__FAR__ const CT__xdc_runtime_Startup_firstFxns xdc_runtime_Startup_firstFxns__C = {2, ((__T1_xdc_runtime_Startup_firstFxns *)xdc_runtime_Startup_firstFxns__A)};

/* lastFxns__C */
#pragma DATA_SECTION(xdc_runtime_Startup_lastFxns__C, ".const:xdc_runtime_Startup_lastFxns__C");
__FAR__ const CT__xdc_runtime_Startup_lastFxns xdc_runtime_Startup_lastFxns__C = {0, 0};

/* startModsFxn__C */
#pragma DATA_SECTION(xdc_runtime_Startup_startModsFxn__C, ".const:xdc_runtime_Startup_startModsFxn__C");
__FAR__ const CT__xdc_runtime_Startup_startModsFxn xdc_runtime_Startup_startModsFxn__C = ((CT__xdc_runtime_Startup_startModsFxn)((xdc_Fxn)xdc_runtime_Startup_startMods__I));

/* execImpl__C */
#pragma DATA_SECTION(xdc_runtime_Startup_execImpl__C, ".const:xdc_runtime_Startup_execImpl__C");
__FAR__ const CT__xdc_runtime_Startup_execImpl xdc_runtime_Startup_execImpl__C = ((CT__xdc_runtime_Startup_execImpl)((xdc_Fxn)xdc_runtime_Startup_exec__I));

/* sfxnTab__C */
#pragma DATA_SECTION(xdc_runtime_Startup_sfxnTab__C, ".const:xdc_runtime_Startup_sfxnTab__C");
__FAR__ const CT__xdc_runtime_Startup_sfxnTab xdc_runtime_Startup_sfxnTab__C = ((CT__xdc_runtime_Startup_sfxnTab)xdc_runtime_Startup_sfxnTab__A);

/* sfxnRts__C */
#pragma DATA_SECTION(xdc_runtime_Startup_sfxnRts__C, ".const:xdc_runtime_Startup_sfxnRts__C");
__FAR__ const CT__xdc_runtime_Startup_sfxnRts xdc_runtime_Startup_sfxnRts__C = ((CT__xdc_runtime_Startup_sfxnRts)xdc_runtime_Startup_sfxnRts__A);


/*
 * ======== xdc.runtime.SysStd INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(xdc_runtime_SysStd_Module__diagsEnabled__C, ".const:xdc_runtime_SysStd_Module__diagsEnabled__C");
__FAR__ const CT__xdc_runtime_SysStd_Module__diagsEnabled xdc_runtime_SysStd_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(xdc_runtime_SysStd_Module__diagsIncluded__C, ".const:xdc_runtime_SysStd_Module__diagsIncluded__C");
__FAR__ const CT__xdc_runtime_SysStd_Module__diagsIncluded xdc_runtime_SysStd_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
#pragma DATA_SECTION(xdc_runtime_SysStd_Module__diagsMask__C, ".const:xdc_runtime_SysStd_Module__diagsMask__C");
__FAR__ const CT__xdc_runtime_SysStd_Module__diagsMask xdc_runtime_SysStd_Module__diagsMask__C = ((CT__xdc_runtime_SysStd_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(xdc_runtime_SysStd_Module__gateObj__C, ".const:xdc_runtime_SysStd_Module__gateObj__C");
__FAR__ const CT__xdc_runtime_SysStd_Module__gateObj xdc_runtime_SysStd_Module__gateObj__C = ((CT__xdc_runtime_SysStd_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(xdc_runtime_SysStd_Module__gatePrms__C, ".const:xdc_runtime_SysStd_Module__gatePrms__C");
__FAR__ const CT__xdc_runtime_SysStd_Module__gatePrms xdc_runtime_SysStd_Module__gatePrms__C = ((CT__xdc_runtime_SysStd_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(xdc_runtime_SysStd_Module__id__C, ".const:xdc_runtime_SysStd_Module__id__C");
__FAR__ const CT__xdc_runtime_SysStd_Module__id xdc_runtime_SysStd_Module__id__C = (xdc_Bits16)0x800e;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(xdc_runtime_SysStd_Module__loggerDefined__C, ".const:xdc_runtime_SysStd_Module__loggerDefined__C");
__FAR__ const CT__xdc_runtime_SysStd_Module__loggerDefined xdc_runtime_SysStd_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(xdc_runtime_SysStd_Module__loggerObj__C, ".const:xdc_runtime_SysStd_Module__loggerObj__C");
__FAR__ const CT__xdc_runtime_SysStd_Module__loggerObj xdc_runtime_SysStd_Module__loggerObj__C = ((CT__xdc_runtime_SysStd_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(xdc_runtime_SysStd_Module__loggerFxn0__C, ".const:xdc_runtime_SysStd_Module__loggerFxn0__C");
__FAR__ const CT__xdc_runtime_SysStd_Module__loggerFxn0 xdc_runtime_SysStd_Module__loggerFxn0__C = ((CT__xdc_runtime_SysStd_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(xdc_runtime_SysStd_Module__loggerFxn1__C, ".const:xdc_runtime_SysStd_Module__loggerFxn1__C");
__FAR__ const CT__xdc_runtime_SysStd_Module__loggerFxn1 xdc_runtime_SysStd_Module__loggerFxn1__C = ((CT__xdc_runtime_SysStd_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(xdc_runtime_SysStd_Module__loggerFxn2__C, ".const:xdc_runtime_SysStd_Module__loggerFxn2__C");
__FAR__ const CT__xdc_runtime_SysStd_Module__loggerFxn2 xdc_runtime_SysStd_Module__loggerFxn2__C = ((CT__xdc_runtime_SysStd_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(xdc_runtime_SysStd_Module__loggerFxn4__C, ".const:xdc_runtime_SysStd_Module__loggerFxn4__C");
__FAR__ const CT__xdc_runtime_SysStd_Module__loggerFxn4 xdc_runtime_SysStd_Module__loggerFxn4__C = ((CT__xdc_runtime_SysStd_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(xdc_runtime_SysStd_Module__loggerFxn8__C, ".const:xdc_runtime_SysStd_Module__loggerFxn8__C");
__FAR__ const CT__xdc_runtime_SysStd_Module__loggerFxn8 xdc_runtime_SysStd_Module__loggerFxn8__C = ((CT__xdc_runtime_SysStd_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(xdc_runtime_SysStd_Object__count__C, ".const:xdc_runtime_SysStd_Object__count__C");
__FAR__ const CT__xdc_runtime_SysStd_Object__count xdc_runtime_SysStd_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(xdc_runtime_SysStd_Object__heap__C, ".const:xdc_runtime_SysStd_Object__heap__C");
__FAR__ const CT__xdc_runtime_SysStd_Object__heap xdc_runtime_SysStd_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(xdc_runtime_SysStd_Object__sizeof__C, ".const:xdc_runtime_SysStd_Object__sizeof__C");
__FAR__ const CT__xdc_runtime_SysStd_Object__sizeof xdc_runtime_SysStd_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(xdc_runtime_SysStd_Object__table__C, ".const:xdc_runtime_SysStd_Object__table__C");
__FAR__ const CT__xdc_runtime_SysStd_Object__table xdc_runtime_SysStd_Object__table__C = 0;


/*
 * ======== xdc.runtime.System INITIALIZERS ========
 */

/* --> xdc_runtime_System_Module_State_0_atexitHandlers__A */
__T1_xdc_runtime_System_Module_State__atexitHandlers xdc_runtime_System_Module_State_0_atexitHandlers__A[8] = {
    ((xdc_Void(*)(xdc_Int))0),  /* [0] */
    ((xdc_Void(*)(xdc_Int))0),  /* [1] */
    ((xdc_Void(*)(xdc_Int))0),  /* [2] */
    ((xdc_Void(*)(xdc_Int))0),  /* [3] */
    ((xdc_Void(*)(xdc_Int))0),  /* [4] */
    ((xdc_Void(*)(xdc_Int))0),  /* [5] */
    ((xdc_Void(*)(xdc_Int))0),  /* [6] */
    ((xdc_Void(*)(xdc_Int))0),  /* [7] */
};

/* Module__state__V */
#if defined (__ICCARM__)
#pragma location = ".data_xdc_runtime_System_Module__state__V"
#endif
#if defined(__llvm__) && defined(__ti__)
xdc_runtime_System_Module_State__ xdc_runtime_System_Module__state__V __attribute__ ((section(".data:xdc_runtime_System_Module__state__V")));
#elif defined(__GNUC__) && !(defined(__MACH__) && defined(__APPLE__))
#ifndef __TI_COMPILER_VERSION__
xdc_runtime_System_Module_State__ xdc_runtime_System_Module__state__V __attribute__ ((section(".data_xdc_runtime_System_Module__state__V")));
#endif
#endif
xdc_runtime_System_Module_State__ xdc_runtime_System_Module__state__V = {
    ((void*)xdc_runtime_System_Module_State_0_atexitHandlers__A),  /* atexitHandlers */
    (xdc_Int)0x0,  /* numAtexitHandlers */
};

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(xdc_runtime_System_Module__diagsEnabled__C, ".const:xdc_runtime_System_Module__diagsEnabled__C");
__FAR__ const CT__xdc_runtime_System_Module__diagsEnabled xdc_runtime_System_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(xdc_runtime_System_Module__diagsIncluded__C, ".const:xdc_runtime_System_Module__diagsIncluded__C");
__FAR__ const CT__xdc_runtime_System_Module__diagsIncluded xdc_runtime_System_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
#pragma DATA_SECTION(xdc_runtime_System_Module__diagsMask__C, ".const:xdc_runtime_System_Module__diagsMask__C");
__FAR__ const CT__xdc_runtime_System_Module__diagsMask xdc_runtime_System_Module__diagsMask__C = ((CT__xdc_runtime_System_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(xdc_runtime_System_Module__gateObj__C, ".const:xdc_runtime_System_Module__gateObj__C");
__FAR__ const CT__xdc_runtime_System_Module__gateObj xdc_runtime_System_Module__gateObj__C = ((CT__xdc_runtime_System_Module__gateObj)((const void*)(xdc_runtime_IGateProvider_Handle)&ti_sysbios_gates_GateHwi_Object__table__V[0]));

/* Module__gatePrms__C */
#pragma DATA_SECTION(xdc_runtime_System_Module__gatePrms__C, ".const:xdc_runtime_System_Module__gatePrms__C");
__FAR__ const CT__xdc_runtime_System_Module__gatePrms xdc_runtime_System_Module__gatePrms__C = ((CT__xdc_runtime_System_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(xdc_runtime_System_Module__id__C, ".const:xdc_runtime_System_Module__id__C");
__FAR__ const CT__xdc_runtime_System_Module__id xdc_runtime_System_Module__id__C = (xdc_Bits16)0x800d;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(xdc_runtime_System_Module__loggerDefined__C, ".const:xdc_runtime_System_Module__loggerDefined__C");
__FAR__ const CT__xdc_runtime_System_Module__loggerDefined xdc_runtime_System_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(xdc_runtime_System_Module__loggerObj__C, ".const:xdc_runtime_System_Module__loggerObj__C");
__FAR__ const CT__xdc_runtime_System_Module__loggerObj xdc_runtime_System_Module__loggerObj__C = ((CT__xdc_runtime_System_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(xdc_runtime_System_Module__loggerFxn0__C, ".const:xdc_runtime_System_Module__loggerFxn0__C");
__FAR__ const CT__xdc_runtime_System_Module__loggerFxn0 xdc_runtime_System_Module__loggerFxn0__C = ((CT__xdc_runtime_System_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(xdc_runtime_System_Module__loggerFxn1__C, ".const:xdc_runtime_System_Module__loggerFxn1__C");
__FAR__ const CT__xdc_runtime_System_Module__loggerFxn1 xdc_runtime_System_Module__loggerFxn1__C = ((CT__xdc_runtime_System_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(xdc_runtime_System_Module__loggerFxn2__C, ".const:xdc_runtime_System_Module__loggerFxn2__C");
__FAR__ const CT__xdc_runtime_System_Module__loggerFxn2 xdc_runtime_System_Module__loggerFxn2__C = ((CT__xdc_runtime_System_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(xdc_runtime_System_Module__loggerFxn4__C, ".const:xdc_runtime_System_Module__loggerFxn4__C");
__FAR__ const CT__xdc_runtime_System_Module__loggerFxn4 xdc_runtime_System_Module__loggerFxn4__C = ((CT__xdc_runtime_System_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(xdc_runtime_System_Module__loggerFxn8__C, ".const:xdc_runtime_System_Module__loggerFxn8__C");
__FAR__ const CT__xdc_runtime_System_Module__loggerFxn8 xdc_runtime_System_Module__loggerFxn8__C = ((CT__xdc_runtime_System_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(xdc_runtime_System_Object__count__C, ".const:xdc_runtime_System_Object__count__C");
__FAR__ const CT__xdc_runtime_System_Object__count xdc_runtime_System_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(xdc_runtime_System_Object__heap__C, ".const:xdc_runtime_System_Object__heap__C");
__FAR__ const CT__xdc_runtime_System_Object__heap xdc_runtime_System_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(xdc_runtime_System_Object__sizeof__C, ".const:xdc_runtime_System_Object__sizeof__C");
__FAR__ const CT__xdc_runtime_System_Object__sizeof xdc_runtime_System_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(xdc_runtime_System_Object__table__C, ".const:xdc_runtime_System_Object__table__C");
__FAR__ const CT__xdc_runtime_System_Object__table xdc_runtime_System_Object__table__C = 0;

/* A_cannotFitIntoArg__C */
#pragma DATA_SECTION(xdc_runtime_System_A_cannotFitIntoArg__C, ".const:xdc_runtime_System_A_cannotFitIntoArg__C");
__FAR__ const CT__xdc_runtime_System_A_cannotFitIntoArg xdc_runtime_System_A_cannotFitIntoArg__C = (((xdc_runtime_Assert_Id)352) << 16 | 16);

/* maxAtexitHandlers__C */
#pragma DATA_SECTION(xdc_runtime_System_maxAtexitHandlers__C, ".const:xdc_runtime_System_maxAtexitHandlers__C");
__FAR__ const CT__xdc_runtime_System_maxAtexitHandlers xdc_runtime_System_maxAtexitHandlers__C = (xdc_Int)0x8;

/* abortFxn__C */
#pragma DATA_SECTION(xdc_runtime_System_abortFxn__C, ".const:xdc_runtime_System_abortFxn__C");
__FAR__ const CT__xdc_runtime_System_abortFxn xdc_runtime_System_abortFxn__C = ((CT__xdc_runtime_System_abortFxn)((xdc_Fxn)xdc_runtime_System_abortStd__E));

/* exitFxn__C */
#pragma DATA_SECTION(xdc_runtime_System_exitFxn__C, ".const:xdc_runtime_System_exitFxn__C");
__FAR__ const CT__xdc_runtime_System_exitFxn xdc_runtime_System_exitFxn__C = ((CT__xdc_runtime_System_exitFxn)((xdc_Fxn)xdc_runtime_System_exitStd__E));

/* extendFxn__C */
#pragma DATA_SECTION(xdc_runtime_System_extendFxn__C, ".const:xdc_runtime_System_extendFxn__C");
__FAR__ const CT__xdc_runtime_System_extendFxn xdc_runtime_System_extendFxn__C = ((CT__xdc_runtime_System_extendFxn)((xdc_Fxn)xdc_runtime_System_printfExtend__I));


/*
 * ======== xdc.runtime.System_Module_GateProxy INITIALIZERS ========
 */


/*
 * ======== xdc.runtime.System_SupportProxy INITIALIZERS ========
 */


/*
 * ======== xdc.runtime.Text INITIALIZERS ========
 */

/* Module__state__V */
#if defined (__ICCARM__)
#pragma location = ".data_xdc_runtime_Text_Module__state__V"
#endif
#if defined(__llvm__) && defined(__ti__)
xdc_runtime_Text_Module_State__ xdc_runtime_Text_Module__state__V __attribute__ ((section(".data:xdc_runtime_Text_Module__state__V")));
#elif defined(__GNUC__) && !(defined(__MACH__) && defined(__APPLE__))
#ifndef __TI_COMPILER_VERSION__
xdc_runtime_Text_Module_State__ xdc_runtime_Text_Module__state__V __attribute__ ((section(".data_xdc_runtime_Text_Module__state__V")));
#endif
#endif
xdc_runtime_Text_Module_State__ xdc_runtime_Text_Module__state__V = {
    ((xdc_CPtr)(&xdc_runtime_Text_charTab__A[0])),  /* charBase */
    ((xdc_CPtr)(&xdc_runtime_Text_nodeTab__A[0])),  /* nodeBase */
};

/* --> xdc_runtime_Text_charTab__A */
#pragma DATA_SECTION(xdc_runtime_Text_charTab__A, ".const:xdc_runtime_Text_charTab__A");
const __T1_xdc_runtime_Text_charTab xdc_runtime_Text_charTab__A[6894] = {
    (xdc_Char)0x0,  /* [0] */
    (xdc_Char)0x41,  /* [1] */
    (xdc_Char)0x5f,  /* [2] */
    (xdc_Char)0x69,  /* [3] */
    (xdc_Char)0x6e,  /* [4] */
    (xdc_Char)0x69,  /* [5] */
    (xdc_Char)0x74,  /* [6] */
    (xdc_Char)0x69,  /* [7] */
    (xdc_Char)0x61,  /* [8] */
    (xdc_Char)0x6c,  /* [9] */
    (xdc_Char)0x69,  /* [10] */
    (xdc_Char)0x7a,  /* [11] */
    (xdc_Char)0x65,  /* [12] */
    (xdc_Char)0x64,  /* [13] */
    (xdc_Char)0x50,  /* [14] */
    (xdc_Char)0x61,  /* [15] */
    (xdc_Char)0x72,  /* [16] */
    (xdc_Char)0x61,  /* [17] */
    (xdc_Char)0x6d,  /* [18] */
    (xdc_Char)0x73,  /* [19] */
    (xdc_Char)0x3a,  /* [20] */
    (xdc_Char)0x20,  /* [21] */
    (xdc_Char)0x75,  /* [22] */
    (xdc_Char)0x6e,  /* [23] */
    (xdc_Char)0x69,  /* [24] */
    (xdc_Char)0x6e,  /* [25] */
    (xdc_Char)0x69,  /* [26] */
    (xdc_Char)0x74,  /* [27] */
    (xdc_Char)0x69,  /* [28] */
    (xdc_Char)0x61,  /* [29] */
    (xdc_Char)0x6c,  /* [30] */
    (xdc_Char)0x69,  /* [31] */
    (xdc_Char)0x7a,  /* [32] */
    (xdc_Char)0x65,  /* [33] */
    (xdc_Char)0x64,  /* [34] */
    (xdc_Char)0x20,  /* [35] */
    (xdc_Char)0x50,  /* [36] */
    (xdc_Char)0x61,  /* [37] */
    (xdc_Char)0x72,  /* [38] */
    (xdc_Char)0x61,  /* [39] */
    (xdc_Char)0x6d,  /* [40] */
    (xdc_Char)0x73,  /* [41] */
    (xdc_Char)0x20,  /* [42] */
    (xdc_Char)0x73,  /* [43] */
    (xdc_Char)0x74,  /* [44] */
    (xdc_Char)0x72,  /* [45] */
    (xdc_Char)0x75,  /* [46] */
    (xdc_Char)0x63,  /* [47] */
    (xdc_Char)0x74,  /* [48] */
    (xdc_Char)0x0,  /* [49] */
    (xdc_Char)0x48,  /* [50] */
    (xdc_Char)0x65,  /* [51] */
    (xdc_Char)0x61,  /* [52] */
    (xdc_Char)0x70,  /* [53] */
    (xdc_Char)0x4d,  /* [54] */
    (xdc_Char)0x69,  /* [55] */
    (xdc_Char)0x6e,  /* [56] */
    (xdc_Char)0x5f,  /* [57] */
    (xdc_Char)0x63,  /* [58] */
    (xdc_Char)0x72,  /* [59] */
    (xdc_Char)0x65,  /* [60] */
    (xdc_Char)0x61,  /* [61] */
    (xdc_Char)0x74,  /* [62] */
    (xdc_Char)0x65,  /* [63] */
    (xdc_Char)0x20,  /* [64] */
    (xdc_Char)0x63,  /* [65] */
    (xdc_Char)0x61,  /* [66] */
    (xdc_Char)0x6e,  /* [67] */
    (xdc_Char)0x6e,  /* [68] */
    (xdc_Char)0x6f,  /* [69] */
    (xdc_Char)0x74,  /* [70] */
    (xdc_Char)0x20,  /* [71] */
    (xdc_Char)0x68,  /* [72] */
    (xdc_Char)0x61,  /* [73] */
    (xdc_Char)0x76,  /* [74] */
    (xdc_Char)0x65,  /* [75] */
    (xdc_Char)0x20,  /* [76] */
    (xdc_Char)0x61,  /* [77] */
    (xdc_Char)0x20,  /* [78] */
    (xdc_Char)0x7a,  /* [79] */
    (xdc_Char)0x65,  /* [80] */
    (xdc_Char)0x72,  /* [81] */
    (xdc_Char)0x6f,  /* [82] */
    (xdc_Char)0x20,  /* [83] */
    (xdc_Char)0x73,  /* [84] */
    (xdc_Char)0x69,  /* [85] */
    (xdc_Char)0x7a,  /* [86] */
    (xdc_Char)0x65,  /* [87] */
    (xdc_Char)0x20,  /* [88] */
    (xdc_Char)0x76,  /* [89] */
    (xdc_Char)0x61,  /* [90] */
    (xdc_Char)0x6c,  /* [91] */
    (xdc_Char)0x75,  /* [92] */
    (xdc_Char)0x65,  /* [93] */
    (xdc_Char)0x0,  /* [94] */
    (xdc_Char)0x48,  /* [95] */
    (xdc_Char)0x65,  /* [96] */
    (xdc_Char)0x61,  /* [97] */
    (xdc_Char)0x70,  /* [98] */
    (xdc_Char)0x53,  /* [99] */
    (xdc_Char)0x74,  /* [100] */
    (xdc_Char)0x64,  /* [101] */
    (xdc_Char)0x5f,  /* [102] */
    (xdc_Char)0x63,  /* [103] */
    (xdc_Char)0x72,  /* [104] */
    (xdc_Char)0x65,  /* [105] */
    (xdc_Char)0x61,  /* [106] */
    (xdc_Char)0x74,  /* [107] */
    (xdc_Char)0x65,  /* [108] */
    (xdc_Char)0x20,  /* [109] */
    (xdc_Char)0x63,  /* [110] */
    (xdc_Char)0x61,  /* [111] */
    (xdc_Char)0x6e,  /* [112] */
    (xdc_Char)0x6e,  /* [113] */
    (xdc_Char)0x6f,  /* [114] */
    (xdc_Char)0x74,  /* [115] */
    (xdc_Char)0x20,  /* [116] */
    (xdc_Char)0x68,  /* [117] */
    (xdc_Char)0x61,  /* [118] */
    (xdc_Char)0x76,  /* [119] */
    (xdc_Char)0x65,  /* [120] */
    (xdc_Char)0x20,  /* [121] */
    (xdc_Char)0x61,  /* [122] */
    (xdc_Char)0x20,  /* [123] */
    (xdc_Char)0x7a,  /* [124] */
    (xdc_Char)0x65,  /* [125] */
    (xdc_Char)0x72,  /* [126] */
    (xdc_Char)0x6f,  /* [127] */
    (xdc_Char)0x20,  /* [128] */
    (xdc_Char)0x73,  /* [129] */
    (xdc_Char)0x69,  /* [130] */
    (xdc_Char)0x7a,  /* [131] */
    (xdc_Char)0x65,  /* [132] */
    (xdc_Char)0x20,  /* [133] */
    (xdc_Char)0x76,  /* [134] */
    (xdc_Char)0x61,  /* [135] */
    (xdc_Char)0x6c,  /* [136] */
    (xdc_Char)0x75,  /* [137] */
    (xdc_Char)0x65,  /* [138] */
    (xdc_Char)0x0,  /* [139] */
    (xdc_Char)0x48,  /* [140] */
    (xdc_Char)0x65,  /* [141] */
    (xdc_Char)0x61,  /* [142] */
    (xdc_Char)0x70,  /* [143] */
    (xdc_Char)0x53,  /* [144] */
    (xdc_Char)0x74,  /* [145] */
    (xdc_Char)0x64,  /* [146] */
    (xdc_Char)0x5f,  /* [147] */
    (xdc_Char)0x61,  /* [148] */
    (xdc_Char)0x6c,  /* [149] */
    (xdc_Char)0x6c,  /* [150] */
    (xdc_Char)0x6f,  /* [151] */
    (xdc_Char)0x63,  /* [152] */
    (xdc_Char)0x20,  /* [153] */
    (xdc_Char)0x61,  /* [154] */
    (xdc_Char)0x6c,  /* [155] */
    (xdc_Char)0x69,  /* [156] */
    (xdc_Char)0x67,  /* [157] */
    (xdc_Char)0x6e,  /* [158] */
    (xdc_Char)0x6d,  /* [159] */
    (xdc_Char)0x65,  /* [160] */
    (xdc_Char)0x6e,  /* [161] */
    (xdc_Char)0x74,  /* [162] */
    (xdc_Char)0x20,  /* [163] */
    (xdc_Char)0x6d,  /* [164] */
    (xdc_Char)0x75,  /* [165] */
    (xdc_Char)0x73,  /* [166] */
    (xdc_Char)0x74,  /* [167] */
    (xdc_Char)0x20,  /* [168] */
    (xdc_Char)0x62,  /* [169] */
    (xdc_Char)0x65,  /* [170] */
    (xdc_Char)0x20,  /* [171] */
    (xdc_Char)0x61,  /* [172] */
    (xdc_Char)0x20,  /* [173] */
    (xdc_Char)0x70,  /* [174] */
    (xdc_Char)0x6f,  /* [175] */
    (xdc_Char)0x77,  /* [176] */
    (xdc_Char)0x65,  /* [177] */
    (xdc_Char)0x72,  /* [178] */
    (xdc_Char)0x20,  /* [179] */
    (xdc_Char)0x6f,  /* [180] */
    (xdc_Char)0x66,  /* [181] */
    (xdc_Char)0x20,  /* [182] */
    (xdc_Char)0x32,  /* [183] */
    (xdc_Char)0x0,  /* [184] */
    (xdc_Char)0x48,  /* [185] */
    (xdc_Char)0x65,  /* [186] */
    (xdc_Char)0x61,  /* [187] */
    (xdc_Char)0x70,  /* [188] */
    (xdc_Char)0x53,  /* [189] */
    (xdc_Char)0x74,  /* [190] */
    (xdc_Char)0x64,  /* [191] */
    (xdc_Char)0x20,  /* [192] */
    (xdc_Char)0x69,  /* [193] */
    (xdc_Char)0x6e,  /* [194] */
    (xdc_Char)0x73,  /* [195] */
    (xdc_Char)0x74,  /* [196] */
    (xdc_Char)0x61,  /* [197] */
    (xdc_Char)0x6e,  /* [198] */
    (xdc_Char)0x63,  /* [199] */
    (xdc_Char)0x65,  /* [200] */
    (xdc_Char)0x20,  /* [201] */
    (xdc_Char)0x74,  /* [202] */
    (xdc_Char)0x6f,  /* [203] */
    (xdc_Char)0x74,  /* [204] */
    (xdc_Char)0x61,  /* [205] */
    (xdc_Char)0x6c,  /* [206] */
    (xdc_Char)0x46,  /* [207] */
    (xdc_Char)0x72,  /* [208] */
    (xdc_Char)0x65,  /* [209] */
    (xdc_Char)0x65,  /* [210] */
    (xdc_Char)0x53,  /* [211] */
    (xdc_Char)0x69,  /* [212] */
    (xdc_Char)0x7a,  /* [213] */
    (xdc_Char)0x65,  /* [214] */
    (xdc_Char)0x20,  /* [215] */
    (xdc_Char)0x69,  /* [216] */
    (xdc_Char)0x73,  /* [217] */
    (xdc_Char)0x20,  /* [218] */
    (xdc_Char)0x67,  /* [219] */
    (xdc_Char)0x72,  /* [220] */
    (xdc_Char)0x65,  /* [221] */
    (xdc_Char)0x61,  /* [222] */
    (xdc_Char)0x74,  /* [223] */
    (xdc_Char)0x65,  /* [224] */
    (xdc_Char)0x72,  /* [225] */
    (xdc_Char)0x20,  /* [226] */
    (xdc_Char)0x74,  /* [227] */
    (xdc_Char)0x68,  /* [228] */
    (xdc_Char)0x61,  /* [229] */
    (xdc_Char)0x6e,  /* [230] */
    (xdc_Char)0x20,  /* [231] */
    (xdc_Char)0x73,  /* [232] */
    (xdc_Char)0x74,  /* [233] */
    (xdc_Char)0x61,  /* [234] */
    (xdc_Char)0x72,  /* [235] */
    (xdc_Char)0x74,  /* [236] */
    (xdc_Char)0x69,  /* [237] */
    (xdc_Char)0x6e,  /* [238] */
    (xdc_Char)0x67,  /* [239] */
    (xdc_Char)0x20,  /* [240] */
    (xdc_Char)0x73,  /* [241] */
    (xdc_Char)0x69,  /* [242] */
    (xdc_Char)0x7a,  /* [243] */
    (xdc_Char)0x65,  /* [244] */
    (xdc_Char)0x0,  /* [245] */
    (xdc_Char)0x48,  /* [246] */
    (xdc_Char)0x65,  /* [247] */
    (xdc_Char)0x61,  /* [248] */
    (xdc_Char)0x70,  /* [249] */
    (xdc_Char)0x53,  /* [250] */
    (xdc_Char)0x74,  /* [251] */
    (xdc_Char)0x64,  /* [252] */
    (xdc_Char)0x5f,  /* [253] */
    (xdc_Char)0x61,  /* [254] */
    (xdc_Char)0x6c,  /* [255] */
    (xdc_Char)0x6c,  /* [256] */
    (xdc_Char)0x6f,  /* [257] */
    (xdc_Char)0x63,  /* [258] */
    (xdc_Char)0x20,  /* [259] */
    (xdc_Char)0x2d,  /* [260] */
    (xdc_Char)0x20,  /* [261] */
    (xdc_Char)0x72,  /* [262] */
    (xdc_Char)0x65,  /* [263] */
    (xdc_Char)0x71,  /* [264] */
    (xdc_Char)0x75,  /* [265] */
    (xdc_Char)0x65,  /* [266] */
    (xdc_Char)0x73,  /* [267] */
    (xdc_Char)0x74,  /* [268] */
    (xdc_Char)0x65,  /* [269] */
    (xdc_Char)0x64,  /* [270] */
    (xdc_Char)0x20,  /* [271] */
    (xdc_Char)0x61,  /* [272] */
    (xdc_Char)0x6c,  /* [273] */
    (xdc_Char)0x69,  /* [274] */
    (xdc_Char)0x67,  /* [275] */
    (xdc_Char)0x6e,  /* [276] */
    (xdc_Char)0x6d,  /* [277] */
    (xdc_Char)0x65,  /* [278] */
    (xdc_Char)0x6e,  /* [279] */
    (xdc_Char)0x74,  /* [280] */
    (xdc_Char)0x20,  /* [281] */
    (xdc_Char)0x69,  /* [282] */
    (xdc_Char)0x73,  /* [283] */
    (xdc_Char)0x20,  /* [284] */
    (xdc_Char)0x67,  /* [285] */
    (xdc_Char)0x72,  /* [286] */
    (xdc_Char)0x65,  /* [287] */
    (xdc_Char)0x61,  /* [288] */
    (xdc_Char)0x74,  /* [289] */
    (xdc_Char)0x65,  /* [290] */
    (xdc_Char)0x72,  /* [291] */
    (xdc_Char)0x20,  /* [292] */
    (xdc_Char)0x74,  /* [293] */
    (xdc_Char)0x68,  /* [294] */
    (xdc_Char)0x61,  /* [295] */
    (xdc_Char)0x6e,  /* [296] */
    (xdc_Char)0x20,  /* [297] */
    (xdc_Char)0x61,  /* [298] */
    (xdc_Char)0x6c,  /* [299] */
    (xdc_Char)0x6c,  /* [300] */
    (xdc_Char)0x6f,  /* [301] */
    (xdc_Char)0x77,  /* [302] */
    (xdc_Char)0x65,  /* [303] */
    (xdc_Char)0x64,  /* [304] */
    (xdc_Char)0x0,  /* [305] */
    (xdc_Char)0x41,  /* [306] */
    (xdc_Char)0x5f,  /* [307] */
    (xdc_Char)0x69,  /* [308] */
    (xdc_Char)0x6e,  /* [309] */
    (xdc_Char)0x76,  /* [310] */
    (xdc_Char)0x61,  /* [311] */
    (xdc_Char)0x6c,  /* [312] */
    (xdc_Char)0x69,  /* [313] */
    (xdc_Char)0x64,  /* [314] */
    (xdc_Char)0x4c,  /* [315] */
    (xdc_Char)0x6f,  /* [316] */
    (xdc_Char)0x67,  /* [317] */
    (xdc_Char)0x67,  /* [318] */
    (xdc_Char)0x65,  /* [319] */
    (xdc_Char)0x72,  /* [320] */
    (xdc_Char)0x3a,  /* [321] */
    (xdc_Char)0x20,  /* [322] */
    (xdc_Char)0x54,  /* [323] */
    (xdc_Char)0x68,  /* [324] */
    (xdc_Char)0x65,  /* [325] */
    (xdc_Char)0x20,  /* [326] */
    (xdc_Char)0x6c,  /* [327] */
    (xdc_Char)0x6f,  /* [328] */
    (xdc_Char)0x67,  /* [329] */
    (xdc_Char)0x67,  /* [330] */
    (xdc_Char)0x65,  /* [331] */
    (xdc_Char)0x72,  /* [332] */
    (xdc_Char)0x20,  /* [333] */
    (xdc_Char)0x69,  /* [334] */
    (xdc_Char)0x64,  /* [335] */
    (xdc_Char)0x20,  /* [336] */
    (xdc_Char)0x25,  /* [337] */
    (xdc_Char)0x64,  /* [338] */
    (xdc_Char)0x20,  /* [339] */
    (xdc_Char)0x69,  /* [340] */
    (xdc_Char)0x73,  /* [341] */
    (xdc_Char)0x20,  /* [342] */
    (xdc_Char)0x69,  /* [343] */
    (xdc_Char)0x6e,  /* [344] */
    (xdc_Char)0x76,  /* [345] */
    (xdc_Char)0x61,  /* [346] */
    (xdc_Char)0x6c,  /* [347] */
    (xdc_Char)0x69,  /* [348] */
    (xdc_Char)0x64,  /* [349] */
    (xdc_Char)0x2e,  /* [350] */
    (xdc_Char)0x0,  /* [351] */
    (xdc_Char)0x41,  /* [352] */
    (xdc_Char)0x5f,  /* [353] */
    (xdc_Char)0x63,  /* [354] */
    (xdc_Char)0x61,  /* [355] */
    (xdc_Char)0x6e,  /* [356] */
    (xdc_Char)0x6e,  /* [357] */
    (xdc_Char)0x6f,  /* [358] */
    (xdc_Char)0x74,  /* [359] */
    (xdc_Char)0x46,  /* [360] */
    (xdc_Char)0x69,  /* [361] */
    (xdc_Char)0x74,  /* [362] */
    (xdc_Char)0x49,  /* [363] */
    (xdc_Char)0x6e,  /* [364] */
    (xdc_Char)0x74,  /* [365] */
    (xdc_Char)0x6f,  /* [366] */
    (xdc_Char)0x41,  /* [367] */
    (xdc_Char)0x72,  /* [368] */
    (xdc_Char)0x67,  /* [369] */
    (xdc_Char)0x3a,  /* [370] */
    (xdc_Char)0x20,  /* [371] */
    (xdc_Char)0x73,  /* [372] */
    (xdc_Char)0x69,  /* [373] */
    (xdc_Char)0x7a,  /* [374] */
    (xdc_Char)0x65,  /* [375] */
    (xdc_Char)0x6f,  /* [376] */
    (xdc_Char)0x66,  /* [377] */
    (xdc_Char)0x28,  /* [378] */
    (xdc_Char)0x46,  /* [379] */
    (xdc_Char)0x6c,  /* [380] */
    (xdc_Char)0x6f,  /* [381] */
    (xdc_Char)0x61,  /* [382] */
    (xdc_Char)0x74,  /* [383] */
    (xdc_Char)0x29,  /* [384] */
    (xdc_Char)0x20,  /* [385] */
    (xdc_Char)0x3e,  /* [386] */
    (xdc_Char)0x20,  /* [387] */
    (xdc_Char)0x73,  /* [388] */
    (xdc_Char)0x69,  /* [389] */
    (xdc_Char)0x7a,  /* [390] */
    (xdc_Char)0x65,  /* [391] */
    (xdc_Char)0x6f,  /* [392] */
    (xdc_Char)0x66,  /* [393] */
    (xdc_Char)0x28,  /* [394] */
    (xdc_Char)0x41,  /* [395] */
    (xdc_Char)0x72,  /* [396] */
    (xdc_Char)0x67,  /* [397] */
    (xdc_Char)0x29,  /* [398] */
    (xdc_Char)0x0,  /* [399] */
    (xdc_Char)0x41,  /* [400] */
    (xdc_Char)0x5f,  /* [401] */
    (xdc_Char)0x6e,  /* [402] */
    (xdc_Char)0x75,  /* [403] */
    (xdc_Char)0x6c,  /* [404] */
    (xdc_Char)0x6c,  /* [405] */
    (xdc_Char)0x50,  /* [406] */
    (xdc_Char)0x6f,  /* [407] */
    (xdc_Char)0x69,  /* [408] */
    (xdc_Char)0x6e,  /* [409] */
    (xdc_Char)0x74,  /* [410] */
    (xdc_Char)0x65,  /* [411] */
    (xdc_Char)0x72,  /* [412] */
    (xdc_Char)0x3a,  /* [413] */
    (xdc_Char)0x20,  /* [414] */
    (xdc_Char)0x50,  /* [415] */
    (xdc_Char)0x6f,  /* [416] */
    (xdc_Char)0x69,  /* [417] */
    (xdc_Char)0x6e,  /* [418] */
    (xdc_Char)0x74,  /* [419] */
    (xdc_Char)0x65,  /* [420] */
    (xdc_Char)0x72,  /* [421] */
    (xdc_Char)0x20,  /* [422] */
    (xdc_Char)0x69,  /* [423] */
    (xdc_Char)0x73,  /* [424] */
    (xdc_Char)0x20,  /* [425] */
    (xdc_Char)0x6e,  /* [426] */
    (xdc_Char)0x75,  /* [427] */
    (xdc_Char)0x6c,  /* [428] */
    (xdc_Char)0x6c,  /* [429] */
    (xdc_Char)0x0,  /* [430] */
    (xdc_Char)0x41,  /* [431] */
    (xdc_Char)0x5f,  /* [432] */
    (xdc_Char)0x69,  /* [433] */
    (xdc_Char)0x6e,  /* [434] */
    (xdc_Char)0x76,  /* [435] */
    (xdc_Char)0x61,  /* [436] */
    (xdc_Char)0x6c,  /* [437] */
    (xdc_Char)0x69,  /* [438] */
    (xdc_Char)0x64,  /* [439] */
    (xdc_Char)0x52,  /* [440] */
    (xdc_Char)0x65,  /* [441] */
    (xdc_Char)0x67,  /* [442] */
    (xdc_Char)0x69,  /* [443] */
    (xdc_Char)0x6f,  /* [444] */
    (xdc_Char)0x6e,  /* [445] */
    (xdc_Char)0x49,  /* [446] */
    (xdc_Char)0x64,  /* [447] */
    (xdc_Char)0x3a,  /* [448] */
    (xdc_Char)0x20,  /* [449] */
    (xdc_Char)0x4d,  /* [450] */
    (xdc_Char)0x50,  /* [451] */
    (xdc_Char)0x55,  /* [452] */
    (xdc_Char)0x20,  /* [453] */
    (xdc_Char)0x52,  /* [454] */
    (xdc_Char)0x65,  /* [455] */
    (xdc_Char)0x67,  /* [456] */
    (xdc_Char)0x69,  /* [457] */
    (xdc_Char)0x6f,  /* [458] */
    (xdc_Char)0x6e,  /* [459] */
    (xdc_Char)0x20,  /* [460] */
    (xdc_Char)0x6e,  /* [461] */
    (xdc_Char)0x75,  /* [462] */
    (xdc_Char)0x6d,  /* [463] */
    (xdc_Char)0x62,  /* [464] */
    (xdc_Char)0x65,  /* [465] */
    (xdc_Char)0x72,  /* [466] */
    (xdc_Char)0x20,  /* [467] */
    (xdc_Char)0x70,  /* [468] */
    (xdc_Char)0x61,  /* [469] */
    (xdc_Char)0x73,  /* [470] */
    (xdc_Char)0x73,  /* [471] */
    (xdc_Char)0x65,  /* [472] */
    (xdc_Char)0x64,  /* [473] */
    (xdc_Char)0x20,  /* [474] */
    (xdc_Char)0x69,  /* [475] */
    (xdc_Char)0x73,  /* [476] */
    (xdc_Char)0x20,  /* [477] */
    (xdc_Char)0x69,  /* [478] */
    (xdc_Char)0x6e,  /* [479] */
    (xdc_Char)0x76,  /* [480] */
    (xdc_Char)0x61,  /* [481] */
    (xdc_Char)0x6c,  /* [482] */
    (xdc_Char)0x69,  /* [483] */
    (xdc_Char)0x64,  /* [484] */
    (xdc_Char)0x2e,  /* [485] */
    (xdc_Char)0x0,  /* [486] */
    (xdc_Char)0x41,  /* [487] */
    (xdc_Char)0x5f,  /* [488] */
    (xdc_Char)0x75,  /* [489] */
    (xdc_Char)0x6e,  /* [490] */
    (xdc_Char)0x61,  /* [491] */
    (xdc_Char)0x6c,  /* [492] */
    (xdc_Char)0x69,  /* [493] */
    (xdc_Char)0x67,  /* [494] */
    (xdc_Char)0x6e,  /* [495] */
    (xdc_Char)0x65,  /* [496] */
    (xdc_Char)0x64,  /* [497] */
    (xdc_Char)0x42,  /* [498] */
    (xdc_Char)0x61,  /* [499] */
    (xdc_Char)0x73,  /* [500] */
    (xdc_Char)0x65,  /* [501] */
    (xdc_Char)0x41,  /* [502] */
    (xdc_Char)0x64,  /* [503] */
    (xdc_Char)0x64,  /* [504] */
    (xdc_Char)0x72,  /* [505] */
    (xdc_Char)0x3a,  /* [506] */
    (xdc_Char)0x20,  /* [507] */
    (xdc_Char)0x4d,  /* [508] */
    (xdc_Char)0x50,  /* [509] */
    (xdc_Char)0x55,  /* [510] */
    (xdc_Char)0x20,  /* [511] */
    (xdc_Char)0x72,  /* [512] */
    (xdc_Char)0x65,  /* [513] */
    (xdc_Char)0x67,  /* [514] */
    (xdc_Char)0x69,  /* [515] */
    (xdc_Char)0x6f,  /* [516] */
    (xdc_Char)0x6e,  /* [517] */
    (xdc_Char)0x20,  /* [518] */
    (xdc_Char)0x62,  /* [519] */
    (xdc_Char)0x61,  /* [520] */
    (xdc_Char)0x73,  /* [521] */
    (xdc_Char)0x65,  /* [522] */
    (xdc_Char)0x20,  /* [523] */
    (xdc_Char)0x61,  /* [524] */
    (xdc_Char)0x64,  /* [525] */
    (xdc_Char)0x64,  /* [526] */
    (xdc_Char)0x72,  /* [527] */
    (xdc_Char)0x65,  /* [528] */
    (xdc_Char)0x73,  /* [529] */
    (xdc_Char)0x73,  /* [530] */
    (xdc_Char)0x20,  /* [531] */
    (xdc_Char)0x6e,  /* [532] */
    (xdc_Char)0x6f,  /* [533] */
    (xdc_Char)0x74,  /* [534] */
    (xdc_Char)0x20,  /* [535] */
    (xdc_Char)0x61,  /* [536] */
    (xdc_Char)0x6c,  /* [537] */
    (xdc_Char)0x69,  /* [538] */
    (xdc_Char)0x67,  /* [539] */
    (xdc_Char)0x6e,  /* [540] */
    (xdc_Char)0x65,  /* [541] */
    (xdc_Char)0x64,  /* [542] */
    (xdc_Char)0x20,  /* [543] */
    (xdc_Char)0x74,  /* [544] */
    (xdc_Char)0x6f,  /* [545] */
    (xdc_Char)0x20,  /* [546] */
    (xdc_Char)0x73,  /* [547] */
    (xdc_Char)0x69,  /* [548] */
    (xdc_Char)0x7a,  /* [549] */
    (xdc_Char)0x65,  /* [550] */
    (xdc_Char)0x2e,  /* [551] */
    (xdc_Char)0x0,  /* [552] */
    (xdc_Char)0x41,  /* [553] */
    (xdc_Char)0x5f,  /* [554] */
    (xdc_Char)0x72,  /* [555] */
    (xdc_Char)0x65,  /* [556] */
    (xdc_Char)0x73,  /* [557] */
    (xdc_Char)0x65,  /* [558] */
    (xdc_Char)0x72,  /* [559] */
    (xdc_Char)0x76,  /* [560] */
    (xdc_Char)0x65,  /* [561] */
    (xdc_Char)0x64,  /* [562] */
    (xdc_Char)0x41,  /* [563] */
    (xdc_Char)0x74,  /* [564] */
    (xdc_Char)0x74,  /* [565] */
    (xdc_Char)0x72,  /* [566] */
    (xdc_Char)0x73,  /* [567] */
    (xdc_Char)0x3a,  /* [568] */
    (xdc_Char)0x20,  /* [569] */
    (xdc_Char)0x4d,  /* [570] */
    (xdc_Char)0x50,  /* [571] */
    (xdc_Char)0x55,  /* [572] */
    (xdc_Char)0x20,  /* [573] */
    (xdc_Char)0x72,  /* [574] */
    (xdc_Char)0x65,  /* [575] */
    (xdc_Char)0x67,  /* [576] */
    (xdc_Char)0x69,  /* [577] */
    (xdc_Char)0x6f,  /* [578] */
    (xdc_Char)0x6e,  /* [579] */
    (xdc_Char)0x20,  /* [580] */
    (xdc_Char)0x61,  /* [581] */
    (xdc_Char)0x74,  /* [582] */
    (xdc_Char)0x74,  /* [583] */
    (xdc_Char)0x72,  /* [584] */
    (xdc_Char)0x69,  /* [585] */
    (xdc_Char)0x62,  /* [586] */
    (xdc_Char)0x75,  /* [587] */
    (xdc_Char)0x74,  /* [588] */
    (xdc_Char)0x65,  /* [589] */
    (xdc_Char)0x73,  /* [590] */
    (xdc_Char)0x20,  /* [591] */
    (xdc_Char)0x73,  /* [592] */
    (xdc_Char)0x65,  /* [593] */
    (xdc_Char)0x74,  /* [594] */
    (xdc_Char)0x20,  /* [595] */
    (xdc_Char)0x74,  /* [596] */
    (xdc_Char)0x6f,  /* [597] */
    (xdc_Char)0x20,  /* [598] */
    (xdc_Char)0x72,  /* [599] */
    (xdc_Char)0x65,  /* [600] */
    (xdc_Char)0x73,  /* [601] */
    (xdc_Char)0x65,  /* [602] */
    (xdc_Char)0x72,  /* [603] */
    (xdc_Char)0x76,  /* [604] */
    (xdc_Char)0x65,  /* [605] */
    (xdc_Char)0x64,  /* [606] */
    (xdc_Char)0x20,  /* [607] */
    (xdc_Char)0x76,  /* [608] */
    (xdc_Char)0x61,  /* [609] */
    (xdc_Char)0x6c,  /* [610] */
    (xdc_Char)0x75,  /* [611] */
    (xdc_Char)0x65,  /* [612] */
    (xdc_Char)0x2e,  /* [613] */
    (xdc_Char)0x0,  /* [614] */
    (xdc_Char)0x41,  /* [615] */
    (xdc_Char)0x5f,  /* [616] */
    (xdc_Char)0x63,  /* [617] */
    (xdc_Char)0x6c,  /* [618] */
    (xdc_Char)0x6f,  /* [619] */
    (xdc_Char)0x63,  /* [620] */
    (xdc_Char)0x6b,  /* [621] */
    (xdc_Char)0x44,  /* [622] */
    (xdc_Char)0x69,  /* [623] */
    (xdc_Char)0x73,  /* [624] */
    (xdc_Char)0x61,  /* [625] */
    (xdc_Char)0x62,  /* [626] */
    (xdc_Char)0x6c,  /* [627] */
    (xdc_Char)0x65,  /* [628] */
    (xdc_Char)0x64,  /* [629] */
    (xdc_Char)0x3a,  /* [630] */
    (xdc_Char)0x20,  /* [631] */
    (xdc_Char)0x43,  /* [632] */
    (xdc_Char)0x61,  /* [633] */
    (xdc_Char)0x6e,  /* [634] */
    (xdc_Char)0x6e,  /* [635] */
    (xdc_Char)0x6f,  /* [636] */
    (xdc_Char)0x74,  /* [637] */
    (xdc_Char)0x20,  /* [638] */
    (xdc_Char)0x63,  /* [639] */
    (xdc_Char)0x72,  /* [640] */
    (xdc_Char)0x65,  /* [641] */
    (xdc_Char)0x61,  /* [642] */
    (xdc_Char)0x74,  /* [643] */
    (xdc_Char)0x65,  /* [644] */
    (xdc_Char)0x20,  /* [645] */
    (xdc_Char)0x61,  /* [646] */
    (xdc_Char)0x20,  /* [647] */
    (xdc_Char)0x63,  /* [648] */
    (xdc_Char)0x6c,  /* [649] */
    (xdc_Char)0x6f,  /* [650] */
    (xdc_Char)0x63,  /* [651] */
    (xdc_Char)0x6b,  /* [652] */
    (xdc_Char)0x20,  /* [653] */
    (xdc_Char)0x69,  /* [654] */
    (xdc_Char)0x6e,  /* [655] */
    (xdc_Char)0x73,  /* [656] */
    (xdc_Char)0x74,  /* [657] */
    (xdc_Char)0x61,  /* [658] */
    (xdc_Char)0x6e,  /* [659] */
    (xdc_Char)0x63,  /* [660] */
    (xdc_Char)0x65,  /* [661] */
    (xdc_Char)0x20,  /* [662] */
    (xdc_Char)0x77,  /* [663] */
    (xdc_Char)0x68,  /* [664] */
    (xdc_Char)0x65,  /* [665] */
    (xdc_Char)0x6e,  /* [666] */
    (xdc_Char)0x20,  /* [667] */
    (xdc_Char)0x42,  /* [668] */
    (xdc_Char)0x49,  /* [669] */
    (xdc_Char)0x4f,  /* [670] */
    (xdc_Char)0x53,  /* [671] */
    (xdc_Char)0x2e,  /* [672] */
    (xdc_Char)0x63,  /* [673] */
    (xdc_Char)0x6c,  /* [674] */
    (xdc_Char)0x6f,  /* [675] */
    (xdc_Char)0x63,  /* [676] */
    (xdc_Char)0x6b,  /* [677] */
    (xdc_Char)0x45,  /* [678] */
    (xdc_Char)0x6e,  /* [679] */
    (xdc_Char)0x61,  /* [680] */
    (xdc_Char)0x62,  /* [681] */
    (xdc_Char)0x6c,  /* [682] */
    (xdc_Char)0x65,  /* [683] */
    (xdc_Char)0x64,  /* [684] */
    (xdc_Char)0x20,  /* [685] */
    (xdc_Char)0x69,  /* [686] */
    (xdc_Char)0x73,  /* [687] */
    (xdc_Char)0x20,  /* [688] */
    (xdc_Char)0x66,  /* [689] */
    (xdc_Char)0x61,  /* [690] */
    (xdc_Char)0x6c,  /* [691] */
    (xdc_Char)0x73,  /* [692] */
    (xdc_Char)0x65,  /* [693] */
    (xdc_Char)0x2e,  /* [694] */
    (xdc_Char)0x0,  /* [695] */
    (xdc_Char)0x41,  /* [696] */
    (xdc_Char)0x5f,  /* [697] */
    (xdc_Char)0x62,  /* [698] */
    (xdc_Char)0x61,  /* [699] */
    (xdc_Char)0x64,  /* [700] */
    (xdc_Char)0x54,  /* [701] */
    (xdc_Char)0x68,  /* [702] */
    (xdc_Char)0x72,  /* [703] */
    (xdc_Char)0x65,  /* [704] */
    (xdc_Char)0x61,  /* [705] */
    (xdc_Char)0x64,  /* [706] */
    (xdc_Char)0x54,  /* [707] */
    (xdc_Char)0x79,  /* [708] */
    (xdc_Char)0x70,  /* [709] */
    (xdc_Char)0x65,  /* [710] */
    (xdc_Char)0x3a,  /* [711] */
    (xdc_Char)0x20,  /* [712] */
    (xdc_Char)0x43,  /* [713] */
    (xdc_Char)0x61,  /* [714] */
    (xdc_Char)0x6e,  /* [715] */
    (xdc_Char)0x6e,  /* [716] */
    (xdc_Char)0x6f,  /* [717] */
    (xdc_Char)0x74,  /* [718] */
    (xdc_Char)0x20,  /* [719] */
    (xdc_Char)0x63,  /* [720] */
    (xdc_Char)0x72,  /* [721] */
    (xdc_Char)0x65,  /* [722] */
    (xdc_Char)0x61,  /* [723] */
    (xdc_Char)0x74,  /* [724] */
    (xdc_Char)0x65,  /* [725] */
    (xdc_Char)0x2f,  /* [726] */
    (xdc_Char)0x64,  /* [727] */
    (xdc_Char)0x65,  /* [728] */
    (xdc_Char)0x6c,  /* [729] */
    (xdc_Char)0x65,  /* [730] */
    (xdc_Char)0x74,  /* [731] */
    (xdc_Char)0x65,  /* [732] */
    (xdc_Char)0x20,  /* [733] */
    (xdc_Char)0x61,  /* [734] */
    (xdc_Char)0x20,  /* [735] */
    (xdc_Char)0x43,  /* [736] */
    (xdc_Char)0x6c,  /* [737] */
    (xdc_Char)0x6f,  /* [738] */
    (xdc_Char)0x63,  /* [739] */
    (xdc_Char)0x6b,  /* [740] */
    (xdc_Char)0x20,  /* [741] */
    (xdc_Char)0x66,  /* [742] */
    (xdc_Char)0x72,  /* [743] */
    (xdc_Char)0x6f,  /* [744] */
    (xdc_Char)0x6d,  /* [745] */
    (xdc_Char)0x20,  /* [746] */
    (xdc_Char)0x48,  /* [747] */
    (xdc_Char)0x77,  /* [748] */
    (xdc_Char)0x69,  /* [749] */
    (xdc_Char)0x20,  /* [750] */
    (xdc_Char)0x6f,  /* [751] */
    (xdc_Char)0x72,  /* [752] */
    (xdc_Char)0x20,  /* [753] */
    (xdc_Char)0x53,  /* [754] */
    (xdc_Char)0x77,  /* [755] */
    (xdc_Char)0x69,  /* [756] */
    (xdc_Char)0x20,  /* [757] */
    (xdc_Char)0x74,  /* [758] */
    (xdc_Char)0x68,  /* [759] */
    (xdc_Char)0x72,  /* [760] */
    (xdc_Char)0x65,  /* [761] */
    (xdc_Char)0x61,  /* [762] */
    (xdc_Char)0x64,  /* [763] */
    (xdc_Char)0x2e,  /* [764] */
    (xdc_Char)0x0,  /* [765] */
    (xdc_Char)0x41,  /* [766] */
    (xdc_Char)0x5f,  /* [767] */
    (xdc_Char)0x6e,  /* [768] */
    (xdc_Char)0x75,  /* [769] */
    (xdc_Char)0x6c,  /* [770] */
    (xdc_Char)0x6c,  /* [771] */
    (xdc_Char)0x45,  /* [772] */
    (xdc_Char)0x76,  /* [773] */
    (xdc_Char)0x65,  /* [774] */
    (xdc_Char)0x6e,  /* [775] */
    (xdc_Char)0x74,  /* [776] */
    (xdc_Char)0x4d,  /* [777] */
    (xdc_Char)0x61,  /* [778] */
    (xdc_Char)0x73,  /* [779] */
    (xdc_Char)0x6b,  /* [780] */
    (xdc_Char)0x73,  /* [781] */
    (xdc_Char)0x3a,  /* [782] */
    (xdc_Char)0x20,  /* [783] */
    (xdc_Char)0x6f,  /* [784] */
    (xdc_Char)0x72,  /* [785] */
    (xdc_Char)0x4d,  /* [786] */
    (xdc_Char)0x61,  /* [787] */
    (xdc_Char)0x73,  /* [788] */
    (xdc_Char)0x6b,  /* [789] */
    (xdc_Char)0x20,  /* [790] */
    (xdc_Char)0x61,  /* [791] */
    (xdc_Char)0x6e,  /* [792] */
    (xdc_Char)0x64,  /* [793] */
    (xdc_Char)0x20,  /* [794] */
    (xdc_Char)0x61,  /* [795] */
    (xdc_Char)0x6e,  /* [796] */
    (xdc_Char)0x64,  /* [797] */
    (xdc_Char)0x4d,  /* [798] */
    (xdc_Char)0x61,  /* [799] */
    (xdc_Char)0x73,  /* [800] */
    (xdc_Char)0x6b,  /* [801] */
    (xdc_Char)0x20,  /* [802] */
    (xdc_Char)0x61,  /* [803] */
    (xdc_Char)0x72,  /* [804] */
    (xdc_Char)0x65,  /* [805] */
    (xdc_Char)0x20,  /* [806] */
    (xdc_Char)0x6e,  /* [807] */
    (xdc_Char)0x75,  /* [808] */
    (xdc_Char)0x6c,  /* [809] */
    (xdc_Char)0x6c,  /* [810] */
    (xdc_Char)0x2e,  /* [811] */
    (xdc_Char)0x0,  /* [812] */
    (xdc_Char)0x41,  /* [813] */
    (xdc_Char)0x5f,  /* [814] */
    (xdc_Char)0x6e,  /* [815] */
    (xdc_Char)0x75,  /* [816] */
    (xdc_Char)0x6c,  /* [817] */
    (xdc_Char)0x6c,  /* [818] */
    (xdc_Char)0x45,  /* [819] */
    (xdc_Char)0x76,  /* [820] */
    (xdc_Char)0x65,  /* [821] */
    (xdc_Char)0x6e,  /* [822] */
    (xdc_Char)0x74,  /* [823] */
    (xdc_Char)0x49,  /* [824] */
    (xdc_Char)0x64,  /* [825] */
    (xdc_Char)0x3a,  /* [826] */
    (xdc_Char)0x20,  /* [827] */
    (xdc_Char)0x70,  /* [828] */
    (xdc_Char)0x6f,  /* [829] */
    (xdc_Char)0x73,  /* [830] */
    (xdc_Char)0x74,  /* [831] */
    (xdc_Char)0x65,  /* [832] */
    (xdc_Char)0x64,  /* [833] */
    (xdc_Char)0x20,  /* [834] */
    (xdc_Char)0x65,  /* [835] */
    (xdc_Char)0x76,  /* [836] */
    (xdc_Char)0x65,  /* [837] */
    (xdc_Char)0x6e,  /* [838] */
    (xdc_Char)0x74,  /* [839] */
    (xdc_Char)0x49,  /* [840] */
    (xdc_Char)0x64,  /* [841] */
    (xdc_Char)0x20,  /* [842] */
    (xdc_Char)0x69,  /* [843] */
    (xdc_Char)0x73,  /* [844] */
    (xdc_Char)0x20,  /* [845] */
    (xdc_Char)0x6e,  /* [846] */
    (xdc_Char)0x75,  /* [847] */
    (xdc_Char)0x6c,  /* [848] */
    (xdc_Char)0x6c,  /* [849] */
    (xdc_Char)0x2e,  /* [850] */
    (xdc_Char)0x0,  /* [851] */
    (xdc_Char)0x41,  /* [852] */
    (xdc_Char)0x5f,  /* [853] */
    (xdc_Char)0x65,  /* [854] */
    (xdc_Char)0x76,  /* [855] */
    (xdc_Char)0x65,  /* [856] */
    (xdc_Char)0x6e,  /* [857] */
    (xdc_Char)0x74,  /* [858] */
    (xdc_Char)0x49,  /* [859] */
    (xdc_Char)0x6e,  /* [860] */
    (xdc_Char)0x55,  /* [861] */
    (xdc_Char)0x73,  /* [862] */
    (xdc_Char)0x65,  /* [863] */
    (xdc_Char)0x3a,  /* [864] */
    (xdc_Char)0x20,  /* [865] */
    (xdc_Char)0x45,  /* [866] */
    (xdc_Char)0x76,  /* [867] */
    (xdc_Char)0x65,  /* [868] */
    (xdc_Char)0x6e,  /* [869] */
    (xdc_Char)0x74,  /* [870] */
    (xdc_Char)0x20,  /* [871] */
    (xdc_Char)0x6f,  /* [872] */
    (xdc_Char)0x62,  /* [873] */
    (xdc_Char)0x6a,  /* [874] */
    (xdc_Char)0x65,  /* [875] */
    (xdc_Char)0x63,  /* [876] */
    (xdc_Char)0x74,  /* [877] */
    (xdc_Char)0x20,  /* [878] */
    (xdc_Char)0x61,  /* [879] */
    (xdc_Char)0x6c,  /* [880] */
    (xdc_Char)0x72,  /* [881] */
    (xdc_Char)0x65,  /* [882] */
    (xdc_Char)0x61,  /* [883] */
    (xdc_Char)0x64,  /* [884] */
    (xdc_Char)0x79,  /* [885] */
    (xdc_Char)0x20,  /* [886] */
    (xdc_Char)0x69,  /* [887] */
    (xdc_Char)0x6e,  /* [888] */
    (xdc_Char)0x20,  /* [889] */
    (xdc_Char)0x75,  /* [890] */
    (xdc_Char)0x73,  /* [891] */
    (xdc_Char)0x65,  /* [892] */
    (xdc_Char)0x2e,  /* [893] */
    (xdc_Char)0x0,  /* [894] */
    (xdc_Char)0x41,  /* [895] */
    (xdc_Char)0x5f,  /* [896] */
    (xdc_Char)0x62,  /* [897] */
    (xdc_Char)0x61,  /* [898] */
    (xdc_Char)0x64,  /* [899] */
    (xdc_Char)0x43,  /* [900] */
    (xdc_Char)0x6f,  /* [901] */
    (xdc_Char)0x6e,  /* [902] */
    (xdc_Char)0x74,  /* [903] */
    (xdc_Char)0x65,  /* [904] */
    (xdc_Char)0x78,  /* [905] */
    (xdc_Char)0x74,  /* [906] */
    (xdc_Char)0x3a,  /* [907] */
    (xdc_Char)0x20,  /* [908] */
    (xdc_Char)0x62,  /* [909] */
    (xdc_Char)0x61,  /* [910] */
    (xdc_Char)0x64,  /* [911] */
    (xdc_Char)0x20,  /* [912] */
    (xdc_Char)0x63,  /* [913] */
    (xdc_Char)0x61,  /* [914] */
    (xdc_Char)0x6c,  /* [915] */
    (xdc_Char)0x6c,  /* [916] */
    (xdc_Char)0x69,  /* [917] */
    (xdc_Char)0x6e,  /* [918] */
    (xdc_Char)0x67,  /* [919] */
    (xdc_Char)0x20,  /* [920] */
    (xdc_Char)0x63,  /* [921] */
    (xdc_Char)0x6f,  /* [922] */
    (xdc_Char)0x6e,  /* [923] */
    (xdc_Char)0x74,  /* [924] */
    (xdc_Char)0x65,  /* [925] */
    (xdc_Char)0x78,  /* [926] */
    (xdc_Char)0x74,  /* [927] */
    (xdc_Char)0x2e,  /* [928] */
    (xdc_Char)0x20,  /* [929] */
    (xdc_Char)0x4d,  /* [930] */
    (xdc_Char)0x75,  /* [931] */
    (xdc_Char)0x73,  /* [932] */
    (xdc_Char)0x74,  /* [933] */
    (xdc_Char)0x20,  /* [934] */
    (xdc_Char)0x62,  /* [935] */
    (xdc_Char)0x65,  /* [936] */
    (xdc_Char)0x20,  /* [937] */
    (xdc_Char)0x63,  /* [938] */
    (xdc_Char)0x61,  /* [939] */
    (xdc_Char)0x6c,  /* [940] */
    (xdc_Char)0x6c,  /* [941] */
    (xdc_Char)0x65,  /* [942] */
    (xdc_Char)0x64,  /* [943] */
    (xdc_Char)0x20,  /* [944] */
    (xdc_Char)0x66,  /* [945] */
    (xdc_Char)0x72,  /* [946] */
    (xdc_Char)0x6f,  /* [947] */
    (xdc_Char)0x6d,  /* [948] */
    (xdc_Char)0x20,  /* [949] */
    (xdc_Char)0x61,  /* [950] */
    (xdc_Char)0x20,  /* [951] */
    (xdc_Char)0x54,  /* [952] */
    (xdc_Char)0x61,  /* [953] */
    (xdc_Char)0x73,  /* [954] */
    (xdc_Char)0x6b,  /* [955] */
    (xdc_Char)0x2e,  /* [956] */
    (xdc_Char)0x0,  /* [957] */
    (xdc_Char)0x41,  /* [958] */
    (xdc_Char)0x5f,  /* [959] */
    (xdc_Char)0x70,  /* [960] */
    (xdc_Char)0x65,  /* [961] */
    (xdc_Char)0x6e,  /* [962] */
    (xdc_Char)0x64,  /* [963] */
    (xdc_Char)0x54,  /* [964] */
    (xdc_Char)0x61,  /* [965] */
    (xdc_Char)0x73,  /* [966] */
    (xdc_Char)0x6b,  /* [967] */
    (xdc_Char)0x44,  /* [968] */
    (xdc_Char)0x69,  /* [969] */
    (xdc_Char)0x73,  /* [970] */
    (xdc_Char)0x61,  /* [971] */
    (xdc_Char)0x62,  /* [972] */
    (xdc_Char)0x6c,  /* [973] */
    (xdc_Char)0x65,  /* [974] */
    (xdc_Char)0x64,  /* [975] */
    (xdc_Char)0x3a,  /* [976] */
    (xdc_Char)0x20,  /* [977] */
    (xdc_Char)0x43,  /* [978] */
    (xdc_Char)0x61,  /* [979] */
    (xdc_Char)0x6e,  /* [980] */
    (xdc_Char)0x6e,  /* [981] */
    (xdc_Char)0x6f,  /* [982] */
    (xdc_Char)0x74,  /* [983] */
    (xdc_Char)0x20,  /* [984] */
    (xdc_Char)0x63,  /* [985] */
    (xdc_Char)0x61,  /* [986] */
    (xdc_Char)0x6c,  /* [987] */
    (xdc_Char)0x6c,  /* [988] */
    (xdc_Char)0x20,  /* [989] */
    (xdc_Char)0x45,  /* [990] */
    (xdc_Char)0x76,  /* [991] */
    (xdc_Char)0x65,  /* [992] */
    (xdc_Char)0x6e,  /* [993] */
    (xdc_Char)0x74,  /* [994] */
    (xdc_Char)0x5f,  /* [995] */
    (xdc_Char)0x70,  /* [996] */
    (xdc_Char)0x65,  /* [997] */
    (xdc_Char)0x6e,  /* [998] */
    (xdc_Char)0x64,  /* [999] */
    (xdc_Char)0x28,  /* [1000] */
    (xdc_Char)0x29,  /* [1001] */
    (xdc_Char)0x20,  /* [1002] */
    (xdc_Char)0x77,  /* [1003] */
    (xdc_Char)0x68,  /* [1004] */
    (xdc_Char)0x69,  /* [1005] */
    (xdc_Char)0x6c,  /* [1006] */
    (xdc_Char)0x65,  /* [1007] */
    (xdc_Char)0x20,  /* [1008] */
    (xdc_Char)0x74,  /* [1009] */
    (xdc_Char)0x68,  /* [1010] */
    (xdc_Char)0x65,  /* [1011] */
    (xdc_Char)0x20,  /* [1012] */
    (xdc_Char)0x54,  /* [1013] */
    (xdc_Char)0x61,  /* [1014] */
    (xdc_Char)0x73,  /* [1015] */
    (xdc_Char)0x6b,  /* [1016] */
    (xdc_Char)0x20,  /* [1017] */
    (xdc_Char)0x6f,  /* [1018] */
    (xdc_Char)0x72,  /* [1019] */
    (xdc_Char)0x20,  /* [1020] */
    (xdc_Char)0x53,  /* [1021] */
    (xdc_Char)0x77,  /* [1022] */
    (xdc_Char)0x69,  /* [1023] */
    (xdc_Char)0x20,  /* [1024] */
    (xdc_Char)0x73,  /* [1025] */
    (xdc_Char)0x63,  /* [1026] */
    (xdc_Char)0x68,  /* [1027] */
    (xdc_Char)0x65,  /* [1028] */
    (xdc_Char)0x64,  /* [1029] */
    (xdc_Char)0x75,  /* [1030] */
    (xdc_Char)0x6c,  /* [1031] */
    (xdc_Char)0x65,  /* [1032] */
    (xdc_Char)0x72,  /* [1033] */
    (xdc_Char)0x20,  /* [1034] */
    (xdc_Char)0x69,  /* [1035] */
    (xdc_Char)0x73,  /* [1036] */
    (xdc_Char)0x20,  /* [1037] */
    (xdc_Char)0x64,  /* [1038] */
    (xdc_Char)0x69,  /* [1039] */
    (xdc_Char)0x73,  /* [1040] */
    (xdc_Char)0x61,  /* [1041] */
    (xdc_Char)0x62,  /* [1042] */
    (xdc_Char)0x6c,  /* [1043] */
    (xdc_Char)0x65,  /* [1044] */
    (xdc_Char)0x64,  /* [1045] */
    (xdc_Char)0x2e,  /* [1046] */
    (xdc_Char)0x0,  /* [1047] */
    (xdc_Char)0x4d,  /* [1048] */
    (xdc_Char)0x61,  /* [1049] */
    (xdc_Char)0x69,  /* [1050] */
    (xdc_Char)0x6c,  /* [1051] */
    (xdc_Char)0x62,  /* [1052] */
    (xdc_Char)0x6f,  /* [1053] */
    (xdc_Char)0x78,  /* [1054] */
    (xdc_Char)0x5f,  /* [1055] */
    (xdc_Char)0x63,  /* [1056] */
    (xdc_Char)0x72,  /* [1057] */
    (xdc_Char)0x65,  /* [1058] */
    (xdc_Char)0x61,  /* [1059] */
    (xdc_Char)0x74,  /* [1060] */
    (xdc_Char)0x65,  /* [1061] */
    (xdc_Char)0x27,  /* [1062] */
    (xdc_Char)0x73,  /* [1063] */
    (xdc_Char)0x20,  /* [1064] */
    (xdc_Char)0x62,  /* [1065] */
    (xdc_Char)0x75,  /* [1066] */
    (xdc_Char)0x66,  /* [1067] */
    (xdc_Char)0x53,  /* [1068] */
    (xdc_Char)0x69,  /* [1069] */
    (xdc_Char)0x7a,  /* [1070] */
    (xdc_Char)0x65,  /* [1071] */
    (xdc_Char)0x20,  /* [1072] */
    (xdc_Char)0x70,  /* [1073] */
    (xdc_Char)0x61,  /* [1074] */
    (xdc_Char)0x72,  /* [1075] */
    (xdc_Char)0x61,  /* [1076] */
    (xdc_Char)0x6d,  /* [1077] */
    (xdc_Char)0x65,  /* [1078] */
    (xdc_Char)0x74,  /* [1079] */
    (xdc_Char)0x65,  /* [1080] */
    (xdc_Char)0x72,  /* [1081] */
    (xdc_Char)0x20,  /* [1082] */
    (xdc_Char)0x69,  /* [1083] */
    (xdc_Char)0x73,  /* [1084] */
    (xdc_Char)0x20,  /* [1085] */
    (xdc_Char)0x69,  /* [1086] */
    (xdc_Char)0x6e,  /* [1087] */
    (xdc_Char)0x76,  /* [1088] */
    (xdc_Char)0x61,  /* [1089] */
    (xdc_Char)0x6c,  /* [1090] */
    (xdc_Char)0x69,  /* [1091] */
    (xdc_Char)0x64,  /* [1092] */
    (xdc_Char)0x20,  /* [1093] */
    (xdc_Char)0x28,  /* [1094] */
    (xdc_Char)0x74,  /* [1095] */
    (xdc_Char)0x6f,  /* [1096] */
    (xdc_Char)0x6f,  /* [1097] */
    (xdc_Char)0x20,  /* [1098] */
    (xdc_Char)0x73,  /* [1099] */
    (xdc_Char)0x6d,  /* [1100] */
    (xdc_Char)0x61,  /* [1101] */
    (xdc_Char)0x6c,  /* [1102] */
    (xdc_Char)0x6c,  /* [1103] */
    (xdc_Char)0x29,  /* [1104] */
    (xdc_Char)0x0,  /* [1105] */
    (xdc_Char)0x41,  /* [1106] */
    (xdc_Char)0x5f,  /* [1107] */
    (xdc_Char)0x6e,  /* [1108] */
    (xdc_Char)0x6f,  /* [1109] */
    (xdc_Char)0x45,  /* [1110] */
    (xdc_Char)0x76,  /* [1111] */
    (xdc_Char)0x65,  /* [1112] */
    (xdc_Char)0x6e,  /* [1113] */
    (xdc_Char)0x74,  /* [1114] */
    (xdc_Char)0x73,  /* [1115] */
    (xdc_Char)0x3a,  /* [1116] */
    (xdc_Char)0x20,  /* [1117] */
    (xdc_Char)0x54,  /* [1118] */
    (xdc_Char)0x68,  /* [1119] */
    (xdc_Char)0x65,  /* [1120] */
    (xdc_Char)0x20,  /* [1121] */
    (xdc_Char)0x45,  /* [1122] */
    (xdc_Char)0x76,  /* [1123] */
    (xdc_Char)0x65,  /* [1124] */
    (xdc_Char)0x6e,  /* [1125] */
    (xdc_Char)0x74,  /* [1126] */
    (xdc_Char)0x2e,  /* [1127] */
    (xdc_Char)0x73,  /* [1128] */
    (xdc_Char)0x75,  /* [1129] */
    (xdc_Char)0x70,  /* [1130] */
    (xdc_Char)0x70,  /* [1131] */
    (xdc_Char)0x6f,  /* [1132] */
    (xdc_Char)0x72,  /* [1133] */
    (xdc_Char)0x74,  /* [1134] */
    (xdc_Char)0x73,  /* [1135] */
    (xdc_Char)0x45,  /* [1136] */
    (xdc_Char)0x76,  /* [1137] */
    (xdc_Char)0x65,  /* [1138] */
    (xdc_Char)0x6e,  /* [1139] */
    (xdc_Char)0x74,  /* [1140] */
    (xdc_Char)0x73,  /* [1141] */
    (xdc_Char)0x20,  /* [1142] */
    (xdc_Char)0x66,  /* [1143] */
    (xdc_Char)0x6c,  /* [1144] */
    (xdc_Char)0x61,  /* [1145] */
    (xdc_Char)0x67,  /* [1146] */
    (xdc_Char)0x20,  /* [1147] */
    (xdc_Char)0x69,  /* [1148] */
    (xdc_Char)0x73,  /* [1149] */
    (xdc_Char)0x20,  /* [1150] */
    (xdc_Char)0x64,  /* [1151] */
    (xdc_Char)0x69,  /* [1152] */
    (xdc_Char)0x73,  /* [1153] */
    (xdc_Char)0x61,  /* [1154] */
    (xdc_Char)0x62,  /* [1155] */
    (xdc_Char)0x6c,  /* [1156] */
    (xdc_Char)0x65,  /* [1157] */
    (xdc_Char)0x64,  /* [1158] */
    (xdc_Char)0x2e,  /* [1159] */
    (xdc_Char)0x0,  /* [1160] */
    (xdc_Char)0x41,  /* [1161] */
    (xdc_Char)0x5f,  /* [1162] */
    (xdc_Char)0x69,  /* [1163] */
    (xdc_Char)0x6e,  /* [1164] */
    (xdc_Char)0x76,  /* [1165] */
    (xdc_Char)0x54,  /* [1166] */
    (xdc_Char)0x69,  /* [1167] */
    (xdc_Char)0x6d,  /* [1168] */
    (xdc_Char)0x65,  /* [1169] */
    (xdc_Char)0x6f,  /* [1170] */
    (xdc_Char)0x75,  /* [1171] */
    (xdc_Char)0x74,  /* [1172] */
    (xdc_Char)0x3a,  /* [1173] */
    (xdc_Char)0x20,  /* [1174] */
    (xdc_Char)0x43,  /* [1175] */
    (xdc_Char)0x61,  /* [1176] */
    (xdc_Char)0x6e,  /* [1177] */
    (xdc_Char)0x27,  /* [1178] */
    (xdc_Char)0x74,  /* [1179] */
    (xdc_Char)0x20,  /* [1180] */
    (xdc_Char)0x75,  /* [1181] */
    (xdc_Char)0x73,  /* [1182] */
    (xdc_Char)0x65,  /* [1183] */
    (xdc_Char)0x20,  /* [1184] */
    (xdc_Char)0x42,  /* [1185] */
    (xdc_Char)0x49,  /* [1186] */
    (xdc_Char)0x4f,  /* [1187] */
    (xdc_Char)0x53,  /* [1188] */
    (xdc_Char)0x5f,  /* [1189] */
    (xdc_Char)0x45,  /* [1190] */
    (xdc_Char)0x56,  /* [1191] */
    (xdc_Char)0x45,  /* [1192] */
    (xdc_Char)0x4e,  /* [1193] */
    (xdc_Char)0x54,  /* [1194] */
    (xdc_Char)0x5f,  /* [1195] */
    (xdc_Char)0x41,  /* [1196] */
    (xdc_Char)0x43,  /* [1197] */
    (xdc_Char)0x51,  /* [1198] */
    (xdc_Char)0x55,  /* [1199] */
    (xdc_Char)0x49,  /* [1200] */
    (xdc_Char)0x52,  /* [1201] */
    (xdc_Char)0x45,  /* [1202] */
    (xdc_Char)0x44,  /* [1203] */
    (xdc_Char)0x20,  /* [1204] */
    (xdc_Char)0x77,  /* [1205] */
    (xdc_Char)0x69,  /* [1206] */
    (xdc_Char)0x74,  /* [1207] */
    (xdc_Char)0x68,  /* [1208] */
    (xdc_Char)0x20,  /* [1209] */
    (xdc_Char)0x74,  /* [1210] */
    (xdc_Char)0x68,  /* [1211] */
    (xdc_Char)0x69,  /* [1212] */
    (xdc_Char)0x73,  /* [1213] */
    (xdc_Char)0x20,  /* [1214] */
    (xdc_Char)0x53,  /* [1215] */
    (xdc_Char)0x65,  /* [1216] */
    (xdc_Char)0x6d,  /* [1217] */
    (xdc_Char)0x61,  /* [1218] */
    (xdc_Char)0x70,  /* [1219] */
    (xdc_Char)0x68,  /* [1220] */
    (xdc_Char)0x6f,  /* [1221] */
    (xdc_Char)0x72,  /* [1222] */
    (xdc_Char)0x65,  /* [1223] */
    (xdc_Char)0x2e,  /* [1224] */
    (xdc_Char)0x0,  /* [1225] */
    (xdc_Char)0x41,  /* [1226] */
    (xdc_Char)0x5f,  /* [1227] */
    (xdc_Char)0x6f,  /* [1228] */
    (xdc_Char)0x76,  /* [1229] */
    (xdc_Char)0x65,  /* [1230] */
    (xdc_Char)0x72,  /* [1231] */
    (xdc_Char)0x66,  /* [1232] */
    (xdc_Char)0x6c,  /* [1233] */
    (xdc_Char)0x6f,  /* [1234] */
    (xdc_Char)0x77,  /* [1235] */
    (xdc_Char)0x3a,  /* [1236] */
    (xdc_Char)0x20,  /* [1237] */
    (xdc_Char)0x43,  /* [1238] */
    (xdc_Char)0x6f,  /* [1239] */
    (xdc_Char)0x75,  /* [1240] */
    (xdc_Char)0x6e,  /* [1241] */
    (xdc_Char)0x74,  /* [1242] */
    (xdc_Char)0x20,  /* [1243] */
    (xdc_Char)0x68,  /* [1244] */
    (xdc_Char)0x61,  /* [1245] */
    (xdc_Char)0x73,  /* [1246] */
    (xdc_Char)0x20,  /* [1247] */
    (xdc_Char)0x65,  /* [1248] */
    (xdc_Char)0x78,  /* [1249] */
    (xdc_Char)0x63,  /* [1250] */
    (xdc_Char)0x65,  /* [1251] */
    (xdc_Char)0x65,  /* [1252] */
    (xdc_Char)0x64,  /* [1253] */
    (xdc_Char)0x65,  /* [1254] */
    (xdc_Char)0x64,  /* [1255] */
    (xdc_Char)0x20,  /* [1256] */
    (xdc_Char)0x36,  /* [1257] */
    (xdc_Char)0x35,  /* [1258] */
    (xdc_Char)0x35,  /* [1259] */
    (xdc_Char)0x33,  /* [1260] */
    (xdc_Char)0x35,  /* [1261] */
    (xdc_Char)0x20,  /* [1262] */
    (xdc_Char)0x61,  /* [1263] */
    (xdc_Char)0x6e,  /* [1264] */
    (xdc_Char)0x64,  /* [1265] */
    (xdc_Char)0x20,  /* [1266] */
    (xdc_Char)0x72,  /* [1267] */
    (xdc_Char)0x6f,  /* [1268] */
    (xdc_Char)0x6c,  /* [1269] */
    (xdc_Char)0x6c,  /* [1270] */
    (xdc_Char)0x65,  /* [1271] */
    (xdc_Char)0x64,  /* [1272] */
    (xdc_Char)0x20,  /* [1273] */
    (xdc_Char)0x6f,  /* [1274] */
    (xdc_Char)0x76,  /* [1275] */
    (xdc_Char)0x65,  /* [1276] */
    (xdc_Char)0x72,  /* [1277] */
    (xdc_Char)0x2e,  /* [1278] */
    (xdc_Char)0x0,  /* [1279] */
    (xdc_Char)0x41,  /* [1280] */
    (xdc_Char)0x5f,  /* [1281] */
    (xdc_Char)0x70,  /* [1282] */
    (xdc_Char)0x65,  /* [1283] */
    (xdc_Char)0x6e,  /* [1284] */
    (xdc_Char)0x64,  /* [1285] */
    (xdc_Char)0x54,  /* [1286] */
    (xdc_Char)0x61,  /* [1287] */
    (xdc_Char)0x73,  /* [1288] */
    (xdc_Char)0x6b,  /* [1289] */
    (xdc_Char)0x44,  /* [1290] */
    (xdc_Char)0x69,  /* [1291] */
    (xdc_Char)0x73,  /* [1292] */
    (xdc_Char)0x61,  /* [1293] */
    (xdc_Char)0x62,  /* [1294] */
    (xdc_Char)0x6c,  /* [1295] */
    (xdc_Char)0x65,  /* [1296] */
    (xdc_Char)0x64,  /* [1297] */
    (xdc_Char)0x3a,  /* [1298] */
    (xdc_Char)0x20,  /* [1299] */
    (xdc_Char)0x43,  /* [1300] */
    (xdc_Char)0x61,  /* [1301] */
    (xdc_Char)0x6e,  /* [1302] */
    (xdc_Char)0x6e,  /* [1303] */
    (xdc_Char)0x6f,  /* [1304] */
    (xdc_Char)0x74,  /* [1305] */
    (xdc_Char)0x20,  /* [1306] */
    (xdc_Char)0x63,  /* [1307] */
    (xdc_Char)0x61,  /* [1308] */
    (xdc_Char)0x6c,  /* [1309] */
    (xdc_Char)0x6c,  /* [1310] */
    (xdc_Char)0x20,  /* [1311] */
    (xdc_Char)0x53,  /* [1312] */
    (xdc_Char)0x65,  /* [1313] */
    (xdc_Char)0x6d,  /* [1314] */
    (xdc_Char)0x61,  /* [1315] */
    (xdc_Char)0x70,  /* [1316] */
    (xdc_Char)0x68,  /* [1317] */
    (xdc_Char)0x6f,  /* [1318] */
    (xdc_Char)0x72,  /* [1319] */
    (xdc_Char)0x65,  /* [1320] */
    (xdc_Char)0x5f,  /* [1321] */
    (xdc_Char)0x70,  /* [1322] */
    (xdc_Char)0x65,  /* [1323] */
    (xdc_Char)0x6e,  /* [1324] */
    (xdc_Char)0x64,  /* [1325] */
    (xdc_Char)0x28,  /* [1326] */
    (xdc_Char)0x29,  /* [1327] */
    (xdc_Char)0x20,  /* [1328] */
    (xdc_Char)0x77,  /* [1329] */
    (xdc_Char)0x68,  /* [1330] */
    (xdc_Char)0x69,  /* [1331] */
    (xdc_Char)0x6c,  /* [1332] */
    (xdc_Char)0x65,  /* [1333] */
    (xdc_Char)0x20,  /* [1334] */
    (xdc_Char)0x74,  /* [1335] */
    (xdc_Char)0x68,  /* [1336] */
    (xdc_Char)0x65,  /* [1337] */
    (xdc_Char)0x20,  /* [1338] */
    (xdc_Char)0x54,  /* [1339] */
    (xdc_Char)0x61,  /* [1340] */
    (xdc_Char)0x73,  /* [1341] */
    (xdc_Char)0x6b,  /* [1342] */
    (xdc_Char)0x20,  /* [1343] */
    (xdc_Char)0x6f,  /* [1344] */
    (xdc_Char)0x72,  /* [1345] */
    (xdc_Char)0x20,  /* [1346] */
    (xdc_Char)0x53,  /* [1347] */
    (xdc_Char)0x77,  /* [1348] */
    (xdc_Char)0x69,  /* [1349] */
    (xdc_Char)0x20,  /* [1350] */
    (xdc_Char)0x73,  /* [1351] */
    (xdc_Char)0x63,  /* [1352] */
    (xdc_Char)0x68,  /* [1353] */
    (xdc_Char)0x65,  /* [1354] */
    (xdc_Char)0x64,  /* [1355] */
    (xdc_Char)0x75,  /* [1356] */
    (xdc_Char)0x6c,  /* [1357] */
    (xdc_Char)0x65,  /* [1358] */
    (xdc_Char)0x72,  /* [1359] */
    (xdc_Char)0x20,  /* [1360] */
    (xdc_Char)0x69,  /* [1361] */
    (xdc_Char)0x73,  /* [1362] */
    (xdc_Char)0x20,  /* [1363] */
    (xdc_Char)0x64,  /* [1364] */
    (xdc_Char)0x69,  /* [1365] */
    (xdc_Char)0x73,  /* [1366] */
    (xdc_Char)0x61,  /* [1367] */
    (xdc_Char)0x62,  /* [1368] */
    (xdc_Char)0x6c,  /* [1369] */
    (xdc_Char)0x65,  /* [1370] */
    (xdc_Char)0x64,  /* [1371] */
    (xdc_Char)0x2e,  /* [1372] */
    (xdc_Char)0x0,  /* [1373] */
    (xdc_Char)0x41,  /* [1374] */
    (xdc_Char)0x5f,  /* [1375] */
    (xdc_Char)0x73,  /* [1376] */
    (xdc_Char)0x77,  /* [1377] */
    (xdc_Char)0x69,  /* [1378] */
    (xdc_Char)0x44,  /* [1379] */
    (xdc_Char)0x69,  /* [1380] */
    (xdc_Char)0x73,  /* [1381] */
    (xdc_Char)0x61,  /* [1382] */
    (xdc_Char)0x62,  /* [1383] */
    (xdc_Char)0x6c,  /* [1384] */
    (xdc_Char)0x65,  /* [1385] */
    (xdc_Char)0x64,  /* [1386] */
    (xdc_Char)0x3a,  /* [1387] */
    (xdc_Char)0x20,  /* [1388] */
    (xdc_Char)0x43,  /* [1389] */
    (xdc_Char)0x61,  /* [1390] */
    (xdc_Char)0x6e,  /* [1391] */
    (xdc_Char)0x6e,  /* [1392] */
    (xdc_Char)0x6f,  /* [1393] */
    (xdc_Char)0x74,  /* [1394] */
    (xdc_Char)0x20,  /* [1395] */
    (xdc_Char)0x63,  /* [1396] */
    (xdc_Char)0x72,  /* [1397] */
    (xdc_Char)0x65,  /* [1398] */
    (xdc_Char)0x61,  /* [1399] */
    (xdc_Char)0x74,  /* [1400] */
    (xdc_Char)0x65,  /* [1401] */
    (xdc_Char)0x20,  /* [1402] */
    (xdc_Char)0x61,  /* [1403] */
    (xdc_Char)0x20,  /* [1404] */
    (xdc_Char)0x53,  /* [1405] */
    (xdc_Char)0x77,  /* [1406] */
    (xdc_Char)0x69,  /* [1407] */
    (xdc_Char)0x20,  /* [1408] */
    (xdc_Char)0x77,  /* [1409] */
    (xdc_Char)0x68,  /* [1410] */
    (xdc_Char)0x65,  /* [1411] */
    (xdc_Char)0x6e,  /* [1412] */
    (xdc_Char)0x20,  /* [1413] */
    (xdc_Char)0x53,  /* [1414] */
    (xdc_Char)0x77,  /* [1415] */
    (xdc_Char)0x69,  /* [1416] */
    (xdc_Char)0x20,  /* [1417] */
    (xdc_Char)0x69,  /* [1418] */
    (xdc_Char)0x73,  /* [1419] */
    (xdc_Char)0x20,  /* [1420] */
    (xdc_Char)0x64,  /* [1421] */
    (xdc_Char)0x69,  /* [1422] */
    (xdc_Char)0x73,  /* [1423] */
    (xdc_Char)0x61,  /* [1424] */
    (xdc_Char)0x62,  /* [1425] */
    (xdc_Char)0x6c,  /* [1426] */
    (xdc_Char)0x65,  /* [1427] */
    (xdc_Char)0x64,  /* [1428] */
    (xdc_Char)0x2e,  /* [1429] */
    (xdc_Char)0x0,  /* [1430] */
    (xdc_Char)0x41,  /* [1431] */
    (xdc_Char)0x5f,  /* [1432] */
    (xdc_Char)0x62,  /* [1433] */
    (xdc_Char)0x61,  /* [1434] */
    (xdc_Char)0x64,  /* [1435] */
    (xdc_Char)0x50,  /* [1436] */
    (xdc_Char)0x72,  /* [1437] */
    (xdc_Char)0x69,  /* [1438] */
    (xdc_Char)0x6f,  /* [1439] */
    (xdc_Char)0x72,  /* [1440] */
    (xdc_Char)0x69,  /* [1441] */
    (xdc_Char)0x74,  /* [1442] */
    (xdc_Char)0x79,  /* [1443] */
    (xdc_Char)0x3a,  /* [1444] */
    (xdc_Char)0x20,  /* [1445] */
    (xdc_Char)0x41,  /* [1446] */
    (xdc_Char)0x6e,  /* [1447] */
    (xdc_Char)0x20,  /* [1448] */
    (xdc_Char)0x69,  /* [1449] */
    (xdc_Char)0x6e,  /* [1450] */
    (xdc_Char)0x76,  /* [1451] */
    (xdc_Char)0x61,  /* [1452] */
    (xdc_Char)0x6c,  /* [1453] */
    (xdc_Char)0x69,  /* [1454] */
    (xdc_Char)0x64,  /* [1455] */
    (xdc_Char)0x20,  /* [1456] */
    (xdc_Char)0x53,  /* [1457] */
    (xdc_Char)0x77,  /* [1458] */
    (xdc_Char)0x69,  /* [1459] */
    (xdc_Char)0x20,  /* [1460] */
    (xdc_Char)0x70,  /* [1461] */
    (xdc_Char)0x72,  /* [1462] */
    (xdc_Char)0x69,  /* [1463] */
    (xdc_Char)0x6f,  /* [1464] */
    (xdc_Char)0x72,  /* [1465] */
    (xdc_Char)0x69,  /* [1466] */
    (xdc_Char)0x74,  /* [1467] */
    (xdc_Char)0x79,  /* [1468] */
    (xdc_Char)0x20,  /* [1469] */
    (xdc_Char)0x77,  /* [1470] */
    (xdc_Char)0x61,  /* [1471] */
    (xdc_Char)0x73,  /* [1472] */
    (xdc_Char)0x20,  /* [1473] */
    (xdc_Char)0x75,  /* [1474] */
    (xdc_Char)0x73,  /* [1475] */
    (xdc_Char)0x65,  /* [1476] */
    (xdc_Char)0x64,  /* [1477] */
    (xdc_Char)0x2e,  /* [1478] */
    (xdc_Char)0x0,  /* [1479] */
    (xdc_Char)0x41,  /* [1480] */
    (xdc_Char)0x5f,  /* [1481] */
    (xdc_Char)0x62,  /* [1482] */
    (xdc_Char)0x61,  /* [1483] */
    (xdc_Char)0x64,  /* [1484] */
    (xdc_Char)0x54,  /* [1485] */
    (xdc_Char)0x68,  /* [1486] */
    (xdc_Char)0x72,  /* [1487] */
    (xdc_Char)0x65,  /* [1488] */
    (xdc_Char)0x61,  /* [1489] */
    (xdc_Char)0x64,  /* [1490] */
    (xdc_Char)0x54,  /* [1491] */
    (xdc_Char)0x79,  /* [1492] */
    (xdc_Char)0x70,  /* [1493] */
    (xdc_Char)0x65,  /* [1494] */
    (xdc_Char)0x3a,  /* [1495] */
    (xdc_Char)0x20,  /* [1496] */
    (xdc_Char)0x43,  /* [1497] */
    (xdc_Char)0x61,  /* [1498] */
    (xdc_Char)0x6e,  /* [1499] */
    (xdc_Char)0x6e,  /* [1500] */
    (xdc_Char)0x6f,  /* [1501] */
    (xdc_Char)0x74,  /* [1502] */
    (xdc_Char)0x20,  /* [1503] */
    (xdc_Char)0x63,  /* [1504] */
    (xdc_Char)0x72,  /* [1505] */
    (xdc_Char)0x65,  /* [1506] */
    (xdc_Char)0x61,  /* [1507] */
    (xdc_Char)0x74,  /* [1508] */
    (xdc_Char)0x65,  /* [1509] */
    (xdc_Char)0x2f,  /* [1510] */
    (xdc_Char)0x64,  /* [1511] */
    (xdc_Char)0x65,  /* [1512] */
    (xdc_Char)0x6c,  /* [1513] */
    (xdc_Char)0x65,  /* [1514] */
    (xdc_Char)0x74,  /* [1515] */
    (xdc_Char)0x65,  /* [1516] */
    (xdc_Char)0x20,  /* [1517] */
    (xdc_Char)0x61,  /* [1518] */
    (xdc_Char)0x20,  /* [1519] */
    (xdc_Char)0x74,  /* [1520] */
    (xdc_Char)0x61,  /* [1521] */
    (xdc_Char)0x73,  /* [1522] */
    (xdc_Char)0x6b,  /* [1523] */
    (xdc_Char)0x20,  /* [1524] */
    (xdc_Char)0x66,  /* [1525] */
    (xdc_Char)0x72,  /* [1526] */
    (xdc_Char)0x6f,  /* [1527] */
    (xdc_Char)0x6d,  /* [1528] */
    (xdc_Char)0x20,  /* [1529] */
    (xdc_Char)0x48,  /* [1530] */
    (xdc_Char)0x77,  /* [1531] */
    (xdc_Char)0x69,  /* [1532] */
    (xdc_Char)0x20,  /* [1533] */
    (xdc_Char)0x6f,  /* [1534] */
    (xdc_Char)0x72,  /* [1535] */
    (xdc_Char)0x20,  /* [1536] */
    (xdc_Char)0x53,  /* [1537] */
    (xdc_Char)0x77,  /* [1538] */
    (xdc_Char)0x69,  /* [1539] */
    (xdc_Char)0x20,  /* [1540] */
    (xdc_Char)0x74,  /* [1541] */
    (xdc_Char)0x68,  /* [1542] */
    (xdc_Char)0x72,  /* [1543] */
    (xdc_Char)0x65,  /* [1544] */
    (xdc_Char)0x61,  /* [1545] */
    (xdc_Char)0x64,  /* [1546] */
    (xdc_Char)0x2e,  /* [1547] */
    (xdc_Char)0x0,  /* [1548] */
    (xdc_Char)0x41,  /* [1549] */
    (xdc_Char)0x5f,  /* [1550] */
    (xdc_Char)0x62,  /* [1551] */
    (xdc_Char)0x61,  /* [1552] */
    (xdc_Char)0x64,  /* [1553] */
    (xdc_Char)0x54,  /* [1554] */
    (xdc_Char)0x61,  /* [1555] */
    (xdc_Char)0x73,  /* [1556] */
    (xdc_Char)0x6b,  /* [1557] */
    (xdc_Char)0x53,  /* [1558] */
    (xdc_Char)0x74,  /* [1559] */
    (xdc_Char)0x61,  /* [1560] */
    (xdc_Char)0x74,  /* [1561] */
    (xdc_Char)0x65,  /* [1562] */
    (xdc_Char)0x3a,  /* [1563] */
    (xdc_Char)0x20,  /* [1564] */
    (xdc_Char)0x43,  /* [1565] */
    (xdc_Char)0x61,  /* [1566] */
    (xdc_Char)0x6e,  /* [1567] */
    (xdc_Char)0x27,  /* [1568] */
    (xdc_Char)0x74,  /* [1569] */
    (xdc_Char)0x20,  /* [1570] */
    (xdc_Char)0x64,  /* [1571] */
    (xdc_Char)0x65,  /* [1572] */
    (xdc_Char)0x6c,  /* [1573] */
    (xdc_Char)0x65,  /* [1574] */
    (xdc_Char)0x74,  /* [1575] */
    (xdc_Char)0x65,  /* [1576] */
    (xdc_Char)0x20,  /* [1577] */
    (xdc_Char)0x61,  /* [1578] */
    (xdc_Char)0x20,  /* [1579] */
    (xdc_Char)0x74,  /* [1580] */
    (xdc_Char)0x61,  /* [1581] */
    (xdc_Char)0x73,  /* [1582] */
    (xdc_Char)0x6b,  /* [1583] */
    (xdc_Char)0x20,  /* [1584] */
    (xdc_Char)0x69,  /* [1585] */
    (xdc_Char)0x6e,  /* [1586] */
    (xdc_Char)0x20,  /* [1587] */
    (xdc_Char)0x52,  /* [1588] */
    (xdc_Char)0x55,  /* [1589] */
    (xdc_Char)0x4e,  /* [1590] */
    (xdc_Char)0x4e,  /* [1591] */
    (xdc_Char)0x49,  /* [1592] */
    (xdc_Char)0x4e,  /* [1593] */
    (xdc_Char)0x47,  /* [1594] */
    (xdc_Char)0x20,  /* [1595] */
    (xdc_Char)0x73,  /* [1596] */
    (xdc_Char)0x74,  /* [1597] */
    (xdc_Char)0x61,  /* [1598] */
    (xdc_Char)0x74,  /* [1599] */
    (xdc_Char)0x65,  /* [1600] */
    (xdc_Char)0x2e,  /* [1601] */
    (xdc_Char)0x0,  /* [1602] */
    (xdc_Char)0x41,  /* [1603] */
    (xdc_Char)0x5f,  /* [1604] */
    (xdc_Char)0x6e,  /* [1605] */
    (xdc_Char)0x6f,  /* [1606] */
    (xdc_Char)0x50,  /* [1607] */
    (xdc_Char)0x65,  /* [1608] */
    (xdc_Char)0x6e,  /* [1609] */
    (xdc_Char)0x64,  /* [1610] */
    (xdc_Char)0x45,  /* [1611] */
    (xdc_Char)0x6c,  /* [1612] */
    (xdc_Char)0x65,  /* [1613] */
    (xdc_Char)0x6d,  /* [1614] */
    (xdc_Char)0x3a,  /* [1615] */
    (xdc_Char)0x20,  /* [1616] */
    (xdc_Char)0x4e,  /* [1617] */
    (xdc_Char)0x6f,  /* [1618] */
    (xdc_Char)0x74,  /* [1619] */
    (xdc_Char)0x20,  /* [1620] */
    (xdc_Char)0x65,  /* [1621] */
    (xdc_Char)0x6e,  /* [1622] */
    (xdc_Char)0x6f,  /* [1623] */
    (xdc_Char)0x75,  /* [1624] */
    (xdc_Char)0x67,  /* [1625] */
    (xdc_Char)0x68,  /* [1626] */
    (xdc_Char)0x20,  /* [1627] */
    (xdc_Char)0x69,  /* [1628] */
    (xdc_Char)0x6e,  /* [1629] */
    (xdc_Char)0x66,  /* [1630] */
    (xdc_Char)0x6f,  /* [1631] */
    (xdc_Char)0x20,  /* [1632] */
    (xdc_Char)0x74,  /* [1633] */
    (xdc_Char)0x6f,  /* [1634] */
    (xdc_Char)0x20,  /* [1635] */
    (xdc_Char)0x64,  /* [1636] */
    (xdc_Char)0x65,  /* [1637] */
    (xdc_Char)0x6c,  /* [1638] */
    (xdc_Char)0x65,  /* [1639] */
    (xdc_Char)0x74,  /* [1640] */
    (xdc_Char)0x65,  /* [1641] */
    (xdc_Char)0x20,  /* [1642] */
    (xdc_Char)0x42,  /* [1643] */
    (xdc_Char)0x4c,  /* [1644] */
    (xdc_Char)0x4f,  /* [1645] */
    (xdc_Char)0x43,  /* [1646] */
    (xdc_Char)0x4b,  /* [1647] */
    (xdc_Char)0x45,  /* [1648] */
    (xdc_Char)0x44,  /* [1649] */
    (xdc_Char)0x20,  /* [1650] */
    (xdc_Char)0x74,  /* [1651] */
    (xdc_Char)0x61,  /* [1652] */
    (xdc_Char)0x73,  /* [1653] */
    (xdc_Char)0x6b,  /* [1654] */
    (xdc_Char)0x2e,  /* [1655] */
    (xdc_Char)0x0,  /* [1656] */
    (xdc_Char)0x41,  /* [1657] */
    (xdc_Char)0x5f,  /* [1658] */
    (xdc_Char)0x74,  /* [1659] */
    (xdc_Char)0x61,  /* [1660] */
    (xdc_Char)0x73,  /* [1661] */
    (xdc_Char)0x6b,  /* [1662] */
    (xdc_Char)0x44,  /* [1663] */
    (xdc_Char)0x69,  /* [1664] */
    (xdc_Char)0x73,  /* [1665] */
    (xdc_Char)0x61,  /* [1666] */
    (xdc_Char)0x62,  /* [1667] */
    (xdc_Char)0x6c,  /* [1668] */
    (xdc_Char)0x65,  /* [1669] */
    (xdc_Char)0x64,  /* [1670] */
    (xdc_Char)0x3a,  /* [1671] */
    (xdc_Char)0x20,  /* [1672] */
    (xdc_Char)0x43,  /* [1673] */
    (xdc_Char)0x61,  /* [1674] */
    (xdc_Char)0x6e,  /* [1675] */
    (xdc_Char)0x6e,  /* [1676] */
    (xdc_Char)0x6f,  /* [1677] */
    (xdc_Char)0x74,  /* [1678] */
    (xdc_Char)0x20,  /* [1679] */
    (xdc_Char)0x63,  /* [1680] */
    (xdc_Char)0x72,  /* [1681] */
    (xdc_Char)0x65,  /* [1682] */
    (xdc_Char)0x61,  /* [1683] */
    (xdc_Char)0x74,  /* [1684] */
    (xdc_Char)0x65,  /* [1685] */
    (xdc_Char)0x20,  /* [1686] */
    (xdc_Char)0x61,  /* [1687] */
    (xdc_Char)0x20,  /* [1688] */
    (xdc_Char)0x74,  /* [1689] */
    (xdc_Char)0x61,  /* [1690] */
    (xdc_Char)0x73,  /* [1691] */
    (xdc_Char)0x6b,  /* [1692] */
    (xdc_Char)0x20,  /* [1693] */
    (xdc_Char)0x77,  /* [1694] */
    (xdc_Char)0x68,  /* [1695] */
    (xdc_Char)0x65,  /* [1696] */
    (xdc_Char)0x6e,  /* [1697] */
    (xdc_Char)0x20,  /* [1698] */
    (xdc_Char)0x74,  /* [1699] */
    (xdc_Char)0x61,  /* [1700] */
    (xdc_Char)0x73,  /* [1701] */
    (xdc_Char)0x6b,  /* [1702] */
    (xdc_Char)0x69,  /* [1703] */
    (xdc_Char)0x6e,  /* [1704] */
    (xdc_Char)0x67,  /* [1705] */
    (xdc_Char)0x20,  /* [1706] */
    (xdc_Char)0x69,  /* [1707] */
    (xdc_Char)0x73,  /* [1708] */
    (xdc_Char)0x20,  /* [1709] */
    (xdc_Char)0x64,  /* [1710] */
    (xdc_Char)0x69,  /* [1711] */
    (xdc_Char)0x73,  /* [1712] */
    (xdc_Char)0x61,  /* [1713] */
    (xdc_Char)0x62,  /* [1714] */
    (xdc_Char)0x6c,  /* [1715] */
    (xdc_Char)0x65,  /* [1716] */
    (xdc_Char)0x64,  /* [1717] */
    (xdc_Char)0x2e,  /* [1718] */
    (xdc_Char)0x0,  /* [1719] */
    (xdc_Char)0x41,  /* [1720] */
    (xdc_Char)0x5f,  /* [1721] */
    (xdc_Char)0x62,  /* [1722] */
    (xdc_Char)0x61,  /* [1723] */
    (xdc_Char)0x64,  /* [1724] */
    (xdc_Char)0x50,  /* [1725] */
    (xdc_Char)0x72,  /* [1726] */
    (xdc_Char)0x69,  /* [1727] */
    (xdc_Char)0x6f,  /* [1728] */
    (xdc_Char)0x72,  /* [1729] */
    (xdc_Char)0x69,  /* [1730] */
    (xdc_Char)0x74,  /* [1731] */
    (xdc_Char)0x79,  /* [1732] */
    (xdc_Char)0x3a,  /* [1733] */
    (xdc_Char)0x20,  /* [1734] */
    (xdc_Char)0x41,  /* [1735] */
    (xdc_Char)0x6e,  /* [1736] */
    (xdc_Char)0x20,  /* [1737] */
    (xdc_Char)0x69,  /* [1738] */
    (xdc_Char)0x6e,  /* [1739] */
    (xdc_Char)0x76,  /* [1740] */
    (xdc_Char)0x61,  /* [1741] */
    (xdc_Char)0x6c,  /* [1742] */
    (xdc_Char)0x69,  /* [1743] */
    (xdc_Char)0x64,  /* [1744] */
    (xdc_Char)0x20,  /* [1745] */
    (xdc_Char)0x74,  /* [1746] */
    (xdc_Char)0x61,  /* [1747] */
    (xdc_Char)0x73,  /* [1748] */
    (xdc_Char)0x6b,  /* [1749] */
    (xdc_Char)0x20,  /* [1750] */
    (xdc_Char)0x70,  /* [1751] */
    (xdc_Char)0x72,  /* [1752] */
    (xdc_Char)0x69,  /* [1753] */
    (xdc_Char)0x6f,  /* [1754] */
    (xdc_Char)0x72,  /* [1755] */
    (xdc_Char)0x69,  /* [1756] */
    (xdc_Char)0x74,  /* [1757] */
    (xdc_Char)0x79,  /* [1758] */
    (xdc_Char)0x20,  /* [1759] */
    (xdc_Char)0x77,  /* [1760] */
    (xdc_Char)0x61,  /* [1761] */
    (xdc_Char)0x73,  /* [1762] */
    (xdc_Char)0x20,  /* [1763] */
    (xdc_Char)0x75,  /* [1764] */
    (xdc_Char)0x73,  /* [1765] */
    (xdc_Char)0x65,  /* [1766] */
    (xdc_Char)0x64,  /* [1767] */
    (xdc_Char)0x2e,  /* [1768] */
    (xdc_Char)0x0,  /* [1769] */
    (xdc_Char)0x41,  /* [1770] */
    (xdc_Char)0x5f,  /* [1771] */
    (xdc_Char)0x62,  /* [1772] */
    (xdc_Char)0x61,  /* [1773] */
    (xdc_Char)0x64,  /* [1774] */
    (xdc_Char)0x54,  /* [1775] */
    (xdc_Char)0x69,  /* [1776] */
    (xdc_Char)0x6d,  /* [1777] */
    (xdc_Char)0x65,  /* [1778] */
    (xdc_Char)0x6f,  /* [1779] */
    (xdc_Char)0x75,  /* [1780] */
    (xdc_Char)0x74,  /* [1781] */
    (xdc_Char)0x3a,  /* [1782] */
    (xdc_Char)0x20,  /* [1783] */
    (xdc_Char)0x43,  /* [1784] */
    (xdc_Char)0x61,  /* [1785] */
    (xdc_Char)0x6e,  /* [1786] */
    (xdc_Char)0x27,  /* [1787] */
    (xdc_Char)0x74,  /* [1788] */
    (xdc_Char)0x20,  /* [1789] */
    (xdc_Char)0x73,  /* [1790] */
    (xdc_Char)0x6c,  /* [1791] */
    (xdc_Char)0x65,  /* [1792] */
    (xdc_Char)0x65,  /* [1793] */
    (xdc_Char)0x70,  /* [1794] */
    (xdc_Char)0x20,  /* [1795] */
    (xdc_Char)0x46,  /* [1796] */
    (xdc_Char)0x4f,  /* [1797] */
    (xdc_Char)0x52,  /* [1798] */
    (xdc_Char)0x45,  /* [1799] */
    (xdc_Char)0x56,  /* [1800] */
    (xdc_Char)0x45,  /* [1801] */
    (xdc_Char)0x52,  /* [1802] */
    (xdc_Char)0x2e,  /* [1803] */
    (xdc_Char)0x0,  /* [1804] */
    (xdc_Char)0x41,  /* [1805] */
    (xdc_Char)0x5f,  /* [1806] */
    (xdc_Char)0x62,  /* [1807] */
    (xdc_Char)0x61,  /* [1808] */
    (xdc_Char)0x64,  /* [1809] */
    (xdc_Char)0x41,  /* [1810] */
    (xdc_Char)0x66,  /* [1811] */
    (xdc_Char)0x66,  /* [1812] */
    (xdc_Char)0x69,  /* [1813] */
    (xdc_Char)0x6e,  /* [1814] */
    (xdc_Char)0x69,  /* [1815] */
    (xdc_Char)0x74,  /* [1816] */
    (xdc_Char)0x79,  /* [1817] */
    (xdc_Char)0x3a,  /* [1818] */
    (xdc_Char)0x20,  /* [1819] */
    (xdc_Char)0x49,  /* [1820] */
    (xdc_Char)0x6e,  /* [1821] */
    (xdc_Char)0x76,  /* [1822] */
    (xdc_Char)0x61,  /* [1823] */
    (xdc_Char)0x6c,  /* [1824] */
    (xdc_Char)0x69,  /* [1825] */
    (xdc_Char)0x64,  /* [1826] */
    (xdc_Char)0x20,  /* [1827] */
    (xdc_Char)0x61,  /* [1828] */
    (xdc_Char)0x66,  /* [1829] */
    (xdc_Char)0x66,  /* [1830] */
    (xdc_Char)0x69,  /* [1831] */
    (xdc_Char)0x6e,  /* [1832] */
    (xdc_Char)0x69,  /* [1833] */
    (xdc_Char)0x74,  /* [1834] */
    (xdc_Char)0x79,  /* [1835] */
    (xdc_Char)0x2e,  /* [1836] */
    (xdc_Char)0x0,  /* [1837] */
    (xdc_Char)0x41,  /* [1838] */
    (xdc_Char)0x5f,  /* [1839] */
    (xdc_Char)0x73,  /* [1840] */
    (xdc_Char)0x6c,  /* [1841] */
    (xdc_Char)0x65,  /* [1842] */
    (xdc_Char)0x65,  /* [1843] */
    (xdc_Char)0x70,  /* [1844] */
    (xdc_Char)0x54,  /* [1845] */
    (xdc_Char)0x61,  /* [1846] */
    (xdc_Char)0x73,  /* [1847] */
    (xdc_Char)0x6b,  /* [1848] */
    (xdc_Char)0x44,  /* [1849] */
    (xdc_Char)0x69,  /* [1850] */
    (xdc_Char)0x73,  /* [1851] */
    (xdc_Char)0x61,  /* [1852] */
    (xdc_Char)0x62,  /* [1853] */
    (xdc_Char)0x6c,  /* [1854] */
    (xdc_Char)0x65,  /* [1855] */
    (xdc_Char)0x64,  /* [1856] */
    (xdc_Char)0x3a,  /* [1857] */
    (xdc_Char)0x20,  /* [1858] */
    (xdc_Char)0x43,  /* [1859] */
    (xdc_Char)0x61,  /* [1860] */
    (xdc_Char)0x6e,  /* [1861] */
    (xdc_Char)0x6e,  /* [1862] */
    (xdc_Char)0x6f,  /* [1863] */
    (xdc_Char)0x74,  /* [1864] */
    (xdc_Char)0x20,  /* [1865] */
    (xdc_Char)0x63,  /* [1866] */
    (xdc_Char)0x61,  /* [1867] */
    (xdc_Char)0x6c,  /* [1868] */
    (xdc_Char)0x6c,  /* [1869] */
    (xdc_Char)0x20,  /* [1870] */
    (xdc_Char)0x54,  /* [1871] */
    (xdc_Char)0x61,  /* [1872] */
    (xdc_Char)0x73,  /* [1873] */
    (xdc_Char)0x6b,  /* [1874] */
    (xdc_Char)0x5f,  /* [1875] */
    (xdc_Char)0x73,  /* [1876] */
    (xdc_Char)0x6c,  /* [1877] */
    (xdc_Char)0x65,  /* [1878] */
    (xdc_Char)0x65,  /* [1879] */
    (xdc_Char)0x70,  /* [1880] */
    (xdc_Char)0x28,  /* [1881] */
    (xdc_Char)0x29,  /* [1882] */
    (xdc_Char)0x20,  /* [1883] */
    (xdc_Char)0x77,  /* [1884] */
    (xdc_Char)0x68,  /* [1885] */
    (xdc_Char)0x69,  /* [1886] */
    (xdc_Char)0x6c,  /* [1887] */
    (xdc_Char)0x65,  /* [1888] */
    (xdc_Char)0x20,  /* [1889] */
    (xdc_Char)0x74,  /* [1890] */
    (xdc_Char)0x68,  /* [1891] */
    (xdc_Char)0x65,  /* [1892] */
    (xdc_Char)0x20,  /* [1893] */
    (xdc_Char)0x54,  /* [1894] */
    (xdc_Char)0x61,  /* [1895] */
    (xdc_Char)0x73,  /* [1896] */
    (xdc_Char)0x6b,  /* [1897] */
    (xdc_Char)0x20,  /* [1898] */
    (xdc_Char)0x73,  /* [1899] */
    (xdc_Char)0x63,  /* [1900] */
    (xdc_Char)0x68,  /* [1901] */
    (xdc_Char)0x65,  /* [1902] */
    (xdc_Char)0x64,  /* [1903] */
    (xdc_Char)0x75,  /* [1904] */
    (xdc_Char)0x6c,  /* [1905] */
    (xdc_Char)0x65,  /* [1906] */
    (xdc_Char)0x72,  /* [1907] */
    (xdc_Char)0x20,  /* [1908] */
    (xdc_Char)0x69,  /* [1909] */
    (xdc_Char)0x73,  /* [1910] */
    (xdc_Char)0x20,  /* [1911] */
    (xdc_Char)0x64,  /* [1912] */
    (xdc_Char)0x69,  /* [1913] */
    (xdc_Char)0x73,  /* [1914] */
    (xdc_Char)0x61,  /* [1915] */
    (xdc_Char)0x62,  /* [1916] */
    (xdc_Char)0x6c,  /* [1917] */
    (xdc_Char)0x65,  /* [1918] */
    (xdc_Char)0x64,  /* [1919] */
    (xdc_Char)0x2e,  /* [1920] */
    (xdc_Char)0x0,  /* [1921] */
    (xdc_Char)0x41,  /* [1922] */
    (xdc_Char)0x5f,  /* [1923] */
    (xdc_Char)0x69,  /* [1924] */
    (xdc_Char)0x6e,  /* [1925] */
    (xdc_Char)0x76,  /* [1926] */
    (xdc_Char)0x61,  /* [1927] */
    (xdc_Char)0x6c,  /* [1928] */
    (xdc_Char)0x69,  /* [1929] */
    (xdc_Char)0x64,  /* [1930] */
    (xdc_Char)0x43,  /* [1931] */
    (xdc_Char)0x6f,  /* [1932] */
    (xdc_Char)0x72,  /* [1933] */
    (xdc_Char)0x65,  /* [1934] */
    (xdc_Char)0x49,  /* [1935] */
    (xdc_Char)0x64,  /* [1936] */
    (xdc_Char)0x3a,  /* [1937] */
    (xdc_Char)0x20,  /* [1938] */
    (xdc_Char)0x43,  /* [1939] */
    (xdc_Char)0x61,  /* [1940] */
    (xdc_Char)0x6e,  /* [1941] */
    (xdc_Char)0x6e,  /* [1942] */
    (xdc_Char)0x6f,  /* [1943] */
    (xdc_Char)0x74,  /* [1944] */
    (xdc_Char)0x20,  /* [1945] */
    (xdc_Char)0x70,  /* [1946] */
    (xdc_Char)0x61,  /* [1947] */
    (xdc_Char)0x73,  /* [1948] */
    (xdc_Char)0x73,  /* [1949] */
    (xdc_Char)0x20,  /* [1950] */
    (xdc_Char)0x61,  /* [1951] */
    (xdc_Char)0x20,  /* [1952] */
    (xdc_Char)0x6e,  /* [1953] */
    (xdc_Char)0x6f,  /* [1954] */
    (xdc_Char)0x6e,  /* [1955] */
    (xdc_Char)0x2d,  /* [1956] */
    (xdc_Char)0x7a,  /* [1957] */
    (xdc_Char)0x65,  /* [1958] */
    (xdc_Char)0x72,  /* [1959] */
    (xdc_Char)0x6f,  /* [1960] */
    (xdc_Char)0x20,  /* [1961] */
    (xdc_Char)0x43,  /* [1962] */
    (xdc_Char)0x6f,  /* [1963] */
    (xdc_Char)0x72,  /* [1964] */
    (xdc_Char)0x65,  /* [1965] */
    (xdc_Char)0x49,  /* [1966] */
    (xdc_Char)0x64,  /* [1967] */
    (xdc_Char)0x20,  /* [1968] */
    (xdc_Char)0x69,  /* [1969] */
    (xdc_Char)0x6e,  /* [1970] */
    (xdc_Char)0x20,  /* [1971] */
    (xdc_Char)0x61,  /* [1972] */
    (xdc_Char)0x20,  /* [1973] */
    (xdc_Char)0x6e,  /* [1974] */
    (xdc_Char)0x6f,  /* [1975] */
    (xdc_Char)0x6e,  /* [1976] */
    (xdc_Char)0x2d,  /* [1977] */
    (xdc_Char)0x53,  /* [1978] */
    (xdc_Char)0x4d,  /* [1979] */
    (xdc_Char)0x50,  /* [1980] */
    (xdc_Char)0x20,  /* [1981] */
    (xdc_Char)0x61,  /* [1982] */
    (xdc_Char)0x70,  /* [1983] */
    (xdc_Char)0x70,  /* [1984] */
    (xdc_Char)0x6c,  /* [1985] */
    (xdc_Char)0x69,  /* [1986] */
    (xdc_Char)0x63,  /* [1987] */
    (xdc_Char)0x61,  /* [1988] */
    (xdc_Char)0x74,  /* [1989] */
    (xdc_Char)0x69,  /* [1990] */
    (xdc_Char)0x6f,  /* [1991] */
    (xdc_Char)0x6e,  /* [1992] */
    (xdc_Char)0x2e,  /* [1993] */
    (xdc_Char)0x0,  /* [1994] */
    (xdc_Char)0x62,  /* [1995] */
    (xdc_Char)0x75,  /* [1996] */
    (xdc_Char)0x66,  /* [1997] */
    (xdc_Char)0x20,  /* [1998] */
    (xdc_Char)0x70,  /* [1999] */
    (xdc_Char)0x61,  /* [2000] */
    (xdc_Char)0x72,  /* [2001] */
    (xdc_Char)0x61,  /* [2002] */
    (xdc_Char)0x6d,  /* [2003] */
    (xdc_Char)0x65,  /* [2004] */
    (xdc_Char)0x74,  /* [2005] */
    (xdc_Char)0x65,  /* [2006] */
    (xdc_Char)0x72,  /* [2007] */
    (xdc_Char)0x20,  /* [2008] */
    (xdc_Char)0x63,  /* [2009] */
    (xdc_Char)0x61,  /* [2010] */
    (xdc_Char)0x6e,  /* [2011] */
    (xdc_Char)0x6e,  /* [2012] */
    (xdc_Char)0x6f,  /* [2013] */
    (xdc_Char)0x74,  /* [2014] */
    (xdc_Char)0x20,  /* [2015] */
    (xdc_Char)0x62,  /* [2016] */
    (xdc_Char)0x65,  /* [2017] */
    (xdc_Char)0x20,  /* [2018] */
    (xdc_Char)0x6e,  /* [2019] */
    (xdc_Char)0x75,  /* [2020] */
    (xdc_Char)0x6c,  /* [2021] */
    (xdc_Char)0x6c,  /* [2022] */
    (xdc_Char)0x0,  /* [2023] */
    (xdc_Char)0x62,  /* [2024] */
    (xdc_Char)0x75,  /* [2025] */
    (xdc_Char)0x66,  /* [2026] */
    (xdc_Char)0x20,  /* [2027] */
    (xdc_Char)0x6e,  /* [2028] */
    (xdc_Char)0x6f,  /* [2029] */
    (xdc_Char)0x74,  /* [2030] */
    (xdc_Char)0x20,  /* [2031] */
    (xdc_Char)0x70,  /* [2032] */
    (xdc_Char)0x72,  /* [2033] */
    (xdc_Char)0x6f,  /* [2034] */
    (xdc_Char)0x70,  /* [2035] */
    (xdc_Char)0x65,  /* [2036] */
    (xdc_Char)0x72,  /* [2037] */
    (xdc_Char)0x6c,  /* [2038] */
    (xdc_Char)0x79,  /* [2039] */
    (xdc_Char)0x20,  /* [2040] */
    (xdc_Char)0x61,  /* [2041] */
    (xdc_Char)0x6c,  /* [2042] */
    (xdc_Char)0x69,  /* [2043] */
    (xdc_Char)0x67,  /* [2044] */
    (xdc_Char)0x6e,  /* [2045] */
    (xdc_Char)0x65,  /* [2046] */
    (xdc_Char)0x64,  /* [2047] */
    (xdc_Char)0x0,  /* [2048] */
    (xdc_Char)0x61,  /* [2049] */
    (xdc_Char)0x6c,  /* [2050] */
    (xdc_Char)0x69,  /* [2051] */
    (xdc_Char)0x67,  /* [2052] */
    (xdc_Char)0x6e,  /* [2053] */
    (xdc_Char)0x20,  /* [2054] */
    (xdc_Char)0x70,  /* [2055] */
    (xdc_Char)0x61,  /* [2056] */
    (xdc_Char)0x72,  /* [2057] */
    (xdc_Char)0x61,  /* [2058] */
    (xdc_Char)0x6d,  /* [2059] */
    (xdc_Char)0x65,  /* [2060] */
    (xdc_Char)0x74,  /* [2061] */
    (xdc_Char)0x65,  /* [2062] */
    (xdc_Char)0x72,  /* [2063] */
    (xdc_Char)0x20,  /* [2064] */
    (xdc_Char)0x6d,  /* [2065] */
    (xdc_Char)0x75,  /* [2066] */
    (xdc_Char)0x73,  /* [2067] */
    (xdc_Char)0x74,  /* [2068] */
    (xdc_Char)0x20,  /* [2069] */
    (xdc_Char)0x62,  /* [2070] */
    (xdc_Char)0x65,  /* [2071] */
    (xdc_Char)0x20,  /* [2072] */
    (xdc_Char)0x30,  /* [2073] */
    (xdc_Char)0x20,  /* [2074] */
    (xdc_Char)0x6f,  /* [2075] */
    (xdc_Char)0x72,  /* [2076] */
    (xdc_Char)0x20,  /* [2077] */
    (xdc_Char)0x61,  /* [2078] */
    (xdc_Char)0x20,  /* [2079] */
    (xdc_Char)0x70,  /* [2080] */
    (xdc_Char)0x6f,  /* [2081] */
    (xdc_Char)0x77,  /* [2082] */
    (xdc_Char)0x65,  /* [2083] */
    (xdc_Char)0x72,  /* [2084] */
    (xdc_Char)0x20,  /* [2085] */
    (xdc_Char)0x6f,  /* [2086] */
    (xdc_Char)0x66,  /* [2087] */
    (xdc_Char)0x20,  /* [2088] */
    (xdc_Char)0x32,  /* [2089] */
    (xdc_Char)0x20,  /* [2090] */
    (xdc_Char)0x3e,  /* [2091] */
    (xdc_Char)0x3d,  /* [2092] */
    (xdc_Char)0x20,  /* [2093] */
    (xdc_Char)0x74,  /* [2094] */
    (xdc_Char)0x68,  /* [2095] */
    (xdc_Char)0x65,  /* [2096] */
    (xdc_Char)0x20,  /* [2097] */
    (xdc_Char)0x76,  /* [2098] */
    (xdc_Char)0x61,  /* [2099] */
    (xdc_Char)0x6c,  /* [2100] */
    (xdc_Char)0x75,  /* [2101] */
    (xdc_Char)0x65,  /* [2102] */
    (xdc_Char)0x20,  /* [2103] */
    (xdc_Char)0x6f,  /* [2104] */
    (xdc_Char)0x66,  /* [2105] */
    (xdc_Char)0x20,  /* [2106] */
    (xdc_Char)0x4d,  /* [2107] */
    (xdc_Char)0x65,  /* [2108] */
    (xdc_Char)0x6d,  /* [2109] */
    (xdc_Char)0x6f,  /* [2110] */
    (xdc_Char)0x72,  /* [2111] */
    (xdc_Char)0x79,  /* [2112] */
    (xdc_Char)0x5f,  /* [2113] */
    (xdc_Char)0x67,  /* [2114] */
    (xdc_Char)0x65,  /* [2115] */
    (xdc_Char)0x74,  /* [2116] */
    (xdc_Char)0x4d,  /* [2117] */
    (xdc_Char)0x61,  /* [2118] */
    (xdc_Char)0x78,  /* [2119] */
    (xdc_Char)0x44,  /* [2120] */
    (xdc_Char)0x65,  /* [2121] */
    (xdc_Char)0x66,  /* [2122] */
    (xdc_Char)0x61,  /* [2123] */
    (xdc_Char)0x75,  /* [2124] */
    (xdc_Char)0x6c,  /* [2125] */
    (xdc_Char)0x74,  /* [2126] */
    (xdc_Char)0x54,  /* [2127] */
    (xdc_Char)0x79,  /* [2128] */
    (xdc_Char)0x70,  /* [2129] */
    (xdc_Char)0x65,  /* [2130] */
    (xdc_Char)0x41,  /* [2131] */
    (xdc_Char)0x6c,  /* [2132] */
    (xdc_Char)0x69,  /* [2133] */
    (xdc_Char)0x67,  /* [2134] */
    (xdc_Char)0x6e,  /* [2135] */
    (xdc_Char)0x28,  /* [2136] */
    (xdc_Char)0x29,  /* [2137] */
    (xdc_Char)0x0,  /* [2138] */
    (xdc_Char)0x61,  /* [2139] */
    (xdc_Char)0x6c,  /* [2140] */
    (xdc_Char)0x69,  /* [2141] */
    (xdc_Char)0x67,  /* [2142] */
    (xdc_Char)0x6e,  /* [2143] */
    (xdc_Char)0x20,  /* [2144] */
    (xdc_Char)0x70,  /* [2145] */
    (xdc_Char)0x61,  /* [2146] */
    (xdc_Char)0x72,  /* [2147] */
    (xdc_Char)0x61,  /* [2148] */
    (xdc_Char)0x6d,  /* [2149] */
    (xdc_Char)0x65,  /* [2150] */
    (xdc_Char)0x74,  /* [2151] */
    (xdc_Char)0x65,  /* [2152] */
    (xdc_Char)0x72,  /* [2153] */
    (xdc_Char)0x20,  /* [2154] */
    (xdc_Char)0x31,  /* [2155] */
    (xdc_Char)0x29,  /* [2156] */
    (xdc_Char)0x20,  /* [2157] */
    (xdc_Char)0x6d,  /* [2158] */
    (xdc_Char)0x75,  /* [2159] */
    (xdc_Char)0x73,  /* [2160] */
    (xdc_Char)0x74,  /* [2161] */
    (xdc_Char)0x20,  /* [2162] */
    (xdc_Char)0x62,  /* [2163] */
    (xdc_Char)0x65,  /* [2164] */
    (xdc_Char)0x20,  /* [2165] */
    (xdc_Char)0x30,  /* [2166] */
    (xdc_Char)0x20,  /* [2167] */
    (xdc_Char)0x6f,  /* [2168] */
    (xdc_Char)0x72,  /* [2169] */
    (xdc_Char)0x20,  /* [2170] */
    (xdc_Char)0x61,  /* [2171] */
    (xdc_Char)0x20,  /* [2172] */
    (xdc_Char)0x70,  /* [2173] */
    (xdc_Char)0x6f,  /* [2174] */
    (xdc_Char)0x77,  /* [2175] */
    (xdc_Char)0x65,  /* [2176] */
    (xdc_Char)0x72,  /* [2177] */
    (xdc_Char)0x20,  /* [2178] */
    (xdc_Char)0x6f,  /* [2179] */
    (xdc_Char)0x66,  /* [2180] */
    (xdc_Char)0x20,  /* [2181] */
    (xdc_Char)0x32,  /* [2182] */
    (xdc_Char)0x20,  /* [2183] */
    (xdc_Char)0x61,  /* [2184] */
    (xdc_Char)0x6e,  /* [2185] */
    (xdc_Char)0x64,  /* [2186] */
    (xdc_Char)0x20,  /* [2187] */
    (xdc_Char)0x32,  /* [2188] */
    (xdc_Char)0x29,  /* [2189] */
    (xdc_Char)0x20,  /* [2190] */
    (xdc_Char)0x6e,  /* [2191] */
    (xdc_Char)0x6f,  /* [2192] */
    (xdc_Char)0x74,  /* [2193] */
    (xdc_Char)0x20,  /* [2194] */
    (xdc_Char)0x67,  /* [2195] */
    (xdc_Char)0x72,  /* [2196] */
    (xdc_Char)0x65,  /* [2197] */
    (xdc_Char)0x61,  /* [2198] */
    (xdc_Char)0x74,  /* [2199] */
    (xdc_Char)0x65,  /* [2200] */
    (xdc_Char)0x72,  /* [2201] */
    (xdc_Char)0x20,  /* [2202] */
    (xdc_Char)0x74,  /* [2203] */
    (xdc_Char)0x68,  /* [2204] */
    (xdc_Char)0x61,  /* [2205] */
    (xdc_Char)0x6e,  /* [2206] */
    (xdc_Char)0x20,  /* [2207] */
    (xdc_Char)0x74,  /* [2208] */
    (xdc_Char)0x68,  /* [2209] */
    (xdc_Char)0x65,  /* [2210] */
    (xdc_Char)0x20,  /* [2211] */
    (xdc_Char)0x68,  /* [2212] */
    (xdc_Char)0x65,  /* [2213] */
    (xdc_Char)0x61,  /* [2214] */
    (xdc_Char)0x70,  /* [2215] */
    (xdc_Char)0x73,  /* [2216] */
    (xdc_Char)0x20,  /* [2217] */
    (xdc_Char)0x61,  /* [2218] */
    (xdc_Char)0x6c,  /* [2219] */
    (xdc_Char)0x69,  /* [2220] */
    (xdc_Char)0x67,  /* [2221] */
    (xdc_Char)0x6e,  /* [2222] */
    (xdc_Char)0x6d,  /* [2223] */
    (xdc_Char)0x65,  /* [2224] */
    (xdc_Char)0x6e,  /* [2225] */
    (xdc_Char)0x74,  /* [2226] */
    (xdc_Char)0x0,  /* [2227] */
    (xdc_Char)0x62,  /* [2228] */
    (xdc_Char)0x6c,  /* [2229] */
    (xdc_Char)0x6f,  /* [2230] */
    (xdc_Char)0x63,  /* [2231] */
    (xdc_Char)0x6b,  /* [2232] */
    (xdc_Char)0x53,  /* [2233] */
    (xdc_Char)0x69,  /* [2234] */
    (xdc_Char)0x7a,  /* [2235] */
    (xdc_Char)0x65,  /* [2236] */
    (xdc_Char)0x20,  /* [2237] */
    (xdc_Char)0x6d,  /* [2238] */
    (xdc_Char)0x75,  /* [2239] */
    (xdc_Char)0x73,  /* [2240] */
    (xdc_Char)0x74,  /* [2241] */
    (xdc_Char)0x20,  /* [2242] */
    (xdc_Char)0x62,  /* [2243] */
    (xdc_Char)0x65,  /* [2244] */
    (xdc_Char)0x20,  /* [2245] */
    (xdc_Char)0x6c,  /* [2246] */
    (xdc_Char)0x61,  /* [2247] */
    (xdc_Char)0x72,  /* [2248] */
    (xdc_Char)0x67,  /* [2249] */
    (xdc_Char)0x65,  /* [2250] */
    (xdc_Char)0x20,  /* [2251] */
    (xdc_Char)0x65,  /* [2252] */
    (xdc_Char)0x6e,  /* [2253] */
    (xdc_Char)0x6f,  /* [2254] */
    (xdc_Char)0x75,  /* [2255] */
    (xdc_Char)0x67,  /* [2256] */
    (xdc_Char)0x68,  /* [2257] */
    (xdc_Char)0x20,  /* [2258] */
    (xdc_Char)0x74,  /* [2259] */
    (xdc_Char)0x6f,  /* [2260] */
    (xdc_Char)0x20,  /* [2261] */
    (xdc_Char)0x68,  /* [2262] */
    (xdc_Char)0x6f,  /* [2263] */
    (xdc_Char)0x6c,  /* [2264] */
    (xdc_Char)0x64,  /* [2265] */
    (xdc_Char)0x20,  /* [2266] */
    (xdc_Char)0x61,  /* [2267] */
    (xdc_Char)0x74,  /* [2268] */
    (xdc_Char)0x6c,  /* [2269] */
    (xdc_Char)0x65,  /* [2270] */
    (xdc_Char)0x61,  /* [2271] */
    (xdc_Char)0x73,  /* [2272] */
    (xdc_Char)0x74,  /* [2273] */
    (xdc_Char)0x20,  /* [2274] */
    (xdc_Char)0x74,  /* [2275] */
    (xdc_Char)0x77,  /* [2276] */
    (xdc_Char)0x6f,  /* [2277] */
    (xdc_Char)0x20,  /* [2278] */
    (xdc_Char)0x70,  /* [2279] */
    (xdc_Char)0x6f,  /* [2280] */
    (xdc_Char)0x69,  /* [2281] */
    (xdc_Char)0x6e,  /* [2282] */
    (xdc_Char)0x74,  /* [2283] */
    (xdc_Char)0x65,  /* [2284] */
    (xdc_Char)0x72,  /* [2285] */
    (xdc_Char)0x73,  /* [2286] */
    (xdc_Char)0x0,  /* [2287] */
    (xdc_Char)0x6e,  /* [2288] */
    (xdc_Char)0x75,  /* [2289] */
    (xdc_Char)0x6d,  /* [2290] */
    (xdc_Char)0x42,  /* [2291] */
    (xdc_Char)0x6c,  /* [2292] */
    (xdc_Char)0x6f,  /* [2293] */
    (xdc_Char)0x63,  /* [2294] */
    (xdc_Char)0x6b,  /* [2295] */
    (xdc_Char)0x73,  /* [2296] */
    (xdc_Char)0x20,  /* [2297] */
    (xdc_Char)0x63,  /* [2298] */
    (xdc_Char)0x61,  /* [2299] */
    (xdc_Char)0x6e,  /* [2300] */
    (xdc_Char)0x6e,  /* [2301] */
    (xdc_Char)0x6f,  /* [2302] */
    (xdc_Char)0x74,  /* [2303] */
    (xdc_Char)0x20,  /* [2304] */
    (xdc_Char)0x62,  /* [2305] */
    (xdc_Char)0x65,  /* [2306] */
    (xdc_Char)0x20,  /* [2307] */
    (xdc_Char)0x7a,  /* [2308] */
    (xdc_Char)0x65,  /* [2309] */
    (xdc_Char)0x72,  /* [2310] */
    (xdc_Char)0x6f,  /* [2311] */
    (xdc_Char)0x0,  /* [2312] */
    (xdc_Char)0x62,  /* [2313] */
    (xdc_Char)0x75,  /* [2314] */
    (xdc_Char)0x66,  /* [2315] */
    (xdc_Char)0x53,  /* [2316] */
    (xdc_Char)0x69,  /* [2317] */
    (xdc_Char)0x7a,  /* [2318] */
    (xdc_Char)0x65,  /* [2319] */
    (xdc_Char)0x20,  /* [2320] */
    (xdc_Char)0x63,  /* [2321] */
    (xdc_Char)0x61,  /* [2322] */
    (xdc_Char)0x6e,  /* [2323] */
    (xdc_Char)0x6e,  /* [2324] */
    (xdc_Char)0x6f,  /* [2325] */
    (xdc_Char)0x74,  /* [2326] */
    (xdc_Char)0x20,  /* [2327] */
    (xdc_Char)0x62,  /* [2328] */
    (xdc_Char)0x65,  /* [2329] */
    (xdc_Char)0x20,  /* [2330] */
    (xdc_Char)0x7a,  /* [2331] */
    (xdc_Char)0x65,  /* [2332] */
    (xdc_Char)0x72,  /* [2333] */
    (xdc_Char)0x6f,  /* [2334] */
    (xdc_Char)0x0,  /* [2335] */
    (xdc_Char)0x48,  /* [2336] */
    (xdc_Char)0x65,  /* [2337] */
    (xdc_Char)0x61,  /* [2338] */
    (xdc_Char)0x70,  /* [2339] */
    (xdc_Char)0x42,  /* [2340] */
    (xdc_Char)0x75,  /* [2341] */
    (xdc_Char)0x66,  /* [2342] */
    (xdc_Char)0x5f,  /* [2343] */
    (xdc_Char)0x63,  /* [2344] */
    (xdc_Char)0x72,  /* [2345] */
    (xdc_Char)0x65,  /* [2346] */
    (xdc_Char)0x61,  /* [2347] */
    (xdc_Char)0x74,  /* [2348] */
    (xdc_Char)0x65,  /* [2349] */
    (xdc_Char)0x27,  /* [2350] */
    (xdc_Char)0x73,  /* [2351] */
    (xdc_Char)0x20,  /* [2352] */
    (xdc_Char)0x62,  /* [2353] */
    (xdc_Char)0x75,  /* [2354] */
    (xdc_Char)0x66,  /* [2355] */
    (xdc_Char)0x53,  /* [2356] */
    (xdc_Char)0x69,  /* [2357] */
    (xdc_Char)0x7a,  /* [2358] */
    (xdc_Char)0x65,  /* [2359] */
    (xdc_Char)0x20,  /* [2360] */
    (xdc_Char)0x70,  /* [2361] */
    (xdc_Char)0x61,  /* [2362] */
    (xdc_Char)0x72,  /* [2363] */
    (xdc_Char)0x61,  /* [2364] */
    (xdc_Char)0x6d,  /* [2365] */
    (xdc_Char)0x65,  /* [2366] */
    (xdc_Char)0x74,  /* [2367] */
    (xdc_Char)0x65,  /* [2368] */
    (xdc_Char)0x72,  /* [2369] */
    (xdc_Char)0x20,  /* [2370] */
    (xdc_Char)0x69,  /* [2371] */
    (xdc_Char)0x73,  /* [2372] */
    (xdc_Char)0x20,  /* [2373] */
    (xdc_Char)0x69,  /* [2374] */
    (xdc_Char)0x6e,  /* [2375] */
    (xdc_Char)0x76,  /* [2376] */
    (xdc_Char)0x61,  /* [2377] */
    (xdc_Char)0x6c,  /* [2378] */
    (xdc_Char)0x69,  /* [2379] */
    (xdc_Char)0x64,  /* [2380] */
    (xdc_Char)0x20,  /* [2381] */
    (xdc_Char)0x28,  /* [2382] */
    (xdc_Char)0x74,  /* [2383] */
    (xdc_Char)0x6f,  /* [2384] */
    (xdc_Char)0x6f,  /* [2385] */
    (xdc_Char)0x20,  /* [2386] */
    (xdc_Char)0x73,  /* [2387] */
    (xdc_Char)0x6d,  /* [2388] */
    (xdc_Char)0x61,  /* [2389] */
    (xdc_Char)0x6c,  /* [2390] */
    (xdc_Char)0x6c,  /* [2391] */
    (xdc_Char)0x29,  /* [2392] */
    (xdc_Char)0x0,  /* [2393] */
    (xdc_Char)0x43,  /* [2394] */
    (xdc_Char)0x61,  /* [2395] */
    (xdc_Char)0x6e,  /* [2396] */
    (xdc_Char)0x6e,  /* [2397] */
    (xdc_Char)0x6f,  /* [2398] */
    (xdc_Char)0x74,  /* [2399] */
    (xdc_Char)0x20,  /* [2400] */
    (xdc_Char)0x63,  /* [2401] */
    (xdc_Char)0x61,  /* [2402] */
    (xdc_Char)0x6c,  /* [2403] */
    (xdc_Char)0x6c,  /* [2404] */
    (xdc_Char)0x20,  /* [2405] */
    (xdc_Char)0x48,  /* [2406] */
    (xdc_Char)0x65,  /* [2407] */
    (xdc_Char)0x61,  /* [2408] */
    (xdc_Char)0x70,  /* [2409] */
    (xdc_Char)0x42,  /* [2410] */
    (xdc_Char)0x75,  /* [2411] */
    (xdc_Char)0x66,  /* [2412] */
    (xdc_Char)0x5f,  /* [2413] */
    (xdc_Char)0x66,  /* [2414] */
    (xdc_Char)0x72,  /* [2415] */
    (xdc_Char)0x65,  /* [2416] */
    (xdc_Char)0x65,  /* [2417] */
    (xdc_Char)0x20,  /* [2418] */
    (xdc_Char)0x77,  /* [2419] */
    (xdc_Char)0x68,  /* [2420] */
    (xdc_Char)0x65,  /* [2421] */
    (xdc_Char)0x6e,  /* [2422] */
    (xdc_Char)0x20,  /* [2423] */
    (xdc_Char)0x6e,  /* [2424] */
    (xdc_Char)0x6f,  /* [2425] */
    (xdc_Char)0x20,  /* [2426] */
    (xdc_Char)0x62,  /* [2427] */
    (xdc_Char)0x6c,  /* [2428] */
    (xdc_Char)0x6f,  /* [2429] */
    (xdc_Char)0x63,  /* [2430] */
    (xdc_Char)0x6b,  /* [2431] */
    (xdc_Char)0x73,  /* [2432] */
    (xdc_Char)0x20,  /* [2433] */
    (xdc_Char)0x68,  /* [2434] */
    (xdc_Char)0x61,  /* [2435] */
    (xdc_Char)0x76,  /* [2436] */
    (xdc_Char)0x65,  /* [2437] */
    (xdc_Char)0x20,  /* [2438] */
    (xdc_Char)0x62,  /* [2439] */
    (xdc_Char)0x65,  /* [2440] */
    (xdc_Char)0x65,  /* [2441] */
    (xdc_Char)0x6e,  /* [2442] */
    (xdc_Char)0x20,  /* [2443] */
    (xdc_Char)0x61,  /* [2444] */
    (xdc_Char)0x6c,  /* [2445] */
    (xdc_Char)0x6c,  /* [2446] */
    (xdc_Char)0x6f,  /* [2447] */
    (xdc_Char)0x63,  /* [2448] */
    (xdc_Char)0x61,  /* [2449] */
    (xdc_Char)0x74,  /* [2450] */
    (xdc_Char)0x65,  /* [2451] */
    (xdc_Char)0x64,  /* [2452] */
    (xdc_Char)0x0,  /* [2453] */
    (xdc_Char)0x41,  /* [2454] */
    (xdc_Char)0x5f,  /* [2455] */
    (xdc_Char)0x69,  /* [2456] */
    (xdc_Char)0x6e,  /* [2457] */
    (xdc_Char)0x76,  /* [2458] */
    (xdc_Char)0x61,  /* [2459] */
    (xdc_Char)0x6c,  /* [2460] */
    (xdc_Char)0x69,  /* [2461] */
    (xdc_Char)0x64,  /* [2462] */
    (xdc_Char)0x46,  /* [2463] */
    (xdc_Char)0x72,  /* [2464] */
    (xdc_Char)0x65,  /* [2465] */
    (xdc_Char)0x65,  /* [2466] */
    (xdc_Char)0x3a,  /* [2467] */
    (xdc_Char)0x20,  /* [2468] */
    (xdc_Char)0x49,  /* [2469] */
    (xdc_Char)0x6e,  /* [2470] */
    (xdc_Char)0x76,  /* [2471] */
    (xdc_Char)0x61,  /* [2472] */
    (xdc_Char)0x6c,  /* [2473] */
    (xdc_Char)0x69,  /* [2474] */
    (xdc_Char)0x64,  /* [2475] */
    (xdc_Char)0x20,  /* [2476] */
    (xdc_Char)0x66,  /* [2477] */
    (xdc_Char)0x72,  /* [2478] */
    (xdc_Char)0x65,  /* [2479] */
    (xdc_Char)0x65,  /* [2480] */
    (xdc_Char)0x0,  /* [2481] */
    (xdc_Char)0x41,  /* [2482] */
    (xdc_Char)0x5f,  /* [2483] */
    (xdc_Char)0x7a,  /* [2484] */
    (xdc_Char)0x65,  /* [2485] */
    (xdc_Char)0x72,  /* [2486] */
    (xdc_Char)0x6f,  /* [2487] */
    (xdc_Char)0x42,  /* [2488] */
    (xdc_Char)0x6c,  /* [2489] */
    (xdc_Char)0x6f,  /* [2490] */
    (xdc_Char)0x63,  /* [2491] */
    (xdc_Char)0x6b,  /* [2492] */
    (xdc_Char)0x3a,  /* [2493] */
    (xdc_Char)0x20,  /* [2494] */
    (xdc_Char)0x43,  /* [2495] */
    (xdc_Char)0x61,  /* [2496] */
    (xdc_Char)0x6e,  /* [2497] */
    (xdc_Char)0x6e,  /* [2498] */
    (xdc_Char)0x6f,  /* [2499] */
    (xdc_Char)0x74,  /* [2500] */
    (xdc_Char)0x20,  /* [2501] */
    (xdc_Char)0x61,  /* [2502] */
    (xdc_Char)0x6c,  /* [2503] */
    (xdc_Char)0x6c,  /* [2504] */
    (xdc_Char)0x6f,  /* [2505] */
    (xdc_Char)0x63,  /* [2506] */
    (xdc_Char)0x61,  /* [2507] */
    (xdc_Char)0x74,  /* [2508] */
    (xdc_Char)0x65,  /* [2509] */
    (xdc_Char)0x20,  /* [2510] */
    (xdc_Char)0x73,  /* [2511] */
    (xdc_Char)0x69,  /* [2512] */
    (xdc_Char)0x7a,  /* [2513] */
    (xdc_Char)0x65,  /* [2514] */
    (xdc_Char)0x20,  /* [2515] */
    (xdc_Char)0x30,  /* [2516] */
    (xdc_Char)0x0,  /* [2517] */
    (xdc_Char)0x41,  /* [2518] */
    (xdc_Char)0x5f,  /* [2519] */
    (xdc_Char)0x68,  /* [2520] */
    (xdc_Char)0x65,  /* [2521] */
    (xdc_Char)0x61,  /* [2522] */
    (xdc_Char)0x70,  /* [2523] */
    (xdc_Char)0x53,  /* [2524] */
    (xdc_Char)0x69,  /* [2525] */
    (xdc_Char)0x7a,  /* [2526] */
    (xdc_Char)0x65,  /* [2527] */
    (xdc_Char)0x3a,  /* [2528] */
    (xdc_Char)0x20,  /* [2529] */
    (xdc_Char)0x52,  /* [2530] */
    (xdc_Char)0x65,  /* [2531] */
    (xdc_Char)0x71,  /* [2532] */
    (xdc_Char)0x75,  /* [2533] */
    (xdc_Char)0x65,  /* [2534] */
    (xdc_Char)0x73,  /* [2535] */
    (xdc_Char)0x74,  /* [2536] */
    (xdc_Char)0x65,  /* [2537] */
    (xdc_Char)0x64,  /* [2538] */
    (xdc_Char)0x20,  /* [2539] */
    (xdc_Char)0x68,  /* [2540] */
    (xdc_Char)0x65,  /* [2541] */
    (xdc_Char)0x61,  /* [2542] */
    (xdc_Char)0x70,  /* [2543] */
    (xdc_Char)0x20,  /* [2544] */
    (xdc_Char)0x73,  /* [2545] */
    (xdc_Char)0x69,  /* [2546] */
    (xdc_Char)0x7a,  /* [2547] */
    (xdc_Char)0x65,  /* [2548] */
    (xdc_Char)0x20,  /* [2549] */
    (xdc_Char)0x69,  /* [2550] */
    (xdc_Char)0x73,  /* [2551] */
    (xdc_Char)0x20,  /* [2552] */
    (xdc_Char)0x74,  /* [2553] */
    (xdc_Char)0x6f,  /* [2554] */
    (xdc_Char)0x6f,  /* [2555] */
    (xdc_Char)0x20,  /* [2556] */
    (xdc_Char)0x73,  /* [2557] */
    (xdc_Char)0x6d,  /* [2558] */
    (xdc_Char)0x61,  /* [2559] */
    (xdc_Char)0x6c,  /* [2560] */
    (xdc_Char)0x6c,  /* [2561] */
    (xdc_Char)0x0,  /* [2562] */
    (xdc_Char)0x41,  /* [2563] */
    (xdc_Char)0x5f,  /* [2564] */
    (xdc_Char)0x61,  /* [2565] */
    (xdc_Char)0x6c,  /* [2566] */
    (xdc_Char)0x69,  /* [2567] */
    (xdc_Char)0x67,  /* [2568] */
    (xdc_Char)0x6e,  /* [2569] */
    (xdc_Char)0x3a,  /* [2570] */
    (xdc_Char)0x20,  /* [2571] */
    (xdc_Char)0x52,  /* [2572] */
    (xdc_Char)0x65,  /* [2573] */
    (xdc_Char)0x71,  /* [2574] */
    (xdc_Char)0x75,  /* [2575] */
    (xdc_Char)0x65,  /* [2576] */
    (xdc_Char)0x73,  /* [2577] */
    (xdc_Char)0x74,  /* [2578] */
    (xdc_Char)0x65,  /* [2579] */
    (xdc_Char)0x64,  /* [2580] */
    (xdc_Char)0x20,  /* [2581] */
    (xdc_Char)0x61,  /* [2582] */
    (xdc_Char)0x6c,  /* [2583] */
    (xdc_Char)0x69,  /* [2584] */
    (xdc_Char)0x67,  /* [2585] */
    (xdc_Char)0x6e,  /* [2586] */
    (xdc_Char)0x20,  /* [2587] */
    (xdc_Char)0x69,  /* [2588] */
    (xdc_Char)0x73,  /* [2589] */
    (xdc_Char)0x20,  /* [2590] */
    (xdc_Char)0x6e,  /* [2591] */
    (xdc_Char)0x6f,  /* [2592] */
    (xdc_Char)0x74,  /* [2593] */
    (xdc_Char)0x20,  /* [2594] */
    (xdc_Char)0x61,  /* [2595] */
    (xdc_Char)0x20,  /* [2596] */
    (xdc_Char)0x70,  /* [2597] */
    (xdc_Char)0x6f,  /* [2598] */
    (xdc_Char)0x77,  /* [2599] */
    (xdc_Char)0x65,  /* [2600] */
    (xdc_Char)0x72,  /* [2601] */
    (xdc_Char)0x20,  /* [2602] */
    (xdc_Char)0x6f,  /* [2603] */
    (xdc_Char)0x66,  /* [2604] */
    (xdc_Char)0x20,  /* [2605] */
    (xdc_Char)0x32,  /* [2606] */
    (xdc_Char)0x0,  /* [2607] */
    (xdc_Char)0x49,  /* [2608] */
    (xdc_Char)0x6e,  /* [2609] */
    (xdc_Char)0x76,  /* [2610] */
    (xdc_Char)0x61,  /* [2611] */
    (xdc_Char)0x6c,  /* [2612] */
    (xdc_Char)0x69,  /* [2613] */
    (xdc_Char)0x64,  /* [2614] */
    (xdc_Char)0x20,  /* [2615] */
    (xdc_Char)0x62,  /* [2616] */
    (xdc_Char)0x6c,  /* [2617] */
    (xdc_Char)0x6f,  /* [2618] */
    (xdc_Char)0x63,  /* [2619] */
    (xdc_Char)0x6b,  /* [2620] */
    (xdc_Char)0x20,  /* [2621] */
    (xdc_Char)0x61,  /* [2622] */
    (xdc_Char)0x64,  /* [2623] */
    (xdc_Char)0x64,  /* [2624] */
    (xdc_Char)0x72,  /* [2625] */
    (xdc_Char)0x65,  /* [2626] */
    (xdc_Char)0x73,  /* [2627] */
    (xdc_Char)0x73,  /* [2628] */
    (xdc_Char)0x20,  /* [2629] */
    (xdc_Char)0x6f,  /* [2630] */
    (xdc_Char)0x6e,  /* [2631] */
    (xdc_Char)0x20,  /* [2632] */
    (xdc_Char)0x74,  /* [2633] */
    (xdc_Char)0x68,  /* [2634] */
    (xdc_Char)0x65,  /* [2635] */
    (xdc_Char)0x20,  /* [2636] */
    (xdc_Char)0x66,  /* [2637] */
    (xdc_Char)0x72,  /* [2638] */
    (xdc_Char)0x65,  /* [2639] */
    (xdc_Char)0x65,  /* [2640] */
    (xdc_Char)0x2e,  /* [2641] */
    (xdc_Char)0x20,  /* [2642] */
    (xdc_Char)0x46,  /* [2643] */
    (xdc_Char)0x61,  /* [2644] */
    (xdc_Char)0x69,  /* [2645] */
    (xdc_Char)0x6c,  /* [2646] */
    (xdc_Char)0x65,  /* [2647] */
    (xdc_Char)0x64,  /* [2648] */
    (xdc_Char)0x20,  /* [2649] */
    (xdc_Char)0x74,  /* [2650] */
    (xdc_Char)0x6f,  /* [2651] */
    (xdc_Char)0x20,  /* [2652] */
    (xdc_Char)0x66,  /* [2653] */
    (xdc_Char)0x72,  /* [2654] */
    (xdc_Char)0x65,  /* [2655] */
    (xdc_Char)0x65,  /* [2656] */
    (xdc_Char)0x20,  /* [2657] */
    (xdc_Char)0x62,  /* [2658] */
    (xdc_Char)0x6c,  /* [2659] */
    (xdc_Char)0x6f,  /* [2660] */
    (xdc_Char)0x63,  /* [2661] */
    (xdc_Char)0x6b,  /* [2662] */
    (xdc_Char)0x20,  /* [2663] */
    (xdc_Char)0x62,  /* [2664] */
    (xdc_Char)0x61,  /* [2665] */
    (xdc_Char)0x63,  /* [2666] */
    (xdc_Char)0x6b,  /* [2667] */
    (xdc_Char)0x20,  /* [2668] */
    (xdc_Char)0x74,  /* [2669] */
    (xdc_Char)0x6f,  /* [2670] */
    (xdc_Char)0x20,  /* [2671] */
    (xdc_Char)0x68,  /* [2672] */
    (xdc_Char)0x65,  /* [2673] */
    (xdc_Char)0x61,  /* [2674] */
    (xdc_Char)0x70,  /* [2675] */
    (xdc_Char)0x2e,  /* [2676] */
    (xdc_Char)0x0,  /* [2677] */
    (xdc_Char)0x41,  /* [2678] */
    (xdc_Char)0x5f,  /* [2679] */
    (xdc_Char)0x64,  /* [2680] */
    (xdc_Char)0x6f,  /* [2681] */
    (xdc_Char)0x75,  /* [2682] */
    (xdc_Char)0x62,  /* [2683] */
    (xdc_Char)0x6c,  /* [2684] */
    (xdc_Char)0x65,  /* [2685] */
    (xdc_Char)0x46,  /* [2686] */
    (xdc_Char)0x72,  /* [2687] */
    (xdc_Char)0x65,  /* [2688] */
    (xdc_Char)0x65,  /* [2689] */
    (xdc_Char)0x3a,  /* [2690] */
    (xdc_Char)0x20,  /* [2691] */
    (xdc_Char)0x42,  /* [2692] */
    (xdc_Char)0x75,  /* [2693] */
    (xdc_Char)0x66,  /* [2694] */
    (xdc_Char)0x66,  /* [2695] */
    (xdc_Char)0x65,  /* [2696] */
    (xdc_Char)0x72,  /* [2697] */
    (xdc_Char)0x20,  /* [2698] */
    (xdc_Char)0x61,  /* [2699] */
    (xdc_Char)0x6c,  /* [2700] */
    (xdc_Char)0x72,  /* [2701] */
    (xdc_Char)0x65,  /* [2702] */
    (xdc_Char)0x61,  /* [2703] */
    (xdc_Char)0x64,  /* [2704] */
    (xdc_Char)0x79,  /* [2705] */
    (xdc_Char)0x20,  /* [2706] */
    (xdc_Char)0x66,  /* [2707] */
    (xdc_Char)0x72,  /* [2708] */
    (xdc_Char)0x65,  /* [2709] */
    (xdc_Char)0x65,  /* [2710] */
    (xdc_Char)0x0,  /* [2711] */
    (xdc_Char)0x41,  /* [2712] */
    (xdc_Char)0x5f,  /* [2713] */
    (xdc_Char)0x62,  /* [2714] */
    (xdc_Char)0x75,  /* [2715] */
    (xdc_Char)0x66,  /* [2716] */
    (xdc_Char)0x4f,  /* [2717] */
    (xdc_Char)0x76,  /* [2718] */
    (xdc_Char)0x65,  /* [2719] */
    (xdc_Char)0x72,  /* [2720] */
    (xdc_Char)0x66,  /* [2721] */
    (xdc_Char)0x6c,  /* [2722] */
    (xdc_Char)0x6f,  /* [2723] */
    (xdc_Char)0x77,  /* [2724] */
    (xdc_Char)0x3a,  /* [2725] */
    (xdc_Char)0x20,  /* [2726] */
    (xdc_Char)0x42,  /* [2727] */
    (xdc_Char)0x75,  /* [2728] */
    (xdc_Char)0x66,  /* [2729] */
    (xdc_Char)0x66,  /* [2730] */
    (xdc_Char)0x65,  /* [2731] */
    (xdc_Char)0x72,  /* [2732] */
    (xdc_Char)0x20,  /* [2733] */
    (xdc_Char)0x6f,  /* [2734] */
    (xdc_Char)0x76,  /* [2735] */
    (xdc_Char)0x65,  /* [2736] */
    (xdc_Char)0x72,  /* [2737] */
    (xdc_Char)0x66,  /* [2738] */
    (xdc_Char)0x6c,  /* [2739] */
    (xdc_Char)0x6f,  /* [2740] */
    (xdc_Char)0x77,  /* [2741] */
    (xdc_Char)0x0,  /* [2742] */
    (xdc_Char)0x41,  /* [2743] */
    (xdc_Char)0x5f,  /* [2744] */
    (xdc_Char)0x6e,  /* [2745] */
    (xdc_Char)0x6f,  /* [2746] */
    (xdc_Char)0x74,  /* [2747] */
    (xdc_Char)0x45,  /* [2748] */
    (xdc_Char)0x6d,  /* [2749] */
    (xdc_Char)0x70,  /* [2750] */
    (xdc_Char)0x74,  /* [2751] */
    (xdc_Char)0x79,  /* [2752] */
    (xdc_Char)0x3a,  /* [2753] */
    (xdc_Char)0x20,  /* [2754] */
    (xdc_Char)0x48,  /* [2755] */
    (xdc_Char)0x65,  /* [2756] */
    (xdc_Char)0x61,  /* [2757] */
    (xdc_Char)0x70,  /* [2758] */
    (xdc_Char)0x20,  /* [2759] */
    (xdc_Char)0x6e,  /* [2760] */
    (xdc_Char)0x6f,  /* [2761] */
    (xdc_Char)0x74,  /* [2762] */
    (xdc_Char)0x20,  /* [2763] */
    (xdc_Char)0x65,  /* [2764] */
    (xdc_Char)0x6d,  /* [2765] */
    (xdc_Char)0x70,  /* [2766] */
    (xdc_Char)0x74,  /* [2767] */
    (xdc_Char)0x79,  /* [2768] */
    (xdc_Char)0x0,  /* [2769] */
    (xdc_Char)0x41,  /* [2770] */
    (xdc_Char)0x5f,  /* [2771] */
    (xdc_Char)0x6e,  /* [2772] */
    (xdc_Char)0x75,  /* [2773] */
    (xdc_Char)0x6c,  /* [2774] */
    (xdc_Char)0x6c,  /* [2775] */
    (xdc_Char)0x4f,  /* [2776] */
    (xdc_Char)0x62,  /* [2777] */
    (xdc_Char)0x6a,  /* [2778] */
    (xdc_Char)0x65,  /* [2779] */
    (xdc_Char)0x63,  /* [2780] */
    (xdc_Char)0x74,  /* [2781] */
    (xdc_Char)0x3a,  /* [2782] */
    (xdc_Char)0x20,  /* [2783] */
    (xdc_Char)0x48,  /* [2784] */
    (xdc_Char)0x65,  /* [2785] */
    (xdc_Char)0x61,  /* [2786] */
    (xdc_Char)0x70,  /* [2787] */
    (xdc_Char)0x54,  /* [2788] */
    (xdc_Char)0x72,  /* [2789] */
    (xdc_Char)0x61,  /* [2790] */
    (xdc_Char)0x63,  /* [2791] */
    (xdc_Char)0x6b,  /* [2792] */
    (xdc_Char)0x5f,  /* [2793] */
    (xdc_Char)0x70,  /* [2794] */
    (xdc_Char)0x72,  /* [2795] */
    (xdc_Char)0x69,  /* [2796] */
    (xdc_Char)0x6e,  /* [2797] */
    (xdc_Char)0x74,  /* [2798] */
    (xdc_Char)0x48,  /* [2799] */
    (xdc_Char)0x65,  /* [2800] */
    (xdc_Char)0x61,  /* [2801] */
    (xdc_Char)0x70,  /* [2802] */
    (xdc_Char)0x20,  /* [2803] */
    (xdc_Char)0x63,  /* [2804] */
    (xdc_Char)0x61,  /* [2805] */
    (xdc_Char)0x6c,  /* [2806] */
    (xdc_Char)0x6c,  /* [2807] */
    (xdc_Char)0x65,  /* [2808] */
    (xdc_Char)0x64,  /* [2809] */
    (xdc_Char)0x20,  /* [2810] */
    (xdc_Char)0x77,  /* [2811] */
    (xdc_Char)0x69,  /* [2812] */
    (xdc_Char)0x74,  /* [2813] */
    (xdc_Char)0x68,  /* [2814] */
    (xdc_Char)0x20,  /* [2815] */
    (xdc_Char)0x6e,  /* [2816] */
    (xdc_Char)0x75,  /* [2817] */
    (xdc_Char)0x6c,  /* [2818] */
    (xdc_Char)0x6c,  /* [2819] */
    (xdc_Char)0x20,  /* [2820] */
    (xdc_Char)0x6f,  /* [2821] */
    (xdc_Char)0x62,  /* [2822] */
    (xdc_Char)0x6a,  /* [2823] */
    (xdc_Char)0x0,  /* [2824] */
    (xdc_Char)0x41,  /* [2825] */
    (xdc_Char)0x5f,  /* [2826] */
    (xdc_Char)0x62,  /* [2827] */
    (xdc_Char)0x61,  /* [2828] */
    (xdc_Char)0x64,  /* [2829] */
    (xdc_Char)0x43,  /* [2830] */
    (xdc_Char)0x68,  /* [2831] */
    (xdc_Char)0x61,  /* [2832] */
    (xdc_Char)0x6e,  /* [2833] */
    (xdc_Char)0x6e,  /* [2834] */
    (xdc_Char)0x65,  /* [2835] */
    (xdc_Char)0x6c,  /* [2836] */
    (xdc_Char)0x49,  /* [2837] */
    (xdc_Char)0x64,  /* [2838] */
    (xdc_Char)0x3a,  /* [2839] */
    (xdc_Char)0x20,  /* [2840] */
    (xdc_Char)0x43,  /* [2841] */
    (xdc_Char)0x68,  /* [2842] */
    (xdc_Char)0x61,  /* [2843] */
    (xdc_Char)0x6e,  /* [2844] */
    (xdc_Char)0x6e,  /* [2845] */
    (xdc_Char)0x65,  /* [2846] */
    (xdc_Char)0x6c,  /* [2847] */
    (xdc_Char)0x49,  /* [2848] */
    (xdc_Char)0x64,  /* [2849] */
    (xdc_Char)0x20,  /* [2850] */
    (xdc_Char)0x69,  /* [2851] */
    (xdc_Char)0x73,  /* [2852] */
    (xdc_Char)0x20,  /* [2853] */
    (xdc_Char)0x65,  /* [2854] */
    (xdc_Char)0x69,  /* [2855] */
    (xdc_Char)0x74,  /* [2856] */
    (xdc_Char)0x68,  /* [2857] */
    (xdc_Char)0x65,  /* [2858] */
    (xdc_Char)0x72,  /* [2859] */
    (xdc_Char)0x20,  /* [2860] */
    (xdc_Char)0x6e,  /* [2861] */
    (xdc_Char)0x6f,  /* [2862] */
    (xdc_Char)0x74,  /* [2863] */
    (xdc_Char)0x20,  /* [2864] */
    (xdc_Char)0x72,  /* [2865] */
    (xdc_Char)0x65,  /* [2866] */
    (xdc_Char)0x2d,  /* [2867] */
    (xdc_Char)0x6d,  /* [2868] */
    (xdc_Char)0x61,  /* [2869] */
    (xdc_Char)0x70,  /* [2870] */
    (xdc_Char)0x70,  /* [2871] */
    (xdc_Char)0x61,  /* [2872] */
    (xdc_Char)0x62,  /* [2873] */
    (xdc_Char)0x6c,  /* [2874] */
    (xdc_Char)0x65,  /* [2875] */
    (xdc_Char)0x20,  /* [2876] */
    (xdc_Char)0x6f,  /* [2877] */
    (xdc_Char)0x72,  /* [2878] */
    (xdc_Char)0x20,  /* [2879] */
    (xdc_Char)0x69,  /* [2880] */
    (xdc_Char)0x6e,  /* [2881] */
    (xdc_Char)0x76,  /* [2882] */
    (xdc_Char)0x61,  /* [2883] */
    (xdc_Char)0x6c,  /* [2884] */
    (xdc_Char)0x69,  /* [2885] */
    (xdc_Char)0x64,  /* [2886] */
    (xdc_Char)0x2e,  /* [2887] */
    (xdc_Char)0x0,  /* [2888] */
    (xdc_Char)0x41,  /* [2889] */
    (xdc_Char)0x5f,  /* [2890] */
    (xdc_Char)0x7a,  /* [2891] */
    (xdc_Char)0x65,  /* [2892] */
    (xdc_Char)0x72,  /* [2893] */
    (xdc_Char)0x6f,  /* [2894] */
    (xdc_Char)0x54,  /* [2895] */
    (xdc_Char)0x69,  /* [2896] */
    (xdc_Char)0x6d,  /* [2897] */
    (xdc_Char)0x65,  /* [2898] */
    (xdc_Char)0x6f,  /* [2899] */
    (xdc_Char)0x75,  /* [2900] */
    (xdc_Char)0x74,  /* [2901] */
    (xdc_Char)0x3a,  /* [2902] */
    (xdc_Char)0x20,  /* [2903] */
    (xdc_Char)0x54,  /* [2904] */
    (xdc_Char)0x69,  /* [2905] */
    (xdc_Char)0x6d,  /* [2906] */
    (xdc_Char)0x65,  /* [2907] */
    (xdc_Char)0x6f,  /* [2908] */
    (xdc_Char)0x75,  /* [2909] */
    (xdc_Char)0x74,  /* [2910] */
    (xdc_Char)0x20,  /* [2911] */
    (xdc_Char)0x76,  /* [2912] */
    (xdc_Char)0x61,  /* [2913] */
    (xdc_Char)0x6c,  /* [2914] */
    (xdc_Char)0x75,  /* [2915] */
    (xdc_Char)0x65,  /* [2916] */
    (xdc_Char)0x20,  /* [2917] */
    (xdc_Char)0x61,  /* [2918] */
    (xdc_Char)0x6e,  /* [2919] */
    (xdc_Char)0x6e,  /* [2920] */
    (xdc_Char)0x6f,  /* [2921] */
    (xdc_Char)0x74,  /* [2922] */
    (xdc_Char)0x20,  /* [2923] */
    (xdc_Char)0x62,  /* [2924] */
    (xdc_Char)0x65,  /* [2925] */
    (xdc_Char)0x20,  /* [2926] */
    (xdc_Char)0x7a,  /* [2927] */
    (xdc_Char)0x65,  /* [2928] */
    (xdc_Char)0x72,  /* [2929] */
    (xdc_Char)0x6f,  /* [2930] */
    (xdc_Char)0x0,  /* [2931] */
    (xdc_Char)0x41,  /* [2932] */
    (xdc_Char)0x5f,  /* [2933] */
    (xdc_Char)0x69,  /* [2934] */
    (xdc_Char)0x6e,  /* [2935] */
    (xdc_Char)0x76,  /* [2936] */
    (xdc_Char)0x61,  /* [2937] */
    (xdc_Char)0x6c,  /* [2938] */
    (xdc_Char)0x69,  /* [2939] */
    (xdc_Char)0x64,  /* [2940] */
    (xdc_Char)0x4b,  /* [2941] */
    (xdc_Char)0x65,  /* [2942] */
    (xdc_Char)0x79,  /* [2943] */
    (xdc_Char)0x3a,  /* [2944] */
    (xdc_Char)0x20,  /* [2945] */
    (xdc_Char)0x74,  /* [2946] */
    (xdc_Char)0x68,  /* [2947] */
    (xdc_Char)0x65,  /* [2948] */
    (xdc_Char)0x20,  /* [2949] */
    (xdc_Char)0x6b,  /* [2950] */
    (xdc_Char)0x65,  /* [2951] */
    (xdc_Char)0x79,  /* [2952] */
    (xdc_Char)0x20,  /* [2953] */
    (xdc_Char)0x6d,  /* [2954] */
    (xdc_Char)0x75,  /* [2955] */
    (xdc_Char)0x73,  /* [2956] */
    (xdc_Char)0x74,  /* [2957] */
    (xdc_Char)0x20,  /* [2958] */
    (xdc_Char)0x62,  /* [2959] */
    (xdc_Char)0x65,  /* [2960] */
    (xdc_Char)0x20,  /* [2961] */
    (xdc_Char)0x73,  /* [2962] */
    (xdc_Char)0x65,  /* [2963] */
    (xdc_Char)0x74,  /* [2964] */
    (xdc_Char)0x20,  /* [2965] */
    (xdc_Char)0x74,  /* [2966] */
    (xdc_Char)0x6f,  /* [2967] */
    (xdc_Char)0x20,  /* [2968] */
    (xdc_Char)0x61,  /* [2969] */
    (xdc_Char)0x20,  /* [2970] */
    (xdc_Char)0x6e,  /* [2971] */
    (xdc_Char)0x6f,  /* [2972] */
    (xdc_Char)0x6e,  /* [2973] */
    (xdc_Char)0x2d,  /* [2974] */
    (xdc_Char)0x64,  /* [2975] */
    (xdc_Char)0x65,  /* [2976] */
    (xdc_Char)0x66,  /* [2977] */
    (xdc_Char)0x61,  /* [2978] */
    (xdc_Char)0x75,  /* [2979] */
    (xdc_Char)0x6c,  /* [2980] */
    (xdc_Char)0x74,  /* [2981] */
    (xdc_Char)0x20,  /* [2982] */
    (xdc_Char)0x76,  /* [2983] */
    (xdc_Char)0x61,  /* [2984] */
    (xdc_Char)0x6c,  /* [2985] */
    (xdc_Char)0x75,  /* [2986] */
    (xdc_Char)0x65,  /* [2987] */
    (xdc_Char)0x0,  /* [2988] */
    (xdc_Char)0x41,  /* [2989] */
    (xdc_Char)0x5f,  /* [2990] */
    (xdc_Char)0x62,  /* [2991] */
    (xdc_Char)0x61,  /* [2992] */
    (xdc_Char)0x64,  /* [2993] */
    (xdc_Char)0x43,  /* [2994] */
    (xdc_Char)0x6f,  /* [2995] */
    (xdc_Char)0x6e,  /* [2996] */
    (xdc_Char)0x74,  /* [2997] */
    (xdc_Char)0x65,  /* [2998] */
    (xdc_Char)0x78,  /* [2999] */
    (xdc_Char)0x74,  /* [3000] */
    (xdc_Char)0x3a,  /* [3001] */
    (xdc_Char)0x20,  /* [3002] */
    (xdc_Char)0x62,  /* [3003] */
    (xdc_Char)0x61,  /* [3004] */
    (xdc_Char)0x64,  /* [3005] */
    (xdc_Char)0x20,  /* [3006] */
    (xdc_Char)0x63,  /* [3007] */
    (xdc_Char)0x61,  /* [3008] */
    (xdc_Char)0x6c,  /* [3009] */
    (xdc_Char)0x6c,  /* [3010] */
    (xdc_Char)0x69,  /* [3011] */
    (xdc_Char)0x6e,  /* [3012] */
    (xdc_Char)0x67,  /* [3013] */
    (xdc_Char)0x20,  /* [3014] */
    (xdc_Char)0x63,  /* [3015] */
    (xdc_Char)0x6f,  /* [3016] */
    (xdc_Char)0x6e,  /* [3017] */
    (xdc_Char)0x74,  /* [3018] */
    (xdc_Char)0x65,  /* [3019] */
    (xdc_Char)0x78,  /* [3020] */
    (xdc_Char)0x74,  /* [3021] */
    (xdc_Char)0x2e,  /* [3022] */
    (xdc_Char)0x20,  /* [3023] */
    (xdc_Char)0x4d,  /* [3024] */
    (xdc_Char)0x61,  /* [3025] */
    (xdc_Char)0x79,  /* [3026] */
    (xdc_Char)0x20,  /* [3027] */
    (xdc_Char)0x6e,  /* [3028] */
    (xdc_Char)0x6f,  /* [3029] */
    (xdc_Char)0x74,  /* [3030] */
    (xdc_Char)0x20,  /* [3031] */
    (xdc_Char)0x62,  /* [3032] */
    (xdc_Char)0x65,  /* [3033] */
    (xdc_Char)0x20,  /* [3034] */
    (xdc_Char)0x65,  /* [3035] */
    (xdc_Char)0x6e,  /* [3036] */
    (xdc_Char)0x74,  /* [3037] */
    (xdc_Char)0x65,  /* [3038] */
    (xdc_Char)0x72,  /* [3039] */
    (xdc_Char)0x65,  /* [3040] */
    (xdc_Char)0x64,  /* [3041] */
    (xdc_Char)0x20,  /* [3042] */
    (xdc_Char)0x66,  /* [3043] */
    (xdc_Char)0x72,  /* [3044] */
    (xdc_Char)0x6f,  /* [3045] */
    (xdc_Char)0x6d,  /* [3046] */
    (xdc_Char)0x20,  /* [3047] */
    (xdc_Char)0x61,  /* [3048] */
    (xdc_Char)0x20,  /* [3049] */
    (xdc_Char)0x68,  /* [3050] */
    (xdc_Char)0x61,  /* [3051] */
    (xdc_Char)0x72,  /* [3052] */
    (xdc_Char)0x64,  /* [3053] */
    (xdc_Char)0x77,  /* [3054] */
    (xdc_Char)0x61,  /* [3055] */
    (xdc_Char)0x72,  /* [3056] */
    (xdc_Char)0x65,  /* [3057] */
    (xdc_Char)0x20,  /* [3058] */
    (xdc_Char)0x69,  /* [3059] */
    (xdc_Char)0x6e,  /* [3060] */
    (xdc_Char)0x74,  /* [3061] */
    (xdc_Char)0x65,  /* [3062] */
    (xdc_Char)0x72,  /* [3063] */
    (xdc_Char)0x72,  /* [3064] */
    (xdc_Char)0x75,  /* [3065] */
    (xdc_Char)0x70,  /* [3066] */
    (xdc_Char)0x74,  /* [3067] */
    (xdc_Char)0x20,  /* [3068] */
    (xdc_Char)0x74,  /* [3069] */
    (xdc_Char)0x68,  /* [3070] */
    (xdc_Char)0x72,  /* [3071] */
    (xdc_Char)0x65,  /* [3072] */
    (xdc_Char)0x61,  /* [3073] */
    (xdc_Char)0x64,  /* [3074] */
    (xdc_Char)0x2e,  /* [3075] */
    (xdc_Char)0x0,  /* [3076] */
    (xdc_Char)0x41,  /* [3077] */
    (xdc_Char)0x5f,  /* [3078] */
    (xdc_Char)0x62,  /* [3079] */
    (xdc_Char)0x61,  /* [3080] */
    (xdc_Char)0x64,  /* [3081] */
    (xdc_Char)0x43,  /* [3082] */
    (xdc_Char)0x6f,  /* [3083] */
    (xdc_Char)0x6e,  /* [3084] */
    (xdc_Char)0x74,  /* [3085] */
    (xdc_Char)0x65,  /* [3086] */
    (xdc_Char)0x78,  /* [3087] */
    (xdc_Char)0x74,  /* [3088] */
    (xdc_Char)0x3a,  /* [3089] */
    (xdc_Char)0x20,  /* [3090] */
    (xdc_Char)0x62,  /* [3091] */
    (xdc_Char)0x61,  /* [3092] */
    (xdc_Char)0x64,  /* [3093] */
    (xdc_Char)0x20,  /* [3094] */
    (xdc_Char)0x63,  /* [3095] */
    (xdc_Char)0x61,  /* [3096] */
    (xdc_Char)0x6c,  /* [3097] */
    (xdc_Char)0x6c,  /* [3098] */
    (xdc_Char)0x69,  /* [3099] */
    (xdc_Char)0x6e,  /* [3100] */
    (xdc_Char)0x67,  /* [3101] */
    (xdc_Char)0x20,  /* [3102] */
    (xdc_Char)0x63,  /* [3103] */
    (xdc_Char)0x6f,  /* [3104] */
    (xdc_Char)0x6e,  /* [3105] */
    (xdc_Char)0x74,  /* [3106] */
    (xdc_Char)0x65,  /* [3107] */
    (xdc_Char)0x78,  /* [3108] */
    (xdc_Char)0x74,  /* [3109] */
    (xdc_Char)0x2e,  /* [3110] */
    (xdc_Char)0x20,  /* [3111] */
    (xdc_Char)0x4d,  /* [3112] */
    (xdc_Char)0x61,  /* [3113] */
    (xdc_Char)0x79,  /* [3114] */
    (xdc_Char)0x20,  /* [3115] */
    (xdc_Char)0x6e,  /* [3116] */
    (xdc_Char)0x6f,  /* [3117] */
    (xdc_Char)0x74,  /* [3118] */
    (xdc_Char)0x20,  /* [3119] */
    (xdc_Char)0x62,  /* [3120] */
    (xdc_Char)0x65,  /* [3121] */
    (xdc_Char)0x20,  /* [3122] */
    (xdc_Char)0x65,  /* [3123] */
    (xdc_Char)0x6e,  /* [3124] */
    (xdc_Char)0x74,  /* [3125] */
    (xdc_Char)0x65,  /* [3126] */
    (xdc_Char)0x72,  /* [3127] */
    (xdc_Char)0x65,  /* [3128] */
    (xdc_Char)0x64,  /* [3129] */
    (xdc_Char)0x20,  /* [3130] */
    (xdc_Char)0x66,  /* [3131] */
    (xdc_Char)0x72,  /* [3132] */
    (xdc_Char)0x6f,  /* [3133] */
    (xdc_Char)0x6d,  /* [3134] */
    (xdc_Char)0x20,  /* [3135] */
    (xdc_Char)0x61,  /* [3136] */
    (xdc_Char)0x20,  /* [3137] */
    (xdc_Char)0x73,  /* [3138] */
    (xdc_Char)0x6f,  /* [3139] */
    (xdc_Char)0x66,  /* [3140] */
    (xdc_Char)0x74,  /* [3141] */
    (xdc_Char)0x77,  /* [3142] */
    (xdc_Char)0x61,  /* [3143] */
    (xdc_Char)0x72,  /* [3144] */
    (xdc_Char)0x65,  /* [3145] */
    (xdc_Char)0x20,  /* [3146] */
    (xdc_Char)0x6f,  /* [3147] */
    (xdc_Char)0x72,  /* [3148] */
    (xdc_Char)0x20,  /* [3149] */
    (xdc_Char)0x68,  /* [3150] */
    (xdc_Char)0x61,  /* [3151] */
    (xdc_Char)0x72,  /* [3152] */
    (xdc_Char)0x64,  /* [3153] */
    (xdc_Char)0x77,  /* [3154] */
    (xdc_Char)0x61,  /* [3155] */
    (xdc_Char)0x72,  /* [3156] */
    (xdc_Char)0x65,  /* [3157] */
    (xdc_Char)0x20,  /* [3158] */
    (xdc_Char)0x69,  /* [3159] */
    (xdc_Char)0x6e,  /* [3160] */
    (xdc_Char)0x74,  /* [3161] */
    (xdc_Char)0x65,  /* [3162] */
    (xdc_Char)0x72,  /* [3163] */
    (xdc_Char)0x72,  /* [3164] */
    (xdc_Char)0x75,  /* [3165] */
    (xdc_Char)0x70,  /* [3166] */
    (xdc_Char)0x74,  /* [3167] */
    (xdc_Char)0x20,  /* [3168] */
    (xdc_Char)0x74,  /* [3169] */
    (xdc_Char)0x68,  /* [3170] */
    (xdc_Char)0x72,  /* [3171] */
    (xdc_Char)0x65,  /* [3172] */
    (xdc_Char)0x61,  /* [3173] */
    (xdc_Char)0x64,  /* [3174] */
    (xdc_Char)0x2e,  /* [3175] */
    (xdc_Char)0x0,  /* [3176] */
    (xdc_Char)0x41,  /* [3177] */
    (xdc_Char)0x5f,  /* [3178] */
    (xdc_Char)0x62,  /* [3179] */
    (xdc_Char)0x61,  /* [3180] */
    (xdc_Char)0x64,  /* [3181] */
    (xdc_Char)0x43,  /* [3182] */
    (xdc_Char)0x6f,  /* [3183] */
    (xdc_Char)0x6e,  /* [3184] */
    (xdc_Char)0x74,  /* [3185] */
    (xdc_Char)0x65,  /* [3186] */
    (xdc_Char)0x78,  /* [3187] */
    (xdc_Char)0x74,  /* [3188] */
    (xdc_Char)0x3a,  /* [3189] */
    (xdc_Char)0x20,  /* [3190] */
    (xdc_Char)0x62,  /* [3191] */
    (xdc_Char)0x61,  /* [3192] */
    (xdc_Char)0x64,  /* [3193] */
    (xdc_Char)0x20,  /* [3194] */
    (xdc_Char)0x63,  /* [3195] */
    (xdc_Char)0x61,  /* [3196] */
    (xdc_Char)0x6c,  /* [3197] */
    (xdc_Char)0x6c,  /* [3198] */
    (xdc_Char)0x69,  /* [3199] */
    (xdc_Char)0x6e,  /* [3200] */
    (xdc_Char)0x67,  /* [3201] */
    (xdc_Char)0x20,  /* [3202] */
    (xdc_Char)0x63,  /* [3203] */
    (xdc_Char)0x6f,  /* [3204] */
    (xdc_Char)0x6e,  /* [3205] */
    (xdc_Char)0x74,  /* [3206] */
    (xdc_Char)0x65,  /* [3207] */
    (xdc_Char)0x78,  /* [3208] */
    (xdc_Char)0x74,  /* [3209] */
    (xdc_Char)0x2e,  /* [3210] */
    (xdc_Char)0x20,  /* [3211] */
    (xdc_Char)0x53,  /* [3212] */
    (xdc_Char)0x65,  /* [3213] */
    (xdc_Char)0x65,  /* [3214] */
    (xdc_Char)0x20,  /* [3215] */
    (xdc_Char)0x47,  /* [3216] */
    (xdc_Char)0x61,  /* [3217] */
    (xdc_Char)0x74,  /* [3218] */
    (xdc_Char)0x65,  /* [3219] */
    (xdc_Char)0x4d,  /* [3220] */
    (xdc_Char)0x75,  /* [3221] */
    (xdc_Char)0x74,  /* [3222] */
    (xdc_Char)0x65,  /* [3223] */
    (xdc_Char)0x78,  /* [3224] */
    (xdc_Char)0x50,  /* [3225] */
    (xdc_Char)0x72,  /* [3226] */
    (xdc_Char)0x69,  /* [3227] */
    (xdc_Char)0x20,  /* [3228] */
    (xdc_Char)0x41,  /* [3229] */
    (xdc_Char)0x50,  /* [3230] */
    (xdc_Char)0x49,  /* [3231] */
    (xdc_Char)0x20,  /* [3232] */
    (xdc_Char)0x64,  /* [3233] */
    (xdc_Char)0x6f,  /* [3234] */
    (xdc_Char)0x63,  /* [3235] */
    (xdc_Char)0x20,  /* [3236] */
    (xdc_Char)0x66,  /* [3237] */
    (xdc_Char)0x6f,  /* [3238] */
    (xdc_Char)0x72,  /* [3239] */
    (xdc_Char)0x20,  /* [3240] */
    (xdc_Char)0x64,  /* [3241] */
    (xdc_Char)0x65,  /* [3242] */
    (xdc_Char)0x74,  /* [3243] */
    (xdc_Char)0x61,  /* [3244] */
    (xdc_Char)0x69,  /* [3245] */
    (xdc_Char)0x6c,  /* [3246] */
    (xdc_Char)0x73,  /* [3247] */
    (xdc_Char)0x2e,  /* [3248] */
    (xdc_Char)0x0,  /* [3249] */
    (xdc_Char)0x41,  /* [3250] */
    (xdc_Char)0x5f,  /* [3251] */
    (xdc_Char)0x65,  /* [3252] */
    (xdc_Char)0x6e,  /* [3253] */
    (xdc_Char)0x74,  /* [3254] */
    (xdc_Char)0x65,  /* [3255] */
    (xdc_Char)0x72,  /* [3256] */
    (xdc_Char)0x54,  /* [3257] */
    (xdc_Char)0x61,  /* [3258] */
    (xdc_Char)0x73,  /* [3259] */
    (xdc_Char)0x6b,  /* [3260] */
    (xdc_Char)0x44,  /* [3261] */
    (xdc_Char)0x69,  /* [3262] */
    (xdc_Char)0x73,  /* [3263] */
    (xdc_Char)0x61,  /* [3264] */
    (xdc_Char)0x62,  /* [3265] */
    (xdc_Char)0x6c,  /* [3266] */
    (xdc_Char)0x65,  /* [3267] */
    (xdc_Char)0x64,  /* [3268] */
    (xdc_Char)0x3a,  /* [3269] */
    (xdc_Char)0x20,  /* [3270] */
    (xdc_Char)0x43,  /* [3271] */
    (xdc_Char)0x61,  /* [3272] */
    (xdc_Char)0x6e,  /* [3273] */
    (xdc_Char)0x6e,  /* [3274] */
    (xdc_Char)0x6f,  /* [3275] */
    (xdc_Char)0x74,  /* [3276] */
    (xdc_Char)0x20,  /* [3277] */
    (xdc_Char)0x63,  /* [3278] */
    (xdc_Char)0x61,  /* [3279] */
    (xdc_Char)0x6c,  /* [3280] */
    (xdc_Char)0x6c,  /* [3281] */
    (xdc_Char)0x20,  /* [3282] */
    (xdc_Char)0x47,  /* [3283] */
    (xdc_Char)0x61,  /* [3284] */
    (xdc_Char)0x74,  /* [3285] */
    (xdc_Char)0x65,  /* [3286] */
    (xdc_Char)0x4d,  /* [3287] */
    (xdc_Char)0x75,  /* [3288] */
    (xdc_Char)0x74,  /* [3289] */
    (xdc_Char)0x65,  /* [3290] */
    (xdc_Char)0x78,  /* [3291] */
    (xdc_Char)0x50,  /* [3292] */
    (xdc_Char)0x72,  /* [3293] */
    (xdc_Char)0x69,  /* [3294] */
    (xdc_Char)0x5f,  /* [3295] */
    (xdc_Char)0x65,  /* [3296] */
    (xdc_Char)0x6e,  /* [3297] */
    (xdc_Char)0x74,  /* [3298] */
    (xdc_Char)0x65,  /* [3299] */
    (xdc_Char)0x72,  /* [3300] */
    (xdc_Char)0x28,  /* [3301] */
    (xdc_Char)0x29,  /* [3302] */
    (xdc_Char)0x20,  /* [3303] */
    (xdc_Char)0x77,  /* [3304] */
    (xdc_Char)0x68,  /* [3305] */
    (xdc_Char)0x69,  /* [3306] */
    (xdc_Char)0x6c,  /* [3307] */
    (xdc_Char)0x65,  /* [3308] */
    (xdc_Char)0x20,  /* [3309] */
    (xdc_Char)0x74,  /* [3310] */
    (xdc_Char)0x68,  /* [3311] */
    (xdc_Char)0x65,  /* [3312] */
    (xdc_Char)0x20,  /* [3313] */
    (xdc_Char)0x54,  /* [3314] */
    (xdc_Char)0x61,  /* [3315] */
    (xdc_Char)0x73,  /* [3316] */
    (xdc_Char)0x6b,  /* [3317] */
    (xdc_Char)0x20,  /* [3318] */
    (xdc_Char)0x6f,  /* [3319] */
    (xdc_Char)0x72,  /* [3320] */
    (xdc_Char)0x20,  /* [3321] */
    (xdc_Char)0x53,  /* [3322] */
    (xdc_Char)0x77,  /* [3323] */
    (xdc_Char)0x69,  /* [3324] */
    (xdc_Char)0x20,  /* [3325] */
    (xdc_Char)0x73,  /* [3326] */
    (xdc_Char)0x63,  /* [3327] */
    (xdc_Char)0x68,  /* [3328] */
    (xdc_Char)0x65,  /* [3329] */
    (xdc_Char)0x64,  /* [3330] */
    (xdc_Char)0x75,  /* [3331] */
    (xdc_Char)0x6c,  /* [3332] */
    (xdc_Char)0x65,  /* [3333] */
    (xdc_Char)0x72,  /* [3334] */
    (xdc_Char)0x20,  /* [3335] */
    (xdc_Char)0x69,  /* [3336] */
    (xdc_Char)0x73,  /* [3337] */
    (xdc_Char)0x20,  /* [3338] */
    (xdc_Char)0x64,  /* [3339] */
    (xdc_Char)0x69,  /* [3340] */
    (xdc_Char)0x73,  /* [3341] */
    (xdc_Char)0x61,  /* [3342] */
    (xdc_Char)0x62,  /* [3343] */
    (xdc_Char)0x6c,  /* [3344] */
    (xdc_Char)0x65,  /* [3345] */
    (xdc_Char)0x64,  /* [3346] */
    (xdc_Char)0x2e,  /* [3347] */
    (xdc_Char)0x0,  /* [3348] */
    (xdc_Char)0x41,  /* [3349] */
    (xdc_Char)0x5f,  /* [3350] */
    (xdc_Char)0x62,  /* [3351] */
    (xdc_Char)0x61,  /* [3352] */
    (xdc_Char)0x64,  /* [3353] */
    (xdc_Char)0x43,  /* [3354] */
    (xdc_Char)0x6f,  /* [3355] */
    (xdc_Char)0x6e,  /* [3356] */
    (xdc_Char)0x74,  /* [3357] */
    (xdc_Char)0x65,  /* [3358] */
    (xdc_Char)0x78,  /* [3359] */
    (xdc_Char)0x74,  /* [3360] */
    (xdc_Char)0x3a,  /* [3361] */
    (xdc_Char)0x20,  /* [3362] */
    (xdc_Char)0x62,  /* [3363] */
    (xdc_Char)0x61,  /* [3364] */
    (xdc_Char)0x64,  /* [3365] */
    (xdc_Char)0x20,  /* [3366] */
    (xdc_Char)0x63,  /* [3367] */
    (xdc_Char)0x61,  /* [3368] */
    (xdc_Char)0x6c,  /* [3369] */
    (xdc_Char)0x6c,  /* [3370] */
    (xdc_Char)0x69,  /* [3371] */
    (xdc_Char)0x6e,  /* [3372] */
    (xdc_Char)0x67,  /* [3373] */
    (xdc_Char)0x20,  /* [3374] */
    (xdc_Char)0x63,  /* [3375] */
    (xdc_Char)0x6f,  /* [3376] */
    (xdc_Char)0x6e,  /* [3377] */
    (xdc_Char)0x74,  /* [3378] */
    (xdc_Char)0x65,  /* [3379] */
    (xdc_Char)0x78,  /* [3380] */
    (xdc_Char)0x74,  /* [3381] */
    (xdc_Char)0x2e,  /* [3382] */
    (xdc_Char)0x20,  /* [3383] */
    (xdc_Char)0x53,  /* [3384] */
    (xdc_Char)0x65,  /* [3385] */
    (xdc_Char)0x65,  /* [3386] */
    (xdc_Char)0x20,  /* [3387] */
    (xdc_Char)0x47,  /* [3388] */
    (xdc_Char)0x61,  /* [3389] */
    (xdc_Char)0x74,  /* [3390] */
    (xdc_Char)0x65,  /* [3391] */
    (xdc_Char)0x4d,  /* [3392] */
    (xdc_Char)0x75,  /* [3393] */
    (xdc_Char)0x74,  /* [3394] */
    (xdc_Char)0x65,  /* [3395] */
    (xdc_Char)0x78,  /* [3396] */
    (xdc_Char)0x20,  /* [3397] */
    (xdc_Char)0x41,  /* [3398] */
    (xdc_Char)0x50,  /* [3399] */
    (xdc_Char)0x49,  /* [3400] */
    (xdc_Char)0x20,  /* [3401] */
    (xdc_Char)0x64,  /* [3402] */
    (xdc_Char)0x6f,  /* [3403] */
    (xdc_Char)0x63,  /* [3404] */
    (xdc_Char)0x20,  /* [3405] */
    (xdc_Char)0x66,  /* [3406] */
    (xdc_Char)0x6f,  /* [3407] */
    (xdc_Char)0x72,  /* [3408] */
    (xdc_Char)0x20,  /* [3409] */
    (xdc_Char)0x64,  /* [3410] */
    (xdc_Char)0x65,  /* [3411] */
    (xdc_Char)0x74,  /* [3412] */
    (xdc_Char)0x61,  /* [3413] */
    (xdc_Char)0x69,  /* [3414] */
    (xdc_Char)0x6c,  /* [3415] */
    (xdc_Char)0x73,  /* [3416] */
    (xdc_Char)0x2e,  /* [3417] */
    (xdc_Char)0x0,  /* [3418] */
    (xdc_Char)0x41,  /* [3419] */
    (xdc_Char)0x5f,  /* [3420] */
    (xdc_Char)0x62,  /* [3421] */
    (xdc_Char)0x61,  /* [3422] */
    (xdc_Char)0x64,  /* [3423] */
    (xdc_Char)0x43,  /* [3424] */
    (xdc_Char)0x6f,  /* [3425] */
    (xdc_Char)0x6e,  /* [3426] */
    (xdc_Char)0x74,  /* [3427] */
    (xdc_Char)0x65,  /* [3428] */
    (xdc_Char)0x78,  /* [3429] */
    (xdc_Char)0x74,  /* [3430] */
    (xdc_Char)0x3a,  /* [3431] */
    (xdc_Char)0x20,  /* [3432] */
    (xdc_Char)0x62,  /* [3433] */
    (xdc_Char)0x61,  /* [3434] */
    (xdc_Char)0x64,  /* [3435] */
    (xdc_Char)0x20,  /* [3436] */
    (xdc_Char)0x63,  /* [3437] */
    (xdc_Char)0x61,  /* [3438] */
    (xdc_Char)0x6c,  /* [3439] */
    (xdc_Char)0x6c,  /* [3440] */
    (xdc_Char)0x69,  /* [3441] */
    (xdc_Char)0x6e,  /* [3442] */
    (xdc_Char)0x67,  /* [3443] */
    (xdc_Char)0x20,  /* [3444] */
    (xdc_Char)0x63,  /* [3445] */
    (xdc_Char)0x6f,  /* [3446] */
    (xdc_Char)0x6e,  /* [3447] */
    (xdc_Char)0x74,  /* [3448] */
    (xdc_Char)0x65,  /* [3449] */
    (xdc_Char)0x78,  /* [3450] */
    (xdc_Char)0x74,  /* [3451] */
    (xdc_Char)0x2e,  /* [3452] */
    (xdc_Char)0x20,  /* [3453] */
    (xdc_Char)0x53,  /* [3454] */
    (xdc_Char)0x65,  /* [3455] */
    (xdc_Char)0x65,  /* [3456] */
    (xdc_Char)0x20,  /* [3457] */
    (xdc_Char)0x47,  /* [3458] */
    (xdc_Char)0x61,  /* [3459] */
    (xdc_Char)0x74,  /* [3460] */
    (xdc_Char)0x65,  /* [3461] */
    (xdc_Char)0x53,  /* [3462] */
    (xdc_Char)0x70,  /* [3463] */
    (xdc_Char)0x69,  /* [3464] */
    (xdc_Char)0x6e,  /* [3465] */
    (xdc_Char)0x6c,  /* [3466] */
    (xdc_Char)0x6f,  /* [3467] */
    (xdc_Char)0x63,  /* [3468] */
    (xdc_Char)0x6b,  /* [3469] */
    (xdc_Char)0x20,  /* [3470] */
    (xdc_Char)0x41,  /* [3471] */
    (xdc_Char)0x50,  /* [3472] */
    (xdc_Char)0x49,  /* [3473] */
    (xdc_Char)0x20,  /* [3474] */
    (xdc_Char)0x64,  /* [3475] */
    (xdc_Char)0x6f,  /* [3476] */
    (xdc_Char)0x63,  /* [3477] */
    (xdc_Char)0x20,  /* [3478] */
    (xdc_Char)0x66,  /* [3479] */
    (xdc_Char)0x6f,  /* [3480] */
    (xdc_Char)0x72,  /* [3481] */
    (xdc_Char)0x20,  /* [3482] */
    (xdc_Char)0x64,  /* [3483] */
    (xdc_Char)0x65,  /* [3484] */
    (xdc_Char)0x74,  /* [3485] */
    (xdc_Char)0x61,  /* [3486] */
    (xdc_Char)0x69,  /* [3487] */
    (xdc_Char)0x6c,  /* [3488] */
    (xdc_Char)0x73,  /* [3489] */
    (xdc_Char)0x2e,  /* [3490] */
    (xdc_Char)0x0,  /* [3491] */
    (xdc_Char)0x41,  /* [3492] */
    (xdc_Char)0x5f,  /* [3493] */
    (xdc_Char)0x69,  /* [3494] */
    (xdc_Char)0x6e,  /* [3495] */
    (xdc_Char)0x76,  /* [3496] */
    (xdc_Char)0x61,  /* [3497] */
    (xdc_Char)0x6c,  /* [3498] */
    (xdc_Char)0x69,  /* [3499] */
    (xdc_Char)0x64,  /* [3500] */
    (xdc_Char)0x51,  /* [3501] */
    (xdc_Char)0x75,  /* [3502] */
    (xdc_Char)0x61,  /* [3503] */
    (xdc_Char)0x6c,  /* [3504] */
    (xdc_Char)0x69,  /* [3505] */
    (xdc_Char)0x74,  /* [3506] */
    (xdc_Char)0x79,  /* [3507] */
    (xdc_Char)0x3a,  /* [3508] */
    (xdc_Char)0x20,  /* [3509] */
    (xdc_Char)0x53,  /* [3510] */
    (xdc_Char)0x65,  /* [3511] */
    (xdc_Char)0x65,  /* [3512] */
    (xdc_Char)0x20,  /* [3513] */
    (xdc_Char)0x47,  /* [3514] */
    (xdc_Char)0x61,  /* [3515] */
    (xdc_Char)0x74,  /* [3516] */
    (xdc_Char)0x65,  /* [3517] */
    (xdc_Char)0x53,  /* [3518] */
    (xdc_Char)0x70,  /* [3519] */
    (xdc_Char)0x69,  /* [3520] */
    (xdc_Char)0x6e,  /* [3521] */
    (xdc_Char)0x6c,  /* [3522] */
    (xdc_Char)0x6f,  /* [3523] */
    (xdc_Char)0x63,  /* [3524] */
    (xdc_Char)0x6b,  /* [3525] */
    (xdc_Char)0x20,  /* [3526] */
    (xdc_Char)0x41,  /* [3527] */
    (xdc_Char)0x50,  /* [3528] */
    (xdc_Char)0x49,  /* [3529] */
    (xdc_Char)0x20,  /* [3530] */
    (xdc_Char)0x64,  /* [3531] */
    (xdc_Char)0x6f,  /* [3532] */
    (xdc_Char)0x63,  /* [3533] */
    (xdc_Char)0x20,  /* [3534] */
    (xdc_Char)0x66,  /* [3535] */
    (xdc_Char)0x6f,  /* [3536] */
    (xdc_Char)0x72,  /* [3537] */
    (xdc_Char)0x20,  /* [3538] */
    (xdc_Char)0x64,  /* [3539] */
    (xdc_Char)0x65,  /* [3540] */
    (xdc_Char)0x74,  /* [3541] */
    (xdc_Char)0x61,  /* [3542] */
    (xdc_Char)0x69,  /* [3543] */
    (xdc_Char)0x6c,  /* [3544] */
    (xdc_Char)0x73,  /* [3545] */
    (xdc_Char)0x2e,  /* [3546] */
    (xdc_Char)0x0,  /* [3547] */
    (xdc_Char)0x41,  /* [3548] */
    (xdc_Char)0x5f,  /* [3549] */
    (xdc_Char)0x69,  /* [3550] */
    (xdc_Char)0x6e,  /* [3551] */
    (xdc_Char)0x76,  /* [3552] */
    (xdc_Char)0x61,  /* [3553] */
    (xdc_Char)0x6c,  /* [3554] */
    (xdc_Char)0x69,  /* [3555] */
    (xdc_Char)0x64,  /* [3556] */
    (xdc_Char)0x54,  /* [3557] */
    (xdc_Char)0x69,  /* [3558] */
    (xdc_Char)0x6d,  /* [3559] */
    (xdc_Char)0x65,  /* [3560] */
    (xdc_Char)0x72,  /* [3561] */
    (xdc_Char)0x3a,  /* [3562] */
    (xdc_Char)0x20,  /* [3563] */
    (xdc_Char)0x49,  /* [3564] */
    (xdc_Char)0x6e,  /* [3565] */
    (xdc_Char)0x76,  /* [3566] */
    (xdc_Char)0x61,  /* [3567] */
    (xdc_Char)0x6c,  /* [3568] */
    (xdc_Char)0x69,  /* [3569] */
    (xdc_Char)0x64,  /* [3570] */
    (xdc_Char)0x20,  /* [3571] */
    (xdc_Char)0x54,  /* [3572] */
    (xdc_Char)0x69,  /* [3573] */
    (xdc_Char)0x6d,  /* [3574] */
    (xdc_Char)0x65,  /* [3575] */
    (xdc_Char)0x72,  /* [3576] */
    (xdc_Char)0x20,  /* [3577] */
    (xdc_Char)0x49,  /* [3578] */
    (xdc_Char)0x64,  /* [3579] */
    (xdc_Char)0x2e,  /* [3580] */
    (xdc_Char)0x0,  /* [3581] */
    (xdc_Char)0x41,  /* [3582] */
    (xdc_Char)0x5f,  /* [3583] */
    (xdc_Char)0x62,  /* [3584] */
    (xdc_Char)0x61,  /* [3585] */
    (xdc_Char)0x64,  /* [3586] */
    (xdc_Char)0x42,  /* [3587] */
    (xdc_Char)0x6c,  /* [3588] */
    (xdc_Char)0x6f,  /* [3589] */
    (xdc_Char)0x63,  /* [3590] */
    (xdc_Char)0x6b,  /* [3591] */
    (xdc_Char)0x4c,  /* [3592] */
    (xdc_Char)0x65,  /* [3593] */
    (xdc_Char)0x6e,  /* [3594] */
    (xdc_Char)0x67,  /* [3595] */
    (xdc_Char)0x74,  /* [3596] */
    (xdc_Char)0x68,  /* [3597] */
    (xdc_Char)0x3a,  /* [3598] */
    (xdc_Char)0x20,  /* [3599] */
    (xdc_Char)0x42,  /* [3600] */
    (xdc_Char)0x6c,  /* [3601] */
    (xdc_Char)0x6f,  /* [3602] */
    (xdc_Char)0x63,  /* [3603] */
    (xdc_Char)0x6b,  /* [3604] */
    (xdc_Char)0x20,  /* [3605] */
    (xdc_Char)0x6c,  /* [3606] */
    (xdc_Char)0x65,  /* [3607] */
    (xdc_Char)0x6e,  /* [3608] */
    (xdc_Char)0x67,  /* [3609] */
    (xdc_Char)0x74,  /* [3610] */
    (xdc_Char)0x68,  /* [3611] */
    (xdc_Char)0x20,  /* [3612] */
    (xdc_Char)0x74,  /* [3613] */
    (xdc_Char)0x6f,  /* [3614] */
    (xdc_Char)0x6f,  /* [3615] */
    (xdc_Char)0x20,  /* [3616] */
    (xdc_Char)0x6c,  /* [3617] */
    (xdc_Char)0x61,  /* [3618] */
    (xdc_Char)0x72,  /* [3619] */
    (xdc_Char)0x67,  /* [3620] */
    (xdc_Char)0x65,  /* [3621] */
    (xdc_Char)0x2e,  /* [3622] */
    (xdc_Char)0x20,  /* [3623] */
    (xdc_Char)0x4d,  /* [3624] */
    (xdc_Char)0x75,  /* [3625] */
    (xdc_Char)0x73,  /* [3626] */
    (xdc_Char)0x74,  /* [3627] */
    (xdc_Char)0x20,  /* [3628] */
    (xdc_Char)0x62,  /* [3629] */
    (xdc_Char)0x65,  /* [3630] */
    (xdc_Char)0x20,  /* [3631] */
    (xdc_Char)0x3c,  /* [3632] */
    (xdc_Char)0x3d,  /* [3633] */
    (xdc_Char)0x20,  /* [3634] */
    (xdc_Char)0x4c,  /* [3635] */
    (xdc_Char)0x32,  /* [3636] */
    (xdc_Char)0x20,  /* [3637] */
    (xdc_Char)0x77,  /* [3638] */
    (xdc_Char)0x61,  /* [3639] */
    (xdc_Char)0x79,  /* [3640] */
    (xdc_Char)0x20,  /* [3641] */
    (xdc_Char)0x73,  /* [3642] */
    (xdc_Char)0x69,  /* [3643] */
    (xdc_Char)0x7a,  /* [3644] */
    (xdc_Char)0x65,  /* [3645] */
    (xdc_Char)0x2e,  /* [3646] */
    (xdc_Char)0x0,  /* [3647] */
    (xdc_Char)0x41,  /* [3648] */
    (xdc_Char)0x5f,  /* [3649] */
    (xdc_Char)0x62,  /* [3650] */
    (xdc_Char)0x6c,  /* [3651] */
    (xdc_Char)0x6f,  /* [3652] */
    (xdc_Char)0x63,  /* [3653] */
    (xdc_Char)0x6b,  /* [3654] */
    (xdc_Char)0x43,  /* [3655] */
    (xdc_Char)0x72,  /* [3656] */
    (xdc_Char)0x6f,  /* [3657] */
    (xdc_Char)0x73,  /* [3658] */
    (xdc_Char)0x73,  /* [3659] */
    (xdc_Char)0x65,  /* [3660] */
    (xdc_Char)0x73,  /* [3661] */
    (xdc_Char)0x50,  /* [3662] */
    (xdc_Char)0x61,  /* [3663] */
    (xdc_Char)0x67,  /* [3664] */
    (xdc_Char)0x65,  /* [3665] */
    (xdc_Char)0x3a,  /* [3666] */
    (xdc_Char)0x20,  /* [3667] */
    (xdc_Char)0x4d,  /* [3668] */
    (xdc_Char)0x65,  /* [3669] */
    (xdc_Char)0x6d,  /* [3670] */
    (xdc_Char)0x6f,  /* [3671] */
    (xdc_Char)0x72,  /* [3672] */
    (xdc_Char)0x79,  /* [3673] */
    (xdc_Char)0x20,  /* [3674] */
    (xdc_Char)0x62,  /* [3675] */
    (xdc_Char)0x6c,  /* [3676] */
    (xdc_Char)0x6f,  /* [3677] */
    (xdc_Char)0x63,  /* [3678] */
    (xdc_Char)0x6b,  /* [3679] */
    (xdc_Char)0x20,  /* [3680] */
    (xdc_Char)0x63,  /* [3681] */
    (xdc_Char)0x72,  /* [3682] */
    (xdc_Char)0x6f,  /* [3683] */
    (xdc_Char)0x73,  /* [3684] */
    (xdc_Char)0x73,  /* [3685] */
    (xdc_Char)0x65,  /* [3686] */
    (xdc_Char)0x73,  /* [3687] */
    (xdc_Char)0x20,  /* [3688] */
    (xdc_Char)0x4c,  /* [3689] */
    (xdc_Char)0x32,  /* [3690] */
    (xdc_Char)0x20,  /* [3691] */
    (xdc_Char)0x77,  /* [3692] */
    (xdc_Char)0x61,  /* [3693] */
    (xdc_Char)0x79,  /* [3694] */
    (xdc_Char)0x20,  /* [3695] */
    (xdc_Char)0x70,  /* [3696] */
    (xdc_Char)0x61,  /* [3697] */
    (xdc_Char)0x67,  /* [3698] */
    (xdc_Char)0x65,  /* [3699] */
    (xdc_Char)0x20,  /* [3700] */
    (xdc_Char)0x62,  /* [3701] */
    (xdc_Char)0x6f,  /* [3702] */
    (xdc_Char)0x75,  /* [3703] */
    (xdc_Char)0x6e,  /* [3704] */
    (xdc_Char)0x64,  /* [3705] */
    (xdc_Char)0x61,  /* [3706] */
    (xdc_Char)0x72,  /* [3707] */
    (xdc_Char)0x79,  /* [3708] */
    (xdc_Char)0x2e,  /* [3709] */
    (xdc_Char)0x0,  /* [3710] */
    (xdc_Char)0x41,  /* [3711] */
    (xdc_Char)0x5f,  /* [3712] */
    (xdc_Char)0x75,  /* [3713] */
    (xdc_Char)0x6e,  /* [3714] */
    (xdc_Char)0x6b,  /* [3715] */
    (xdc_Char)0x6e,  /* [3716] */
    (xdc_Char)0x6f,  /* [3717] */
    (xdc_Char)0x77,  /* [3718] */
    (xdc_Char)0x6e,  /* [3719] */
    (xdc_Char)0x44,  /* [3720] */
    (xdc_Char)0x65,  /* [3721] */
    (xdc_Char)0x73,  /* [3722] */
    (xdc_Char)0x63,  /* [3723] */
    (xdc_Char)0x54,  /* [3724] */
    (xdc_Char)0x79,  /* [3725] */
    (xdc_Char)0x70,  /* [3726] */
    (xdc_Char)0x65,  /* [3727] */
    (xdc_Char)0x3a,  /* [3728] */
    (xdc_Char)0x20,  /* [3729] */
    (xdc_Char)0x44,  /* [3730] */
    (xdc_Char)0x65,  /* [3731] */
    (xdc_Char)0x73,  /* [3732] */
    (xdc_Char)0x63,  /* [3733] */
    (xdc_Char)0x72,  /* [3734] */
    (xdc_Char)0x69,  /* [3735] */
    (xdc_Char)0x70,  /* [3736] */
    (xdc_Char)0x74,  /* [3737] */
    (xdc_Char)0x6f,  /* [3738] */
    (xdc_Char)0x72,  /* [3739] */
    (xdc_Char)0x20,  /* [3740] */
    (xdc_Char)0x74,  /* [3741] */
    (xdc_Char)0x79,  /* [3742] */
    (xdc_Char)0x70,  /* [3743] */
    (xdc_Char)0x65,  /* [3744] */
    (xdc_Char)0x20,  /* [3745] */
    (xdc_Char)0x69,  /* [3746] */
    (xdc_Char)0x73,  /* [3747] */
    (xdc_Char)0x20,  /* [3748] */
    (xdc_Char)0x6e,  /* [3749] */
    (xdc_Char)0x6f,  /* [3750] */
    (xdc_Char)0x74,  /* [3751] */
    (xdc_Char)0x20,  /* [3752] */
    (xdc_Char)0x72,  /* [3753] */
    (xdc_Char)0x65,  /* [3754] */
    (xdc_Char)0x63,  /* [3755] */
    (xdc_Char)0x6f,  /* [3756] */
    (xdc_Char)0x67,  /* [3757] */
    (xdc_Char)0x6e,  /* [3758] */
    (xdc_Char)0x69,  /* [3759] */
    (xdc_Char)0x7a,  /* [3760] */
    (xdc_Char)0x65,  /* [3761] */
    (xdc_Char)0x64,  /* [3762] */
    (xdc_Char)0x0,  /* [3763] */
    (xdc_Char)0x41,  /* [3764] */
    (xdc_Char)0x5f,  /* [3765] */
    (xdc_Char)0x64,  /* [3766] */
    (xdc_Char)0x69,  /* [3767] */
    (xdc_Char)0x73,  /* [3768] */
    (xdc_Char)0x61,  /* [3769] */
    (xdc_Char)0x62,  /* [3770] */
    (xdc_Char)0x6c,  /* [3771] */
    (xdc_Char)0x65,  /* [3772] */
    (xdc_Char)0x55,  /* [3773] */
    (xdc_Char)0x6e,  /* [3774] */
    (xdc_Char)0x73,  /* [3775] */
    (xdc_Char)0x75,  /* [3776] */
    (xdc_Char)0x70,  /* [3777] */
    (xdc_Char)0x70,  /* [3778] */
    (xdc_Char)0x6f,  /* [3779] */
    (xdc_Char)0x72,  /* [3780] */
    (xdc_Char)0x74,  /* [3781] */
    (xdc_Char)0x65,  /* [3782] */
    (xdc_Char)0x64,  /* [3783] */
    (xdc_Char)0x3a,  /* [3784] */
    (xdc_Char)0x20,  /* [3785] */
    (xdc_Char)0x4d,  /* [3786] */
    (xdc_Char)0x4d,  /* [3787] */
    (xdc_Char)0x55,  /* [3788] */
    (xdc_Char)0x20,  /* [3789] */
    (xdc_Char)0x64,  /* [3790] */
    (xdc_Char)0x69,  /* [3791] */
    (xdc_Char)0x73,  /* [3792] */
    (xdc_Char)0x61,  /* [3793] */
    (xdc_Char)0x62,  /* [3794] */
    (xdc_Char)0x6c,  /* [3795] */
    (xdc_Char)0x65,  /* [3796] */
    (xdc_Char)0x20,  /* [3797] */
    (xdc_Char)0x69,  /* [3798] */
    (xdc_Char)0x73,  /* [3799] */
    (xdc_Char)0x20,  /* [3800] */
    (xdc_Char)0x6e,  /* [3801] */
    (xdc_Char)0x6f,  /* [3802] */
    (xdc_Char)0x74,  /* [3803] */
    (xdc_Char)0x20,  /* [3804] */
    (xdc_Char)0x73,  /* [3805] */
    (xdc_Char)0x75,  /* [3806] */
    (xdc_Char)0x70,  /* [3807] */
    (xdc_Char)0x70,  /* [3808] */
    (xdc_Char)0x6f,  /* [3809] */
    (xdc_Char)0x72,  /* [3810] */
    (xdc_Char)0x74,  /* [3811] */
    (xdc_Char)0x65,  /* [3812] */
    (xdc_Char)0x64,  /* [3813] */
    (xdc_Char)0x20,  /* [3814] */
    (xdc_Char)0x69,  /* [3815] */
    (xdc_Char)0x6e,  /* [3816] */
    (xdc_Char)0x20,  /* [3817] */
    (xdc_Char)0x53,  /* [3818] */
    (xdc_Char)0x4d,  /* [3819] */
    (xdc_Char)0x50,  /* [3820] */
    (xdc_Char)0x20,  /* [3821] */
    (xdc_Char)0x6d,  /* [3822] */
    (xdc_Char)0x6f,  /* [3823] */
    (xdc_Char)0x64,  /* [3824] */
    (xdc_Char)0x65,  /* [3825] */
    (xdc_Char)0x2e,  /* [3826] */
    (xdc_Char)0x0,  /* [3827] */
    (xdc_Char)0x61,  /* [3828] */
    (xdc_Char)0x73,  /* [3829] */
    (xdc_Char)0x73,  /* [3830] */
    (xdc_Char)0x65,  /* [3831] */
    (xdc_Char)0x72,  /* [3832] */
    (xdc_Char)0x74,  /* [3833] */
    (xdc_Char)0x69,  /* [3834] */
    (xdc_Char)0x6f,  /* [3835] */
    (xdc_Char)0x6e,  /* [3836] */
    (xdc_Char)0x20,  /* [3837] */
    (xdc_Char)0x66,  /* [3838] */
    (xdc_Char)0x61,  /* [3839] */
    (xdc_Char)0x69,  /* [3840] */
    (xdc_Char)0x6c,  /* [3841] */
    (xdc_Char)0x75,  /* [3842] */
    (xdc_Char)0x72,  /* [3843] */
    (xdc_Char)0x65,  /* [3844] */
    (xdc_Char)0x25,  /* [3845] */
    (xdc_Char)0x73,  /* [3846] */
    (xdc_Char)0x25,  /* [3847] */
    (xdc_Char)0x73,  /* [3848] */
    (xdc_Char)0x0,  /* [3849] */
    (xdc_Char)0x25,  /* [3850] */
    (xdc_Char)0x24,  /* [3851] */
    (xdc_Char)0x53,  /* [3852] */
    (xdc_Char)0x0,  /* [3853] */
    (xdc_Char)0x6f,  /* [3854] */
    (xdc_Char)0x75,  /* [3855] */
    (xdc_Char)0x74,  /* [3856] */
    (xdc_Char)0x20,  /* [3857] */
    (xdc_Char)0x6f,  /* [3858] */
    (xdc_Char)0x66,  /* [3859] */
    (xdc_Char)0x20,  /* [3860] */
    (xdc_Char)0x6d,  /* [3861] */
    (xdc_Char)0x65,  /* [3862] */
    (xdc_Char)0x6d,  /* [3863] */
    (xdc_Char)0x6f,  /* [3864] */
    (xdc_Char)0x72,  /* [3865] */
    (xdc_Char)0x79,  /* [3866] */
    (xdc_Char)0x3a,  /* [3867] */
    (xdc_Char)0x20,  /* [3868] */
    (xdc_Char)0x68,  /* [3869] */
    (xdc_Char)0x65,  /* [3870] */
    (xdc_Char)0x61,  /* [3871] */
    (xdc_Char)0x70,  /* [3872] */
    (xdc_Char)0x3d,  /* [3873] */
    (xdc_Char)0x30,  /* [3874] */
    (xdc_Char)0x78,  /* [3875] */
    (xdc_Char)0x25,  /* [3876] */
    (xdc_Char)0x78,  /* [3877] */
    (xdc_Char)0x2c,  /* [3878] */
    (xdc_Char)0x20,  /* [3879] */
    (xdc_Char)0x73,  /* [3880] */
    (xdc_Char)0x69,  /* [3881] */
    (xdc_Char)0x7a,  /* [3882] */
    (xdc_Char)0x65,  /* [3883] */
    (xdc_Char)0x3d,  /* [3884] */
    (xdc_Char)0x25,  /* [3885] */
    (xdc_Char)0x75,  /* [3886] */
    (xdc_Char)0x0,  /* [3887] */
    (xdc_Char)0x25,  /* [3888] */
    (xdc_Char)0x73,  /* [3889] */
    (xdc_Char)0x20,  /* [3890] */
    (xdc_Char)0x30,  /* [3891] */
    (xdc_Char)0x78,  /* [3892] */
    (xdc_Char)0x25,  /* [3893] */
    (xdc_Char)0x78,  /* [3894] */
    (xdc_Char)0x0,  /* [3895] */
    (xdc_Char)0x45,  /* [3896] */
    (xdc_Char)0x5f,  /* [3897] */
    (xdc_Char)0x62,  /* [3898] */
    (xdc_Char)0x61,  /* [3899] */
    (xdc_Char)0x64,  /* [3900] */
    (xdc_Char)0x4c,  /* [3901] */
    (xdc_Char)0x65,  /* [3902] */
    (xdc_Char)0x76,  /* [3903] */
    (xdc_Char)0x65,  /* [3904] */
    (xdc_Char)0x6c,  /* [3905] */
    (xdc_Char)0x3a,  /* [3906] */
    (xdc_Char)0x20,  /* [3907] */
    (xdc_Char)0x42,  /* [3908] */
    (xdc_Char)0x61,  /* [3909] */
    (xdc_Char)0x64,  /* [3910] */
    (xdc_Char)0x20,  /* [3911] */
    (xdc_Char)0x66,  /* [3912] */
    (xdc_Char)0x69,  /* [3913] */
    (xdc_Char)0x6c,  /* [3914] */
    (xdc_Char)0x74,  /* [3915] */
    (xdc_Char)0x65,  /* [3916] */
    (xdc_Char)0x72,  /* [3917] */
    (xdc_Char)0x20,  /* [3918] */
    (xdc_Char)0x6c,  /* [3919] */
    (xdc_Char)0x65,  /* [3920] */
    (xdc_Char)0x76,  /* [3921] */
    (xdc_Char)0x65,  /* [3922] */
    (xdc_Char)0x6c,  /* [3923] */
    (xdc_Char)0x20,  /* [3924] */
    (xdc_Char)0x76,  /* [3925] */
    (xdc_Char)0x61,  /* [3926] */
    (xdc_Char)0x6c,  /* [3927] */
    (xdc_Char)0x75,  /* [3928] */
    (xdc_Char)0x65,  /* [3929] */
    (xdc_Char)0x3a,  /* [3930] */
    (xdc_Char)0x20,  /* [3931] */
    (xdc_Char)0x25,  /* [3932] */
    (xdc_Char)0x64,  /* [3933] */
    (xdc_Char)0x0,  /* [3934] */
    (xdc_Char)0x66,  /* [3935] */
    (xdc_Char)0x72,  /* [3936] */
    (xdc_Char)0x65,  /* [3937] */
    (xdc_Char)0x65,  /* [3938] */
    (xdc_Char)0x28,  /* [3939] */
    (xdc_Char)0x29,  /* [3940] */
    (xdc_Char)0x20,  /* [3941] */
    (xdc_Char)0x69,  /* [3942] */
    (xdc_Char)0x6e,  /* [3943] */
    (xdc_Char)0x76,  /* [3944] */
    (xdc_Char)0x61,  /* [3945] */
    (xdc_Char)0x6c,  /* [3946] */
    (xdc_Char)0x69,  /* [3947] */
    (xdc_Char)0x64,  /* [3948] */
    (xdc_Char)0x20,  /* [3949] */
    (xdc_Char)0x69,  /* [3950] */
    (xdc_Char)0x6e,  /* [3951] */
    (xdc_Char)0x20,  /* [3952] */
    (xdc_Char)0x67,  /* [3953] */
    (xdc_Char)0x72,  /* [3954] */
    (xdc_Char)0x6f,  /* [3955] */
    (xdc_Char)0x77,  /* [3956] */
    (xdc_Char)0x74,  /* [3957] */
    (xdc_Char)0x68,  /* [3958] */
    (xdc_Char)0x2d,  /* [3959] */
    (xdc_Char)0x6f,  /* [3960] */
    (xdc_Char)0x6e,  /* [3961] */
    (xdc_Char)0x6c,  /* [3962] */
    (xdc_Char)0x79,  /* [3963] */
    (xdc_Char)0x20,  /* [3964] */
    (xdc_Char)0x48,  /* [3965] */
    (xdc_Char)0x65,  /* [3966] */
    (xdc_Char)0x61,  /* [3967] */
    (xdc_Char)0x70,  /* [3968] */
    (xdc_Char)0x4d,  /* [3969] */
    (xdc_Char)0x69,  /* [3970] */
    (xdc_Char)0x6e,  /* [3971] */
    (xdc_Char)0x0,  /* [3972] */
    (xdc_Char)0x54,  /* [3973] */
    (xdc_Char)0x68,  /* [3974] */
    (xdc_Char)0x65,  /* [3975] */
    (xdc_Char)0x20,  /* [3976] */
    (xdc_Char)0x52,  /* [3977] */
    (xdc_Char)0x54,  /* [3978] */
    (xdc_Char)0x53,  /* [3979] */
    (xdc_Char)0x20,  /* [3980] */
    (xdc_Char)0x68,  /* [3981] */
    (xdc_Char)0x65,  /* [3982] */
    (xdc_Char)0x61,  /* [3983] */
    (xdc_Char)0x70,  /* [3984] */
    (xdc_Char)0x20,  /* [3985] */
    (xdc_Char)0x69,  /* [3986] */
    (xdc_Char)0x73,  /* [3987] */
    (xdc_Char)0x20,  /* [3988] */
    (xdc_Char)0x75,  /* [3989] */
    (xdc_Char)0x73,  /* [3990] */
    (xdc_Char)0x65,  /* [3991] */
    (xdc_Char)0x64,  /* [3992] */
    (xdc_Char)0x20,  /* [3993] */
    (xdc_Char)0x75,  /* [3994] */
    (xdc_Char)0x70,  /* [3995] */
    (xdc_Char)0x2e,  /* [3996] */
    (xdc_Char)0x20,  /* [3997] */
    (xdc_Char)0x45,  /* [3998] */
    (xdc_Char)0x78,  /* [3999] */
    (xdc_Char)0x61,  /* [4000] */
    (xdc_Char)0x6d,  /* [4001] */
    (xdc_Char)0x69,  /* [4002] */
    (xdc_Char)0x6e,  /* [4003] */
    (xdc_Char)0x65,  /* [4004] */
    (xdc_Char)0x20,  /* [4005] */
    (xdc_Char)0x50,  /* [4006] */
    (xdc_Char)0x72,  /* [4007] */
    (xdc_Char)0x6f,  /* [4008] */
    (xdc_Char)0x67,  /* [4009] */
    (xdc_Char)0x72,  /* [4010] */
    (xdc_Char)0x61,  /* [4011] */
    (xdc_Char)0x6d,  /* [4012] */
    (xdc_Char)0x2e,  /* [4013] */
    (xdc_Char)0x68,  /* [4014] */
    (xdc_Char)0x65,  /* [4015] */
    (xdc_Char)0x61,  /* [4016] */
    (xdc_Char)0x70,  /* [4017] */
    (xdc_Char)0x2e,  /* [4018] */
    (xdc_Char)0x0,  /* [4019] */
    (xdc_Char)0x45,  /* [4020] */
    (xdc_Char)0x5f,  /* [4021] */
    (xdc_Char)0x62,  /* [4022] */
    (xdc_Char)0x61,  /* [4023] */
    (xdc_Char)0x64,  /* [4024] */
    (xdc_Char)0x43,  /* [4025] */
    (xdc_Char)0x6f,  /* [4026] */
    (xdc_Char)0x6d,  /* [4027] */
    (xdc_Char)0x6d,  /* [4028] */
    (xdc_Char)0x61,  /* [4029] */
    (xdc_Char)0x6e,  /* [4030] */
    (xdc_Char)0x64,  /* [4031] */
    (xdc_Char)0x3a,  /* [4032] */
    (xdc_Char)0x20,  /* [4033] */
    (xdc_Char)0x52,  /* [4034] */
    (xdc_Char)0x65,  /* [4035] */
    (xdc_Char)0x63,  /* [4036] */
    (xdc_Char)0x65,  /* [4037] */
    (xdc_Char)0x69,  /* [4038] */
    (xdc_Char)0x76,  /* [4039] */
    (xdc_Char)0x65,  /* [4040] */
    (xdc_Char)0x64,  /* [4041] */
    (xdc_Char)0x20,  /* [4042] */
    (xdc_Char)0x69,  /* [4043] */
    (xdc_Char)0x6e,  /* [4044] */
    (xdc_Char)0x76,  /* [4045] */
    (xdc_Char)0x61,  /* [4046] */
    (xdc_Char)0x6c,  /* [4047] */
    (xdc_Char)0x69,  /* [4048] */
    (xdc_Char)0x64,  /* [4049] */
    (xdc_Char)0x20,  /* [4050] */
    (xdc_Char)0x63,  /* [4051] */
    (xdc_Char)0x6f,  /* [4052] */
    (xdc_Char)0x6d,  /* [4053] */
    (xdc_Char)0x6d,  /* [4054] */
    (xdc_Char)0x61,  /* [4055] */
    (xdc_Char)0x6e,  /* [4056] */
    (xdc_Char)0x64,  /* [4057] */
    (xdc_Char)0x2c,  /* [4058] */
    (xdc_Char)0x20,  /* [4059] */
    (xdc_Char)0x69,  /* [4060] */
    (xdc_Char)0x64,  /* [4061] */
    (xdc_Char)0x3a,  /* [4062] */
    (xdc_Char)0x20,  /* [4063] */
    (xdc_Char)0x25,  /* [4064] */
    (xdc_Char)0x64,  /* [4065] */
    (xdc_Char)0x2e,  /* [4066] */
    (xdc_Char)0x0,  /* [4067] */
    (xdc_Char)0x45,  /* [4068] */
    (xdc_Char)0x5f,  /* [4069] */
    (xdc_Char)0x6f,  /* [4070] */
    (xdc_Char)0x62,  /* [4071] */
    (xdc_Char)0x6a,  /* [4072] */
    (xdc_Char)0x65,  /* [4073] */
    (xdc_Char)0x63,  /* [4074] */
    (xdc_Char)0x74,  /* [4075] */
    (xdc_Char)0x4e,  /* [4076] */
    (xdc_Char)0x6f,  /* [4077] */
    (xdc_Char)0x74,  /* [4078] */
    (xdc_Char)0x49,  /* [4079] */
    (xdc_Char)0x6e,  /* [4080] */
    (xdc_Char)0x4b,  /* [4081] */
    (xdc_Char)0x65,  /* [4082] */
    (xdc_Char)0x72,  /* [4083] */
    (xdc_Char)0x6e,  /* [4084] */
    (xdc_Char)0x65,  /* [4085] */
    (xdc_Char)0x6c,  /* [4086] */
    (xdc_Char)0x53,  /* [4087] */
    (xdc_Char)0x70,  /* [4088] */
    (xdc_Char)0x61,  /* [4089] */
    (xdc_Char)0x63,  /* [4090] */
    (xdc_Char)0x65,  /* [4091] */
    (xdc_Char)0x3a,  /* [4092] */
    (xdc_Char)0x20,  /* [4093] */
    (xdc_Char)0x53,  /* [4094] */
    (xdc_Char)0x65,  /* [4095] */
    (xdc_Char)0x6d,  /* [4096] */
    (xdc_Char)0x61,  /* [4097] */
    (xdc_Char)0x70,  /* [4098] */
    (xdc_Char)0x68,  /* [4099] */
    (xdc_Char)0x6f,  /* [4100] */
    (xdc_Char)0x72,  /* [4101] */
    (xdc_Char)0x65,  /* [4102] */
    (xdc_Char)0x20,  /* [4103] */
    (xdc_Char)0x6f,  /* [4104] */
    (xdc_Char)0x62,  /* [4105] */
    (xdc_Char)0x6a,  /* [4106] */
    (xdc_Char)0x65,  /* [4107] */
    (xdc_Char)0x63,  /* [4108] */
    (xdc_Char)0x74,  /* [4109] */
    (xdc_Char)0x20,  /* [4110] */
    (xdc_Char)0x70,  /* [4111] */
    (xdc_Char)0x61,  /* [4112] */
    (xdc_Char)0x73,  /* [4113] */
    (xdc_Char)0x73,  /* [4114] */
    (xdc_Char)0x65,  /* [4115] */
    (xdc_Char)0x64,  /* [4116] */
    (xdc_Char)0x20,  /* [4117] */
    (xdc_Char)0x6e,  /* [4118] */
    (xdc_Char)0x6f,  /* [4119] */
    (xdc_Char)0x74,  /* [4120] */
    (xdc_Char)0x20,  /* [4121] */
    (xdc_Char)0x69,  /* [4122] */
    (xdc_Char)0x6e,  /* [4123] */
    (xdc_Char)0x20,  /* [4124] */
    (xdc_Char)0x4b,  /* [4125] */
    (xdc_Char)0x65,  /* [4126] */
    (xdc_Char)0x72,  /* [4127] */
    (xdc_Char)0x6e,  /* [4128] */
    (xdc_Char)0x65,  /* [4129] */
    (xdc_Char)0x6c,  /* [4130] */
    (xdc_Char)0x20,  /* [4131] */
    (xdc_Char)0x61,  /* [4132] */
    (xdc_Char)0x64,  /* [4133] */
    (xdc_Char)0x64,  /* [4134] */
    (xdc_Char)0x72,  /* [4135] */
    (xdc_Char)0x65,  /* [4136] */
    (xdc_Char)0x73,  /* [4137] */
    (xdc_Char)0x73,  /* [4138] */
    (xdc_Char)0x20,  /* [4139] */
    (xdc_Char)0x73,  /* [4140] */
    (xdc_Char)0x70,  /* [4141] */
    (xdc_Char)0x61,  /* [4142] */
    (xdc_Char)0x63,  /* [4143] */
    (xdc_Char)0x65,  /* [4144] */
    (xdc_Char)0x2e,  /* [4145] */
    (xdc_Char)0x0,  /* [4146] */
    (xdc_Char)0x45,  /* [4147] */
    (xdc_Char)0x5f,  /* [4148] */
    (xdc_Char)0x73,  /* [4149] */
    (xdc_Char)0x74,  /* [4150] */
    (xdc_Char)0x61,  /* [4151] */
    (xdc_Char)0x63,  /* [4152] */
    (xdc_Char)0x6b,  /* [4153] */
    (xdc_Char)0x4f,  /* [4154] */
    (xdc_Char)0x76,  /* [4155] */
    (xdc_Char)0x65,  /* [4156] */
    (xdc_Char)0x72,  /* [4157] */
    (xdc_Char)0x66,  /* [4158] */
    (xdc_Char)0x6c,  /* [4159] */
    (xdc_Char)0x6f,  /* [4160] */
    (xdc_Char)0x77,  /* [4161] */
    (xdc_Char)0x3a,  /* [4162] */
    (xdc_Char)0x20,  /* [4163] */
    (xdc_Char)0x54,  /* [4164] */
    (xdc_Char)0x61,  /* [4165] */
    (xdc_Char)0x73,  /* [4166] */
    (xdc_Char)0x6b,  /* [4167] */
    (xdc_Char)0x20,  /* [4168] */
    (xdc_Char)0x30,  /* [4169] */
    (xdc_Char)0x78,  /* [4170] */
    (xdc_Char)0x25,  /* [4171] */
    (xdc_Char)0x78,  /* [4172] */
    (xdc_Char)0x20,  /* [4173] */
    (xdc_Char)0x73,  /* [4174] */
    (xdc_Char)0x74,  /* [4175] */
    (xdc_Char)0x61,  /* [4176] */
    (xdc_Char)0x63,  /* [4177] */
    (xdc_Char)0x6b,  /* [4178] */
    (xdc_Char)0x20,  /* [4179] */
    (xdc_Char)0x6f,  /* [4180] */
    (xdc_Char)0x76,  /* [4181] */
    (xdc_Char)0x65,  /* [4182] */
    (xdc_Char)0x72,  /* [4183] */
    (xdc_Char)0x66,  /* [4184] */
    (xdc_Char)0x6c,  /* [4185] */
    (xdc_Char)0x6f,  /* [4186] */
    (xdc_Char)0x77,  /* [4187] */
    (xdc_Char)0x2e,  /* [4188] */
    (xdc_Char)0x0,  /* [4189] */
    (xdc_Char)0x45,  /* [4190] */
    (xdc_Char)0x5f,  /* [4191] */
    (xdc_Char)0x73,  /* [4192] */
    (xdc_Char)0x70,  /* [4193] */
    (xdc_Char)0x4f,  /* [4194] */
    (xdc_Char)0x75,  /* [4195] */
    (xdc_Char)0x74,  /* [4196] */
    (xdc_Char)0x4f,  /* [4197] */
    (xdc_Char)0x66,  /* [4198] */
    (xdc_Char)0x42,  /* [4199] */
    (xdc_Char)0x6f,  /* [4200] */
    (xdc_Char)0x75,  /* [4201] */
    (xdc_Char)0x6e,  /* [4202] */
    (xdc_Char)0x64,  /* [4203] */
    (xdc_Char)0x73,  /* [4204] */
    (xdc_Char)0x3a,  /* [4205] */
    (xdc_Char)0x20,  /* [4206] */
    (xdc_Char)0x54,  /* [4207] */
    (xdc_Char)0x61,  /* [4208] */
    (xdc_Char)0x73,  /* [4209] */
    (xdc_Char)0x6b,  /* [4210] */
    (xdc_Char)0x20,  /* [4211] */
    (xdc_Char)0x30,  /* [4212] */
    (xdc_Char)0x78,  /* [4213] */
    (xdc_Char)0x25,  /* [4214] */
    (xdc_Char)0x78,  /* [4215] */
    (xdc_Char)0x20,  /* [4216] */
    (xdc_Char)0x73,  /* [4217] */
    (xdc_Char)0x74,  /* [4218] */
    (xdc_Char)0x61,  /* [4219] */
    (xdc_Char)0x63,  /* [4220] */
    (xdc_Char)0x6b,  /* [4221] */
    (xdc_Char)0x20,  /* [4222] */
    (xdc_Char)0x65,  /* [4223] */
    (xdc_Char)0x72,  /* [4224] */
    (xdc_Char)0x72,  /* [4225] */
    (xdc_Char)0x6f,  /* [4226] */
    (xdc_Char)0x72,  /* [4227] */
    (xdc_Char)0x2c,  /* [4228] */
    (xdc_Char)0x20,  /* [4229] */
    (xdc_Char)0x53,  /* [4230] */
    (xdc_Char)0x50,  /* [4231] */
    (xdc_Char)0x20,  /* [4232] */
    (xdc_Char)0x3d,  /* [4233] */
    (xdc_Char)0x20,  /* [4234] */
    (xdc_Char)0x30,  /* [4235] */
    (xdc_Char)0x78,  /* [4236] */
    (xdc_Char)0x25,  /* [4237] */
    (xdc_Char)0x78,  /* [4238] */
    (xdc_Char)0x2e,  /* [4239] */
    (xdc_Char)0x0,  /* [4240] */
    (xdc_Char)0x45,  /* [4241] */
    (xdc_Char)0x5f,  /* [4242] */
    (xdc_Char)0x64,  /* [4243] */
    (xdc_Char)0x65,  /* [4244] */
    (xdc_Char)0x6c,  /* [4245] */
    (xdc_Char)0x65,  /* [4246] */
    (xdc_Char)0x74,  /* [4247] */
    (xdc_Char)0x65,  /* [4248] */
    (xdc_Char)0x4e,  /* [4249] */
    (xdc_Char)0x6f,  /* [4250] */
    (xdc_Char)0x74,  /* [4251] */
    (xdc_Char)0x41,  /* [4252] */
    (xdc_Char)0x6c,  /* [4253] */
    (xdc_Char)0x6c,  /* [4254] */
    (xdc_Char)0x6f,  /* [4255] */
    (xdc_Char)0x77,  /* [4256] */
    (xdc_Char)0x65,  /* [4257] */
    (xdc_Char)0x64,  /* [4258] */
    (xdc_Char)0x3a,  /* [4259] */
    (xdc_Char)0x20,  /* [4260] */
    (xdc_Char)0x54,  /* [4261] */
    (xdc_Char)0x61,  /* [4262] */
    (xdc_Char)0x73,  /* [4263] */
    (xdc_Char)0x6b,  /* [4264] */
    (xdc_Char)0x20,  /* [4265] */
    (xdc_Char)0x30,  /* [4266] */
    (xdc_Char)0x78,  /* [4267] */
    (xdc_Char)0x25,  /* [4268] */
    (xdc_Char)0x78,  /* [4269] */
    (xdc_Char)0x2e,  /* [4270] */
    (xdc_Char)0x0,  /* [4271] */
    (xdc_Char)0x45,  /* [4272] */
    (xdc_Char)0x5f,  /* [4273] */
    (xdc_Char)0x6d,  /* [4274] */
    (xdc_Char)0x6f,  /* [4275] */
    (xdc_Char)0x64,  /* [4276] */
    (xdc_Char)0x75,  /* [4277] */
    (xdc_Char)0x6c,  /* [4278] */
    (xdc_Char)0x65,  /* [4279] */
    (xdc_Char)0x53,  /* [4280] */
    (xdc_Char)0x74,  /* [4281] */
    (xdc_Char)0x61,  /* [4282] */
    (xdc_Char)0x74,  /* [4283] */
    (xdc_Char)0x65,  /* [4284] */
    (xdc_Char)0x43,  /* [4285] */
    (xdc_Char)0x68,  /* [4286] */
    (xdc_Char)0x65,  /* [4287] */
    (xdc_Char)0x63,  /* [4288] */
    (xdc_Char)0x6b,  /* [4289] */
    (xdc_Char)0x46,  /* [4290] */
    (xdc_Char)0x61,  /* [4291] */
    (xdc_Char)0x69,  /* [4292] */
    (xdc_Char)0x6c,  /* [4293] */
    (xdc_Char)0x65,  /* [4294] */
    (xdc_Char)0x64,  /* [4295] */
    (xdc_Char)0x3a,  /* [4296] */
    (xdc_Char)0x20,  /* [4297] */
    (xdc_Char)0x54,  /* [4298] */
    (xdc_Char)0x61,  /* [4299] */
    (xdc_Char)0x73,  /* [4300] */
    (xdc_Char)0x6b,  /* [4301] */
    (xdc_Char)0x20,  /* [4302] */
    (xdc_Char)0x6d,  /* [4303] */
    (xdc_Char)0x6f,  /* [4304] */
    (xdc_Char)0x64,  /* [4305] */
    (xdc_Char)0x75,  /* [4306] */
    (xdc_Char)0x6c,  /* [4307] */
    (xdc_Char)0x65,  /* [4308] */
    (xdc_Char)0x20,  /* [4309] */
    (xdc_Char)0x73,  /* [4310] */
    (xdc_Char)0x74,  /* [4311] */
    (xdc_Char)0x61,  /* [4312] */
    (xdc_Char)0x74,  /* [4313] */
    (xdc_Char)0x65,  /* [4314] */
    (xdc_Char)0x20,  /* [4315] */
    (xdc_Char)0x64,  /* [4316] */
    (xdc_Char)0x61,  /* [4317] */
    (xdc_Char)0x74,  /* [4318] */
    (xdc_Char)0x61,  /* [4319] */
    (xdc_Char)0x20,  /* [4320] */
    (xdc_Char)0x69,  /* [4321] */
    (xdc_Char)0x6e,  /* [4322] */
    (xdc_Char)0x74,  /* [4323] */
    (xdc_Char)0x65,  /* [4324] */
    (xdc_Char)0x67,  /* [4325] */
    (xdc_Char)0x72,  /* [4326] */
    (xdc_Char)0x69,  /* [4327] */
    (xdc_Char)0x74,  /* [4328] */
    (xdc_Char)0x79,  /* [4329] */
    (xdc_Char)0x20,  /* [4330] */
    (xdc_Char)0x63,  /* [4331] */
    (xdc_Char)0x68,  /* [4332] */
    (xdc_Char)0x65,  /* [4333] */
    (xdc_Char)0x63,  /* [4334] */
    (xdc_Char)0x6b,  /* [4335] */
    (xdc_Char)0x20,  /* [4336] */
    (xdc_Char)0x66,  /* [4337] */
    (xdc_Char)0x61,  /* [4338] */
    (xdc_Char)0x69,  /* [4339] */
    (xdc_Char)0x6c,  /* [4340] */
    (xdc_Char)0x65,  /* [4341] */
    (xdc_Char)0x64,  /* [4342] */
    (xdc_Char)0x2e,  /* [4343] */
    (xdc_Char)0x0,  /* [4344] */
    (xdc_Char)0x45,  /* [4345] */
    (xdc_Char)0x5f,  /* [4346] */
    (xdc_Char)0x6f,  /* [4347] */
    (xdc_Char)0x62,  /* [4348] */
    (xdc_Char)0x6a,  /* [4349] */
    (xdc_Char)0x65,  /* [4350] */
    (xdc_Char)0x63,  /* [4351] */
    (xdc_Char)0x74,  /* [4352] */
    (xdc_Char)0x43,  /* [4353] */
    (xdc_Char)0x68,  /* [4354] */
    (xdc_Char)0x65,  /* [4355] */
    (xdc_Char)0x63,  /* [4356] */
    (xdc_Char)0x6b,  /* [4357] */
    (xdc_Char)0x46,  /* [4358] */
    (xdc_Char)0x61,  /* [4359] */
    (xdc_Char)0x69,  /* [4360] */
    (xdc_Char)0x6c,  /* [4361] */
    (xdc_Char)0x65,  /* [4362] */
    (xdc_Char)0x64,  /* [4363] */
    (xdc_Char)0x3a,  /* [4364] */
    (xdc_Char)0x20,  /* [4365] */
    (xdc_Char)0x54,  /* [4366] */
    (xdc_Char)0x61,  /* [4367] */
    (xdc_Char)0x73,  /* [4368] */
    (xdc_Char)0x6b,  /* [4369] */
    (xdc_Char)0x20,  /* [4370] */
    (xdc_Char)0x30,  /* [4371] */
    (xdc_Char)0x78,  /* [4372] */
    (xdc_Char)0x25,  /* [4373] */
    (xdc_Char)0x78,  /* [4374] */
    (xdc_Char)0x20,  /* [4375] */
    (xdc_Char)0x6f,  /* [4376] */
    (xdc_Char)0x62,  /* [4377] */
    (xdc_Char)0x6a,  /* [4378] */
    (xdc_Char)0x65,  /* [4379] */
    (xdc_Char)0x63,  /* [4380] */
    (xdc_Char)0x74,  /* [4381] */
    (xdc_Char)0x20,  /* [4382] */
    (xdc_Char)0x64,  /* [4383] */
    (xdc_Char)0x61,  /* [4384] */
    (xdc_Char)0x74,  /* [4385] */
    (xdc_Char)0x61,  /* [4386] */
    (xdc_Char)0x20,  /* [4387] */
    (xdc_Char)0x69,  /* [4388] */
    (xdc_Char)0x6e,  /* [4389] */
    (xdc_Char)0x74,  /* [4390] */
    (xdc_Char)0x65,  /* [4391] */
    (xdc_Char)0x67,  /* [4392] */
    (xdc_Char)0x72,  /* [4393] */
    (xdc_Char)0x69,  /* [4394] */
    (xdc_Char)0x74,  /* [4395] */
    (xdc_Char)0x79,  /* [4396] */
    (xdc_Char)0x20,  /* [4397] */
    (xdc_Char)0x63,  /* [4398] */
    (xdc_Char)0x68,  /* [4399] */
    (xdc_Char)0x65,  /* [4400] */
    (xdc_Char)0x63,  /* [4401] */
    (xdc_Char)0x6b,  /* [4402] */
    (xdc_Char)0x20,  /* [4403] */
    (xdc_Char)0x66,  /* [4404] */
    (xdc_Char)0x61,  /* [4405] */
    (xdc_Char)0x69,  /* [4406] */
    (xdc_Char)0x6c,  /* [4407] */
    (xdc_Char)0x65,  /* [4408] */
    (xdc_Char)0x64,  /* [4409] */
    (xdc_Char)0x2e,  /* [4410] */
    (xdc_Char)0x0,  /* [4411] */
    (xdc_Char)0x45,  /* [4412] */
    (xdc_Char)0x5f,  /* [4413] */
    (xdc_Char)0x6f,  /* [4414] */
    (xdc_Char)0x62,  /* [4415] */
    (xdc_Char)0x6a,  /* [4416] */
    (xdc_Char)0x65,  /* [4417] */
    (xdc_Char)0x63,  /* [4418] */
    (xdc_Char)0x74,  /* [4419] */
    (xdc_Char)0x4e,  /* [4420] */
    (xdc_Char)0x6f,  /* [4421] */
    (xdc_Char)0x74,  /* [4422] */
    (xdc_Char)0x49,  /* [4423] */
    (xdc_Char)0x6e,  /* [4424] */
    (xdc_Char)0x4b,  /* [4425] */
    (xdc_Char)0x65,  /* [4426] */
    (xdc_Char)0x72,  /* [4427] */
    (xdc_Char)0x6e,  /* [4428] */
    (xdc_Char)0x65,  /* [4429] */
    (xdc_Char)0x6c,  /* [4430] */
    (xdc_Char)0x53,  /* [4431] */
    (xdc_Char)0x70,  /* [4432] */
    (xdc_Char)0x61,  /* [4433] */
    (xdc_Char)0x63,  /* [4434] */
    (xdc_Char)0x65,  /* [4435] */
    (xdc_Char)0x3a,  /* [4436] */
    (xdc_Char)0x20,  /* [4437] */
    (xdc_Char)0x54,  /* [4438] */
    (xdc_Char)0x61,  /* [4439] */
    (xdc_Char)0x73,  /* [4440] */
    (xdc_Char)0x6b,  /* [4441] */
    (xdc_Char)0x20,  /* [4442] */
    (xdc_Char)0x6f,  /* [4443] */
    (xdc_Char)0x62,  /* [4444] */
    (xdc_Char)0x6a,  /* [4445] */
    (xdc_Char)0x65,  /* [4446] */
    (xdc_Char)0x63,  /* [4447] */
    (xdc_Char)0x74,  /* [4448] */
    (xdc_Char)0x20,  /* [4449] */
    (xdc_Char)0x70,  /* [4450] */
    (xdc_Char)0x61,  /* [4451] */
    (xdc_Char)0x73,  /* [4452] */
    (xdc_Char)0x73,  /* [4453] */
    (xdc_Char)0x65,  /* [4454] */
    (xdc_Char)0x64,  /* [4455] */
    (xdc_Char)0x20,  /* [4456] */
    (xdc_Char)0x6e,  /* [4457] */
    (xdc_Char)0x6f,  /* [4458] */
    (xdc_Char)0x74,  /* [4459] */
    (xdc_Char)0x20,  /* [4460] */
    (xdc_Char)0x69,  /* [4461] */
    (xdc_Char)0x6e,  /* [4462] */
    (xdc_Char)0x20,  /* [4463] */
    (xdc_Char)0x4b,  /* [4464] */
    (xdc_Char)0x65,  /* [4465] */
    (xdc_Char)0x72,  /* [4466] */
    (xdc_Char)0x6e,  /* [4467] */
    (xdc_Char)0x65,  /* [4468] */
    (xdc_Char)0x6c,  /* [4469] */
    (xdc_Char)0x20,  /* [4470] */
    (xdc_Char)0x61,  /* [4471] */
    (xdc_Char)0x64,  /* [4472] */
    (xdc_Char)0x64,  /* [4473] */
    (xdc_Char)0x72,  /* [4474] */
    (xdc_Char)0x65,  /* [4475] */
    (xdc_Char)0x73,  /* [4476] */
    (xdc_Char)0x73,  /* [4477] */
    (xdc_Char)0x20,  /* [4478] */
    (xdc_Char)0x73,  /* [4479] */
    (xdc_Char)0x70,  /* [4480] */
    (xdc_Char)0x61,  /* [4481] */
    (xdc_Char)0x63,  /* [4482] */
    (xdc_Char)0x65,  /* [4483] */
    (xdc_Char)0x2e,  /* [4484] */
    (xdc_Char)0x0,  /* [4485] */
    (xdc_Char)0x72,  /* [4486] */
    (xdc_Char)0x65,  /* [4487] */
    (xdc_Char)0x71,  /* [4488] */
    (xdc_Char)0x75,  /* [4489] */
    (xdc_Char)0x65,  /* [4490] */
    (xdc_Char)0x73,  /* [4491] */
    (xdc_Char)0x74,  /* [4492] */
    (xdc_Char)0x65,  /* [4493] */
    (xdc_Char)0x64,  /* [4494] */
    (xdc_Char)0x20,  /* [4495] */
    (xdc_Char)0x73,  /* [4496] */
    (xdc_Char)0x69,  /* [4497] */
    (xdc_Char)0x7a,  /* [4498] */
    (xdc_Char)0x65,  /* [4499] */
    (xdc_Char)0x20,  /* [4500] */
    (xdc_Char)0x69,  /* [4501] */
    (xdc_Char)0x73,  /* [4502] */
    (xdc_Char)0x20,  /* [4503] */
    (xdc_Char)0x74,  /* [4504] */
    (xdc_Char)0x6f,  /* [4505] */
    (xdc_Char)0x6f,  /* [4506] */
    (xdc_Char)0x20,  /* [4507] */
    (xdc_Char)0x62,  /* [4508] */
    (xdc_Char)0x69,  /* [4509] */
    (xdc_Char)0x67,  /* [4510] */
    (xdc_Char)0x3a,  /* [4511] */
    (xdc_Char)0x20,  /* [4512] */
    (xdc_Char)0x68,  /* [4513] */
    (xdc_Char)0x61,  /* [4514] */
    (xdc_Char)0x6e,  /* [4515] */
    (xdc_Char)0x64,  /* [4516] */
    (xdc_Char)0x6c,  /* [4517] */
    (xdc_Char)0x65,  /* [4518] */
    (xdc_Char)0x3d,  /* [4519] */
    (xdc_Char)0x30,  /* [4520] */
    (xdc_Char)0x78,  /* [4521] */
    (xdc_Char)0x25,  /* [4522] */
    (xdc_Char)0x78,  /* [4523] */
    (xdc_Char)0x2c,  /* [4524] */
    (xdc_Char)0x20,  /* [4525] */
    (xdc_Char)0x73,  /* [4526] */
    (xdc_Char)0x69,  /* [4527] */
    (xdc_Char)0x7a,  /* [4528] */
    (xdc_Char)0x65,  /* [4529] */
    (xdc_Char)0x3d,  /* [4530] */
    (xdc_Char)0x25,  /* [4531] */
    (xdc_Char)0x75,  /* [4532] */
    (xdc_Char)0x0,  /* [4533] */
    (xdc_Char)0x6f,  /* [4534] */
    (xdc_Char)0x75,  /* [4535] */
    (xdc_Char)0x74,  /* [4536] */
    (xdc_Char)0x20,  /* [4537] */
    (xdc_Char)0x6f,  /* [4538] */
    (xdc_Char)0x66,  /* [4539] */
    (xdc_Char)0x20,  /* [4540] */
    (xdc_Char)0x6d,  /* [4541] */
    (xdc_Char)0x65,  /* [4542] */
    (xdc_Char)0x6d,  /* [4543] */
    (xdc_Char)0x6f,  /* [4544] */
    (xdc_Char)0x72,  /* [4545] */
    (xdc_Char)0x79,  /* [4546] */
    (xdc_Char)0x3a,  /* [4547] */
    (xdc_Char)0x20,  /* [4548] */
    (xdc_Char)0x68,  /* [4549] */
    (xdc_Char)0x61,  /* [4550] */
    (xdc_Char)0x6e,  /* [4551] */
    (xdc_Char)0x64,  /* [4552] */
    (xdc_Char)0x6c,  /* [4553] */
    (xdc_Char)0x65,  /* [4554] */
    (xdc_Char)0x3d,  /* [4555] */
    (xdc_Char)0x30,  /* [4556] */
    (xdc_Char)0x78,  /* [4557] */
    (xdc_Char)0x25,  /* [4558] */
    (xdc_Char)0x78,  /* [4559] */
    (xdc_Char)0x2c,  /* [4560] */
    (xdc_Char)0x20,  /* [4561] */
    (xdc_Char)0x73,  /* [4562] */
    (xdc_Char)0x69,  /* [4563] */
    (xdc_Char)0x7a,  /* [4564] */
    (xdc_Char)0x65,  /* [4565] */
    (xdc_Char)0x3d,  /* [4566] */
    (xdc_Char)0x25,  /* [4567] */
    (xdc_Char)0x75,  /* [4568] */
    (xdc_Char)0x0,  /* [4569] */
    (xdc_Char)0x45,  /* [4570] */
    (xdc_Char)0x5f,  /* [4571] */
    (xdc_Char)0x61,  /* [4572] */
    (xdc_Char)0x6c,  /* [4573] */
    (xdc_Char)0x72,  /* [4574] */
    (xdc_Char)0x65,  /* [4575] */
    (xdc_Char)0x61,  /* [4576] */
    (xdc_Char)0x64,  /* [4577] */
    (xdc_Char)0x79,  /* [4578] */
    (xdc_Char)0x44,  /* [4579] */
    (xdc_Char)0x65,  /* [4580] */
    (xdc_Char)0x66,  /* [4581] */
    (xdc_Char)0x69,  /* [4582] */
    (xdc_Char)0x6e,  /* [4583] */
    (xdc_Char)0x65,  /* [4584] */
    (xdc_Char)0x64,  /* [4585] */
    (xdc_Char)0x3a,  /* [4586] */
    (xdc_Char)0x20,  /* [4587] */
    (xdc_Char)0x48,  /* [4588] */
    (xdc_Char)0x77,  /* [4589] */
    (xdc_Char)0x69,  /* [4590] */
    (xdc_Char)0x20,  /* [4591] */
    (xdc_Char)0x61,  /* [4592] */
    (xdc_Char)0x6c,  /* [4593] */
    (xdc_Char)0x72,  /* [4594] */
    (xdc_Char)0x65,  /* [4595] */
    (xdc_Char)0x61,  /* [4596] */
    (xdc_Char)0x64,  /* [4597] */
    (xdc_Char)0x79,  /* [4598] */
    (xdc_Char)0x20,  /* [4599] */
    (xdc_Char)0x64,  /* [4600] */
    (xdc_Char)0x65,  /* [4601] */
    (xdc_Char)0x66,  /* [4602] */
    (xdc_Char)0x69,  /* [4603] */
    (xdc_Char)0x6e,  /* [4604] */
    (xdc_Char)0x65,  /* [4605] */
    (xdc_Char)0x64,  /* [4606] */
    (xdc_Char)0x3a,  /* [4607] */
    (xdc_Char)0x20,  /* [4608] */
    (xdc_Char)0x69,  /* [4609] */
    (xdc_Char)0x6e,  /* [4610] */
    (xdc_Char)0x74,  /* [4611] */
    (xdc_Char)0x72,  /* [4612] */
    (xdc_Char)0x23,  /* [4613] */
    (xdc_Char)0x20,  /* [4614] */
    (xdc_Char)0x25,  /* [4615] */
    (xdc_Char)0x64,  /* [4616] */
    (xdc_Char)0x0,  /* [4617] */
    (xdc_Char)0x45,  /* [4618] */
    (xdc_Char)0x5f,  /* [4619] */
    (xdc_Char)0x62,  /* [4620] */
    (xdc_Char)0x61,  /* [4621] */
    (xdc_Char)0x64,  /* [4622] */
    (xdc_Char)0x49,  /* [4623] */
    (xdc_Char)0x6e,  /* [4624] */
    (xdc_Char)0x74,  /* [4625] */
    (xdc_Char)0x4e,  /* [4626] */
    (xdc_Char)0x75,  /* [4627] */
    (xdc_Char)0x6d,  /* [4628] */
    (xdc_Char)0x2c,  /* [4629] */
    (xdc_Char)0x20,  /* [4630] */
    (xdc_Char)0x69,  /* [4631] */
    (xdc_Char)0x6e,  /* [4632] */
    (xdc_Char)0x74,  /* [4633] */
    (xdc_Char)0x6e,  /* [4634] */
    (xdc_Char)0x75,  /* [4635] */
    (xdc_Char)0x6d,  /* [4636] */
    (xdc_Char)0x3a,  /* [4637] */
    (xdc_Char)0x20,  /* [4638] */
    (xdc_Char)0x25,  /* [4639] */
    (xdc_Char)0x64,  /* [4640] */
    (xdc_Char)0x20,  /* [4641] */
    (xdc_Char)0x69,  /* [4642] */
    (xdc_Char)0x73,  /* [4643] */
    (xdc_Char)0x20,  /* [4644] */
    (xdc_Char)0x6f,  /* [4645] */
    (xdc_Char)0x75,  /* [4646] */
    (xdc_Char)0x74,  /* [4647] */
    (xdc_Char)0x20,  /* [4648] */
    (xdc_Char)0x6f,  /* [4649] */
    (xdc_Char)0x66,  /* [4650] */
    (xdc_Char)0x20,  /* [4651] */
    (xdc_Char)0x72,  /* [4652] */
    (xdc_Char)0x61,  /* [4653] */
    (xdc_Char)0x6e,  /* [4654] */
    (xdc_Char)0x67,  /* [4655] */
    (xdc_Char)0x65,  /* [4656] */
    (xdc_Char)0x0,  /* [4657] */
    (xdc_Char)0x45,  /* [4658] */
    (xdc_Char)0x5f,  /* [4659] */
    (xdc_Char)0x75,  /* [4660] */
    (xdc_Char)0x6e,  /* [4661] */
    (xdc_Char)0x64,  /* [4662] */
    (xdc_Char)0x65,  /* [4663] */
    (xdc_Char)0x66,  /* [4664] */
    (xdc_Char)0x69,  /* [4665] */
    (xdc_Char)0x6e,  /* [4666] */
    (xdc_Char)0x65,  /* [4667] */
    (xdc_Char)0x64,  /* [4668] */
    (xdc_Char)0x3a,  /* [4669] */
    (xdc_Char)0x20,  /* [4670] */
    (xdc_Char)0x48,  /* [4671] */
    (xdc_Char)0x77,  /* [4672] */
    (xdc_Char)0x69,  /* [4673] */
    (xdc_Char)0x20,  /* [4674] */
    (xdc_Char)0x75,  /* [4675] */
    (xdc_Char)0x6e,  /* [4676] */
    (xdc_Char)0x64,  /* [4677] */
    (xdc_Char)0x65,  /* [4678] */
    (xdc_Char)0x66,  /* [4679] */
    (xdc_Char)0x69,  /* [4680] */
    (xdc_Char)0x6e,  /* [4681] */
    (xdc_Char)0x65,  /* [4682] */
    (xdc_Char)0x64,  /* [4683] */
    (xdc_Char)0x2c,  /* [4684] */
    (xdc_Char)0x20,  /* [4685] */
    (xdc_Char)0x69,  /* [4686] */
    (xdc_Char)0x6e,  /* [4687] */
    (xdc_Char)0x74,  /* [4688] */
    (xdc_Char)0x6e,  /* [4689] */
    (xdc_Char)0x75,  /* [4690] */
    (xdc_Char)0x6d,  /* [4691] */
    (xdc_Char)0x3a,  /* [4692] */
    (xdc_Char)0x20,  /* [4693] */
    (xdc_Char)0x25,  /* [4694] */
    (xdc_Char)0x64,  /* [4695] */
    (xdc_Char)0x0,  /* [4696] */
    (xdc_Char)0x45,  /* [4697] */
    (xdc_Char)0x5f,  /* [4698] */
    (xdc_Char)0x75,  /* [4699] */
    (xdc_Char)0x6e,  /* [4700] */
    (xdc_Char)0x73,  /* [4701] */
    (xdc_Char)0x75,  /* [4702] */
    (xdc_Char)0x70,  /* [4703] */
    (xdc_Char)0x70,  /* [4704] */
    (xdc_Char)0x6f,  /* [4705] */
    (xdc_Char)0x72,  /* [4706] */
    (xdc_Char)0x74,  /* [4707] */
    (xdc_Char)0x65,  /* [4708] */
    (xdc_Char)0x64,  /* [4709] */
    (xdc_Char)0x4d,  /* [4710] */
    (xdc_Char)0x61,  /* [4711] */
    (xdc_Char)0x73,  /* [4712] */
    (xdc_Char)0x6b,  /* [4713] */
    (xdc_Char)0x69,  /* [4714] */
    (xdc_Char)0x6e,  /* [4715] */
    (xdc_Char)0x67,  /* [4716] */
    (xdc_Char)0x4f,  /* [4717] */
    (xdc_Char)0x70,  /* [4718] */
    (xdc_Char)0x74,  /* [4719] */
    (xdc_Char)0x69,  /* [4720] */
    (xdc_Char)0x6f,  /* [4721] */
    (xdc_Char)0x6e,  /* [4722] */
    (xdc_Char)0x3a,  /* [4723] */
    (xdc_Char)0x20,  /* [4724] */
    (xdc_Char)0x55,  /* [4725] */
    (xdc_Char)0x6e,  /* [4726] */
    (xdc_Char)0x73,  /* [4727] */
    (xdc_Char)0x75,  /* [4728] */
    (xdc_Char)0x70,  /* [4729] */
    (xdc_Char)0x70,  /* [4730] */
    (xdc_Char)0x6f,  /* [4731] */
    (xdc_Char)0x72,  /* [4732] */
    (xdc_Char)0x74,  /* [4733] */
    (xdc_Char)0x65,  /* [4734] */
    (xdc_Char)0x64,  /* [4735] */
    (xdc_Char)0x20,  /* [4736] */
    (xdc_Char)0x6d,  /* [4737] */
    (xdc_Char)0x61,  /* [4738] */
    (xdc_Char)0x73,  /* [4739] */
    (xdc_Char)0x6b,  /* [4740] */
    (xdc_Char)0x69,  /* [4741] */
    (xdc_Char)0x6e,  /* [4742] */
    (xdc_Char)0x67,  /* [4743] */
    (xdc_Char)0x20,  /* [4744] */
    (xdc_Char)0x6f,  /* [4745] */
    (xdc_Char)0x70,  /* [4746] */
    (xdc_Char)0x74,  /* [4747] */
    (xdc_Char)0x69,  /* [4748] */
    (xdc_Char)0x6f,  /* [4749] */
    (xdc_Char)0x6e,  /* [4750] */
    (xdc_Char)0x20,  /* [4751] */
    (xdc_Char)0x70,  /* [4752] */
    (xdc_Char)0x61,  /* [4753] */
    (xdc_Char)0x73,  /* [4754] */
    (xdc_Char)0x73,  /* [4755] */
    (xdc_Char)0x65,  /* [4756] */
    (xdc_Char)0x64,  /* [4757] */
    (xdc_Char)0x2e,  /* [4758] */
    (xdc_Char)0x0,  /* [4759] */
    (xdc_Char)0x45,  /* [4760] */
    (xdc_Char)0x5f,  /* [4761] */
    (xdc_Char)0x70,  /* [4762] */
    (xdc_Char)0x68,  /* [4763] */
    (xdc_Char)0x61,  /* [4764] */
    (xdc_Char)0x6e,  /* [4765] */
    (xdc_Char)0x74,  /* [4766] */
    (xdc_Char)0x6f,  /* [4767] */
    (xdc_Char)0x6d,  /* [4768] */
    (xdc_Char)0x49,  /* [4769] */
    (xdc_Char)0x6e,  /* [4770] */
    (xdc_Char)0x74,  /* [4771] */
    (xdc_Char)0x65,  /* [4772] */
    (xdc_Char)0x72,  /* [4773] */
    (xdc_Char)0x72,  /* [4774] */
    (xdc_Char)0x75,  /* [4775] */
    (xdc_Char)0x70,  /* [4776] */
    (xdc_Char)0x74,  /* [4777] */
    (xdc_Char)0x3a,  /* [4778] */
    (xdc_Char)0x20,  /* [4779] */
    (xdc_Char)0x41,  /* [4780] */
    (xdc_Char)0x20,  /* [4781] */
    (xdc_Char)0x70,  /* [4782] */
    (xdc_Char)0x68,  /* [4783] */
    (xdc_Char)0x61,  /* [4784] */
    (xdc_Char)0x6e,  /* [4785] */
    (xdc_Char)0x74,  /* [4786] */
    (xdc_Char)0x6f,  /* [4787] */
    (xdc_Char)0x6d,  /* [4788] */
    (xdc_Char)0x20,  /* [4789] */
    (xdc_Char)0x69,  /* [4790] */
    (xdc_Char)0x6e,  /* [4791] */
    (xdc_Char)0x74,  /* [4792] */
    (xdc_Char)0x65,  /* [4793] */
    (xdc_Char)0x72,  /* [4794] */
    (xdc_Char)0x72,  /* [4795] */
    (xdc_Char)0x75,  /* [4796] */
    (xdc_Char)0x70,  /* [4797] */
    (xdc_Char)0x74,  /* [4798] */
    (xdc_Char)0x20,  /* [4799] */
    (xdc_Char)0x68,  /* [4800] */
    (xdc_Char)0x61,  /* [4801] */
    (xdc_Char)0x73,  /* [4802] */
    (xdc_Char)0x20,  /* [4803] */
    (xdc_Char)0x6f,  /* [4804] */
    (xdc_Char)0x63,  /* [4805] */
    (xdc_Char)0x63,  /* [4806] */
    (xdc_Char)0x75,  /* [4807] */
    (xdc_Char)0x72,  /* [4808] */
    (xdc_Char)0x72,  /* [4809] */
    (xdc_Char)0x65,  /* [4810] */
    (xdc_Char)0x64,  /* [4811] */
    (xdc_Char)0x2e,  /* [4812] */
    (xdc_Char)0x0,  /* [4813] */
    (xdc_Char)0x45,  /* [4814] */
    (xdc_Char)0x5f,  /* [4815] */
    (xdc_Char)0x73,  /* [4816] */
    (xdc_Char)0x77,  /* [4817] */
    (xdc_Char)0x69,  /* [4818] */
    (xdc_Char)0x3a,  /* [4819] */
    (xdc_Char)0x20,  /* [4820] */
    (xdc_Char)0x70,  /* [4821] */
    (xdc_Char)0x63,  /* [4822] */
    (xdc_Char)0x20,  /* [4823] */
    (xdc_Char)0x3d,  /* [4824] */
    (xdc_Char)0x20,  /* [4825] */
    (xdc_Char)0x30,  /* [4826] */
    (xdc_Char)0x78,  /* [4827] */
    (xdc_Char)0x25,  /* [4828] */
    (xdc_Char)0x30,  /* [4829] */
    (xdc_Char)0x38,  /* [4830] */
    (xdc_Char)0x78,  /* [4831] */
    (xdc_Char)0x2c,  /* [4832] */
    (xdc_Char)0x20,  /* [4833] */
    (xdc_Char)0x6c,  /* [4834] */
    (xdc_Char)0x72,  /* [4835] */
    (xdc_Char)0x20,  /* [4836] */
    (xdc_Char)0x3d,  /* [4837] */
    (xdc_Char)0x20,  /* [4838] */
    (xdc_Char)0x30,  /* [4839] */
    (xdc_Char)0x78,  /* [4840] */
    (xdc_Char)0x25,  /* [4841] */
    (xdc_Char)0x30,  /* [4842] */
    (xdc_Char)0x38,  /* [4843] */
    (xdc_Char)0x78,  /* [4844] */
    (xdc_Char)0x2e,  /* [4845] */
    (xdc_Char)0x0,  /* [4846] */
    (xdc_Char)0x45,  /* [4847] */
    (xdc_Char)0x5f,  /* [4848] */
    (xdc_Char)0x70,  /* [4849] */
    (xdc_Char)0x72,  /* [4850] */
    (xdc_Char)0x65,  /* [4851] */
    (xdc_Char)0x66,  /* [4852] */
    (xdc_Char)0x65,  /* [4853] */
    (xdc_Char)0x74,  /* [4854] */
    (xdc_Char)0x63,  /* [4855] */
    (xdc_Char)0x68,  /* [4856] */
    (xdc_Char)0x41,  /* [4857] */
    (xdc_Char)0x62,  /* [4858] */
    (xdc_Char)0x6f,  /* [4859] */
    (xdc_Char)0x72,  /* [4860] */
    (xdc_Char)0x74,  /* [4861] */
    (xdc_Char)0x3a,  /* [4862] */
    (xdc_Char)0x20,  /* [4863] */
    (xdc_Char)0x70,  /* [4864] */
    (xdc_Char)0x63,  /* [4865] */
    (xdc_Char)0x20,  /* [4866] */
    (xdc_Char)0x3d,  /* [4867] */
    (xdc_Char)0x20,  /* [4868] */
    (xdc_Char)0x30,  /* [4869] */
    (xdc_Char)0x78,  /* [4870] */
    (xdc_Char)0x25,  /* [4871] */
    (xdc_Char)0x30,  /* [4872] */
    (xdc_Char)0x38,  /* [4873] */
    (xdc_Char)0x78,  /* [4874] */
    (xdc_Char)0x2c,  /* [4875] */
    (xdc_Char)0x20,  /* [4876] */
    (xdc_Char)0x6c,  /* [4877] */
    (xdc_Char)0x72,  /* [4878] */
    (xdc_Char)0x20,  /* [4879] */
    (xdc_Char)0x3d,  /* [4880] */
    (xdc_Char)0x20,  /* [4881] */
    (xdc_Char)0x30,  /* [4882] */
    (xdc_Char)0x78,  /* [4883] */
    (xdc_Char)0x25,  /* [4884] */
    (xdc_Char)0x30,  /* [4885] */
    (xdc_Char)0x38,  /* [4886] */
    (xdc_Char)0x78,  /* [4887] */
    (xdc_Char)0x2e,  /* [4888] */
    (xdc_Char)0x0,  /* [4889] */
    (xdc_Char)0x45,  /* [4890] */
    (xdc_Char)0x5f,  /* [4891] */
    (xdc_Char)0x64,  /* [4892] */
    (xdc_Char)0x61,  /* [4893] */
    (xdc_Char)0x74,  /* [4894] */
    (xdc_Char)0x61,  /* [4895] */
    (xdc_Char)0x41,  /* [4896] */
    (xdc_Char)0x62,  /* [4897] */
    (xdc_Char)0x6f,  /* [4898] */
    (xdc_Char)0x72,  /* [4899] */
    (xdc_Char)0x74,  /* [4900] */
    (xdc_Char)0x3a,  /* [4901] */
    (xdc_Char)0x20,  /* [4902] */
    (xdc_Char)0x70,  /* [4903] */
    (xdc_Char)0x63,  /* [4904] */
    (xdc_Char)0x20,  /* [4905] */
    (xdc_Char)0x3d,  /* [4906] */
    (xdc_Char)0x20,  /* [4907] */
    (xdc_Char)0x30,  /* [4908] */
    (xdc_Char)0x78,  /* [4909] */
    (xdc_Char)0x25,  /* [4910] */
    (xdc_Char)0x30,  /* [4911] */
    (xdc_Char)0x38,  /* [4912] */
    (xdc_Char)0x78,  /* [4913] */
    (xdc_Char)0x2c,  /* [4914] */
    (xdc_Char)0x20,  /* [4915] */
    (xdc_Char)0x6c,  /* [4916] */
    (xdc_Char)0x72,  /* [4917] */
    (xdc_Char)0x20,  /* [4918] */
    (xdc_Char)0x3d,  /* [4919] */
    (xdc_Char)0x20,  /* [4920] */
    (xdc_Char)0x30,  /* [4921] */
    (xdc_Char)0x78,  /* [4922] */
    (xdc_Char)0x25,  /* [4923] */
    (xdc_Char)0x30,  /* [4924] */
    (xdc_Char)0x38,  /* [4925] */
    (xdc_Char)0x78,  /* [4926] */
    (xdc_Char)0x2e,  /* [4927] */
    (xdc_Char)0x0,  /* [4928] */
    (xdc_Char)0x45,  /* [4929] */
    (xdc_Char)0x5f,  /* [4930] */
    (xdc_Char)0x75,  /* [4931] */
    (xdc_Char)0x6e,  /* [4932] */
    (xdc_Char)0x64,  /* [4933] */
    (xdc_Char)0x65,  /* [4934] */
    (xdc_Char)0x66,  /* [4935] */
    (xdc_Char)0x69,  /* [4936] */
    (xdc_Char)0x6e,  /* [4937] */
    (xdc_Char)0x65,  /* [4938] */
    (xdc_Char)0x64,  /* [4939] */
    (xdc_Char)0x49,  /* [4940] */
    (xdc_Char)0x6e,  /* [4941] */
    (xdc_Char)0x73,  /* [4942] */
    (xdc_Char)0x74,  /* [4943] */
    (xdc_Char)0x72,  /* [4944] */
    (xdc_Char)0x75,  /* [4945] */
    (xdc_Char)0x63,  /* [4946] */
    (xdc_Char)0x74,  /* [4947] */
    (xdc_Char)0x69,  /* [4948] */
    (xdc_Char)0x6f,  /* [4949] */
    (xdc_Char)0x6e,  /* [4950] */
    (xdc_Char)0x3a,  /* [4951] */
    (xdc_Char)0x20,  /* [4952] */
    (xdc_Char)0x70,  /* [4953] */
    (xdc_Char)0x63,  /* [4954] */
    (xdc_Char)0x20,  /* [4955] */
    (xdc_Char)0x3d,  /* [4956] */
    (xdc_Char)0x20,  /* [4957] */
    (xdc_Char)0x30,  /* [4958] */
    (xdc_Char)0x78,  /* [4959] */
    (xdc_Char)0x25,  /* [4960] */
    (xdc_Char)0x30,  /* [4961] */
    (xdc_Char)0x38,  /* [4962] */
    (xdc_Char)0x78,  /* [4963] */
    (xdc_Char)0x2c,  /* [4964] */
    (xdc_Char)0x20,  /* [4965] */
    (xdc_Char)0x6c,  /* [4966] */
    (xdc_Char)0x72,  /* [4967] */
    (xdc_Char)0x20,  /* [4968] */
    (xdc_Char)0x3d,  /* [4969] */
    (xdc_Char)0x20,  /* [4970] */
    (xdc_Char)0x30,  /* [4971] */
    (xdc_Char)0x78,  /* [4972] */
    (xdc_Char)0x25,  /* [4973] */
    (xdc_Char)0x30,  /* [4974] */
    (xdc_Char)0x38,  /* [4975] */
    (xdc_Char)0x78,  /* [4976] */
    (xdc_Char)0x2e,  /* [4977] */
    (xdc_Char)0x0,  /* [4978] */
    (xdc_Char)0x45,  /* [4979] */
    (xdc_Char)0x5f,  /* [4980] */
    (xdc_Char)0x70,  /* [4981] */
    (xdc_Char)0x72,  /* [4982] */
    (xdc_Char)0x69,  /* [4983] */
    (xdc_Char)0x6f,  /* [4984] */
    (xdc_Char)0x72,  /* [4985] */
    (xdc_Char)0x69,  /* [4986] */
    (xdc_Char)0x74,  /* [4987] */
    (xdc_Char)0x79,  /* [4988] */
    (xdc_Char)0x3a,  /* [4989] */
    (xdc_Char)0x20,  /* [4990] */
    (xdc_Char)0x54,  /* [4991] */
    (xdc_Char)0x68,  /* [4992] */
    (xdc_Char)0x72,  /* [4993] */
    (xdc_Char)0x65,  /* [4994] */
    (xdc_Char)0x61,  /* [4995] */
    (xdc_Char)0x64,  /* [4996] */
    (xdc_Char)0x20,  /* [4997] */
    (xdc_Char)0x70,  /* [4998] */
    (xdc_Char)0x72,  /* [4999] */
    (xdc_Char)0x69,  /* [5000] */
    (xdc_Char)0x6f,  /* [5001] */
    (xdc_Char)0x72,  /* [5002] */
    (xdc_Char)0x69,  /* [5003] */
    (xdc_Char)0x74,  /* [5004] */
    (xdc_Char)0x79,  /* [5005] */
    (xdc_Char)0x20,  /* [5006] */
    (xdc_Char)0x69,  /* [5007] */
    (xdc_Char)0x73,  /* [5008] */
    (xdc_Char)0x20,  /* [5009] */
    (xdc_Char)0x69,  /* [5010] */
    (xdc_Char)0x6e,  /* [5011] */
    (xdc_Char)0x76,  /* [5012] */
    (xdc_Char)0x61,  /* [5013] */
    (xdc_Char)0x6c,  /* [5014] */
    (xdc_Char)0x69,  /* [5015] */
    (xdc_Char)0x64,  /* [5016] */
    (xdc_Char)0x20,  /* [5017] */
    (xdc_Char)0x25,  /* [5018] */
    (xdc_Char)0x64,  /* [5019] */
    (xdc_Char)0x0,  /* [5020] */
    (xdc_Char)0x45,  /* [5021] */
    (xdc_Char)0x5f,  /* [5022] */
    (xdc_Char)0x73,  /* [5023] */
    (xdc_Char)0x74,  /* [5024] */
    (xdc_Char)0x61,  /* [5025] */
    (xdc_Char)0x63,  /* [5026] */
    (xdc_Char)0x6b,  /* [5027] */
    (xdc_Char)0x4f,  /* [5028] */
    (xdc_Char)0x76,  /* [5029] */
    (xdc_Char)0x65,  /* [5030] */
    (xdc_Char)0x72,  /* [5031] */
    (xdc_Char)0x66,  /* [5032] */
    (xdc_Char)0x6c,  /* [5033] */
    (xdc_Char)0x6f,  /* [5034] */
    (xdc_Char)0x77,  /* [5035] */
    (xdc_Char)0x3a,  /* [5036] */
    (xdc_Char)0x20,  /* [5037] */
    (xdc_Char)0x49,  /* [5038] */
    (xdc_Char)0x53,  /* [5039] */
    (xdc_Char)0x52,  /* [5040] */
    (xdc_Char)0x20,  /* [5041] */
    (xdc_Char)0x73,  /* [5042] */
    (xdc_Char)0x74,  /* [5043] */
    (xdc_Char)0x61,  /* [5044] */
    (xdc_Char)0x63,  /* [5045] */
    (xdc_Char)0x6b,  /* [5046] */
    (xdc_Char)0x20,  /* [5047] */
    (xdc_Char)0x6f,  /* [5048] */
    (xdc_Char)0x76,  /* [5049] */
    (xdc_Char)0x65,  /* [5050] */
    (xdc_Char)0x72,  /* [5051] */
    (xdc_Char)0x66,  /* [5052] */
    (xdc_Char)0x6c,  /* [5053] */
    (xdc_Char)0x6f,  /* [5054] */
    (xdc_Char)0x77,  /* [5055] */
    (xdc_Char)0x2e,  /* [5056] */
    (xdc_Char)0x0,  /* [5057] */
    (xdc_Char)0x45,  /* [5058] */
    (xdc_Char)0x5f,  /* [5059] */
    (xdc_Char)0x6d,  /* [5060] */
    (xdc_Char)0x69,  /* [5061] */
    (xdc_Char)0x73,  /* [5062] */
    (xdc_Char)0x6d,  /* [5063] */
    (xdc_Char)0x61,  /* [5064] */
    (xdc_Char)0x74,  /* [5065] */
    (xdc_Char)0x63,  /* [5066] */
    (xdc_Char)0x68,  /* [5067] */
    (xdc_Char)0x65,  /* [5068] */
    (xdc_Char)0x64,  /* [5069] */
    (xdc_Char)0x49,  /* [5070] */
    (xdc_Char)0x64,  /* [5071] */
    (xdc_Char)0x73,  /* [5072] */
    (xdc_Char)0x3a,  /* [5073] */
    (xdc_Char)0x20,  /* [5074] */
    (xdc_Char)0x43,  /* [5075] */
    (xdc_Char)0x6f,  /* [5076] */
    (xdc_Char)0x72,  /* [5077] */
    (xdc_Char)0x65,  /* [5078] */
    (xdc_Char)0x5f,  /* [5079] */
    (xdc_Char)0x49,  /* [5080] */
    (xdc_Char)0x64,  /* [5081] */
    (xdc_Char)0x3a,  /* [5082] */
    (xdc_Char)0x20,  /* [5083] */
    (xdc_Char)0x25,  /* [5084] */
    (xdc_Char)0x64,  /* [5085] */
    (xdc_Char)0x20,  /* [5086] */
    (xdc_Char)0x64,  /* [5087] */
    (xdc_Char)0x6f,  /* [5088] */
    (xdc_Char)0x65,  /* [5089] */
    (xdc_Char)0x73,  /* [5090] */
    (xdc_Char)0x20,  /* [5091] */
    (xdc_Char)0x6e,  /* [5092] */
    (xdc_Char)0x6f,  /* [5093] */
    (xdc_Char)0x74,  /* [5094] */
    (xdc_Char)0x20,  /* [5095] */
    (xdc_Char)0x6d,  /* [5096] */
    (xdc_Char)0x61,  /* [5097] */
    (xdc_Char)0x74,  /* [5098] */
    (xdc_Char)0x63,  /* [5099] */
    (xdc_Char)0x68,  /* [5100] */
    (xdc_Char)0x20,  /* [5101] */
    (xdc_Char)0x68,  /* [5102] */
    (xdc_Char)0x61,  /* [5103] */
    (xdc_Char)0x72,  /* [5104] */
    (xdc_Char)0x64,  /* [5105] */
    (xdc_Char)0x77,  /* [5106] */
    (xdc_Char)0x61,  /* [5107] */
    (xdc_Char)0x72,  /* [5108] */
    (xdc_Char)0x65,  /* [5109] */
    (xdc_Char)0x20,  /* [5110] */
    (xdc_Char)0x63,  /* [5111] */
    (xdc_Char)0x6f,  /* [5112] */
    (xdc_Char)0x72,  /* [5113] */
    (xdc_Char)0x65,  /* [5114] */
    (xdc_Char)0x20,  /* [5115] */
    (xdc_Char)0x49,  /* [5116] */
    (xdc_Char)0x64,  /* [5117] */
    (xdc_Char)0x3a,  /* [5118] */
    (xdc_Char)0x20,  /* [5119] */
    (xdc_Char)0x25,  /* [5120] */
    (xdc_Char)0x64,  /* [5121] */
    (xdc_Char)0x0,  /* [5122] */
    (xdc_Char)0x45,  /* [5123] */
    (xdc_Char)0x5f,  /* [5124] */
    (xdc_Char)0x69,  /* [5125] */
    (xdc_Char)0x6e,  /* [5126] */
    (xdc_Char)0x76,  /* [5127] */
    (xdc_Char)0x61,  /* [5128] */
    (xdc_Char)0x6c,  /* [5129] */
    (xdc_Char)0x69,  /* [5130] */
    (xdc_Char)0x64,  /* [5131] */
    (xdc_Char)0x54,  /* [5132] */
    (xdc_Char)0x69,  /* [5133] */
    (xdc_Char)0x6d,  /* [5134] */
    (xdc_Char)0x65,  /* [5135] */
    (xdc_Char)0x72,  /* [5136] */
    (xdc_Char)0x3a,  /* [5137] */
    (xdc_Char)0x20,  /* [5138] */
    (xdc_Char)0x49,  /* [5139] */
    (xdc_Char)0x6e,  /* [5140] */
    (xdc_Char)0x76,  /* [5141] */
    (xdc_Char)0x61,  /* [5142] */
    (xdc_Char)0x6c,  /* [5143] */
    (xdc_Char)0x69,  /* [5144] */
    (xdc_Char)0x64,  /* [5145] */
    (xdc_Char)0x20,  /* [5146] */
    (xdc_Char)0x54,  /* [5147] */
    (xdc_Char)0x69,  /* [5148] */
    (xdc_Char)0x6d,  /* [5149] */
    (xdc_Char)0x65,  /* [5150] */
    (xdc_Char)0x72,  /* [5151] */
    (xdc_Char)0x20,  /* [5152] */
    (xdc_Char)0x49,  /* [5153] */
    (xdc_Char)0x64,  /* [5154] */
    (xdc_Char)0x20,  /* [5155] */
    (xdc_Char)0x25,  /* [5156] */
    (xdc_Char)0x64,  /* [5157] */
    (xdc_Char)0x0,  /* [5158] */
    (xdc_Char)0x45,  /* [5159] */
    (xdc_Char)0x5f,  /* [5160] */
    (xdc_Char)0x6e,  /* [5161] */
    (xdc_Char)0x6f,  /* [5162] */
    (xdc_Char)0x74,  /* [5163] */
    (xdc_Char)0x41,  /* [5164] */
    (xdc_Char)0x76,  /* [5165] */
    (xdc_Char)0x61,  /* [5166] */
    (xdc_Char)0x69,  /* [5167] */
    (xdc_Char)0x6c,  /* [5168] */
    (xdc_Char)0x61,  /* [5169] */
    (xdc_Char)0x62,  /* [5170] */
    (xdc_Char)0x6c,  /* [5171] */
    (xdc_Char)0x65,  /* [5172] */
    (xdc_Char)0x3a,  /* [5173] */
    (xdc_Char)0x20,  /* [5174] */
    (xdc_Char)0x54,  /* [5175] */
    (xdc_Char)0x69,  /* [5176] */
    (xdc_Char)0x6d,  /* [5177] */
    (xdc_Char)0x65,  /* [5178] */
    (xdc_Char)0x72,  /* [5179] */
    (xdc_Char)0x20,  /* [5180] */
    (xdc_Char)0x6e,  /* [5181] */
    (xdc_Char)0x6f,  /* [5182] */
    (xdc_Char)0x74,  /* [5183] */
    (xdc_Char)0x20,  /* [5184] */
    (xdc_Char)0x61,  /* [5185] */
    (xdc_Char)0x76,  /* [5186] */
    (xdc_Char)0x61,  /* [5187] */
    (xdc_Char)0x69,  /* [5188] */
    (xdc_Char)0x6c,  /* [5189] */
    (xdc_Char)0x61,  /* [5190] */
    (xdc_Char)0x62,  /* [5191] */
    (xdc_Char)0x6c,  /* [5192] */
    (xdc_Char)0x65,  /* [5193] */
    (xdc_Char)0x20,  /* [5194] */
    (xdc_Char)0x25,  /* [5195] */
    (xdc_Char)0x64,  /* [5196] */
    (xdc_Char)0x0,  /* [5197] */
    (xdc_Char)0x45,  /* [5198] */
    (xdc_Char)0x5f,  /* [5199] */
    (xdc_Char)0x49,  /* [5200] */
    (xdc_Char)0x6e,  /* [5201] */
    (xdc_Char)0x76,  /* [5202] */
    (xdc_Char)0x61,  /* [5203] */
    (xdc_Char)0x6c,  /* [5204] */
    (xdc_Char)0x69,  /* [5205] */
    (xdc_Char)0x64,  /* [5206] */
    (xdc_Char)0x4d,  /* [5207] */
    (xdc_Char)0x61,  /* [5208] */
    (xdc_Char)0x73,  /* [5209] */
    (xdc_Char)0x6b,  /* [5210] */
    (xdc_Char)0x3a,  /* [5211] */
    (xdc_Char)0x20,  /* [5212] */
    (xdc_Char)0x4d,  /* [5213] */
    (xdc_Char)0x61,  /* [5214] */
    (xdc_Char)0x73,  /* [5215] */
    (xdc_Char)0x6b,  /* [5216] */
    (xdc_Char)0x20,  /* [5217] */
    (xdc_Char)0x69,  /* [5218] */
    (xdc_Char)0x6e,  /* [5219] */
    (xdc_Char)0x20,  /* [5220] */
    (xdc_Char)0x68,  /* [5221] */
    (xdc_Char)0x77,  /* [5222] */
    (xdc_Char)0x69,  /* [5223] */
    (xdc_Char)0x50,  /* [5224] */
    (xdc_Char)0x61,  /* [5225] */
    (xdc_Char)0x72,  /* [5226] */
    (xdc_Char)0x61,  /* [5227] */
    (xdc_Char)0x6d,  /* [5228] */
    (xdc_Char)0x73,  /* [5229] */
    (xdc_Char)0x20,  /* [5230] */
    (xdc_Char)0x63,  /* [5231] */
    (xdc_Char)0x61,  /* [5232] */
    (xdc_Char)0x6e,  /* [5233] */
    (xdc_Char)0x6e,  /* [5234] */
    (xdc_Char)0x6f,  /* [5235] */
    (xdc_Char)0x74,  /* [5236] */
    (xdc_Char)0x20,  /* [5237] */
    (xdc_Char)0x65,  /* [5238] */
    (xdc_Char)0x6e,  /* [5239] */
    (xdc_Char)0x61,  /* [5240] */
    (xdc_Char)0x62,  /* [5241] */
    (xdc_Char)0x6c,  /* [5242] */
    (xdc_Char)0x65,  /* [5243] */
    (xdc_Char)0x20,  /* [5244] */
    (xdc_Char)0x73,  /* [5245] */
    (xdc_Char)0x65,  /* [5246] */
    (xdc_Char)0x6c,  /* [5247] */
    (xdc_Char)0x66,  /* [5248] */
    (xdc_Char)0x0,  /* [5249] */
    (xdc_Char)0x45,  /* [5250] */
    (xdc_Char)0x5f,  /* [5251] */
    (xdc_Char)0x63,  /* [5252] */
    (xdc_Char)0x61,  /* [5253] */
    (xdc_Char)0x6e,  /* [5254] */
    (xdc_Char)0x6e,  /* [5255] */
    (xdc_Char)0x6f,  /* [5256] */
    (xdc_Char)0x74,  /* [5257] */
    (xdc_Char)0x53,  /* [5258] */
    (xdc_Char)0x75,  /* [5259] */
    (xdc_Char)0x70,  /* [5260] */
    (xdc_Char)0x70,  /* [5261] */
    (xdc_Char)0x6f,  /* [5262] */
    (xdc_Char)0x72,  /* [5263] */
    (xdc_Char)0x74,  /* [5264] */
    (xdc_Char)0x3a,  /* [5265] */
    (xdc_Char)0x20,  /* [5266] */
    (xdc_Char)0x54,  /* [5267] */
    (xdc_Char)0x69,  /* [5268] */
    (xdc_Char)0x6d,  /* [5269] */
    (xdc_Char)0x65,  /* [5270] */
    (xdc_Char)0x72,  /* [5271] */
    (xdc_Char)0x20,  /* [5272] */
    (xdc_Char)0x63,  /* [5273] */
    (xdc_Char)0x61,  /* [5274] */
    (xdc_Char)0x6e,  /* [5275] */
    (xdc_Char)0x6e,  /* [5276] */
    (xdc_Char)0x6f,  /* [5277] */
    (xdc_Char)0x74,  /* [5278] */
    (xdc_Char)0x20,  /* [5279] */
    (xdc_Char)0x73,  /* [5280] */
    (xdc_Char)0x75,  /* [5281] */
    (xdc_Char)0x70,  /* [5282] */
    (xdc_Char)0x70,  /* [5283] */
    (xdc_Char)0x6f,  /* [5284] */
    (xdc_Char)0x72,  /* [5285] */
    (xdc_Char)0x74,  /* [5286] */
    (xdc_Char)0x20,  /* [5287] */
    (xdc_Char)0x72,  /* [5288] */
    (xdc_Char)0x65,  /* [5289] */
    (xdc_Char)0x71,  /* [5290] */
    (xdc_Char)0x75,  /* [5291] */
    (xdc_Char)0x65,  /* [5292] */
    (xdc_Char)0x73,  /* [5293] */
    (xdc_Char)0x74,  /* [5294] */
    (xdc_Char)0x65,  /* [5295] */
    (xdc_Char)0x64,  /* [5296] */
    (xdc_Char)0x20,  /* [5297] */
    (xdc_Char)0x70,  /* [5298] */
    (xdc_Char)0x65,  /* [5299] */
    (xdc_Char)0x72,  /* [5300] */
    (xdc_Char)0x69,  /* [5301] */
    (xdc_Char)0x6f,  /* [5302] */
    (xdc_Char)0x64,  /* [5303] */
    (xdc_Char)0x20,  /* [5304] */
    (xdc_Char)0x25,  /* [5305] */
    (xdc_Char)0x64,  /* [5306] */
    (xdc_Char)0x0,  /* [5307] */
    (xdc_Char)0x3c,  /* [5308] */
    (xdc_Char)0x2d,  /* [5309] */
    (xdc_Char)0x2d,  /* [5310] */
    (xdc_Char)0x20,  /* [5311] */
    (xdc_Char)0x63,  /* [5312] */
    (xdc_Char)0x6f,  /* [5313] */
    (xdc_Char)0x6e,  /* [5314] */
    (xdc_Char)0x73,  /* [5315] */
    (xdc_Char)0x74,  /* [5316] */
    (xdc_Char)0x72,  /* [5317] */
    (xdc_Char)0x75,  /* [5318] */
    (xdc_Char)0x63,  /* [5319] */
    (xdc_Char)0x74,  /* [5320] */
    (xdc_Char)0x3a,  /* [5321] */
    (xdc_Char)0x20,  /* [5322] */
    (xdc_Char)0x25,  /* [5323] */
    (xdc_Char)0x70,  /* [5324] */
    (xdc_Char)0x28,  /* [5325] */
    (xdc_Char)0x27,  /* [5326] */
    (xdc_Char)0x25,  /* [5327] */
    (xdc_Char)0x73,  /* [5328] */
    (xdc_Char)0x27,  /* [5329] */
    (xdc_Char)0x29,  /* [5330] */
    (xdc_Char)0x0,  /* [5331] */
    (xdc_Char)0x3c,  /* [5332] */
    (xdc_Char)0x2d,  /* [5333] */
    (xdc_Char)0x2d,  /* [5334] */
    (xdc_Char)0x20,  /* [5335] */
    (xdc_Char)0x63,  /* [5336] */
    (xdc_Char)0x72,  /* [5337] */
    (xdc_Char)0x65,  /* [5338] */
    (xdc_Char)0x61,  /* [5339] */
    (xdc_Char)0x74,  /* [5340] */
    (xdc_Char)0x65,  /* [5341] */
    (xdc_Char)0x3a,  /* [5342] */
    (xdc_Char)0x20,  /* [5343] */
    (xdc_Char)0x25,  /* [5344] */
    (xdc_Char)0x70,  /* [5345] */
    (xdc_Char)0x28,  /* [5346] */
    (xdc_Char)0x27,  /* [5347] */
    (xdc_Char)0x25,  /* [5348] */
    (xdc_Char)0x73,  /* [5349] */
    (xdc_Char)0x27,  /* [5350] */
    (xdc_Char)0x29,  /* [5351] */
    (xdc_Char)0x0,  /* [5352] */
    (xdc_Char)0x2d,  /* [5353] */
    (xdc_Char)0x2d,  /* [5354] */
    (xdc_Char)0x3e,  /* [5355] */
    (xdc_Char)0x20,  /* [5356] */
    (xdc_Char)0x64,  /* [5357] */
    (xdc_Char)0x65,  /* [5358] */
    (xdc_Char)0x73,  /* [5359] */
    (xdc_Char)0x74,  /* [5360] */
    (xdc_Char)0x72,  /* [5361] */
    (xdc_Char)0x75,  /* [5362] */
    (xdc_Char)0x63,  /* [5363] */
    (xdc_Char)0x74,  /* [5364] */
    (xdc_Char)0x3a,  /* [5365] */
    (xdc_Char)0x20,  /* [5366] */
    (xdc_Char)0x28,  /* [5367] */
    (xdc_Char)0x25,  /* [5368] */
    (xdc_Char)0x70,  /* [5369] */
    (xdc_Char)0x29,  /* [5370] */
    (xdc_Char)0x0,  /* [5371] */
    (xdc_Char)0x2d,  /* [5372] */
    (xdc_Char)0x2d,  /* [5373] */
    (xdc_Char)0x3e,  /* [5374] */
    (xdc_Char)0x20,  /* [5375] */
    (xdc_Char)0x64,  /* [5376] */
    (xdc_Char)0x65,  /* [5377] */
    (xdc_Char)0x6c,  /* [5378] */
    (xdc_Char)0x65,  /* [5379] */
    (xdc_Char)0x74,  /* [5380] */
    (xdc_Char)0x65,  /* [5381] */
    (xdc_Char)0x3a,  /* [5382] */
    (xdc_Char)0x20,  /* [5383] */
    (xdc_Char)0x28,  /* [5384] */
    (xdc_Char)0x25,  /* [5385] */
    (xdc_Char)0x70,  /* [5386] */
    (xdc_Char)0x29,  /* [5387] */
    (xdc_Char)0x0,  /* [5388] */
    (xdc_Char)0x45,  /* [5389] */
    (xdc_Char)0x52,  /* [5390] */
    (xdc_Char)0x52,  /* [5391] */
    (xdc_Char)0x4f,  /* [5392] */
    (xdc_Char)0x52,  /* [5393] */
    (xdc_Char)0x3a,  /* [5394] */
    (xdc_Char)0x20,  /* [5395] */
    (xdc_Char)0x25,  /* [5396] */
    (xdc_Char)0x24,  /* [5397] */
    (xdc_Char)0x46,  /* [5398] */
    (xdc_Char)0x25,  /* [5399] */
    (xdc_Char)0x24,  /* [5400] */
    (xdc_Char)0x53,  /* [5401] */
    (xdc_Char)0x0,  /* [5402] */
    (xdc_Char)0x57,  /* [5403] */
    (xdc_Char)0x41,  /* [5404] */
    (xdc_Char)0x52,  /* [5405] */
    (xdc_Char)0x4e,  /* [5406] */
    (xdc_Char)0x49,  /* [5407] */
    (xdc_Char)0x4e,  /* [5408] */
    (xdc_Char)0x47,  /* [5409] */
    (xdc_Char)0x3a,  /* [5410] */
    (xdc_Char)0x20,  /* [5411] */
    (xdc_Char)0x25,  /* [5412] */
    (xdc_Char)0x24,  /* [5413] */
    (xdc_Char)0x46,  /* [5414] */
    (xdc_Char)0x25,  /* [5415] */
    (xdc_Char)0x24,  /* [5416] */
    (xdc_Char)0x53,  /* [5417] */
    (xdc_Char)0x0,  /* [5418] */
    (xdc_Char)0x25,  /* [5419] */
    (xdc_Char)0x24,  /* [5420] */
    (xdc_Char)0x46,  /* [5421] */
    (xdc_Char)0x25,  /* [5422] */
    (xdc_Char)0x24,  /* [5423] */
    (xdc_Char)0x53,  /* [5424] */
    (xdc_Char)0x0,  /* [5425] */
    (xdc_Char)0x53,  /* [5426] */
    (xdc_Char)0x74,  /* [5427] */
    (xdc_Char)0x61,  /* [5428] */
    (xdc_Char)0x72,  /* [5429] */
    (xdc_Char)0x74,  /* [5430] */
    (xdc_Char)0x3a,  /* [5431] */
    (xdc_Char)0x20,  /* [5432] */
    (xdc_Char)0x25,  /* [5433] */
    (xdc_Char)0x24,  /* [5434] */
    (xdc_Char)0x53,  /* [5435] */
    (xdc_Char)0x0,  /* [5436] */
    (xdc_Char)0x53,  /* [5437] */
    (xdc_Char)0x74,  /* [5438] */
    (xdc_Char)0x6f,  /* [5439] */
    (xdc_Char)0x70,  /* [5440] */
    (xdc_Char)0x3a,  /* [5441] */
    (xdc_Char)0x20,  /* [5442] */
    (xdc_Char)0x25,  /* [5443] */
    (xdc_Char)0x24,  /* [5444] */
    (xdc_Char)0x53,  /* [5445] */
    (xdc_Char)0x0,  /* [5446] */
    (xdc_Char)0x53,  /* [5447] */
    (xdc_Char)0x74,  /* [5448] */
    (xdc_Char)0x61,  /* [5449] */
    (xdc_Char)0x72,  /* [5450] */
    (xdc_Char)0x74,  /* [5451] */
    (xdc_Char)0x49,  /* [5452] */
    (xdc_Char)0x6e,  /* [5453] */
    (xdc_Char)0x73,  /* [5454] */
    (xdc_Char)0x74,  /* [5455] */
    (xdc_Char)0x61,  /* [5456] */
    (xdc_Char)0x6e,  /* [5457] */
    (xdc_Char)0x63,  /* [5458] */
    (xdc_Char)0x65,  /* [5459] */
    (xdc_Char)0x3a,  /* [5460] */
    (xdc_Char)0x20,  /* [5461] */
    (xdc_Char)0x25,  /* [5462] */
    (xdc_Char)0x24,  /* [5463] */
    (xdc_Char)0x53,  /* [5464] */
    (xdc_Char)0x0,  /* [5465] */
    (xdc_Char)0x53,  /* [5466] */
    (xdc_Char)0x74,  /* [5467] */
    (xdc_Char)0x6f,  /* [5468] */
    (xdc_Char)0x70,  /* [5469] */
    (xdc_Char)0x49,  /* [5470] */
    (xdc_Char)0x6e,  /* [5471] */
    (xdc_Char)0x73,  /* [5472] */
    (xdc_Char)0x74,  /* [5473] */
    (xdc_Char)0x61,  /* [5474] */
    (xdc_Char)0x6e,  /* [5475] */
    (xdc_Char)0x63,  /* [5476] */
    (xdc_Char)0x65,  /* [5477] */
    (xdc_Char)0x3a,  /* [5478] */
    (xdc_Char)0x20,  /* [5479] */
    (xdc_Char)0x25,  /* [5480] */
    (xdc_Char)0x24,  /* [5481] */
    (xdc_Char)0x53,  /* [5482] */
    (xdc_Char)0x0,  /* [5483] */
    (xdc_Char)0x4c,  /* [5484] */
    (xdc_Char)0x57,  /* [5485] */
    (xdc_Char)0x5f,  /* [5486] */
    (xdc_Char)0x64,  /* [5487] */
    (xdc_Char)0x65,  /* [5488] */
    (xdc_Char)0x6c,  /* [5489] */
    (xdc_Char)0x61,  /* [5490] */
    (xdc_Char)0x79,  /* [5491] */
    (xdc_Char)0x65,  /* [5492] */
    (xdc_Char)0x64,  /* [5493] */
    (xdc_Char)0x3a,  /* [5494] */
    (xdc_Char)0x20,  /* [5495] */
    (xdc_Char)0x64,  /* [5496] */
    (xdc_Char)0x65,  /* [5497] */
    (xdc_Char)0x6c,  /* [5498] */
    (xdc_Char)0x61,  /* [5499] */
    (xdc_Char)0x79,  /* [5500] */
    (xdc_Char)0x3a,  /* [5501] */
    (xdc_Char)0x20,  /* [5502] */
    (xdc_Char)0x25,  /* [5503] */
    (xdc_Char)0x64,  /* [5504] */
    (xdc_Char)0x0,  /* [5505] */
    (xdc_Char)0x4c,  /* [5506] */
    (xdc_Char)0x4d,  /* [5507] */
    (xdc_Char)0x5f,  /* [5508] */
    (xdc_Char)0x74,  /* [5509] */
    (xdc_Char)0x69,  /* [5510] */
    (xdc_Char)0x63,  /* [5511] */
    (xdc_Char)0x6b,  /* [5512] */
    (xdc_Char)0x3a,  /* [5513] */
    (xdc_Char)0x20,  /* [5514] */
    (xdc_Char)0x74,  /* [5515] */
    (xdc_Char)0x69,  /* [5516] */
    (xdc_Char)0x63,  /* [5517] */
    (xdc_Char)0x6b,  /* [5518] */
    (xdc_Char)0x3a,  /* [5519] */
    (xdc_Char)0x20,  /* [5520] */
    (xdc_Char)0x25,  /* [5521] */
    (xdc_Char)0x64,  /* [5522] */
    (xdc_Char)0x0,  /* [5523] */
    (xdc_Char)0x4c,  /* [5524] */
    (xdc_Char)0x4d,  /* [5525] */
    (xdc_Char)0x5f,  /* [5526] */
    (xdc_Char)0x62,  /* [5527] */
    (xdc_Char)0x65,  /* [5528] */
    (xdc_Char)0x67,  /* [5529] */
    (xdc_Char)0x69,  /* [5530] */
    (xdc_Char)0x6e,  /* [5531] */
    (xdc_Char)0x3a,  /* [5532] */
    (xdc_Char)0x20,  /* [5533] */
    (xdc_Char)0x63,  /* [5534] */
    (xdc_Char)0x6c,  /* [5535] */
    (xdc_Char)0x6b,  /* [5536] */
    (xdc_Char)0x3a,  /* [5537] */
    (xdc_Char)0x20,  /* [5538] */
    (xdc_Char)0x30,  /* [5539] */
    (xdc_Char)0x78,  /* [5540] */
    (xdc_Char)0x25,  /* [5541] */
    (xdc_Char)0x78,  /* [5542] */
    (xdc_Char)0x2c,  /* [5543] */
    (xdc_Char)0x20,  /* [5544] */
    (xdc_Char)0x66,  /* [5545] */
    (xdc_Char)0x75,  /* [5546] */
    (xdc_Char)0x6e,  /* [5547] */
    (xdc_Char)0x63,  /* [5548] */
    (xdc_Char)0x3a,  /* [5549] */
    (xdc_Char)0x20,  /* [5550] */
    (xdc_Char)0x30,  /* [5551] */
    (xdc_Char)0x78,  /* [5552] */
    (xdc_Char)0x25,  /* [5553] */
    (xdc_Char)0x78,  /* [5554] */
    (xdc_Char)0x0,  /* [5555] */
    (xdc_Char)0x4c,  /* [5556] */
    (xdc_Char)0x4d,  /* [5557] */
    (xdc_Char)0x5f,  /* [5558] */
    (xdc_Char)0x70,  /* [5559] */
    (xdc_Char)0x6f,  /* [5560] */
    (xdc_Char)0x73,  /* [5561] */
    (xdc_Char)0x74,  /* [5562] */
    (xdc_Char)0x3a,  /* [5563] */
    (xdc_Char)0x20,  /* [5564] */
    (xdc_Char)0x65,  /* [5565] */
    (xdc_Char)0x76,  /* [5566] */
    (xdc_Char)0x65,  /* [5567] */
    (xdc_Char)0x6e,  /* [5568] */
    (xdc_Char)0x74,  /* [5569] */
    (xdc_Char)0x3a,  /* [5570] */
    (xdc_Char)0x20,  /* [5571] */
    (xdc_Char)0x30,  /* [5572] */
    (xdc_Char)0x78,  /* [5573] */
    (xdc_Char)0x25,  /* [5574] */
    (xdc_Char)0x78,  /* [5575] */
    (xdc_Char)0x2c,  /* [5576] */
    (xdc_Char)0x20,  /* [5577] */
    (xdc_Char)0x63,  /* [5578] */
    (xdc_Char)0x75,  /* [5579] */
    (xdc_Char)0x72,  /* [5580] */
    (xdc_Char)0x72,  /* [5581] */
    (xdc_Char)0x45,  /* [5582] */
    (xdc_Char)0x76,  /* [5583] */
    (xdc_Char)0x65,  /* [5584] */
    (xdc_Char)0x6e,  /* [5585] */
    (xdc_Char)0x74,  /* [5586] */
    (xdc_Char)0x73,  /* [5587] */
    (xdc_Char)0x3a,  /* [5588] */
    (xdc_Char)0x20,  /* [5589] */
    (xdc_Char)0x30,  /* [5590] */
    (xdc_Char)0x78,  /* [5591] */
    (xdc_Char)0x25,  /* [5592] */
    (xdc_Char)0x78,  /* [5593] */
    (xdc_Char)0x2c,  /* [5594] */
    (xdc_Char)0x20,  /* [5595] */
    (xdc_Char)0x65,  /* [5596] */
    (xdc_Char)0x76,  /* [5597] */
    (xdc_Char)0x65,  /* [5598] */
    (xdc_Char)0x6e,  /* [5599] */
    (xdc_Char)0x74,  /* [5600] */
    (xdc_Char)0x49,  /* [5601] */
    (xdc_Char)0x64,  /* [5602] */
    (xdc_Char)0x3a,  /* [5603] */
    (xdc_Char)0x20,  /* [5604] */
    (xdc_Char)0x30,  /* [5605] */
    (xdc_Char)0x78,  /* [5606] */
    (xdc_Char)0x25,  /* [5607] */
    (xdc_Char)0x78,  /* [5608] */
    (xdc_Char)0x0,  /* [5609] */
    (xdc_Char)0x4c,  /* [5610] */
    (xdc_Char)0x4d,  /* [5611] */
    (xdc_Char)0x5f,  /* [5612] */
    (xdc_Char)0x70,  /* [5613] */
    (xdc_Char)0x65,  /* [5614] */
    (xdc_Char)0x6e,  /* [5615] */
    (xdc_Char)0x64,  /* [5616] */
    (xdc_Char)0x3a,  /* [5617] */
    (xdc_Char)0x20,  /* [5618] */
    (xdc_Char)0x65,  /* [5619] */
    (xdc_Char)0x76,  /* [5620] */
    (xdc_Char)0x65,  /* [5621] */
    (xdc_Char)0x6e,  /* [5622] */
    (xdc_Char)0x74,  /* [5623] */
    (xdc_Char)0x3a,  /* [5624] */
    (xdc_Char)0x20,  /* [5625] */
    (xdc_Char)0x30,  /* [5626] */
    (xdc_Char)0x78,  /* [5627] */
    (xdc_Char)0x25,  /* [5628] */
    (xdc_Char)0x78,  /* [5629] */
    (xdc_Char)0x2c,  /* [5630] */
    (xdc_Char)0x20,  /* [5631] */
    (xdc_Char)0x63,  /* [5632] */
    (xdc_Char)0x75,  /* [5633] */
    (xdc_Char)0x72,  /* [5634] */
    (xdc_Char)0x72,  /* [5635] */
    (xdc_Char)0x45,  /* [5636] */
    (xdc_Char)0x76,  /* [5637] */
    (xdc_Char)0x65,  /* [5638] */
    (xdc_Char)0x6e,  /* [5639] */
    (xdc_Char)0x74,  /* [5640] */
    (xdc_Char)0x73,  /* [5641] */
    (xdc_Char)0x3a,  /* [5642] */
    (xdc_Char)0x20,  /* [5643] */
    (xdc_Char)0x30,  /* [5644] */
    (xdc_Char)0x78,  /* [5645] */
    (xdc_Char)0x25,  /* [5646] */
    (xdc_Char)0x78,  /* [5647] */
    (xdc_Char)0x2c,  /* [5648] */
    (xdc_Char)0x20,  /* [5649] */
    (xdc_Char)0x61,  /* [5650] */
    (xdc_Char)0x6e,  /* [5651] */
    (xdc_Char)0x64,  /* [5652] */
    (xdc_Char)0x4d,  /* [5653] */
    (xdc_Char)0x61,  /* [5654] */
    (xdc_Char)0x73,  /* [5655] */
    (xdc_Char)0x6b,  /* [5656] */
    (xdc_Char)0x3a,  /* [5657] */
    (xdc_Char)0x20,  /* [5658] */
    (xdc_Char)0x30,  /* [5659] */
    (xdc_Char)0x78,  /* [5660] */
    (xdc_Char)0x25,  /* [5661] */
    (xdc_Char)0x78,  /* [5662] */
    (xdc_Char)0x2c,  /* [5663] */
    (xdc_Char)0x20,  /* [5664] */
    (xdc_Char)0x6f,  /* [5665] */
    (xdc_Char)0x72,  /* [5666] */
    (xdc_Char)0x4d,  /* [5667] */
    (xdc_Char)0x61,  /* [5668] */
    (xdc_Char)0x73,  /* [5669] */
    (xdc_Char)0x6b,  /* [5670] */
    (xdc_Char)0x3a,  /* [5671] */
    (xdc_Char)0x20,  /* [5672] */
    (xdc_Char)0x30,  /* [5673] */
    (xdc_Char)0x78,  /* [5674] */
    (xdc_Char)0x25,  /* [5675] */
    (xdc_Char)0x78,  /* [5676] */
    (xdc_Char)0x2c,  /* [5677] */
    (xdc_Char)0x20,  /* [5678] */
    (xdc_Char)0x74,  /* [5679] */
    (xdc_Char)0x69,  /* [5680] */
    (xdc_Char)0x6d,  /* [5681] */
    (xdc_Char)0x65,  /* [5682] */
    (xdc_Char)0x6f,  /* [5683] */
    (xdc_Char)0x75,  /* [5684] */
    (xdc_Char)0x74,  /* [5685] */
    (xdc_Char)0x3a,  /* [5686] */
    (xdc_Char)0x20,  /* [5687] */
    (xdc_Char)0x25,  /* [5688] */
    (xdc_Char)0x64,  /* [5689] */
    (xdc_Char)0x0,  /* [5690] */
    (xdc_Char)0x4c,  /* [5691] */
    (xdc_Char)0x4d,  /* [5692] */
    (xdc_Char)0x5f,  /* [5693] */
    (xdc_Char)0x70,  /* [5694] */
    (xdc_Char)0x6f,  /* [5695] */
    (xdc_Char)0x73,  /* [5696] */
    (xdc_Char)0x74,  /* [5697] */
    (xdc_Char)0x3a,  /* [5698] */
    (xdc_Char)0x20,  /* [5699] */
    (xdc_Char)0x73,  /* [5700] */
    (xdc_Char)0x65,  /* [5701] */
    (xdc_Char)0x6d,  /* [5702] */
    (xdc_Char)0x3a,  /* [5703] */
    (xdc_Char)0x20,  /* [5704] */
    (xdc_Char)0x30,  /* [5705] */
    (xdc_Char)0x78,  /* [5706] */
    (xdc_Char)0x25,  /* [5707] */
    (xdc_Char)0x78,  /* [5708] */
    (xdc_Char)0x2c,  /* [5709] */
    (xdc_Char)0x20,  /* [5710] */
    (xdc_Char)0x63,  /* [5711] */
    (xdc_Char)0x6f,  /* [5712] */
    (xdc_Char)0x75,  /* [5713] */
    (xdc_Char)0x6e,  /* [5714] */
    (xdc_Char)0x74,  /* [5715] */
    (xdc_Char)0x3a,  /* [5716] */
    (xdc_Char)0x20,  /* [5717] */
    (xdc_Char)0x25,  /* [5718] */
    (xdc_Char)0x64,  /* [5719] */
    (xdc_Char)0x0,  /* [5720] */
    (xdc_Char)0x4c,  /* [5721] */
    (xdc_Char)0x4d,  /* [5722] */
    (xdc_Char)0x5f,  /* [5723] */
    (xdc_Char)0x70,  /* [5724] */
    (xdc_Char)0x65,  /* [5725] */
    (xdc_Char)0x6e,  /* [5726] */
    (xdc_Char)0x64,  /* [5727] */
    (xdc_Char)0x3a,  /* [5728] */
    (xdc_Char)0x20,  /* [5729] */
    (xdc_Char)0x73,  /* [5730] */
    (xdc_Char)0x65,  /* [5731] */
    (xdc_Char)0x6d,  /* [5732] */
    (xdc_Char)0x3a,  /* [5733] */
    (xdc_Char)0x20,  /* [5734] */
    (xdc_Char)0x30,  /* [5735] */
    (xdc_Char)0x78,  /* [5736] */
    (xdc_Char)0x25,  /* [5737] */
    (xdc_Char)0x78,  /* [5738] */
    (xdc_Char)0x2c,  /* [5739] */
    (xdc_Char)0x20,  /* [5740] */
    (xdc_Char)0x63,  /* [5741] */
    (xdc_Char)0x6f,  /* [5742] */
    (xdc_Char)0x75,  /* [5743] */
    (xdc_Char)0x6e,  /* [5744] */
    (xdc_Char)0x74,  /* [5745] */
    (xdc_Char)0x3a,  /* [5746] */
    (xdc_Char)0x20,  /* [5747] */
    (xdc_Char)0x25,  /* [5748] */
    (xdc_Char)0x64,  /* [5749] */
    (xdc_Char)0x2c,  /* [5750] */
    (xdc_Char)0x20,  /* [5751] */
    (xdc_Char)0x74,  /* [5752] */
    (xdc_Char)0x69,  /* [5753] */
    (xdc_Char)0x6d,  /* [5754] */
    (xdc_Char)0x65,  /* [5755] */
    (xdc_Char)0x6f,  /* [5756] */
    (xdc_Char)0x75,  /* [5757] */
    (xdc_Char)0x74,  /* [5758] */
    (xdc_Char)0x3a,  /* [5759] */
    (xdc_Char)0x20,  /* [5760] */
    (xdc_Char)0x25,  /* [5761] */
    (xdc_Char)0x64,  /* [5762] */
    (xdc_Char)0x0,  /* [5763] */
    (xdc_Char)0x4c,  /* [5764] */
    (xdc_Char)0x4d,  /* [5765] */
    (xdc_Char)0x5f,  /* [5766] */
    (xdc_Char)0x62,  /* [5767] */
    (xdc_Char)0x65,  /* [5768] */
    (xdc_Char)0x67,  /* [5769] */
    (xdc_Char)0x69,  /* [5770] */
    (xdc_Char)0x6e,  /* [5771] */
    (xdc_Char)0x3a,  /* [5772] */
    (xdc_Char)0x20,  /* [5773] */
    (xdc_Char)0x73,  /* [5774] */
    (xdc_Char)0x77,  /* [5775] */
    (xdc_Char)0x69,  /* [5776] */
    (xdc_Char)0x3a,  /* [5777] */
    (xdc_Char)0x20,  /* [5778] */
    (xdc_Char)0x30,  /* [5779] */
    (xdc_Char)0x78,  /* [5780] */
    (xdc_Char)0x25,  /* [5781] */
    (xdc_Char)0x78,  /* [5782] */
    (xdc_Char)0x2c,  /* [5783] */
    (xdc_Char)0x20,  /* [5784] */
    (xdc_Char)0x66,  /* [5785] */
    (xdc_Char)0x75,  /* [5786] */
    (xdc_Char)0x6e,  /* [5787] */
    (xdc_Char)0x63,  /* [5788] */
    (xdc_Char)0x3a,  /* [5789] */
    (xdc_Char)0x20,  /* [5790] */
    (xdc_Char)0x30,  /* [5791] */
    (xdc_Char)0x78,  /* [5792] */
    (xdc_Char)0x25,  /* [5793] */
    (xdc_Char)0x78,  /* [5794] */
    (xdc_Char)0x2c,  /* [5795] */
    (xdc_Char)0x20,  /* [5796] */
    (xdc_Char)0x70,  /* [5797] */
    (xdc_Char)0x72,  /* [5798] */
    (xdc_Char)0x65,  /* [5799] */
    (xdc_Char)0x54,  /* [5800] */
    (xdc_Char)0x68,  /* [5801] */
    (xdc_Char)0x72,  /* [5802] */
    (xdc_Char)0x65,  /* [5803] */
    (xdc_Char)0x61,  /* [5804] */
    (xdc_Char)0x64,  /* [5805] */
    (xdc_Char)0x3a,  /* [5806] */
    (xdc_Char)0x20,  /* [5807] */
    (xdc_Char)0x25,  /* [5808] */
    (xdc_Char)0x64,  /* [5809] */
    (xdc_Char)0x0,  /* [5810] */
    (xdc_Char)0x4c,  /* [5811] */
    (xdc_Char)0x44,  /* [5812] */
    (xdc_Char)0x5f,  /* [5813] */
    (xdc_Char)0x65,  /* [5814] */
    (xdc_Char)0x6e,  /* [5815] */
    (xdc_Char)0x64,  /* [5816] */
    (xdc_Char)0x3a,  /* [5817] */
    (xdc_Char)0x20,  /* [5818] */
    (xdc_Char)0x73,  /* [5819] */
    (xdc_Char)0x77,  /* [5820] */
    (xdc_Char)0x69,  /* [5821] */
    (xdc_Char)0x3a,  /* [5822] */
    (xdc_Char)0x20,  /* [5823] */
    (xdc_Char)0x30,  /* [5824] */
    (xdc_Char)0x78,  /* [5825] */
    (xdc_Char)0x25,  /* [5826] */
    (xdc_Char)0x78,  /* [5827] */
    (xdc_Char)0x0,  /* [5828] */
    (xdc_Char)0x4c,  /* [5829] */
    (xdc_Char)0x4d,  /* [5830] */
    (xdc_Char)0x5f,  /* [5831] */
    (xdc_Char)0x70,  /* [5832] */
    (xdc_Char)0x6f,  /* [5833] */
    (xdc_Char)0x73,  /* [5834] */
    (xdc_Char)0x74,  /* [5835] */
    (xdc_Char)0x3a,  /* [5836] */
    (xdc_Char)0x20,  /* [5837] */
    (xdc_Char)0x73,  /* [5838] */
    (xdc_Char)0x77,  /* [5839] */
    (xdc_Char)0x69,  /* [5840] */
    (xdc_Char)0x3a,  /* [5841] */
    (xdc_Char)0x20,  /* [5842] */
    (xdc_Char)0x30,  /* [5843] */
    (xdc_Char)0x78,  /* [5844] */
    (xdc_Char)0x25,  /* [5845] */
    (xdc_Char)0x78,  /* [5846] */
    (xdc_Char)0x2c,  /* [5847] */
    (xdc_Char)0x20,  /* [5848] */
    (xdc_Char)0x66,  /* [5849] */
    (xdc_Char)0x75,  /* [5850] */
    (xdc_Char)0x6e,  /* [5851] */
    (xdc_Char)0x63,  /* [5852] */
    (xdc_Char)0x3a,  /* [5853] */
    (xdc_Char)0x20,  /* [5854] */
    (xdc_Char)0x30,  /* [5855] */
    (xdc_Char)0x78,  /* [5856] */
    (xdc_Char)0x25,  /* [5857] */
    (xdc_Char)0x78,  /* [5858] */
    (xdc_Char)0x2c,  /* [5859] */
    (xdc_Char)0x20,  /* [5860] */
    (xdc_Char)0x70,  /* [5861] */
    (xdc_Char)0x72,  /* [5862] */
    (xdc_Char)0x69,  /* [5863] */
    (xdc_Char)0x3a,  /* [5864] */
    (xdc_Char)0x20,  /* [5865] */
    (xdc_Char)0x25,  /* [5866] */
    (xdc_Char)0x64,  /* [5867] */
    (xdc_Char)0x0,  /* [5868] */
    (xdc_Char)0x4c,  /* [5869] */
    (xdc_Char)0x4d,  /* [5870] */
    (xdc_Char)0x5f,  /* [5871] */
    (xdc_Char)0x73,  /* [5872] */
    (xdc_Char)0x77,  /* [5873] */
    (xdc_Char)0x69,  /* [5874] */
    (xdc_Char)0x74,  /* [5875] */
    (xdc_Char)0x63,  /* [5876] */
    (xdc_Char)0x68,  /* [5877] */
    (xdc_Char)0x3a,  /* [5878] */
    (xdc_Char)0x20,  /* [5879] */
    (xdc_Char)0x6f,  /* [5880] */
    (xdc_Char)0x6c,  /* [5881] */
    (xdc_Char)0x64,  /* [5882] */
    (xdc_Char)0x74,  /* [5883] */
    (xdc_Char)0x73,  /* [5884] */
    (xdc_Char)0x6b,  /* [5885] */
    (xdc_Char)0x3a,  /* [5886] */
    (xdc_Char)0x20,  /* [5887] */
    (xdc_Char)0x30,  /* [5888] */
    (xdc_Char)0x78,  /* [5889] */
    (xdc_Char)0x25,  /* [5890] */
    (xdc_Char)0x78,  /* [5891] */
    (xdc_Char)0x2c,  /* [5892] */
    (xdc_Char)0x20,  /* [5893] */
    (xdc_Char)0x6f,  /* [5894] */
    (xdc_Char)0x6c,  /* [5895] */
    (xdc_Char)0x64,  /* [5896] */
    (xdc_Char)0x66,  /* [5897] */
    (xdc_Char)0x75,  /* [5898] */
    (xdc_Char)0x6e,  /* [5899] */
    (xdc_Char)0x63,  /* [5900] */
    (xdc_Char)0x3a,  /* [5901] */
    (xdc_Char)0x20,  /* [5902] */
    (xdc_Char)0x30,  /* [5903] */
    (xdc_Char)0x78,  /* [5904] */
    (xdc_Char)0x25,  /* [5905] */
    (xdc_Char)0x78,  /* [5906] */
    (xdc_Char)0x2c,  /* [5907] */
    (xdc_Char)0x20,  /* [5908] */
    (xdc_Char)0x6e,  /* [5909] */
    (xdc_Char)0x65,  /* [5910] */
    (xdc_Char)0x77,  /* [5911] */
    (xdc_Char)0x74,  /* [5912] */
    (xdc_Char)0x73,  /* [5913] */
    (xdc_Char)0x6b,  /* [5914] */
    (xdc_Char)0x3a,  /* [5915] */
    (xdc_Char)0x20,  /* [5916] */
    (xdc_Char)0x30,  /* [5917] */
    (xdc_Char)0x78,  /* [5918] */
    (xdc_Char)0x25,  /* [5919] */
    (xdc_Char)0x78,  /* [5920] */
    (xdc_Char)0x2c,  /* [5921] */
    (xdc_Char)0x20,  /* [5922] */
    (xdc_Char)0x6e,  /* [5923] */
    (xdc_Char)0x65,  /* [5924] */
    (xdc_Char)0x77,  /* [5925] */
    (xdc_Char)0x66,  /* [5926] */
    (xdc_Char)0x75,  /* [5927] */
    (xdc_Char)0x6e,  /* [5928] */
    (xdc_Char)0x63,  /* [5929] */
    (xdc_Char)0x3a,  /* [5930] */
    (xdc_Char)0x20,  /* [5931] */
    (xdc_Char)0x30,  /* [5932] */
    (xdc_Char)0x78,  /* [5933] */
    (xdc_Char)0x25,  /* [5934] */
    (xdc_Char)0x78,  /* [5935] */
    (xdc_Char)0x0,  /* [5936] */
    (xdc_Char)0x4c,  /* [5937] */
    (xdc_Char)0x4d,  /* [5938] */
    (xdc_Char)0x5f,  /* [5939] */
    (xdc_Char)0x73,  /* [5940] */
    (xdc_Char)0x6c,  /* [5941] */
    (xdc_Char)0x65,  /* [5942] */
    (xdc_Char)0x65,  /* [5943] */
    (xdc_Char)0x70,  /* [5944] */
    (xdc_Char)0x3a,  /* [5945] */
    (xdc_Char)0x20,  /* [5946] */
    (xdc_Char)0x74,  /* [5947] */
    (xdc_Char)0x73,  /* [5948] */
    (xdc_Char)0x6b,  /* [5949] */
    (xdc_Char)0x3a,  /* [5950] */
    (xdc_Char)0x20,  /* [5951] */
    (xdc_Char)0x30,  /* [5952] */
    (xdc_Char)0x78,  /* [5953] */
    (xdc_Char)0x25,  /* [5954] */
    (xdc_Char)0x78,  /* [5955] */
    (xdc_Char)0x2c,  /* [5956] */
    (xdc_Char)0x20,  /* [5957] */
    (xdc_Char)0x66,  /* [5958] */
    (xdc_Char)0x75,  /* [5959] */
    (xdc_Char)0x6e,  /* [5960] */
    (xdc_Char)0x63,  /* [5961] */
    (xdc_Char)0x3a,  /* [5962] */
    (xdc_Char)0x20,  /* [5963] */
    (xdc_Char)0x30,  /* [5964] */
    (xdc_Char)0x78,  /* [5965] */
    (xdc_Char)0x25,  /* [5966] */
    (xdc_Char)0x78,  /* [5967] */
    (xdc_Char)0x2c,  /* [5968] */
    (xdc_Char)0x20,  /* [5969] */
    (xdc_Char)0x74,  /* [5970] */
    (xdc_Char)0x69,  /* [5971] */
    (xdc_Char)0x6d,  /* [5972] */
    (xdc_Char)0x65,  /* [5973] */
    (xdc_Char)0x6f,  /* [5974] */
    (xdc_Char)0x75,  /* [5975] */
    (xdc_Char)0x74,  /* [5976] */
    (xdc_Char)0x3a,  /* [5977] */
    (xdc_Char)0x20,  /* [5978] */
    (xdc_Char)0x25,  /* [5979] */
    (xdc_Char)0x64,  /* [5980] */
    (xdc_Char)0x0,  /* [5981] */
    (xdc_Char)0x4c,  /* [5982] */
    (xdc_Char)0x44,  /* [5983] */
    (xdc_Char)0x5f,  /* [5984] */
    (xdc_Char)0x72,  /* [5985] */
    (xdc_Char)0x65,  /* [5986] */
    (xdc_Char)0x61,  /* [5987] */
    (xdc_Char)0x64,  /* [5988] */
    (xdc_Char)0x79,  /* [5989] */
    (xdc_Char)0x3a,  /* [5990] */
    (xdc_Char)0x20,  /* [5991] */
    (xdc_Char)0x74,  /* [5992] */
    (xdc_Char)0x73,  /* [5993] */
    (xdc_Char)0x6b,  /* [5994] */
    (xdc_Char)0x3a,  /* [5995] */
    (xdc_Char)0x20,  /* [5996] */
    (xdc_Char)0x30,  /* [5997] */
    (xdc_Char)0x78,  /* [5998] */
    (xdc_Char)0x25,  /* [5999] */
    (xdc_Char)0x78,  /* [6000] */
    (xdc_Char)0x2c,  /* [6001] */
    (xdc_Char)0x20,  /* [6002] */
    (xdc_Char)0x66,  /* [6003] */
    (xdc_Char)0x75,  /* [6004] */
    (xdc_Char)0x6e,  /* [6005] */
    (xdc_Char)0x63,  /* [6006] */
    (xdc_Char)0x3a,  /* [6007] */
    (xdc_Char)0x20,  /* [6008] */
    (xdc_Char)0x30,  /* [6009] */
    (xdc_Char)0x78,  /* [6010] */
    (xdc_Char)0x25,  /* [6011] */
    (xdc_Char)0x78,  /* [6012] */
    (xdc_Char)0x2c,  /* [6013] */
    (xdc_Char)0x20,  /* [6014] */
    (xdc_Char)0x70,  /* [6015] */
    (xdc_Char)0x72,  /* [6016] */
    (xdc_Char)0x69,  /* [6017] */
    (xdc_Char)0x3a,  /* [6018] */
    (xdc_Char)0x20,  /* [6019] */
    (xdc_Char)0x25,  /* [6020] */
    (xdc_Char)0x64,  /* [6021] */
    (xdc_Char)0x0,  /* [6022] */
    (xdc_Char)0x4c,  /* [6023] */
    (xdc_Char)0x44,  /* [6024] */
    (xdc_Char)0x5f,  /* [6025] */
    (xdc_Char)0x62,  /* [6026] */
    (xdc_Char)0x6c,  /* [6027] */
    (xdc_Char)0x6f,  /* [6028] */
    (xdc_Char)0x63,  /* [6029] */
    (xdc_Char)0x6b,  /* [6030] */
    (xdc_Char)0x3a,  /* [6031] */
    (xdc_Char)0x20,  /* [6032] */
    (xdc_Char)0x74,  /* [6033] */
    (xdc_Char)0x73,  /* [6034] */
    (xdc_Char)0x6b,  /* [6035] */
    (xdc_Char)0x3a,  /* [6036] */
    (xdc_Char)0x20,  /* [6037] */
    (xdc_Char)0x30,  /* [6038] */
    (xdc_Char)0x78,  /* [6039] */
    (xdc_Char)0x25,  /* [6040] */
    (xdc_Char)0x78,  /* [6041] */
    (xdc_Char)0x2c,  /* [6042] */
    (xdc_Char)0x20,  /* [6043] */
    (xdc_Char)0x66,  /* [6044] */
    (xdc_Char)0x75,  /* [6045] */
    (xdc_Char)0x6e,  /* [6046] */
    (xdc_Char)0x63,  /* [6047] */
    (xdc_Char)0x3a,  /* [6048] */
    (xdc_Char)0x20,  /* [6049] */
    (xdc_Char)0x30,  /* [6050] */
    (xdc_Char)0x78,  /* [6051] */
    (xdc_Char)0x25,  /* [6052] */
    (xdc_Char)0x78,  /* [6053] */
    (xdc_Char)0x0,  /* [6054] */
    (xdc_Char)0x4c,  /* [6055] */
    (xdc_Char)0x4d,  /* [6056] */
    (xdc_Char)0x5f,  /* [6057] */
    (xdc_Char)0x79,  /* [6058] */
    (xdc_Char)0x69,  /* [6059] */
    (xdc_Char)0x65,  /* [6060] */
    (xdc_Char)0x6c,  /* [6061] */
    (xdc_Char)0x64,  /* [6062] */
    (xdc_Char)0x3a,  /* [6063] */
    (xdc_Char)0x20,  /* [6064] */
    (xdc_Char)0x74,  /* [6065] */
    (xdc_Char)0x73,  /* [6066] */
    (xdc_Char)0x6b,  /* [6067] */
    (xdc_Char)0x3a,  /* [6068] */
    (xdc_Char)0x20,  /* [6069] */
    (xdc_Char)0x30,  /* [6070] */
    (xdc_Char)0x78,  /* [6071] */
    (xdc_Char)0x25,  /* [6072] */
    (xdc_Char)0x78,  /* [6073] */
    (xdc_Char)0x2c,  /* [6074] */
    (xdc_Char)0x20,  /* [6075] */
    (xdc_Char)0x66,  /* [6076] */
    (xdc_Char)0x75,  /* [6077] */
    (xdc_Char)0x6e,  /* [6078] */
    (xdc_Char)0x63,  /* [6079] */
    (xdc_Char)0x3a,  /* [6080] */
    (xdc_Char)0x20,  /* [6081] */
    (xdc_Char)0x30,  /* [6082] */
    (xdc_Char)0x78,  /* [6083] */
    (xdc_Char)0x25,  /* [6084] */
    (xdc_Char)0x78,  /* [6085] */
    (xdc_Char)0x2c,  /* [6086] */
    (xdc_Char)0x20,  /* [6087] */
    (xdc_Char)0x63,  /* [6088] */
    (xdc_Char)0x75,  /* [6089] */
    (xdc_Char)0x72,  /* [6090] */
    (xdc_Char)0x72,  /* [6091] */
    (xdc_Char)0x54,  /* [6092] */
    (xdc_Char)0x68,  /* [6093] */
    (xdc_Char)0x72,  /* [6094] */
    (xdc_Char)0x65,  /* [6095] */
    (xdc_Char)0x61,  /* [6096] */
    (xdc_Char)0x64,  /* [6097] */
    (xdc_Char)0x3a,  /* [6098] */
    (xdc_Char)0x20,  /* [6099] */
    (xdc_Char)0x25,  /* [6100] */
    (xdc_Char)0x64,  /* [6101] */
    (xdc_Char)0x0,  /* [6102] */
    (xdc_Char)0x4c,  /* [6103] */
    (xdc_Char)0x4d,  /* [6104] */
    (xdc_Char)0x5f,  /* [6105] */
    (xdc_Char)0x73,  /* [6106] */
    (xdc_Char)0x65,  /* [6107] */
    (xdc_Char)0x74,  /* [6108] */
    (xdc_Char)0x50,  /* [6109] */
    (xdc_Char)0x72,  /* [6110] */
    (xdc_Char)0x69,  /* [6111] */
    (xdc_Char)0x3a,  /* [6112] */
    (xdc_Char)0x20,  /* [6113] */
    (xdc_Char)0x74,  /* [6114] */
    (xdc_Char)0x73,  /* [6115] */
    (xdc_Char)0x6b,  /* [6116] */
    (xdc_Char)0x3a,  /* [6117] */
    (xdc_Char)0x20,  /* [6118] */
    (xdc_Char)0x30,  /* [6119] */
    (xdc_Char)0x78,  /* [6120] */
    (xdc_Char)0x25,  /* [6121] */
    (xdc_Char)0x78,  /* [6122] */
    (xdc_Char)0x2c,  /* [6123] */
    (xdc_Char)0x20,  /* [6124] */
    (xdc_Char)0x66,  /* [6125] */
    (xdc_Char)0x75,  /* [6126] */
    (xdc_Char)0x6e,  /* [6127] */
    (xdc_Char)0x63,  /* [6128] */
    (xdc_Char)0x3a,  /* [6129] */
    (xdc_Char)0x20,  /* [6130] */
    (xdc_Char)0x30,  /* [6131] */
    (xdc_Char)0x78,  /* [6132] */
    (xdc_Char)0x25,  /* [6133] */
    (xdc_Char)0x78,  /* [6134] */
    (xdc_Char)0x2c,  /* [6135] */
    (xdc_Char)0x20,  /* [6136] */
    (xdc_Char)0x6f,  /* [6137] */
    (xdc_Char)0x6c,  /* [6138] */
    (xdc_Char)0x64,  /* [6139] */
    (xdc_Char)0x50,  /* [6140] */
    (xdc_Char)0x72,  /* [6141] */
    (xdc_Char)0x69,  /* [6142] */
    (xdc_Char)0x3a,  /* [6143] */
    (xdc_Char)0x20,  /* [6144] */
    (xdc_Char)0x25,  /* [6145] */
    (xdc_Char)0x64,  /* [6146] */
    (xdc_Char)0x2c,  /* [6147] */
    (xdc_Char)0x20,  /* [6148] */
    (xdc_Char)0x6e,  /* [6149] */
    (xdc_Char)0x65,  /* [6150] */
    (xdc_Char)0x77,  /* [6151] */
    (xdc_Char)0x50,  /* [6152] */
    (xdc_Char)0x72,  /* [6153] */
    (xdc_Char)0x69,  /* [6154] */
    (xdc_Char)0x20,  /* [6155] */
    (xdc_Char)0x25,  /* [6156] */
    (xdc_Char)0x64,  /* [6157] */
    (xdc_Char)0x0,  /* [6158] */
    (xdc_Char)0x4c,  /* [6159] */
    (xdc_Char)0x44,  /* [6160] */
    (xdc_Char)0x5f,  /* [6161] */
    (xdc_Char)0x65,  /* [6162] */
    (xdc_Char)0x78,  /* [6163] */
    (xdc_Char)0x69,  /* [6164] */
    (xdc_Char)0x74,  /* [6165] */
    (xdc_Char)0x3a,  /* [6166] */
    (xdc_Char)0x20,  /* [6167] */
    (xdc_Char)0x74,  /* [6168] */
    (xdc_Char)0x73,  /* [6169] */
    (xdc_Char)0x6b,  /* [6170] */
    (xdc_Char)0x3a,  /* [6171] */
    (xdc_Char)0x20,  /* [6172] */
    (xdc_Char)0x30,  /* [6173] */
    (xdc_Char)0x78,  /* [6174] */
    (xdc_Char)0x25,  /* [6175] */
    (xdc_Char)0x78,  /* [6176] */
    (xdc_Char)0x2c,  /* [6177] */
    (xdc_Char)0x20,  /* [6178] */
    (xdc_Char)0x66,  /* [6179] */
    (xdc_Char)0x75,  /* [6180] */
    (xdc_Char)0x6e,  /* [6181] */
    (xdc_Char)0x63,  /* [6182] */
    (xdc_Char)0x3a,  /* [6183] */
    (xdc_Char)0x20,  /* [6184] */
    (xdc_Char)0x30,  /* [6185] */
    (xdc_Char)0x78,  /* [6186] */
    (xdc_Char)0x25,  /* [6187] */
    (xdc_Char)0x78,  /* [6188] */
    (xdc_Char)0x0,  /* [6189] */
    (xdc_Char)0x4c,  /* [6190] */
    (xdc_Char)0x4d,  /* [6191] */
    (xdc_Char)0x5f,  /* [6192] */
    (xdc_Char)0x73,  /* [6193] */
    (xdc_Char)0x65,  /* [6194] */
    (xdc_Char)0x74,  /* [6195] */
    (xdc_Char)0x41,  /* [6196] */
    (xdc_Char)0x66,  /* [6197] */
    (xdc_Char)0x66,  /* [6198] */
    (xdc_Char)0x69,  /* [6199] */
    (xdc_Char)0x6e,  /* [6200] */
    (xdc_Char)0x69,  /* [6201] */
    (xdc_Char)0x74,  /* [6202] */
    (xdc_Char)0x79,  /* [6203] */
    (xdc_Char)0x3a,  /* [6204] */
    (xdc_Char)0x20,  /* [6205] */
    (xdc_Char)0x74,  /* [6206] */
    (xdc_Char)0x73,  /* [6207] */
    (xdc_Char)0x6b,  /* [6208] */
    (xdc_Char)0x3a,  /* [6209] */
    (xdc_Char)0x20,  /* [6210] */
    (xdc_Char)0x30,  /* [6211] */
    (xdc_Char)0x78,  /* [6212] */
    (xdc_Char)0x25,  /* [6213] */
    (xdc_Char)0x78,  /* [6214] */
    (xdc_Char)0x2c,  /* [6215] */
    (xdc_Char)0x20,  /* [6216] */
    (xdc_Char)0x66,  /* [6217] */
    (xdc_Char)0x75,  /* [6218] */
    (xdc_Char)0x6e,  /* [6219] */
    (xdc_Char)0x63,  /* [6220] */
    (xdc_Char)0x3a,  /* [6221] */
    (xdc_Char)0x20,  /* [6222] */
    (xdc_Char)0x30,  /* [6223] */
    (xdc_Char)0x78,  /* [6224] */
    (xdc_Char)0x25,  /* [6225] */
    (xdc_Char)0x78,  /* [6226] */
    (xdc_Char)0x2c,  /* [6227] */
    (xdc_Char)0x20,  /* [6228] */
    (xdc_Char)0x6f,  /* [6229] */
    (xdc_Char)0x6c,  /* [6230] */
    (xdc_Char)0x64,  /* [6231] */
    (xdc_Char)0x43,  /* [6232] */
    (xdc_Char)0x6f,  /* [6233] */
    (xdc_Char)0x72,  /* [6234] */
    (xdc_Char)0x65,  /* [6235] */
    (xdc_Char)0x3a,  /* [6236] */
    (xdc_Char)0x20,  /* [6237] */
    (xdc_Char)0x25,  /* [6238] */
    (xdc_Char)0x64,  /* [6239] */
    (xdc_Char)0x2c,  /* [6240] */
    (xdc_Char)0x20,  /* [6241] */
    (xdc_Char)0x6f,  /* [6242] */
    (xdc_Char)0x6c,  /* [6243] */
    (xdc_Char)0x64,  /* [6244] */
    (xdc_Char)0x41,  /* [6245] */
    (xdc_Char)0x66,  /* [6246] */
    (xdc_Char)0x66,  /* [6247] */
    (xdc_Char)0x69,  /* [6248] */
    (xdc_Char)0x6e,  /* [6249] */
    (xdc_Char)0x69,  /* [6250] */
    (xdc_Char)0x74,  /* [6251] */
    (xdc_Char)0x79,  /* [6252] */
    (xdc_Char)0x20,  /* [6253] */
    (xdc_Char)0x25,  /* [6254] */
    (xdc_Char)0x64,  /* [6255] */
    (xdc_Char)0x2c,  /* [6256] */
    (xdc_Char)0x20,  /* [6257] */
    (xdc_Char)0x6e,  /* [6258] */
    (xdc_Char)0x65,  /* [6259] */
    (xdc_Char)0x77,  /* [6260] */
    (xdc_Char)0x41,  /* [6261] */
    (xdc_Char)0x66,  /* [6262] */
    (xdc_Char)0x66,  /* [6263] */
    (xdc_Char)0x69,  /* [6264] */
    (xdc_Char)0x6e,  /* [6265] */
    (xdc_Char)0x69,  /* [6266] */
    (xdc_Char)0x74,  /* [6267] */
    (xdc_Char)0x79,  /* [6268] */
    (xdc_Char)0x20,  /* [6269] */
    (xdc_Char)0x25,  /* [6270] */
    (xdc_Char)0x64,  /* [6271] */
    (xdc_Char)0x0,  /* [6272] */
    (xdc_Char)0x4c,  /* [6273] */
    (xdc_Char)0x44,  /* [6274] */
    (xdc_Char)0x5f,  /* [6275] */
    (xdc_Char)0x73,  /* [6276] */
    (xdc_Char)0x63,  /* [6277] */
    (xdc_Char)0x68,  /* [6278] */
    (xdc_Char)0x65,  /* [6279] */
    (xdc_Char)0x64,  /* [6280] */
    (xdc_Char)0x75,  /* [6281] */
    (xdc_Char)0x6c,  /* [6282] */
    (xdc_Char)0x65,  /* [6283] */
    (xdc_Char)0x3a,  /* [6284] */
    (xdc_Char)0x20,  /* [6285] */
    (xdc_Char)0x63,  /* [6286] */
    (xdc_Char)0x6f,  /* [6287] */
    (xdc_Char)0x72,  /* [6288] */
    (xdc_Char)0x65,  /* [6289] */
    (xdc_Char)0x49,  /* [6290] */
    (xdc_Char)0x64,  /* [6291] */
    (xdc_Char)0x3a,  /* [6292] */
    (xdc_Char)0x20,  /* [6293] */
    (xdc_Char)0x25,  /* [6294] */
    (xdc_Char)0x64,  /* [6295] */
    (xdc_Char)0x2c,  /* [6296] */
    (xdc_Char)0x20,  /* [6297] */
    (xdc_Char)0x77,  /* [6298] */
    (xdc_Char)0x6f,  /* [6299] */
    (xdc_Char)0x72,  /* [6300] */
    (xdc_Char)0x6b,  /* [6301] */
    (xdc_Char)0x46,  /* [6302] */
    (xdc_Char)0x6c,  /* [6303] */
    (xdc_Char)0x61,  /* [6304] */
    (xdc_Char)0x67,  /* [6305] */
    (xdc_Char)0x3a,  /* [6306] */
    (xdc_Char)0x20,  /* [6307] */
    (xdc_Char)0x25,  /* [6308] */
    (xdc_Char)0x64,  /* [6309] */
    (xdc_Char)0x2c,  /* [6310] */
    (xdc_Char)0x20,  /* [6311] */
    (xdc_Char)0x63,  /* [6312] */
    (xdc_Char)0x75,  /* [6313] */
    (xdc_Char)0x72,  /* [6314] */
    (xdc_Char)0x53,  /* [6315] */
    (xdc_Char)0x65,  /* [6316] */
    (xdc_Char)0x74,  /* [6317] */
    (xdc_Char)0x4c,  /* [6318] */
    (xdc_Char)0x6f,  /* [6319] */
    (xdc_Char)0x63,  /* [6320] */
    (xdc_Char)0x61,  /* [6321] */
    (xdc_Char)0x6c,  /* [6322] */
    (xdc_Char)0x3a,  /* [6323] */
    (xdc_Char)0x20,  /* [6324] */
    (xdc_Char)0x25,  /* [6325] */
    (xdc_Char)0x64,  /* [6326] */
    (xdc_Char)0x2c,  /* [6327] */
    (xdc_Char)0x20,  /* [6328] */
    (xdc_Char)0x63,  /* [6329] */
    (xdc_Char)0x75,  /* [6330] */
    (xdc_Char)0x72,  /* [6331] */
    (xdc_Char)0x53,  /* [6332] */
    (xdc_Char)0x65,  /* [6333] */
    (xdc_Char)0x74,  /* [6334] */
    (xdc_Char)0x58,  /* [6335] */
    (xdc_Char)0x3a,  /* [6336] */
    (xdc_Char)0x20,  /* [6337] */
    (xdc_Char)0x25,  /* [6338] */
    (xdc_Char)0x64,  /* [6339] */
    (xdc_Char)0x2c,  /* [6340] */
    (xdc_Char)0x20,  /* [6341] */
    (xdc_Char)0x63,  /* [6342] */
    (xdc_Char)0x75,  /* [6343] */
    (xdc_Char)0x72,  /* [6344] */
    (xdc_Char)0x4d,  /* [6345] */
    (xdc_Char)0x61,  /* [6346] */
    (xdc_Char)0x73,  /* [6347] */
    (xdc_Char)0x6b,  /* [6348] */
    (xdc_Char)0x4c,  /* [6349] */
    (xdc_Char)0x6f,  /* [6350] */
    (xdc_Char)0x63,  /* [6351] */
    (xdc_Char)0x61,  /* [6352] */
    (xdc_Char)0x6c,  /* [6353] */
    (xdc_Char)0x3a,  /* [6354] */
    (xdc_Char)0x20,  /* [6355] */
    (xdc_Char)0x25,  /* [6356] */
    (xdc_Char)0x64,  /* [6357] */
    (xdc_Char)0x0,  /* [6358] */
    (xdc_Char)0x4c,  /* [6359] */
    (xdc_Char)0x44,  /* [6360] */
    (xdc_Char)0x5f,  /* [6361] */
    (xdc_Char)0x6e,  /* [6362] */
    (xdc_Char)0x6f,  /* [6363] */
    (xdc_Char)0x57,  /* [6364] */
    (xdc_Char)0x6f,  /* [6365] */
    (xdc_Char)0x72,  /* [6366] */
    (xdc_Char)0x6b,  /* [6367] */
    (xdc_Char)0x3a,  /* [6368] */
    (xdc_Char)0x20,  /* [6369] */
    (xdc_Char)0x63,  /* [6370] */
    (xdc_Char)0x6f,  /* [6371] */
    (xdc_Char)0x72,  /* [6372] */
    (xdc_Char)0x65,  /* [6373] */
    (xdc_Char)0x49,  /* [6374] */
    (xdc_Char)0x64,  /* [6375] */
    (xdc_Char)0x3a,  /* [6376] */
    (xdc_Char)0x20,  /* [6377] */
    (xdc_Char)0x25,  /* [6378] */
    (xdc_Char)0x64,  /* [6379] */
    (xdc_Char)0x2c,  /* [6380] */
    (xdc_Char)0x20,  /* [6381] */
    (xdc_Char)0x63,  /* [6382] */
    (xdc_Char)0x75,  /* [6383] */
    (xdc_Char)0x72,  /* [6384] */
    (xdc_Char)0x53,  /* [6385] */
    (xdc_Char)0x65,  /* [6386] */
    (xdc_Char)0x74,  /* [6387] */
    (xdc_Char)0x4c,  /* [6388] */
    (xdc_Char)0x6f,  /* [6389] */
    (xdc_Char)0x63,  /* [6390] */
    (xdc_Char)0x61,  /* [6391] */
    (xdc_Char)0x6c,  /* [6392] */
    (xdc_Char)0x3a,  /* [6393] */
    (xdc_Char)0x20,  /* [6394] */
    (xdc_Char)0x25,  /* [6395] */
    (xdc_Char)0x64,  /* [6396] */
    (xdc_Char)0x2c,  /* [6397] */
    (xdc_Char)0x20,  /* [6398] */
    (xdc_Char)0x63,  /* [6399] */
    (xdc_Char)0x75,  /* [6400] */
    (xdc_Char)0x72,  /* [6401] */
    (xdc_Char)0x53,  /* [6402] */
    (xdc_Char)0x65,  /* [6403] */
    (xdc_Char)0x74,  /* [6404] */
    (xdc_Char)0x58,  /* [6405] */
    (xdc_Char)0x3a,  /* [6406] */
    (xdc_Char)0x20,  /* [6407] */
    (xdc_Char)0x25,  /* [6408] */
    (xdc_Char)0x64,  /* [6409] */
    (xdc_Char)0x2c,  /* [6410] */
    (xdc_Char)0x20,  /* [6411] */
    (xdc_Char)0x63,  /* [6412] */
    (xdc_Char)0x75,  /* [6413] */
    (xdc_Char)0x72,  /* [6414] */
    (xdc_Char)0x4d,  /* [6415] */
    (xdc_Char)0x61,  /* [6416] */
    (xdc_Char)0x73,  /* [6417] */
    (xdc_Char)0x6b,  /* [6418] */
    (xdc_Char)0x4c,  /* [6419] */
    (xdc_Char)0x6f,  /* [6420] */
    (xdc_Char)0x63,  /* [6421] */
    (xdc_Char)0x61,  /* [6422] */
    (xdc_Char)0x6c,  /* [6423] */
    (xdc_Char)0x3a,  /* [6424] */
    (xdc_Char)0x20,  /* [6425] */
    (xdc_Char)0x25,  /* [6426] */
    (xdc_Char)0x64,  /* [6427] */
    (xdc_Char)0x0,  /* [6428] */
    (xdc_Char)0x4c,  /* [6429] */
    (xdc_Char)0x4d,  /* [6430] */
    (xdc_Char)0x5f,  /* [6431] */
    (xdc_Char)0x62,  /* [6432] */
    (xdc_Char)0x65,  /* [6433] */
    (xdc_Char)0x67,  /* [6434] */
    (xdc_Char)0x69,  /* [6435] */
    (xdc_Char)0x6e,  /* [6436] */
    (xdc_Char)0x3a,  /* [6437] */
    (xdc_Char)0x20,  /* [6438] */
    (xdc_Char)0x68,  /* [6439] */
    (xdc_Char)0x77,  /* [6440] */
    (xdc_Char)0x69,  /* [6441] */
    (xdc_Char)0x3a,  /* [6442] */
    (xdc_Char)0x20,  /* [6443] */
    (xdc_Char)0x30,  /* [6444] */
    (xdc_Char)0x78,  /* [6445] */
    (xdc_Char)0x25,  /* [6446] */
    (xdc_Char)0x78,  /* [6447] */
    (xdc_Char)0x2c,  /* [6448] */
    (xdc_Char)0x20,  /* [6449] */
    (xdc_Char)0x66,  /* [6450] */
    (xdc_Char)0x75,  /* [6451] */
    (xdc_Char)0x6e,  /* [6452] */
    (xdc_Char)0x63,  /* [6453] */
    (xdc_Char)0x3a,  /* [6454] */
    (xdc_Char)0x20,  /* [6455] */
    (xdc_Char)0x30,  /* [6456] */
    (xdc_Char)0x78,  /* [6457] */
    (xdc_Char)0x25,  /* [6458] */
    (xdc_Char)0x78,  /* [6459] */
    (xdc_Char)0x2c,  /* [6460] */
    (xdc_Char)0x20,  /* [6461] */
    (xdc_Char)0x70,  /* [6462] */
    (xdc_Char)0x72,  /* [6463] */
    (xdc_Char)0x65,  /* [6464] */
    (xdc_Char)0x54,  /* [6465] */
    (xdc_Char)0x68,  /* [6466] */
    (xdc_Char)0x72,  /* [6467] */
    (xdc_Char)0x65,  /* [6468] */
    (xdc_Char)0x61,  /* [6469] */
    (xdc_Char)0x64,  /* [6470] */
    (xdc_Char)0x3a,  /* [6471] */
    (xdc_Char)0x20,  /* [6472] */
    (xdc_Char)0x25,  /* [6473] */
    (xdc_Char)0x64,  /* [6474] */
    (xdc_Char)0x2c,  /* [6475] */
    (xdc_Char)0x20,  /* [6476] */
    (xdc_Char)0x69,  /* [6477] */
    (xdc_Char)0x6e,  /* [6478] */
    (xdc_Char)0x74,  /* [6479] */
    (xdc_Char)0x4e,  /* [6480] */
    (xdc_Char)0x75,  /* [6481] */
    (xdc_Char)0x6d,  /* [6482] */
    (xdc_Char)0x3a,  /* [6483] */
    (xdc_Char)0x20,  /* [6484] */
    (xdc_Char)0x25,  /* [6485] */
    (xdc_Char)0x64,  /* [6486] */
    (xdc_Char)0x2c,  /* [6487] */
    (xdc_Char)0x20,  /* [6488] */
    (xdc_Char)0x69,  /* [6489] */
    (xdc_Char)0x72,  /* [6490] */
    (xdc_Char)0x70,  /* [6491] */
    (xdc_Char)0x3a,  /* [6492] */
    (xdc_Char)0x20,  /* [6493] */
    (xdc_Char)0x30,  /* [6494] */
    (xdc_Char)0x78,  /* [6495] */
    (xdc_Char)0x25,  /* [6496] */
    (xdc_Char)0x78,  /* [6497] */
    (xdc_Char)0x0,  /* [6498] */
    (xdc_Char)0x4c,  /* [6499] */
    (xdc_Char)0x44,  /* [6500] */
    (xdc_Char)0x5f,  /* [6501] */
    (xdc_Char)0x65,  /* [6502] */
    (xdc_Char)0x6e,  /* [6503] */
    (xdc_Char)0x64,  /* [6504] */
    (xdc_Char)0x3a,  /* [6505] */
    (xdc_Char)0x20,  /* [6506] */
    (xdc_Char)0x68,  /* [6507] */
    (xdc_Char)0x77,  /* [6508] */
    (xdc_Char)0x69,  /* [6509] */
    (xdc_Char)0x3a,  /* [6510] */
    (xdc_Char)0x20,  /* [6511] */
    (xdc_Char)0x30,  /* [6512] */
    (xdc_Char)0x78,  /* [6513] */
    (xdc_Char)0x25,  /* [6514] */
    (xdc_Char)0x78,  /* [6515] */
    (xdc_Char)0x0,  /* [6516] */
    (xdc_Char)0x78,  /* [6517] */
    (xdc_Char)0x64,  /* [6518] */
    (xdc_Char)0x63,  /* [6519] */
    (xdc_Char)0x2e,  /* [6520] */
    (xdc_Char)0x0,  /* [6521] */
    (xdc_Char)0x72,  /* [6522] */
    (xdc_Char)0x75,  /* [6523] */
    (xdc_Char)0x6e,  /* [6524] */
    (xdc_Char)0x74,  /* [6525] */
    (xdc_Char)0x69,  /* [6526] */
    (xdc_Char)0x6d,  /* [6527] */
    (xdc_Char)0x65,  /* [6528] */
    (xdc_Char)0x2e,  /* [6529] */
    (xdc_Char)0x0,  /* [6530] */
    (xdc_Char)0x41,  /* [6531] */
    (xdc_Char)0x73,  /* [6532] */
    (xdc_Char)0x73,  /* [6533] */
    (xdc_Char)0x65,  /* [6534] */
    (xdc_Char)0x72,  /* [6535] */
    (xdc_Char)0x74,  /* [6536] */
    (xdc_Char)0x0,  /* [6537] */
    (xdc_Char)0x43,  /* [6538] */
    (xdc_Char)0x6f,  /* [6539] */
    (xdc_Char)0x72,  /* [6540] */
    (xdc_Char)0x65,  /* [6541] */
    (xdc_Char)0x0,  /* [6542] */
    (xdc_Char)0x44,  /* [6543] */
    (xdc_Char)0x65,  /* [6544] */
    (xdc_Char)0x66,  /* [6545] */
    (xdc_Char)0x61,  /* [6546] */
    (xdc_Char)0x75,  /* [6547] */
    (xdc_Char)0x6c,  /* [6548] */
    (xdc_Char)0x74,  /* [6549] */
    (xdc_Char)0x73,  /* [6550] */
    (xdc_Char)0x0,  /* [6551] */
    (xdc_Char)0x44,  /* [6552] */
    (xdc_Char)0x69,  /* [6553] */
    (xdc_Char)0x61,  /* [6554] */
    (xdc_Char)0x67,  /* [6555] */
    (xdc_Char)0x73,  /* [6556] */
    (xdc_Char)0x0,  /* [6557] */
    (xdc_Char)0x45,  /* [6558] */
    (xdc_Char)0x72,  /* [6559] */
    (xdc_Char)0x72,  /* [6560] */
    (xdc_Char)0x6f,  /* [6561] */
    (xdc_Char)0x72,  /* [6562] */
    (xdc_Char)0x0,  /* [6563] */
    (xdc_Char)0x47,  /* [6564] */
    (xdc_Char)0x61,  /* [6565] */
    (xdc_Char)0x74,  /* [6566] */
    (xdc_Char)0x65,  /* [6567] */
    (xdc_Char)0x0,  /* [6568] */
    (xdc_Char)0x4c,  /* [6569] */
    (xdc_Char)0x6f,  /* [6570] */
    (xdc_Char)0x67,  /* [6571] */
    (xdc_Char)0x0,  /* [6572] */
    (xdc_Char)0x4d,  /* [6573] */
    (xdc_Char)0x61,  /* [6574] */
    (xdc_Char)0x69,  /* [6575] */
    (xdc_Char)0x6e,  /* [6576] */
    (xdc_Char)0x0,  /* [6577] */
    (xdc_Char)0x4d,  /* [6578] */
    (xdc_Char)0x65,  /* [6579] */
    (xdc_Char)0x6d,  /* [6580] */
    (xdc_Char)0x6f,  /* [6581] */
    (xdc_Char)0x72,  /* [6582] */
    (xdc_Char)0x79,  /* [6583] */
    (xdc_Char)0x0,  /* [6584] */
    (xdc_Char)0x52,  /* [6585] */
    (xdc_Char)0x65,  /* [6586] */
    (xdc_Char)0x67,  /* [6587] */
    (xdc_Char)0x69,  /* [6588] */
    (xdc_Char)0x73,  /* [6589] */
    (xdc_Char)0x74,  /* [6590] */
    (xdc_Char)0x72,  /* [6591] */
    (xdc_Char)0x79,  /* [6592] */
    (xdc_Char)0x0,  /* [6593] */
    (xdc_Char)0x53,  /* [6594] */
    (xdc_Char)0x74,  /* [6595] */
    (xdc_Char)0x61,  /* [6596] */
    (xdc_Char)0x72,  /* [6597] */
    (xdc_Char)0x74,  /* [6598] */
    (xdc_Char)0x75,  /* [6599] */
    (xdc_Char)0x70,  /* [6600] */
    (xdc_Char)0x0,  /* [6601] */
    (xdc_Char)0x53,  /* [6602] */
    (xdc_Char)0x79,  /* [6603] */
    (xdc_Char)0x73,  /* [6604] */
    (xdc_Char)0x74,  /* [6605] */
    (xdc_Char)0x65,  /* [6606] */
    (xdc_Char)0x6d,  /* [6607] */
    (xdc_Char)0x0,  /* [6608] */
    (xdc_Char)0x53,  /* [6609] */
    (xdc_Char)0x79,  /* [6610] */
    (xdc_Char)0x73,  /* [6611] */
    (xdc_Char)0x53,  /* [6612] */
    (xdc_Char)0x74,  /* [6613] */
    (xdc_Char)0x64,  /* [6614] */
    (xdc_Char)0x0,  /* [6615] */
    (xdc_Char)0x54,  /* [6616] */
    (xdc_Char)0x65,  /* [6617] */
    (xdc_Char)0x78,  /* [6618] */
    (xdc_Char)0x74,  /* [6619] */
    (xdc_Char)0x0,  /* [6620] */
    (xdc_Char)0x74,  /* [6621] */
    (xdc_Char)0x69,  /* [6622] */
    (xdc_Char)0x2e,  /* [6623] */
    (xdc_Char)0x0,  /* [6624] */
    (xdc_Char)0x73,  /* [6625] */
    (xdc_Char)0x79,  /* [6626] */
    (xdc_Char)0x73,  /* [6627] */
    (xdc_Char)0x62,  /* [6628] */
    (xdc_Char)0x69,  /* [6629] */
    (xdc_Char)0x6f,  /* [6630] */
    (xdc_Char)0x73,  /* [6631] */
    (xdc_Char)0x2e,  /* [6632] */
    (xdc_Char)0x0,  /* [6633] */
    (xdc_Char)0x66,  /* [6634] */
    (xdc_Char)0x61,  /* [6635] */
    (xdc_Char)0x6d,  /* [6636] */
    (xdc_Char)0x69,  /* [6637] */
    (xdc_Char)0x6c,  /* [6638] */
    (xdc_Char)0x79,  /* [6639] */
    (xdc_Char)0x2e,  /* [6640] */
    (xdc_Char)0x0,  /* [6641] */
    (xdc_Char)0x61,  /* [6642] */
    (xdc_Char)0x72,  /* [6643] */
    (xdc_Char)0x6d,  /* [6644] */
    (xdc_Char)0x2e,  /* [6645] */
    (xdc_Char)0x0,  /* [6646] */
    (xdc_Char)0x49,  /* [6647] */
    (xdc_Char)0x6e,  /* [6648] */
    (xdc_Char)0x74,  /* [6649] */
    (xdc_Char)0x72,  /* [6650] */
    (xdc_Char)0x69,  /* [6651] */
    (xdc_Char)0x6e,  /* [6652] */
    (xdc_Char)0x73,  /* [6653] */
    (xdc_Char)0x69,  /* [6654] */
    (xdc_Char)0x63,  /* [6655] */
    (xdc_Char)0x73,  /* [6656] */
    (xdc_Char)0x53,  /* [6657] */
    (xdc_Char)0x75,  /* [6658] */
    (xdc_Char)0x70,  /* [6659] */
    (xdc_Char)0x70,  /* [6660] */
    (xdc_Char)0x6f,  /* [6661] */
    (xdc_Char)0x72,  /* [6662] */
    (xdc_Char)0x74,  /* [6663] */
    (xdc_Char)0x0,  /* [6664] */
    (xdc_Char)0x54,  /* [6665] */
    (xdc_Char)0x61,  /* [6666] */
    (xdc_Char)0x73,  /* [6667] */
    (xdc_Char)0x6b,  /* [6668] */
    (xdc_Char)0x53,  /* [6669] */
    (xdc_Char)0x75,  /* [6670] */
    (xdc_Char)0x70,  /* [6671] */
    (xdc_Char)0x70,  /* [6672] */
    (xdc_Char)0x6f,  /* [6673] */
    (xdc_Char)0x72,  /* [6674] */
    (xdc_Char)0x74,  /* [6675] */
    (xdc_Char)0x0,  /* [6676] */
    (xdc_Char)0x42,  /* [6677] */
    (xdc_Char)0x49,  /* [6678] */
    (xdc_Char)0x4f,  /* [6679] */
    (xdc_Char)0x53,  /* [6680] */
    (xdc_Char)0x0,  /* [6681] */
    (xdc_Char)0x6b,  /* [6682] */
    (xdc_Char)0x6e,  /* [6683] */
    (xdc_Char)0x6c,  /* [6684] */
    (xdc_Char)0x2e,  /* [6685] */
    (xdc_Char)0x0,  /* [6686] */
    (xdc_Char)0x43,  /* [6687] */
    (xdc_Char)0x6c,  /* [6688] */
    (xdc_Char)0x6f,  /* [6689] */
    (xdc_Char)0x63,  /* [6690] */
    (xdc_Char)0x6b,  /* [6691] */
    (xdc_Char)0x0,  /* [6692] */
    (xdc_Char)0x49,  /* [6693] */
    (xdc_Char)0x64,  /* [6694] */
    (xdc_Char)0x6c,  /* [6695] */
    (xdc_Char)0x65,  /* [6696] */
    (xdc_Char)0x0,  /* [6697] */
    (xdc_Char)0x49,  /* [6698] */
    (xdc_Char)0x6e,  /* [6699] */
    (xdc_Char)0x74,  /* [6700] */
    (xdc_Char)0x72,  /* [6701] */
    (xdc_Char)0x69,  /* [6702] */
    (xdc_Char)0x6e,  /* [6703] */
    (xdc_Char)0x73,  /* [6704] */
    (xdc_Char)0x69,  /* [6705] */
    (xdc_Char)0x63,  /* [6706] */
    (xdc_Char)0x73,  /* [6707] */
    (xdc_Char)0x0,  /* [6708] */
    (xdc_Char)0x45,  /* [6709] */
    (xdc_Char)0x76,  /* [6710] */
    (xdc_Char)0x65,  /* [6711] */
    (xdc_Char)0x6e,  /* [6712] */
    (xdc_Char)0x74,  /* [6713] */
    (xdc_Char)0x0,  /* [6714] */
    (xdc_Char)0x51,  /* [6715] */
    (xdc_Char)0x75,  /* [6716] */
    (xdc_Char)0x65,  /* [6717] */
    (xdc_Char)0x75,  /* [6718] */
    (xdc_Char)0x65,  /* [6719] */
    (xdc_Char)0x0,  /* [6720] */
    (xdc_Char)0x53,  /* [6721] */
    (xdc_Char)0x65,  /* [6722] */
    (xdc_Char)0x6d,  /* [6723] */
    (xdc_Char)0x61,  /* [6724] */
    (xdc_Char)0x70,  /* [6725] */
    (xdc_Char)0x68,  /* [6726] */
    (xdc_Char)0x6f,  /* [6727] */
    (xdc_Char)0x72,  /* [6728] */
    (xdc_Char)0x65,  /* [6729] */
    (xdc_Char)0x0,  /* [6730] */
    (xdc_Char)0x53,  /* [6731] */
    (xdc_Char)0x77,  /* [6732] */
    (xdc_Char)0x69,  /* [6733] */
    (xdc_Char)0x0,  /* [6734] */
    (xdc_Char)0x54,  /* [6735] */
    (xdc_Char)0x61,  /* [6736] */
    (xdc_Char)0x73,  /* [6737] */
    (xdc_Char)0x6b,  /* [6738] */
    (xdc_Char)0x0,  /* [6739] */
    (xdc_Char)0x68,  /* [6740] */
    (xdc_Char)0x65,  /* [6741] */
    (xdc_Char)0x61,  /* [6742] */
    (xdc_Char)0x70,  /* [6743] */
    (xdc_Char)0x73,  /* [6744] */
    (xdc_Char)0x2e,  /* [6745] */
    (xdc_Char)0x0,  /* [6746] */
    (xdc_Char)0x48,  /* [6747] */
    (xdc_Char)0x65,  /* [6748] */
    (xdc_Char)0x61,  /* [6749] */
    (xdc_Char)0x70,  /* [6750] */
    (xdc_Char)0x42,  /* [6751] */
    (xdc_Char)0x75,  /* [6752] */
    (xdc_Char)0x66,  /* [6753] */
    (xdc_Char)0x0,  /* [6754] */
    (xdc_Char)0x48,  /* [6755] */
    (xdc_Char)0x65,  /* [6756] */
    (xdc_Char)0x61,  /* [6757] */
    (xdc_Char)0x70,  /* [6758] */
    (xdc_Char)0x4d,  /* [6759] */
    (xdc_Char)0x65,  /* [6760] */
    (xdc_Char)0x6d,  /* [6761] */
    (xdc_Char)0x0,  /* [6762] */
    (xdc_Char)0x76,  /* [6763] */
    (xdc_Char)0x37,  /* [6764] */
    (xdc_Char)0x72,  /* [6765] */
    (xdc_Char)0x2e,  /* [6766] */
    (xdc_Char)0x0,  /* [6767] */
    (xdc_Char)0x76,  /* [6768] */
    (xdc_Char)0x69,  /* [6769] */
    (xdc_Char)0x6d,  /* [6770] */
    (xdc_Char)0x2e,  /* [6771] */
    (xdc_Char)0x0,  /* [6772] */
    (xdc_Char)0x48,  /* [6773] */
    (xdc_Char)0x77,  /* [6774] */
    (xdc_Char)0x69,  /* [6775] */
    (xdc_Char)0x0,  /* [6776] */
    (xdc_Char)0x65,  /* [6777] */
    (xdc_Char)0x78,  /* [6778] */
    (xdc_Char)0x63,  /* [6779] */
    (xdc_Char)0x2e,  /* [6780] */
    (xdc_Char)0x0,  /* [6781] */
    (xdc_Char)0x45,  /* [6782] */
    (xdc_Char)0x78,  /* [6783] */
    (xdc_Char)0x63,  /* [6784] */
    (xdc_Char)0x65,  /* [6785] */
    (xdc_Char)0x70,  /* [6786] */
    (xdc_Char)0x74,  /* [6787] */
    (xdc_Char)0x69,  /* [6788] */
    (xdc_Char)0x6f,  /* [6789] */
    (xdc_Char)0x6e,  /* [6790] */
    (xdc_Char)0x0,  /* [6791] */
    (xdc_Char)0x67,  /* [6792] */
    (xdc_Char)0x61,  /* [6793] */
    (xdc_Char)0x74,  /* [6794] */
    (xdc_Char)0x65,  /* [6795] */
    (xdc_Char)0x73,  /* [6796] */
    (xdc_Char)0x2e,  /* [6797] */
    (xdc_Char)0x0,  /* [6798] */
    (xdc_Char)0x47,  /* [6799] */
    (xdc_Char)0x61,  /* [6800] */
    (xdc_Char)0x74,  /* [6801] */
    (xdc_Char)0x65,  /* [6802] */
    (xdc_Char)0x48,  /* [6803] */
    (xdc_Char)0x77,  /* [6804] */
    (xdc_Char)0x69,  /* [6805] */
    (xdc_Char)0x0,  /* [6806] */
    (xdc_Char)0x47,  /* [6807] */
    (xdc_Char)0x61,  /* [6808] */
    (xdc_Char)0x74,  /* [6809] */
    (xdc_Char)0x65,  /* [6810] */
    (xdc_Char)0x4d,  /* [6811] */
    (xdc_Char)0x75,  /* [6812] */
    (xdc_Char)0x74,  /* [6813] */
    (xdc_Char)0x65,  /* [6814] */
    (xdc_Char)0x78,  /* [6815] */
    (xdc_Char)0x0,  /* [6816] */
    (xdc_Char)0x68,  /* [6817] */
    (xdc_Char)0x61,  /* [6818] */
    (xdc_Char)0x6c,  /* [6819] */
    (xdc_Char)0x2e,  /* [6820] */
    (xdc_Char)0x0,  /* [6821] */
    (xdc_Char)0x43,  /* [6822] */
    (xdc_Char)0x61,  /* [6823] */
    (xdc_Char)0x63,  /* [6824] */
    (xdc_Char)0x68,  /* [6825] */
    (xdc_Char)0x65,  /* [6826] */
    (xdc_Char)0x0,  /* [6827] */
    (xdc_Char)0x43,  /* [6828] */
    (xdc_Char)0x61,  /* [6829] */
    (xdc_Char)0x63,  /* [6830] */
    (xdc_Char)0x68,  /* [6831] */
    (xdc_Char)0x65,  /* [6832] */
    (xdc_Char)0x4e,  /* [6833] */
    (xdc_Char)0x75,  /* [6834] */
    (xdc_Char)0x6c,  /* [6835] */
    (xdc_Char)0x6c,  /* [6836] */
    (xdc_Char)0x0,  /* [6837] */
    (xdc_Char)0x74,  /* [6838] */
    (xdc_Char)0x6d,  /* [6839] */
    (xdc_Char)0x73,  /* [6840] */
    (xdc_Char)0x35,  /* [6841] */
    (xdc_Char)0x37,  /* [6842] */
    (xdc_Char)0x30,  /* [6843] */
    (xdc_Char)0x2e,  /* [6844] */
    (xdc_Char)0x0,  /* [6845] */
    (xdc_Char)0x74,  /* [6846] */
    (xdc_Char)0x69,  /* [6847] */
    (xdc_Char)0x6d,  /* [6848] */
    (xdc_Char)0x65,  /* [6849] */
    (xdc_Char)0x72,  /* [6850] */
    (xdc_Char)0x73,  /* [6851] */
    (xdc_Char)0x2e,  /* [6852] */
    (xdc_Char)0x0,  /* [6853] */
    (xdc_Char)0x72,  /* [6854] */
    (xdc_Char)0x74,  /* [6855] */
    (xdc_Char)0x69,  /* [6856] */
    (xdc_Char)0x2e,  /* [6857] */
    (xdc_Char)0x0,  /* [6858] */
    (xdc_Char)0x54,  /* [6859] */
    (xdc_Char)0x69,  /* [6860] */
    (xdc_Char)0x6d,  /* [6861] */
    (xdc_Char)0x65,  /* [6862] */
    (xdc_Char)0x72,  /* [6863] */
    (xdc_Char)0x0,  /* [6864] */
    (xdc_Char)0x74,  /* [6865] */
    (xdc_Char)0x69,  /* [6866] */
    (xdc_Char)0x2e,  /* [6867] */
    (xdc_Char)0x73,  /* [6868] */
    (xdc_Char)0x79,  /* [6869] */
    (xdc_Char)0x73,  /* [6870] */
    (xdc_Char)0x62,  /* [6871] */
    (xdc_Char)0x69,  /* [6872] */
    (xdc_Char)0x6f,  /* [6873] */
    (xdc_Char)0x73,  /* [6874] */
    (xdc_Char)0x2e,  /* [6875] */
    (xdc_Char)0x6b,  /* [6876] */
    (xdc_Char)0x6e,  /* [6877] */
    (xdc_Char)0x6c,  /* [6878] */
    (xdc_Char)0x2e,  /* [6879] */
    (xdc_Char)0x54,  /* [6880] */
    (xdc_Char)0x61,  /* [6881] */
    (xdc_Char)0x73,  /* [6882] */
    (xdc_Char)0x6b,  /* [6883] */
    (xdc_Char)0x2e,  /* [6884] */
    (xdc_Char)0x49,  /* [6885] */
    (xdc_Char)0x64,  /* [6886] */
    (xdc_Char)0x6c,  /* [6887] */
    (xdc_Char)0x65,  /* [6888] */
    (xdc_Char)0x54,  /* [6889] */
    (xdc_Char)0x61,  /* [6890] */
    (xdc_Char)0x73,  /* [6891] */
    (xdc_Char)0x6b,  /* [6892] */
    (xdc_Char)0x0,  /* [6893] */
};

/* --> xdc_runtime_Text_nodeTab__A */
#pragma DATA_SECTION(xdc_runtime_Text_nodeTab__A, ".const:xdc_runtime_Text_nodeTab__A");
const __T1_xdc_runtime_Text_nodeTab xdc_runtime_Text_nodeTab__A[52] = {
    {
        (xdc_Bits16)0x0,  /* left */
        (xdc_Bits16)0x0,  /* right */
    },  /* [0] */
    {
        (xdc_Bits16)0x1975,  /* left */
        (xdc_Bits16)0x197a,  /* right */
    },  /* [1] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0x1983,  /* right */
    },  /* [2] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0x198a,  /* right */
    },  /* [3] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0x198f,  /* right */
    },  /* [4] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0x1998,  /* right */
    },  /* [5] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0x199e,  /* right */
    },  /* [6] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0x19a4,  /* right */
    },  /* [7] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0x19a9,  /* right */
    },  /* [8] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0x19ad,  /* right */
    },  /* [9] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0x19b2,  /* right */
    },  /* [10] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0x19b9,  /* right */
    },  /* [11] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0x19c2,  /* right */
    },  /* [12] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0x19ca,  /* right */
    },  /* [13] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0x19d1,  /* right */
    },  /* [14] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0x19d8,  /* right */
    },  /* [15] */
    {
        (xdc_Bits16)0x19dd,  /* left */
        (xdc_Bits16)0x19e1,  /* right */
    },  /* [16] */
    {
        (xdc_Bits16)0x8010,  /* left */
        (xdc_Bits16)0x19ea,  /* right */
    },  /* [17] */
    {
        (xdc_Bits16)0x8011,  /* left */
        (xdc_Bits16)0x19f2,  /* right */
    },  /* [18] */
    {
        (xdc_Bits16)0x8012,  /* left */
        (xdc_Bits16)0x19f7,  /* right */
    },  /* [19] */
    {
        (xdc_Bits16)0x8012,  /* left */
        (xdc_Bits16)0x1a09,  /* right */
    },  /* [20] */
    {
        (xdc_Bits16)0x8010,  /* left */
        (xdc_Bits16)0x1a15,  /* right */
    },  /* [21] */
    {
        (xdc_Bits16)0x8010,  /* left */
        (xdc_Bits16)0x1a1a,  /* right */
    },  /* [22] */
    {
        (xdc_Bits16)0x8016,  /* left */
        (xdc_Bits16)0x1a1f,  /* right */
    },  /* [23] */
    {
        (xdc_Bits16)0x8016,  /* left */
        (xdc_Bits16)0x1a25,  /* right */
    },  /* [24] */
    {
        (xdc_Bits16)0x8016,  /* left */
        (xdc_Bits16)0x1a2a,  /* right */
    },  /* [25] */
    {
        (xdc_Bits16)0x8016,  /* left */
        (xdc_Bits16)0x1a35,  /* right */
    },  /* [26] */
    {
        (xdc_Bits16)0x8016,  /* left */
        (xdc_Bits16)0x1a3b,  /* right */
    },  /* [27] */
    {
        (xdc_Bits16)0x8016,  /* left */
        (xdc_Bits16)0x1a41,  /* right */
    },  /* [28] */
    {
        (xdc_Bits16)0x8016,  /* left */
        (xdc_Bits16)0x1a4b,  /* right */
    },  /* [29] */
    {
        (xdc_Bits16)0x8016,  /* left */
        (xdc_Bits16)0x1a4f,  /* right */
    },  /* [30] */
    {
        (xdc_Bits16)0x8010,  /* left */
        (xdc_Bits16)0x1a54,  /* right */
    },  /* [31] */
    {
        (xdc_Bits16)0x801f,  /* left */
        (xdc_Bits16)0x1a5b,  /* right */
    },  /* [32] */
    {
        (xdc_Bits16)0x801f,  /* left */
        (xdc_Bits16)0x1a63,  /* right */
    },  /* [33] */
    {
        (xdc_Bits16)0x8012,  /* left */
        (xdc_Bits16)0x1a6b,  /* right */
    },  /* [34] */
    {
        (xdc_Bits16)0x8022,  /* left */
        (xdc_Bits16)0x1a70,  /* right */
    },  /* [35] */
    {
        (xdc_Bits16)0x8023,  /* left */
        (xdc_Bits16)0x1a75,  /* right */
    },  /* [36] */
    {
        (xdc_Bits16)0x8012,  /* left */
        (xdc_Bits16)0x1a79,  /* right */
    },  /* [37] */
    {
        (xdc_Bits16)0x8025,  /* left */
        (xdc_Bits16)0x1a7e,  /* right */
    },  /* [38] */
    {
        (xdc_Bits16)0x8010,  /* left */
        (xdc_Bits16)0x1a88,  /* right */
    },  /* [39] */
    {
        (xdc_Bits16)0x8027,  /* left */
        (xdc_Bits16)0x1a8f,  /* right */
    },  /* [40] */
    {
        (xdc_Bits16)0x8027,  /* left */
        (xdc_Bits16)0x1a97,  /* right */
    },  /* [41] */
    {
        (xdc_Bits16)0x8010,  /* left */
        (xdc_Bits16)0x1aa1,  /* right */
    },  /* [42] */
    {
        (xdc_Bits16)0x802a,  /* left */
        (xdc_Bits16)0x1aa6,  /* right */
    },  /* [43] */
    {
        (xdc_Bits16)0x802a,  /* left */
        (xdc_Bits16)0x1aac,  /* right */
    },  /* [44] */
    {
        (xdc_Bits16)0x802a,  /* left */
        (xdc_Bits16)0x198a,  /* right */
    },  /* [45] */
    {
        (xdc_Bits16)0x802a,  /* left */
        (xdc_Bits16)0x1a75,  /* right */
    },  /* [46] */
    {
        (xdc_Bits16)0x8022,  /* left */
        (xdc_Bits16)0x1ab6,  /* right */
    },  /* [47] */
    {
        (xdc_Bits16)0x802f,  /* left */
        (xdc_Bits16)0x198a,  /* right */
    },  /* [48] */
    {
        (xdc_Bits16)0x8010,  /* left */
        (xdc_Bits16)0x1abe,  /* right */
    },  /* [49] */
    {
        (xdc_Bits16)0x8031,  /* left */
        (xdc_Bits16)0x1ac6,  /* right */
    },  /* [50] */
    {
        (xdc_Bits16)0x8032,  /* left */
        (xdc_Bits16)0x1acb,  /* right */
    },  /* [51] */
};

/* Module__diagsEnabled__C */
#pragma DATA_SECTION(xdc_runtime_Text_Module__diagsEnabled__C, ".const:xdc_runtime_Text_Module__diagsEnabled__C");
__FAR__ const CT__xdc_runtime_Text_Module__diagsEnabled xdc_runtime_Text_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
#pragma DATA_SECTION(xdc_runtime_Text_Module__diagsIncluded__C, ".const:xdc_runtime_Text_Module__diagsIncluded__C");
__FAR__ const CT__xdc_runtime_Text_Module__diagsIncluded xdc_runtime_Text_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
#pragma DATA_SECTION(xdc_runtime_Text_Module__diagsMask__C, ".const:xdc_runtime_Text_Module__diagsMask__C");
__FAR__ const CT__xdc_runtime_Text_Module__diagsMask xdc_runtime_Text_Module__diagsMask__C = ((CT__xdc_runtime_Text_Module__diagsMask)0);

/* Module__gateObj__C */
#pragma DATA_SECTION(xdc_runtime_Text_Module__gateObj__C, ".const:xdc_runtime_Text_Module__gateObj__C");
__FAR__ const CT__xdc_runtime_Text_Module__gateObj xdc_runtime_Text_Module__gateObj__C = ((CT__xdc_runtime_Text_Module__gateObj)0);

/* Module__gatePrms__C */
#pragma DATA_SECTION(xdc_runtime_Text_Module__gatePrms__C, ".const:xdc_runtime_Text_Module__gatePrms__C");
__FAR__ const CT__xdc_runtime_Text_Module__gatePrms xdc_runtime_Text_Module__gatePrms__C = ((CT__xdc_runtime_Text_Module__gatePrms)0);

/* Module__id__C */
#pragma DATA_SECTION(xdc_runtime_Text_Module__id__C, ".const:xdc_runtime_Text_Module__id__C");
__FAR__ const CT__xdc_runtime_Text_Module__id xdc_runtime_Text_Module__id__C = (xdc_Bits16)0x800f;

/* Module__loggerDefined__C */
#pragma DATA_SECTION(xdc_runtime_Text_Module__loggerDefined__C, ".const:xdc_runtime_Text_Module__loggerDefined__C");
__FAR__ const CT__xdc_runtime_Text_Module__loggerDefined xdc_runtime_Text_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
#pragma DATA_SECTION(xdc_runtime_Text_Module__loggerObj__C, ".const:xdc_runtime_Text_Module__loggerObj__C");
__FAR__ const CT__xdc_runtime_Text_Module__loggerObj xdc_runtime_Text_Module__loggerObj__C = ((CT__xdc_runtime_Text_Module__loggerObj)0);

/* Module__loggerFxn0__C */
#pragma DATA_SECTION(xdc_runtime_Text_Module__loggerFxn0__C, ".const:xdc_runtime_Text_Module__loggerFxn0__C");
__FAR__ const CT__xdc_runtime_Text_Module__loggerFxn0 xdc_runtime_Text_Module__loggerFxn0__C = ((CT__xdc_runtime_Text_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
#pragma DATA_SECTION(xdc_runtime_Text_Module__loggerFxn1__C, ".const:xdc_runtime_Text_Module__loggerFxn1__C");
__FAR__ const CT__xdc_runtime_Text_Module__loggerFxn1 xdc_runtime_Text_Module__loggerFxn1__C = ((CT__xdc_runtime_Text_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
#pragma DATA_SECTION(xdc_runtime_Text_Module__loggerFxn2__C, ".const:xdc_runtime_Text_Module__loggerFxn2__C");
__FAR__ const CT__xdc_runtime_Text_Module__loggerFxn2 xdc_runtime_Text_Module__loggerFxn2__C = ((CT__xdc_runtime_Text_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
#pragma DATA_SECTION(xdc_runtime_Text_Module__loggerFxn4__C, ".const:xdc_runtime_Text_Module__loggerFxn4__C");
__FAR__ const CT__xdc_runtime_Text_Module__loggerFxn4 xdc_runtime_Text_Module__loggerFxn4__C = ((CT__xdc_runtime_Text_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
#pragma DATA_SECTION(xdc_runtime_Text_Module__loggerFxn8__C, ".const:xdc_runtime_Text_Module__loggerFxn8__C");
__FAR__ const CT__xdc_runtime_Text_Module__loggerFxn8 xdc_runtime_Text_Module__loggerFxn8__C = ((CT__xdc_runtime_Text_Module__loggerFxn8)0);

/* Object__count__C */
#pragma DATA_SECTION(xdc_runtime_Text_Object__count__C, ".const:xdc_runtime_Text_Object__count__C");
__FAR__ const CT__xdc_runtime_Text_Object__count xdc_runtime_Text_Object__count__C = 0;

/* Object__heap__C */
#pragma DATA_SECTION(xdc_runtime_Text_Object__heap__C, ".const:xdc_runtime_Text_Object__heap__C");
__FAR__ const CT__xdc_runtime_Text_Object__heap xdc_runtime_Text_Object__heap__C = 0;

/* Object__sizeof__C */
#pragma DATA_SECTION(xdc_runtime_Text_Object__sizeof__C, ".const:xdc_runtime_Text_Object__sizeof__C");
__FAR__ const CT__xdc_runtime_Text_Object__sizeof xdc_runtime_Text_Object__sizeof__C = 0;

/* Object__table__C */
#pragma DATA_SECTION(xdc_runtime_Text_Object__table__C, ".const:xdc_runtime_Text_Object__table__C");
__FAR__ const CT__xdc_runtime_Text_Object__table xdc_runtime_Text_Object__table__C = 0;

/* nameUnknown__C */
#pragma DATA_SECTION(xdc_runtime_Text_nameUnknown__C, ".const:xdc_runtime_Text_nameUnknown__C");
__FAR__ const CT__xdc_runtime_Text_nameUnknown xdc_runtime_Text_nameUnknown__C = "{unknown-instance-name}";

/* nameEmpty__C */
#pragma DATA_SECTION(xdc_runtime_Text_nameEmpty__C, ".const:xdc_runtime_Text_nameEmpty__C");
__FAR__ const CT__xdc_runtime_Text_nameEmpty xdc_runtime_Text_nameEmpty__C = "{empty-instance-name}";

/* nameStatic__C */
#pragma DATA_SECTION(xdc_runtime_Text_nameStatic__C, ".const:xdc_runtime_Text_nameStatic__C");
__FAR__ const CT__xdc_runtime_Text_nameStatic xdc_runtime_Text_nameStatic__C = "{static-instance-name}";

/* isLoaded__C */
#pragma DATA_SECTION(xdc_runtime_Text_isLoaded__C, ".const:xdc_runtime_Text_isLoaded__C");
__FAR__ const CT__xdc_runtime_Text_isLoaded xdc_runtime_Text_isLoaded__C = 1;

/* charTab__C */
#pragma DATA_SECTION(xdc_runtime_Text_charTab__C, ".const:xdc_runtime_Text_charTab__C");
__FAR__ const CT__xdc_runtime_Text_charTab xdc_runtime_Text_charTab__C = ((CT__xdc_runtime_Text_charTab)xdc_runtime_Text_charTab__A);

/* nodeTab__C */
#pragma DATA_SECTION(xdc_runtime_Text_nodeTab__C, ".const:xdc_runtime_Text_nodeTab__C");
__FAR__ const CT__xdc_runtime_Text_nodeTab xdc_runtime_Text_nodeTab__C = ((CT__xdc_runtime_Text_nodeTab)xdc_runtime_Text_nodeTab__A);

/* charCnt__C */
#pragma DATA_SECTION(xdc_runtime_Text_charCnt__C, ".const:xdc_runtime_Text_charCnt__C");
__FAR__ const CT__xdc_runtime_Text_charCnt xdc_runtime_Text_charCnt__C = (xdc_Int16)0x1aee;

/* nodeCnt__C */
#pragma DATA_SECTION(xdc_runtime_Text_nodeCnt__C, ".const:xdc_runtime_Text_nodeCnt__C");
__FAR__ const CT__xdc_runtime_Text_nodeCnt xdc_runtime_Text_nodeCnt__C = (xdc_Int16)0x34;

/* unnamedModsLastId__C */
#pragma DATA_SECTION(xdc_runtime_Text_unnamedModsLastId__C, ".const:xdc_runtime_Text_unnamedModsLastId__C");
__FAR__ const CT__xdc_runtime_Text_unnamedModsLastId xdc_runtime_Text_unnamedModsLastId__C = (xdc_UInt16)0x4000;

/* registryModsLastId__C */
#pragma DATA_SECTION(xdc_runtime_Text_registryModsLastId__C, ".const:xdc_runtime_Text_registryModsLastId__C");
__FAR__ const CT__xdc_runtime_Text_registryModsLastId xdc_runtime_Text_registryModsLastId__C = (xdc_UInt16)0x7fff;

/* visitRopeFxn__C */
#pragma DATA_SECTION(xdc_runtime_Text_visitRopeFxn__C, ".const:xdc_runtime_Text_visitRopeFxn__C");
__FAR__ const CT__xdc_runtime_Text_visitRopeFxn xdc_runtime_Text_visitRopeFxn__C = ((CT__xdc_runtime_Text_visitRopeFxn)((xdc_Fxn)xdc_runtime_Text_visitRope__I));

/* visitRopeFxn2__C */
#pragma DATA_SECTION(xdc_runtime_Text_visitRopeFxn2__C, ".const:xdc_runtime_Text_visitRopeFxn2__C");
__FAR__ const CT__xdc_runtime_Text_visitRopeFxn2 xdc_runtime_Text_visitRopeFxn2__C = ((CT__xdc_runtime_Text_visitRopeFxn2)((xdc_Fxn)xdc_runtime_Text_visitRope2__I));


/*
 * ======== xdc.runtime.System FUNCTION STUBS ========
 */

/* printf_va__E */
xdc_Int xdc_runtime_System_printf_va__E( xdc_CString fmt, va_list __va ) 
{
    return xdc_runtime_System_printf_va__F(fmt, __va);
}

/* printf__E */
xdc_Int xdc_runtime_System_printf__E( xdc_CString fmt, ... ) 
{
    xdc_Int __ret;

    va_list __va; va_start(__va, fmt);
    __ret = xdc_runtime_System_printf_va__F(fmt, __va);

    va_end(__va);
    return __ret;
}

/* aprintf_va__E */
xdc_Int xdc_runtime_System_aprintf_va__E( xdc_CString fmt, va_list __va ) 
{
    return xdc_runtime_System_aprintf_va__F(fmt, __va);
}

/* aprintf__E */
xdc_Int xdc_runtime_System_aprintf__E( xdc_CString fmt, ... ) 
{
    xdc_Int __ret;

    va_list __va; va_start(__va, fmt);
    __ret = xdc_runtime_System_aprintf_va__F(fmt, __va);

    va_end(__va);
    return __ret;
}

/* sprintf_va__E */
xdc_Int xdc_runtime_System_sprintf_va__E( xdc_Char buf[], xdc_CString fmt, va_list __va ) 
{
    return xdc_runtime_System_sprintf_va__F(buf, fmt, __va);
}

/* sprintf__E */
xdc_Int xdc_runtime_System_sprintf__E( xdc_Char buf[], xdc_CString fmt, ... ) 
{
    xdc_Int __ret;

    va_list __va; va_start(__va, fmt);
    __ret = xdc_runtime_System_sprintf_va__F(buf, fmt, __va);

    va_end(__va);
    return __ret;
}

/* asprintf_va__E */
xdc_Int xdc_runtime_System_asprintf_va__E( xdc_Char buf[], xdc_CString fmt, va_list __va ) 
{
    return xdc_runtime_System_asprintf_va__F(buf, fmt, __va);
}

/* asprintf__E */
xdc_Int xdc_runtime_System_asprintf__E( xdc_Char buf[], xdc_CString fmt, ... ) 
{
    xdc_Int __ret;

    va_list __va; va_start(__va, fmt);
    __ret = xdc_runtime_System_asprintf_va__F(buf, fmt, __va);

    va_end(__va);
    return __ret;
}

/* snprintf_va__E */
xdc_Int xdc_runtime_System_snprintf_va__E( xdc_Char buf[], xdc_SizeT n, xdc_CString fmt, va_list __va ) 
{
    return xdc_runtime_System_snprintf_va__F(buf, n, fmt, __va);
}

/* snprintf__E */
xdc_Int xdc_runtime_System_snprintf__E( xdc_Char buf[], xdc_SizeT n, xdc_CString fmt, ... ) 
{
    xdc_Int __ret;

    va_list __va; va_start(__va, fmt);
    __ret = xdc_runtime_System_snprintf_va__F(buf, n, fmt, __va);

    va_end(__va);
    return __ret;
}


/*
 * ======== ti.sysbios.BIOS_RtsGateProxy PROXY BODY ========
 */

/* DELEGATES TO ti.sysbios.gates.GateMutex */

/* Module__startupDone__S */
xdc_Bool ti_sysbios_BIOS_RtsGateProxy_Module__startupDone__S( void ) 
{
    return ti_sysbios_gates_GateMutex_Module__startupDone__S();
}

/* create */
ti_sysbios_BIOS_RtsGateProxy_Handle ti_sysbios_BIOS_RtsGateProxy_create( const ti_sysbios_BIOS_RtsGateProxy_Params *prms, xdc_runtime_Error_Block *eb )
{
    return (ti_sysbios_BIOS_RtsGateProxy_Handle)ti_sysbios_gates_GateMutex_create((const ti_sysbios_gates_GateMutex_Params *)prms, eb);
}

/* delete */
void ti_sysbios_BIOS_RtsGateProxy_delete(ti_sysbios_BIOS_RtsGateProxy_Handle *instp)
{
    ti_sysbios_gates_GateMutex_Object__delete__S(instp);
}

/* Params__init__S */
void ti_sysbios_BIOS_RtsGateProxy_Params__init__S( xdc_Ptr dst, const void *src, xdc_SizeT psz, xdc_SizeT isz )
{
    ti_sysbios_gates_GateMutex_Params__init__S(dst, src, psz, isz);
}

/* Handle__label__S */
xdc_runtime_Types_Label *ti_sysbios_BIOS_RtsGateProxy_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label *lab)
{
    return ti_sysbios_gates_GateMutex_Handle__label__S(obj, lab);
}

/* query__E */
xdc_Bool ti_sysbios_BIOS_RtsGateProxy_query__E( xdc_Int qual )
{
    return ti_sysbios_gates_GateMutex_query(qual);
}

/* enter__E */
xdc_IArg ti_sysbios_BIOS_RtsGateProxy_enter__E( ti_sysbios_BIOS_RtsGateProxy_Handle __inst )
{
    return ti_sysbios_gates_GateMutex_enter((ti_sysbios_gates_GateMutex_Handle)__inst);
}

/* leave__E */
xdc_Void ti_sysbios_BIOS_RtsGateProxy_leave__E( ti_sysbios_BIOS_RtsGateProxy_Handle __inst, xdc_IArg key )
{
    ti_sysbios_gates_GateMutex_leave((ti_sysbios_gates_GateMutex_Handle)__inst, key);
}


/*
 * ======== ti.sysbios.hal.Cache_CacheProxy PROXY BODY ========
 */

/* DELEGATES TO ti.sysbios.hal.CacheNull */

/* Module__startupDone__S */
xdc_Bool ti_sysbios_hal_Cache_CacheProxy_Module__startupDone__S( void ) 
{
    return ti_sysbios_hal_CacheNull_Module__startupDone__S();
}

/* enable__E */
xdc_Void ti_sysbios_hal_Cache_CacheProxy_enable__E( xdc_Bits16 type )
{
    ti_sysbios_hal_CacheNull_enable(type);
}

/* disable__E */
xdc_Void ti_sysbios_hal_Cache_CacheProxy_disable__E( xdc_Bits16 type )
{
    ti_sysbios_hal_CacheNull_disable(type);
}

/* inv__E */
xdc_Void ti_sysbios_hal_Cache_CacheProxy_inv__E( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait )
{
    ti_sysbios_hal_CacheNull_inv(blockPtr, byteCnt, type, wait);
}

/* wb__E */
xdc_Void ti_sysbios_hal_Cache_CacheProxy_wb__E( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait )
{
    ti_sysbios_hal_CacheNull_wb(blockPtr, byteCnt, type, wait);
}

/* wbInv__E */
xdc_Void ti_sysbios_hal_Cache_CacheProxy_wbInv__E( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait )
{
    ti_sysbios_hal_CacheNull_wbInv(blockPtr, byteCnt, type, wait);
}

/* wbAll__E */
xdc_Void ti_sysbios_hal_Cache_CacheProxy_wbAll__E( void )
{
    ti_sysbios_hal_CacheNull_wbAll();
}

/* wbInvAll__E */
xdc_Void ti_sysbios_hal_Cache_CacheProxy_wbInvAll__E( void )
{
    ti_sysbios_hal_CacheNull_wbInvAll();
}

/* wait__E */
xdc_Void ti_sysbios_hal_Cache_CacheProxy_wait__E( void )
{
    ti_sysbios_hal_CacheNull_wait();
}


/*
 * ======== ti.sysbios.hal.Core_CoreProxy PROXY BODY ========
 */

/* DELEGATES TO ti.sysbios.family.arm.v7r.tms570.Core */

/* Module__startupDone__S */
xdc_Bool ti_sysbios_hal_Core_CoreProxy_Module__startupDone__S( void ) 
{
    return ti_sysbios_family_arm_v7r_tms570_Core_Module__startupDone__S();
}

/* getId__E */
xdc_UInt ti_sysbios_hal_Core_CoreProxy_getId__E( void )
{
    return ti_sysbios_family_arm_v7r_tms570_Core_getId();
}

/* interruptCore__E */
xdc_Void ti_sysbios_hal_Core_CoreProxy_interruptCore__E( xdc_UInt coreId )
{
    ti_sysbios_family_arm_v7r_tms570_Core_interruptCore(coreId);
}

/* lock__E */
xdc_IArg ti_sysbios_hal_Core_CoreProxy_lock__E( void )
{
    return ti_sysbios_family_arm_v7r_tms570_Core_lock();
}

/* unlock__E */
xdc_Void ti_sysbios_hal_Core_CoreProxy_unlock__E( void )
{
    ti_sysbios_family_arm_v7r_tms570_Core_unlock();
}

/* hwiDisable__E */
xdc_UInt ti_sysbios_hal_Core_CoreProxy_hwiDisable__E( void )
{
    return ti_sysbios_family_arm_v7r_tms570_Core_hwiDisable();
}

/* hwiEnable__E */
xdc_UInt ti_sysbios_hal_Core_CoreProxy_hwiEnable__E( void )
{
    return ti_sysbios_family_arm_v7r_tms570_Core_hwiEnable();
}

/* hwiRestore__E */
xdc_Void ti_sysbios_hal_Core_CoreProxy_hwiRestore__E( xdc_UInt key )
{
    ti_sysbios_family_arm_v7r_tms570_Core_hwiRestore(key);
}


/*
 * ======== ti.sysbios.hal.Hwi_HwiProxy PROXY BODY ========
 */

/* DELEGATES TO ti.sysbios.family.arm.v7r.vim.Hwi */

/* Module__startupDone__S */
xdc_Bool ti_sysbios_hal_Hwi_HwiProxy_Module__startupDone__S( void ) 
{
    return ti_sysbios_family_arm_v7r_vim_Hwi_Module__startupDone__S();
}

/* create */
ti_sysbios_hal_Hwi_HwiProxy_Handle ti_sysbios_hal_Hwi_HwiProxy_create( xdc_Int intNum, ti_sysbios_interfaces_IHwi_FuncPtr hwiFxn, const ti_sysbios_hal_Hwi_HwiProxy_Params *prms, xdc_runtime_Error_Block *eb )
{
    return (ti_sysbios_hal_Hwi_HwiProxy_Handle)ti_sysbios_family_arm_v7r_vim_Hwi_create(intNum, hwiFxn, (const ti_sysbios_family_arm_v7r_vim_Hwi_Params *)prms, eb);
}

/* delete */
void ti_sysbios_hal_Hwi_HwiProxy_delete(ti_sysbios_hal_Hwi_HwiProxy_Handle *instp)
{
    ti_sysbios_family_arm_v7r_vim_Hwi_Object__delete__S(instp);
}

/* Params__init__S */
void ti_sysbios_hal_Hwi_HwiProxy_Params__init__S( xdc_Ptr dst, const void *src, xdc_SizeT psz, xdc_SizeT isz )
{
    ti_sysbios_family_arm_v7r_vim_Hwi_Params__init__S(dst, src, psz, isz);
}

/* Handle__label__S */
xdc_runtime_Types_Label *ti_sysbios_hal_Hwi_HwiProxy_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label *lab)
{
    return ti_sysbios_family_arm_v7r_vim_Hwi_Handle__label__S(obj, lab);
}

/* getStackInfo__E */
xdc_Bool ti_sysbios_hal_Hwi_HwiProxy_getStackInfo__E( ti_sysbios_interfaces_IHwi_StackInfo *stkInfo, xdc_Bool computeStackDepth )
{
    return ti_sysbios_family_arm_v7r_vim_Hwi_getStackInfo(stkInfo, computeStackDepth);
}

/* getCoreStackInfo__E */
xdc_Bool ti_sysbios_hal_Hwi_HwiProxy_getCoreStackInfo__E( ti_sysbios_interfaces_IHwi_StackInfo *stkInfo, xdc_Bool computeStackDepth, xdc_UInt coreId )
{
    return ti_sysbios_family_arm_v7r_vim_Hwi_getCoreStackInfo(stkInfo, computeStackDepth, coreId);
}

/* startup__E */
xdc_Void ti_sysbios_hal_Hwi_HwiProxy_startup__E( void )
{
    ti_sysbios_family_arm_v7r_vim_Hwi_startup();
}

/* disable__E */
xdc_UInt ti_sysbios_hal_Hwi_HwiProxy_disable__E( void )
{
    return ti_sysbios_family_arm_v7r_vim_Hwi_disable();
}

/* enable__E */
xdc_UInt ti_sysbios_hal_Hwi_HwiProxy_enable__E( void )
{
    return ti_sysbios_family_arm_v7r_vim_Hwi_enable();
}

/* restore__E */
xdc_Void ti_sysbios_hal_Hwi_HwiProxy_restore__E( xdc_UInt key )
{
    ti_sysbios_family_arm_v7r_vim_Hwi_restore(key);
}

/* switchFromBootStack__E */
xdc_Void ti_sysbios_hal_Hwi_HwiProxy_switchFromBootStack__E( void )
{
    ti_sysbios_family_arm_v7r_vim_Hwi_switchFromBootStack();
}

/* post__E */
xdc_Void ti_sysbios_hal_Hwi_HwiProxy_post__E( xdc_UInt intNum )
{
    ti_sysbios_family_arm_v7r_vim_Hwi_post(intNum);
}

/* getTaskSP__E */
xdc_Char *ti_sysbios_hal_Hwi_HwiProxy_getTaskSP__E( void )
{
    return ti_sysbios_family_arm_v7r_vim_Hwi_getTaskSP();
}

/* disableInterrupt__E */
xdc_UInt ti_sysbios_hal_Hwi_HwiProxy_disableInterrupt__E( xdc_UInt intNum )
{
    return ti_sysbios_family_arm_v7r_vim_Hwi_disableInterrupt(intNum);
}

/* enableInterrupt__E */
xdc_UInt ti_sysbios_hal_Hwi_HwiProxy_enableInterrupt__E( xdc_UInt intNum )
{
    return ti_sysbios_family_arm_v7r_vim_Hwi_enableInterrupt(intNum);
}

/* restoreInterrupt__E */
xdc_Void ti_sysbios_hal_Hwi_HwiProxy_restoreInterrupt__E( xdc_UInt intNum, xdc_UInt key )
{
    ti_sysbios_family_arm_v7r_vim_Hwi_restoreInterrupt(intNum, key);
}

/* clearInterrupt__E */
xdc_Void ti_sysbios_hal_Hwi_HwiProxy_clearInterrupt__E( xdc_UInt intNum )
{
    ti_sysbios_family_arm_v7r_vim_Hwi_clearInterrupt(intNum);
}

/* getFunc__E */
ti_sysbios_interfaces_IHwi_FuncPtr ti_sysbios_hal_Hwi_HwiProxy_getFunc__E( ti_sysbios_hal_Hwi_HwiProxy_Handle __inst, xdc_UArg *arg )
{
    return ti_sysbios_family_arm_v7r_vim_Hwi_getFunc((ti_sysbios_family_arm_v7r_vim_Hwi_Handle)__inst, arg);
}

/* setFunc__E */
xdc_Void ti_sysbios_hal_Hwi_HwiProxy_setFunc__E( ti_sysbios_hal_Hwi_HwiProxy_Handle __inst, ti_sysbios_interfaces_IHwi_FuncPtr fxn, xdc_UArg arg )
{
    ti_sysbios_family_arm_v7r_vim_Hwi_setFunc((ti_sysbios_family_arm_v7r_vim_Hwi_Handle)__inst, fxn, arg);
}

/* getHookContext__E */
xdc_Ptr ti_sysbios_hal_Hwi_HwiProxy_getHookContext__E( ti_sysbios_hal_Hwi_HwiProxy_Handle __inst, xdc_Int id )
{
    return ti_sysbios_family_arm_v7r_vim_Hwi_getHookContext((ti_sysbios_family_arm_v7r_vim_Hwi_Handle)__inst, id);
}

/* setHookContext__E */
xdc_Void ti_sysbios_hal_Hwi_HwiProxy_setHookContext__E( ti_sysbios_hal_Hwi_HwiProxy_Handle __inst, xdc_Int id, xdc_Ptr hookContext )
{
    ti_sysbios_family_arm_v7r_vim_Hwi_setHookContext((ti_sysbios_family_arm_v7r_vim_Hwi_Handle)__inst, id, hookContext);
}

/* getIrp__E */
ti_sysbios_interfaces_IHwi_Irp ti_sysbios_hal_Hwi_HwiProxy_getIrp__E( ti_sysbios_hal_Hwi_HwiProxy_Handle __inst )
{
    return ti_sysbios_family_arm_v7r_vim_Hwi_getIrp((ti_sysbios_family_arm_v7r_vim_Hwi_Handle)__inst);
}


/*
 * ======== ti.sysbios.heaps.HeapMem_Module_GateProxy PROXY BODY ========
 */

/* DELEGATES TO ti.sysbios.gates.GateMutex */

/* Module__startupDone__S */
xdc_Bool ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__startupDone__S( void ) 
{
    return ti_sysbios_gates_GateMutex_Module__startupDone__S();
}

/* create */
ti_sysbios_heaps_HeapMem_Module_GateProxy_Handle ti_sysbios_heaps_HeapMem_Module_GateProxy_create( const ti_sysbios_heaps_HeapMem_Module_GateProxy_Params *prms, xdc_runtime_Error_Block *eb )
{
    return (ti_sysbios_heaps_HeapMem_Module_GateProxy_Handle)ti_sysbios_gates_GateMutex_create((const ti_sysbios_gates_GateMutex_Params *)prms, eb);
}

/* delete */
void ti_sysbios_heaps_HeapMem_Module_GateProxy_delete(ti_sysbios_heaps_HeapMem_Module_GateProxy_Handle *instp)
{
    ti_sysbios_gates_GateMutex_Object__delete__S(instp);
}

/* Params__init__S */
void ti_sysbios_heaps_HeapMem_Module_GateProxy_Params__init__S( xdc_Ptr dst, const void *src, xdc_SizeT psz, xdc_SizeT isz )
{
    ti_sysbios_gates_GateMutex_Params__init__S(dst, src, psz, isz);
}

/* Handle__label__S */
xdc_runtime_Types_Label *ti_sysbios_heaps_HeapMem_Module_GateProxy_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label *lab)
{
    return ti_sysbios_gates_GateMutex_Handle__label__S(obj, lab);
}

/* query__E */
xdc_Bool ti_sysbios_heaps_HeapMem_Module_GateProxy_query__E( xdc_Int qual )
{
    return ti_sysbios_gates_GateMutex_query(qual);
}

/* enter__E */
xdc_IArg ti_sysbios_heaps_HeapMem_Module_GateProxy_enter__E( ti_sysbios_heaps_HeapMem_Module_GateProxy_Handle __inst )
{
    return ti_sysbios_gates_GateMutex_enter((ti_sysbios_gates_GateMutex_Handle)__inst);
}

/* leave__E */
xdc_Void ti_sysbios_heaps_HeapMem_Module_GateProxy_leave__E( ti_sysbios_heaps_HeapMem_Module_GateProxy_Handle __inst, xdc_IArg key )
{
    ti_sysbios_gates_GateMutex_leave((ti_sysbios_gates_GateMutex_Handle)__inst, key);
}


/*
 * ======== ti.sysbios.knl.Clock_TimerProxy PROXY BODY ========
 */

/* DELEGATES TO ti.sysbios.timers.rti.Timer */

/* Module__startupDone__S */
xdc_Bool ti_sysbios_knl_Clock_TimerProxy_Module__startupDone__S( void ) 
{
    return ti_sysbios_timers_rti_Timer_Module__startupDone__S();
}

/* create */
ti_sysbios_knl_Clock_TimerProxy_Handle ti_sysbios_knl_Clock_TimerProxy_create( xdc_Int id, ti_sysbios_interfaces_ITimer_FuncPtr tickFxn, const ti_sysbios_knl_Clock_TimerProxy_Params *prms, xdc_runtime_Error_Block *eb )
{
    return (ti_sysbios_knl_Clock_TimerProxy_Handle)ti_sysbios_timers_rti_Timer_create(id, tickFxn, (const ti_sysbios_timers_rti_Timer_Params *)prms, eb);
}

/* delete */
void ti_sysbios_knl_Clock_TimerProxy_delete(ti_sysbios_knl_Clock_TimerProxy_Handle *instp)
{
    ti_sysbios_timers_rti_Timer_Object__delete__S(instp);
}

/* Params__init__S */
void ti_sysbios_knl_Clock_TimerProxy_Params__init__S( xdc_Ptr dst, const void *src, xdc_SizeT psz, xdc_SizeT isz )
{
    ti_sysbios_timers_rti_Timer_Params__init__S(dst, src, psz, isz);
}

/* Handle__label__S */
xdc_runtime_Types_Label *ti_sysbios_knl_Clock_TimerProxy_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label *lab)
{
    return ti_sysbios_timers_rti_Timer_Handle__label__S(obj, lab);
}

/* getNumTimers__E */
xdc_UInt ti_sysbios_knl_Clock_TimerProxy_getNumTimers__E( void )
{
    return ti_sysbios_timers_rti_Timer_getNumTimers();
}

/* getStatus__E */
ti_sysbios_interfaces_ITimer_Status ti_sysbios_knl_Clock_TimerProxy_getStatus__E( xdc_UInt id )
{
    return ti_sysbios_timers_rti_Timer_getStatus(id);
}

/* startup__E */
xdc_Void ti_sysbios_knl_Clock_TimerProxy_startup__E( void )
{
    ti_sysbios_timers_rti_Timer_startup();
}

/* getMaxTicks__E */
xdc_UInt32 ti_sysbios_knl_Clock_TimerProxy_getMaxTicks__E( ti_sysbios_knl_Clock_TimerProxy_Handle __inst )
{
    return ti_sysbios_timers_rti_Timer_getMaxTicks((ti_sysbios_timers_rti_Timer_Handle)__inst);
}

/* setNextTick__E */
xdc_Void ti_sysbios_knl_Clock_TimerProxy_setNextTick__E( ti_sysbios_knl_Clock_TimerProxy_Handle __inst, xdc_UInt32 ticks )
{
    ti_sysbios_timers_rti_Timer_setNextTick((ti_sysbios_timers_rti_Timer_Handle)__inst, ticks);
}

/* start__E */
xdc_Void ti_sysbios_knl_Clock_TimerProxy_start__E( ti_sysbios_knl_Clock_TimerProxy_Handle __inst )
{
    ti_sysbios_timers_rti_Timer_start((ti_sysbios_timers_rti_Timer_Handle)__inst);
}

/* stop__E */
xdc_Void ti_sysbios_knl_Clock_TimerProxy_stop__E( ti_sysbios_knl_Clock_TimerProxy_Handle __inst )
{
    ti_sysbios_timers_rti_Timer_stop((ti_sysbios_timers_rti_Timer_Handle)__inst);
}

/* setPeriod__E */
xdc_Void ti_sysbios_knl_Clock_TimerProxy_setPeriod__E( ti_sysbios_knl_Clock_TimerProxy_Handle __inst, xdc_UInt32 period )
{
    ti_sysbios_timers_rti_Timer_setPeriod((ti_sysbios_timers_rti_Timer_Handle)__inst, period);
}

/* setPeriodMicroSecs__E */
xdc_Bool ti_sysbios_knl_Clock_TimerProxy_setPeriodMicroSecs__E( ti_sysbios_knl_Clock_TimerProxy_Handle __inst, xdc_UInt32 microsecs )
{
    return ti_sysbios_timers_rti_Timer_setPeriodMicroSecs((ti_sysbios_timers_rti_Timer_Handle)__inst, microsecs);
}

/* getPeriod__E */
xdc_UInt32 ti_sysbios_knl_Clock_TimerProxy_getPeriod__E( ti_sysbios_knl_Clock_TimerProxy_Handle __inst )
{
    return ti_sysbios_timers_rti_Timer_getPeriod((ti_sysbios_timers_rti_Timer_Handle)__inst);
}

/* getCount__E */
xdc_UInt32 ti_sysbios_knl_Clock_TimerProxy_getCount__E( ti_sysbios_knl_Clock_TimerProxy_Handle __inst )
{
    return ti_sysbios_timers_rti_Timer_getCount((ti_sysbios_timers_rti_Timer_Handle)__inst);
}

/* getFreq__E */
xdc_Void ti_sysbios_knl_Clock_TimerProxy_getFreq__E( ti_sysbios_knl_Clock_TimerProxy_Handle __inst, xdc_runtime_Types_FreqHz *freq )
{
    ti_sysbios_timers_rti_Timer_getFreq((ti_sysbios_timers_rti_Timer_Handle)__inst, freq);
}

/* getFunc__E */
ti_sysbios_interfaces_ITimer_FuncPtr ti_sysbios_knl_Clock_TimerProxy_getFunc__E( ti_sysbios_knl_Clock_TimerProxy_Handle __inst, xdc_UArg *arg )
{
    return ti_sysbios_timers_rti_Timer_getFunc((ti_sysbios_timers_rti_Timer_Handle)__inst, arg);
}

/* setFunc__E */
xdc_Void ti_sysbios_knl_Clock_TimerProxy_setFunc__E( ti_sysbios_knl_Clock_TimerProxy_Handle __inst, ti_sysbios_interfaces_ITimer_FuncPtr fxn, xdc_UArg arg )
{
    ti_sysbios_timers_rti_Timer_setFunc((ti_sysbios_timers_rti_Timer_Handle)__inst, fxn, arg);
}

/* trigger__E */
xdc_Void ti_sysbios_knl_Clock_TimerProxy_trigger__E( ti_sysbios_knl_Clock_TimerProxy_Handle __inst, xdc_UInt32 cycles )
{
    ti_sysbios_timers_rti_Timer_trigger((ti_sysbios_timers_rti_Timer_Handle)__inst, cycles);
}

/* getExpiredCounts__E */
xdc_UInt32 ti_sysbios_knl_Clock_TimerProxy_getExpiredCounts__E( ti_sysbios_knl_Clock_TimerProxy_Handle __inst )
{
    return ti_sysbios_timers_rti_Timer_getExpiredCounts((ti_sysbios_timers_rti_Timer_Handle)__inst);
}

/* getExpiredTicks__E */
xdc_UInt32 ti_sysbios_knl_Clock_TimerProxy_getExpiredTicks__E( ti_sysbios_knl_Clock_TimerProxy_Handle __inst, xdc_UInt32 tickPeriod )
{
    return ti_sysbios_timers_rti_Timer_getExpiredTicks((ti_sysbios_timers_rti_Timer_Handle)__inst, tickPeriod);
}

/* getCurrentTick__E */
xdc_UInt32 ti_sysbios_knl_Clock_TimerProxy_getCurrentTick__E( ti_sysbios_knl_Clock_TimerProxy_Handle __inst, xdc_Bool save )
{
    return ti_sysbios_timers_rti_Timer_getCurrentTick((ti_sysbios_timers_rti_Timer_Handle)__inst, save);
}


/*
 * ======== ti.sysbios.knl.Intrinsics_SupportProxy PROXY BODY ========
 */

/* DELEGATES TO ti.sysbios.family.arm.IntrinsicsSupport */

/* Module__startupDone__S */
xdc_Bool ti_sysbios_knl_Intrinsics_SupportProxy_Module__startupDone__S( void ) 
{
    return ti_sysbios_family_arm_IntrinsicsSupport_Module__startupDone__S();
}

/* maxbit__E */
xdc_UInt ti_sysbios_knl_Intrinsics_SupportProxy_maxbit__E( xdc_UInt bits )
{
    return ti_sysbios_family_arm_IntrinsicsSupport_maxbit(bits);
}


/*
 * ======== ti.sysbios.knl.Task_SupportProxy PROXY BODY ========
 */

/* DELEGATES TO ti.sysbios.family.arm.TaskSupport */

/* Module__startupDone__S */
xdc_Bool ti_sysbios_knl_Task_SupportProxy_Module__startupDone__S( void ) 
{
    return ti_sysbios_family_arm_TaskSupport_Module__startupDone__S();
}

/* start__E */
xdc_Ptr ti_sysbios_knl_Task_SupportProxy_start__E( xdc_Ptr curTask, ti_sysbios_interfaces_ITaskSupport_FuncPtr enterFxn, ti_sysbios_interfaces_ITaskSupport_FuncPtr exitFxn, xdc_runtime_Error_Block *eb )
{
    return ti_sysbios_family_arm_TaskSupport_start(curTask, enterFxn, exitFxn, eb);
}

/* swap__E */
xdc_Void ti_sysbios_knl_Task_SupportProxy_swap__E( xdc_Ptr *oldtskContext, xdc_Ptr *newtskContext )
{
    ti_sysbios_family_arm_TaskSupport_swap(oldtskContext, newtskContext);
}

/* checkStack__E */
xdc_Bool ti_sysbios_knl_Task_SupportProxy_checkStack__E( xdc_Char *stack, xdc_SizeT size )
{
    return ti_sysbios_family_arm_TaskSupport_checkStack(stack, size);
}

/* stackUsed__E */
xdc_SizeT ti_sysbios_knl_Task_SupportProxy_stackUsed__E( xdc_Char *stack, xdc_SizeT size )
{
    return ti_sysbios_family_arm_TaskSupport_stackUsed(stack, size);
}

/* getStackAlignment__E */
xdc_UInt ti_sysbios_knl_Task_SupportProxy_getStackAlignment__E( void )
{
    return ti_sysbios_family_arm_TaskSupport_getStackAlignment();
}

/* getDefaultStackSize__E */
xdc_SizeT ti_sysbios_knl_Task_SupportProxy_getDefaultStackSize__E( void )
{
    return ti_sysbios_family_arm_TaskSupport_getDefaultStackSize();
}


/*
 * ======== xdc.runtime.Main_Module_GateProxy PROXY BODY ========
 */

/* DELEGATES TO ti.sysbios.gates.GateHwi */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Main_Module_GateProxy_Module__startupDone__S( void ) 
{
    return ti_sysbios_gates_GateHwi_Module__startupDone__S();
}

/* create */
xdc_runtime_Main_Module_GateProxy_Handle xdc_runtime_Main_Module_GateProxy_create( const xdc_runtime_Main_Module_GateProxy_Params *prms, xdc_runtime_Error_Block *eb )
{
    return (xdc_runtime_Main_Module_GateProxy_Handle)ti_sysbios_gates_GateHwi_create((const ti_sysbios_gates_GateHwi_Params *)prms, eb);
}

/* delete */
void xdc_runtime_Main_Module_GateProxy_delete(xdc_runtime_Main_Module_GateProxy_Handle *instp)
{
    ti_sysbios_gates_GateHwi_Object__delete__S(instp);
}

/* Params__init__S */
void xdc_runtime_Main_Module_GateProxy_Params__init__S( xdc_Ptr dst, const void *src, xdc_SizeT psz, xdc_SizeT isz )
{
    ti_sysbios_gates_GateHwi_Params__init__S(dst, src, psz, isz);
}

/* Handle__label__S */
xdc_runtime_Types_Label *xdc_runtime_Main_Module_GateProxy_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label *lab)
{
    return ti_sysbios_gates_GateHwi_Handle__label__S(obj, lab);
}

/* query__E */
xdc_Bool xdc_runtime_Main_Module_GateProxy_query__E( xdc_Int qual )
{
    return ti_sysbios_gates_GateHwi_query(qual);
}

/* enter__E */
xdc_IArg xdc_runtime_Main_Module_GateProxy_enter__E( xdc_runtime_Main_Module_GateProxy_Handle __inst )
{
    return ti_sysbios_gates_GateHwi_enter((ti_sysbios_gates_GateHwi_Handle)__inst);
}

/* leave__E */
xdc_Void xdc_runtime_Main_Module_GateProxy_leave__E( xdc_runtime_Main_Module_GateProxy_Handle __inst, xdc_IArg key )
{
    ti_sysbios_gates_GateHwi_leave((ti_sysbios_gates_GateHwi_Handle)__inst, key);
}


/*
 * ======== xdc.runtime.Memory_HeapProxy PROXY BODY ========
 */

/* DELEGATES TO ti.sysbios.heaps.HeapMem */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Memory_HeapProxy_Module__startupDone__S( void ) 
{
    return ti_sysbios_heaps_HeapMem_Module__startupDone__S();
}

/* create */
xdc_runtime_Memory_HeapProxy_Handle xdc_runtime_Memory_HeapProxy_create( const xdc_runtime_Memory_HeapProxy_Params *prms, xdc_runtime_Error_Block *eb )
{
    return (xdc_runtime_Memory_HeapProxy_Handle)ti_sysbios_heaps_HeapMem_create((const ti_sysbios_heaps_HeapMem_Params *)prms, eb);
}

/* delete */
void xdc_runtime_Memory_HeapProxy_delete(xdc_runtime_Memory_HeapProxy_Handle *instp)
{
    ti_sysbios_heaps_HeapMem_Object__delete__S(instp);
}

/* Params__init__S */
void xdc_runtime_Memory_HeapProxy_Params__init__S( xdc_Ptr dst, const void *src, xdc_SizeT psz, xdc_SizeT isz )
{
    ti_sysbios_heaps_HeapMem_Params__init__S(dst, src, psz, isz);
}

/* Handle__label__S */
xdc_runtime_Types_Label *xdc_runtime_Memory_HeapProxy_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label *lab)
{
    return ti_sysbios_heaps_HeapMem_Handle__label__S(obj, lab);
}

/* alloc__E */
xdc_Ptr xdc_runtime_Memory_HeapProxy_alloc__E( xdc_runtime_Memory_HeapProxy_Handle __inst, xdc_SizeT size, xdc_SizeT align, xdc_runtime_Error_Block *eb )
{
    return xdc_runtime_IHeap_alloc((xdc_runtime_IHeap_Handle)__inst, size, align, eb);
}

/* free__E */
xdc_Void xdc_runtime_Memory_HeapProxy_free__E( xdc_runtime_Memory_HeapProxy_Handle __inst, xdc_Ptr block, xdc_SizeT size )
{
    xdc_runtime_IHeap_free((xdc_runtime_IHeap_Handle)__inst, block, size);
}

/* isBlocking__E */
xdc_Bool xdc_runtime_Memory_HeapProxy_isBlocking__E( xdc_runtime_Memory_HeapProxy_Handle __inst )
{
    return xdc_runtime_IHeap_isBlocking((xdc_runtime_IHeap_Handle)__inst);
}

/* getStats__E */
xdc_Void xdc_runtime_Memory_HeapProxy_getStats__E( xdc_runtime_Memory_HeapProxy_Handle __inst, xdc_runtime_Memory_Stats *stats )
{
    xdc_runtime_IHeap_getStats((xdc_runtime_IHeap_Handle)__inst, stats);
}


/*
 * ======== xdc.runtime.System_Module_GateProxy PROXY BODY ========
 */

/* DELEGATES TO ti.sysbios.gates.GateHwi */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_System_Module_GateProxy_Module__startupDone__S( void ) 
{
    return ti_sysbios_gates_GateHwi_Module__startupDone__S();
}

/* create */
xdc_runtime_System_Module_GateProxy_Handle xdc_runtime_System_Module_GateProxy_create( const xdc_runtime_System_Module_GateProxy_Params *prms, xdc_runtime_Error_Block *eb )
{
    return (xdc_runtime_System_Module_GateProxy_Handle)ti_sysbios_gates_GateHwi_create((const ti_sysbios_gates_GateHwi_Params *)prms, eb);
}

/* delete */
void xdc_runtime_System_Module_GateProxy_delete(xdc_runtime_System_Module_GateProxy_Handle *instp)
{
    ti_sysbios_gates_GateHwi_Object__delete__S(instp);
}

/* Params__init__S */
void xdc_runtime_System_Module_GateProxy_Params__init__S( xdc_Ptr dst, const void *src, xdc_SizeT psz, xdc_SizeT isz )
{
    ti_sysbios_gates_GateHwi_Params__init__S(dst, src, psz, isz);
}

/* Handle__label__S */
xdc_runtime_Types_Label *xdc_runtime_System_Module_GateProxy_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label *lab)
{
    return ti_sysbios_gates_GateHwi_Handle__label__S(obj, lab);
}

/* query__E */
xdc_Bool xdc_runtime_System_Module_GateProxy_query__E( xdc_Int qual )
{
    return ti_sysbios_gates_GateHwi_query(qual);
}

/* enter__E */
xdc_IArg xdc_runtime_System_Module_GateProxy_enter__E( xdc_runtime_System_Module_GateProxy_Handle __inst )
{
    return ti_sysbios_gates_GateHwi_enter((ti_sysbios_gates_GateHwi_Handle)__inst);
}

/* leave__E */
xdc_Void xdc_runtime_System_Module_GateProxy_leave__E( xdc_runtime_System_Module_GateProxy_Handle __inst, xdc_IArg key )
{
    ti_sysbios_gates_GateHwi_leave((ti_sysbios_gates_GateHwi_Handle)__inst, key);
}


/*
 * ======== xdc.runtime.System_SupportProxy PROXY BODY ========
 */

/* DELEGATES TO xdc.runtime.SysStd */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_System_SupportProxy_Module__startupDone__S( void ) 
{
    return xdc_runtime_SysStd_Module__startupDone__S();
}

/* abort__E */
xdc_Void xdc_runtime_System_SupportProxy_abort__E( xdc_CString str )
{
    xdc_runtime_SysStd_abort(str);
}

/* exit__E */
xdc_Void xdc_runtime_System_SupportProxy_exit__E( xdc_Int stat )
{
    xdc_runtime_SysStd_exit(stat);
}

/* flush__E */
xdc_Void xdc_runtime_System_SupportProxy_flush__E( void )
{
    xdc_runtime_SysStd_flush();
}

/* putch__E */
xdc_Void xdc_runtime_System_SupportProxy_putch__E( xdc_Char ch )
{
    xdc_runtime_SysStd_putch(ch);
}

/* ready__E */
xdc_Bool xdc_runtime_System_SupportProxy_ready__E( void )
{
    return xdc_runtime_SysStd_ready();
}


/*
 * ======== ti.sysbios.family.arm.v7r.vim.Hwi OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { ti_sysbios_family_arm_v7r_vim_Hwi_Object2__ s0; char c; } ti_sysbios_family_arm_v7r_vim_Hwi___S1;
#pragma DATA_SECTION(ti_sysbios_family_arm_v7r_vim_Hwi_Object__DESC__C, ".const:ti_sysbios_family_arm_v7r_vim_Hwi_Object__DESC__C");
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_family_arm_v7r_vim_Hwi_Object__DESC__C = {
    (xdc_CPtr)0, /* fxnTab */
    &ti_sysbios_family_arm_v7r_vim_Hwi_Module__root__V.link, /* modLink */
    sizeof(ti_sysbios_family_arm_v7r_vim_Hwi___S1) - sizeof(ti_sysbios_family_arm_v7r_vim_Hwi_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(ti_sysbios_family_arm_v7r_vim_Hwi_Object2__), /* objSize */
    (xdc_CPtr)&ti_sysbios_family_arm_v7r_vim_Hwi_Object__PARAMS__C, /* prmsInit */
    sizeof(ti_sysbios_family_arm_v7r_vim_Hwi_Params), /* prmsSize */
};


/*
 * ======== ti.sysbios.gates.GateHwi OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { ti_sysbios_gates_GateHwi_Object2__ s0; char c; } ti_sysbios_gates_GateHwi___S1;
#pragma DATA_SECTION(ti_sysbios_gates_GateHwi_Object__DESC__C, ".const:ti_sysbios_gates_GateHwi_Object__DESC__C");
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_gates_GateHwi_Object__DESC__C = {
    (xdc_CPtr)&ti_sysbios_gates_GateHwi_Module__FXNS__C, /* fxnTab */
    &ti_sysbios_gates_GateHwi_Module__root__V.link, /* modLink */
    sizeof(ti_sysbios_gates_GateHwi___S1) - sizeof(ti_sysbios_gates_GateHwi_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(ti_sysbios_gates_GateHwi_Object2__), /* objSize */
    (xdc_CPtr)&ti_sysbios_gates_GateHwi_Object__PARAMS__C, /* prmsInit */
    sizeof(ti_sysbios_gates_GateHwi_Params), /* prmsSize */
};


/*
 * ======== ti.sysbios.gates.GateMutex OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { ti_sysbios_gates_GateMutex_Object2__ s0; char c; } ti_sysbios_gates_GateMutex___S1;
#pragma DATA_SECTION(ti_sysbios_gates_GateMutex_Object__DESC__C, ".const:ti_sysbios_gates_GateMutex_Object__DESC__C");
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_gates_GateMutex_Object__DESC__C = {
    (xdc_CPtr)&ti_sysbios_gates_GateMutex_Module__FXNS__C, /* fxnTab */
    &ti_sysbios_gates_GateMutex_Module__root__V.link, /* modLink */
    sizeof(ti_sysbios_gates_GateMutex___S1) - sizeof(ti_sysbios_gates_GateMutex_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(ti_sysbios_gates_GateMutex_Object2__), /* objSize */
    (xdc_CPtr)&ti_sysbios_gates_GateMutex_Object__PARAMS__C, /* prmsInit */
    sizeof(ti_sysbios_gates_GateMutex_Params), /* prmsSize */
};


/*
 * ======== ti.sysbios.hal.Hwi OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { ti_sysbios_hal_Hwi_Object2__ s0; char c; } ti_sysbios_hal_Hwi___S1;
#pragma DATA_SECTION(ti_sysbios_hal_Hwi_Object__DESC__C, ".const:ti_sysbios_hal_Hwi_Object__DESC__C");
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_hal_Hwi_Object__DESC__C = {
    (xdc_CPtr)0, /* fxnTab */
    &ti_sysbios_hal_Hwi_Module__root__V.link, /* modLink */
    sizeof(ti_sysbios_hal_Hwi___S1) - sizeof(ti_sysbios_hal_Hwi_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(ti_sysbios_hal_Hwi_Object2__), /* objSize */
    (xdc_CPtr)&ti_sysbios_hal_Hwi_Object__PARAMS__C, /* prmsInit */
    sizeof(ti_sysbios_hal_Hwi_Params), /* prmsSize */
};


/*
 * ======== ti.sysbios.heaps.HeapBuf OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { ti_sysbios_heaps_HeapBuf_Object2__ s0; char c; } ti_sysbios_heaps_HeapBuf___S1;
#pragma DATA_SECTION(ti_sysbios_heaps_HeapBuf_Object__DESC__C, ".const:ti_sysbios_heaps_HeapBuf_Object__DESC__C");
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_heaps_HeapBuf_Object__DESC__C = {
    (xdc_CPtr)&ti_sysbios_heaps_HeapBuf_Module__FXNS__C, /* fxnTab */
    &ti_sysbios_heaps_HeapBuf_Module__root__V.link, /* modLink */
    sizeof(ti_sysbios_heaps_HeapBuf___S1) - sizeof(ti_sysbios_heaps_HeapBuf_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(ti_sysbios_heaps_HeapBuf_Object2__), /* objSize */
    (xdc_CPtr)&ti_sysbios_heaps_HeapBuf_Object__PARAMS__C, /* prmsInit */
    sizeof(ti_sysbios_heaps_HeapBuf_Params), /* prmsSize */
};


/*
 * ======== ti.sysbios.heaps.HeapMem OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { ti_sysbios_heaps_HeapMem_Object2__ s0; char c; } ti_sysbios_heaps_HeapMem___S1;
#pragma DATA_SECTION(ti_sysbios_heaps_HeapMem_Object__DESC__C, ".const:ti_sysbios_heaps_HeapMem_Object__DESC__C");
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_heaps_HeapMem_Object__DESC__C = {
    (xdc_CPtr)&ti_sysbios_heaps_HeapMem_Module__FXNS__C, /* fxnTab */
    &ti_sysbios_heaps_HeapMem_Module__root__V.link, /* modLink */
    sizeof(ti_sysbios_heaps_HeapMem___S1) - sizeof(ti_sysbios_heaps_HeapMem_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(ti_sysbios_heaps_HeapMem_Object2__), /* objSize */
    (xdc_CPtr)&ti_sysbios_heaps_HeapMem_Object__PARAMS__C, /* prmsInit */
    sizeof(ti_sysbios_heaps_HeapMem_Params), /* prmsSize */
};


/*
 * ======== ti.sysbios.knl.Clock OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { ti_sysbios_knl_Clock_Object2__ s0; char c; } ti_sysbios_knl_Clock___S1;
#pragma DATA_SECTION(ti_sysbios_knl_Clock_Object__DESC__C, ".const:ti_sysbios_knl_Clock_Object__DESC__C");
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_knl_Clock_Object__DESC__C = {
    (xdc_CPtr)-1, /* fxnTab */
    &ti_sysbios_knl_Clock_Module__root__V.link, /* modLink */
    sizeof(ti_sysbios_knl_Clock___S1) - sizeof(ti_sysbios_knl_Clock_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(ti_sysbios_knl_Clock_Object2__), /* objSize */
    (xdc_CPtr)&ti_sysbios_knl_Clock_Object__PARAMS__C, /* prmsInit */
    sizeof(ti_sysbios_knl_Clock_Params), /* prmsSize */
};


/*
 * ======== ti.sysbios.knl.Event OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { ti_sysbios_knl_Event_Object2__ s0; char c; } ti_sysbios_knl_Event___S1;
#pragma DATA_SECTION(ti_sysbios_knl_Event_Object__DESC__C, ".const:ti_sysbios_knl_Event_Object__DESC__C");
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_knl_Event_Object__DESC__C = {
    (xdc_CPtr)-1, /* fxnTab */
    &ti_sysbios_knl_Event_Module__root__V.link, /* modLink */
    sizeof(ti_sysbios_knl_Event___S1) - sizeof(ti_sysbios_knl_Event_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(ti_sysbios_knl_Event_Object2__), /* objSize */
    (xdc_CPtr)&ti_sysbios_knl_Event_Object__PARAMS__C, /* prmsInit */
    sizeof(ti_sysbios_knl_Event_Params), /* prmsSize */
};


/*
 * ======== ti.sysbios.knl.Queue OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { ti_sysbios_knl_Queue_Object2__ s0; char c; } ti_sysbios_knl_Queue___S1;
#pragma DATA_SECTION(ti_sysbios_knl_Queue_Object__DESC__C, ".const:ti_sysbios_knl_Queue_Object__DESC__C");
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_knl_Queue_Object__DESC__C = {
    (xdc_CPtr)-1, /* fxnTab */
    &ti_sysbios_knl_Queue_Module__root__V.link, /* modLink */
    sizeof(ti_sysbios_knl_Queue___S1) - sizeof(ti_sysbios_knl_Queue_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(ti_sysbios_knl_Queue_Object2__), /* objSize */
    (xdc_CPtr)&ti_sysbios_knl_Queue_Object__PARAMS__C, /* prmsInit */
    sizeof(ti_sysbios_knl_Queue_Params), /* prmsSize */
};


/*
 * ======== ti.sysbios.knl.Semaphore OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { ti_sysbios_knl_Semaphore_Object2__ s0; char c; } ti_sysbios_knl_Semaphore___S1;
#pragma DATA_SECTION(ti_sysbios_knl_Semaphore_Object__DESC__C, ".const:ti_sysbios_knl_Semaphore_Object__DESC__C");
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_knl_Semaphore_Object__DESC__C = {
    (xdc_CPtr)-1, /* fxnTab */
    &ti_sysbios_knl_Semaphore_Module__root__V.link, /* modLink */
    sizeof(ti_sysbios_knl_Semaphore___S1) - sizeof(ti_sysbios_knl_Semaphore_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(ti_sysbios_knl_Semaphore_Object2__), /* objSize */
    (xdc_CPtr)&ti_sysbios_knl_Semaphore_Object__PARAMS__C, /* prmsInit */
    sizeof(ti_sysbios_knl_Semaphore_Params), /* prmsSize */
};


/*
 * ======== ti.sysbios.knl.Swi OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { ti_sysbios_knl_Swi_Object2__ s0; char c; } ti_sysbios_knl_Swi___S1;
#pragma DATA_SECTION(ti_sysbios_knl_Swi_Object__DESC__C, ".const:ti_sysbios_knl_Swi_Object__DESC__C");
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_knl_Swi_Object__DESC__C = {
    (xdc_CPtr)-1, /* fxnTab */
    &ti_sysbios_knl_Swi_Module__root__V.link, /* modLink */
    sizeof(ti_sysbios_knl_Swi___S1) - sizeof(ti_sysbios_knl_Swi_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(ti_sysbios_knl_Swi_Object2__), /* objSize */
    (xdc_CPtr)&ti_sysbios_knl_Swi_Object__PARAMS__C, /* prmsInit */
    sizeof(ti_sysbios_knl_Swi_Params), /* prmsSize */
};


/*
 * ======== ti.sysbios.knl.Task OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { ti_sysbios_knl_Task_Object2__ s0; char c; } ti_sysbios_knl_Task___S1;
#pragma DATA_SECTION(ti_sysbios_knl_Task_Object__DESC__C, ".const:ti_sysbios_knl_Task_Object__DESC__C");
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_knl_Task_Object__DESC__C = {
    (xdc_CPtr)-1, /* fxnTab */
    &ti_sysbios_knl_Task_Module__root__V.link, /* modLink */
    sizeof(ti_sysbios_knl_Task___S1) - sizeof(ti_sysbios_knl_Task_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(ti_sysbios_knl_Task_Object2__), /* objSize */
    (xdc_CPtr)&ti_sysbios_knl_Task_Object__PARAMS__C, /* prmsInit */
    sizeof(ti_sysbios_knl_Task_Params), /* prmsSize */
};


/*
 * ======== ti.sysbios.timers.rti.Timer OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { ti_sysbios_timers_rti_Timer_Object2__ s0; char c; } ti_sysbios_timers_rti_Timer___S1;
#pragma DATA_SECTION(ti_sysbios_timers_rti_Timer_Object__DESC__C, ".const:ti_sysbios_timers_rti_Timer_Object__DESC__C");
__FAR__ const xdc_runtime_Core_ObjDesc ti_sysbios_timers_rti_Timer_Object__DESC__C = {
    (xdc_CPtr)0, /* fxnTab */
    &ti_sysbios_timers_rti_Timer_Module__root__V.link, /* modLink */
    sizeof(ti_sysbios_timers_rti_Timer___S1) - sizeof(ti_sysbios_timers_rti_Timer_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(ti_sysbios_timers_rti_Timer_Object2__), /* objSize */
    (xdc_CPtr)&ti_sysbios_timers_rti_Timer_Object__PARAMS__C, /* prmsInit */
    sizeof(ti_sysbios_timers_rti_Timer_Params), /* prmsSize */
};


/*
 * ======== xdc.runtime.IHeap VIRTUAL FUNCTIONS ========
 */

/* create */
xdc_runtime_IHeap_Handle xdc_runtime_IHeap_create( xdc_runtime_IHeap_Module mod, const xdc_runtime_IHeap_Params *prms, xdc_runtime_Error_Block *eb )
{
    return (xdc_runtime_IHeap_Handle) mod->__sysp->__create(0, (const xdc_UChar*)prms, sizeof (xdc_runtime_IHeap_Params), eb);
}

/* delete */
void xdc_runtime_IHeap_delete( xdc_runtime_IHeap_Handle *instp )
{
    (*instp)->__fxns->__sysp->__delete(instp);
}


/*
 * ======== xdc.runtime.IGateProvider VIRTUAL FUNCTIONS ========
 */

/* create */
xdc_runtime_IGateProvider_Handle xdc_runtime_IGateProvider_create( xdc_runtime_IGateProvider_Module mod, const xdc_runtime_IGateProvider_Params *prms, xdc_runtime_Error_Block *eb )
{
    return (xdc_runtime_IGateProvider_Handle) mod->__sysp->__create(0, (const xdc_UChar*)prms, sizeof (xdc_runtime_IGateProvider_Params), eb);
}

/* delete */
void xdc_runtime_IGateProvider_delete( xdc_runtime_IGateProvider_Handle *instp )
{
    (*instp)->__fxns->__sysp->__delete(instp);
}


/*
 * ======== ti.sysbios.BIOS SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool ti_sysbios_BIOS_Module__startupDone__S( void )
{
    return 1;
}



/*
 * ======== ti.sysbios.BIOS_RtsGateProxy SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#ifdef ti_sysbios_BIOS_RtsGateProxy_Module__id__CR
#define Module__MID (*((CT__ti_sysbios_BIOS_RtsGateProxy_Module__id *)(xdcRomConstPtr + ti_sysbios_BIOS_RtsGateProxy_Module__id__C_offset)))
#else
#define Module__MID ti_sysbios_BIOS_RtsGateProxy_Module__id__C
#endif

#undef Module__DGSINCL
#ifdef ti_sysbios_BIOS_RtsGateProxy_Module__diagsIncluded__CR
#define Module__DGSINCL (*((CT__ti_sysbios_BIOS_RtsGateProxy_Module__diagsIncluded *)(xdcRomConstPtr + ti_sysbios_BIOS_RtsGateProxy_Module__diagsIncluded__C_offset)))
#else
#define Module__DGSINCL ti_sysbios_BIOS_RtsGateProxy_Module__diagsIncluded__C
#endif

#undef Module__DGSENAB
#ifdef ti_sysbios_BIOS_RtsGateProxy_Module__diagsEnabled__CR
#define Module__DGSENAB (*((CT__ti_sysbios_BIOS_RtsGateProxy_Module__diagsEnabled *)(xdcRomConstPtr + ti_sysbios_BIOS_RtsGateProxy_Module__diagsEnabled__C_offset)))
#else
#define Module__DGSENAB ti_sysbios_BIOS_RtsGateProxy_Module__diagsEnabled__C
#endif

#undef Module__DGSMASK
#ifdef ti_sysbios_BIOS_RtsGateProxy_Module__diagsMask__CR
#define Module__DGSMASK (*((CT__ti_sysbios_BIOS_RtsGateProxy_Module__diagsMask *)(xdcRomConstPtr + ti_sysbios_BIOS_RtsGateProxy_Module__diagsMask__C_offset)))
#else
#define Module__DGSMASK ti_sysbios_BIOS_RtsGateProxy_Module__diagsMask__C
#endif

#undef Module__LOGDEF
#ifdef ti_sysbios_BIOS_RtsGateProxy_Module__loggerDefined__CR
#define Module__LOGDEF (*((CT__ti_sysbios_BIOS_RtsGateProxy_Module__loggerDefined *)(xdcRomConstPtr + ti_sysbios_BIOS_RtsGateProxy_Module__loggerDefined__C_offset)))
#else
#define Module__LOGDEF ti_sysbios_BIOS_RtsGateProxy_Module__loggerDefined__C
#endif

#undef Module__LOGOBJ
#ifdef ti_sysbios_BIOS_RtsGateProxy_Module__loggerObj__CR
#define Module__LOGOBJ ti_sysbios_BIOS_RtsGateProxy_Module__loggerObj__R
#define Module__LOGOBJ (*((CT__ti_sysbios_BIOS_RtsGateProxy_Module__loggerObj *)(xdcRomConstPtr + ti_sysbios_BIOS_RtsGateProxy_Module__loggerObj__C_offset)))
#else
#define Module__LOGOBJ ti_sysbios_BIOS_RtsGateProxy_Module__loggerObj__C
#endif

#undef Module__LOGFXN0
#ifdef ti_sysbios_BIOS_RtsGateProxy_Module__loggerFxn0__CR
#define Module__LOGFXN0 (*((CT__ti_sysbios_BIOS_RtsGateProxy_Module__loggerFxn0 *)(xdcRomConstPtr + ti_sysbios_BIOS_RtsGateProxy_Module__loggerFxn0__C_offset)))
#else
#define Module__LOGFXN0 ti_sysbios_BIOS_RtsGateProxy_Module__loggerFxn0__C
#endif

#undef Module__LOGFXN1
#ifdef ti_sysbios_BIOS_RtsGateProxy_Module__loggerFxn1__CR
#define Module__LOGFXN1 (*((CT__ti_sysbios_BIOS_RtsGateProxy_Module__loggerFxn1 *)(xdcRomConstPtr + ti_sysbios_BIOS_RtsGateProxy_Module__loggerFxn1__C_offset)))
#else
#define Module__LOGFXN1 ti_sysbios_BIOS_RtsGateProxy_Module__loggerFxn1__C
#endif

#undef Module__LOGFXN2
#ifdef ti_sysbios_BIOS_RtsGateProxy_Module__loggerFxn2__CR
#define Module__LOGFXN2 (*((CT__ti_sysbios_BIOS_RtsGateProxy_Module__loggerFxn2 *)(xdcRomConstPtr + ti_sysbios_BIOS_RtsGateProxy_Module__loggerFxn2__C_offset)))
#else
#define Module__LOGFXN2 ti_sysbios_BIOS_RtsGateProxy_Module__loggerFxn2__C
#endif

#undef Module__LOGFXN4
#ifdef ti_sysbios_BIOS_RtsGateProxy_Module__loggerFxn4__CR
#define Module__LOGFXN4 (*((CT__ti_sysbios_BIOS_RtsGateProxy_Module__loggerFxn4 *)(xdcRomConstPtr + ti_sysbios_BIOS_RtsGateProxy_Module__loggerFxn4__C_offset)))
#else
#define Module__LOGFXN4 ti_sysbios_BIOS_RtsGateProxy_Module__loggerFxn4__C
#endif

#undef Module__LOGFXN8
#ifdef ti_sysbios_BIOS_RtsGateProxy_Module__loggerFxn8__CR
#define Module__LOGFXN8 (*((CT__ti_sysbios_BIOS_RtsGateProxy_Module__loggerFxn8 *)(xdcRomConstPtr + ti_sysbios_BIOS_RtsGateProxy_Module__loggerFxn8__C_offset)))
#else
#define Module__LOGFXN8 ti_sysbios_BIOS_RtsGateProxy_Module__loggerFxn8__C
#endif

#undef Module__G_OBJ
#ifdef ti_sysbios_BIOS_RtsGateProxy_Module__gateObj__CR
#define Module__G_OBJ (*((CT__ti_sysbios_BIOS_RtsGateProxy_Module__gateObj *)(xdcRomConstPtr + ti_sysbios_BIOS_RtsGateProxy_Module__gateObj__C_offset)))
#else
#define Module__G_OBJ ti_sysbios_BIOS_RtsGateProxy_Module__gateObj__C
#endif

#undef Module__G_PRMS
#ifdef ti_sysbios_BIOS_RtsGateProxy_Module__gatePrms__CR
#define Module__G_PRMS (*((CT__ti_sysbios_BIOS_RtsGateProxy_Module__gatePrms *)(xdcRomConstPtr + ti_sysbios_BIOS_RtsGateProxy_Module__gatePrms__C_offset)))
#else
#define Module__G_PRMS ti_sysbios_BIOS_RtsGateProxy_Module__gatePrms__C
#endif

#undef Module__GP_create
#define Module__GP_create ti_sysbios_BIOS_RtsGateProxy_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sysbios_BIOS_RtsGateProxy_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sysbios_BIOS_RtsGateProxy_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sysbios_BIOS_RtsGateProxy_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sysbios_BIOS_RtsGateProxy_Module_GateProxy_query

xdc_Bool ti_sysbios_BIOS_RtsGateProxy_Proxy__abstract__S(void)
{
    return 0;
}
xdc_CPtr ti_sysbios_BIOS_RtsGateProxy_Proxy__delegate__S(void)
{
    return (const void *)&ti_sysbios_gates_GateMutex_Module__FXNS__C;
}



/*
 * ======== ti.sysbios.family.arm.IntrinsicsSupport SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool ti_sysbios_family_arm_IntrinsicsSupport_Module__startupDone__S( void )
{
    return 1;
}



/*
 * ======== ti.sysbios.family.arm.TaskSupport SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool ti_sysbios_family_arm_TaskSupport_Module__startupDone__S( void )
{
    return 1;
}



/*
 * ======== ti.sysbios.family.arm.exc.Exception SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool ti_sysbios_family_arm_exc_Exception_Module__startupDone__S( void )
{
    return ti_sysbios_family_arm_exc_Exception_Module__startupDone__F();
}



/*
 * ======== ti.sysbios.family.arm.v7r.tms570.Core SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool ti_sysbios_family_arm_v7r_tms570_Core_Module__startupDone__S( void )
{
    return ti_sysbios_family_arm_v7r_tms570_Core_Module__startupDone__F();
}



/*
 * ======== ti.sysbios.family.arm.v7r.vim.Hwi SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#ifdef ti_sysbios_family_arm_v7r_vim_Hwi_Module__id__CR
#define Module__MID (*((CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__id *)(xdcRomConstPtr + ti_sysbios_family_arm_v7r_vim_Hwi_Module__id__C_offset)))
#else
#define Module__MID ti_sysbios_family_arm_v7r_vim_Hwi_Module__id__C
#endif

#undef Module__DGSINCL
#ifdef ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsIncluded__CR
#define Module__DGSINCL (*((CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsIncluded *)(xdcRomConstPtr + ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsIncluded__C_offset)))
#else
#define Module__DGSINCL ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsIncluded__C
#endif

#undef Module__DGSENAB
#ifdef ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsEnabled__CR
#define Module__DGSENAB (*((CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsEnabled *)(xdcRomConstPtr + ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsEnabled__C_offset)))
#else
#define Module__DGSENAB ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsEnabled__C
#endif

#undef Module__DGSMASK
#ifdef ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsMask__CR
#define Module__DGSMASK (*((CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsMask *)(xdcRomConstPtr + ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsMask__C_offset)))
#else
#define Module__DGSMASK ti_sysbios_family_arm_v7r_vim_Hwi_Module__diagsMask__C
#endif

#undef Module__LOGDEF
#ifdef ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerDefined__CR
#define Module__LOGDEF (*((CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerDefined *)(xdcRomConstPtr + ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerDefined__C_offset)))
#else
#define Module__LOGDEF ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerDefined__C
#endif

#undef Module__LOGOBJ
#ifdef ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerObj__CR
#define Module__LOGOBJ ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerObj__R
#define Module__LOGOBJ (*((CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerObj *)(xdcRomConstPtr + ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerObj__C_offset)))
#else
#define Module__LOGOBJ ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerObj__C
#endif

#undef Module__LOGFXN0
#ifdef ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn0__CR
#define Module__LOGFXN0 (*((CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn0 *)(xdcRomConstPtr + ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn0__C_offset)))
#else
#define Module__LOGFXN0 ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn0__C
#endif

#undef Module__LOGFXN1
#ifdef ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn1__CR
#define Module__LOGFXN1 (*((CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn1 *)(xdcRomConstPtr + ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn1__C_offset)))
#else
#define Module__LOGFXN1 ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn1__C
#endif

#undef Module__LOGFXN2
#ifdef ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn2__CR
#define Module__LOGFXN2 (*((CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn2 *)(xdcRomConstPtr + ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn2__C_offset)))
#else
#define Module__LOGFXN2 ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn2__C
#endif

#undef Module__LOGFXN4
#ifdef ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn4__CR
#define Module__LOGFXN4 (*((CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn4 *)(xdcRomConstPtr + ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn4__C_offset)))
#else
#define Module__LOGFXN4 ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn4__C
#endif

#undef Module__LOGFXN8
#ifdef ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn8__CR
#define Module__LOGFXN8 (*((CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn8 *)(xdcRomConstPtr + ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn8__C_offset)))
#else
#define Module__LOGFXN8 ti_sysbios_family_arm_v7r_vim_Hwi_Module__loggerFxn8__C
#endif

#undef Module__G_OBJ
#ifdef ti_sysbios_family_arm_v7r_vim_Hwi_Module__gateObj__CR
#define Module__G_OBJ (*((CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__gateObj *)(xdcRomConstPtr + ti_sysbios_family_arm_v7r_vim_Hwi_Module__gateObj__C_offset)))
#else
#define Module__G_OBJ ti_sysbios_family_arm_v7r_vim_Hwi_Module__gateObj__C
#endif

#undef Module__G_PRMS
#ifdef ti_sysbios_family_arm_v7r_vim_Hwi_Module__gatePrms__CR
#define Module__G_PRMS (*((CT__ti_sysbios_family_arm_v7r_vim_Hwi_Module__gatePrms *)(xdcRomConstPtr + ti_sysbios_family_arm_v7r_vim_Hwi_Module__gatePrms__C_offset)))
#else
#define Module__G_PRMS ti_sysbios_family_arm_v7r_vim_Hwi_Module__gatePrms__C
#endif

#undef Module__GP_create
#define Module__GP_create ti_sysbios_family_arm_v7r_vim_Hwi_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sysbios_family_arm_v7r_vim_Hwi_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sysbios_family_arm_v7r_vim_Hwi_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sysbios_family_arm_v7r_vim_Hwi_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sysbios_family_arm_v7r_vim_Hwi_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool ti_sysbios_family_arm_v7r_vim_Hwi_Module__startupDone__S( void )
{
    return ti_sysbios_family_arm_v7r_vim_Hwi_Module__startupDone__F();
}

/* Handle__label__S */
xdc_runtime_Types_Label *ti_sysbios_family_arm_v7r_vim_Hwi_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label *lab) 
{
    lab->handle = obj;
    lab->modId = 32804;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void ti_sysbios_family_arm_v7r_vim_Hwi_Params__init__S( xdc_Ptr prms, const void *src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_CPtr)(src ? src : &ti_sysbios_family_arm_v7r_vim_Hwi_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr ti_sysbios_family_arm_v7r_vim_Hwi_Object__get__S(xdc_Ptr oa, xdc_Int i)
{
    if (oa) {
        return ((ti_sysbios_family_arm_v7r_vim_Hwi_Object__ *)oa) + i;
    }

    /* the bogus 'const' in the cast suppresses Klocwork MISRA complaints */
    return ((ti_sysbios_family_arm_v7r_vim_Hwi_Object__ * const)ti_sysbios_family_arm_v7r_vim_Hwi_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr ti_sysbios_family_arm_v7r_vim_Hwi_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)ti_sysbios_family_arm_v7r_vim_Hwi_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&ti_sysbios_family_arm_v7r_vim_Hwi_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr ti_sysbios_family_arm_v7r_vim_Hwi_Object__next__S( xdc_Ptr obj )
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&ti_sysbios_family_arm_v7r_vim_Hwi_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr ti_sysbios_family_arm_v7r_vim_Hwi_Object__create__S (
    xdc_CPtr __aa,
    const xdc_UChar *__paramsPtr,
    xdc_SizeT __psz,
    xdc_runtime_Error_Block *eb)
{
    const ti_sysbios_family_arm_v7r_vim_Hwi_Args__create *__args = __aa;
    ti_sysbios_family_arm_v7r_vim_Hwi_Params __prms;
    ti_sysbios_family_arm_v7r_vim_Hwi_Object *__obj;
    int iStat;

    /* common instance initialization */
    __obj = xdc_runtime_Core_createObject__I(&ti_sysbios_family_arm_v7r_vim_Hwi_Object__DESC__C, NULL, &__prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (__obj == NULL) {
        return NULL;
    }


    /* module-specific initialization */
    iStat = ti_sysbios_family_arm_v7r_vim_Hwi_Instance_init__E(__obj, __args->intNum, __args->hwiFxn, &__prms, eb);
    if (iStat) {
        xdc_runtime_Core_deleteObject__I(&ti_sysbios_family_arm_v7r_vim_Hwi_Object__DESC__C, __obj, (xdc_Fxn)ti_sysbios_family_arm_v7r_vim_Hwi_Instance_finalize__E, iStat, 0);
        return NULL;
    }

    return __obj;
}

/* create */
ti_sysbios_family_arm_v7r_vim_Hwi_Handle ti_sysbios_family_arm_v7r_vim_Hwi_create( xdc_Int intNum, ti_sysbios_interfaces_IHwi_FuncPtr hwiFxn, const ti_sysbios_family_arm_v7r_vim_Hwi_Params *__paramsPtr, xdc_runtime_Error_Block *eb )
{
    ti_sysbios_family_arm_v7r_vim_Hwi_Params prms;
    ti_sysbios_family_arm_v7r_vim_Hwi_Object *obj;

    int iStat;

    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&ti_sysbios_family_arm_v7r_vim_Hwi_Object__DESC__C, NULL, &prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    iStat = ti_sysbios_family_arm_v7r_vim_Hwi_Instance_init__E(obj, intNum, hwiFxn, &prms, eb);
    if (iStat) {
        xdc_runtime_Core_deleteObject__I(&ti_sysbios_family_arm_v7r_vim_Hwi_Object__DESC__C, obj, (xdc_Fxn)ti_sysbios_family_arm_v7r_vim_Hwi_Instance_finalize__E, iStat, 0);
        return NULL;
    }

    return obj;
}

/* construct */
void ti_sysbios_family_arm_v7r_vim_Hwi_construct(ti_sysbios_family_arm_v7r_vim_Hwi_Struct *__obj, xdc_Int intNum, ti_sysbios_interfaces_IHwi_FuncPtr hwiFxn, const ti_sysbios_family_arm_v7r_vim_Hwi_Params *__paramsPtr, xdc_runtime_Error_Block *eb )
{
    ti_sysbios_family_arm_v7r_vim_Hwi_Params __prms;
    int iStat;

    /* common instance initialization */
    xdc_runtime_Core_constructObject__I(&ti_sysbios_family_arm_v7r_vim_Hwi_Object__DESC__C, __obj, &__prms, (xdc_CPtr)__paramsPtr, 0, eb);

    /* module-specific initialization */
    iStat = ti_sysbios_family_arm_v7r_vim_Hwi_Instance_init__E((xdc_Ptr)__obj, intNum, hwiFxn, &__prms, eb);
    if (iStat) {
        xdc_runtime_Core_deleteObject__I(&ti_sysbios_family_arm_v7r_vim_Hwi_Object__DESC__C, __obj, (xdc_Fxn)ti_sysbios_family_arm_v7r_vim_Hwi_Instance_finalize__E, iStat, 1);
    }

}

/* destruct */
void ti_sysbios_family_arm_v7r_vim_Hwi_destruct(ti_sysbios_family_arm_v7r_vim_Hwi_Struct *obj)
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_family_arm_v7r_vim_Hwi_Object__DESC__C, obj, (xdc_Fxn)ti_sysbios_family_arm_v7r_vim_Hwi_Instance_finalize__E, 0, TRUE);
}

/* Object__delete__S */
xdc_Void ti_sysbios_family_arm_v7r_vim_Hwi_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_family_arm_v7r_vim_Hwi_Object__DESC__C, *((ti_sysbios_family_arm_v7r_vim_Hwi_Object**)instp), (xdc_Fxn)ti_sysbios_family_arm_v7r_vim_Hwi_Instance_finalize__E, 0, FALSE);
    *((ti_sysbios_family_arm_v7r_vim_Hwi_Handle*)instp) = NULL;
}

/* delete */
void ti_sysbios_family_arm_v7r_vim_Hwi_delete(ti_sysbios_family_arm_v7r_vim_Hwi_Handle *instp)
{
    ti_sysbios_family_arm_v7r_vim_Hwi_Object__delete__S(instp);
}


/*
 * ======== ti.sysbios.gates.GateHwi SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#ifdef ti_sysbios_gates_GateHwi_Module__id__CR
#define Module__MID (*((CT__ti_sysbios_gates_GateHwi_Module__id *)(xdcRomConstPtr + ti_sysbios_gates_GateHwi_Module__id__C_offset)))
#else
#define Module__MID ti_sysbios_gates_GateHwi_Module__id__C
#endif

#undef Module__DGSINCL
#ifdef ti_sysbios_gates_GateHwi_Module__diagsIncluded__CR
#define Module__DGSINCL (*((CT__ti_sysbios_gates_GateHwi_Module__diagsIncluded *)(xdcRomConstPtr + ti_sysbios_gates_GateHwi_Module__diagsIncluded__C_offset)))
#else
#define Module__DGSINCL ti_sysbios_gates_GateHwi_Module__diagsIncluded__C
#endif

#undef Module__DGSENAB
#ifdef ti_sysbios_gates_GateHwi_Module__diagsEnabled__CR
#define Module__DGSENAB (*((CT__ti_sysbios_gates_GateHwi_Module__diagsEnabled *)(xdcRomConstPtr + ti_sysbios_gates_GateHwi_Module__diagsEnabled__C_offset)))
#else
#define Module__DGSENAB ti_sysbios_gates_GateHwi_Module__diagsEnabled__C
#endif

#undef Module__DGSMASK
#ifdef ti_sysbios_gates_GateHwi_Module__diagsMask__CR
#define Module__DGSMASK (*((CT__ti_sysbios_gates_GateHwi_Module__diagsMask *)(xdcRomConstPtr + ti_sysbios_gates_GateHwi_Module__diagsMask__C_offset)))
#else
#define Module__DGSMASK ti_sysbios_gates_GateHwi_Module__diagsMask__C
#endif

#undef Module__LOGDEF
#ifdef ti_sysbios_gates_GateHwi_Module__loggerDefined__CR
#define Module__LOGDEF (*((CT__ti_sysbios_gates_GateHwi_Module__loggerDefined *)(xdcRomConstPtr + ti_sysbios_gates_GateHwi_Module__loggerDefined__C_offset)))
#else
#define Module__LOGDEF ti_sysbios_gates_GateHwi_Module__loggerDefined__C
#endif

#undef Module__LOGOBJ
#ifdef ti_sysbios_gates_GateHwi_Module__loggerObj__CR
#define Module__LOGOBJ ti_sysbios_gates_GateHwi_Module__loggerObj__R
#define Module__LOGOBJ (*((CT__ti_sysbios_gates_GateHwi_Module__loggerObj *)(xdcRomConstPtr + ti_sysbios_gates_GateHwi_Module__loggerObj__C_offset)))
#else
#define Module__LOGOBJ ti_sysbios_gates_GateHwi_Module__loggerObj__C
#endif

#undef Module__LOGFXN0
#ifdef ti_sysbios_gates_GateHwi_Module__loggerFxn0__CR
#define Module__LOGFXN0 (*((CT__ti_sysbios_gates_GateHwi_Module__loggerFxn0 *)(xdcRomConstPtr + ti_sysbios_gates_GateHwi_Module__loggerFxn0__C_offset)))
#else
#define Module__LOGFXN0 ti_sysbios_gates_GateHwi_Module__loggerFxn0__C
#endif

#undef Module__LOGFXN1
#ifdef ti_sysbios_gates_GateHwi_Module__loggerFxn1__CR
#define Module__LOGFXN1 (*((CT__ti_sysbios_gates_GateHwi_Module__loggerFxn1 *)(xdcRomConstPtr + ti_sysbios_gates_GateHwi_Module__loggerFxn1__C_offset)))
#else
#define Module__LOGFXN1 ti_sysbios_gates_GateHwi_Module__loggerFxn1__C
#endif

#undef Module__LOGFXN2
#ifdef ti_sysbios_gates_GateHwi_Module__loggerFxn2__CR
#define Module__LOGFXN2 (*((CT__ti_sysbios_gates_GateHwi_Module__loggerFxn2 *)(xdcRomConstPtr + ti_sysbios_gates_GateHwi_Module__loggerFxn2__C_offset)))
#else
#define Module__LOGFXN2 ti_sysbios_gates_GateHwi_Module__loggerFxn2__C
#endif

#undef Module__LOGFXN4
#ifdef ti_sysbios_gates_GateHwi_Module__loggerFxn4__CR
#define Module__LOGFXN4 (*((CT__ti_sysbios_gates_GateHwi_Module__loggerFxn4 *)(xdcRomConstPtr + ti_sysbios_gates_GateHwi_Module__loggerFxn4__C_offset)))
#else
#define Module__LOGFXN4 ti_sysbios_gates_GateHwi_Module__loggerFxn4__C
#endif

#undef Module__LOGFXN8
#ifdef ti_sysbios_gates_GateHwi_Module__loggerFxn8__CR
#define Module__LOGFXN8 (*((CT__ti_sysbios_gates_GateHwi_Module__loggerFxn8 *)(xdcRomConstPtr + ti_sysbios_gates_GateHwi_Module__loggerFxn8__C_offset)))
#else
#define Module__LOGFXN8 ti_sysbios_gates_GateHwi_Module__loggerFxn8__C
#endif

#undef Module__G_OBJ
#ifdef ti_sysbios_gates_GateHwi_Module__gateObj__CR
#define Module__G_OBJ (*((CT__ti_sysbios_gates_GateHwi_Module__gateObj *)(xdcRomConstPtr + ti_sysbios_gates_GateHwi_Module__gateObj__C_offset)))
#else
#define Module__G_OBJ ti_sysbios_gates_GateHwi_Module__gateObj__C
#endif

#undef Module__G_PRMS
#ifdef ti_sysbios_gates_GateHwi_Module__gatePrms__CR
#define Module__G_PRMS (*((CT__ti_sysbios_gates_GateHwi_Module__gatePrms *)(xdcRomConstPtr + ti_sysbios_gates_GateHwi_Module__gatePrms__C_offset)))
#else
#define Module__G_PRMS ti_sysbios_gates_GateHwi_Module__gatePrms__C
#endif

#undef Module__GP_create
#define Module__GP_create ti_sysbios_gates_GateHwi_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sysbios_gates_GateHwi_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sysbios_gates_GateHwi_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sysbios_gates_GateHwi_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sysbios_gates_GateHwi_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool ti_sysbios_gates_GateHwi_Module__startupDone__S( void )
{
    return 1;
}

/* Handle__label__S */
xdc_runtime_Types_Label *ti_sysbios_gates_GateHwi_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label *lab) 
{
    lab->handle = obj;
    lab->modId = 32808;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void ti_sysbios_gates_GateHwi_Params__init__S( xdc_Ptr prms, const void *src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_CPtr)(src ? src : &ti_sysbios_gates_GateHwi_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr ti_sysbios_gates_GateHwi_Object__get__S(xdc_Ptr oa, xdc_Int i)
{
    if (oa) {
        return ((ti_sysbios_gates_GateHwi_Object__ *)oa) + i;
    }

    /* the bogus 'const' in the cast suppresses Klocwork MISRA complaints */
    return ((ti_sysbios_gates_GateHwi_Object__ * const)ti_sysbios_gates_GateHwi_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr ti_sysbios_gates_GateHwi_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)ti_sysbios_gates_GateHwi_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&ti_sysbios_gates_GateHwi_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr ti_sysbios_gates_GateHwi_Object__next__S( xdc_Ptr obj )
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&ti_sysbios_gates_GateHwi_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr ti_sysbios_gates_GateHwi_Object__create__S (
    xdc_CPtr __aa,
    const xdc_UChar *__paramsPtr,
    xdc_SizeT __psz,
    xdc_runtime_Error_Block *eb)
{
    ti_sysbios_gates_GateHwi_Params __prms;
    ti_sysbios_gates_GateHwi_Object *__obj;

    /* common instance initialization */
    __obj = xdc_runtime_Core_createObject__I(&ti_sysbios_gates_GateHwi_Object__DESC__C, NULL, &__prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (__obj == NULL) {
        return NULL;
    }


    /* module-specific initialization */
    ti_sysbios_gates_GateHwi_Instance_init__E(__obj, &__prms);
    return __obj;
}

/* create */
ti_sysbios_gates_GateHwi_Handle ti_sysbios_gates_GateHwi_create( const ti_sysbios_gates_GateHwi_Params *__paramsPtr, xdc_runtime_Error_Block *eb )
{
    ti_sysbios_gates_GateHwi_Params prms;
    ti_sysbios_gates_GateHwi_Object *obj;


    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&ti_sysbios_gates_GateHwi_Object__DESC__C, NULL, &prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    ti_sysbios_gates_GateHwi_Instance_init__E(obj, &prms);
    return obj;
}

/* construct */
void ti_sysbios_gates_GateHwi_construct(ti_sysbios_gates_GateHwi_Struct *__obj, const ti_sysbios_gates_GateHwi_Params *__paramsPtr )
{
    ti_sysbios_gates_GateHwi_Params __prms;

    /* common instance initialization */
    xdc_runtime_Core_constructObject__I(&ti_sysbios_gates_GateHwi_Object__DESC__C, __obj, &__prms, (xdc_CPtr)__paramsPtr, 0, NULL);

    /* module-specific initialization */
    ti_sysbios_gates_GateHwi_Instance_init__E((xdc_Ptr)__obj, &__prms);
}

/* destruct */
void ti_sysbios_gates_GateHwi_destruct(ti_sysbios_gates_GateHwi_Struct *obj)
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_gates_GateHwi_Object__DESC__C, obj, NULL, -1, TRUE);
}

/* Object__delete__S */
xdc_Void ti_sysbios_gates_GateHwi_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_gates_GateHwi_Object__DESC__C, *((ti_sysbios_gates_GateHwi_Object**)instp), NULL, -1, FALSE);
    *((ti_sysbios_gates_GateHwi_Handle*)instp) = NULL;
}

/* delete */
void ti_sysbios_gates_GateHwi_delete(ti_sysbios_gates_GateHwi_Handle *instp)
{
    ti_sysbios_gates_GateHwi_Object__delete__S(instp);
}


/*
 * ======== ti.sysbios.gates.GateMutex SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#ifdef ti_sysbios_gates_GateMutex_Module__id__CR
#define Module__MID (*((CT__ti_sysbios_gates_GateMutex_Module__id *)(xdcRomConstPtr + ti_sysbios_gates_GateMutex_Module__id__C_offset)))
#else
#define Module__MID ti_sysbios_gates_GateMutex_Module__id__C
#endif

#undef Module__DGSINCL
#ifdef ti_sysbios_gates_GateMutex_Module__diagsIncluded__CR
#define Module__DGSINCL (*((CT__ti_sysbios_gates_GateMutex_Module__diagsIncluded *)(xdcRomConstPtr + ti_sysbios_gates_GateMutex_Module__diagsIncluded__C_offset)))
#else
#define Module__DGSINCL ti_sysbios_gates_GateMutex_Module__diagsIncluded__C
#endif

#undef Module__DGSENAB
#ifdef ti_sysbios_gates_GateMutex_Module__diagsEnabled__CR
#define Module__DGSENAB (*((CT__ti_sysbios_gates_GateMutex_Module__diagsEnabled *)(xdcRomConstPtr + ti_sysbios_gates_GateMutex_Module__diagsEnabled__C_offset)))
#else
#define Module__DGSENAB ti_sysbios_gates_GateMutex_Module__diagsEnabled__C
#endif

#undef Module__DGSMASK
#ifdef ti_sysbios_gates_GateMutex_Module__diagsMask__CR
#define Module__DGSMASK (*((CT__ti_sysbios_gates_GateMutex_Module__diagsMask *)(xdcRomConstPtr + ti_sysbios_gates_GateMutex_Module__diagsMask__C_offset)))
#else
#define Module__DGSMASK ti_sysbios_gates_GateMutex_Module__diagsMask__C
#endif

#undef Module__LOGDEF
#ifdef ti_sysbios_gates_GateMutex_Module__loggerDefined__CR
#define Module__LOGDEF (*((CT__ti_sysbios_gates_GateMutex_Module__loggerDefined *)(xdcRomConstPtr + ti_sysbios_gates_GateMutex_Module__loggerDefined__C_offset)))
#else
#define Module__LOGDEF ti_sysbios_gates_GateMutex_Module__loggerDefined__C
#endif

#undef Module__LOGOBJ
#ifdef ti_sysbios_gates_GateMutex_Module__loggerObj__CR
#define Module__LOGOBJ ti_sysbios_gates_GateMutex_Module__loggerObj__R
#define Module__LOGOBJ (*((CT__ti_sysbios_gates_GateMutex_Module__loggerObj *)(xdcRomConstPtr + ti_sysbios_gates_GateMutex_Module__loggerObj__C_offset)))
#else
#define Module__LOGOBJ ti_sysbios_gates_GateMutex_Module__loggerObj__C
#endif

#undef Module__LOGFXN0
#ifdef ti_sysbios_gates_GateMutex_Module__loggerFxn0__CR
#define Module__LOGFXN0 (*((CT__ti_sysbios_gates_GateMutex_Module__loggerFxn0 *)(xdcRomConstPtr + ti_sysbios_gates_GateMutex_Module__loggerFxn0__C_offset)))
#else
#define Module__LOGFXN0 ti_sysbios_gates_GateMutex_Module__loggerFxn0__C
#endif

#undef Module__LOGFXN1
#ifdef ti_sysbios_gates_GateMutex_Module__loggerFxn1__CR
#define Module__LOGFXN1 (*((CT__ti_sysbios_gates_GateMutex_Module__loggerFxn1 *)(xdcRomConstPtr + ti_sysbios_gates_GateMutex_Module__loggerFxn1__C_offset)))
#else
#define Module__LOGFXN1 ti_sysbios_gates_GateMutex_Module__loggerFxn1__C
#endif

#undef Module__LOGFXN2
#ifdef ti_sysbios_gates_GateMutex_Module__loggerFxn2__CR
#define Module__LOGFXN2 (*((CT__ti_sysbios_gates_GateMutex_Module__loggerFxn2 *)(xdcRomConstPtr + ti_sysbios_gates_GateMutex_Module__loggerFxn2__C_offset)))
#else
#define Module__LOGFXN2 ti_sysbios_gates_GateMutex_Module__loggerFxn2__C
#endif

#undef Module__LOGFXN4
#ifdef ti_sysbios_gates_GateMutex_Module__loggerFxn4__CR
#define Module__LOGFXN4 (*((CT__ti_sysbios_gates_GateMutex_Module__loggerFxn4 *)(xdcRomConstPtr + ti_sysbios_gates_GateMutex_Module__loggerFxn4__C_offset)))
#else
#define Module__LOGFXN4 ti_sysbios_gates_GateMutex_Module__loggerFxn4__C
#endif

#undef Module__LOGFXN8
#ifdef ti_sysbios_gates_GateMutex_Module__loggerFxn8__CR
#define Module__LOGFXN8 (*((CT__ti_sysbios_gates_GateMutex_Module__loggerFxn8 *)(xdcRomConstPtr + ti_sysbios_gates_GateMutex_Module__loggerFxn8__C_offset)))
#else
#define Module__LOGFXN8 ti_sysbios_gates_GateMutex_Module__loggerFxn8__C
#endif

#undef Module__G_OBJ
#ifdef ti_sysbios_gates_GateMutex_Module__gateObj__CR
#define Module__G_OBJ (*((CT__ti_sysbios_gates_GateMutex_Module__gateObj *)(xdcRomConstPtr + ti_sysbios_gates_GateMutex_Module__gateObj__C_offset)))
#else
#define Module__G_OBJ ti_sysbios_gates_GateMutex_Module__gateObj__C
#endif

#undef Module__G_PRMS
#ifdef ti_sysbios_gates_GateMutex_Module__gatePrms__CR
#define Module__G_PRMS (*((CT__ti_sysbios_gates_GateMutex_Module__gatePrms *)(xdcRomConstPtr + ti_sysbios_gates_GateMutex_Module__gatePrms__C_offset)))
#else
#define Module__G_PRMS ti_sysbios_gates_GateMutex_Module__gatePrms__C
#endif

#undef Module__GP_create
#define Module__GP_create ti_sysbios_gates_GateMutex_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sysbios_gates_GateMutex_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sysbios_gates_GateMutex_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sysbios_gates_GateMutex_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sysbios_gates_GateMutex_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool ti_sysbios_gates_GateMutex_Module__startupDone__S( void )
{
    return 1;
}

/* Handle__label__S */
xdc_runtime_Types_Label *ti_sysbios_gates_GateMutex_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label *lab) 
{
    lab->handle = obj;
    lab->modId = 32809;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void ti_sysbios_gates_GateMutex_Params__init__S( xdc_Ptr prms, const void *src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_CPtr)(src ? src : &ti_sysbios_gates_GateMutex_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr ti_sysbios_gates_GateMutex_Object__get__S(xdc_Ptr oa, xdc_Int i)
{
    if (oa) {
        return ((ti_sysbios_gates_GateMutex_Object__ *)oa) + i;
    }

    /* the bogus 'const' in the cast suppresses Klocwork MISRA complaints */
    return ((ti_sysbios_gates_GateMutex_Object__ * const)ti_sysbios_gates_GateMutex_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr ti_sysbios_gates_GateMutex_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)ti_sysbios_gates_GateMutex_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&ti_sysbios_gates_GateMutex_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr ti_sysbios_gates_GateMutex_Object__next__S( xdc_Ptr obj )
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&ti_sysbios_gates_GateMutex_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr ti_sysbios_gates_GateMutex_Object__create__S (
    xdc_CPtr __aa,
    const xdc_UChar *__paramsPtr,
    xdc_SizeT __psz,
    xdc_runtime_Error_Block *eb)
{
    ti_sysbios_gates_GateMutex_Params __prms;
    ti_sysbios_gates_GateMutex_Object *__obj;

    /* common instance initialization */
    __obj = xdc_runtime_Core_createObject__I(&ti_sysbios_gates_GateMutex_Object__DESC__C, NULL, &__prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (__obj == NULL) {
        return NULL;
    }


    /* module-specific initialization */
    ti_sysbios_gates_GateMutex_Instance_init__E(__obj, &__prms);
    return __obj;
}

/* create */
ti_sysbios_gates_GateMutex_Handle ti_sysbios_gates_GateMutex_create( const ti_sysbios_gates_GateMutex_Params *__paramsPtr, xdc_runtime_Error_Block *eb )
{
    ti_sysbios_gates_GateMutex_Params prms;
    ti_sysbios_gates_GateMutex_Object *obj;


    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&ti_sysbios_gates_GateMutex_Object__DESC__C, NULL, &prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    ti_sysbios_gates_GateMutex_Instance_init__E(obj, &prms);
    return obj;
}

/* construct */
void ti_sysbios_gates_GateMutex_construct(ti_sysbios_gates_GateMutex_Struct *__obj, const ti_sysbios_gates_GateMutex_Params *__paramsPtr )
{
    ti_sysbios_gates_GateMutex_Params __prms;

    /* common instance initialization */
    xdc_runtime_Core_constructObject__I(&ti_sysbios_gates_GateMutex_Object__DESC__C, __obj, &__prms, (xdc_CPtr)__paramsPtr, 0, NULL);

    /* module-specific initialization */
    ti_sysbios_gates_GateMutex_Instance_init__E((xdc_Ptr)__obj, &__prms);
}

/* destruct */
void ti_sysbios_gates_GateMutex_destruct(ti_sysbios_gates_GateMutex_Struct *obj)
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_gates_GateMutex_Object__DESC__C, obj, (xdc_Fxn)ti_sysbios_gates_GateMutex_Instance_finalize__E, -1, TRUE);
}

/* Object__delete__S */
xdc_Void ti_sysbios_gates_GateMutex_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_gates_GateMutex_Object__DESC__C, *((ti_sysbios_gates_GateMutex_Object**)instp), (xdc_Fxn)ti_sysbios_gates_GateMutex_Instance_finalize__E, -1, FALSE);
    *((ti_sysbios_gates_GateMutex_Handle*)instp) = NULL;
}

/* delete */
void ti_sysbios_gates_GateMutex_delete(ti_sysbios_gates_GateMutex_Handle *instp)
{
    ti_sysbios_gates_GateMutex_Object__delete__S(instp);
}


/*
 * ======== ti.sysbios.hal.Cache SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool ti_sysbios_hal_Cache_Module__startupDone__S( void )
{
    return 1;
}



/*
 * ======== ti.sysbios.hal.CacheNull SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool ti_sysbios_hal_CacheNull_Module__startupDone__S( void )
{
    return 1;
}



/*
 * ======== ti.sysbios.hal.Cache_CacheProxy SYSTEM FUNCTIONS ========
 */


xdc_Bool ti_sysbios_hal_Cache_CacheProxy_Proxy__abstract__S(void)
{
    return 0;
}
xdc_CPtr ti_sysbios_hal_Cache_CacheProxy_Proxy__delegate__S(void)
{
    return (const void *)&ti_sysbios_hal_CacheNull_Module__FXNS__C;
}


/*
 * ======== ti.sysbios.hal.Core SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool ti_sysbios_hal_Core_Module__startupDone__S( void )
{
    return 1;
}



/*
 * ======== ti.sysbios.hal.Core_CoreProxy SYSTEM FUNCTIONS ========
 */


xdc_Bool ti_sysbios_hal_Core_CoreProxy_Proxy__abstract__S(void)
{
    return 0;
}
xdc_CPtr ti_sysbios_hal_Core_CoreProxy_Proxy__delegate__S(void)
{
    return 0;
}


/*
 * ======== ti.sysbios.hal.Hwi SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#ifdef ti_sysbios_hal_Hwi_Module__id__CR
#define Module__MID (*((CT__ti_sysbios_hal_Hwi_Module__id *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_Module__id__C_offset)))
#else
#define Module__MID ti_sysbios_hal_Hwi_Module__id__C
#endif

#undef Module__DGSINCL
#ifdef ti_sysbios_hal_Hwi_Module__diagsIncluded__CR
#define Module__DGSINCL (*((CT__ti_sysbios_hal_Hwi_Module__diagsIncluded *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_Module__diagsIncluded__C_offset)))
#else
#define Module__DGSINCL ti_sysbios_hal_Hwi_Module__diagsIncluded__C
#endif

#undef Module__DGSENAB
#ifdef ti_sysbios_hal_Hwi_Module__diagsEnabled__CR
#define Module__DGSENAB (*((CT__ti_sysbios_hal_Hwi_Module__diagsEnabled *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_Module__diagsEnabled__C_offset)))
#else
#define Module__DGSENAB ti_sysbios_hal_Hwi_Module__diagsEnabled__C
#endif

#undef Module__DGSMASK
#ifdef ti_sysbios_hal_Hwi_Module__diagsMask__CR
#define Module__DGSMASK (*((CT__ti_sysbios_hal_Hwi_Module__diagsMask *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_Module__diagsMask__C_offset)))
#else
#define Module__DGSMASK ti_sysbios_hal_Hwi_Module__diagsMask__C
#endif

#undef Module__LOGDEF
#ifdef ti_sysbios_hal_Hwi_Module__loggerDefined__CR
#define Module__LOGDEF (*((CT__ti_sysbios_hal_Hwi_Module__loggerDefined *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_Module__loggerDefined__C_offset)))
#else
#define Module__LOGDEF ti_sysbios_hal_Hwi_Module__loggerDefined__C
#endif

#undef Module__LOGOBJ
#ifdef ti_sysbios_hal_Hwi_Module__loggerObj__CR
#define Module__LOGOBJ ti_sysbios_hal_Hwi_Module__loggerObj__R
#define Module__LOGOBJ (*((CT__ti_sysbios_hal_Hwi_Module__loggerObj *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_Module__loggerObj__C_offset)))
#else
#define Module__LOGOBJ ti_sysbios_hal_Hwi_Module__loggerObj__C
#endif

#undef Module__LOGFXN0
#ifdef ti_sysbios_hal_Hwi_Module__loggerFxn0__CR
#define Module__LOGFXN0 (*((CT__ti_sysbios_hal_Hwi_Module__loggerFxn0 *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_Module__loggerFxn0__C_offset)))
#else
#define Module__LOGFXN0 ti_sysbios_hal_Hwi_Module__loggerFxn0__C
#endif

#undef Module__LOGFXN1
#ifdef ti_sysbios_hal_Hwi_Module__loggerFxn1__CR
#define Module__LOGFXN1 (*((CT__ti_sysbios_hal_Hwi_Module__loggerFxn1 *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_Module__loggerFxn1__C_offset)))
#else
#define Module__LOGFXN1 ti_sysbios_hal_Hwi_Module__loggerFxn1__C
#endif

#undef Module__LOGFXN2
#ifdef ti_sysbios_hal_Hwi_Module__loggerFxn2__CR
#define Module__LOGFXN2 (*((CT__ti_sysbios_hal_Hwi_Module__loggerFxn2 *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_Module__loggerFxn2__C_offset)))
#else
#define Module__LOGFXN2 ti_sysbios_hal_Hwi_Module__loggerFxn2__C
#endif

#undef Module__LOGFXN4
#ifdef ti_sysbios_hal_Hwi_Module__loggerFxn4__CR
#define Module__LOGFXN4 (*((CT__ti_sysbios_hal_Hwi_Module__loggerFxn4 *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_Module__loggerFxn4__C_offset)))
#else
#define Module__LOGFXN4 ti_sysbios_hal_Hwi_Module__loggerFxn4__C
#endif

#undef Module__LOGFXN8
#ifdef ti_sysbios_hal_Hwi_Module__loggerFxn8__CR
#define Module__LOGFXN8 (*((CT__ti_sysbios_hal_Hwi_Module__loggerFxn8 *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_Module__loggerFxn8__C_offset)))
#else
#define Module__LOGFXN8 ti_sysbios_hal_Hwi_Module__loggerFxn8__C
#endif

#undef Module__G_OBJ
#ifdef ti_sysbios_hal_Hwi_Module__gateObj__CR
#define Module__G_OBJ (*((CT__ti_sysbios_hal_Hwi_Module__gateObj *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_Module__gateObj__C_offset)))
#else
#define Module__G_OBJ ti_sysbios_hal_Hwi_Module__gateObj__C
#endif

#undef Module__G_PRMS
#ifdef ti_sysbios_hal_Hwi_Module__gatePrms__CR
#define Module__G_PRMS (*((CT__ti_sysbios_hal_Hwi_Module__gatePrms *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_Module__gatePrms__C_offset)))
#else
#define Module__G_PRMS ti_sysbios_hal_Hwi_Module__gatePrms__C
#endif

#undef Module__GP_create
#define Module__GP_create ti_sysbios_hal_Hwi_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sysbios_hal_Hwi_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sysbios_hal_Hwi_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sysbios_hal_Hwi_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sysbios_hal_Hwi_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool ti_sysbios_hal_Hwi_Module__startupDone__S( void )
{
    return ti_sysbios_hal_Hwi_Module__startupDone__F();
}

/* Handle__label__S */
xdc_runtime_Types_Label *ti_sysbios_hal_Hwi_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label *lab) 
{
    lab->handle = obj;
    lab->modId = 32814;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void ti_sysbios_hal_Hwi_Params__init__S( xdc_Ptr prms, const void *src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_CPtr)(src ? src : &ti_sysbios_hal_Hwi_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr ti_sysbios_hal_Hwi_Object__get__S(xdc_Ptr oa, xdc_Int i)
{
    if (oa) {
        return ((ti_sysbios_hal_Hwi_Object__ *)oa) + i;
    }

    /* the bogus 'const' in the cast suppresses Klocwork MISRA complaints */
    return ((ti_sysbios_hal_Hwi_Object__ * const)ti_sysbios_hal_Hwi_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr ti_sysbios_hal_Hwi_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)ti_sysbios_hal_Hwi_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&ti_sysbios_hal_Hwi_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr ti_sysbios_hal_Hwi_Object__next__S( xdc_Ptr obj )
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&ti_sysbios_hal_Hwi_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr ti_sysbios_hal_Hwi_Object__create__S (
    xdc_CPtr __aa,
    const xdc_UChar *__paramsPtr,
    xdc_SizeT __psz,
    xdc_runtime_Error_Block *eb)
{
    const ti_sysbios_hal_Hwi_Args__create *__args = __aa;
    ti_sysbios_hal_Hwi_Params __prms;
    ti_sysbios_hal_Hwi_Object *__obj;
    int iStat;

    /* common instance initialization */
    __obj = xdc_runtime_Core_createObject__I(&ti_sysbios_hal_Hwi_Object__DESC__C, NULL, &__prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (__obj == NULL) {
        return NULL;
    }


    /* module-specific initialization */
    iStat = ti_sysbios_hal_Hwi_Instance_init__E(__obj, __args->intNum, __args->hwiFxn, &__prms, eb);
    if (iStat) {
        xdc_runtime_Core_deleteObject__I(&ti_sysbios_hal_Hwi_Object__DESC__C, __obj, (xdc_Fxn)ti_sysbios_hal_Hwi_Instance_finalize__E, iStat, 0);
        return NULL;
    }

    return __obj;
}

/* create */
ti_sysbios_hal_Hwi_Handle ti_sysbios_hal_Hwi_create( xdc_Int intNum, ti_sysbios_hal_Hwi_FuncPtr hwiFxn, const ti_sysbios_hal_Hwi_Params *__paramsPtr, xdc_runtime_Error_Block *eb )
{
    ti_sysbios_hal_Hwi_Params prms;
    ti_sysbios_hal_Hwi_Object *obj;

    int iStat;

    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&ti_sysbios_hal_Hwi_Object__DESC__C, NULL, &prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    iStat = ti_sysbios_hal_Hwi_Instance_init__E(obj, intNum, hwiFxn, &prms, eb);
    if (iStat) {
        xdc_runtime_Core_deleteObject__I(&ti_sysbios_hal_Hwi_Object__DESC__C, obj, (xdc_Fxn)ti_sysbios_hal_Hwi_Instance_finalize__E, iStat, 0);
        return NULL;
    }

    return obj;
}

/* construct */
void ti_sysbios_hal_Hwi_construct(ti_sysbios_hal_Hwi_Struct *__obj, xdc_Int intNum, ti_sysbios_hal_Hwi_FuncPtr hwiFxn, const ti_sysbios_hal_Hwi_Params *__paramsPtr, xdc_runtime_Error_Block *eb )
{
    ti_sysbios_hal_Hwi_Params __prms;
    int iStat;

    /* common instance initialization */
    xdc_runtime_Core_constructObject__I(&ti_sysbios_hal_Hwi_Object__DESC__C, __obj, &__prms, (xdc_CPtr)__paramsPtr, 0, eb);

    /* module-specific initialization */
    iStat = ti_sysbios_hal_Hwi_Instance_init__E((xdc_Ptr)__obj, intNum, hwiFxn, &__prms, eb);
    if (iStat) {
        xdc_runtime_Core_deleteObject__I(&ti_sysbios_hal_Hwi_Object__DESC__C, __obj, (xdc_Fxn)ti_sysbios_hal_Hwi_Instance_finalize__E, iStat, 1);
    }

}

/* destruct */
void ti_sysbios_hal_Hwi_destruct(ti_sysbios_hal_Hwi_Struct *obj)
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_hal_Hwi_Object__DESC__C, obj, (xdc_Fxn)ti_sysbios_hal_Hwi_Instance_finalize__E, 0, TRUE);
}

/* Object__delete__S */
xdc_Void ti_sysbios_hal_Hwi_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_hal_Hwi_Object__DESC__C, *((ti_sysbios_hal_Hwi_Object**)instp), (xdc_Fxn)ti_sysbios_hal_Hwi_Instance_finalize__E, 0, FALSE);
    *((ti_sysbios_hal_Hwi_Handle*)instp) = NULL;
}

/* delete */
void ti_sysbios_hal_Hwi_delete(ti_sysbios_hal_Hwi_Handle *instp)
{
    ti_sysbios_hal_Hwi_Object__delete__S(instp);
}


/*
 * ======== ti.sysbios.hal.Hwi_HwiProxy SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#ifdef ti_sysbios_hal_Hwi_HwiProxy_Module__id__CR
#define Module__MID (*((CT__ti_sysbios_hal_Hwi_HwiProxy_Module__id *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_HwiProxy_Module__id__C_offset)))
#else
#define Module__MID ti_sysbios_hal_Hwi_HwiProxy_Module__id__C
#endif

#undef Module__DGSINCL
#ifdef ti_sysbios_hal_Hwi_HwiProxy_Module__diagsIncluded__CR
#define Module__DGSINCL (*((CT__ti_sysbios_hal_Hwi_HwiProxy_Module__diagsIncluded *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_HwiProxy_Module__diagsIncluded__C_offset)))
#else
#define Module__DGSINCL ti_sysbios_hal_Hwi_HwiProxy_Module__diagsIncluded__C
#endif

#undef Module__DGSENAB
#ifdef ti_sysbios_hal_Hwi_HwiProxy_Module__diagsEnabled__CR
#define Module__DGSENAB (*((CT__ti_sysbios_hal_Hwi_HwiProxy_Module__diagsEnabled *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_HwiProxy_Module__diagsEnabled__C_offset)))
#else
#define Module__DGSENAB ti_sysbios_hal_Hwi_HwiProxy_Module__diagsEnabled__C
#endif

#undef Module__DGSMASK
#ifdef ti_sysbios_hal_Hwi_HwiProxy_Module__diagsMask__CR
#define Module__DGSMASK (*((CT__ti_sysbios_hal_Hwi_HwiProxy_Module__diagsMask *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_HwiProxy_Module__diagsMask__C_offset)))
#else
#define Module__DGSMASK ti_sysbios_hal_Hwi_HwiProxy_Module__diagsMask__C
#endif

#undef Module__LOGDEF
#ifdef ti_sysbios_hal_Hwi_HwiProxy_Module__loggerDefined__CR
#define Module__LOGDEF (*((CT__ti_sysbios_hal_Hwi_HwiProxy_Module__loggerDefined *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_HwiProxy_Module__loggerDefined__C_offset)))
#else
#define Module__LOGDEF ti_sysbios_hal_Hwi_HwiProxy_Module__loggerDefined__C
#endif

#undef Module__LOGOBJ
#ifdef ti_sysbios_hal_Hwi_HwiProxy_Module__loggerObj__CR
#define Module__LOGOBJ ti_sysbios_hal_Hwi_HwiProxy_Module__loggerObj__R
#define Module__LOGOBJ (*((CT__ti_sysbios_hal_Hwi_HwiProxy_Module__loggerObj *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_HwiProxy_Module__loggerObj__C_offset)))
#else
#define Module__LOGOBJ ti_sysbios_hal_Hwi_HwiProxy_Module__loggerObj__C
#endif

#undef Module__LOGFXN0
#ifdef ti_sysbios_hal_Hwi_HwiProxy_Module__loggerFxn0__CR
#define Module__LOGFXN0 (*((CT__ti_sysbios_hal_Hwi_HwiProxy_Module__loggerFxn0 *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_HwiProxy_Module__loggerFxn0__C_offset)))
#else
#define Module__LOGFXN0 ti_sysbios_hal_Hwi_HwiProxy_Module__loggerFxn0__C
#endif

#undef Module__LOGFXN1
#ifdef ti_sysbios_hal_Hwi_HwiProxy_Module__loggerFxn1__CR
#define Module__LOGFXN1 (*((CT__ti_sysbios_hal_Hwi_HwiProxy_Module__loggerFxn1 *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_HwiProxy_Module__loggerFxn1__C_offset)))
#else
#define Module__LOGFXN1 ti_sysbios_hal_Hwi_HwiProxy_Module__loggerFxn1__C
#endif

#undef Module__LOGFXN2
#ifdef ti_sysbios_hal_Hwi_HwiProxy_Module__loggerFxn2__CR
#define Module__LOGFXN2 (*((CT__ti_sysbios_hal_Hwi_HwiProxy_Module__loggerFxn2 *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_HwiProxy_Module__loggerFxn2__C_offset)))
#else
#define Module__LOGFXN2 ti_sysbios_hal_Hwi_HwiProxy_Module__loggerFxn2__C
#endif

#undef Module__LOGFXN4
#ifdef ti_sysbios_hal_Hwi_HwiProxy_Module__loggerFxn4__CR
#define Module__LOGFXN4 (*((CT__ti_sysbios_hal_Hwi_HwiProxy_Module__loggerFxn4 *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_HwiProxy_Module__loggerFxn4__C_offset)))
#else
#define Module__LOGFXN4 ti_sysbios_hal_Hwi_HwiProxy_Module__loggerFxn4__C
#endif

#undef Module__LOGFXN8
#ifdef ti_sysbios_hal_Hwi_HwiProxy_Module__loggerFxn8__CR
#define Module__LOGFXN8 (*((CT__ti_sysbios_hal_Hwi_HwiProxy_Module__loggerFxn8 *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_HwiProxy_Module__loggerFxn8__C_offset)))
#else
#define Module__LOGFXN8 ti_sysbios_hal_Hwi_HwiProxy_Module__loggerFxn8__C
#endif

#undef Module__G_OBJ
#ifdef ti_sysbios_hal_Hwi_HwiProxy_Module__gateObj__CR
#define Module__G_OBJ (*((CT__ti_sysbios_hal_Hwi_HwiProxy_Module__gateObj *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_HwiProxy_Module__gateObj__C_offset)))
#else
#define Module__G_OBJ ti_sysbios_hal_Hwi_HwiProxy_Module__gateObj__C
#endif

#undef Module__G_PRMS
#ifdef ti_sysbios_hal_Hwi_HwiProxy_Module__gatePrms__CR
#define Module__G_PRMS (*((CT__ti_sysbios_hal_Hwi_HwiProxy_Module__gatePrms *)(xdcRomConstPtr + ti_sysbios_hal_Hwi_HwiProxy_Module__gatePrms__C_offset)))
#else
#define Module__G_PRMS ti_sysbios_hal_Hwi_HwiProxy_Module__gatePrms__C
#endif

#undef Module__GP_create
#define Module__GP_create ti_sysbios_hal_Hwi_HwiProxy_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sysbios_hal_Hwi_HwiProxy_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sysbios_hal_Hwi_HwiProxy_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sysbios_hal_Hwi_HwiProxy_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sysbios_hal_Hwi_HwiProxy_Module_GateProxy_query

xdc_Bool ti_sysbios_hal_Hwi_HwiProxy_Proxy__abstract__S(void)
{
    return 0;
}
xdc_CPtr ti_sysbios_hal_Hwi_HwiProxy_Proxy__delegate__S(void)
{
    return 0;
}



/*
 * ======== ti.sysbios.heaps.HeapBuf SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#ifdef ti_sysbios_heaps_HeapBuf_Module__id__CR
#define Module__MID (*((CT__ti_sysbios_heaps_HeapBuf_Module__id *)(xdcRomConstPtr + ti_sysbios_heaps_HeapBuf_Module__id__C_offset)))
#else
#define Module__MID ti_sysbios_heaps_HeapBuf_Module__id__C
#endif

#undef Module__DGSINCL
#ifdef ti_sysbios_heaps_HeapBuf_Module__diagsIncluded__CR
#define Module__DGSINCL (*((CT__ti_sysbios_heaps_HeapBuf_Module__diagsIncluded *)(xdcRomConstPtr + ti_sysbios_heaps_HeapBuf_Module__diagsIncluded__C_offset)))
#else
#define Module__DGSINCL ti_sysbios_heaps_HeapBuf_Module__diagsIncluded__C
#endif

#undef Module__DGSENAB
#ifdef ti_sysbios_heaps_HeapBuf_Module__diagsEnabled__CR
#define Module__DGSENAB (*((CT__ti_sysbios_heaps_HeapBuf_Module__diagsEnabled *)(xdcRomConstPtr + ti_sysbios_heaps_HeapBuf_Module__diagsEnabled__C_offset)))
#else
#define Module__DGSENAB ti_sysbios_heaps_HeapBuf_Module__diagsEnabled__C
#endif

#undef Module__DGSMASK
#ifdef ti_sysbios_heaps_HeapBuf_Module__diagsMask__CR
#define Module__DGSMASK (*((CT__ti_sysbios_heaps_HeapBuf_Module__diagsMask *)(xdcRomConstPtr + ti_sysbios_heaps_HeapBuf_Module__diagsMask__C_offset)))
#else
#define Module__DGSMASK ti_sysbios_heaps_HeapBuf_Module__diagsMask__C
#endif

#undef Module__LOGDEF
#ifdef ti_sysbios_heaps_HeapBuf_Module__loggerDefined__CR
#define Module__LOGDEF (*((CT__ti_sysbios_heaps_HeapBuf_Module__loggerDefined *)(xdcRomConstPtr + ti_sysbios_heaps_HeapBuf_Module__loggerDefined__C_offset)))
#else
#define Module__LOGDEF ti_sysbios_heaps_HeapBuf_Module__loggerDefined__C
#endif

#undef Module__LOGOBJ
#ifdef ti_sysbios_heaps_HeapBuf_Module__loggerObj__CR
#define Module__LOGOBJ ti_sysbios_heaps_HeapBuf_Module__loggerObj__R
#define Module__LOGOBJ (*((CT__ti_sysbios_heaps_HeapBuf_Module__loggerObj *)(xdcRomConstPtr + ti_sysbios_heaps_HeapBuf_Module__loggerObj__C_offset)))
#else
#define Module__LOGOBJ ti_sysbios_heaps_HeapBuf_Module__loggerObj__C
#endif

#undef Module__LOGFXN0
#ifdef ti_sysbios_heaps_HeapBuf_Module__loggerFxn0__CR
#define Module__LOGFXN0 (*((CT__ti_sysbios_heaps_HeapBuf_Module__loggerFxn0 *)(xdcRomConstPtr + ti_sysbios_heaps_HeapBuf_Module__loggerFxn0__C_offset)))
#else
#define Module__LOGFXN0 ti_sysbios_heaps_HeapBuf_Module__loggerFxn0__C
#endif

#undef Module__LOGFXN1
#ifdef ti_sysbios_heaps_HeapBuf_Module__loggerFxn1__CR
#define Module__LOGFXN1 (*((CT__ti_sysbios_heaps_HeapBuf_Module__loggerFxn1 *)(xdcRomConstPtr + ti_sysbios_heaps_HeapBuf_Module__loggerFxn1__C_offset)))
#else
#define Module__LOGFXN1 ti_sysbios_heaps_HeapBuf_Module__loggerFxn1__C
#endif

#undef Module__LOGFXN2
#ifdef ti_sysbios_heaps_HeapBuf_Module__loggerFxn2__CR
#define Module__LOGFXN2 (*((CT__ti_sysbios_heaps_HeapBuf_Module__loggerFxn2 *)(xdcRomConstPtr + ti_sysbios_heaps_HeapBuf_Module__loggerFxn2__C_offset)))
#else
#define Module__LOGFXN2 ti_sysbios_heaps_HeapBuf_Module__loggerFxn2__C
#endif

#undef Module__LOGFXN4
#ifdef ti_sysbios_heaps_HeapBuf_Module__loggerFxn4__CR
#define Module__LOGFXN4 (*((CT__ti_sysbios_heaps_HeapBuf_Module__loggerFxn4 *)(xdcRomConstPtr + ti_sysbios_heaps_HeapBuf_Module__loggerFxn4__C_offset)))
#else
#define Module__LOGFXN4 ti_sysbios_heaps_HeapBuf_Module__loggerFxn4__C
#endif

#undef Module__LOGFXN8
#ifdef ti_sysbios_heaps_HeapBuf_Module__loggerFxn8__CR
#define Module__LOGFXN8 (*((CT__ti_sysbios_heaps_HeapBuf_Module__loggerFxn8 *)(xdcRomConstPtr + ti_sysbios_heaps_HeapBuf_Module__loggerFxn8__C_offset)))
#else
#define Module__LOGFXN8 ti_sysbios_heaps_HeapBuf_Module__loggerFxn8__C
#endif

#undef Module__G_OBJ
#ifdef ti_sysbios_heaps_HeapBuf_Module__gateObj__CR
#define Module__G_OBJ (*((CT__ti_sysbios_heaps_HeapBuf_Module__gateObj *)(xdcRomConstPtr + ti_sysbios_heaps_HeapBuf_Module__gateObj__C_offset)))
#else
#define Module__G_OBJ ti_sysbios_heaps_HeapBuf_Module__gateObj__C
#endif

#undef Module__G_PRMS
#ifdef ti_sysbios_heaps_HeapBuf_Module__gatePrms__CR
#define Module__G_PRMS (*((CT__ti_sysbios_heaps_HeapBuf_Module__gatePrms *)(xdcRomConstPtr + ti_sysbios_heaps_HeapBuf_Module__gatePrms__C_offset)))
#else
#define Module__G_PRMS ti_sysbios_heaps_HeapBuf_Module__gatePrms__C
#endif

#undef Module__GP_create
#define Module__GP_create ti_sysbios_heaps_HeapBuf_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sysbios_heaps_HeapBuf_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sysbios_heaps_HeapBuf_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sysbios_heaps_HeapBuf_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sysbios_heaps_HeapBuf_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool ti_sysbios_heaps_HeapBuf_Module__startupDone__S( void )
{
    return ti_sysbios_heaps_HeapBuf_Module__startupDone__F();
}

/* Handle__label__S */
xdc_runtime_Types_Label *ti_sysbios_heaps_HeapBuf_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label *lab) 
{
    lab->handle = obj;
    lab->modId = 32800;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void ti_sysbios_heaps_HeapBuf_Params__init__S( xdc_Ptr prms, const void *src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_CPtr)(src ? src : &ti_sysbios_heaps_HeapBuf_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr ti_sysbios_heaps_HeapBuf_Object__get__S(xdc_Ptr oa, xdc_Int i)
{
    if (oa) {
        return ((ti_sysbios_heaps_HeapBuf_Object__ *)oa) + i;
    }

    return NULL;
}


/* Object__first__S */
xdc_Ptr ti_sysbios_heaps_HeapBuf_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)ti_sysbios_heaps_HeapBuf_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&ti_sysbios_heaps_HeapBuf_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr ti_sysbios_heaps_HeapBuf_Object__next__S( xdc_Ptr obj )
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&ti_sysbios_heaps_HeapBuf_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr ti_sysbios_heaps_HeapBuf_Object__create__S (
    xdc_CPtr __aa,
    const xdc_UChar *__paramsPtr,
    xdc_SizeT __psz,
    xdc_runtime_Error_Block *eb)
{
    ti_sysbios_heaps_HeapBuf_Params __prms;
    ti_sysbios_heaps_HeapBuf_Object *__obj;
    int iStat;

    /* common instance initialization */
    __obj = xdc_runtime_Core_createObject__I(&ti_sysbios_heaps_HeapBuf_Object__DESC__C, NULL, &__prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (__obj == NULL) {
        return NULL;
    }


    /* module-specific initialization */
    iStat = ti_sysbios_heaps_HeapBuf_Instance_init__E(__obj, &__prms, eb);
    if (iStat) {
        xdc_runtime_Core_deleteObject__I(&ti_sysbios_heaps_HeapBuf_Object__DESC__C, __obj, (xdc_Fxn)ti_sysbios_heaps_HeapBuf_Instance_finalize__E, iStat, 0);
        return NULL;
    }

    return __obj;
}

/* create */
ti_sysbios_heaps_HeapBuf_Handle ti_sysbios_heaps_HeapBuf_create( const ti_sysbios_heaps_HeapBuf_Params *__paramsPtr, xdc_runtime_Error_Block *eb )
{
    ti_sysbios_heaps_HeapBuf_Params prms;
    ti_sysbios_heaps_HeapBuf_Object *obj;

    int iStat;

    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&ti_sysbios_heaps_HeapBuf_Object__DESC__C, NULL, &prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    iStat = ti_sysbios_heaps_HeapBuf_Instance_init__E(obj, &prms, eb);
    if (iStat) {
        xdc_runtime_Core_deleteObject__I(&ti_sysbios_heaps_HeapBuf_Object__DESC__C, obj, (xdc_Fxn)ti_sysbios_heaps_HeapBuf_Instance_finalize__E, iStat, 0);
        return NULL;
    }

    return obj;
}

/* construct */
void ti_sysbios_heaps_HeapBuf_construct(ti_sysbios_heaps_HeapBuf_Struct *__obj, const ti_sysbios_heaps_HeapBuf_Params *__paramsPtr, xdc_runtime_Error_Block *eb )
{
    ti_sysbios_heaps_HeapBuf_Params __prms;
    int iStat;

    /* common instance initialization */
    xdc_runtime_Core_constructObject__I(&ti_sysbios_heaps_HeapBuf_Object__DESC__C, __obj, &__prms, (xdc_CPtr)__paramsPtr, 0, eb);

    /* module-specific initialization */
    iStat = ti_sysbios_heaps_HeapBuf_Instance_init__E((xdc_Ptr)__obj, &__prms, eb);
    if (iStat) {
        xdc_runtime_Core_deleteObject__I(&ti_sysbios_heaps_HeapBuf_Object__DESC__C, __obj, (xdc_Fxn)ti_sysbios_heaps_HeapBuf_Instance_finalize__E, iStat, 1);
    }

}

/* destruct */
void ti_sysbios_heaps_HeapBuf_destruct(ti_sysbios_heaps_HeapBuf_Struct *obj)
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_heaps_HeapBuf_Object__DESC__C, obj, (xdc_Fxn)ti_sysbios_heaps_HeapBuf_Instance_finalize__E, 0, TRUE);
}

/* Object__delete__S */
xdc_Void ti_sysbios_heaps_HeapBuf_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_heaps_HeapBuf_Object__DESC__C, *((ti_sysbios_heaps_HeapBuf_Object**)instp), (xdc_Fxn)ti_sysbios_heaps_HeapBuf_Instance_finalize__E, 0, FALSE);
    *((ti_sysbios_heaps_HeapBuf_Handle*)instp) = NULL;
}

/* delete */
void ti_sysbios_heaps_HeapBuf_delete(ti_sysbios_heaps_HeapBuf_Handle *instp)
{
    ti_sysbios_heaps_HeapBuf_Object__delete__S(instp);
}


/*
 * ======== ti.sysbios.heaps.HeapMem SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#ifdef ti_sysbios_heaps_HeapMem_Module__id__CR
#define Module__MID (*((CT__ti_sysbios_heaps_HeapMem_Module__id *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module__id__C_offset)))
#else
#define Module__MID ti_sysbios_heaps_HeapMem_Module__id__C
#endif

#undef Module__DGSINCL
#ifdef ti_sysbios_heaps_HeapMem_Module__diagsIncluded__CR
#define Module__DGSINCL (*((CT__ti_sysbios_heaps_HeapMem_Module__diagsIncluded *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module__diagsIncluded__C_offset)))
#else
#define Module__DGSINCL ti_sysbios_heaps_HeapMem_Module__diagsIncluded__C
#endif

#undef Module__DGSENAB
#ifdef ti_sysbios_heaps_HeapMem_Module__diagsEnabled__CR
#define Module__DGSENAB (*((CT__ti_sysbios_heaps_HeapMem_Module__diagsEnabled *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module__diagsEnabled__C_offset)))
#else
#define Module__DGSENAB ti_sysbios_heaps_HeapMem_Module__diagsEnabled__C
#endif

#undef Module__DGSMASK
#ifdef ti_sysbios_heaps_HeapMem_Module__diagsMask__CR
#define Module__DGSMASK (*((CT__ti_sysbios_heaps_HeapMem_Module__diagsMask *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module__diagsMask__C_offset)))
#else
#define Module__DGSMASK ti_sysbios_heaps_HeapMem_Module__diagsMask__C
#endif

#undef Module__LOGDEF
#ifdef ti_sysbios_heaps_HeapMem_Module__loggerDefined__CR
#define Module__LOGDEF (*((CT__ti_sysbios_heaps_HeapMem_Module__loggerDefined *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module__loggerDefined__C_offset)))
#else
#define Module__LOGDEF ti_sysbios_heaps_HeapMem_Module__loggerDefined__C
#endif

#undef Module__LOGOBJ
#ifdef ti_sysbios_heaps_HeapMem_Module__loggerObj__CR
#define Module__LOGOBJ ti_sysbios_heaps_HeapMem_Module__loggerObj__R
#define Module__LOGOBJ (*((CT__ti_sysbios_heaps_HeapMem_Module__loggerObj *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module__loggerObj__C_offset)))
#else
#define Module__LOGOBJ ti_sysbios_heaps_HeapMem_Module__loggerObj__C
#endif

#undef Module__LOGFXN0
#ifdef ti_sysbios_heaps_HeapMem_Module__loggerFxn0__CR
#define Module__LOGFXN0 (*((CT__ti_sysbios_heaps_HeapMem_Module__loggerFxn0 *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module__loggerFxn0__C_offset)))
#else
#define Module__LOGFXN0 ti_sysbios_heaps_HeapMem_Module__loggerFxn0__C
#endif

#undef Module__LOGFXN1
#ifdef ti_sysbios_heaps_HeapMem_Module__loggerFxn1__CR
#define Module__LOGFXN1 (*((CT__ti_sysbios_heaps_HeapMem_Module__loggerFxn1 *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module__loggerFxn1__C_offset)))
#else
#define Module__LOGFXN1 ti_sysbios_heaps_HeapMem_Module__loggerFxn1__C
#endif

#undef Module__LOGFXN2
#ifdef ti_sysbios_heaps_HeapMem_Module__loggerFxn2__CR
#define Module__LOGFXN2 (*((CT__ti_sysbios_heaps_HeapMem_Module__loggerFxn2 *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module__loggerFxn2__C_offset)))
#else
#define Module__LOGFXN2 ti_sysbios_heaps_HeapMem_Module__loggerFxn2__C
#endif

#undef Module__LOGFXN4
#ifdef ti_sysbios_heaps_HeapMem_Module__loggerFxn4__CR
#define Module__LOGFXN4 (*((CT__ti_sysbios_heaps_HeapMem_Module__loggerFxn4 *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module__loggerFxn4__C_offset)))
#else
#define Module__LOGFXN4 ti_sysbios_heaps_HeapMem_Module__loggerFxn4__C
#endif

#undef Module__LOGFXN8
#ifdef ti_sysbios_heaps_HeapMem_Module__loggerFxn8__CR
#define Module__LOGFXN8 (*((CT__ti_sysbios_heaps_HeapMem_Module__loggerFxn8 *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module__loggerFxn8__C_offset)))
#else
#define Module__LOGFXN8 ti_sysbios_heaps_HeapMem_Module__loggerFxn8__C
#endif

#undef Module__G_OBJ
#ifdef ti_sysbios_heaps_HeapMem_Module__gateObj__CR
#define Module__G_OBJ (*((CT__ti_sysbios_heaps_HeapMem_Module__gateObj *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module__gateObj__C_offset)))
#else
#define Module__G_OBJ ti_sysbios_heaps_HeapMem_Module__gateObj__C
#endif

#undef Module__G_PRMS
#ifdef ti_sysbios_heaps_HeapMem_Module__gatePrms__CR
#define Module__G_PRMS (*((CT__ti_sysbios_heaps_HeapMem_Module__gatePrms *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module__gatePrms__C_offset)))
#else
#define Module__G_PRMS ti_sysbios_heaps_HeapMem_Module__gatePrms__C
#endif

#undef Module__GP_create
#define Module__GP_create ti_sysbios_heaps_HeapMem_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sysbios_heaps_HeapMem_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sysbios_heaps_HeapMem_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sysbios_heaps_HeapMem_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sysbios_heaps_HeapMem_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool ti_sysbios_heaps_HeapMem_Module__startupDone__S( void )
{
    return 1;
}

/* Handle__label__S */
xdc_runtime_Types_Label *ti_sysbios_heaps_HeapMem_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label *lab) 
{
    lab->handle = obj;
    lab->modId = 32801;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void ti_sysbios_heaps_HeapMem_Params__init__S( xdc_Ptr prms, const void *src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_CPtr)(src ? src : &ti_sysbios_heaps_HeapMem_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr ti_sysbios_heaps_HeapMem_Object__get__S(xdc_Ptr oa, xdc_Int i)
{
    if (oa) {
        return ((ti_sysbios_heaps_HeapMem_Object__ *)oa) + i;
    }

    /* the bogus 'const' in the cast suppresses Klocwork MISRA complaints */
    return ((ti_sysbios_heaps_HeapMem_Object__ * const)ti_sysbios_heaps_HeapMem_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr ti_sysbios_heaps_HeapMem_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)ti_sysbios_heaps_HeapMem_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&ti_sysbios_heaps_HeapMem_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr ti_sysbios_heaps_HeapMem_Object__next__S( xdc_Ptr obj )
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&ti_sysbios_heaps_HeapMem_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr ti_sysbios_heaps_HeapMem_Object__create__S (
    xdc_CPtr __aa,
    const xdc_UChar *__paramsPtr,
    xdc_SizeT __psz,
    xdc_runtime_Error_Block *eb)
{
    ti_sysbios_heaps_HeapMem_Params __prms;
    ti_sysbios_heaps_HeapMem_Object *__obj;

    /* common instance initialization */
    __obj = xdc_runtime_Core_createObject__I(&ti_sysbios_heaps_HeapMem_Object__DESC__C, NULL, &__prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (__obj == NULL) {
        return NULL;
    }


    /* module-specific initialization */
    ti_sysbios_heaps_HeapMem_Instance_init__E(__obj, &__prms);
    return __obj;
}

/* create */
ti_sysbios_heaps_HeapMem_Handle ti_sysbios_heaps_HeapMem_create( const ti_sysbios_heaps_HeapMem_Params *__paramsPtr, xdc_runtime_Error_Block *eb )
{
    ti_sysbios_heaps_HeapMem_Params prms;
    ti_sysbios_heaps_HeapMem_Object *obj;


    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&ti_sysbios_heaps_HeapMem_Object__DESC__C, NULL, &prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    ti_sysbios_heaps_HeapMem_Instance_init__E(obj, &prms);
    return obj;
}

/* construct */
void ti_sysbios_heaps_HeapMem_construct(ti_sysbios_heaps_HeapMem_Struct *__obj, const ti_sysbios_heaps_HeapMem_Params *__paramsPtr )
{
    ti_sysbios_heaps_HeapMem_Params __prms;

    /* common instance initialization */
    xdc_runtime_Core_constructObject__I(&ti_sysbios_heaps_HeapMem_Object__DESC__C, __obj, &__prms, (xdc_CPtr)__paramsPtr, 0, NULL);

    /* module-specific initialization */
    ti_sysbios_heaps_HeapMem_Instance_init__E((xdc_Ptr)__obj, &__prms);
}

/* destruct */
void ti_sysbios_heaps_HeapMem_destruct(ti_sysbios_heaps_HeapMem_Struct *obj)
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_heaps_HeapMem_Object__DESC__C, obj, NULL, -1, TRUE);
}

/* Object__delete__S */
xdc_Void ti_sysbios_heaps_HeapMem_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_heaps_HeapMem_Object__DESC__C, *((ti_sysbios_heaps_HeapMem_Object**)instp), NULL, -1, FALSE);
    *((ti_sysbios_heaps_HeapMem_Handle*)instp) = NULL;
}

/* delete */
void ti_sysbios_heaps_HeapMem_delete(ti_sysbios_heaps_HeapMem_Handle *instp)
{
    ti_sysbios_heaps_HeapMem_Object__delete__S(instp);
}


/*
 * ======== ti.sysbios.heaps.HeapMem_Module_GateProxy SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#ifdef ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__id__CR
#define Module__MID (*((CT__ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__id *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__id__C_offset)))
#else
#define Module__MID ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__id__C
#endif

#undef Module__DGSINCL
#ifdef ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__diagsIncluded__CR
#define Module__DGSINCL (*((CT__ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__diagsIncluded *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__diagsIncluded__C_offset)))
#else
#define Module__DGSINCL ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__diagsIncluded__C
#endif

#undef Module__DGSENAB
#ifdef ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__diagsEnabled__CR
#define Module__DGSENAB (*((CT__ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__diagsEnabled *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__diagsEnabled__C_offset)))
#else
#define Module__DGSENAB ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__diagsEnabled__C
#endif

#undef Module__DGSMASK
#ifdef ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__diagsMask__CR
#define Module__DGSMASK (*((CT__ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__diagsMask *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__diagsMask__C_offset)))
#else
#define Module__DGSMASK ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__diagsMask__C
#endif

#undef Module__LOGDEF
#ifdef ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerDefined__CR
#define Module__LOGDEF (*((CT__ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerDefined *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerDefined__C_offset)))
#else
#define Module__LOGDEF ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerDefined__C
#endif

#undef Module__LOGOBJ
#ifdef ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerObj__CR
#define Module__LOGOBJ ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerObj__R
#define Module__LOGOBJ (*((CT__ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerObj *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerObj__C_offset)))
#else
#define Module__LOGOBJ ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerObj__C
#endif

#undef Module__LOGFXN0
#ifdef ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerFxn0__CR
#define Module__LOGFXN0 (*((CT__ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerFxn0 *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerFxn0__C_offset)))
#else
#define Module__LOGFXN0 ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerFxn0__C
#endif

#undef Module__LOGFXN1
#ifdef ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerFxn1__CR
#define Module__LOGFXN1 (*((CT__ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerFxn1 *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerFxn1__C_offset)))
#else
#define Module__LOGFXN1 ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerFxn1__C
#endif

#undef Module__LOGFXN2
#ifdef ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerFxn2__CR
#define Module__LOGFXN2 (*((CT__ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerFxn2 *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerFxn2__C_offset)))
#else
#define Module__LOGFXN2 ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerFxn2__C
#endif

#undef Module__LOGFXN4
#ifdef ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerFxn4__CR
#define Module__LOGFXN4 (*((CT__ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerFxn4 *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerFxn4__C_offset)))
#else
#define Module__LOGFXN4 ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerFxn4__C
#endif

#undef Module__LOGFXN8
#ifdef ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerFxn8__CR
#define Module__LOGFXN8 (*((CT__ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerFxn8 *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerFxn8__C_offset)))
#else
#define Module__LOGFXN8 ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__loggerFxn8__C
#endif

#undef Module__G_OBJ
#ifdef ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__gateObj__CR
#define Module__G_OBJ (*((CT__ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__gateObj *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__gateObj__C_offset)))
#else
#define Module__G_OBJ ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__gateObj__C
#endif

#undef Module__G_PRMS
#ifdef ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__gatePrms__CR
#define Module__G_PRMS (*((CT__ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__gatePrms *)(xdcRomConstPtr + ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__gatePrms__C_offset)))
#else
#define Module__G_PRMS ti_sysbios_heaps_HeapMem_Module_GateProxy_Module__gatePrms__C
#endif

#undef Module__GP_create
#define Module__GP_create ti_sysbios_heaps_HeapMem_Module_GateProxy_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sysbios_heaps_HeapMem_Module_GateProxy_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sysbios_heaps_HeapMem_Module_GateProxy_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sysbios_heaps_HeapMem_Module_GateProxy_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sysbios_heaps_HeapMem_Module_GateProxy_Module_GateProxy_query

xdc_Bool ti_sysbios_heaps_HeapMem_Module_GateProxy_Proxy__abstract__S(void)
{
    return 0;
}
xdc_CPtr ti_sysbios_heaps_HeapMem_Module_GateProxy_Proxy__delegate__S(void)
{
    return (const void *)&ti_sysbios_gates_GateMutex_Module__FXNS__C;
}



/*
 * ======== ti.sysbios.knl.Clock SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#ifdef ti_sysbios_knl_Clock_Module__id__CR
#define Module__MID (*((CT__ti_sysbios_knl_Clock_Module__id *)(xdcRomConstPtr + ti_sysbios_knl_Clock_Module__id__C_offset)))
#else
#define Module__MID ti_sysbios_knl_Clock_Module__id__C
#endif

#undef Module__DGSINCL
#ifdef ti_sysbios_knl_Clock_Module__diagsIncluded__CR
#define Module__DGSINCL (*((CT__ti_sysbios_knl_Clock_Module__diagsIncluded *)(xdcRomConstPtr + ti_sysbios_knl_Clock_Module__diagsIncluded__C_offset)))
#else
#define Module__DGSINCL ti_sysbios_knl_Clock_Module__diagsIncluded__C
#endif

#undef Module__DGSENAB
#ifdef ti_sysbios_knl_Clock_Module__diagsEnabled__CR
#define Module__DGSENAB (*((CT__ti_sysbios_knl_Clock_Module__diagsEnabled *)(xdcRomConstPtr + ti_sysbios_knl_Clock_Module__diagsEnabled__C_offset)))
#else
#define Module__DGSENAB ti_sysbios_knl_Clock_Module__diagsEnabled__C
#endif

#undef Module__DGSMASK
#ifdef ti_sysbios_knl_Clock_Module__diagsMask__CR
#define Module__DGSMASK (*((CT__ti_sysbios_knl_Clock_Module__diagsMask *)(xdcRomConstPtr + ti_sysbios_knl_Clock_Module__diagsMask__C_offset)))
#else
#define Module__DGSMASK ti_sysbios_knl_Clock_Module__diagsMask__C
#endif

#undef Module__LOGDEF
#ifdef ti_sysbios_knl_Clock_Module__loggerDefined__CR
#define Module__LOGDEF (*((CT__ti_sysbios_knl_Clock_Module__loggerDefined *)(xdcRomConstPtr + ti_sysbios_knl_Clock_Module__loggerDefined__C_offset)))
#else
#define Module__LOGDEF ti_sysbios_knl_Clock_Module__loggerDefined__C
#endif

#undef Module__LOGOBJ
#ifdef ti_sysbios_knl_Clock_Module__loggerObj__CR
#define Module__LOGOBJ ti_sysbios_knl_Clock_Module__loggerObj__R
#define Module__LOGOBJ (*((CT__ti_sysbios_knl_Clock_Module__loggerObj *)(xdcRomConstPtr + ti_sysbios_knl_Clock_Module__loggerObj__C_offset)))
#else
#define Module__LOGOBJ ti_sysbios_knl_Clock_Module__loggerObj__C
#endif

#undef Module__LOGFXN0
#ifdef ti_sysbios_knl_Clock_Module__loggerFxn0__CR
#define Module__LOGFXN0 (*((CT__ti_sysbios_knl_Clock_Module__loggerFxn0 *)(xdcRomConstPtr + ti_sysbios_knl_Clock_Module__loggerFxn0__C_offset)))
#else
#define Module__LOGFXN0 ti_sysbios_knl_Clock_Module__loggerFxn0__C
#endif

#undef Module__LOGFXN1
#ifdef ti_sysbios_knl_Clock_Module__loggerFxn1__CR
#define Module__LOGFXN1 (*((CT__ti_sysbios_knl_Clock_Module__loggerFxn1 *)(xdcRomConstPtr + ti_sysbios_knl_Clock_Module__loggerFxn1__C_offset)))
#else
#define Module__LOGFXN1 ti_sysbios_knl_Clock_Module__loggerFxn1__C
#endif

#undef Module__LOGFXN2
#ifdef ti_sysbios_knl_Clock_Module__loggerFxn2__CR
#define Module__LOGFXN2 (*((CT__ti_sysbios_knl_Clock_Module__loggerFxn2 *)(xdcRomConstPtr + ti_sysbios_knl_Clock_Module__loggerFxn2__C_offset)))
#else
#define Module__LOGFXN2 ti_sysbios_knl_Clock_Module__loggerFxn2__C
#endif

#undef Module__LOGFXN4
#ifdef ti_sysbios_knl_Clock_Module__loggerFxn4__CR
#define Module__LOGFXN4 (*((CT__ti_sysbios_knl_Clock_Module__loggerFxn4 *)(xdcRomConstPtr + ti_sysbios_knl_Clock_Module__loggerFxn4__C_offset)))
#else
#define Module__LOGFXN4 ti_sysbios_knl_Clock_Module__loggerFxn4__C
#endif

#undef Module__LOGFXN8
#ifdef ti_sysbios_knl_Clock_Module__loggerFxn8__CR
#define Module__LOGFXN8 (*((CT__ti_sysbios_knl_Clock_Module__loggerFxn8 *)(xdcRomConstPtr + ti_sysbios_knl_Clock_Module__loggerFxn8__C_offset)))
#else
#define Module__LOGFXN8 ti_sysbios_knl_Clock_Module__loggerFxn8__C
#endif

#undef Module__G_OBJ
#ifdef ti_sysbios_knl_Clock_Module__gateObj__CR
#define Module__G_OBJ (*((CT__ti_sysbios_knl_Clock_Module__gateObj *)(xdcRomConstPtr + ti_sysbios_knl_Clock_Module__gateObj__C_offset)))
#else
#define Module__G_OBJ ti_sysbios_knl_Clock_Module__gateObj__C
#endif

#undef Module__G_PRMS
#ifdef ti_sysbios_knl_Clock_Module__gatePrms__CR
#define Module__G_PRMS (*((CT__ti_sysbios_knl_Clock_Module__gatePrms *)(xdcRomConstPtr + ti_sysbios_knl_Clock_Module__gatePrms__C_offset)))
#else
#define Module__G_PRMS ti_sysbios_knl_Clock_Module__gatePrms__C
#endif

#undef Module__GP_create
#define Module__GP_create ti_sysbios_knl_Clock_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sysbios_knl_Clock_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sysbios_knl_Clock_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sysbios_knl_Clock_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sysbios_knl_Clock_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool ti_sysbios_knl_Clock_Module__startupDone__S( void )
{
    return ti_sysbios_knl_Clock_Module__startupDone__F();
}

/* Handle__label__S */
xdc_runtime_Types_Label *ti_sysbios_knl_Clock_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label *lab) 
{
    lab->handle = obj;
    lab->modId = 32791;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void ti_sysbios_knl_Clock_Params__init__S( xdc_Ptr prms, const void *src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_CPtr)(src ? src : &ti_sysbios_knl_Clock_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr ti_sysbios_knl_Clock_Object__get__S(xdc_Ptr oa, xdc_Int i)
{
    if (oa) {
        return ((ti_sysbios_knl_Clock_Object__ *)oa) + i;
    }

    return NULL;
}


/* Object__first__S */
xdc_Ptr ti_sysbios_knl_Clock_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)ti_sysbios_knl_Clock_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&ti_sysbios_knl_Clock_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr ti_sysbios_knl_Clock_Object__next__S( xdc_Ptr obj )
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&ti_sysbios_knl_Clock_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr ti_sysbios_knl_Clock_Object__create__S (
    xdc_CPtr __aa,
    const xdc_UChar *__paramsPtr,
    xdc_SizeT __psz,
    xdc_runtime_Error_Block *eb)
{
    const ti_sysbios_knl_Clock_Args__create *__args = __aa;
    ti_sysbios_knl_Clock_Params __prms;
    ti_sysbios_knl_Clock_Object *__obj;

    /* common instance initialization */
    __obj = xdc_runtime_Core_createObject__I(&ti_sysbios_knl_Clock_Object__DESC__C, NULL, &__prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (__obj == NULL) {
        return NULL;
    }


    /* module-specific initialization */
    ti_sysbios_knl_Clock_Instance_init__E(__obj, __args->clockFxn, __args->timeout, &__prms);
    return __obj;
}

/* create */
ti_sysbios_knl_Clock_Handle ti_sysbios_knl_Clock_create( ti_sysbios_knl_Clock_FuncPtr clockFxn, xdc_UInt timeout, const ti_sysbios_knl_Clock_Params *__paramsPtr, xdc_runtime_Error_Block *eb )
{
    ti_sysbios_knl_Clock_Params prms;
    ti_sysbios_knl_Clock_Object *obj;


    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&ti_sysbios_knl_Clock_Object__DESC__C, NULL, &prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    ti_sysbios_knl_Clock_Instance_init__E(obj, clockFxn, timeout, &prms);
    return obj;
}

/* construct */
void ti_sysbios_knl_Clock_construct(ti_sysbios_knl_Clock_Struct *__obj, ti_sysbios_knl_Clock_FuncPtr clockFxn, xdc_UInt timeout, const ti_sysbios_knl_Clock_Params *__paramsPtr )
{
    ti_sysbios_knl_Clock_Params __prms;

    /* common instance initialization */
    xdc_runtime_Core_constructObject__I(&ti_sysbios_knl_Clock_Object__DESC__C, __obj, &__prms, (xdc_CPtr)__paramsPtr, 0, NULL);

    /* module-specific initialization */
    ti_sysbios_knl_Clock_Instance_init__E((xdc_Ptr)__obj, clockFxn, timeout, &__prms);
}

/* destruct */
void ti_sysbios_knl_Clock_destruct(ti_sysbios_knl_Clock_Struct *obj)
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_knl_Clock_Object__DESC__C, obj, (xdc_Fxn)ti_sysbios_knl_Clock_Instance_finalize__E, -1, TRUE);
}

/* Object__delete__S */
xdc_Void ti_sysbios_knl_Clock_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_knl_Clock_Object__DESC__C, *((ti_sysbios_knl_Clock_Object**)instp), (xdc_Fxn)ti_sysbios_knl_Clock_Instance_finalize__E, -1, FALSE);
    *((ti_sysbios_knl_Clock_Handle*)instp) = NULL;
}

/* delete */
void ti_sysbios_knl_Clock_delete(ti_sysbios_knl_Clock_Handle *instp)
{
    ti_sysbios_knl_Clock_Object__delete__S(instp);
}


/*
 * ======== ti.sysbios.knl.Clock_TimerProxy SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#ifdef ti_sysbios_knl_Clock_TimerProxy_Module__id__CR
#define Module__MID (*((CT__ti_sysbios_knl_Clock_TimerProxy_Module__id *)(xdcRomConstPtr + ti_sysbios_knl_Clock_TimerProxy_Module__id__C_offset)))
#else
#define Module__MID ti_sysbios_knl_Clock_TimerProxy_Module__id__C
#endif

#undef Module__DGSINCL
#ifdef ti_sysbios_knl_Clock_TimerProxy_Module__diagsIncluded__CR
#define Module__DGSINCL (*((CT__ti_sysbios_knl_Clock_TimerProxy_Module__diagsIncluded *)(xdcRomConstPtr + ti_sysbios_knl_Clock_TimerProxy_Module__diagsIncluded__C_offset)))
#else
#define Module__DGSINCL ti_sysbios_knl_Clock_TimerProxy_Module__diagsIncluded__C
#endif

#undef Module__DGSENAB
#ifdef ti_sysbios_knl_Clock_TimerProxy_Module__diagsEnabled__CR
#define Module__DGSENAB (*((CT__ti_sysbios_knl_Clock_TimerProxy_Module__diagsEnabled *)(xdcRomConstPtr + ti_sysbios_knl_Clock_TimerProxy_Module__diagsEnabled__C_offset)))
#else
#define Module__DGSENAB ti_sysbios_knl_Clock_TimerProxy_Module__diagsEnabled__C
#endif

#undef Module__DGSMASK
#ifdef ti_sysbios_knl_Clock_TimerProxy_Module__diagsMask__CR
#define Module__DGSMASK (*((CT__ti_sysbios_knl_Clock_TimerProxy_Module__diagsMask *)(xdcRomConstPtr + ti_sysbios_knl_Clock_TimerProxy_Module__diagsMask__C_offset)))
#else
#define Module__DGSMASK ti_sysbios_knl_Clock_TimerProxy_Module__diagsMask__C
#endif

#undef Module__LOGDEF
#ifdef ti_sysbios_knl_Clock_TimerProxy_Module__loggerDefined__CR
#define Module__LOGDEF (*((CT__ti_sysbios_knl_Clock_TimerProxy_Module__loggerDefined *)(xdcRomConstPtr + ti_sysbios_knl_Clock_TimerProxy_Module__loggerDefined__C_offset)))
#else
#define Module__LOGDEF ti_sysbios_knl_Clock_TimerProxy_Module__loggerDefined__C
#endif

#undef Module__LOGOBJ
#ifdef ti_sysbios_knl_Clock_TimerProxy_Module__loggerObj__CR
#define Module__LOGOBJ ti_sysbios_knl_Clock_TimerProxy_Module__loggerObj__R
#define Module__LOGOBJ (*((CT__ti_sysbios_knl_Clock_TimerProxy_Module__loggerObj *)(xdcRomConstPtr + ti_sysbios_knl_Clock_TimerProxy_Module__loggerObj__C_offset)))
#else
#define Module__LOGOBJ ti_sysbios_knl_Clock_TimerProxy_Module__loggerObj__C
#endif

#undef Module__LOGFXN0
#ifdef ti_sysbios_knl_Clock_TimerProxy_Module__loggerFxn0__CR
#define Module__LOGFXN0 (*((CT__ti_sysbios_knl_Clock_TimerProxy_Module__loggerFxn0 *)(xdcRomConstPtr + ti_sysbios_knl_Clock_TimerProxy_Module__loggerFxn0__C_offset)))
#else
#define Module__LOGFXN0 ti_sysbios_knl_Clock_TimerProxy_Module__loggerFxn0__C
#endif

#undef Module__LOGFXN1
#ifdef ti_sysbios_knl_Clock_TimerProxy_Module__loggerFxn1__CR
#define Module__LOGFXN1 (*((CT__ti_sysbios_knl_Clock_TimerProxy_Module__loggerFxn1 *)(xdcRomConstPtr + ti_sysbios_knl_Clock_TimerProxy_Module__loggerFxn1__C_offset)))
#else
#define Module__LOGFXN1 ti_sysbios_knl_Clock_TimerProxy_Module__loggerFxn1__C
#endif

#undef Module__LOGFXN2
#ifdef ti_sysbios_knl_Clock_TimerProxy_Module__loggerFxn2__CR
#define Module__LOGFXN2 (*((CT__ti_sysbios_knl_Clock_TimerProxy_Module__loggerFxn2 *)(xdcRomConstPtr + ti_sysbios_knl_Clock_TimerProxy_Module__loggerFxn2__C_offset)))
#else
#define Module__LOGFXN2 ti_sysbios_knl_Clock_TimerProxy_Module__loggerFxn2__C
#endif

#undef Module__LOGFXN4
#ifdef ti_sysbios_knl_Clock_TimerProxy_Module__loggerFxn4__CR
#define Module__LOGFXN4 (*((CT__ti_sysbios_knl_Clock_TimerProxy_Module__loggerFxn4 *)(xdcRomConstPtr + ti_sysbios_knl_Clock_TimerProxy_Module__loggerFxn4__C_offset)))
#else
#define Module__LOGFXN4 ti_sysbios_knl_Clock_TimerProxy_Module__loggerFxn4__C
#endif

#undef Module__LOGFXN8
#ifdef ti_sysbios_knl_Clock_TimerProxy_Module__loggerFxn8__CR
#define Module__LOGFXN8 (*((CT__ti_sysbios_knl_Clock_TimerProxy_Module__loggerFxn8 *)(xdcRomConstPtr + ti_sysbios_knl_Clock_TimerProxy_Module__loggerFxn8__C_offset)))
#else
#define Module__LOGFXN8 ti_sysbios_knl_Clock_TimerProxy_Module__loggerFxn8__C
#endif

#undef Module__G_OBJ
#ifdef ti_sysbios_knl_Clock_TimerProxy_Module__gateObj__CR
#define Module__G_OBJ (*((CT__ti_sysbios_knl_Clock_TimerProxy_Module__gateObj *)(xdcRomConstPtr + ti_sysbios_knl_Clock_TimerProxy_Module__gateObj__C_offset)))
#else
#define Module__G_OBJ ti_sysbios_knl_Clock_TimerProxy_Module__gateObj__C
#endif

#undef Module__G_PRMS
#ifdef ti_sysbios_knl_Clock_TimerProxy_Module__gatePrms__CR
#define Module__G_PRMS (*((CT__ti_sysbios_knl_Clock_TimerProxy_Module__gatePrms *)(xdcRomConstPtr + ti_sysbios_knl_Clock_TimerProxy_Module__gatePrms__C_offset)))
#else
#define Module__G_PRMS ti_sysbios_knl_Clock_TimerProxy_Module__gatePrms__C
#endif

#undef Module__GP_create
#define Module__GP_create ti_sysbios_knl_Clock_TimerProxy_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sysbios_knl_Clock_TimerProxy_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sysbios_knl_Clock_TimerProxy_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sysbios_knl_Clock_TimerProxy_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sysbios_knl_Clock_TimerProxy_Module_GateProxy_query

xdc_Bool ti_sysbios_knl_Clock_TimerProxy_Proxy__abstract__S(void)
{
    return 0;
}
xdc_CPtr ti_sysbios_knl_Clock_TimerProxy_Proxy__delegate__S(void)
{
    return 0;
}



/*
 * ======== ti.sysbios.knl.Event SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#ifdef ti_sysbios_knl_Event_Module__id__CR
#define Module__MID (*((CT__ti_sysbios_knl_Event_Module__id *)(xdcRomConstPtr + ti_sysbios_knl_Event_Module__id__C_offset)))
#else
#define Module__MID ti_sysbios_knl_Event_Module__id__C
#endif

#undef Module__DGSINCL
#ifdef ti_sysbios_knl_Event_Module__diagsIncluded__CR
#define Module__DGSINCL (*((CT__ti_sysbios_knl_Event_Module__diagsIncluded *)(xdcRomConstPtr + ti_sysbios_knl_Event_Module__diagsIncluded__C_offset)))
#else
#define Module__DGSINCL ti_sysbios_knl_Event_Module__diagsIncluded__C
#endif

#undef Module__DGSENAB
#ifdef ti_sysbios_knl_Event_Module__diagsEnabled__CR
#define Module__DGSENAB (*((CT__ti_sysbios_knl_Event_Module__diagsEnabled *)(xdcRomConstPtr + ti_sysbios_knl_Event_Module__diagsEnabled__C_offset)))
#else
#define Module__DGSENAB ti_sysbios_knl_Event_Module__diagsEnabled__C
#endif

#undef Module__DGSMASK
#ifdef ti_sysbios_knl_Event_Module__diagsMask__CR
#define Module__DGSMASK (*((CT__ti_sysbios_knl_Event_Module__diagsMask *)(xdcRomConstPtr + ti_sysbios_knl_Event_Module__diagsMask__C_offset)))
#else
#define Module__DGSMASK ti_sysbios_knl_Event_Module__diagsMask__C
#endif

#undef Module__LOGDEF
#ifdef ti_sysbios_knl_Event_Module__loggerDefined__CR
#define Module__LOGDEF (*((CT__ti_sysbios_knl_Event_Module__loggerDefined *)(xdcRomConstPtr + ti_sysbios_knl_Event_Module__loggerDefined__C_offset)))
#else
#define Module__LOGDEF ti_sysbios_knl_Event_Module__loggerDefined__C
#endif

#undef Module__LOGOBJ
#ifdef ti_sysbios_knl_Event_Module__loggerObj__CR
#define Module__LOGOBJ ti_sysbios_knl_Event_Module__loggerObj__R
#define Module__LOGOBJ (*((CT__ti_sysbios_knl_Event_Module__loggerObj *)(xdcRomConstPtr + ti_sysbios_knl_Event_Module__loggerObj__C_offset)))
#else
#define Module__LOGOBJ ti_sysbios_knl_Event_Module__loggerObj__C
#endif

#undef Module__LOGFXN0
#ifdef ti_sysbios_knl_Event_Module__loggerFxn0__CR
#define Module__LOGFXN0 (*((CT__ti_sysbios_knl_Event_Module__loggerFxn0 *)(xdcRomConstPtr + ti_sysbios_knl_Event_Module__loggerFxn0__C_offset)))
#else
#define Module__LOGFXN0 ti_sysbios_knl_Event_Module__loggerFxn0__C
#endif

#undef Module__LOGFXN1
#ifdef ti_sysbios_knl_Event_Module__loggerFxn1__CR
#define Module__LOGFXN1 (*((CT__ti_sysbios_knl_Event_Module__loggerFxn1 *)(xdcRomConstPtr + ti_sysbios_knl_Event_Module__loggerFxn1__C_offset)))
#else
#define Module__LOGFXN1 ti_sysbios_knl_Event_Module__loggerFxn1__C
#endif

#undef Module__LOGFXN2
#ifdef ti_sysbios_knl_Event_Module__loggerFxn2__CR
#define Module__LOGFXN2 (*((CT__ti_sysbios_knl_Event_Module__loggerFxn2 *)(xdcRomConstPtr + ti_sysbios_knl_Event_Module__loggerFxn2__C_offset)))
#else
#define Module__LOGFXN2 ti_sysbios_knl_Event_Module__loggerFxn2__C
#endif

#undef Module__LOGFXN4
#ifdef ti_sysbios_knl_Event_Module__loggerFxn4__CR
#define Module__LOGFXN4 (*((CT__ti_sysbios_knl_Event_Module__loggerFxn4 *)(xdcRomConstPtr + ti_sysbios_knl_Event_Module__loggerFxn4__C_offset)))
#else
#define Module__LOGFXN4 ti_sysbios_knl_Event_Module__loggerFxn4__C
#endif

#undef Module__LOGFXN8
#ifdef ti_sysbios_knl_Event_Module__loggerFxn8__CR
#define Module__LOGFXN8 (*((CT__ti_sysbios_knl_Event_Module__loggerFxn8 *)(xdcRomConstPtr + ti_sysbios_knl_Event_Module__loggerFxn8__C_offset)))
#else
#define Module__LOGFXN8 ti_sysbios_knl_Event_Module__loggerFxn8__C
#endif

#undef Module__G_OBJ
#ifdef ti_sysbios_knl_Event_Module__gateObj__CR
#define Module__G_OBJ (*((CT__ti_sysbios_knl_Event_Module__gateObj *)(xdcRomConstPtr + ti_sysbios_knl_Event_Module__gateObj__C_offset)))
#else
#define Module__G_OBJ ti_sysbios_knl_Event_Module__gateObj__C
#endif

#undef Module__G_PRMS
#ifdef ti_sysbios_knl_Event_Module__gatePrms__CR
#define Module__G_PRMS (*((CT__ti_sysbios_knl_Event_Module__gatePrms *)(xdcRomConstPtr + ti_sysbios_knl_Event_Module__gatePrms__C_offset)))
#else
#define Module__G_PRMS ti_sysbios_knl_Event_Module__gatePrms__C
#endif

#undef Module__GP_create
#define Module__GP_create ti_sysbios_knl_Event_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sysbios_knl_Event_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sysbios_knl_Event_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sysbios_knl_Event_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sysbios_knl_Event_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool ti_sysbios_knl_Event_Module__startupDone__S( void )
{
    return 1;
}

/* Handle__label__S */
xdc_runtime_Types_Label *ti_sysbios_knl_Event_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label *lab) 
{
    lab->handle = obj;
    lab->modId = 32794;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void ti_sysbios_knl_Event_Params__init__S( xdc_Ptr prms, const void *src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_CPtr)(src ? src : &ti_sysbios_knl_Event_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr ti_sysbios_knl_Event_Object__get__S(xdc_Ptr oa, xdc_Int i)
{
    if (oa) {
        return ((ti_sysbios_knl_Event_Object__ *)oa) + i;
    }

    return NULL;
}


/* Object__first__S */
xdc_Ptr ti_sysbios_knl_Event_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)ti_sysbios_knl_Event_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&ti_sysbios_knl_Event_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr ti_sysbios_knl_Event_Object__next__S( xdc_Ptr obj )
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&ti_sysbios_knl_Event_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr ti_sysbios_knl_Event_Object__create__S (
    xdc_CPtr __aa,
    const xdc_UChar *__paramsPtr,
    xdc_SizeT __psz,
    xdc_runtime_Error_Block *eb)
{
    ti_sysbios_knl_Event_Params __prms;
    ti_sysbios_knl_Event_Object *__obj;

    /* common instance initialization */
    __obj = xdc_runtime_Core_createObject__I(&ti_sysbios_knl_Event_Object__DESC__C, NULL, &__prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (__obj == NULL) {
        return NULL;
    }


    /* module-specific initialization */
    ti_sysbios_knl_Event_Instance_init__E(__obj, &__prms);
    return __obj;
}

/* create */
ti_sysbios_knl_Event_Handle ti_sysbios_knl_Event_create( const ti_sysbios_knl_Event_Params *__paramsPtr, xdc_runtime_Error_Block *eb )
{
    ti_sysbios_knl_Event_Params prms;
    ti_sysbios_knl_Event_Object *obj;


    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&ti_sysbios_knl_Event_Object__DESC__C, NULL, &prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    ti_sysbios_knl_Event_Instance_init__E(obj, &prms);
    return obj;
}

/* construct */
void ti_sysbios_knl_Event_construct(ti_sysbios_knl_Event_Struct *__obj, const ti_sysbios_knl_Event_Params *__paramsPtr )
{
    ti_sysbios_knl_Event_Params __prms;

    /* common instance initialization */
    xdc_runtime_Core_constructObject__I(&ti_sysbios_knl_Event_Object__DESC__C, __obj, &__prms, (xdc_CPtr)__paramsPtr, 0, NULL);

    /* module-specific initialization */
    ti_sysbios_knl_Event_Instance_init__E((xdc_Ptr)__obj, &__prms);
}

/* destruct */
void ti_sysbios_knl_Event_destruct(ti_sysbios_knl_Event_Struct *obj)
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_knl_Event_Object__DESC__C, obj, NULL, -1, TRUE);
}

/* Object__delete__S */
xdc_Void ti_sysbios_knl_Event_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_knl_Event_Object__DESC__C, *((ti_sysbios_knl_Event_Object**)instp), NULL, -1, FALSE);
    *((ti_sysbios_knl_Event_Handle*)instp) = NULL;
}

/* delete */
void ti_sysbios_knl_Event_delete(ti_sysbios_knl_Event_Handle *instp)
{
    ti_sysbios_knl_Event_Object__delete__S(instp);
}


/*
 * ======== ti.sysbios.knl.Idle SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool ti_sysbios_knl_Idle_Module__startupDone__S( void )
{
    return 1;
}



/*
 * ======== ti.sysbios.knl.Intrinsics SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool ti_sysbios_knl_Intrinsics_Module__startupDone__S( void )
{
    return 1;
}



/*
 * ======== ti.sysbios.knl.Intrinsics_SupportProxy SYSTEM FUNCTIONS ========
 */


xdc_Bool ti_sysbios_knl_Intrinsics_SupportProxy_Proxy__abstract__S(void)
{
    return 0;
}
xdc_CPtr ti_sysbios_knl_Intrinsics_SupportProxy_Proxy__delegate__S(void)
{
    return 0;
}


/*
 * ======== ti.sysbios.knl.Queue SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#ifdef ti_sysbios_knl_Queue_Module__id__CR
#define Module__MID (*((CT__ti_sysbios_knl_Queue_Module__id *)(xdcRomConstPtr + ti_sysbios_knl_Queue_Module__id__C_offset)))
#else
#define Module__MID ti_sysbios_knl_Queue_Module__id__C
#endif

#undef Module__DGSINCL
#ifdef ti_sysbios_knl_Queue_Module__diagsIncluded__CR
#define Module__DGSINCL (*((CT__ti_sysbios_knl_Queue_Module__diagsIncluded *)(xdcRomConstPtr + ti_sysbios_knl_Queue_Module__diagsIncluded__C_offset)))
#else
#define Module__DGSINCL ti_sysbios_knl_Queue_Module__diagsIncluded__C
#endif

#undef Module__DGSENAB
#ifdef ti_sysbios_knl_Queue_Module__diagsEnabled__CR
#define Module__DGSENAB (*((CT__ti_sysbios_knl_Queue_Module__diagsEnabled *)(xdcRomConstPtr + ti_sysbios_knl_Queue_Module__diagsEnabled__C_offset)))
#else
#define Module__DGSENAB ti_sysbios_knl_Queue_Module__diagsEnabled__C
#endif

#undef Module__DGSMASK
#ifdef ti_sysbios_knl_Queue_Module__diagsMask__CR
#define Module__DGSMASK (*((CT__ti_sysbios_knl_Queue_Module__diagsMask *)(xdcRomConstPtr + ti_sysbios_knl_Queue_Module__diagsMask__C_offset)))
#else
#define Module__DGSMASK ti_sysbios_knl_Queue_Module__diagsMask__C
#endif

#undef Module__LOGDEF
#ifdef ti_sysbios_knl_Queue_Module__loggerDefined__CR
#define Module__LOGDEF (*((CT__ti_sysbios_knl_Queue_Module__loggerDefined *)(xdcRomConstPtr + ti_sysbios_knl_Queue_Module__loggerDefined__C_offset)))
#else
#define Module__LOGDEF ti_sysbios_knl_Queue_Module__loggerDefined__C
#endif

#undef Module__LOGOBJ
#ifdef ti_sysbios_knl_Queue_Module__loggerObj__CR
#define Module__LOGOBJ ti_sysbios_knl_Queue_Module__loggerObj__R
#define Module__LOGOBJ (*((CT__ti_sysbios_knl_Queue_Module__loggerObj *)(xdcRomConstPtr + ti_sysbios_knl_Queue_Module__loggerObj__C_offset)))
#else
#define Module__LOGOBJ ti_sysbios_knl_Queue_Module__loggerObj__C
#endif

#undef Module__LOGFXN0
#ifdef ti_sysbios_knl_Queue_Module__loggerFxn0__CR
#define Module__LOGFXN0 (*((CT__ti_sysbios_knl_Queue_Module__loggerFxn0 *)(xdcRomConstPtr + ti_sysbios_knl_Queue_Module__loggerFxn0__C_offset)))
#else
#define Module__LOGFXN0 ti_sysbios_knl_Queue_Module__loggerFxn0__C
#endif

#undef Module__LOGFXN1
#ifdef ti_sysbios_knl_Queue_Module__loggerFxn1__CR
#define Module__LOGFXN1 (*((CT__ti_sysbios_knl_Queue_Module__loggerFxn1 *)(xdcRomConstPtr + ti_sysbios_knl_Queue_Module__loggerFxn1__C_offset)))
#else
#define Module__LOGFXN1 ti_sysbios_knl_Queue_Module__loggerFxn1__C
#endif

#undef Module__LOGFXN2
#ifdef ti_sysbios_knl_Queue_Module__loggerFxn2__CR
#define Module__LOGFXN2 (*((CT__ti_sysbios_knl_Queue_Module__loggerFxn2 *)(xdcRomConstPtr + ti_sysbios_knl_Queue_Module__loggerFxn2__C_offset)))
#else
#define Module__LOGFXN2 ti_sysbios_knl_Queue_Module__loggerFxn2__C
#endif

#undef Module__LOGFXN4
#ifdef ti_sysbios_knl_Queue_Module__loggerFxn4__CR
#define Module__LOGFXN4 (*((CT__ti_sysbios_knl_Queue_Module__loggerFxn4 *)(xdcRomConstPtr + ti_sysbios_knl_Queue_Module__loggerFxn4__C_offset)))
#else
#define Module__LOGFXN4 ti_sysbios_knl_Queue_Module__loggerFxn4__C
#endif

#undef Module__LOGFXN8
#ifdef ti_sysbios_knl_Queue_Module__loggerFxn8__CR
#define Module__LOGFXN8 (*((CT__ti_sysbios_knl_Queue_Module__loggerFxn8 *)(xdcRomConstPtr + ti_sysbios_knl_Queue_Module__loggerFxn8__C_offset)))
#else
#define Module__LOGFXN8 ti_sysbios_knl_Queue_Module__loggerFxn8__C
#endif

#undef Module__G_OBJ
#ifdef ti_sysbios_knl_Queue_Module__gateObj__CR
#define Module__G_OBJ (*((CT__ti_sysbios_knl_Queue_Module__gateObj *)(xdcRomConstPtr + ti_sysbios_knl_Queue_Module__gateObj__C_offset)))
#else
#define Module__G_OBJ ti_sysbios_knl_Queue_Module__gateObj__C
#endif

#undef Module__G_PRMS
#ifdef ti_sysbios_knl_Queue_Module__gatePrms__CR
#define Module__G_PRMS (*((CT__ti_sysbios_knl_Queue_Module__gatePrms *)(xdcRomConstPtr + ti_sysbios_knl_Queue_Module__gatePrms__C_offset)))
#else
#define Module__G_PRMS ti_sysbios_knl_Queue_Module__gatePrms__C
#endif

#undef Module__GP_create
#define Module__GP_create ti_sysbios_knl_Queue_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sysbios_knl_Queue_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sysbios_knl_Queue_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sysbios_knl_Queue_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sysbios_knl_Queue_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool ti_sysbios_knl_Queue_Module__startupDone__S( void )
{
    return 1;
}

/* Handle__label__S */
xdc_runtime_Types_Label *ti_sysbios_knl_Queue_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label *lab) 
{
    lab->handle = obj;
    lab->modId = 32795;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void ti_sysbios_knl_Queue_Params__init__S( xdc_Ptr prms, const void *src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_CPtr)(src ? src : &ti_sysbios_knl_Queue_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr ti_sysbios_knl_Queue_Object__get__S(xdc_Ptr oa, xdc_Int i)
{
    if (oa) {
        return ((ti_sysbios_knl_Queue_Object__ *)oa) + i;
    }

    return NULL;
}


/* Object__first__S */
xdc_Ptr ti_sysbios_knl_Queue_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)ti_sysbios_knl_Queue_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&ti_sysbios_knl_Queue_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr ti_sysbios_knl_Queue_Object__next__S( xdc_Ptr obj )
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&ti_sysbios_knl_Queue_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr ti_sysbios_knl_Queue_Object__create__S (
    xdc_CPtr __aa,
    const xdc_UChar *__paramsPtr,
    xdc_SizeT __psz,
    xdc_runtime_Error_Block *eb)
{
    ti_sysbios_knl_Queue_Params __prms;
    ti_sysbios_knl_Queue_Object *__obj;

    /* common instance initialization */
    __obj = xdc_runtime_Core_createObject__I(&ti_sysbios_knl_Queue_Object__DESC__C, NULL, &__prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (__obj == NULL) {
        return NULL;
    }


    /* module-specific initialization */
    ti_sysbios_knl_Queue_Instance_init__E(__obj, &__prms);
    return __obj;
}

/* create */
ti_sysbios_knl_Queue_Handle ti_sysbios_knl_Queue_create( const ti_sysbios_knl_Queue_Params *__paramsPtr, xdc_runtime_Error_Block *eb )
{
    ti_sysbios_knl_Queue_Params prms;
    ti_sysbios_knl_Queue_Object *obj;


    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&ti_sysbios_knl_Queue_Object__DESC__C, NULL, &prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    ti_sysbios_knl_Queue_Instance_init__E(obj, &prms);
    return obj;
}

/* construct */
void ti_sysbios_knl_Queue_construct(ti_sysbios_knl_Queue_Struct *__obj, const ti_sysbios_knl_Queue_Params *__paramsPtr )
{
    ti_sysbios_knl_Queue_Params __prms;

    /* common instance initialization */
    xdc_runtime_Core_constructObject__I(&ti_sysbios_knl_Queue_Object__DESC__C, __obj, &__prms, (xdc_CPtr)__paramsPtr, 0, NULL);

    /* module-specific initialization */
    ti_sysbios_knl_Queue_Instance_init__E((xdc_Ptr)__obj, &__prms);
}

/* destruct */
void ti_sysbios_knl_Queue_destruct(ti_sysbios_knl_Queue_Struct *obj)
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_knl_Queue_Object__DESC__C, obj, NULL, -1, TRUE);
}

/* Object__delete__S */
xdc_Void ti_sysbios_knl_Queue_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_knl_Queue_Object__DESC__C, *((ti_sysbios_knl_Queue_Object**)instp), NULL, -1, FALSE);
    *((ti_sysbios_knl_Queue_Handle*)instp) = NULL;
}

/* delete */
void ti_sysbios_knl_Queue_delete(ti_sysbios_knl_Queue_Handle *instp)
{
    ti_sysbios_knl_Queue_Object__delete__S(instp);
}


/*
 * ======== ti.sysbios.knl.Semaphore SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#ifdef ti_sysbios_knl_Semaphore_Module__id__CR
#define Module__MID (*((CT__ti_sysbios_knl_Semaphore_Module__id *)(xdcRomConstPtr + ti_sysbios_knl_Semaphore_Module__id__C_offset)))
#else
#define Module__MID ti_sysbios_knl_Semaphore_Module__id__C
#endif

#undef Module__DGSINCL
#ifdef ti_sysbios_knl_Semaphore_Module__diagsIncluded__CR
#define Module__DGSINCL (*((CT__ti_sysbios_knl_Semaphore_Module__diagsIncluded *)(xdcRomConstPtr + ti_sysbios_knl_Semaphore_Module__diagsIncluded__C_offset)))
#else
#define Module__DGSINCL ti_sysbios_knl_Semaphore_Module__diagsIncluded__C
#endif

#undef Module__DGSENAB
#ifdef ti_sysbios_knl_Semaphore_Module__diagsEnabled__CR
#define Module__DGSENAB (*((CT__ti_sysbios_knl_Semaphore_Module__diagsEnabled *)(xdcRomConstPtr + ti_sysbios_knl_Semaphore_Module__diagsEnabled__C_offset)))
#else
#define Module__DGSENAB ti_sysbios_knl_Semaphore_Module__diagsEnabled__C
#endif

#undef Module__DGSMASK
#ifdef ti_sysbios_knl_Semaphore_Module__diagsMask__CR
#define Module__DGSMASK (*((CT__ti_sysbios_knl_Semaphore_Module__diagsMask *)(xdcRomConstPtr + ti_sysbios_knl_Semaphore_Module__diagsMask__C_offset)))
#else
#define Module__DGSMASK ti_sysbios_knl_Semaphore_Module__diagsMask__C
#endif

#undef Module__LOGDEF
#ifdef ti_sysbios_knl_Semaphore_Module__loggerDefined__CR
#define Module__LOGDEF (*((CT__ti_sysbios_knl_Semaphore_Module__loggerDefined *)(xdcRomConstPtr + ti_sysbios_knl_Semaphore_Module__loggerDefined__C_offset)))
#else
#define Module__LOGDEF ti_sysbios_knl_Semaphore_Module__loggerDefined__C
#endif

#undef Module__LOGOBJ
#ifdef ti_sysbios_knl_Semaphore_Module__loggerObj__CR
#define Module__LOGOBJ ti_sysbios_knl_Semaphore_Module__loggerObj__R
#define Module__LOGOBJ (*((CT__ti_sysbios_knl_Semaphore_Module__loggerObj *)(xdcRomConstPtr + ti_sysbios_knl_Semaphore_Module__loggerObj__C_offset)))
#else
#define Module__LOGOBJ ti_sysbios_knl_Semaphore_Module__loggerObj__C
#endif

#undef Module__LOGFXN0
#ifdef ti_sysbios_knl_Semaphore_Module__loggerFxn0__CR
#define Module__LOGFXN0 (*((CT__ti_sysbios_knl_Semaphore_Module__loggerFxn0 *)(xdcRomConstPtr + ti_sysbios_knl_Semaphore_Module__loggerFxn0__C_offset)))
#else
#define Module__LOGFXN0 ti_sysbios_knl_Semaphore_Module__loggerFxn0__C
#endif

#undef Module__LOGFXN1
#ifdef ti_sysbios_knl_Semaphore_Module__loggerFxn1__CR
#define Module__LOGFXN1 (*((CT__ti_sysbios_knl_Semaphore_Module__loggerFxn1 *)(xdcRomConstPtr + ti_sysbios_knl_Semaphore_Module__loggerFxn1__C_offset)))
#else
#define Module__LOGFXN1 ti_sysbios_knl_Semaphore_Module__loggerFxn1__C
#endif

#undef Module__LOGFXN2
#ifdef ti_sysbios_knl_Semaphore_Module__loggerFxn2__CR
#define Module__LOGFXN2 (*((CT__ti_sysbios_knl_Semaphore_Module__loggerFxn2 *)(xdcRomConstPtr + ti_sysbios_knl_Semaphore_Module__loggerFxn2__C_offset)))
#else
#define Module__LOGFXN2 ti_sysbios_knl_Semaphore_Module__loggerFxn2__C
#endif

#undef Module__LOGFXN4
#ifdef ti_sysbios_knl_Semaphore_Module__loggerFxn4__CR
#define Module__LOGFXN4 (*((CT__ti_sysbios_knl_Semaphore_Module__loggerFxn4 *)(xdcRomConstPtr + ti_sysbios_knl_Semaphore_Module__loggerFxn4__C_offset)))
#else
#define Module__LOGFXN4 ti_sysbios_knl_Semaphore_Module__loggerFxn4__C
#endif

#undef Module__LOGFXN8
#ifdef ti_sysbios_knl_Semaphore_Module__loggerFxn8__CR
#define Module__LOGFXN8 (*((CT__ti_sysbios_knl_Semaphore_Module__loggerFxn8 *)(xdcRomConstPtr + ti_sysbios_knl_Semaphore_Module__loggerFxn8__C_offset)))
#else
#define Module__LOGFXN8 ti_sysbios_knl_Semaphore_Module__loggerFxn8__C
#endif

#undef Module__G_OBJ
#ifdef ti_sysbios_knl_Semaphore_Module__gateObj__CR
#define Module__G_OBJ (*((CT__ti_sysbios_knl_Semaphore_Module__gateObj *)(xdcRomConstPtr + ti_sysbios_knl_Semaphore_Module__gateObj__C_offset)))
#else
#define Module__G_OBJ ti_sysbios_knl_Semaphore_Module__gateObj__C
#endif

#undef Module__G_PRMS
#ifdef ti_sysbios_knl_Semaphore_Module__gatePrms__CR
#define Module__G_PRMS (*((CT__ti_sysbios_knl_Semaphore_Module__gatePrms *)(xdcRomConstPtr + ti_sysbios_knl_Semaphore_Module__gatePrms__C_offset)))
#else
#define Module__G_PRMS ti_sysbios_knl_Semaphore_Module__gatePrms__C
#endif

#undef Module__GP_create
#define Module__GP_create ti_sysbios_knl_Semaphore_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sysbios_knl_Semaphore_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sysbios_knl_Semaphore_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sysbios_knl_Semaphore_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sysbios_knl_Semaphore_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool ti_sysbios_knl_Semaphore_Module__startupDone__S( void )
{
    return 1;
}

/* Handle__label__S */
xdc_runtime_Types_Label *ti_sysbios_knl_Semaphore_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label *lab) 
{
    lab->handle = obj;
    lab->modId = 32796;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void ti_sysbios_knl_Semaphore_Params__init__S( xdc_Ptr prms, const void *src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_CPtr)(src ? src : &ti_sysbios_knl_Semaphore_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr ti_sysbios_knl_Semaphore_Object__get__S(xdc_Ptr oa, xdc_Int i)
{
    if (oa) {
        return ((ti_sysbios_knl_Semaphore_Object__ *)oa) + i;
    }

    return NULL;
}


/* Object__first__S */
xdc_Ptr ti_sysbios_knl_Semaphore_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)ti_sysbios_knl_Semaphore_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&ti_sysbios_knl_Semaphore_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr ti_sysbios_knl_Semaphore_Object__next__S( xdc_Ptr obj )
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&ti_sysbios_knl_Semaphore_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr ti_sysbios_knl_Semaphore_Object__create__S (
    xdc_CPtr __aa,
    const xdc_UChar *__paramsPtr,
    xdc_SizeT __psz,
    xdc_runtime_Error_Block *eb)
{
    const ti_sysbios_knl_Semaphore_Args__create *__args = __aa;
    ti_sysbios_knl_Semaphore_Params __prms;
    ti_sysbios_knl_Semaphore_Object *__obj;

    /* common instance initialization */
    __obj = xdc_runtime_Core_createObject__I(&ti_sysbios_knl_Semaphore_Object__DESC__C, NULL, &__prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (__obj == NULL) {
        return NULL;
    }


    /* module-specific initialization */
    ti_sysbios_knl_Semaphore_Instance_init__E(__obj, __args->count, &__prms);
    return __obj;
}

/* create */
ti_sysbios_knl_Semaphore_Handle ti_sysbios_knl_Semaphore_create( xdc_Int count, const ti_sysbios_knl_Semaphore_Params *__paramsPtr, xdc_runtime_Error_Block *eb )
{
    ti_sysbios_knl_Semaphore_Params prms;
    ti_sysbios_knl_Semaphore_Object *obj;


    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&ti_sysbios_knl_Semaphore_Object__DESC__C, NULL, &prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    ti_sysbios_knl_Semaphore_Instance_init__E(obj, count, &prms);
    return obj;
}

/* construct */
void ti_sysbios_knl_Semaphore_construct(ti_sysbios_knl_Semaphore_Struct *__obj, xdc_Int count, const ti_sysbios_knl_Semaphore_Params *__paramsPtr )
{
    ti_sysbios_knl_Semaphore_Params __prms;

    /* common instance initialization */
    xdc_runtime_Core_constructObject__I(&ti_sysbios_knl_Semaphore_Object__DESC__C, __obj, &__prms, (xdc_CPtr)__paramsPtr, 0, NULL);

    /* module-specific initialization */
    ti_sysbios_knl_Semaphore_Instance_init__E((xdc_Ptr)__obj, count, &__prms);
}

/* destruct */
void ti_sysbios_knl_Semaphore_destruct(ti_sysbios_knl_Semaphore_Struct *obj)
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_knl_Semaphore_Object__DESC__C, obj, (xdc_Fxn)ti_sysbios_knl_Semaphore_Instance_finalize__E, -1, TRUE);
}

/* Object__delete__S */
xdc_Void ti_sysbios_knl_Semaphore_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_knl_Semaphore_Object__DESC__C, *((ti_sysbios_knl_Semaphore_Object**)instp), (xdc_Fxn)ti_sysbios_knl_Semaphore_Instance_finalize__E, -1, FALSE);
    *((ti_sysbios_knl_Semaphore_Handle*)instp) = NULL;
}

/* delete */
void ti_sysbios_knl_Semaphore_delete(ti_sysbios_knl_Semaphore_Handle *instp)
{
    ti_sysbios_knl_Semaphore_Object__delete__S(instp);
}


/*
 * ======== ti.sysbios.knl.Swi SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#ifdef ti_sysbios_knl_Swi_Module__id__CR
#define Module__MID (*((CT__ti_sysbios_knl_Swi_Module__id *)(xdcRomConstPtr + ti_sysbios_knl_Swi_Module__id__C_offset)))
#else
#define Module__MID ti_sysbios_knl_Swi_Module__id__C
#endif

#undef Module__DGSINCL
#ifdef ti_sysbios_knl_Swi_Module__diagsIncluded__CR
#define Module__DGSINCL (*((CT__ti_sysbios_knl_Swi_Module__diagsIncluded *)(xdcRomConstPtr + ti_sysbios_knl_Swi_Module__diagsIncluded__C_offset)))
#else
#define Module__DGSINCL ti_sysbios_knl_Swi_Module__diagsIncluded__C
#endif

#undef Module__DGSENAB
#ifdef ti_sysbios_knl_Swi_Module__diagsEnabled__CR
#define Module__DGSENAB (*((CT__ti_sysbios_knl_Swi_Module__diagsEnabled *)(xdcRomConstPtr + ti_sysbios_knl_Swi_Module__diagsEnabled__C_offset)))
#else
#define Module__DGSENAB ti_sysbios_knl_Swi_Module__diagsEnabled__C
#endif

#undef Module__DGSMASK
#ifdef ti_sysbios_knl_Swi_Module__diagsMask__CR
#define Module__DGSMASK (*((CT__ti_sysbios_knl_Swi_Module__diagsMask *)(xdcRomConstPtr + ti_sysbios_knl_Swi_Module__diagsMask__C_offset)))
#else
#define Module__DGSMASK ti_sysbios_knl_Swi_Module__diagsMask__C
#endif

#undef Module__LOGDEF
#ifdef ti_sysbios_knl_Swi_Module__loggerDefined__CR
#define Module__LOGDEF (*((CT__ti_sysbios_knl_Swi_Module__loggerDefined *)(xdcRomConstPtr + ti_sysbios_knl_Swi_Module__loggerDefined__C_offset)))
#else
#define Module__LOGDEF ti_sysbios_knl_Swi_Module__loggerDefined__C
#endif

#undef Module__LOGOBJ
#ifdef ti_sysbios_knl_Swi_Module__loggerObj__CR
#define Module__LOGOBJ ti_sysbios_knl_Swi_Module__loggerObj__R
#define Module__LOGOBJ (*((CT__ti_sysbios_knl_Swi_Module__loggerObj *)(xdcRomConstPtr + ti_sysbios_knl_Swi_Module__loggerObj__C_offset)))
#else
#define Module__LOGOBJ ti_sysbios_knl_Swi_Module__loggerObj__C
#endif

#undef Module__LOGFXN0
#ifdef ti_sysbios_knl_Swi_Module__loggerFxn0__CR
#define Module__LOGFXN0 (*((CT__ti_sysbios_knl_Swi_Module__loggerFxn0 *)(xdcRomConstPtr + ti_sysbios_knl_Swi_Module__loggerFxn0__C_offset)))
#else
#define Module__LOGFXN0 ti_sysbios_knl_Swi_Module__loggerFxn0__C
#endif

#undef Module__LOGFXN1
#ifdef ti_sysbios_knl_Swi_Module__loggerFxn1__CR
#define Module__LOGFXN1 (*((CT__ti_sysbios_knl_Swi_Module__loggerFxn1 *)(xdcRomConstPtr + ti_sysbios_knl_Swi_Module__loggerFxn1__C_offset)))
#else
#define Module__LOGFXN1 ti_sysbios_knl_Swi_Module__loggerFxn1__C
#endif

#undef Module__LOGFXN2
#ifdef ti_sysbios_knl_Swi_Module__loggerFxn2__CR
#define Module__LOGFXN2 (*((CT__ti_sysbios_knl_Swi_Module__loggerFxn2 *)(xdcRomConstPtr + ti_sysbios_knl_Swi_Module__loggerFxn2__C_offset)))
#else
#define Module__LOGFXN2 ti_sysbios_knl_Swi_Module__loggerFxn2__C
#endif

#undef Module__LOGFXN4
#ifdef ti_sysbios_knl_Swi_Module__loggerFxn4__CR
#define Module__LOGFXN4 (*((CT__ti_sysbios_knl_Swi_Module__loggerFxn4 *)(xdcRomConstPtr + ti_sysbios_knl_Swi_Module__loggerFxn4__C_offset)))
#else
#define Module__LOGFXN4 ti_sysbios_knl_Swi_Module__loggerFxn4__C
#endif

#undef Module__LOGFXN8
#ifdef ti_sysbios_knl_Swi_Module__loggerFxn8__CR
#define Module__LOGFXN8 (*((CT__ti_sysbios_knl_Swi_Module__loggerFxn8 *)(xdcRomConstPtr + ti_sysbios_knl_Swi_Module__loggerFxn8__C_offset)))
#else
#define Module__LOGFXN8 ti_sysbios_knl_Swi_Module__loggerFxn8__C
#endif

#undef Module__G_OBJ
#ifdef ti_sysbios_knl_Swi_Module__gateObj__CR
#define Module__G_OBJ (*((CT__ti_sysbios_knl_Swi_Module__gateObj *)(xdcRomConstPtr + ti_sysbios_knl_Swi_Module__gateObj__C_offset)))
#else
#define Module__G_OBJ ti_sysbios_knl_Swi_Module__gateObj__C
#endif

#undef Module__G_PRMS
#ifdef ti_sysbios_knl_Swi_Module__gatePrms__CR
#define Module__G_PRMS (*((CT__ti_sysbios_knl_Swi_Module__gatePrms *)(xdcRomConstPtr + ti_sysbios_knl_Swi_Module__gatePrms__C_offset)))
#else
#define Module__G_PRMS ti_sysbios_knl_Swi_Module__gatePrms__C
#endif

#undef Module__GP_create
#define Module__GP_create ti_sysbios_knl_Swi_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sysbios_knl_Swi_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sysbios_knl_Swi_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sysbios_knl_Swi_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sysbios_knl_Swi_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool ti_sysbios_knl_Swi_Module__startupDone__S( void )
{
    return ti_sysbios_knl_Swi_Module__startupDone__F();
}

/* Handle__label__S */
xdc_runtime_Types_Label *ti_sysbios_knl_Swi_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label *lab) 
{
    lab->handle = obj;
    lab->modId = 32797;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void ti_sysbios_knl_Swi_Params__init__S( xdc_Ptr prms, const void *src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_CPtr)(src ? src : &ti_sysbios_knl_Swi_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr ti_sysbios_knl_Swi_Object__get__S(xdc_Ptr oa, xdc_Int i)
{
    if (oa) {
        return ((ti_sysbios_knl_Swi_Object__ *)oa) + i;
    }

    /* the bogus 'const' in the cast suppresses Klocwork MISRA complaints */
    return ((ti_sysbios_knl_Swi_Object__ * const)ti_sysbios_knl_Swi_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr ti_sysbios_knl_Swi_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)ti_sysbios_knl_Swi_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&ti_sysbios_knl_Swi_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr ti_sysbios_knl_Swi_Object__next__S( xdc_Ptr obj )
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&ti_sysbios_knl_Swi_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr ti_sysbios_knl_Swi_Object__create__S (
    xdc_CPtr __aa,
    const xdc_UChar *__paramsPtr,
    xdc_SizeT __psz,
    xdc_runtime_Error_Block *eb)
{
    const ti_sysbios_knl_Swi_Args__create *__args = __aa;
    ti_sysbios_knl_Swi_Params __prms;
    ti_sysbios_knl_Swi_Object *__obj;
    int iStat;

    /* common instance initialization */
    __obj = xdc_runtime_Core_createObject__I(&ti_sysbios_knl_Swi_Object__DESC__C, NULL, &__prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (__obj == NULL) {
        return NULL;
    }


    /* module-specific initialization */
    iStat = ti_sysbios_knl_Swi_Instance_init__E(__obj, __args->swiFxn, &__prms, eb);
    if (iStat) {
        xdc_runtime_Core_deleteObject__I(&ti_sysbios_knl_Swi_Object__DESC__C, __obj, (xdc_Fxn)ti_sysbios_knl_Swi_Instance_finalize__E, iStat, 0);
        return NULL;
    }

    return __obj;
}

/* create */
ti_sysbios_knl_Swi_Handle ti_sysbios_knl_Swi_create( ti_sysbios_knl_Swi_FuncPtr swiFxn, const ti_sysbios_knl_Swi_Params *__paramsPtr, xdc_runtime_Error_Block *eb )
{
    ti_sysbios_knl_Swi_Params prms;
    ti_sysbios_knl_Swi_Object *obj;

    int iStat;

    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&ti_sysbios_knl_Swi_Object__DESC__C, NULL, &prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    iStat = ti_sysbios_knl_Swi_Instance_init__E(obj, swiFxn, &prms, eb);
    if (iStat) {
        xdc_runtime_Core_deleteObject__I(&ti_sysbios_knl_Swi_Object__DESC__C, obj, (xdc_Fxn)ti_sysbios_knl_Swi_Instance_finalize__E, iStat, 0);
        return NULL;
    }

    return obj;
}

/* construct */
void ti_sysbios_knl_Swi_construct(ti_sysbios_knl_Swi_Struct *__obj, ti_sysbios_knl_Swi_FuncPtr swiFxn, const ti_sysbios_knl_Swi_Params *__paramsPtr, xdc_runtime_Error_Block *eb )
{
    ti_sysbios_knl_Swi_Params __prms;
    int iStat;

    /* common instance initialization */
    xdc_runtime_Core_constructObject__I(&ti_sysbios_knl_Swi_Object__DESC__C, __obj, &__prms, (xdc_CPtr)__paramsPtr, 0, eb);

    /* module-specific initialization */
    iStat = ti_sysbios_knl_Swi_Instance_init__E((xdc_Ptr)__obj, swiFxn, &__prms, eb);
    if (iStat) {
        xdc_runtime_Core_deleteObject__I(&ti_sysbios_knl_Swi_Object__DESC__C, __obj, (xdc_Fxn)ti_sysbios_knl_Swi_Instance_finalize__E, iStat, 1);
    }

}

/* destruct */
void ti_sysbios_knl_Swi_destruct(ti_sysbios_knl_Swi_Struct *obj)
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_knl_Swi_Object__DESC__C, obj, (xdc_Fxn)ti_sysbios_knl_Swi_Instance_finalize__E, 0, TRUE);
}

/* Object__delete__S */
xdc_Void ti_sysbios_knl_Swi_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_knl_Swi_Object__DESC__C, *((ti_sysbios_knl_Swi_Object**)instp), (xdc_Fxn)ti_sysbios_knl_Swi_Instance_finalize__E, 0, FALSE);
    *((ti_sysbios_knl_Swi_Handle*)instp) = NULL;
}

/* delete */
void ti_sysbios_knl_Swi_delete(ti_sysbios_knl_Swi_Handle *instp)
{
    ti_sysbios_knl_Swi_Object__delete__S(instp);
}


/*
 * ======== ti.sysbios.knl.Task SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#ifdef ti_sysbios_knl_Task_Module__id__CR
#define Module__MID (*((CT__ti_sysbios_knl_Task_Module__id *)(xdcRomConstPtr + ti_sysbios_knl_Task_Module__id__C_offset)))
#else
#define Module__MID ti_sysbios_knl_Task_Module__id__C
#endif

#undef Module__DGSINCL
#ifdef ti_sysbios_knl_Task_Module__diagsIncluded__CR
#define Module__DGSINCL (*((CT__ti_sysbios_knl_Task_Module__diagsIncluded *)(xdcRomConstPtr + ti_sysbios_knl_Task_Module__diagsIncluded__C_offset)))
#else
#define Module__DGSINCL ti_sysbios_knl_Task_Module__diagsIncluded__C
#endif

#undef Module__DGSENAB
#ifdef ti_sysbios_knl_Task_Module__diagsEnabled__CR
#define Module__DGSENAB (*((CT__ti_sysbios_knl_Task_Module__diagsEnabled *)(xdcRomConstPtr + ti_sysbios_knl_Task_Module__diagsEnabled__C_offset)))
#else
#define Module__DGSENAB ti_sysbios_knl_Task_Module__diagsEnabled__C
#endif

#undef Module__DGSMASK
#ifdef ti_sysbios_knl_Task_Module__diagsMask__CR
#define Module__DGSMASK (*((CT__ti_sysbios_knl_Task_Module__diagsMask *)(xdcRomConstPtr + ti_sysbios_knl_Task_Module__diagsMask__C_offset)))
#else
#define Module__DGSMASK ti_sysbios_knl_Task_Module__diagsMask__C
#endif

#undef Module__LOGDEF
#ifdef ti_sysbios_knl_Task_Module__loggerDefined__CR
#define Module__LOGDEF (*((CT__ti_sysbios_knl_Task_Module__loggerDefined *)(xdcRomConstPtr + ti_sysbios_knl_Task_Module__loggerDefined__C_offset)))
#else
#define Module__LOGDEF ti_sysbios_knl_Task_Module__loggerDefined__C
#endif

#undef Module__LOGOBJ
#ifdef ti_sysbios_knl_Task_Module__loggerObj__CR
#define Module__LOGOBJ ti_sysbios_knl_Task_Module__loggerObj__R
#define Module__LOGOBJ (*((CT__ti_sysbios_knl_Task_Module__loggerObj *)(xdcRomConstPtr + ti_sysbios_knl_Task_Module__loggerObj__C_offset)))
#else
#define Module__LOGOBJ ti_sysbios_knl_Task_Module__loggerObj__C
#endif

#undef Module__LOGFXN0
#ifdef ti_sysbios_knl_Task_Module__loggerFxn0__CR
#define Module__LOGFXN0 (*((CT__ti_sysbios_knl_Task_Module__loggerFxn0 *)(xdcRomConstPtr + ti_sysbios_knl_Task_Module__loggerFxn0__C_offset)))
#else
#define Module__LOGFXN0 ti_sysbios_knl_Task_Module__loggerFxn0__C
#endif

#undef Module__LOGFXN1
#ifdef ti_sysbios_knl_Task_Module__loggerFxn1__CR
#define Module__LOGFXN1 (*((CT__ti_sysbios_knl_Task_Module__loggerFxn1 *)(xdcRomConstPtr + ti_sysbios_knl_Task_Module__loggerFxn1__C_offset)))
#else
#define Module__LOGFXN1 ti_sysbios_knl_Task_Module__loggerFxn1__C
#endif

#undef Module__LOGFXN2
#ifdef ti_sysbios_knl_Task_Module__loggerFxn2__CR
#define Module__LOGFXN2 (*((CT__ti_sysbios_knl_Task_Module__loggerFxn2 *)(xdcRomConstPtr + ti_sysbios_knl_Task_Module__loggerFxn2__C_offset)))
#else
#define Module__LOGFXN2 ti_sysbios_knl_Task_Module__loggerFxn2__C
#endif

#undef Module__LOGFXN4
#ifdef ti_sysbios_knl_Task_Module__loggerFxn4__CR
#define Module__LOGFXN4 (*((CT__ti_sysbios_knl_Task_Module__loggerFxn4 *)(xdcRomConstPtr + ti_sysbios_knl_Task_Module__loggerFxn4__C_offset)))
#else
#define Module__LOGFXN4 ti_sysbios_knl_Task_Module__loggerFxn4__C
#endif

#undef Module__LOGFXN8
#ifdef ti_sysbios_knl_Task_Module__loggerFxn8__CR
#define Module__LOGFXN8 (*((CT__ti_sysbios_knl_Task_Module__loggerFxn8 *)(xdcRomConstPtr + ti_sysbios_knl_Task_Module__loggerFxn8__C_offset)))
#else
#define Module__LOGFXN8 ti_sysbios_knl_Task_Module__loggerFxn8__C
#endif

#undef Module__G_OBJ
#ifdef ti_sysbios_knl_Task_Module__gateObj__CR
#define Module__G_OBJ (*((CT__ti_sysbios_knl_Task_Module__gateObj *)(xdcRomConstPtr + ti_sysbios_knl_Task_Module__gateObj__C_offset)))
#else
#define Module__G_OBJ ti_sysbios_knl_Task_Module__gateObj__C
#endif

#undef Module__G_PRMS
#ifdef ti_sysbios_knl_Task_Module__gatePrms__CR
#define Module__G_PRMS (*((CT__ti_sysbios_knl_Task_Module__gatePrms *)(xdcRomConstPtr + ti_sysbios_knl_Task_Module__gatePrms__C_offset)))
#else
#define Module__G_PRMS ti_sysbios_knl_Task_Module__gatePrms__C
#endif

#undef Module__GP_create
#define Module__GP_create ti_sysbios_knl_Task_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sysbios_knl_Task_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sysbios_knl_Task_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sysbios_knl_Task_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sysbios_knl_Task_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool ti_sysbios_knl_Task_Module__startupDone__S( void )
{
    return ti_sysbios_knl_Task_Module__startupDone__F();
}

/* Handle__label__S */
xdc_runtime_Types_Label *ti_sysbios_knl_Task_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label *lab) 
{
    lab->handle = obj;
    lab->modId = 32798;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void ti_sysbios_knl_Task_Params__init__S( xdc_Ptr prms, const void *src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_CPtr)(src ? src : &ti_sysbios_knl_Task_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr ti_sysbios_knl_Task_Object__get__S(xdc_Ptr oa, xdc_Int i)
{
    if (oa) {
        return ((ti_sysbios_knl_Task_Object__ *)oa) + i;
    }

    /* the bogus 'const' in the cast suppresses Klocwork MISRA complaints */
    return ((ti_sysbios_knl_Task_Object__ * const)ti_sysbios_knl_Task_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr ti_sysbios_knl_Task_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)ti_sysbios_knl_Task_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&ti_sysbios_knl_Task_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr ti_sysbios_knl_Task_Object__next__S( xdc_Ptr obj )
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&ti_sysbios_knl_Task_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr ti_sysbios_knl_Task_Object__create__S (
    xdc_CPtr __aa,
    const xdc_UChar *__paramsPtr,
    xdc_SizeT __psz,
    xdc_runtime_Error_Block *eb)
{
    const ti_sysbios_knl_Task_Args__create *__args = __aa;
    ti_sysbios_knl_Task_Params __prms;
    ti_sysbios_knl_Task_Object *__obj;
    int iStat;

    /* common instance initialization */
    __obj = xdc_runtime_Core_createObject__I(&ti_sysbios_knl_Task_Object__DESC__C, NULL, &__prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (__obj == NULL) {
        return NULL;
    }


    /* module-specific initialization */
    iStat = ti_sysbios_knl_Task_Instance_init__E(__obj, __args->fxn, &__prms, eb);
    if (iStat) {
        xdc_runtime_Core_deleteObject__I(&ti_sysbios_knl_Task_Object__DESC__C, __obj, (xdc_Fxn)ti_sysbios_knl_Task_Instance_finalize__E, iStat, 0);
        return NULL;
    }

    return __obj;
}

/* create */
ti_sysbios_knl_Task_Handle ti_sysbios_knl_Task_create( ti_sysbios_knl_Task_FuncPtr fxn, const ti_sysbios_knl_Task_Params *__paramsPtr, xdc_runtime_Error_Block *eb )
{
    ti_sysbios_knl_Task_Params prms;
    ti_sysbios_knl_Task_Object *obj;

    int iStat;

    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&ti_sysbios_knl_Task_Object__DESC__C, NULL, &prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    iStat = ti_sysbios_knl_Task_Instance_init__E(obj, fxn, &prms, eb);
    if (iStat) {
        xdc_runtime_Core_deleteObject__I(&ti_sysbios_knl_Task_Object__DESC__C, obj, (xdc_Fxn)ti_sysbios_knl_Task_Instance_finalize__E, iStat, 0);
        return NULL;
    }

    return obj;
}

/* construct */
void ti_sysbios_knl_Task_construct(ti_sysbios_knl_Task_Struct *__obj, ti_sysbios_knl_Task_FuncPtr fxn, const ti_sysbios_knl_Task_Params *__paramsPtr, xdc_runtime_Error_Block *eb )
{
    ti_sysbios_knl_Task_Params __prms;
    int iStat;

    /* common instance initialization */
    xdc_runtime_Core_constructObject__I(&ti_sysbios_knl_Task_Object__DESC__C, __obj, &__prms, (xdc_CPtr)__paramsPtr, 0, eb);

    /* module-specific initialization */
    iStat = ti_sysbios_knl_Task_Instance_init__E((xdc_Ptr)__obj, fxn, &__prms, eb);
    if (iStat) {
        xdc_runtime_Core_deleteObject__I(&ti_sysbios_knl_Task_Object__DESC__C, __obj, (xdc_Fxn)ti_sysbios_knl_Task_Instance_finalize__E, iStat, 1);
    }

}

/* destruct */
void ti_sysbios_knl_Task_destruct(ti_sysbios_knl_Task_Struct *obj)
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_knl_Task_Object__DESC__C, obj, (xdc_Fxn)ti_sysbios_knl_Task_Instance_finalize__E, 0, TRUE);
}

/* Object__delete__S */
xdc_Void ti_sysbios_knl_Task_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_knl_Task_Object__DESC__C, *((ti_sysbios_knl_Task_Object**)instp), (xdc_Fxn)ti_sysbios_knl_Task_Instance_finalize__E, 0, FALSE);
    *((ti_sysbios_knl_Task_Handle*)instp) = NULL;
}

/* delete */
void ti_sysbios_knl_Task_delete(ti_sysbios_knl_Task_Handle *instp)
{
    ti_sysbios_knl_Task_Object__delete__S(instp);
}


/*
 * ======== ti.sysbios.knl.Task_SupportProxy SYSTEM FUNCTIONS ========
 */


xdc_Bool ti_sysbios_knl_Task_SupportProxy_Proxy__abstract__S(void)
{
    return 0;
}
xdc_CPtr ti_sysbios_knl_Task_SupportProxy_Proxy__delegate__S(void)
{
    return 0;
}


/*
 * ======== ti.sysbios.timers.rti.Timer SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#ifdef ti_sysbios_timers_rti_Timer_Module__id__CR
#define Module__MID (*((CT__ti_sysbios_timers_rti_Timer_Module__id *)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__id__C_offset)))
#else
#define Module__MID ti_sysbios_timers_rti_Timer_Module__id__C
#endif

#undef Module__DGSINCL
#ifdef ti_sysbios_timers_rti_Timer_Module__diagsIncluded__CR
#define Module__DGSINCL (*((CT__ti_sysbios_timers_rti_Timer_Module__diagsIncluded *)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__diagsIncluded__C_offset)))
#else
#define Module__DGSINCL ti_sysbios_timers_rti_Timer_Module__diagsIncluded__C
#endif

#undef Module__DGSENAB
#ifdef ti_sysbios_timers_rti_Timer_Module__diagsEnabled__CR
#define Module__DGSENAB (*((CT__ti_sysbios_timers_rti_Timer_Module__diagsEnabled *)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__diagsEnabled__C_offset)))
#else
#define Module__DGSENAB ti_sysbios_timers_rti_Timer_Module__diagsEnabled__C
#endif

#undef Module__DGSMASK
#ifdef ti_sysbios_timers_rti_Timer_Module__diagsMask__CR
#define Module__DGSMASK (*((CT__ti_sysbios_timers_rti_Timer_Module__diagsMask *)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__diagsMask__C_offset)))
#else
#define Module__DGSMASK ti_sysbios_timers_rti_Timer_Module__diagsMask__C
#endif

#undef Module__LOGDEF
#ifdef ti_sysbios_timers_rti_Timer_Module__loggerDefined__CR
#define Module__LOGDEF (*((CT__ti_sysbios_timers_rti_Timer_Module__loggerDefined *)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__loggerDefined__C_offset)))
#else
#define Module__LOGDEF ti_sysbios_timers_rti_Timer_Module__loggerDefined__C
#endif

#undef Module__LOGOBJ
#ifdef ti_sysbios_timers_rti_Timer_Module__loggerObj__CR
#define Module__LOGOBJ ti_sysbios_timers_rti_Timer_Module__loggerObj__R
#define Module__LOGOBJ (*((CT__ti_sysbios_timers_rti_Timer_Module__loggerObj *)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__loggerObj__C_offset)))
#else
#define Module__LOGOBJ ti_sysbios_timers_rti_Timer_Module__loggerObj__C
#endif

#undef Module__LOGFXN0
#ifdef ti_sysbios_timers_rti_Timer_Module__loggerFxn0__CR
#define Module__LOGFXN0 (*((CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn0 *)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__loggerFxn0__C_offset)))
#else
#define Module__LOGFXN0 ti_sysbios_timers_rti_Timer_Module__loggerFxn0__C
#endif

#undef Module__LOGFXN1
#ifdef ti_sysbios_timers_rti_Timer_Module__loggerFxn1__CR
#define Module__LOGFXN1 (*((CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn1 *)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__loggerFxn1__C_offset)))
#else
#define Module__LOGFXN1 ti_sysbios_timers_rti_Timer_Module__loggerFxn1__C
#endif

#undef Module__LOGFXN2
#ifdef ti_sysbios_timers_rti_Timer_Module__loggerFxn2__CR
#define Module__LOGFXN2 (*((CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn2 *)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__loggerFxn2__C_offset)))
#else
#define Module__LOGFXN2 ti_sysbios_timers_rti_Timer_Module__loggerFxn2__C
#endif

#undef Module__LOGFXN4
#ifdef ti_sysbios_timers_rti_Timer_Module__loggerFxn4__CR
#define Module__LOGFXN4 (*((CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn4 *)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__loggerFxn4__C_offset)))
#else
#define Module__LOGFXN4 ti_sysbios_timers_rti_Timer_Module__loggerFxn4__C
#endif

#undef Module__LOGFXN8
#ifdef ti_sysbios_timers_rti_Timer_Module__loggerFxn8__CR
#define Module__LOGFXN8 (*((CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn8 *)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__loggerFxn8__C_offset)))
#else
#define Module__LOGFXN8 ti_sysbios_timers_rti_Timer_Module__loggerFxn8__C
#endif

#undef Module__G_OBJ
#ifdef ti_sysbios_timers_rti_Timer_Module__gateObj__CR
#define Module__G_OBJ (*((CT__ti_sysbios_timers_rti_Timer_Module__gateObj *)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__gateObj__C_offset)))
#else
#define Module__G_OBJ ti_sysbios_timers_rti_Timer_Module__gateObj__C
#endif

#undef Module__G_PRMS
#ifdef ti_sysbios_timers_rti_Timer_Module__gatePrms__CR
#define Module__G_PRMS (*((CT__ti_sysbios_timers_rti_Timer_Module__gatePrms *)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__gatePrms__C_offset)))
#else
#define Module__G_PRMS ti_sysbios_timers_rti_Timer_Module__gatePrms__C
#endif

#undef Module__GP_create
#define Module__GP_create ti_sysbios_timers_rti_Timer_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sysbios_timers_rti_Timer_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sysbios_timers_rti_Timer_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sysbios_timers_rti_Timer_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sysbios_timers_rti_Timer_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool ti_sysbios_timers_rti_Timer_Module__startupDone__S( void )
{
    return ti_sysbios_timers_rti_Timer_Module__startupDone__F();
}

/* Handle__label__S */
xdc_runtime_Types_Label *ti_sysbios_timers_rti_Timer_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label *lab) 
{
    lab->handle = obj;
    lab->modId = 32819;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void ti_sysbios_timers_rti_Timer_Params__init__S( xdc_Ptr prms, const void *src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_CPtr)(src ? src : &ti_sysbios_timers_rti_Timer_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr ti_sysbios_timers_rti_Timer_Object__get__S(xdc_Ptr oa, xdc_Int i)
{
    if (oa) {
        return ((ti_sysbios_timers_rti_Timer_Object__ *)oa) + i;
    }

    /* the bogus 'const' in the cast suppresses Klocwork MISRA complaints */
    return ((ti_sysbios_timers_rti_Timer_Object__ * const)ti_sysbios_timers_rti_Timer_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr ti_sysbios_timers_rti_Timer_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)ti_sysbios_timers_rti_Timer_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&ti_sysbios_timers_rti_Timer_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr ti_sysbios_timers_rti_Timer_Object__next__S( xdc_Ptr obj )
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&ti_sysbios_timers_rti_Timer_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr ti_sysbios_timers_rti_Timer_Object__create__S (
    xdc_CPtr __aa,
    const xdc_UChar *__paramsPtr,
    xdc_SizeT __psz,
    xdc_runtime_Error_Block *eb)
{
    const ti_sysbios_timers_rti_Timer_Args__create *__args = __aa;
    ti_sysbios_timers_rti_Timer_Params __prms;
    ti_sysbios_timers_rti_Timer_Object *__obj;
    int iStat;

    /* common instance initialization */
    __obj = xdc_runtime_Core_createObject__I(&ti_sysbios_timers_rti_Timer_Object__DESC__C, NULL, &__prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (__obj == NULL) {
        return NULL;
    }


    /* module-specific initialization */
    iStat = ti_sysbios_timers_rti_Timer_Instance_init__E(__obj, __args->id, __args->tickFxn, &__prms, eb);
    if (iStat) {
        xdc_runtime_Core_deleteObject__I(&ti_sysbios_timers_rti_Timer_Object__DESC__C, __obj, (xdc_Fxn)ti_sysbios_timers_rti_Timer_Instance_finalize__E, iStat, 0);
        return NULL;
    }

    return __obj;
}

/* create */
ti_sysbios_timers_rti_Timer_Handle ti_sysbios_timers_rti_Timer_create( xdc_Int id, ti_sysbios_interfaces_ITimer_FuncPtr tickFxn, const ti_sysbios_timers_rti_Timer_Params *__paramsPtr, xdc_runtime_Error_Block *eb )
{
    ti_sysbios_timers_rti_Timer_Params prms;
    ti_sysbios_timers_rti_Timer_Object *obj;

    int iStat;

    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&ti_sysbios_timers_rti_Timer_Object__DESC__C, NULL, &prms, (xdc_CPtr)__paramsPtr, 0, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    iStat = ti_sysbios_timers_rti_Timer_Instance_init__E(obj, id, tickFxn, &prms, eb);
    if (iStat) {
        xdc_runtime_Core_deleteObject__I(&ti_sysbios_timers_rti_Timer_Object__DESC__C, obj, (xdc_Fxn)ti_sysbios_timers_rti_Timer_Instance_finalize__E, iStat, 0);
        return NULL;
    }

    return obj;
}

/* construct */
void ti_sysbios_timers_rti_Timer_construct(ti_sysbios_timers_rti_Timer_Struct *__obj, xdc_Int id, ti_sysbios_interfaces_ITimer_FuncPtr tickFxn, const ti_sysbios_timers_rti_Timer_Params *__paramsPtr, xdc_runtime_Error_Block *eb )
{
    ti_sysbios_timers_rti_Timer_Params __prms;
    int iStat;

    /* common instance initialization */
    xdc_runtime_Core_constructObject__I(&ti_sysbios_timers_rti_Timer_Object__DESC__C, __obj, &__prms, (xdc_CPtr)__paramsPtr, 0, eb);

    /* module-specific initialization */
    iStat = ti_sysbios_timers_rti_Timer_Instance_init__E((xdc_Ptr)__obj, id, tickFxn, &__prms, eb);
    if (iStat) {
        xdc_runtime_Core_deleteObject__I(&ti_sysbios_timers_rti_Timer_Object__DESC__C, __obj, (xdc_Fxn)ti_sysbios_timers_rti_Timer_Instance_finalize__E, iStat, 1);
    }

}

/* destruct */
void ti_sysbios_timers_rti_Timer_destruct(ti_sysbios_timers_rti_Timer_Struct *obj)
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_timers_rti_Timer_Object__DESC__C, obj, (xdc_Fxn)ti_sysbios_timers_rti_Timer_Instance_finalize__E, 0, TRUE);
}

/* Object__delete__S */
xdc_Void ti_sysbios_timers_rti_Timer_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Core_deleteObject__I(&ti_sysbios_timers_rti_Timer_Object__DESC__C, *((ti_sysbios_timers_rti_Timer_Object**)instp), (xdc_Fxn)ti_sysbios_timers_rti_Timer_Instance_finalize__E, 0, FALSE);
    *((ti_sysbios_timers_rti_Timer_Handle*)instp) = NULL;
}

/* delete */
void ti_sysbios_timers_rti_Timer_delete(ti_sysbios_timers_rti_Timer_Handle *instp)
{
    ti_sysbios_timers_rti_Timer_Object__delete__S(instp);
}


/*
 * ======== xdc.runtime.Assert SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Assert_Module__startupDone__S( void )
{
    return 1;
}



/*
 * ======== xdc.runtime.Core SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Core_Module__startupDone__S( void )
{
    return 1;
}



/*
 * ======== xdc.runtime.Defaults SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Defaults_Module__startupDone__S( void )
{
    return 1;
}



/*
 * ======== xdc.runtime.Diags SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Diags_Module__startupDone__S( void )
{
    return 1;
}



/*
 * ======== xdc.runtime.Error SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Error_Module__startupDone__S( void )
{
    return 1;
}



/*
 * ======== xdc.runtime.Gate SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Gate_Module__startupDone__S( void )
{
    return 1;
}



/*
 * ======== xdc.runtime.Log SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Log_Module__startupDone__S( void )
{
    return 1;
}



/*
 * ======== xdc.runtime.Main SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Main_Module__startupDone__S( void )
{
    return 1;
}



/*
 * ======== xdc.runtime.Main_Module_GateProxy SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#ifdef xdc_runtime_Main_Module_GateProxy_Module__id__CR
#define Module__MID (*((CT__xdc_runtime_Main_Module_GateProxy_Module__id *)(xdcRomConstPtr + xdc_runtime_Main_Module_GateProxy_Module__id__C_offset)))
#else
#define Module__MID xdc_runtime_Main_Module_GateProxy_Module__id__C
#endif

#undef Module__DGSINCL
#ifdef xdc_runtime_Main_Module_GateProxy_Module__diagsIncluded__CR
#define Module__DGSINCL (*((CT__xdc_runtime_Main_Module_GateProxy_Module__diagsIncluded *)(xdcRomConstPtr + xdc_runtime_Main_Module_GateProxy_Module__diagsIncluded__C_offset)))
#else
#define Module__DGSINCL xdc_runtime_Main_Module_GateProxy_Module__diagsIncluded__C
#endif

#undef Module__DGSENAB
#ifdef xdc_runtime_Main_Module_GateProxy_Module__diagsEnabled__CR
#define Module__DGSENAB (*((CT__xdc_runtime_Main_Module_GateProxy_Module__diagsEnabled *)(xdcRomConstPtr + xdc_runtime_Main_Module_GateProxy_Module__diagsEnabled__C_offset)))
#else
#define Module__DGSENAB xdc_runtime_Main_Module_GateProxy_Module__diagsEnabled__C
#endif

#undef Module__DGSMASK
#ifdef xdc_runtime_Main_Module_GateProxy_Module__diagsMask__CR
#define Module__DGSMASK (*((CT__xdc_runtime_Main_Module_GateProxy_Module__diagsMask *)(xdcRomConstPtr + xdc_runtime_Main_Module_GateProxy_Module__diagsMask__C_offset)))
#else
#define Module__DGSMASK xdc_runtime_Main_Module_GateProxy_Module__diagsMask__C
#endif

#undef Module__LOGDEF
#ifdef xdc_runtime_Main_Module_GateProxy_Module__loggerDefined__CR
#define Module__LOGDEF (*((CT__xdc_runtime_Main_Module_GateProxy_Module__loggerDefined *)(xdcRomConstPtr + xdc_runtime_Main_Module_GateProxy_Module__loggerDefined__C_offset)))
#else
#define Module__LOGDEF xdc_runtime_Main_Module_GateProxy_Module__loggerDefined__C
#endif

#undef Module__LOGOBJ
#ifdef xdc_runtime_Main_Module_GateProxy_Module__loggerObj__CR
#define Module__LOGOBJ xdc_runtime_Main_Module_GateProxy_Module__loggerObj__R
#define Module__LOGOBJ (*((CT__xdc_runtime_Main_Module_GateProxy_Module__loggerObj *)(xdcRomConstPtr + xdc_runtime_Main_Module_GateProxy_Module__loggerObj__C_offset)))
#else
#define Module__LOGOBJ xdc_runtime_Main_Module_GateProxy_Module__loggerObj__C
#endif

#undef Module__LOGFXN0
#ifdef xdc_runtime_Main_Module_GateProxy_Module__loggerFxn0__CR
#define Module__LOGFXN0 (*((CT__xdc_runtime_Main_Module_GateProxy_Module__loggerFxn0 *)(xdcRomConstPtr + xdc_runtime_Main_Module_GateProxy_Module__loggerFxn0__C_offset)))
#else
#define Module__LOGFXN0 xdc_runtime_Main_Module_GateProxy_Module__loggerFxn0__C
#endif

#undef Module__LOGFXN1
#ifdef xdc_runtime_Main_Module_GateProxy_Module__loggerFxn1__CR
#define Module__LOGFXN1 (*((CT__xdc_runtime_Main_Module_GateProxy_Module__loggerFxn1 *)(xdcRomConstPtr + xdc_runtime_Main_Module_GateProxy_Module__loggerFxn1__C_offset)))
#else
#define Module__LOGFXN1 xdc_runtime_Main_Module_GateProxy_Module__loggerFxn1__C
#endif

#undef Module__LOGFXN2
#ifdef xdc_runtime_Main_Module_GateProxy_Module__loggerFxn2__CR
#define Module__LOGFXN2 (*((CT__xdc_runtime_Main_Module_GateProxy_Module__loggerFxn2 *)(xdcRomConstPtr + xdc_runtime_Main_Module_GateProxy_Module__loggerFxn2__C_offset)))
#else
#define Module__LOGFXN2 xdc_runtime_Main_Module_GateProxy_Module__loggerFxn2__C
#endif

#undef Module__LOGFXN4
#ifdef xdc_runtime_Main_Module_GateProxy_Module__loggerFxn4__CR
#define Module__LOGFXN4 (*((CT__xdc_runtime_Main_Module_GateProxy_Module__loggerFxn4 *)(xdcRomConstPtr + xdc_runtime_Main_Module_GateProxy_Module__loggerFxn4__C_offset)))
#else
#define Module__LOGFXN4 xdc_runtime_Main_Module_GateProxy_Module__loggerFxn4__C
#endif

#undef Module__LOGFXN8
#ifdef xdc_runtime_Main_Module_GateProxy_Module__loggerFxn8__CR
#define Module__LOGFXN8 (*((CT__xdc_runtime_Main_Module_GateProxy_Module__loggerFxn8 *)(xdcRomConstPtr + xdc_runtime_Main_Module_GateProxy_Module__loggerFxn8__C_offset)))
#else
#define Module__LOGFXN8 xdc_runtime_Main_Module_GateProxy_Module__loggerFxn8__C
#endif

#undef Module__G_OBJ
#ifdef xdc_runtime_Main_Module_GateProxy_Module__gateObj__CR
#define Module__G_OBJ (*((CT__xdc_runtime_Main_Module_GateProxy_Module__gateObj *)(xdcRomConstPtr + xdc_runtime_Main_Module_GateProxy_Module__gateObj__C_offset)))
#else
#define Module__G_OBJ xdc_runtime_Main_Module_GateProxy_Module__gateObj__C
#endif

#undef Module__G_PRMS
#ifdef xdc_runtime_Main_Module_GateProxy_Module__gatePrms__CR
#define Module__G_PRMS (*((CT__xdc_runtime_Main_Module_GateProxy_Module__gatePrms *)(xdcRomConstPtr + xdc_runtime_Main_Module_GateProxy_Module__gatePrms__C_offset)))
#else
#define Module__G_PRMS xdc_runtime_Main_Module_GateProxy_Module__gatePrms__C
#endif

#undef Module__GP_create
#define Module__GP_create xdc_runtime_Main_Module_GateProxy_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_Main_Module_GateProxy_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_Main_Module_GateProxy_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_Main_Module_GateProxy_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_Main_Module_GateProxy_Module_GateProxy_query

xdc_Bool xdc_runtime_Main_Module_GateProxy_Proxy__abstract__S(void)
{
    return 0;
}
xdc_CPtr xdc_runtime_Main_Module_GateProxy_Proxy__delegate__S(void)
{
    return (const void *)&ti_sysbios_gates_GateHwi_Module__FXNS__C;
}



/*
 * ======== xdc.runtime.Memory SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Memory_Module__startupDone__S( void )
{
    return 1;
}



/*
 * ======== xdc.runtime.Memory_HeapProxy SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#ifdef xdc_runtime_Memory_HeapProxy_Module__id__CR
#define Module__MID (*((CT__xdc_runtime_Memory_HeapProxy_Module__id *)(xdcRomConstPtr + xdc_runtime_Memory_HeapProxy_Module__id__C_offset)))
#else
#define Module__MID xdc_runtime_Memory_HeapProxy_Module__id__C
#endif

#undef Module__DGSINCL
#ifdef xdc_runtime_Memory_HeapProxy_Module__diagsIncluded__CR
#define Module__DGSINCL (*((CT__xdc_runtime_Memory_HeapProxy_Module__diagsIncluded *)(xdcRomConstPtr + xdc_runtime_Memory_HeapProxy_Module__diagsIncluded__C_offset)))
#else
#define Module__DGSINCL xdc_runtime_Memory_HeapProxy_Module__diagsIncluded__C
#endif

#undef Module__DGSENAB
#ifdef xdc_runtime_Memory_HeapProxy_Module__diagsEnabled__CR
#define Module__DGSENAB (*((CT__xdc_runtime_Memory_HeapProxy_Module__diagsEnabled *)(xdcRomConstPtr + xdc_runtime_Memory_HeapProxy_Module__diagsEnabled__C_offset)))
#else
#define Module__DGSENAB xdc_runtime_Memory_HeapProxy_Module__diagsEnabled__C
#endif

#undef Module__DGSMASK
#ifdef xdc_runtime_Memory_HeapProxy_Module__diagsMask__CR
#define Module__DGSMASK (*((CT__xdc_runtime_Memory_HeapProxy_Module__diagsMask *)(xdcRomConstPtr + xdc_runtime_Memory_HeapProxy_Module__diagsMask__C_offset)))
#else
#define Module__DGSMASK xdc_runtime_Memory_HeapProxy_Module__diagsMask__C
#endif

#undef Module__LOGDEF
#ifdef xdc_runtime_Memory_HeapProxy_Module__loggerDefined__CR
#define Module__LOGDEF (*((CT__xdc_runtime_Memory_HeapProxy_Module__loggerDefined *)(xdcRomConstPtr + xdc_runtime_Memory_HeapProxy_Module__loggerDefined__C_offset)))
#else
#define Module__LOGDEF xdc_runtime_Memory_HeapProxy_Module__loggerDefined__C
#endif

#undef Module__LOGOBJ
#ifdef xdc_runtime_Memory_HeapProxy_Module__loggerObj__CR
#define Module__LOGOBJ xdc_runtime_Memory_HeapProxy_Module__loggerObj__R
#define Module__LOGOBJ (*((CT__xdc_runtime_Memory_HeapProxy_Module__loggerObj *)(xdcRomConstPtr + xdc_runtime_Memory_HeapProxy_Module__loggerObj__C_offset)))
#else
#define Module__LOGOBJ xdc_runtime_Memory_HeapProxy_Module__loggerObj__C
#endif

#undef Module__LOGFXN0
#ifdef xdc_runtime_Memory_HeapProxy_Module__loggerFxn0__CR
#define Module__LOGFXN0 (*((CT__xdc_runtime_Memory_HeapProxy_Module__loggerFxn0 *)(xdcRomConstPtr + xdc_runtime_Memory_HeapProxy_Module__loggerFxn0__C_offset)))
#else
#define Module__LOGFXN0 xdc_runtime_Memory_HeapProxy_Module__loggerFxn0__C
#endif

#undef Module__LOGFXN1
#ifdef xdc_runtime_Memory_HeapProxy_Module__loggerFxn1__CR
#define Module__LOGFXN1 (*((CT__xdc_runtime_Memory_HeapProxy_Module__loggerFxn1 *)(xdcRomConstPtr + xdc_runtime_Memory_HeapProxy_Module__loggerFxn1__C_offset)))
#else
#define Module__LOGFXN1 xdc_runtime_Memory_HeapProxy_Module__loggerFxn1__C
#endif

#undef Module__LOGFXN2
#ifdef xdc_runtime_Memory_HeapProxy_Module__loggerFxn2__CR
#define Module__LOGFXN2 (*((CT__xdc_runtime_Memory_HeapProxy_Module__loggerFxn2 *)(xdcRomConstPtr + xdc_runtime_Memory_HeapProxy_Module__loggerFxn2__C_offset)))
#else
#define Module__LOGFXN2 xdc_runtime_Memory_HeapProxy_Module__loggerFxn2__C
#endif

#undef Module__LOGFXN4
#ifdef xdc_runtime_Memory_HeapProxy_Module__loggerFxn4__CR
#define Module__LOGFXN4 (*((CT__xdc_runtime_Memory_HeapProxy_Module__loggerFxn4 *)(xdcRomConstPtr + xdc_runtime_Memory_HeapProxy_Module__loggerFxn4__C_offset)))
#else
#define Module__LOGFXN4 xdc_runtime_Memory_HeapProxy_Module__loggerFxn4__C
#endif

#undef Module__LOGFXN8
#ifdef xdc_runtime_Memory_HeapProxy_Module__loggerFxn8__CR
#define Module__LOGFXN8 (*((CT__xdc_runtime_Memory_HeapProxy_Module__loggerFxn8 *)(xdcRomConstPtr + xdc_runtime_Memory_HeapProxy_Module__loggerFxn8__C_offset)))
#else
#define Module__LOGFXN8 xdc_runtime_Memory_HeapProxy_Module__loggerFxn8__C
#endif

#undef Module__G_OBJ
#ifdef xdc_runtime_Memory_HeapProxy_Module__gateObj__CR
#define Module__G_OBJ (*((CT__xdc_runtime_Memory_HeapProxy_Module__gateObj *)(xdcRomConstPtr + xdc_runtime_Memory_HeapProxy_Module__gateObj__C_offset)))
#else
#define Module__G_OBJ xdc_runtime_Memory_HeapProxy_Module__gateObj__C
#endif

#undef Module__G_PRMS
#ifdef xdc_runtime_Memory_HeapProxy_Module__gatePrms__CR
#define Module__G_PRMS (*((CT__xdc_runtime_Memory_HeapProxy_Module__gatePrms *)(xdcRomConstPtr + xdc_runtime_Memory_HeapProxy_Module__gatePrms__C_offset)))
#else
#define Module__G_PRMS xdc_runtime_Memory_HeapProxy_Module__gatePrms__C
#endif

#undef Module__GP_create
#define Module__GP_create xdc_runtime_Memory_HeapProxy_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_Memory_HeapProxy_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_Memory_HeapProxy_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_Memory_HeapProxy_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_Memory_HeapProxy_Module_GateProxy_query

xdc_Bool xdc_runtime_Memory_HeapProxy_Proxy__abstract__S(void)
{
    return 1;
}
xdc_CPtr xdc_runtime_Memory_HeapProxy_Proxy__delegate__S(void)
{
    return (const void *)&ti_sysbios_heaps_HeapMem_Module__FXNS__C;
}



/*
 * ======== xdc.runtime.Registry SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Registry_Module__startupDone__S( void )
{
    return 1;
}



/*
 * ======== xdc.runtime.Startup SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Startup_Module__startupDone__S( void )
{
    return 1;
}



/*
 * ======== xdc.runtime.SysStd SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_SysStd_Module__startupDone__S( void )
{
    return 1;
}



/*
 * ======== xdc.runtime.System SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_System_Module__startupDone__S( void )
{
    return xdc_runtime_System_Module__startupDone__F();
}



/*
 * ======== xdc.runtime.System_Module_GateProxy SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#ifdef xdc_runtime_System_Module_GateProxy_Module__id__CR
#define Module__MID (*((CT__xdc_runtime_System_Module_GateProxy_Module__id *)(xdcRomConstPtr + xdc_runtime_System_Module_GateProxy_Module__id__C_offset)))
#else
#define Module__MID xdc_runtime_System_Module_GateProxy_Module__id__C
#endif

#undef Module__DGSINCL
#ifdef xdc_runtime_System_Module_GateProxy_Module__diagsIncluded__CR
#define Module__DGSINCL (*((CT__xdc_runtime_System_Module_GateProxy_Module__diagsIncluded *)(xdcRomConstPtr + xdc_runtime_System_Module_GateProxy_Module__diagsIncluded__C_offset)))
#else
#define Module__DGSINCL xdc_runtime_System_Module_GateProxy_Module__diagsIncluded__C
#endif

#undef Module__DGSENAB
#ifdef xdc_runtime_System_Module_GateProxy_Module__diagsEnabled__CR
#define Module__DGSENAB (*((CT__xdc_runtime_System_Module_GateProxy_Module__diagsEnabled *)(xdcRomConstPtr + xdc_runtime_System_Module_GateProxy_Module__diagsEnabled__C_offset)))
#else
#define Module__DGSENAB xdc_runtime_System_Module_GateProxy_Module__diagsEnabled__C
#endif

#undef Module__DGSMASK
#ifdef xdc_runtime_System_Module_GateProxy_Module__diagsMask__CR
#define Module__DGSMASK (*((CT__xdc_runtime_System_Module_GateProxy_Module__diagsMask *)(xdcRomConstPtr + xdc_runtime_System_Module_GateProxy_Module__diagsMask__C_offset)))
#else
#define Module__DGSMASK xdc_runtime_System_Module_GateProxy_Module__diagsMask__C
#endif

#undef Module__LOGDEF
#ifdef xdc_runtime_System_Module_GateProxy_Module__loggerDefined__CR
#define Module__LOGDEF (*((CT__xdc_runtime_System_Module_GateProxy_Module__loggerDefined *)(xdcRomConstPtr + xdc_runtime_System_Module_GateProxy_Module__loggerDefined__C_offset)))
#else
#define Module__LOGDEF xdc_runtime_System_Module_GateProxy_Module__loggerDefined__C
#endif

#undef Module__LOGOBJ
#ifdef xdc_runtime_System_Module_GateProxy_Module__loggerObj__CR
#define Module__LOGOBJ xdc_runtime_System_Module_GateProxy_Module__loggerObj__R
#define Module__LOGOBJ (*((CT__xdc_runtime_System_Module_GateProxy_Module__loggerObj *)(xdcRomConstPtr + xdc_runtime_System_Module_GateProxy_Module__loggerObj__C_offset)))
#else
#define Module__LOGOBJ xdc_runtime_System_Module_GateProxy_Module__loggerObj__C
#endif

#undef Module__LOGFXN0
#ifdef xdc_runtime_System_Module_GateProxy_Module__loggerFxn0__CR
#define Module__LOGFXN0 (*((CT__xdc_runtime_System_Module_GateProxy_Module__loggerFxn0 *)(xdcRomConstPtr + xdc_runtime_System_Module_GateProxy_Module__loggerFxn0__C_offset)))
#else
#define Module__LOGFXN0 xdc_runtime_System_Module_GateProxy_Module__loggerFxn0__C
#endif

#undef Module__LOGFXN1
#ifdef xdc_runtime_System_Module_GateProxy_Module__loggerFxn1__CR
#define Module__LOGFXN1 (*((CT__xdc_runtime_System_Module_GateProxy_Module__loggerFxn1 *)(xdcRomConstPtr + xdc_runtime_System_Module_GateProxy_Module__loggerFxn1__C_offset)))
#else
#define Module__LOGFXN1 xdc_runtime_System_Module_GateProxy_Module__loggerFxn1__C
#endif

#undef Module__LOGFXN2
#ifdef xdc_runtime_System_Module_GateProxy_Module__loggerFxn2__CR
#define Module__LOGFXN2 (*((CT__xdc_runtime_System_Module_GateProxy_Module__loggerFxn2 *)(xdcRomConstPtr + xdc_runtime_System_Module_GateProxy_Module__loggerFxn2__C_offset)))
#else
#define Module__LOGFXN2 xdc_runtime_System_Module_GateProxy_Module__loggerFxn2__C
#endif

#undef Module__LOGFXN4
#ifdef xdc_runtime_System_Module_GateProxy_Module__loggerFxn4__CR
#define Module__LOGFXN4 (*((CT__xdc_runtime_System_Module_GateProxy_Module__loggerFxn4 *)(xdcRomConstPtr + xdc_runtime_System_Module_GateProxy_Module__loggerFxn4__C_offset)))
#else
#define Module__LOGFXN4 xdc_runtime_System_Module_GateProxy_Module__loggerFxn4__C
#endif

#undef Module__LOGFXN8
#ifdef xdc_runtime_System_Module_GateProxy_Module__loggerFxn8__CR
#define Module__LOGFXN8 (*((CT__xdc_runtime_System_Module_GateProxy_Module__loggerFxn8 *)(xdcRomConstPtr + xdc_runtime_System_Module_GateProxy_Module__loggerFxn8__C_offset)))
#else
#define Module__LOGFXN8 xdc_runtime_System_Module_GateProxy_Module__loggerFxn8__C
#endif

#undef Module__G_OBJ
#ifdef xdc_runtime_System_Module_GateProxy_Module__gateObj__CR
#define Module__G_OBJ (*((CT__xdc_runtime_System_Module_GateProxy_Module__gateObj *)(xdcRomConstPtr + xdc_runtime_System_Module_GateProxy_Module__gateObj__C_offset)))
#else
#define Module__G_OBJ xdc_runtime_System_Module_GateProxy_Module__gateObj__C
#endif

#undef Module__G_PRMS
#ifdef xdc_runtime_System_Module_GateProxy_Module__gatePrms__CR
#define Module__G_PRMS (*((CT__xdc_runtime_System_Module_GateProxy_Module__gatePrms *)(xdcRomConstPtr + xdc_runtime_System_Module_GateProxy_Module__gatePrms__C_offset)))
#else
#define Module__G_PRMS xdc_runtime_System_Module_GateProxy_Module__gatePrms__C
#endif

#undef Module__GP_create
#define Module__GP_create xdc_runtime_System_Module_GateProxy_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_System_Module_GateProxy_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_System_Module_GateProxy_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_System_Module_GateProxy_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_System_Module_GateProxy_Module_GateProxy_query

xdc_Bool xdc_runtime_System_Module_GateProxy_Proxy__abstract__S(void)
{
    return 0;
}
xdc_CPtr xdc_runtime_System_Module_GateProxy_Proxy__delegate__S(void)
{
    return (const void *)&ti_sysbios_gates_GateHwi_Module__FXNS__C;
}



/*
 * ======== xdc.runtime.System_SupportProxy SYSTEM FUNCTIONS ========
 */


xdc_Bool xdc_runtime_System_SupportProxy_Proxy__abstract__S(void)
{
    return 0;
}
xdc_CPtr xdc_runtime_System_SupportProxy_Proxy__delegate__S(void)
{
    return (const void *)&xdc_runtime_SysStd_Module__FXNS__C;
}


/*
 * ======== xdc.runtime.Text SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Text_Module__startupDone__S( void )
{
    return 1;
}



/*
 * ======== INITIALIZATION ENTRY POINT ========
 */

extern int __xdc__init(void);
#ifdef __GNUC__
#if defined(__llvm__) && defined(__ti__)
    __attribute__ ((used))
#else
#ifndef __TI_COMPILER_VERSION__
    __attribute__ ((externally_visible))
#endif
#endif
#endif
__FAR__ int (* volatile __xdc__init__addr)(void) = &__xdc__init;


/*
 * ======== PROGRAM GLOBALS ========
 */

#ifdef __GNUC__
#if defined(__llvm__) && defined(__ti__)
    __attribute__ ((used))
#else
#ifndef __TI_COMPILER_VERSION__
    __attribute__ ((externally_visible))
#endif
#endif
#endif
const ti_sysbios_heaps_HeapMem_Handle heap0 = (ti_sysbios_heaps_HeapMem_Handle)((ti_sysbios_heaps_HeapMem_Handle)&ti_sysbios_heaps_HeapMem_Object__table__V[0]);

