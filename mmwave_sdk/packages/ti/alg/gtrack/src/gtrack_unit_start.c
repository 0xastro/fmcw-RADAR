/**
 *   @file  gtrack_unit_start.c
 *
 *   @brief
 *      Unit level start function for the GTRACK Algorithm
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

#include <ti/alg/gtrack/gtrack.h>
#include <ti/alg/gtrack/include/gtrack_int.h>

extern const float pInit[];
extern const float spreadInit[];

/**
*  @b Description
*  @n
*		GTRACK Module calls this function to start target tracking. This function is called during modules' allocation step, 
*		once new set of points passes allocation thresholds 
*
*  @param[in]  handle
*		This is handle to GTRACK unit
*  @param[in]  timeStamp
*		This is an allocation time stamp
*  @param[in]  tid
*		This is a target identifier given to a unit
*  @param[in]  uCenter
*		This is a pointer to the centroid in measurement coordinates
*
*  \ingroup GTRACK_ALG_UNIT_FUNCTION
*
*  @retval
*      None
*/

void gtrack_unitStart(void *handle, uint64_t timeStamp, uint32_t tid, GTRACK_measurement_vector *uCenter)
{
    GtrackUnitInstance *inst;
    GTRACK_measurementUnion u;
    int n;

    inst = (GtrackUnitInstance *)handle;

	inst->tid = tid;
	inst->heartBeatCount = timeStamp;
	inst->allocationTime = timeStamp;
    inst->allocationRange = uCenter->range;
    inst->allocationVelocity = uCenter->doppler;
    inst->estNumOfPoints = 0;

	/* Initialize state and counters */
	inst->state = TRACK_STATE_DETECTION;
	inst->active2freeCount = 0;
	inst->detect2activeCount = 0;
	inst->detect2freeCount = 0;
    inst->currentStateVectorType = inst->stateVectorType;

    inst->isTargetStatic = false;

	/* Radial Velocity initialization */
	/* Radial Velocity handling is set to start with range rate filtering */
	inst->velocityHandling = VELOCITY_INIT;

    u.vector = *uCenter;

    u.vector.doppler = gtrack_unrollRadialVelocity(inst->maxRadialVelocity, inst->initialRadialVelocity, uCenter->doppler);
    inst->rangeRate = u.vector.doppler;

	/* Initialize a-priori State information */
    gtrack_spherical2cartesian(inst->currentStateVectorType, u.array, inst->S_apriori_hat);
    memcpy(inst->H_s.array, u.array, sizeof(u.array)); /* Initialize Hs to measurment vector */
	/* Initialize a-priori Process covariance */

	/* P_apriori_hat = eye(6) */
	/* memcpy(inst->P_apriori_hat, eye6x6, sizeof(eye6x6)); */

	/* P_apriori_hat = diag([0,0,0.5,0.5,1,1]) */
	memset(inst->P_apriori_hat, 0, sizeof(inst->P_apriori_hat));
    for(n = 0; n < GTRACK_STATE_VECTOR_SIZE; n++)
            inst->P_apriori_hat[n*GTRACK_STATE_VECTOR_SIZE+n] = pInit[n];

    memset(inst->gD, 0, sizeof(inst->gD));
    for(n = 0; n < GTRACK_MEASUREMENT_VECTOR_SIZE; n++)
        inst->estSpread.array[n] = spreadInit[n];

    inst->G = 1.0f;
    inst->sFactor = 1.0f;


#ifdef GTRACK_LOG_ENABLED
	if(inst->verbose & VERBOSE_STATE_INFO)
        gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d] ALLOCATED, TID %d, Range %5.2f, Angle %2.4f, Doppler %5.1f=>%5.1f\n", inst->heartBeatCount, inst->uid, inst->tid, u.vector.range, u.array[1], uCenter->doppler, u.vector.doppler);
#endif
}
