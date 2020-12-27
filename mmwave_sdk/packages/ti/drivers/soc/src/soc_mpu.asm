;-------------------------------------------------------------------------------
; socSOC_MPU.asm
;
; Copyright (C) 2009-2014 Texas Instruments Incorporated - TI web adress www.ti.com
;
;
;  Redistribution and use in source and binary forms, with or without
;  modification, are permitted provided that the following conditions
;  are met:
;
;    Redistributions of source code must retain the above copyright
;    notice, this list of conditions and the following disclaimer.
;
;    Redistributions in binary form must reproduce the above copyright
;    notice, this list of conditions and the following disclaimer in the
;    documentation and/or other materials provided with the
;    distribution.
;
;    Neither the name of Texas Instruments Incorporated nor the names of
;    its contributors may be used to endorse or promote products derived
;    from this software without specific prior written permission.
;
;  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
;  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
;  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
;  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
;  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
;  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
;  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
;  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
;  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
;  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
;  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;
;
;

    .text
    .arm

    .cdecls C,LIST,"ti/drivers/soc/include/soc_mpu.h"

;-------------------------------------------------------------------------------
; Enable Mpu
; SourceId :
; DesignId :
; Requirements :

    .thumb
    .thumbfunc SOC_MPUEnable
    .def     SOC_MPUEnable
    .asmfunc

SOC_MPUEnable

        stmfd sp!, {r0}
        mrc   p15, #0, r0, c1, c0, #0
        orr   r0,  r0, #1
        dsb
        mcr   p15, #0, r0, c1, c0, #0
        isb
        ldmfd sp!, {r0}
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Disable Mpu
; SourceId :
; DesignId :
; Requirements :

    .thumb
    .thumbfunc SOC_MPUDisable
    .def     SOC_MPUDisable
    .asmfunc

SOC_MPUDisable

        stmfd sp!, {r0}
        mrc   p15, #0, r0, c1, c0, #0
        bic   r0,  r0, #1
        dsb
        mcr   p15, #0, r0, c1, c0, #0
        isb
        ldmfd sp!, {r0}
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Enable Mpu background region
; SourceId :
; DesignId :
; Requirements :

    .thumb
    .thumbfunc SOC_MPUEnableBackgroundRegion
    .def     SOC_MPUEnableBackgroundRegion
    .asmfunc

SOC_MPUEnableBackgroundRegion

        stmfd sp!, {r0}
        mrc   p15, #0, r0,      c1, c0, #0
        orr   r0,  r0, #0x20000
        mcr   p15, #0, r0,      c1, c0, #0
        ldmfd sp!, {r0}
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Disable Mpu background region
; SourceId :
; DesignId :
; Requirements :

    .thumb
    .thumbfunc SOC_MPUDisableBackgroundRegion
    .def     SOC_MPUDisableBackgroundRegion
    .asmfunc

SOC_MPUDisableBackgroundRegion

        stmfd sp!, {r0}
        mrc   p15, #0, r0,      c1, c0, #0
        bic   r0,  r0, #0x20000
        mcr   p15, #0, r0,      c1, c0, #0
        ldmfd sp!, {r0}
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Returns number of implemented Mpu regions
; SourceId :
; DesignId :
; Requirements :

    .thumb
    .thumbfunc SOC_MPUGetNumberOfRegions
    .def     SOC_MPUGetNumberOfRegions
    .asmfunc

SOC_MPUGetNumberOfRegions

        mrc   p15, #0, r0,      c0, c0, #4
        uxtb  r0,  r0, ROR #8
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Returns the type of the implemented mpu regions
; SourceId :
; DesignId :
; Requirements :

    .thumb
    .thumbfunc SOC_MPUAreRegionsSeparate
    .def     SOC_MPUAreRegionsSeparate
    .asmfunc

SOC_MPUAreRegionsSeparate

        mrc   p15, #0, r0,      c0, c0, #4
        uxtb  r0,  r0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Set mpu region number
; SourceId :
; DesignId :
; Requirements :

    .thumb
    .thumbfunc SOC_MPUSetRegion
    .def     SOC_MPUSetRegion
    .asmfunc

SOC_MPUSetRegion

        mcr   p15, #0, r0, c6, c2, #0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Get mpu region number
; SourceId :
; DesignId :
; Requirements :

    .thumb
    .thumbfunc SOC_MPUGetRegion
    .def     SOC_MPUGetRegion
    .asmfunc

SOC_MPUGetRegion

        mrc   p15, #0, r0, c6, c2, #0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Set base address
; SourceId :
; DesignId :
; Requirements :

    .thumb
    .thumbfunc SOC_MPUSetRegionBaseAddress
    .def     SOC_MPUSetRegionBaseAddress
    .asmfunc

SOC_MPUSetRegionBaseAddress

        mcr   p15, #0, r0, c6, c1, #0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Get base address
; SourceId :
; DesignId :
; Requirements :

    .thumb
    .thumbfunc SOC_MPUGetRegionBaseAddress
    .def     SOC_MPUGetRegionBaseAddress
    .asmfunc

SOC_MPUGetRegionBaseAddress

        mrc   p15, #0, r0, c6, c1, #0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Set type and permission
; SourceId :
; DesignId :
; Requirements :

    .thumb
    .thumbfunc SOC_MPUSetRegionTypeAndPermission
    .def     SOC_MPUSetRegionTypeAndPermission
    .asmfunc

SOC_MPUSetRegionTypeAndPermission

        orr   r0,  r0, r1
        mcr   p15, #0, r0, c6, c1, #4
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Get type
; SourceId :
; DesignId :
; Requirements :

    .thumb
    .thumbfunc SOC_MPUGetRegionType
    .def     SOC_MPUGetRegionType
    .asmfunc

SOC_MPUGetRegionType

        mrc   p15, #0, r0,     c6, c1, #4
        bic   r0,  r0, #0xFF00
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Get permission
; SourceId :
; DesignId :
; Requirements :

    .thumb
    .thumbfunc SOC_MPUGetRegionPermission
    .def     SOC_MPUGetRegionPermission
    .asmfunc

SOC_MPUGetRegionPermission

        mrc   p15, #0, r0,   c6, c1, #4
        bic   r0,  r0, #0xFF
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Set region size register value
; SourceId :
; DesignId :
; Requirements :

    .thumb
    .thumbfunc SOC_MPUSetRegionSizeRegister
    .def     SOC_MPUSetRegionSizeRegister
    .asmfunc

SOC_MPUSetRegionSizeRegister


        mcr   p15, #0, r0, c6, c1, #2
        bx    lr

    .endasmfunc



;-------------------------------------------------------------------------------

