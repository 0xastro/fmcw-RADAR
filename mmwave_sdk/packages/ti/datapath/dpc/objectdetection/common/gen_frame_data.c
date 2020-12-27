/**
 *   @file  gen_frame_data.c
 *
 *   @brief
 *      Radar test input generator.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2018 Texas Instruments, Inc.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include <ti/utils/mathutils/mathutils.h>

#include "gen_frame_data.h"

uint16_t RandSimple(void);
void GaussianNoise(float *NoiseVariance, float *pxn, float *pyn);

/*
 *  Generates Gausian distributed random number, with unit variance
 */
void GaussianNoise(
        float *NoiseVariance,
        float *pxn,
        float *pyn)
{

    float y, dx2, pi2, r, t, temp;
    static float pi = (float)3.1415926535897;
    uint16_t itemp1;
    float gauss_var;

    pi2 = 2.f * pi;
    gauss_var = * NoiseVariance/2; /* half per dimension */

    itemp1 = RandSimple();
    while(itemp1 == 0)
        itemp1 = RandSimple();
    r = (float)(itemp1)/32767.0f;

    temp = 2.f*gauss_var*(float)log(1./r);
    y = (float)sqrt(temp);

    itemp1 = RandSimple();
    t = (float)(itemp1)/32767.0f;

    dx2 = pi2*t;
    *pxn = y*(float)cos(dx2);
    *pyn = y*(float)sin(dx2);
}

/*
 *  Generates a random number 0 - 0x7fff
 */
uint16_t RandSimple(void)
{
    static int64_t next = 1;
    uint16_t ret2;

    next = (next * 16807) % 2147483647;
    ret2 = (uint16_t)(next & 0x7fff);

    return (ret2);
}


/*!
 * \brief
 * Function Name       :    genChirpData
 *
 * \par
 * <b>Description</b>  : Generates one chirp of reference data into adc buffer. Function is called per chirp.
 *
 * @param[in]      obj Pointer to sub-frame data object structure
 *
 * @param[in]      frm Pointer to frame reference data structure
 *
 * @param[in]      outputBuffer   Pointer to buffer with generated samples. The samples are stored in cmplx16ImRe_t format.
 *
 * @return         void
 *
 */
void genChirpData (ObjectDetection_Test_SubFr_DataPathObj *obj,
                   genFrameStruc_t *frm,
                   uint8_t *outputBuffer)
{
    float noiseRe, noiseIm;
    float xRe, xIm;
    float var = 1;
    uint32_t antIdx;
    uint32_t targIdx;
    uint32_t rangeIdx;
    targetObjStruc_t * pT;
    uint32_t idx;
    bool processingElevationAntenna = false;
    cmplx16ImRe_t * adcBuff = (cmplx16ImRe_t *) outputBuffer;

    if(adcBuff == NULL)
    {
        printf("\nError getChirpData. adcBuff not defined.\n");
        return;
    }


    if(obj->numVirtualAntElev > 0)
    {
        /*Elevation antenna is the last TX antenna to be chirped*/
        if(frm->chirpModNumTxAntCntr == (frm->numTxAntennas-1))
        {
            /* Now will be processing the elevation antenna*/
            processingElevationAntenna = true;
        } 
    }


    memset ((uint8_t *) adcBuff, 0, obj->numRxAntennas * obj->numAdcSamples * sizeof(cmplx16ReIm_t));

    for (antIdx = 0; antIdx < obj->numRxAntennas; antIdx++)
    {
        for (targIdx = 0; targIdx < frm->numTargets; targIdx++)
        {
            pT = &frm->targetObj[targIdx];
            pT->rngPhy = 0;
            if(obj->isBpmEnabled)
            {
                /*For BPM the azimuth angle must reset for every chirp*/
                if(antIdx == 0)
                {
                    pT->azimPhy = 0;
                }
            }
        }

        for (rangeIdx = 0; rangeIdx < obj->numAdcSamples;  rangeIdx++)
        {
            xRe = 0;
            xIm = 0;
            for (targIdx = 0; targIdx < frm->numTargets; targIdx++)
            {
                pT = &frm->targetObj[targIdx];
                GaussianNoise(&var, &noiseRe, &noiseIm);
                

                if(processingElevationAntenna)
                {
                    /*If this is the elevation antenna, need to set the azimuth to the correct value*/
                    pT->azimPhy = pT->azimPhyElevAntenna[antIdx] - pT->elevationPhy;
                }


                xRe  += pT->amplitude * cos(pT->rngPhy + pT->doppPhy + pT->azimPhy) + frm->noiseScale * noiseRe;
                xIm  += pT->amplitude * sin(pT->rngPhy + pT->doppPhy + pT->azimPhy) + frm->noiseScale * noiseIm;
                
                if(obj->isBpmEnabled)
                {
                    /* Adding contribution from second TX antenna */
                    GaussianNoise(&var, &noiseRe, &noiseIm);
                    if(frm->chirpIndex % 2 == 0)
                    {
                        xRe  += pT->amplitude * cos(pT->rngPhy + pT->doppPhy + (pT->azimPhy + (obj->numRxAntennas)*pT->azimPhyIncr)) + frm->noiseScale * noiseRe;
                        xIm  += pT->amplitude * sin(pT->rngPhy + pT->doppPhy + (pT->azimPhy + (obj->numRxAntennas)*pT->azimPhyIncr)) + frm->noiseScale * noiseIm;
                    }
                    else
                    {
                        xRe  -= pT->amplitude * cos(pT->rngPhy + pT->doppPhy + (+pT->azimPhy + (obj->numRxAntennas)*pT->azimPhyIncr)) + frm->noiseScale * noiseRe;
                        xIm  -= pT->amplitude * sin(pT->rngPhy + pT->doppPhy + (+pT->azimPhy + (obj->numRxAntennas)*pT->azimPhyIncr)) + frm->noiseScale * noiseIm;
                    }    
                }
                
                pT->rngPhy += pT->rngPhyIncr;
            }
            if (obj->isInterleavedSampInAdcBuf)
            {
                /*samples are interleaved*/
                idx = rangeIdx*obj->numRxAntennas + antIdx;
            }
            else
            {
                /*samples are not interleaved (all samples for antenna[i] are sequential)*/
                idx = antIdx*obj->rxChanOffsetInSamples + rangeIdx;
            }
            adcBuff[idx].real = (int16_t) MATHUTILS_ROUND_FLOAT(xRe);
            adcBuff[idx].imag = (int16_t) MATHUTILS_ROUND_FLOAT(xIm);
            
        }

        for (targIdx = 0; targIdx < frm->numTargets; targIdx++)
        {
            pT = &frm->targetObj[targIdx];

            /*need to record azimuth for elevation antenna?*/
            if(obj->numVirtualAntElev > 0)
            {
                /*Elevation antenna is present. Now check if need to record azimuth for this particular
                  non-elevation antenna to be used later on.
                  chirpModNumTxAntCntr=0 is for TX 1 - non-elevation
                  chirpModNumTxAntCntr=1 is for TX 3 - non-elevation
                  chirpModNumTxAntCntr=2 is for TX 2 - elevation*/
                if(frm->chirpModNumTxAntCntr == 0)
                {
                    if(antIdx == 2)
                    {
                        pT->azimPhyElevAntenna[0] = pT->azimPhy;
                    }
                    else if(antIdx == 3)
                    {
                        pT->azimPhyElevAntenna[1] = pT->azimPhy;
                    }    
                }
                else if(frm->chirpModNumTxAntCntr == 1)
                {
                    if(antIdx == 0)
                    {
                        pT->azimPhyElevAntenna[2] = pT->azimPhy;
                    }    
                    else if(antIdx == 1)
                    {
                        pT->azimPhyElevAntenna[3] = pT->azimPhy;
                    }    
                }
            }

            /*increment the azimPhy if this is not the elevation antenna*/
            if(processingElevationAntenna == false)
            {
                pT->azimPhy += pT->azimPhyIncr;
            }    
        }
    }

    for (targIdx = 0; targIdx < frm->numTargets; targIdx++)
    {
        pT = &frm->targetObj[targIdx];
        pT->doppPhy += pT->doppPhyIncr;
    }

    frm->chirpIndex++;
    frm->chirpModNumTxAntCntr++;

    if (frm->chirpModNumTxAntCntr == frm->numTxAntennas)
    {
        frm->chirpModNumTxAntCntr = 0;
        for (targIdx = 0; targIdx < frm->numTargets; targIdx++)
        {
            pT = &frm->targetObj[targIdx];
            pT->azimPhy = 0;
        }
    }
}

/*!
 * \brief
 * Function Name       :    initFrameData
 *
 * \par
 * <b>Description</b>  : Initializes radar ADC reference data.
 *
 * @param[in]      dataPathObj Pointer to sub-frame data
 *
 * @param[out]      genFrmDataObj Pointer to frame reference data
 *
 * @param[in]      sceneConfig   Pointer to scene configuration
 *
 * @return         void
 *
 */
void initFrameData (ObjectDetection_Test_SubFr_DataPathObj *dataPathObj,
                    genFrameStruc_t *genFrmDataObj,
                    sceneConfig_t *sceneConfig)
{
    uint32_t i;
    uint32_t numRangeBins;

    genFrmDataObj->numTargets = sceneConfig->numTargets;
    genFrmDataObj->snrdB = (float)sceneConfig->snrdB; //relative to the first target

    for (i = 0; i < sceneConfig->numTargets; i++)
    {
        genFrmDataObj->targetObj[i].amplitude  = (float)sceneConfig->sceneObj[i].amplitude;
        genFrmDataObj->targetObj[i].azimPhyBin =  sceneConfig->sceneObj[i].azimPhyBin;
        genFrmDataObj->targetObj[i].rngPhyBin  = sceneConfig->sceneObj[i].rngPhyBin;
        genFrmDataObj->targetObj[i].doppPhyBin =  sceneConfig->sceneObj[i].doppPhyBin;
        genFrmDataObj->targetObj[i].elevationPhy = PI_ * sin(sceneConfig->sceneObj[i].elevationDeg * PI_/180);
    }

    genFrmDataObj->noiseScale =  genFrmDataObj->targetObj[0].amplitude / 
        pow(20., genFrmDataObj->snrdB/20.) / sqrt(2.);

    genFrmDataObj->numTxAntennas = dataPathObj->numTxAntennas;
    genFrmDataObj->chirpIndex = 0;
    genFrmDataObj->chirpModNumTxAntCntr = 0;

	//Because of workaround for range DPU limitation (number of virtual antennas = 12 and range FFT size = 1024):
    if (dataPathObj->numRangeBins == 1022)
    {
        numRangeBins = 1024;
    }
    else
    {
        numRangeBins = dataPathObj->numRangeBins;
    }

    for (i=0; i < genFrmDataObj->numTargets; i++)
    {
        genFrmDataObj->targetObj[i].azimPhyIncr =  2 * PI_ * 
            genFrmDataObj->targetObj[i].azimPhyBin / (float) dataPathObj->numAngleBins;
        genFrmDataObj->targetObj[i].rngPhyIncr = 2 * PI_ * 
            genFrmDataObj->targetObj[i].rngPhyBin / (float) numRangeBins;
        genFrmDataObj->targetObj[i].doppPhyIncr =  2 * PI_ * 
            genFrmDataObj->targetObj[i].doppPhyBin  /((float) dataPathObj->numTxAntennas * (float) dataPathObj->numDopplerBins);
        genFrmDataObj->targetObj[i].azimPhy =  0;
        genFrmDataObj->targetObj[i].rngPhy =  0;
        genFrmDataObj->targetObj[i].doppPhy =  0;
    }
}

