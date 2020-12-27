/**
 *   @file  gtrack_unit_score.c
 *
 *   @brief
 *      Unit level scoring function for the GTRACK Algorithm
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

#include <math.h>
#include <string.h>

#include <ti/alg/gtrack/gtrack.h>
#include <ti/alg/gtrack/include/gtrack_int.h>


/**
*  @b Description
*  @n
*		GTRACK Module calls this function to obtain the measurement vector scoring from the GTRACK unit perspective
*
*  @param[in]  handle
*		This is handle to GTRACK unit
*  @param[in]  point
*		This is an array of measurement points
*  @param[inout]  bestScore
*		This is a pointer current scoresheet with best scores. Only better scores are updated
*  @param[inout]  bestInd
*		This is a pointer current scoresheet winners. Only better scores winners are updated
*  @param[in]  num
*		Number of measurement points
*
*  \ingroup GTRACK_ALG_UNIT_FUNCTION
*
*  @retval
*      None
*/
void gtrack_unitScore(void *handle, GTRACK_measurementPoint *point, float *bestScore, uint8_t *bestInd, uint16_t num)
{
	GtrackUnitInstance *inst;
	uint16_t n;

	float chi2;
    
    GTRACK_measurementUnion u_tilda;

	float score;
	float logdet;

	float rvOut;
	
	inst = (GtrackUnitInstance *)handle;
  
    inst->G = gtrack_gateCreateLim(inst->gatingParams->volume, inst->gC_inv, inst->H_s.vector.range, inst->gatingParams->limitsArray);

//	gtrack_matrixDet3(inst->gC, &det);
    logdet = inst->sFactor*logf(inst->gC_det);

#ifdef GTRACK_LOG_ENABLED
	if(inst->verbose & VERBOSE_ASSOSIATION_INFO) {
		gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: Scoring: G=%5.2f, log(det)=%5.2f\n",inst->heartBeatCount, inst->uid, inst->G, logdet);
	}
#endif

	for(n=0; n < num; n++) {

		if(bestInd[n] == GTRACK_ID_POINT_BEHIND_THE_WALL)
			continue;

        gtrack_vectorSub(GTRACK_MEASUREMENT_VECTOR_SIZE, point[n].array, inst->H_s.array, u_tilda.array);

		if(inst->velocityHandling < VELOCITY_LOCKED) {
			/* Radial velocity estimation is not yet known, unroll based on velocity measured at allocation time */
			rvOut = gtrack_unrollRadialVelocity(inst->maxRadialVelocity, inst->allocationVelocity, point[n].vector.doppler);
            u_tilda.vector.doppler = rvOut - inst->allocationVelocity;
		}
		else {
			/* Radial velocity estimation is known */
            rvOut = gtrack_unrollRadialVelocity(inst->maxRadialVelocity, inst->H_s.vector.doppler, point[n].vector.doppler);
            u_tilda.vector.doppler = rvOut - inst->H_s.vector.doppler;
		}

		gtrack_computeMahalanobis(u_tilda.array, inst->gC_inv, &chi2);
		/* Gating Step */
		if(chi2 < inst->G) {
			/* Scoring */
			score = logdet + chi2;
			if(score < bestScore[n]) {
				/* If we are the best, register our score, and the index */
				bestScore[n] = score;
				bestInd[n] = (uint8_t)inst->uid;
				point[n].vector.doppler = rvOut;
			}
		}
	}	
    memcpy(inst->ec, inst->gC_inv, sizeof(inst->ec));
}


