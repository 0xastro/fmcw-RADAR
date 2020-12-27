/**
 *   @file  gtrack_unit_update.c
 *
 *   @brief
 *      Unit level update function for the GTRACK Algorithm
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

#define MSIZE (GTRACK_MEASUREMENT_VECTOR_SIZE)
#define SSIZE (GTRACK_STATE_VECTOR_SIZE)

extern const float spreadMin[];

#define GTRACK_POS_DIMENSION (0U)
#define GTRACK_VEL_DIMENSION (1U)
#define GTRACK_ACC_DIMENSION (2U)

#define GTRACK_SPREAD_ALPHA     	                (0.01f) /* 1 percent */
#define GTRACK_DOPPLER_VARIANCE_EXPANSION_FACTOR    (4)     /* 2x sigma */
#define GTRACK_MIN_DISPERSION_ALPHA	                (0.1f)  /* 10 percent */
#define GTRACK_MIN_POINTS_TO_UPDATE_DISPERSION      (3)     /* 3 points */
#define GTRACK_MIN_STATIC_VELOCITY                  (1)     /* 1 m/s */

/**
*  @b Description
*  @n
*		GTRACK Module calls this function to perform an update step for the tracking unit. 
*
*  @param[in]  handle
*		This is handle to GTRACK unit
*  @param[in]  point
*		This is an array of measurement points
*  @param[in]  var
*      Pointer to an array of input measurment variances. Shall be set to NULL if variances are unknown
*  @param[in]  pInd
*		This is an array of associated UIDs. After association and allocation steps, each measurment shall have a UID assigned.
*  @param[in]  num
*		Number of measurement points
*
*  \ingroup GTRACK_ALG_UNIT_FUNCTION
*
*  @retval
*      None
*/

TrackState gtrack_unitUpdate(void *handle, GTRACK_measurementPoint *point, GTRACK_measurement_vector *var, uint8_t *pInd, uint16_t num)
{
    GtrackUnitInstance *inst;
	uint16_t n, m;

	uint32_t myPointNum; 
	float alpha;
    float J[MSIZE*SSIZE];
	float PJ[SSIZE*MSIZE];
	float JPJ[MSIZE*MSIZE];
	float u_tilda[MSIZE];
	float cC[MSIZE*MSIZE], cC_inv[MSIZE*MSIZE]; /* centroid Covariance and centroid Covariance inverse */
	float K[SSIZE*MSIZE];
	float rvPilot;
    float vel;

	GTRACK_measurementUnion 	u;
    GTRACK_measurementUnion 	u_sum;
	GTRACK_measurementUnion 	u_centroid;

    float u_max[MSIZE];
    float u_min[MSIZE];

    float centroidSnr = 0;
    float spread;
    float sigma;
    float rvIn;
    bool targetCanStop = false;

	GTRACK_measurementUnion 	uvar;
    GTRACK_measurementUnion     uvar_sum;
    GTRACK_measurementUnion 	uvar_mean;

    GTRACK_measurementUnion 	rm_diag;

    /* D is Dispersion matrix */
    float D[MSIZE*MSIZE];
    /* Rm is Measurement error covariance matrix */
    float Rm[MSIZE*MSIZE];
    /* Rc is Measurment error covariance matrix for the centroid used for Kalman update */
    float Rc[MSIZE*MSIZE];

	float temp1[SSIZE*SSIZE];

    inst = (GtrackUnitInstance *)handle;

	
	myPointNum = 0;

    gtrack_matrixInit(MSIZE, SSIZE, 0.f, J);
    gtrack_matrixInit(MSIZE, MSIZE, 0.f, D);
    gtrack_vectorInit(MSIZE, 0.f, u_sum.array);
    gtrack_vectorInit(MSIZE, 0.f, uvar_sum.array);
    gtrack_vectorInit(MSIZE, -FLT_MAX, u_max);
    gtrack_vectorInit(MSIZE, FLT_MAX, u_min);

    /* Compute means of associated measurement points */
    /* Accumulate measurements */
	for(n = 0; n < num; n++) {
		if(pInd[n] == inst->uid) {

            u.vector = point[n].vector;

            /* Compute maximums and minimums for each measurement */ 
            for(m = 0; m < MSIZE; m++) {
                if(u.array[m] > u_max[m])
                    u_max[m] = u.array[m];
                if(u.array[m] < u_min[m])
                    u_min[m] = u.array[m];
            }

            if(var != 0) {
                uvar.vector = var[n];
                gtrack_vectorAdd(GTRACK_MEASUREMENT_VECTOR_SIZE, uvar.array, uvar_sum.array, uvar_sum.array); 
            }

            if(myPointNum == 0)
                rvPilot = u.vector.doppler;
            else
                u.vector.doppler = gtrack_unrollRadialVelocity(inst->maxRadialVelocity, rvPilot, u.vector.doppler); /* Unroll using rvPilot */

            centroidSnr += point[n].snr;
            gtrack_vectorScalarMulAcc(GTRACK_MEASUREMENT_VECTOR_SIZE, u.array,  point[n].snr, u_sum.array);
			myPointNum++;
		}
	}

    if(myPointNum) {

        /* Update estimated number of points */
        if(myPointNum > inst->estNumOfPoints)
            inst->estNumOfPoints = (float)myPointNum;
        else
            inst->estNumOfPoints = 0.99f*inst->estNumOfPoints + 0.01f*(float)myPointNum;

        /* Compute measurement centroid as SNR-weighted mean of associated points */
        gtrack_vectorScalarMul(GTRACK_MEASUREMENT_VECTOR_SIZE, u_sum.array, 1.0f/centroidSnr, u_centroid.array);

        /* Unroll centroid radial velocity based on target state */
        rvIn = u_centroid.vector.doppler;
        gtrack_velocityStateHandling(inst, &u_centroid.vector);

        if(inst->isTargetStatic) {
            /* Returning from static condition */
            /* Restore state information */	
            memcpy(inst->S_apriori_hat, inst->S_apriori_saved, sizeof(inst->S_apriori_saved));
            inst->isTargetStatic = false;

            /* Restore scoring powers */
            inst->sFactor = 1.0f;
        }

        /* Compute mean measurment variance, if availbale */
        if(var != 0) {
            gtrack_vectorScalarMul(GTRACK_MEASUREMENT_VECTOR_SIZE, uvar_sum.array, 1.0f/(float)myPointNum, uvar_mean.array);
        }

        /* Update measurement spread if we have 2 or more points */
        if(myPointNum > 1) {
            for(m = 0; m < MSIZE; m++) {            
                spread = u_max[m] - u_min[m];
                /* Unbiased spread estimation */
                spread = spread*(myPointNum+1)/(myPointNum-1);
                if(spread < spreadMin[m])
                    spread = spreadMin[m];
                if(spread > inst->estSpread.array[m])
                    inst->estSpread.array[m] = spread;
                else
                    inst->estSpread.array[m] = (1.0f-GTRACK_SPREAD_ALPHA)*inst->estSpread.array[m] + GTRACK_SPREAD_ALPHA*spread;
            }
            
            gtrack_calcDim(inst->estSpread.array, u_centroid.vector.range, inst->estDim);
        }
    }
    else {
        /* Erasures handling: no measurements available */
        if(inst->isTargetStatic == false) {

            for(n=0; n<inst->stateVectorDimNum; n++) {
                vel = fabsf(inst->S_hat[GTRACK_VEL_DIMENSION*inst->stateVectorDimNum + n]);
                if(vel < GTRACK_MIN_STATIC_VELOCITY)
                    targetCanStop = true;
            }
            if(targetCanStop) {
                /* Target is static */
                inst->isTargetStatic = true;

                /* Save state information */
                memcpy(inst->S_apriori_saved, inst->S_apriori_hat, sizeof(inst->S_apriori_hat));

                /* Force zero velocity/zero acceleration */
                for(n=0; n<inst->stateVectorDimNum; n++) {
                    inst->S_apriori_hat[GTRACK_VEL_DIMENSION*inst->stateVectorDimNum + n] = 0.f;
                    inst->S_apriori_hat[GTRACK_ACC_DIMENSION*inst->stateVectorDimNum + n] = 0.f;
                }

                /* Keep process covariance equal to predicted */
                memcpy(inst->P_hat, inst->P_apriori_hat, sizeof(inst->P_hat));
                inst->sFactor = 0.5;
            }
            else {
                /* Target is moving => force constant velocity model */
                /* Force zero velocity/zero acceleration */
                for(n=0; n<inst->stateVectorDimNum; n++) {
                    inst->S_apriori_hat[GTRACK_ACC_DIMENSION*inst->stateVectorDimNum + n] = 0.f;
                }
            }
        }
    }
    
    /* Compute Rm, Measurement Noise covariance matrix */
    if(var == 0) {    
        for(m = 0; m < MSIZE; m++) {   
            /* spread is covered by plus/minus 2 sigmas */
            sigma = inst->estSpread.array[m]/4;
            uvar_mean.array[m] = sigma*sigma;
        }
        /* Expect doppler variance beyond long term torso/body variance */
        uvar_mean.vector.doppler = uvar_mean.vector.doppler*GTRACK_DOPPLER_VARIANCE_EXPANSION_FACTOR; 
    }
    gtrack_matrixSetDiag(GTRACK_MEASUREMENT_VECTOR_SIZE, uvar_mean.array, Rm);

	/* Update Group Dispersion gD matrix */
	if(myPointNum > GTRACK_MIN_POINTS_TO_UPDATE_DISPERSION) {
		/* D is the new dispersion matrix, MSIZExMSIZE */
		for(n = 0; n < num; n++) {
			if(pInd[n] == inst->uid)
                /* Accumulate covariance from all associated points */
                gtrack_matrixCovAcc(GTRACK_MEASUREMENT_VECTOR_SIZE, D, point[n].array, u_centroid.array);
		}
		/* Normalize it */
        gtrack_matrixCovNormalize(GTRACK_MEASUREMENT_VECTOR_SIZE, D, myPointNum);
			
        /* Update persistant group dispersion based on instantaneous D */
        /* The factor alpha goes from maximum (1.f) at the first allocation down to minimum of 0.1f once the target is observed for the long time */ 
        alpha = ((float)myPointNum)/inst->estNumOfPoints;

        /*  inst->gD = (1-alpha)*inst->gD + alpha*D */
        gtrack_matrixCovFilter(GTRACK_MEASUREMENT_VECTOR_SIZE, inst->gD, D, alpha);
	}

	/* Compute state vector partial derivatives (Jacobian matrix) */
	gtrack_computeJacobian(inst->currentStateVectorType, inst->S_apriori_hat, J);
    /* Compute JPJ' = J(:,1:mSize) * obj.P_apriori(1:mSize,1:mSize) * J(:,1:mSize)' */
    gtrack_matrixComputePJT(inst->P_apriori_hat, J, PJ);
    gtrack_matrixMultiply(MSIZE, SSIZE, MSIZE, J, PJ, JPJ);

    if(myPointNum)
    {
        /* Compute centroid measurement noise covariance matrix Rc used for Kalman updates */
        /* First term represents the error in measuring the centroid and decreased with the number of measurments */
        /* Second term represents the centroid unsertanty due to the fact that not all the memebers observed */
        /* See page 327, 11A.2 of S.Blackman */
        /* Rc = Rm/num + alpha*unit.gD*eye(mSize); */

        /* alpha is weighting factor that is the function of the number of observed poionts versus total number of reflections in a group */
        /* alpha = (unit.maxAssociatedPoints-num)/((unit.maxAssociatedPoints-1)*num); */
        /* See page 327, 11A.3 of S.Blackman */

        alpha = ((float)(inst->estNumOfPoints-myPointNum))/((inst->estNumOfPoints-1)*myPointNum);
	    
        //  Rc.e11 = Rm.e11/myPointNum + alpha*inst->gD[0]; 
        //	Rc.e22 = Rm.e22/myPointNum + alpha*inst->gD[4]; 
        //	Rc.e33 = Rm.e33/myPointNum + alpha*inst->gD[8]; 

        gtrack_matrixGetDiag(GTRACK_MEASUREMENT_VECTOR_SIZE, inst->gD, rm_diag.array);
        for(n=0; n<GTRACK_MEASUREMENT_VECTOR_SIZE; n++)
            rm_diag.array[n] = uvar_mean.array[n]/myPointNum + rm_diag.array[n]*alpha;
        gtrack_matrixSetDiag(GTRACK_MEASUREMENT_VECTOR_SIZE, rm_diag.array, Rc);

#ifdef GTRACK_LOG_ENABLED
	    if(inst->verbose & VERBOSE_MATRIX_INFO) {
		    gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: spread\n", inst->heartBeatCount, inst->uid);
            gtrack_matrixPrint(1, MSIZE, inst->estSpread.array);
		    gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: Rm\n", inst->heartBeatCount, inst->uid);
		    gtrack_matrixPrint(MSIZE, MSIZE, Rm);
		    gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: D\n", inst->heartBeatCount, inst->uid);
		    gtrack_matrixPrint(MSIZE, MSIZE, D);
		    gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: gD\n", inst->heartBeatCount, inst->uid);
		    gtrack_matrixPrint(MSIZE, MSIZE, inst->gD);
		    gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: Rc\n", inst->heartBeatCount, inst->uid);
		    gtrack_matrixPrint(MSIZE, MSIZE, Rc);
	    }
#endif

#ifdef GTRACK_LOG_ENABLED
	    if(inst->verbose & VERBOSE_MATRIX_INFO) {
		    gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: S-apriori\n", inst->heartBeatCount, inst->uid);
            gtrack_matrixPrint(1,SSIZE, inst->S_apriori_hat);
		    gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: J\n", inst->heartBeatCount, inst->uid);
		    gtrack_matrixPrint(MSIZE, SSIZE, J);
	    }
#endif

	    /* Compute innovation */
        gtrack_matrixSub(MSIZE, 1, u_centroid.array, inst->H_s.array, u_tilda);

#ifdef GTRACK_LOG_ENABLED
	    if(inst->verbose & VERBOSE_MATRIX_INFO) {
		    gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: u_tilda\n", inst->heartBeatCount, inst->uid);
		    gtrack_matrixPrint(MSIZE, 1, u_tilda);
	    }
#endif

	    /* Compute centroid covariance cC = [3x6]x[6x6]x[6x3]+[3x3] */
        /* cC = J(:,1:mSize) * obj.P_apriori(1:mSize,1:mSize) * J(:,1:mSize)' + Rc */
	    gtrack_matrixAdd(MSIZE, MSIZE, JPJ, Rc, cC);

	    /* Compute inverse of cC */
        gtrack_matrixInv(cC, temp1, cC_inv);

#ifdef GTRACK_LOG_ENABLED
	    if(inst->verbose & VERBOSE_MATRIX_INFO) {
		    gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: P\n", inst->heartBeatCount, inst->uid);
		    gtrack_matrixPrint(SSIZE, SSIZE, inst->P_apriori_hat);
		    gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: cC\n", inst->heartBeatCount, inst->uid);
		    gtrack_matrixPrint(MSIZE, MSIZE, cC);
		    gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: cC_inv\n", inst->heartBeatCount, inst->uid);
		    gtrack_matrixPrint(MSIZE, MSIZE, cC_inv);
	    }
#endif

	    /* Compute Kalman Gain K[6x3] = P[6x6]xJ[3x6]'xIC_inv[3x3]=[6x3] */
        /* K = obj.P_apriori(1:mSize,1:mSize) * J(:,1:mSize)' * iC_inv */
	    gtrack_matrixMultiply(SSIZE, MSIZE, MSIZE, PJ, cC_inv, K);

#ifdef GTRACK_LOG_ENABLED
	    if(inst->verbose & VERBOSE_MATRIX_INFO) {
		    gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: K\n", inst->heartBeatCount, inst->uid);
		    gtrack_matrixPrint(SSIZE, MSIZE, K);
	    }
#endif

	    /* State estimation */
	    /* obj.S_hat(1:mSize) = obj.S_apriori_hat(1:mSize) + K * u_tilda */
	    gtrack_matrixMultiply(SSIZE, MSIZE, 1, K, u_tilda, temp1);
	    gtrack_matrixAdd(SSIZE,1, inst->S_apriori_hat, temp1, inst->S_hat);

	    /* Covariance estimation */
        /* obj.P(1:mSize,1:mSize) = obj.P_apriori(1:mSize,1:mSize) - K * J(:,1:mSize) * obj.P_apriori(1:mSize,1:mSize) */
	    gtrack_matrixTransposeMultiply(SSIZE, MSIZE, SSIZE, K, PJ, temp1);
	    gtrack_matrixSub(SSIZE, SSIZE, inst->P_apriori_hat, temp1, inst->P_hat);

    }
    else
    {
        /* Handling of erasures */	
        memcpy(inst->S_hat, inst->S_apriori_hat, sizeof(inst->S_hat));	
        memcpy(inst->P_hat, inst->P_apriori_hat, sizeof(inst->P_hat));
    }

#ifdef GTRACK_LOG_ENABLED
	if(inst->verbose & VERBOSE_MATRIX_INFO) {
		gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: S-hat\n", inst->heartBeatCount, inst->uid);
		gtrack_matrixPrint(SSIZE, 1, inst->S_hat);
		gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: P-hat\n", inst->heartBeatCount, inst->uid);
		gtrack_matrixPrint(SSIZE, SSIZE, inst->P_hat);
	}
#endif

	/* Compute groupCovariance gC (will be used in gating) */
    /* We will use ellipsoidal gating, that acounts for the dispersion of the group, target maneuver, and measurement noise */
    /* gC = gD + JPJ + Rm */
	gtrack_matrixAdd(MSIZE, MSIZE, JPJ, Rm, temp1);	
	gtrack_matrixAdd(MSIZE, MSIZE, temp1, inst->gD, inst->gC);

	/* Compute inverse of group innovation */
    gtrack_matrixInv(inst->gC, &inst->gC_det, inst->gC_inv);

#ifdef GTRACK_LOG_ENABLED
	if(inst->verbose & VERBOSE_MATRIX_INFO) {
		gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: gC\n", inst->heartBeatCount, inst->uid);
		gtrack_matrixPrint(MSIZE, MSIZE, inst->gC);
		gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: gC_inv\n", inst->heartBeatCount, inst->uid);
		gtrack_matrixPrint(MSIZE, MSIZE, inst->gC_inv);
	}
#endif

#ifdef GTRACK_LOG_ENABLED   
    if(inst->verbose & VERBOSE_DEBUG_INFO) {

        gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: ", inst->heartBeatCount, inst->uid);
        if(myPointNum) {
            gtrack_log(GTRACK_VERBOSE_DEBUG, "Update, %d points, ", myPointNum);

            gtrack_log(GTRACK_VERBOSE_DEBUG,"U={");
            for(m=0; m<MSIZE-1; m++) { 
                gtrack_log(GTRACK_VERBOSE_DEBUG,"%3.1f,", u_centroid.array[m]);
            }
            gtrack_log(GTRACK_VERBOSE_DEBUG,"%3.1f=>%3.1f}, ", rvIn, u_centroid.vector.doppler);
        }
        else {
            gtrack_log(GTRACK_VERBOSE_DEBUG, "Miss, ");
            if(inst->isTargetStatic) {
                gtrack_log(GTRACK_VERBOSE_DEBUG, "Static, ");
            }
            else {
                gtrack_log(GTRACK_VERBOSE_DEBUG, "Moving, ");
            }
        }

        gtrack_log(GTRACK_VERBOSE_DEBUG, "dim={");
        for(m=0; m<MSIZE-1; m++) {    
            gtrack_log(GTRACK_VERBOSE_DEBUG, "%4.3f,", inst->estDim[m]);
        }
        gtrack_log(GTRACK_VERBOSE_DEBUG, "%4.3f}, ", inst->estDim[MSIZE-1]);

		gtrack_log(GTRACK_VERBOSE_DEBUG, "S={");
        for(n=0; n<SSIZE-1; n++) {
            gtrack_log(GTRACK_VERBOSE_DEBUG, "%3.1f,", inst->S_hat[n]);
        }
        gtrack_log(GTRACK_VERBOSE_DEBUG, "%3.1f}\n", inst->S_hat[SSIZE-1]);
	}
#endif

	gtrack_unitEvent(inst, myPointNum);
	return(inst->state);
}

void gtrack_velocityStateHandling(void *handle, GTRACK_measurement_vector *uVec)
{
	GtrackUnitInstance *inst;
	float instanteneousRangeRate;
	float rrError;
	float rvError;
	float rvIn;

	inst = (GtrackUnitInstance *)handle;
    rvIn = uVec->doppler;

	switch(inst->velocityHandling) {

		case VELOCITY_INIT:
            uVec->doppler = inst->rangeRate;
			inst->velocityHandling = VELOCITY_RATE_FILTER;
#ifdef GTRACK_LOG_ENABLED
			if(inst->verbose & VERBOSE_UNROLL_INFO) {
                gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: Update vState VINIT=>VFILT, %3.1f=>%3.1f\n", inst->heartBeatCount, inst->uid, rvIn, uVec->doppler);
			}
#endif
			break;

		case VELOCITY_RATE_FILTER:
			/* In this state we are using filtered Rate Range to unroll radial velocity, stabilizing Range rate */
            instanteneousRangeRate = (uVec->range - inst->allocationRange)/((inst->heartBeatCount-inst->allocationTime)*inst->dt);

			inst->rangeRate = inst->unrollingParams->alpha * inst->rangeRate + (1-inst->unrollingParams->alpha) * instanteneousRangeRate;
            uVec->doppler = gtrack_unrollRadialVelocity(inst->maxRadialVelocity, inst->rangeRate, rvIn);

			rrError = (instanteneousRangeRate - inst->rangeRate)/inst->rangeRate;
		
			if(fabsf(rrError) < inst->unrollingParams->confidence) {
				inst->velocityHandling = VELOCITY_TRACKING;
#ifdef GTRACK_LOG_ENABLED
				if(inst->verbose & VERBOSE_UNROLL_INFO) {
                    gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: Update vState VFILT=>VTRACK, Unrolling with RangeRate=%3.1f: %3.1f=>%3.1f\n", inst->heartBeatCount, inst->uid, inst->rangeRate, rvIn, uVec->doppler);
				}
#endif
			}
			else {	
#ifdef GTRACK_LOG_ENABLED
				if(inst->verbose & VERBOSE_UNROLL_INFO) {
                    gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: Update vState VFILT, RangeRate=%3.1f, H-s=%3.1f, rvIn=%3.1f=>%3.1f\n", inst->heartBeatCount, inst->uid, inst->rangeRate, inst->H_s.vector.doppler, rvIn, uVec->doppler);
				}
#endif
			}
			break;

		case VELOCITY_TRACKING:
			/* In this state we are using filtered Rate Range to unroll radial velocity and monitoring Hs error */
            instanteneousRangeRate = (uVec->range - inst->allocationRange)/((inst->heartBeatCount-inst->allocationTime)*inst->dt);

			inst->rangeRate = inst->unrollingParams->alpha * inst->rangeRate + (1-inst->unrollingParams->alpha) * instanteneousRangeRate;
            uVec->doppler = gtrack_unrollRadialVelocity(inst->maxRadialVelocity, inst->rangeRate, rvIn);

            rvError = (inst->H_s.vector.doppler - uVec->doppler)/uVec->doppler;
			if(fabsf(rvError) < 0.1f) {
				inst->velocityHandling = VELOCITY_LOCKED;

#ifdef GTRACK_LOG_ENABLED
				if(inst->verbose & VERBOSE_UNROLL_INFO) {
                    gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: Update vState VTRACK=>VLOCK, Unrolling with RangeRate=%3.1f, H-s=%3.1f: %3.1f=>%3.1f\n", inst->heartBeatCount, inst->uid, inst->rangeRate, inst->H_s.vector.doppler, rvIn, uVec->doppler);
				}
#endif
			}
			else {

#ifdef GTRACK_LOG_ENABLED
				if(inst->verbose & VERBOSE_UNROLL_INFO) {
                    gtrack_log(GTRACK_VERBOSE_DEBUG, "%llu: uid[%d]: Update vState VTRACK, Unrolling with RangeRate=%3.1f, H-s=%3.1f: %3.1f=>%3.1f\n", inst->heartBeatCount, inst->uid, inst->rangeRate, inst->H_s.vector.doppler, rvIn, uVec->doppler);
				}
#endif
			}
			break;

		case VELOCITY_LOCKED:
            uVec->doppler = gtrack_unrollRadialVelocity(inst->maxRadialVelocity, inst->H_s.vector.doppler, uVec->doppler);
			break;
	}
}
