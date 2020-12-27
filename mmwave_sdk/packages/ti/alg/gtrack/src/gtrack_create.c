/**
 *   @file  gtrack_create.c
 *
 *   @brief
 *      This function creates an instance of the GTRACK Algorithm
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2017 Texas Instruments, Inc.
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
#include <string.h>
#include <math.h>
#include <float.h>

#include <ti/alg/gtrack/gtrack.h>
#include <ti/alg/gtrack/include/gtrack_int.h>

const GTRACK_sceneryParams defaultSceneryParams = {0,{{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f}}, 0,{{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f}}}; /* no boundaries, no static boxes */
const GTRACK_gatingParams defaultGatingParams = {2.f, {3.f, 2.f, 2.f, 0.f}};
const GTRACK_stateParams defaultStateParams = {3U, 3U, 5U, 5U, 5U}; /* 3 frames to activate, 100 frames to forget */
const GTRACK_allocationParams defaultAllocationParams = {100.f, 100.f, 0.5f, 5U, 1.f, 2.f}; /* At least 100 SNR, 100 SNR when obscured, 0.5 m/s, 5 points: up to 1m in distance, up to 2m/c in velocity */
const GTRACK_unrollingParams defaultUnrollingParams = {0.5f, 0.1f};
const GTRACK_varParams defaultVariationParams = {1.f/3.46f, 1.f/3.46f, 2.f}; /* Based on standard deviation of uniformly distributed variable in range [a b]: 1/sqrt(12)*(b-a). For object of 1m height, 1m width, 1m/s doppler spread */

const float zero3x3[9] = {
	0.f, 0.f, 0.f,
	0.f, 0.f, 0.f,
	0.f, 0.f, 0.f
};

const float pinit6x6[36] = {
	0.f,  0.f,  0.f,  0.f,  0.f,  0.f,
	0.f,  0.f,  0.f,  0.f,  0.f,  0.f,
	0.f,  0.f,  0.5f, 0.f,  0.f,  0.f,
	0.f,  0.f,  0.f,  0.5f, 0.f,  0.f,
	0.f,  0.f,  0.f,  0.f,  1.f,  0.f,
	0.f,  0.f,  0.f,  0.f,  0.f,  1.f
};

/**
*  @b Description
*  @n
*		Algorithm level create funtion.
*       Application calls this function to create an instance of GTRACK algorithm with desired configuration parameters. 
*		Function returns a handle, which shall be used to execute a single frame step function, or a delete function
*
*  @param[in]  config
*		This is a pointer to the configuration structure. The structure contains all parameters that are exposed by GTRACK alrorithm.
*		The configuration does not need to persist.
*		Advanced configuration structure can be set to NULL to use the default one.
*		Any field within Advanced configuration can also be set to NULL to use the default values for the field.
*  @param[out] errCode
*      Error code populated on error, see \ref GTRACK_ERROR_CODE
*
*  \ingroup GTRACK_ALG_EXTERNAL_FUNCTION
*
*  @retval
*      Handle to GTRACK module
*/

void *gtrack_create(GTRACK_moduleConfig *config, int32_t *errCode)
{
	GtrackModuleInstance *inst = NULL;
	uint8_t uid;
	float dt, dt2, dt3, dt4;
    float varX, varY, varZ;

	*errCode = GTRACK_EOK;

	if(config->maxNumPoints > GTRACK_NUM_POINTS_MAX) {
		*errCode = GTRACK_EINVAL;
		goto exit;
	}

	if(config->maxNumTracks > GTRACK_NUM_TRACKS_MAX) {
		*errCode = GTRACK_EINVAL;
		goto exit;
	}

	inst = (GtrackModuleInstance *)gtrack_alloc(1, sizeof(GtrackModuleInstance));
	if(inst == NULL) {
		*errCode = GTRACK_ENOMEM;
		goto exit;
	}

	memset(inst, 0, sizeof(GtrackModuleInstance));

	inst->maxNumPoints = config->maxNumPoints;
	inst->maxNumTracks = config->maxNumTracks;

	inst->heartBeat = 0U;

    /* default parameters */
	inst->params.gatingParams = defaultGatingParams;
    inst->params.stateParams = defaultStateParams;
    inst->params.unrollingParams = defaultUnrollingParams;
    inst->params.allocationParams = defaultAllocationParams;
	inst->params.variationParams = defaultVariationParams;
    inst->params.sceneryParams = defaultSceneryParams;

	if(config->advParams != NULL) {
		/* user overwrites default parameters */
	    if(config->advParams->gatingParams)
			memcpy(&inst->params.gatingParams, config->advParams->gatingParams, sizeof(GTRACK_gatingParams));
		if(config->advParams->stateParams)
			memcpy(&inst->params.stateParams, config->advParams->stateParams, sizeof(GTRACK_stateParams));
		if(config->advParams->unrollingParams)
			memcpy(&inst->params.unrollingParams, config->advParams->unrollingParams, sizeof(GTRACK_unrollingParams));
		if(config->advParams->allocationParams)
			memcpy(&inst->params.allocationParams, config->advParams->allocationParams, sizeof(GTRACK_allocationParams));
		if(config->advParams->variationParams)
			memcpy(&inst->params.variationParams, config->advParams->variationParams, sizeof(GTRACK_varParams));
        if(config->advParams->sceneryParams)
            memcpy(&inst->params.sceneryParams, config->advParams->sceneryParams, sizeof(GTRACK_sceneryParams));
	}

	inst->params.deltaT = config->deltaT;
    dt = config->deltaT;
    dt2 = powf(dt,2);
    dt3 = powf(dt,3);
    dt4 = powf(dt,4);

    /* initialize process variance to 1/2 of maximum target acceleration */
    memcpy(inst->params.maxAcceleration, config->maxAcceleration, sizeof(config->maxAcceleration));

    varX = powf(0.5f*config->maxAcceleration[0],2);
    varY = powf(0.5f*config->maxAcceleration[1],2);
    varZ = powf(0.5f*config->maxAcceleration[2],2);

	/* configured parameters */
    switch(config->stateVectorType) {
        case GTRACK_STATE_VECTORS_2DA:
            {
        		float F6[36] = {				
		        1.f, 0.f, dt,  0.f,	dt2/2, 0.f, 
		        0.f, 1.f, 0.f, dt,	0.f,   dt2/2,
		        0.f, 0.f, 1.f, 0.f,	dt,    0.f,
		        0.f, 0.f, 0.f, 1.f,	0.f,   dt,
		        0.f, 0.f, 0.f, 0.f,	1.f,   0.f,
		        0.f, 0.f, 0.f, 0.f,	0.f,   1.f};

		        float Q6[36] = {
		        dt4/4*varX,	0.f,        dt3/2*varX, 0.f,        dt2/2*varX,	0.f,
		        0.f,        dt4/4*varY,	0.f,        dt3/2*varY,	0.f,        dt2/2*varY,
		        dt3/2*varX,	0.f,        dt2*varX,	0.f,        dt*varX,    0.f,
		        0.f,        dt3/2*varY,	0.f,        dt2*varY,	0.f,        dt*varY,
		        dt2/2*varX,	0.f,        dt*varX,    0.f,        1.f*varX,   0.f,
		        0.f,        dt2/2*varY, 0.f,        dt*varY,    0.f,        1.f*varY};

            	inst->params.F = (float *)gtrack_alloc(1, sizeof(F6));
            	inst->params.Q = (float *)gtrack_alloc(1, sizeof(Q6));

                memcpy(inst->params.F, F6, sizeof(F6));
                memcpy(inst->params.Q, Q6, sizeof(Q6));
            }
            break;

        case GTRACK_STATE_VECTORS_3DA:
            {
           		float F9[81] = {				
		        1.f, 0.f, 0.f, dt,  0.f, 0.f, dt2/2, 0.f,   0.f,
                0.f, 1.f, 0.f, 0.f, dt,  0.f, 0.f,   dt2/2, 0.f,
                0.f, 0.f, 1.f, 0.f, 0.f, dt,  0.f,   0.f,   dt2/2,
		        0.f, 0.f, 0.f, 1.f, 0.f, 0.f, dt,    0.f,   0.f,
		        0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,   dt,    0.f,
		        0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f,   0.f,   dt,
                0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f,   0.f,   0.f,
                0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,   1.f,   0.f,
                0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,   0.f,   1.f};

		        float Q9[81] = {
		        dt4/4*varX,	0.f,        0.f,        dt3/2*varX, 0.f,        0.f,        dt2/2*varX, 0.f,        0.f,
		        0.f,        dt4/4*varY,	0.f,        0.f,        dt3/2*varY,	0.f,        0.f,        dt2/2*varY, 0.f,
		        0.f,        0.f,        dt4/4*varZ,	0.f,        0.f,        dt3/2*varZ,	0.f,        0.f,        dt2/2*varZ,
		        dt3/2*varX,	0.f,        0.f,        dt2*varX,	0.f,        0.f,        dt*varX,    0.f,        0.f,
		        0.f,        dt3/2*varY,	0.f,        0.f,        dt2*varY,	0.f,        0.f,        dt*varY,    0.f,
		        0.f,        0.f,        dt3/2*varZ,	0.f,        0.f,        dt2*varZ,	0.f,        0.f,        dt*varZ,
		        dt2/2*varX,	0.f,        0.f,        dt*varX,    0.f,        0.f,        1.f*varX,   0.f,        0.f,
		        0.f,        dt2/2*varY,	0.f,        0.f,        dt*varY,    0.f,        0.f,        1.f*varY,   0.f,
                0.f,        0.f,        dt2/2*varZ,	0.f,        0.f,        dt*varZ,    0.f,        0.f,        1.f*varZ};

                inst->params.F = (float *)gtrack_alloc(1, sizeof(F9));
            	inst->params.Q = (float *)gtrack_alloc(1, sizeof(Q9));

                memcpy(inst->params.F, F9, sizeof(F9));
                memcpy(inst->params.Q, Q9, sizeof(Q9));
            }
            break;

        default:
    		*errCode = GTRACK_EINVAL;
	    	goto exit;
    }
	inst->params.stateVectorType = config->stateVectorType;

    inst->params.maxRadialVelocity = config->maxRadialVelocity;
	inst->params.radialVelocityResolution = config->radialVelocityResolution;
	inst->params.initialRadialVelocity = config->initialRadialVelocity;

	switch(config->verbose) {
		case GTRACK_VERBOSE_NONE:
			inst->params.verbose = 0U;
			break;
		case GTRACK_VERBOSE_ERROR:
			inst->params.verbose = VERBOSE_ERROR_INFO;
			break;
		case GTRACK_VERBOSE_WARNING:
			inst->params.verbose = VERBOSE_ERROR_INFO | VERBOSE_WARNING_INFO;
			break;
		default:
		case GTRACK_VERBOSE_DEBUG:
			inst->params.verbose = VERBOSE_ERROR_INFO | VERBOSE_WARNING_INFO | VERBOSE_DEBUG_INFO | VERBOSE_UNROLL_INFO | VERBOSE_STATE_INFO;
			break;
		case GTRACK_VERBOSE_MATRIX:			
			inst->params.verbose = VERBOSE_ERROR_INFO | VERBOSE_WARNING_INFO | VERBOSE_DEBUG_INFO | VERBOSE_MATRIX_INFO;
			break;
		case GTRACK_VERBOSE_MAXIMUM:
			inst->params.verbose = VERBOSE_ERROR_INFO | VERBOSE_WARNING_INFO | VERBOSE_DEBUG_INFO | VERBOSE_MATRIX_INFO | VERBOSE_UNROLL_INFO | VERBOSE_STATE_INFO | VERBOSE_ASSOSIATION_INFO;
			break;
	}

	/* hTrack is an array of void pointers */
	inst->hTrack = (void **) gtrack_alloc(inst->maxNumTracks, sizeof(GtrackUnitInstance *));
	if(inst->hTrack == NULL) {
		*errCode = GTRACK_ENOMEM;
		goto exit;
	}

	/* scoreSheet is an array of best scores */
	inst->bestScore = (float *) gtrack_alloc(inst->maxNumPoints, sizeof(float));
	if(inst->bestScore == NULL) {
		*errCode = GTRACK_ENOMEM;
		goto exit;
	}

	/* association array holds the ids of the best scorers */
	inst->bestIndex = (uint8_t *) gtrack_alloc(inst->maxNumPoints, sizeof(uint8_t));
	if(inst->bestIndex == NULL) {
		*errCode = GTRACK_ENOMEM;
		goto exit;
	}
	/* allocation array holds the measurement indices of allocation set */
	inst->allocIndex = (uint16_t *) gtrack_alloc(inst->maxNumPoints, sizeof(uint16_t));
	if(inst->allocIndex == NULL) {
		*errCode = GTRACK_ENOMEM;
		goto exit;
	}

	/* array of tracking IDs */
	inst->uidElem = (GTrack_ListElem *) gtrack_alloc(inst->maxNumTracks, sizeof(GTrack_ListElem));
	if(inst->uidElem == NULL) {
		*errCode = GTRACK_ENOMEM;
		goto exit;
	}

	inst->targetNumTotal = 0U;
	inst->targetNumCurrent = 0U;

	gtrack_listInit(&inst->freeList);
	gtrack_listInit(&inst->activeList);

	/* Create unit trackers */
	for(uid=0U; uid < inst->maxNumTracks; uid++) {
		inst->uidElem[uid].data = uid;
		gtrack_listEnqueue(&inst->freeList, &inst->uidElem[uid]);

        inst->params.uid = uid;
		inst->hTrack[uid] = gtrack_unitCreate(&inst->params, errCode);
		if(*errCode != GTRACK_EOK) {
			goto exit;
		}
	}

exit:
	if(*errCode != GTRACK_EOK) {
		if(inst != NULL) {
			gtrack_delete(inst);
		}
		inst = NULL;
	}

	return inst;
}
