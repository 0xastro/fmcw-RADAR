/**
*   @file  Extended_Kalman_Filter_xyz.c
*
*   @brief
*      Implements an 'Extended Kalman Filter' for radar tracking.
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ti/mathlib/mathlib.h>
#define DebugP_ASSERT_ENABLED 1
#include <ti/drivers/osal/DebugP.h>
#include <assert.h>

#include "dss_data_path.h"
#include "EKF_XYZ_Consts.h"
#include "EKF_XYZ_Interface.h"


/* local function definitions. */
void InitAssociatedMeasurementIndx(int16_t * restrict assocMeasIndx, const int32_t numMeas);
void initAssociatedTrackerIndx(int16_t * restrict assocObjIndx, const int32_t numObj);
int32_t stateVecTimeUpdate(float * restrict state, const float td);
void stateCovmatTimeUpdate(float * restrict covmat, float const * restrict Qvec, const float td, KFtrackerInstance_t * restrict inst);
int32_t computePredictedMeas(float * restrict rrd, float const * restrict xyz, float * restrict p_invRange);
int32_t isTargetWithinDataAssociationThresh(float * restrict measResidual, float const * restrict state_rrd, float const * restrict meas_rrd, float const * restrict state_xyz, KFtrackerInstance_t * restrict inst, float * restrict pdistSq, int32_t tick, int32_t age);
int32_t symMatInv(float * restrict inv, float  const * restrict m);
void computeHmat(float * restrict hMat, float const * restrict statevec_xyz, float const * restrict stateVec, const float invrange);
void residualCovarianceComputation(float * restrict residCovmat, float const * restrict state_covmat, float const * restrict measCovVec, float const * restrict hMat);
void kalmanGainComputation(float * restrict kalmanGain, float const * restrict state_covmat, float const * restrict hMat, float const * restrict invResidCovmat, KFtrackerInstance_t * inst);
int32_t stateVecMeasurementUpdate(float * restrict state, float const * restrict kalmanGain, float const * restrict measResidual);
void stateCovmatMeasurementUpdate(float * restrict covmat, float const * restrict kalmanGain, float const * restrict hMat, KFtrackerInstance_t * inst);
int32_t selectMeas(int16_t * restrict selectedMeas, trackingInputReport_t const * restrict measArray, int16_t * restrict assocMeasIndx, const int32_t nFree, const int32_t numUnassociatedMeas, const int32_t numMeasTotal);
void createNewTracks(DSS_DataPathObj * restrict dataPathObj, trackingInputReport_t const * restrict measArray, int16_t  const * restrict freeTrackerIndxArray, int16_t const * restrict selectedIndxArr, const int32_t numSelected);
void initNewTracker(KFstate_t * restrict obj, trackingInputReport_t const * restrict meas);
float * select_QVec(float const * restrict QvecList, uint8_t tick, uint8_t age, const float range);
uint32_t isWithinBoundingBox(KFstate_t* restrict currTrack, DSS_DataPathObj * restrict dataPathObj);
void arrangeTracksByAge(KFstate_t * restrict TrackList, const uint32_t numTracksTotal);
int32_t invalidateCurrTrack(KFstate_t* restrict currTrack, int16_t * restrict freeTrackerIndxArray, int32_t nFree, int32_t iTrack);

/**
 *  @b Description
 *  @n
 *      This function initializes the memory used by the tracking function. 
 *      It also initializes all the trackers to an invalid state, and  then 
 *      populates the 'process noise matrix' or 'Qmat'. 
 *
 *  @param[in]  dataPathObj data path object 
 *
 *  @retval
 *      Not Applicable.
 */
void ekfInit(DSS_DataPathObj * restrict dataPathObj)
{
    KFstate_t * restrict currTrack;
    KFtrackerInstance_t * restrict inst = &(dataPathObj->trackerInstance);
    float * restrict QvecList = dataPathObj->trackerQvecList;
    float at_x, halfatsq_x;
    float at_y, halfatsq_y;

    int32_t iTrack; /* index to a tracker. */
    /* Allocate the memory for the temporary variables. */
    /* 1. First for the floats */
    inst->scratchPadFlt     = dataPathObj->trackerScratchPadFlt;
    inst->stateVecRrd       = &(inst->scratchPadFlt[0]); inst->scratchPadFlt+=N_MEASUREMENTS;
    inst->residCovmat       = &(inst->scratchPadFlt[0]); inst->scratchPadFlt+=N_UNIQ_ELEM_IN_SYM_RESIDCOVMAT;
    inst->invResidCovmat    = &(inst->scratchPadFlt[0]); inst->scratchPadFlt+=N_UNIQ_ELEM_IN_SYM_RESIDCOVMAT;
    inst->hMat              = &(inst->scratchPadFlt[0]); inst->scratchPadFlt+=N_UNIQ_ELEM_IN_HMAT;
    inst->kalmanGain        = &(inst->scratchPadFlt[0]); inst->scratchPadFlt+=N_STATES*N_MEASUREMENTS;
    inst->covmattmp         = &(inst->scratchPadFlt[0]); inst->scratchPadFlt+=N_UNIQ_ELEM_IN_SYM_COVMAT;
    inst->kalmanGainTemp    = &(inst->scratchPadFlt[0]); inst->scratchPadFlt+=N_STATES*N_MEASUREMENTS;
    inst->stateCovMattemp   = &(inst->scratchPadFlt[0]); inst->scratchPadFlt+=N_STATES*N_STATES;
    inst->stateCovMattempP  = &(inst->scratchPadFlt[0]); inst->scratchPadFlt+=N_UNIQ_ELEM_IN_SYM_COVMAT;
    inst->measResidual      = &(inst->scratchPadFlt[0]); inst->scratchPadFlt+=N_MEASUREMENTS; 
    
    inst->scratchPadShort = dataPathObj->trackerScratchPadShort;
    inst->assocMeasIndx     = &(inst->scratchPadShort[0]);    inst->scratchPadShort+=MRR_MAX_OBJ_OUT;
    inst->freeTrackerIndxArray  = &(inst->scratchPadShort[0]);    inst->scratchPadShort+=MAX_TRK_OBJs; 
    inst->selectedIndxArr   = &(inst->scratchPadShort[0]);    inst->scratchPadShort+=MAX_TRK_OBJs;     

    currTrack = dataPathObj->trackerState;

    for (iTrack = 0;
        iTrack < MAX_TRK_OBJs;
        iTrack++, currTrack++)  
    {
        /* Invalidate this object.  */
        currTrack->validity  = IS_INVALID;
    }

    at_x = (MAX_ACCEL_X_M_P_SECSQ * FRAME_PERIODICITY_SEC);
    at_y = (MAX_ACCEL_Y_M_P_SECSQ * FRAME_PERIODICITY_SEC);
    halfatsq_x = 0.5f * (MAX_ACCEL_X_M_P_SECSQ * FRAME_PERIODICITY_SEC * FRAME_PERIODICITY_SEC);
    halfatsq_y = 0.5f * (MAX_ACCEL_Y_M_P_SECSQ * FRAME_PERIODICITY_SEC * FRAME_PERIODICITY_SEC);
    
    /* We allow for three different Qvecs.
     * A. The fast convergence option 1. */
    QvecList[iX] = 8 * (halfatsq_x * halfatsq_x);;
    QvecList[iY] = 8 * (halfatsq_y * halfatsq_y);;
    QvecList[iXd] = 8 * (at_x * at_x);;
    QvecList[iYd] = 8 * (at_y * at_y);;

    /* B. The not so fast convergence option 2. */
    QvecList[iX + N_STATES] = 4 * (halfatsq_x * halfatsq_x);;
    QvecList[iY + N_STATES] = 4 * (halfatsq_y * halfatsq_y);;
    QvecList[iXd + N_STATES] = 4 * (at_x * at_x);;
    QvecList[iYd + N_STATES] = 4 * (at_y * at_y);;

    /* C. The expected process noise (somewhat slower convergence). */
    QvecList[iX + 2 * N_STATES] = (halfatsq_x * halfatsq_x);
    QvecList[iY + 2 * N_STATES] = (halfatsq_y * halfatsq_y);
    QvecList[iXd + 2 * N_STATES] = (at_x * at_x);
    QvecList[iYd + 2 * N_STATES] = (at_y * at_y);

}


/**
 *  @b Description
 *  @n
 *      This function runs the kalmanfilter on a list of input data  
 *
 *  @param[in]  numMeas The number of tracking inputs.
 *  @param[in]  measArray An array of structures holding the measurements.  
 *  @param[in]  dataPathObj data path object
 *  @param[in]  QvecList A array of structures holding the 'process noise 
 *              matrix.
 *  @param[in]  td The update rate of the measurement. 
 *
 *  @retval
 *      Not Applicable.
 */
void ekfRun(const int32_t numMeas, 
            trackingInputReport_t const * restrict measArray, 
            DSS_DataPathObj * restrict dataPathObj, 
            float const * restrict QvecList, 
            const float td)  
{

    float distMetric;
    float distMetricMin;
    KFstate_t* restrict currTrack;
    trackingInputReport_t const * restrict currMeas;
    KFtrackerInstance_t * restrict inst = &(dataPathObj->trackerInstance);
    /* For every dataPathObj, perform a time update. Then attempt to associate a measurement.*/
    int32_t iMeas = 0; /* index to a measurement. */
    int32_t iTrack; /* index to an Tracker, total number of objects. */
    int32_t canBeAssociated;
    int32_t nAssociated = 0;
    int32_t nFree = 0;
    int32_t isInvOk = 0; 
    int32_t iAssocIndx;
    /* Assign addresses to the temporaries. */
    int16_t * restrict assocMeasIndx = inst->assocMeasIndx;
    int16_t * restrict freeTrackerIndxArray = inst->freeTrackerIndxArray;
    int16_t * restrict selectedIndxArr = inst->selectedIndxArr;    
    float * restrict stateVecRrd = inst->stateVecRrd;
    float * restrict residCovmat = inst->residCovmat;
    float * restrict invResidCovmat = inst->invResidCovmat;
    float * restrict hMat = inst->hMat;
    float * restrict kalmanGain = inst->kalmanGain;
    float * restrict measResidual = inst->measResidual; 
    float * restrict Qvec;
    float invRange;
    currTrack = dataPathObj->trackerState;

    /* Initialise some arrays. */
    
    /* The 'associated measurement Indx array' or assocMeasIndx holds the mapping from the index of 
     * a measurement to the  tracked object it is associated to. It is initialized to 
     * NOT_ASSOCIATED */
    InitAssociatedMeasurementIndx(assocMeasIndx, numMeas);
    
    /* The 'freeTrackerIndxArray' is a list of unassociated trackers. It is also initalized to 
     * NOT_ASSOCIATED. */
    initAssociatedTrackerIndx(freeTrackerIndxArray, MAX_TRK_OBJs);
    
    /* We need to associate each dataPathObj to an incoming measurement before we can do a 
     * Measurement update (Second step of EKF). For every valid tracked object, do a time update, 
     * then attempt to associate one of the available measurement. If a succesful  association is 
     * achieved, perform a time update. Finally at the  end of the loop, find all the aged 
     * objects and delete them. */
    for (iTrack = 0, nAssociated = 0, nFree = 0;
        iTrack < MAX_TRK_OBJs;
        iTrack++, currTrack++)  
    {

        /* If the object isn't valid, simply go on to the next obj. If the object has exceeded the 
         * bounding boxes (i.e. max range) , mark the object as invalid, and go on to the next object. */ 
        if ((currTrack->validity == IS_INVALID) || (!isWithinBoundingBox(currTrack, dataPathObj))) 
        {
            nFree = invalidateCurrTrack(currTrack, freeTrackerIndxArray, nFree, iTrack);
            continue;
        }
        
        canBeAssociated = 0;
        
        /* A> Time update*/
        /* a. State update. */
        /*    state = F*state. 
         * Also check if the object's updated position is behind the radar. */
        if (stateVecTimeUpdate(currTrack->vec, td) == IS_INVALID)
        {
            nFree = invalidateCurrTrack(currTrack, freeTrackerIndxArray, nFree, iTrack);
            continue;
        }

        /* b. select the appropriate 'process noise covariance matrix' or 'the Qmat' diagonal.*/
        Qvec = select_QVec(QvecList, currTrack->tick, currTrack->age, currTrack->vec[iY]);
        
        /* c. Covariance update.*/
        /*    covmat = F*covmat*F' + Q. */
        stateCovmatTimeUpdate(currTrack->covmat, Qvec, td, inst);
        
        /* Compute predicted measurements, also make sure that inv(range) is reasonable. */
        if (computePredictedMeas(stateVecRrd, currTrack->vec, &invRange) == IS_INVALID)
        {
            nFree = invalidateCurrTrack(currTrack, freeTrackerIndxArray, nFree, iTrack);
            continue;
        }
        
        /* B> Data Association. Attempt to associate a measurement to the object */
        distMetricMin = 655350.0f;
        iAssocIndx = NOT_ASSOCIATED;
        currMeas = &measArray[0];
        
        for (iMeas = 0; iMeas < numMeas; iMeas++)  
        {
            if (assocMeasIndx[iMeas] == NOT_ASSOCIATED)
            {
                /* If the measurement has not been associated, attempt to associate it.  */
                canBeAssociated = isTargetWithinDataAssociationThresh(measResidual, stateVecRrd, currMeas->measVec, currTrack->vec, inst, &distMetric, currTrack->tick, currTrack->age);
                if (canBeAssociated == IS_ASSOCIATED)
                {
                    if (distMetric < distMetricMin)
                    {
                        distMetricMin = distMetric;
                        iAssocIndx = iMeas;
                    }
                }
            }
            currMeas++;
        }

        canBeAssociated = (iAssocIndx != NOT_ASSOCIATED);

        if (canBeAssociated)
        {
            currMeas = &measArray[iAssocIndx];
                         
            /* We have a possible association, we can proceed with the measurement update. 
            * However, there is a catch, if the residual covmat is singular, we will have 
            * to abandon this measurement. */

            /* Compute the Hmat*/
            computeHmat(hMat, currTrack->vec, stateVecRrd, invRange);

            /* C> measurement update*/

            /* a. Measurement residual. */
            /* y_k = measVec - computeH(state).  */
            measResidual[iRANGE] = currMeas->measVec[iRANGE] - stateVecRrd[iRANGE] ;
            measResidual[iRANGE_RATE] = currMeas->measVec[iRANGE_RATE] - stateVecRrd[iRANGE_RATE];
            measResidual[iSIN_AZIM] = currMeas->measVec[iSIN_AZIM] - stateVecRrd[iSIN_AZIM];

            /* b. Residual Covariance computation. */
            /*  residCovmat = hMat*state_covmat*transpose(hMat) + measCovVec */
            residualCovarianceComputation(residCovmat, currTrack->covmat, currMeas->measCovVec, hMat);

            /*  invResidCovmat = 1/residCovmat */
            isInvOk = symMatInv(invResidCovmat, residCovmat);
            
            if (isInvOk == 1) 
            {
                /* c. Kalman Gain. */
                /*    kalmanGain = state_covmat*transpose(hMat)*inv(residCovmat)*/
                kalmanGainComputation(kalmanGain, currTrack->covmat, hMat, invResidCovmat, inst);

                /* d. State update. */
                /*    state = state  + kalmanGain*measResidual. 
                 * If the updated state is behind the radar, we invalidate the track. */
                if (stateVecMeasurementUpdate(currTrack->vec, kalmanGain, measResidual) == IS_INVALID)
                {
                    nFree = invalidateCurrTrack(currTrack, freeTrackerIndxArray, nFree, iTrack);                    
                    continue;
                }
                
                /* e. Covariance update. */
                /*    P = (I - KH)*P      */
                stateCovmatMeasurementUpdate(currTrack->covmat, kalmanGain, hMat, inst);
                        
                /* We have a definite association, let us 
                 * b1. mark this measurement as associated. */
                assocMeasIndx[iAssocIndx] = iTrack;
                /* b2. increment the number of associated measurements. */
                nAssociated++;
                /* b3. Since we have a new measurement, the 'age' of this track is zero. */
                currTrack->age = 0;
                /* b4. Increment the tick count of the object. */
                if (currTrack->tick < UINT8_MAX)
                {
                    currTrack->tick ++;
                }
            
                /* b5. Update the size of the cluster using the current size and a 1st order IIR. */
                currTrack->xSize = (int16_t) ((7 * ((int32_t) currTrack->xSize)) + ((int32_t) currMeas->xSize)) >> 3;
                currTrack->ySize = (int16_t) ((7 * ((int32_t) currTrack->ySize)) + ((int32_t) currMeas->ySize)) >> 3;

            } 
            else 
            {
               /* Remove the association and try again. */
                canBeAssociated = 0;
            }
        }

        if (canBeAssociated == 0)
        {
            /* No Association*/
            currTrack->age++;
            
            /* b. Decrement the tick counter for the track if this is second 
              * consecutive lack of association. */
            if ((currTrack->tick > 0) && (currTrack->age > 1))
            {
                currTrack->tick --;
            }

            /* Clear out the aged tracked objects, update the number of free tracks. */
            if (currTrack->age >  AGED_OBJ_DELETION_THRESH)
            {
                nFree = invalidateCurrTrack(currTrack, freeTrackerIndxArray, nFree, iTrack);
            }
        }
    }

    if (nFree > 0) 
    {
        int32_t numNewTracks;
        
        /* Select the unassociated measurements that are to be used for the new tracks. */
        numNewTracks = selectMeas(selectedIndxArr, measArray, assocMeasIndx, nFree, numMeas - nAssociated, numMeas);

        /* Create new tracks. */
        if (numNewTracks > 0) 
        {
            createNewTracks(dataPathObj, measArray, freeTrackerIndxArray, selectedIndxArr, numNewTracks);
        }
    }
    
    arrangeTracksByAge(dataPathObj->trackerState, MAX_TRK_OBJs);
}


/**
 *  @b Description
 *  @n
 *      Since we use a greedy algorithm to associate tracks and measurements, it is important that 
 *    stable (older) tracks are associated before the newer (more unstable) tracks. This will ensure
 *    that newer tracks do not associate with measurements that would have better associated with 
 *    older tracks.
 *
 *
 *  @retval
 *      Not Applicable.
 */
void arrangeTracksByAge(KFstate_t * restrict TrackList, const uint32_t numTracksTotal)
{
    KFstate_t *  currTrackFwd = &TrackList[0];
    KFstate_t *  currTrackRev = &TrackList[numTracksTotal-1];
    KFstate_t temp;
    uint32_t numTracks = numTracksTotal; 
    uint32_t i, j;
    
    /* For each track, assign the newer tracks to the bottom of the list. Copy the older tracks 
     * to the beginning.  */
    for (i = 0; i < numTracksTotal; i++)
    {
        if ((currTrackFwd->validity == IS_VALID) && (currTrackFwd->tick < 10))
        {
            for (j = 0; j < numTracks-(i+1); j++)
            {
                if ((currTrackRev->tick >= 10) && (currTrackFwd->validity == IS_VALID))
                {
                    temp = * currTrackRev;
                    *currTrackRev = *currTrackFwd;
                    *currTrackFwd = temp; 
                    currTrackRev--; j++;
                    break;
                }
                else
                {
                    currTrackRev--;    
                }
            }
            numTracks -= j;
        }
        
        if (j >= numTracks)
        {
            break;
        }
        currTrackFwd ++ ;
    }
}


/**
 *  @b Description
 *  @n
 *      This function initializes the assocMeasIndx to state that no measurements have 
 *      been associated to an existing tracker. 
 *
 *  @param[in]  assocMeasIndx An array that will hold the mapping of the incoming measurement to the
 *              existing tracker to which it is associated to.    
 *  @param[in]  nuMeas  number of measurement (for this epoch). 
 *
 *  @retval
 *      Not Applicable.
 */
void InitAssociatedMeasurementIndx(int16_t * restrict assocMeasIndx, const int32_t numMeas)
{
    int32_t ik;

    for (ik = 0; ik < numMeas; ik++){
        assocMeasIndx[ik] = NOT_ASSOCIATED;
    }

}

/**
 *  @b Description
 *  @n
 *      This function initializes the freeTrackerIndxArray to state that no objects have 
 *      been associated to an existing measurement. 
 *
 *  @param[in]  freeTrackerIndxArray An array that will hold the mapping of the existing tracker to 
 *              a measurement.    
 *  @param[in]  numObj  number of trackers (for this epoch). 
 *
 *  @retval
 *      Not Applicable.
 */
void initAssociatedTrackerIndx(int16_t * restrict freeTrackerIndxArray, const int32_t numObj)
{
    int32_t ik;

    for (ik = 0; ik < numObj; ik++){
        freeTrackerIndxArray[ik] = NOT_ASSOCIATED;
    }
}

/**
 *  @b Description
 *  @n
 *      This function updates the state vector. 
 *
 *  @param[in]  state    state vector.    
 *  @param[in]  td       time delta. 
 *
 *  @retval
 *      Is the object behind the radar?.
 */
int32_t stateVecTimeUpdate(float * restrict state, const float td)     
{
    state[iX] = state[iX] + td*state[iXd];
    state[iY] = state[iY] + td*state[iYd];
#if KF_3D
    state[iZ] = state[iZ] + td*state[iZd];
#endif

    if (state[iY] < 0)
    {
        return IS_INVALID;
    }

    return IS_VALID;
}

/**
 *  @b Description
 *  @n
 *      This function updates the state covariance matrix. 
 *
 *  @param[out]  covmat    state covariance matrix.    
 *  @param[in]  Q         process noise diagonal .    
 *  @param[in]  td       time delta. 
 *  @param[in]  inst     KFtrackerInstance_t (for scratchpad). 
 *
 *  @retval
 *      Not Applicable.
 */
void stateCovmatTimeUpdate(float * restrict  covmat, float const * restrict Q, const float td, KFtrackerInstance_t * restrict inst)      
{
    int32_t indx;
    float tdsq = td*td;
    float * restrict covmattmp = inst->covmattmp;


#if KF_2D
    covmattmp[iXX] = covmat[iXX] + 2 * td * covmat[iXXd] + tdsq * covmat[iXdXd] + Q[iX];
    covmattmp[iXY] = covmat[iXY] + td * (covmat[iXYd] + covmat[iYXd]) + tdsq * covmat[iXdYd];

    covmattmp[iXXd] = covmat[iXdXd] * td + covmat[iXXd];;
    covmattmp[iXYd] = covmat[iXdYd] * td + covmat[iXYd];;

    covmattmp[iYY] = tdsq*covmat[iYdYd] + 2 * td*covmat[iYYd] + covmat[iYY] + Q[iY];

    covmattmp[iYXd] = covmat[iXdYd] * td + covmat[iYXd];
    covmattmp[iYYd] = covmat[iYdYd] * td + covmat[iYYd];

    covmattmp[iXdXd] = covmat[iXdXd] + Q[iXd];
    covmattmp[iXdYd] = covmat[iXdYd];

    covmattmp[iYdYd] = covmat[iYdYd] + Q[iYd];
#elif KF_3D
    /* Autogenerated. */
    covmattmp[iXX] = covmat[iXdXd] * tdsq + 2 * covmat[iXXd] * td + covmat[iXX] + Q[iX];
    covmattmp[iXY] = covmat[iXdYd] * tdsq + (covmat[iYXd] + covmat[iXYd])*td + covmat[iXY];
    covmattmp[iXZ] = covmat[iXdZd] * tdsq + (covmat[iZXd] + covmat[iXZd])*td + covmat[iXZ];
    covmattmp[iXXd] = covmat[iXdXd] * td + covmat[iXXd];
    covmattmp[iXYd] = covmat[iXdYd] * td + covmat[iXYd];
    covmattmp[iXZd] = covmat[iXdZd] * td + covmat[iXZd];
    covmattmp[iYY] = covmat[iYdYd] * tdsq + 2 * covmat[iYYd] * td + covmat[iYY] + Q[iY];
    covmattmp[iYZ] = covmat[iYdZd] * tdsq + (covmat[iZYd] + covmat[iYZd])*td + covmat[iYZ];
    covmattmp[iYXd] = covmat[iXdYd] * td + covmat[iYXd];
    covmattmp[iYYd] = covmat[iYdYd] * td + covmat[iYYd];
    covmattmp[iYZd] = covmat[iYdZd] * td + covmat[iYZd];
    covmattmp[iZZ] = covmat[iZdZd] * tdsq + 2 * covmat[iZZd] * td + covmat[iZZ] + Q[iZ];
    covmattmp[iZXd] = covmat[iXdZd] * td + covmat[iZXd];
    covmattmp[iZYd] = covmat[iYdZd] * td + covmat[iZYd];
    covmattmp[iZZd] = covmat[iZdZd] * td + covmat[iZZd];
    covmattmp[iXdXd] = covmat[iXdXd] + Q[iXd];
    covmattmp[iXdYd] = covmat[iXdYd];
    covmattmp[iXdZd] = covmat[iXdZd];
    covmattmp[iYdYd] = covmat[iYdYd] + Q[iYd];
    covmattmp[iYdZd] = covmat[iYdZd];
    covmattmp[iZdZd] = covmat[iZdZd] + Q[iZd];
#endif

    /* Copy it back. */
    for (indx = 0; indx < N_UNIQ_ELEM_IN_SYM_COVMAT; indx++) {
        covmat[indx] = covmattmp[indx];
    }

}

/**
 *  @b Description
 *  @n
 *      This function computes the predicted measurements from the state vector.
 *      i.e. R = H(x);
 *
 *  @param[out]  rrd    predicted measurement.    
 *  @param[in]  xyz    state. 
 *  @param[in]  p_invRange The inv sqrt of the range is also computed and used 
 *              in the hMat computation. 
 *
 *  @retval
 *      Not Applicable.
 */
int32_t computePredictedMeas(float * restrict rrd, float const * restrict xyz, float * restrict p_invRange) 
{
    float rangesq, invrange;
#if KF_2D  
    rangesq = (xyz[iX] * xyz[iX]) + (xyz[iY] * xyz[iY]);
    if (rangesq < 0.005f)
    {
        return IS_INVALID;
    }
    
    invrange = rsqrtsp(rangesq);
    rrd[iRANGE] = sqrtsp(rangesq);
    rrd[iRANGE_RATE] = ((xyz[iX] * xyz[iXd]) + (xyz[iY] * xyz[iYd]))*invrange;
    rrd[iSIN_AZIM] = xyz[iX] * invrange;
#elif KF_3D
    rangesq = (xyz[iX] * xyz[iX]) + (xyz[iY] * xyz[iY]) + (xyz[iZ] * xyz[iZ]);
    invrange = rsqrtsp(rangesq);
    rrd[iRANGE] = sqrtsp(rangesq);
    rrd[iRANGE_RATE] = ((xyz[iX] * xyz[iXd]) + (xyz[iY] * xyz[iYd]) + (xyz[iZ] * xyz[iZd]))*invrange;
    rrd[iCOS_ELEV_SIN_AZIM] = xyz[iX] * invrange;
    rrd[iSIN_ELEV] = xyz[iZ] * invrange;
#endif
    (*p_invRange) = invrange;
    
    return IS_VALID;
}

/**
 *  @b Description
 *  @n
 *      This function checks whether the predicted measurements are close to 
 *    the new measurements ( i.e. can they be associated). If they can be 
 *    then the mean sq distance between the predicted and the new 
 *    is passed back, so that the closest measurement is used. 
 *
 *    Track association is not a traditional part of Kalman filtering, and
 *    hence our approach (based on experiment) is ad-hoc.
 *
 *  @param[out]  measResidual    measurement residual.    
 *  @param[in]  state_rrd       predicted measurents. 
 *  @param[in]  meas_rrd        new measurements. 
 *  @param[in]  state_xyz       predicted state (in x y co-ordinates)
 *  @param[in]  inst            KFtrackerInstance_t object (for 
 *                              scratchpad.)
 *  @param[out] pdistSq     The distance sq between predicted and new measurement. 
 *                          Only valid if the measurement is deemed to be 
 *                          'associatable'.
 *  @param[in]  tick        age of the Track. Newer tracks are given more leeway
 &                          in associating, as they take some time to converge. 
 *
 *  @retval
 *      IS_ASSOCIATED or NOT_ASSOCIATED.
 */
int32_t isTargetWithinDataAssociationThresh(float * restrict measResidual, 
                                            float const * restrict state_rrd, 
                                            float const * restrict meas_rrd, 
                                            float const * restrict state_xyz, 
                                            KFtrackerInstance_t * restrict inst, 
                                            float * restrict pdistSq, 
                                            int32_t tick, 
                                            int32_t age)
{
#if KF_2D
    
    float x, y, dx, dy, distSq;
    float modifier;
    /* If this is a new track, search for an association in a wider range.  */
    if (tick < 10) 
    {
        modifier = 4.0f; 
    }
    else if (state_rrd[iRANGE] < 20.0f)
    {
        modifier = 1.5f;
    }
    else
    {
        modifier = 1.0f;
    }
    
    /* If the object hasn't been associated in the previous tick, search in a wider range. */
    if ((age > 0) && (tick > 10))
    {
        modifier = modifier * (2.0f);
    }
    
    measResidual[iRANGE] = meas_rrd[iRANGE] - state_rrd[iRANGE] ;
    
    if (_fabsf(measResidual[iRANGE]) < (modifier*inst->rangeAssocThresh)) 
    {
        
        measResidual[iRANGE_RATE] = meas_rrd[iRANGE_RATE] - state_rrd[iRANGE_RATE];
        
        if (_fabsf(measResidual[iRANGE_RATE])< (modifier*inst->velAssocThresh)) 
        {
            measResidual[iSIN_AZIM] = meas_rrd[iSIN_AZIM] - state_rrd[iSIN_AZIM];
            
            if (_fabsf(measResidual[iSIN_AZIM]) < (modifier*inst->azimAssocThresh)) 
            {
                x = meas_rrd[iRANGE]*meas_rrd[iSIN_AZIM];
                y = sqrtsp((meas_rrd[iRANGE]*meas_rrd[iRANGE]) - (x*x));
                dx = (state_xyz[iX] - x);
                dy = (state_xyz[iY] - y);
                distSq = (dx*dx) + (dy*dy);
                if (    (distSq < (inst->distAssocThreshSq * modifier * modifier )) 
                    ||  (measResidual[iRANGE] > 40.0f))
                {
                    /* We adding in the velocity residual as well to the distance metric. */
                    *pdistSq = (distSq + (measResidual[iRANGE_RATE]*measResidual[iRANGE_RATE]));
                    return IS_ASSOCIATED;
                }
            }
        }         
    }

    
    
#elif KF_3D

    measResidual[iRANGE] = meas_rrd[iRANGE] - state_rrd[iRANGE] ;

    if (_fabsf(measResidual[iRANGE]) < inst->rangeAssocThresh) 
    {
        measResidual[iRANGE_RATE] = meas_rrd[iRANGE_RATE] - state_rrd[iRANGE_RATE];
        if (_fabsf(measResidual[iRANGE_RATE]) < inst->velAssocThresh) 
        {
            measResidual[iCOS_ELEV_SIN_AZIM] = meas_rrd[iCOS_ELEV_SIN_AZIM] - state_rrd[iCOS_ELEV_SIN_AZIM] ;
            if (_fabsf(measResidual[iCOS_ELEV_SIN_AZIM]) < inst->cosEleSinAzimAssocThresh) 
            {
                measResidual[iSIN_ELEV] = meas_rrd[iSIN_ELEV] - state_rrd[iSIN_ELEV];
                if (_fabsf(measResidual[iSIN_ELEV]) < inst->sinElevThresh)
                {
                    return IS_ASSOCIATED;
                }
            }
        }         
    }
    
#endif



    return NOT_ASSOCIATED;
}

/**
 *  @b Description
 *  @n
 *      This function computes the inverse of a symettric 
 *      N_MEASUREMENTS x N_MEASUREMENTS matrix. 
 *
 *  @param[out]  inv    inverted matrix.    
 *  @param[in]  m       input matrix. 
 *
 *  @retval
 *      Not Applicable.
 */
int32_t symMatInv(float  * restrict inv, float const * restrict m){
    float det, invDet;
    int32_t ik;
#if KF_2D 
    /* The 2D Kalman filter's residual covariance matrix is a 3x3 matrix */
    /* Since the Adjoint32_t is also symmetric, we only need to compute 6    */
    /* components of the adjoint. */
    inv[0] = (m[3] * m[5]) - (m[4] * m[4]);
    inv[1] = (m[4] * m[2]) - (m[1] * m[5]);
    inv[2] = (m[1] * m[4]) - (m[3] * m[2]);
    inv[3] = (m[0] * m[5]) - (m[2] * m[2]);
    inv[4] = (m[1] * m[2]) - (m[0] * m[4]);
    inv[5] = (m[0] * m[3]) - (m[1] * m[1]);

    det = m[0] * inv[0] + m[1] * inv[1] + m[2] * inv[2];

    
#elif KF_3D
    /* The 3D Kalman filter's residual covariance matrix is a 4x4 matrix */
    /* Since the Adjoint is also symmetric, we only need to compute 10   */
    /* components of the adjoint. */
    inv[0] = m[4] * (m[7] * m[9] - m[8] * m[8]) - m[5] * (m[5] * m[9] - m[6] * m[8]) + m[6] * (m[5] * m[8] - m[6] * m[7]);
    inv[1] = -m[1] * (m[7] * m[9] - m[8] * m[8]) + m[2] * (m[5] * m[9] - m[6] * m[8]) - m[3] * (m[5] * m[8] - m[6] * m[7]);
    inv[2] = m[1] * (m[5] * m[9] - m[6] * m[8]) - m[2] * (m[4] * m[9] - m[6] * m[6]) + m[3] * (m[4] * m[8] - m[5] * m[6]);
    inv[3] = -m[1] * (m[5] * m[8] - m[6] * m[7]) + m[2] * (m[4] * m[8] - m[5] * m[6]) - m[3] * (m[4] * m[7] - m[5] * m[5]);
    inv[4] = m[0] * (m[7] * m[9] - m[8] * m[8]) - m[2] * (m[2] * m[9] - m[3] * m[8]) + m[3] * (m[2] * m[8] - m[3] * m[7]);
    inv[5] = -m[0] * (m[5] * m[9] - m[6] * m[8]) + m[2] * (m[1] * m[9] - m[3] * m[6]) - m[3] * (m[1] * m[8] - m[3] * m[5]);
    inv[6] = m[0] * (m[5] * m[8] - m[6] * m[7]) - m[2] * (m[1] * m[8] - m[2] * m[6]) + m[3] * (m[1] * m[7] - m[2] * m[5]);
    inv[7] = m[0] * (m[4] * m[9] - m[6] * m[6]) - m[1] * (m[1] * m[9] - m[3] * m[6]) + m[3] * (m[1] * m[6] - m[3] * m[4]);
    inv[8] = -m[0] * (m[4] * m[8] - m[5] * m[6]) + m[1] * (m[1] * m[8] - m[2] * m[6]) - m[3] * (m[1] * m[5] - m[2] * m[4]);
    inv[9] = m[0] * (m[4] * m[7] - m[5] * m[5]) - m[1] * (m[1] * m[7] - m[2] * m[5]) + m[2] * (m[1] * m[5] - m[2] * m[4]);
    det = m[0] * inv[0] + m[1] * inv[1] + m[2] * inv[2] + m[3] * inv[3];
#endif
    
    /* Correlation matrices are positive semi-definite. We also 
     * whether it is singular or nan. */
    if (det > EPS)  
    {
        invDet = recipsp(det);

        for (ik = 0; ik < N_UNIQ_ELEM_IN_SYM_RESIDCOVMAT; ik++)
        {
            inv[ik] = inv[ik] * invDet;
        }

        return true;
    }
    else
    {
        return false;
    }

}

/**
 *  @b Description
 *  @n
 *      This function computes the observation matrix (hMat). 
 *
 *  @param[out]  hMat    a vector containing the non-zero elements of the hmat.    
 *  @param[in]  statevec_xyz      predicted state (in x y co-ordinates)
 *  @param[in]  stateVecRrd       predicted measurents.
 *  @param[in]  invrange          1/r - computed in the computePredictedMeas function.
 *
 *  @retval
 *      Not Applicable.
 */
void computeHmat(float * restrict hMat, float const * restrict statevec_xyz, float const * restrict stateVecRrd, const float invrange)
{
#if KF_2D
    /* Only six elements of the hMat need to computed for KF_2D */
    hMat[0] = stateVecRrd[iSIN_AZIM]; /*azim */
    hMat[1] = statevec_xyz[iY] * invrange;   /* y/r */
    hMat[2] = (statevec_xyz[iXd] - (stateVecRrd[iSIN_AZIM] * stateVecRrd[iRANGE_RATE]))*invrange; /* xd/r-(azim*rd)/r */
    hMat[3] = (statevec_xyz[iYd] - (stateVecRrd[iRANGE_RATE] * hMat[1]))*invrange; /* yd/r-(rd*y)/r^2 */
    hMat[4] = (1.0f - (stateVecRrd[iSIN_AZIM] * stateVecRrd[iSIN_AZIM]))*invrange;  /* 1/r-azim^2/r */
    hMat[5] = -(stateVecRrd[iSIN_AZIM] * hMat[1])*invrange; /* -(azim*y)/r^2 */
#elif KF_3D
    /* Only 12 elements need to computed for KF_3D */
    hMat[0] = stateVecRrd[iCOS_ELEV_SIN_AZIM]; /*  azim              */
    hMat[1] = statevec_xyz[iY] * invrange;  /*  y/r               */
    hMat[2] = stateVecRrd[iSIN_ELEV];  /*  elev              */
    hMat[3] = (statevec_xyz[iXd] - (stateVecRrd[iCOS_ELEV_SIN_AZIM] * stateVecRrd[iRANGE_RATE]))* invrange; /*  xd/r-(azim*rd)/r  */
    hMat[4] = (statevec_xyz[iYd] - (hMat[1] * stateVecRrd[iRANGE_RATE]))* invrange; /*  yd/r-(rd*y)/r^2   */
    hMat[5] = (statevec_xyz[iZd] - (stateVecRrd[iSIN_ELEV] * stateVecRrd[iRANGE_RATE]))* invrange; /*  zd/r-(elev*rd)/r  */
    hMat[6] = (1.0f - (stateVecRrd[iCOS_ELEV_SIN_AZIM] * stateVecRrd[iCOS_ELEV_SIN_AZIM]))*invrange;  /*  1/r-azim^2/r      */
    hMat[7] = -(stateVecRrd[iCOS_ELEV_SIN_AZIM] * hMat[1])*invrange; /*  -(azim*y)/r^2     */
    hMat[8] = -(stateVecRrd[iCOS_ELEV_SIN_AZIM] * stateVecRrd[iSIN_ELEV])*invrange; /*  -(azim*elev)/r    */
    hMat[9] = hMat[8];  /*  -(azim*elev)/r    */
    hMat[10] = -(stateVecRrd[iSIN_ELEV] * hMat[1])*invrange; /*  -(elev*y)/r^2     */
    hMat[11] = (1.0f - (stateVecRrd[iSIN_ELEV] * stateVecRrd[iSIN_ELEV]))*invrange; /*  1/r-elev^2/r      */
#endif
}

/**
 *  @b Description
 *  @n
 *      This function performs the residual covariance matrix computation.
 *      i.e. 
 *      residCovmat = hMat*state_covmat*transpose(hMat) + measCovVec      
 *
 *  @param[out]  residCovmat    residual covariance matrix.    
 *  @param[in]   state_covmat   state covariance matrix. 
 *  @param[in]   measCovVec     variance of the measurement. 
 *  @param[in]   hMat           (observation matrix) Hmat
 *
 *  @retval
 *      Not Applicable.
 */
void residualCovarianceComputation(float * restrict residCovmat, 
                                   float const * restrict state_covmat, 
                                   float const * restrict measCovVec, 
                                   float const * restrict hMat)
{
#if KF_2D
    /* Auto generated */

    /*  residCovmat = hMat*state_covmat*transpose(hMat) + measCovVec */
    float   A = hMat[0];
    float     B = hMat[1];
    float     C = hMat[2];
    float     D = hMat[3];
    float     E = hMat[4];
    float     F = hMat[5];

    float   Asq = hMat[0] * hMat[0];
    float     Bsq = hMat[1] * hMat[1];
    float     Csq = hMat[2] * hMat[2];
    float     Dsq = hMat[3] * hMat[3];
    float     Esq = hMat[4] * hMat[4];
    float     Fsq = hMat[5] * hMat[5];

    /*  residCovmat = hMat*state_covmat*transpose(hMat) */
    residCovmat[iRR] = Bsq*state_covmat[iYY] + 2 * A*B*state_covmat[iXY] + Asq*state_covmat[iXX];
    residCovmat[iRRd] = B*D*state_covmat[iYY] + Bsq*state_covmat[iYYd] + A*B*state_covmat[iYXd] + (A*D + B*C)*state_covmat[iXY] + A*B*state_covmat[iXYd] + A*C*state_covmat[iXX] + Asq*state_covmat[iXXd];
    residCovmat[iRAz] = B*F*state_covmat[iYY] + (A*F + B*E)*state_covmat[iXY] + A*E*state_covmat[iXX];
    residCovmat[iRdRd] = Bsq*state_covmat[iYdYd] + Dsq*state_covmat[iYY] + 2 * B*D*state_covmat[iYYd] + 2 * A*D*state_covmat[iYXd] + 2 * A*B*state_covmat[iXdYd] + Asq*state_covmat[iXdXd] + 2 * C*D*state_covmat[iXY] + 2 * B*C*state_covmat[iXYd] + Csq* state_covmat[iXX] + 2 * A*C*state_covmat[iXXd];
    residCovmat[iRdAz] = D*F*state_covmat[iYY] + B*F*state_covmat[iYYd] + A*F*state_covmat[iYXd] + (C*F + D*E)*state_covmat[iXY] + B*E*state_covmat[iXYd] + C*E*state_covmat[iXX] + A*E*state_covmat[iXXd];
    residCovmat[iAzAz] = Fsq*state_covmat[iYY] + 2 * E*F*state_covmat[iXY] + Esq*state_covmat[iXX];

    /*  residCovmat = residCovmat + measCovVec. */
    residCovmat[iRR] += measCovVec[iRANGE];
    residCovmat[iRdRd] += measCovVec[iRANGE_RATE];
    residCovmat[iAzAz] += measCovVec[iSIN_AZIM];
#elif KF_3D
    /*  residCovmat = hMat*state_covmat*transpose(hMat) + measCovVec */
    float   A = hMat[0];
    float     B = hMat[1];
    float     C = hMat[2];
    float     D = hMat[3];
    float     E = hMat[4];
    float     F = hMat[5];
    float     G = hMat[6];
    float     H = hMat[7];
    float     I = hMat[8];
    float     J = hMat[9];
    float     K = hMat[10];
    float     L = hMat[11];

    float   Asq = hMat[0] * hMat[0];
    float     Bsq = hMat[1] * hMat[1];
    float     Csq = hMat[2] * hMat[2];
    float     Dsq = hMat[3] * hMat[3];
    float     Esq = hMat[4] * hMat[4];
    float     Fsq = hMat[5] * hMat[5];
    float     Gsq = hMat[6] * hMat[6];
    float     Hsq = hMat[7] * hMat[7];
    float     Isq = hMat[8] * hMat[8];
    float     Jsq = hMat[9] * hMat[9];
    float     Ksq = hMat[10] * hMat[10];
    float     Lsq = hMat[11] * hMat[11];

    /*  residCovmat = hMat*state_covmat*transpose(hMat) */

    residCovmat[iRR] = Csq*state_covmat[iZZ] + 2 * B*C*state_covmat[iYZ] + Bsq*state_covmat[iYY] + 2 * A*C*state_covmat[iXZ] + 2 * A*B*state_covmat[iXY] + Asq*state_covmat[iXX];
    residCovmat[iRRd] = C*F*state_covmat[iZZ] + Csq*state_covmat[iZZd] + B*C*state_covmat[iZYd] + A*C*state_covmat[iZXd] + (B*F + C*E)*state_covmat[iYZ] + B*C*state_covmat[iYZd] + B*E*state_covmat[iYY] + Bsq*state_covmat[iYYd] + A*B* state_covmat[iYXd] + (A*F + C*D)*state_covmat[iXZ] + A*C*state_covmat[iXZd] + (A*E + B*D)*state_covmat[iXY] + A*B*state_covmat[iXYd] + A*D*state_covmat[iXX] + Asq*state_covmat[iXXd];
    residCovmat[iRAz] = C*I*state_covmat[iZZ] + (B*I + C*H)*state_covmat[iYZ] + B*H*state_covmat[iYY] + (A*I + C*G)*state_covmat[iXZ] + (A*H + B*G)*state_covmat[iXY] + A*G*state_covmat[iXX];
    residCovmat[iREl] = C*L*state_covmat[iZZ] + (B*L + C*K)*state_covmat[iYZ] + B*K*state_covmat[iYY] + (A*L + C*J)*state_covmat[iXZ] + (A*K + B*J)*state_covmat[iXY] + A*J*state_covmat[iXX];
    residCovmat[iRdRd] = Csq*state_covmat[iZdZd] + Fsq*state_covmat[iZZ] + 2 * C*F*state_covmat[iZZd] + 2 * B*F*state_covmat[iZYd] + 2 * A*F*state_covmat[iZXd] + 2 * B*C*state_covmat[iYdZd] + Bsq*state_covmat[iYdYd] + 2 * E*F*state_covmat[iYZ] + 2 * C* E*state_covmat[iYZd] + Esq*state_covmat[iYY] + 2 * B*E*state_covmat[iYYd] + 2 * A*E*state_covmat[iYXd] + 2 * A*C*state_covmat[iXdZd] + 2 * A*B*state_covmat[iXdYd] + Asq*state_covmat[iXdXd] + 2 * D*F*state_covmat[iXZ] + 2 * C*D*state_covmat[iXZd] + 2 * D*E*state_covmat[iXY] + 2 * B*D*state_covmat[iXYd] + Dsq*state_covmat[iXX] + 2 * A*D*state_covmat[iXXd];
    residCovmat[iRdAz] = F*I*state_covmat[iZZ] + C*I*state_covmat[iZZd] + B*I*state_covmat[iZYd] + A*I*state_covmat[iZXd] + (E*I + F*H)*state_covmat[iYZ] + C*H*state_covmat[iYZd] + E*H*state_covmat[iYY] + B*H*state_covmat[iYYd] + A*H* state_covmat[iYXd] + (D*I + F*G)*state_covmat[iXZ] + C*G*state_covmat[iXZd] + (D*H + E*G)*state_covmat[iXY] + B*G*state_covmat[iXYd] + D*G*state_covmat[iXX] + A*G*state_covmat[iXXd];
    residCovmat[iRdEl] = F*L*state_covmat[iZZ] + C*L*state_covmat[iZZd] + B*L*state_covmat[iZYd] + A*L*state_covmat[iZXd] + (E*L + F*K)*state_covmat[iYZ] + C*K*state_covmat[iYZd] + E*K*state_covmat[iYY] + B*K*state_covmat[iYYd] + A*K* state_covmat[iYXd] + (D*L + F*J)*state_covmat[iXZ] + C*J*state_covmat[iXZd] + (D*K + E*J)*state_covmat[iXY] + B*J*state_covmat[iXYd] + D*J*state_covmat[iXX] + A*J*state_covmat[iXXd];
    residCovmat[iAzAz] = Isq*state_covmat[iZZ] + 2 * H*I*state_covmat[iYZ] + Hsq*state_covmat[iYY] + 2 * G*I*state_covmat[iXZ] + 2 * G*H*state_covmat[iXY] + Gsq*state_covmat[iXX];
    residCovmat[iAzEl] = I*L*state_covmat[iZZ] + (H*L + I*K)*state_covmat[iYZ] + H*K*state_covmat[iYY] + (G*L + I*J)*state_covmat[iXZ] + (G*K + H*J)*state_covmat[iXY] + G*J*state_covmat[iXX];
    residCovmat[iElEl] = Lsq*state_covmat[iZZ] + 2 * K*L*state_covmat[iYZ] + Ksq*state_covmat[iYY] + 2 * J*L*state_covmat[iXZ] + 2 * J*K*state_covmat[iXY] + Jsq*state_covmat[iXX];
    /*  resid_covma = residCovmat + measCovVec. */
    residCovmat[iRR] += measCovVec[iRANGE];
    residCovmat[iRdRd] += measCovVec[iRANGE_RATE];
    residCovmat[iAzAz] += measCovVec[iCOS_ELEV_SIN_AZIM];
    residCovmat[iElEl] += measCovVec[iSIN_ELEV];
#endif

}


/**
 *  @b Description
 *  @n
 *      This function performs the (near-optimal) kalman gain computation.
 *      i.e. in matlab 
 *          kalmanGain = state_covmat*transpose(hMat)*inv(residCovmat)
 *
 *  @param[out]  kalmanGain     kalman gain matrix.    
 *  @param[in]   state_covmat   state covariance matrix. 
 *  @param[in]   hMat           (observation matrix) Hmat
 *  @param[in]   measCovVec     inverse of the residual covMat. 
 *  @param[in]  inst            KFtrackerInstance_t object (for 
 *                              scratchpad.)
 *  @retval
 *      Not Applicable.
 */
void kalmanGainComputation(float * restrict kalmanGain, 
                           float const * restrict state_covmat, 
                           float const * restrict hMat, 
                           float const * restrict invResidCovmat, 
                           KFtrackerInstance_t * inst)  
{

    float * restrict temp = inst->kalmanGainTemp;

#if KF_2D
    float   A = hMat[0];
    float     B = hMat[1];
    float     C = hMat[2];
    float     D = hMat[3];
    float     E = hMat[4];
    float     F = hMat[5];
    /* a.  temp = transpose(hMat)*inv(residCovmat)*/
    temp[0] = A*invResidCovmat[iRR] + C*invResidCovmat[iRRd] + E*invResidCovmat[iRAz];
    temp[1] = B*invResidCovmat[iRR] + D*invResidCovmat[iRRd] + F*invResidCovmat[iRAz];
    temp[2] = A*invResidCovmat[iRRd];
    temp[3] = B*invResidCovmat[iRRd];
    temp[4] = C*invResidCovmat[iRdRd] + E*invResidCovmat[iRdAz] + A*invResidCovmat[iRRd];
    temp[5] = D*invResidCovmat[iRdRd] + F*invResidCovmat[iRdAz] + B*invResidCovmat[iRRd];
    temp[6] = A*invResidCovmat[iRdRd];
    temp[7] = B*invResidCovmat[iRdRd];
    temp[8] = C*invResidCovmat[iRdAz] + A*invResidCovmat[iRAz] + E*invResidCovmat[iAzAz];
    temp[9] = D*invResidCovmat[iRdAz] + B*invResidCovmat[iRAz] + F*invResidCovmat[iAzAz];
    temp[10] = A*invResidCovmat[iRdAz];
    temp[11] = B*invResidCovmat[iRdAz];

    /* b.  kalmanGain = state_covmat*temp  */
    kalmanGain[0] = state_covmat[iXYd] * temp[3] + state_covmat[iXXd] * temp[2] + state_covmat[iXY] * temp[1] + state_covmat[iXX] * temp[0];
    kalmanGain[1] = state_covmat[iYYd] * temp[3] + state_covmat[iYXd] * temp[2] + state_covmat[iYY] * temp[1] + state_covmat[iXY] * temp[0];
    kalmanGain[2] = state_covmat[iXdYd] * temp[3] + state_covmat[iXdXd] * temp[2] + state_covmat[iYXd] * temp[1] + state_covmat[iXXd] * temp[0];
    kalmanGain[3] = state_covmat[iYdYd] * temp[3] + state_covmat[iXdYd] * temp[2] + state_covmat[iYYd] * temp[1] + state_covmat[iXYd] * temp[0];
    kalmanGain[4] = state_covmat[iXYd] * temp[7] + state_covmat[iXXd] * temp[6] + state_covmat[iXY] * temp[5] + state_covmat[iXX] * temp[4];
    kalmanGain[5] = state_covmat[iYYd] * temp[7] + state_covmat[iYXd] * temp[6] + state_covmat[iYY] * temp[5] + state_covmat[iXY] * temp[4];
    kalmanGain[6] = state_covmat[iXdYd] * temp[7] + state_covmat[iXdXd] * temp[6] + state_covmat[iYXd] * temp[5] + state_covmat[iXXd] * temp[4];
    kalmanGain[7] = state_covmat[iYdYd] * temp[7] + state_covmat[iXdYd] * temp[6] + state_covmat[iYYd] * temp[5] + state_covmat[iXYd] * temp[4];
    kalmanGain[8] = state_covmat[iXY] * temp[9] + state_covmat[iXX] * temp[8] + state_covmat[iXYd] * temp[11] + state_covmat[iXXd] * temp[10];
    kalmanGain[9] = state_covmat[iYY] * temp[9] + state_covmat[iXY] * temp[8] + state_covmat[iYYd] * temp[11] + state_covmat[iYXd] * temp[10];
    kalmanGain[10] = state_covmat[iYXd] * temp[9] + state_covmat[iXXd] * temp[8] + state_covmat[iXdYd] * temp[11] + state_covmat[iXdXd] * temp[10];
    kalmanGain[11] = state_covmat[iYYd] * temp[9] + state_covmat[iXYd] * temp[8] + state_covmat[iYdYd] * temp[11] + state_covmat[iXdYd] * temp[10];

#elif  KF_3D
    float   A = hMat[0];
    float     B = hMat[1];
    float     C = hMat[2];
    float     D = hMat[3];
    float     E = hMat[4];
    float     F = hMat[5];
    float     G = hMat[6];
    float     H = hMat[7];
    float     I = hMat[8];
    float     J = hMat[9];
    float     K = hMat[10];
    float     L = hMat[11];
    /* a.  temp = transpose(hMat)*inv(residCovmat)*/
    temp[0] = A*invResidCovmat[iRR] + D*invResidCovmat[iRRd] + J*invResidCovmat[iREl] + G*invResidCovmat[iRAz];
    temp[1] = B*invResidCovmat[iRR] + E*invResidCovmat[iRRd] + K*invResidCovmat[iREl] + H*invResidCovmat[iRAz];
    temp[2] = C*invResidCovmat[iRR] + F*invResidCovmat[iRRd] + L*invResidCovmat[iREl] + I*invResidCovmat[iRAz];
    temp[3] = A*invResidCovmat[iRRd];
    temp[4] = B*invResidCovmat[iRRd];
    temp[5] = C*invResidCovmat[iRRd];
    temp[6] = D*invResidCovmat[iRdRd] + J*invResidCovmat[iRdEl] + G*invResidCovmat[iRdAz] + A*invResidCovmat[iRRd];
    temp[7] = E*invResidCovmat[iRdRd] + K*invResidCovmat[iRdEl] + H*invResidCovmat[iRdAz] + B*invResidCovmat[iRRd];
    temp[8] = F*invResidCovmat[iRdRd] + L*invResidCovmat[iRdEl] + I*invResidCovmat[iRdAz] + C*invResidCovmat[iRRd];
    temp[9] = A*invResidCovmat[iRdRd];
    temp[10] = B*invResidCovmat[iRdRd];
    temp[11] = C*invResidCovmat[iRdRd];
    temp[12] = D*invResidCovmat[iRdAz] + A*invResidCovmat[iRAz] + J*invResidCovmat[iAzEl] + G*invResidCovmat[iAzAz];
    temp[13] = E*invResidCovmat[iRdAz] + B*invResidCovmat[iRAz] + K*invResidCovmat[iAzEl] + H*invResidCovmat[iAzAz];
    temp[14] = F*invResidCovmat[iRdAz] + C*invResidCovmat[iRAz] + L*invResidCovmat[iAzEl] + I*invResidCovmat[iAzAz];
    temp[15] = A*invResidCovmat[iRdAz];
    temp[16] = B*invResidCovmat[iRdAz];
    temp[17] = C*invResidCovmat[iRdAz];
    temp[18] = D*invResidCovmat[iRdEl] + A*invResidCovmat[iRAz] + J*invResidCovmat[iElEl] + G*invResidCovmat[iAzEl];
    temp[19] = E*invResidCovmat[iRdEl] + B*invResidCovmat[iRAz] + K*invResidCovmat[iElEl] + H*invResidCovmat[iAzEl];
    temp[20] = F*invResidCovmat[iRdEl] + C*invResidCovmat[iRAz] + L*invResidCovmat[iElEl] + I*invResidCovmat[iAzEl];
    temp[21] = A*invResidCovmat[iRdEl];
    temp[22] = B*invResidCovmat[iRdEl];
    temp[23] = C*invResidCovmat[iRdEl];


    /* b.  kalmanGain = state_covmat*temp  */
    kalmanGain[0] = state_covmat[iXZd] * temp[5] + state_covmat[iXYd] * temp[4] + state_covmat[iXXd] * temp[3] + state_covmat[iXZ] * temp[2] + state_covmat[iXY] * temp[1] + state_covmat[iXX] * temp[0];
    kalmanGain[1] = state_covmat[iYZd] * temp[5] + state_covmat[iYYd] * temp[4] + state_covmat[iYXd] * temp[3] + state_covmat[iYZ] * temp[2] + state_covmat[iYY] * temp[1] + state_covmat[iXY] * temp[0];
    kalmanGain[2] = state_covmat[iZZd] * temp[5] + state_covmat[iZYd] * temp[4] + state_covmat[iZXd] * temp[3] + state_covmat[iZZ] * temp[2] + state_covmat[iYZ] * temp[1] + state_covmat[iXZ] * temp[0];
    kalmanGain[3] = state_covmat[iXdZd] * temp[5] + state_covmat[iXdYd] * temp[4] + state_covmat[iXdXd] * temp[3] + state_covmat[iZXd] * temp[2] + state_covmat[iYXd] * temp[1] + state_covmat[iXXd] * temp[0];
    kalmanGain[4] = state_covmat[iYdZd] * temp[5] + state_covmat[iYdYd] * temp[4] + state_covmat[iXdYd] * temp[3] + state_covmat[iZYd] * temp[2] + state_covmat[iYYd] * temp[1] + state_covmat[iXYd] * temp[0];
    kalmanGain[5] = state_covmat[iZdZd] * temp[5] + state_covmat[iYdZd] * temp[4] + state_covmat[iXdZd] * temp[3] + state_covmat[iZZd] * temp[2] + state_covmat[iYZd] * temp[1] + state_covmat[iXZd] * temp[0];
    kalmanGain[6] = state_covmat[iXXd] * temp[9] + state_covmat[iXZ] * temp[8] + state_covmat[iXY] * temp[7] + state_covmat[iXX] * temp[6] + state_covmat[iXZd] * temp[11] + state_covmat[iXYd] * temp[10];
    kalmanGain[7] = state_covmat[iYXd] * temp[9] + state_covmat[iYZ] * temp[8] + state_covmat[iYY] * temp[7] + state_covmat[iXY] * temp[6] + state_covmat[iYZd] * temp[11] + state_covmat[iYYd] * temp[10];
    kalmanGain[8] = state_covmat[iZXd] * temp[9] + state_covmat[iZZ] * temp[8] + state_covmat[iYZ] * temp[7] + state_covmat[iXZ] * temp[6] + state_covmat[iZZd] * temp[11] + state_covmat[iZYd] * temp[10];
    kalmanGain[9] = state_covmat[iXdXd] * temp[9] + state_covmat[iZXd] * temp[8] + state_covmat[iYXd] * temp[7] + state_covmat[iXXd] * temp[6] + state_covmat[iXdZd] * temp[11] + state_covmat[iXdYd] * temp[10];
    kalmanGain[10] = state_covmat[iXdYd] * temp[9] + state_covmat[iZYd] * temp[8] + state_covmat[iYYd] * temp[7] + state_covmat[iXYd] * temp[6] + state_covmat[iYdZd] * temp[11] + state_covmat[iYdYd] * temp[10];
    kalmanGain[11] = state_covmat[iXdZd] * temp[9] + state_covmat[iZZd] * temp[8] + state_covmat[iYZd] * temp[7] + state_covmat[iXZd] * temp[6] + state_covmat[iZdZd] * temp[11] + state_covmat[iYdZd] * temp[10];
    kalmanGain[12] = state_covmat[iXZd] * temp[17] + state_covmat[iXYd] * temp[16] + state_covmat[iXXd] * temp[15] + state_covmat[iXZ] * temp[14] + state_covmat[iXY] * temp[13] + state_covmat[iXX] * temp[12];
    kalmanGain[13] = state_covmat[iYZd] * temp[17] + state_covmat[iYYd] * temp[16] + state_covmat[iYXd] * temp[15] + state_covmat[iYZ] * temp[14] + state_covmat[iYY] * temp[13] + state_covmat[iXY] * temp[12];
    kalmanGain[14] = state_covmat[iZZd] * temp[17] + state_covmat[iZYd] * temp[16] + state_covmat[iZXd] * temp[15] + state_covmat[iZZ] * temp[14] + state_covmat[iYZ] * temp[13] + state_covmat[iXZ] * temp[12];
    kalmanGain[15] = state_covmat[iXdZd] * temp[17] + state_covmat[iXdYd] * temp[16] + state_covmat[iXdXd] * temp[15] + state_covmat[iZXd] * temp[14] + state_covmat[iYXd] * temp[13] + state_covmat[iXXd] * temp[12];
    kalmanGain[16] = state_covmat[iYdZd] * temp[17] + state_covmat[iYdYd] * temp[16] + state_covmat[iXdYd] * temp[15] + state_covmat[iZYd] * temp[14] + state_covmat[iYYd] * temp[13] + state_covmat[iXYd] * temp[12];
    kalmanGain[17] = state_covmat[iZdZd] * temp[17] + state_covmat[iYdZd] * temp[16] + state_covmat[iXdZd] * temp[15] + state_covmat[iZZd] * temp[14] + state_covmat[iYZd] * temp[13] + state_covmat[iXZd] * temp[12];
    kalmanGain[18] = state_covmat[iXZd] * temp[23] + state_covmat[iXYd] * temp[22] + state_covmat[iXXd] * temp[21] + state_covmat[iXZ] * temp[20] + state_covmat[iXY] * temp[19] + state_covmat[iXX] * temp[18];
    kalmanGain[19] = state_covmat[iYZd] * temp[23] + state_covmat[iYYd] * temp[22] + state_covmat[iYXd] * temp[21] + state_covmat[iYZ] * temp[20] + state_covmat[iYY] * temp[19] + state_covmat[iXY] * temp[18];
    kalmanGain[20] = state_covmat[iZZd] * temp[23] + state_covmat[iZYd] * temp[22] + state_covmat[iZXd] * temp[21] + state_covmat[iZZ] * temp[20] + state_covmat[iYZ] * temp[19] + state_covmat[iXZ] * temp[18];
    kalmanGain[21] = state_covmat[iXdZd] * temp[23] + state_covmat[iXdYd] * temp[22] + state_covmat[iXdXd] * temp[21] + state_covmat[iZXd] * temp[20] + state_covmat[iYXd] * temp[19] + state_covmat[iXXd] * temp[18];
    kalmanGain[22] = state_covmat[iYdZd] * temp[23] + state_covmat[iYdYd] * temp[22] + state_covmat[iXdYd] * temp[21] + state_covmat[iZYd] * temp[20] + state_covmat[iYYd] * temp[19] + state_covmat[iXYd] * temp[18];
    kalmanGain[23] = state_covmat[iZdZd] * temp[23] + state_covmat[iYdZd] * temp[22] + state_covmat[iXdZd] * temp[21] + state_covmat[iZZd] * temp[20] + state_covmat[iYZd] * temp[19] + state_covmat[iXZd] * temp[18];
#endif

}

/**
 *  @b Description
 *  @n
 *      This function performs the state measurement update.
 *      i.e. in matlab 
 *          state = state  + kalmanGain*measResidual
 *
 *  @param[in,out]  state          updated state .    
 *  @param[in]      kalmanGain     kalman gain matrix.    
 *  @param[in]      meas_residual  measurement residual.
 * 
 *  @retval
 *      Not Applicable.
 */
int32_t stateVecMeasurementUpdate(float * restrict state, float const * restrict kalmanGain, float const * restrict meas_residual) {

    float sum;
    int32_t ik, kk;

    /*    state = state  + kalmanGain*meas_residual */
    for (ik = 0; ik < N_STATES; ik++) {
        sum = 0;
        for (kk = 0; kk < N_MEASUREMENTS; kk++) {
            sum += kalmanGain[(kk*N_STATES) + ik] * meas_residual[kk];
        }
        state[ik] += sum;
    }

    /* Nothing can lie behind the radar. */
    if (state[iY] < 0)
    {
        return IS_INVALID;
    }
    else
    {
        return IS_VALID;
    }
}

/**
 *  @b Description
 *  @n
 *      This function performs the state covariance matrix update.
 *      i.e. in matlab 
 *              P = (I - KH)*P      
 *
 *  @param[in,out]  covmat     updated state covariance matrix (P).    
 *  @param[in]      kalmanGain  Kalman gain (K).    
 *  @param[in]      hMat       observation matrix (H).    
 *  @param[in]      KFtrackerInstance_t  scratchpad pointers.
 * 
 *  @retval
 *      Not Applicable.
 */
void stateCovmatMeasurementUpdate(float * restrict covmat, 
                                  float const * restrict const kalmanGain, 
                                  float const * restrict const hMat, 
                                  KFtrackerInstance_t * inst)
{

    int32_t ik, jk, kk;
    float sum;
    float * restrict temp = inst->stateCovMattemp;
    float * restrict tempP = inst->stateCovMattempP;

    /*
     * In the case KF_3D, 'Kalman gain' K is '6 x 4' matrix, the hMat is '4 x 6' matrix. We
     * store the hMat as '4 x 3' matrix. The first matrix multiplication is simply
     * K*H(:,1:3). The second is K*H(2,1:3), which is the same as K*H(:,4:6).*/

     /* First Matrix multiplication.   I - K*H(:,1:3)*/
    for (ik = 0; ik < N_STATES; ik++) {
        for (jk = 0; jk < (N_STATES / 2); jk++) {

            /* If the */
            if (ik == jk) {
                sum = 1.0f;
            }
            else {
                sum = 0.0f;
            }

            for (kk = 0; kk < N_MEASUREMENTS; kk++) {
                sum -= (kalmanGain[(kk*N_STATES) + ik] * hMat[(jk)+kk*(N_STATES / 2)]);
            }

            temp[(ik*N_STATES) + jk] = sum;
        }
    }

    /* Second Matrix multiplication.  I - K*H(:,4:6), however, there is only 1 non-zero row in H(:,4:6), the 2nd row, which is equal to H(1,1:3)*/
    kk = 1;
    for (ik = 0; ik < N_STATES; ik++) {
        for (jk = (N_STATES / 2); jk < N_STATES; jk++) {

            if (ik == jk) {
                sum = 1.0f;
            }
            else {
                sum = 0.0f;
            }

            sum -= kalmanGain[(kk*N_STATES) + (ik)] * hMat[((jk - (N_STATES / 2))) + 0 * (N_STATES / 2)];


            temp[(ik*N_STATES) + jk] = sum;
        }
    }

    // temp*P
#if KF_2D
    tempP[0] = covmat[iXYd] * temp[3] + covmat[iXXd] * temp[2] + covmat[iXY] * temp[1] + covmat[iXX] * temp[0];
    tempP[1] = covmat[iXYd] * temp[7] + covmat[iXXd] * temp[6] + covmat[iXY] * temp[5] + covmat[iXX] * temp[4];
    tempP[2] = covmat[iXY] * temp[9] + covmat[iXX] * temp[8] + covmat[iXYd] * temp[11] + covmat[iXXd] * temp[10];
    tempP[3] = covmat[iXYd] * temp[15] + covmat[iXXd] * temp[14] + covmat[iXY] * temp[13] + covmat[iXX] * temp[12];
    tempP[4] = covmat[iYYd] * temp[7] + covmat[iYXd] * temp[6] + covmat[iYY] * temp[5] + covmat[iXY] * temp[4];
    tempP[5] = covmat[iYY] * temp[9] + covmat[iXY] * temp[8] + covmat[iYYd] * temp[11] + covmat[iYXd] * temp[10];
    tempP[6] = covmat[iYYd] * temp[15] + covmat[iYXd] * temp[14] + covmat[iYY] * temp[13] + covmat[iXY] * temp[12];
    tempP[7] = covmat[iYXd] * temp[9] + covmat[iXXd] * temp[8] + covmat[iXdYd] * temp[11] + covmat[iXdXd] * temp[10];
    tempP[8] = covmat[iXdYd] * temp[15] + covmat[iXdXd] * temp[14] + covmat[iYXd] * temp[13] + covmat[iXXd] * temp[12];
    tempP[9] = covmat[iYdYd] * temp[15] + covmat[iXdYd] * temp[14] + covmat[iYYd] * temp[13] + covmat[iXYd] * temp[12];
#elif KF_3D

    tempP[0] = covmat[iXZd] * temp[5] + covmat[iXYd] * temp[4] + covmat[iXXd] * temp[3] + covmat[iXZ] * temp[2] + covmat[iXY] * temp[1] + covmat[iXX] * temp[0];
    tempP[1] = covmat[iYZd] * temp[5] + covmat[iYYd] * temp[4] + covmat[iYXd] * temp[3] + covmat[iYZ] * temp[2] + covmat[iYY] * temp[1] + covmat[iXY] * temp[0];
    tempP[2] = covmat[iZZd] * temp[5] + covmat[iZYd] * temp[4] + covmat[iZXd] * temp[3] + covmat[iZZ] * temp[2] + covmat[iYZ] * temp[1] + covmat[iXZ] * temp[0];
    tempP[3] = covmat[iXdZd] * temp[5] + covmat[iXdYd] * temp[4] + covmat[iXdXd] * temp[3] + covmat[iZXd] * temp[2] + covmat[iYXd] * temp[1] + covmat[iXXd] * temp[0];
    tempP[4] = covmat[iYdZd] * temp[5] + covmat[iYdYd] * temp[4] + covmat[iXdYd] * temp[3] + covmat[iZYd] * temp[2] + covmat[iYYd] * temp[1] + covmat[iXYd] * temp[0];
    tempP[5] = covmat[iZdZd] * temp[5] + covmat[iYdZd] * temp[4] + covmat[iXdZd] * temp[3] + covmat[iZZd] * temp[2] + covmat[iYZd] * temp[1] + covmat[iXZd] * temp[0];
    tempP[6] = covmat[iYXd] * temp[9] + covmat[iYZ] * temp[8] + covmat[iYY] * temp[7] + covmat[iXY] * temp[6] + covmat[iYZd] * temp[11] + covmat[iYYd] * temp[10];
    tempP[7] = covmat[iZXd] * temp[9] + covmat[iZZ] * temp[8] + covmat[iYZ] * temp[7] + covmat[iXZ] * temp[6] + covmat[iZZd] * temp[11] + covmat[iZYd] * temp[10];
    tempP[8] = covmat[iXdXd] * temp[9] + covmat[iZXd] * temp[8] + covmat[iYXd] * temp[7] + covmat[iXXd] * temp[6] + covmat[iXdZd] * temp[11] + covmat[iXdYd] * temp[10];
    tempP[9] = covmat[iXdYd] * temp[9] + covmat[iZYd] * temp[8] + covmat[iYYd] * temp[7] + covmat[iXYd] * temp[6] + covmat[iYdZd] * temp[11] + covmat[iYdYd] * temp[10];
    tempP[10] = covmat[iXdZd] * temp[9] + covmat[iZZd] * temp[8] + covmat[iYZd] * temp[7] + covmat[iXZd] * temp[6] + covmat[iZdZd] * temp[11] + covmat[iYdZd] * temp[10];
    tempP[11] = covmat[iZZd] * temp[17] + covmat[iZYd] * temp[16] + covmat[iZXd] * temp[15] + covmat[iZZ] * temp[14] + covmat[iYZ] * temp[13] + covmat[iXZ] * temp[12];
    tempP[12] = covmat[iXdZd] * temp[17] + covmat[iXdYd] * temp[16] + covmat[iXdXd] * temp[15] + covmat[iZXd] * temp[14] + covmat[iYXd] * temp[13] + covmat[iXXd] * temp[12];
    tempP[13] = covmat[iYdZd] * temp[17] + covmat[iYdYd] * temp[16] + covmat[iXdYd] * temp[15] + covmat[iZYd] * temp[14] + covmat[iYYd] * temp[13] + covmat[iXYd] * temp[12];
    tempP[14] = covmat[iZdZd] * temp[17] + covmat[iYdZd] * temp[16] + covmat[iXdZd] * temp[15] + covmat[iZZd] * temp[14] + covmat[iYZd] * temp[13] + covmat[iXZd] * temp[12];
    tempP[15] = covmat[iXdZd] * temp[23] + covmat[iXdYd] * temp[22] + covmat[iXdXd] * temp[21] + covmat[iZXd] * temp[20] + covmat[iYXd] * temp[19] + covmat[iXXd] * temp[18];
    tempP[16] = covmat[iYdZd] * temp[23] + covmat[iYdYd] * temp[22] + covmat[iXdYd] * temp[21] + covmat[iZYd] * temp[20] + covmat[iYYd] * temp[19] + covmat[iXYd] * temp[18];
    tempP[17] = covmat[iZdZd] * temp[23] + covmat[iYdZd] * temp[22] + covmat[iXdZd] * temp[21] + covmat[iZZd] * temp[20] + covmat[iYZd] * temp[19] + covmat[iXZd] * temp[18];
    tempP[18] = covmat[iYdZd] * temp[29] + covmat[iYdYd] * temp[28] + covmat[iXdYd] * temp[27] + covmat[iZYd] * temp[26] + covmat[iYYd] * temp[25] + covmat[iXYd] * temp[24];
    tempP[19] = covmat[iZdZd] * temp[29] + covmat[iYdZd] * temp[28] + covmat[iXdZd] * temp[27] + covmat[iZZd] * temp[26] + covmat[iYZd] * temp[25] + covmat[iXZd] * temp[24];
    tempP[20] = covmat[iZdZd] * temp[35] + covmat[iYdZd] * temp[34] + covmat[iXdZd] * temp[33] + covmat[iZZd] * temp[32] + covmat[iYZd] * temp[31] + covmat[iXZd] * temp[30];
#endif

    /* Copy it back. */
    for (ik = 0; ik < N_UNIQ_ELEM_IN_SYM_COVMAT; ik++) {
        covmat[ik] = tempP[ik];
    }
}

/**
 *  @b Description
 *  @n
 *      This function selects the unassociated measurements that are to be used to create the  new 
 *   tracks. 
 *
 *  @param[out]  selectedMeas           The unassociated measurements selected for tracking.    
 *  @param[in]   measArray              List of input measurements 
 *  @param[in]   assocMeasIndx          An array declaring whether a measurement has
 *                                      been associated or not. 
 *  @param[in]   nFree                  Number of free tracks. 
 *  @param[in]   numUnassociatedMeas    Number of unassociated measurements.
 *  @param[in]   numMeasTotal.          Total number of measurements. 
 * 
 *  @retval
 *      number of selected measurements. .
 */
int32_t selectMeas(int16_t * restrict selectedMeas, 
                   trackingInputReport_t const * restrict measArray, 
                   int16_t * restrict assocMeasIndx, 
                   const int32_t nFree, 
                   const int32_t numUnassociatedMeas, 
                   const int32_t numMeasTotal) 
{
    int32_t iMeas;
    int32_t numSelected = 0;
    /* If the number of new measurements is smaller than the number of free tracks, assign all. 
     * Otherwise divide the new measurements into high SNR and low SNR ones. Then assign high
     * SNR ones first, then assign the low SNR ones. The order of priority for measurements
     * within the high SNR list will be 'first-come-first-serve'. */
    if ( (numUnassociatedMeas == 0) 
            || (numMeasTotal == 0))
    {
        return 0;
    }
    
    if (numUnassociatedMeas > nFree)
    {
        for (iMeas = 0;
            (iMeas < numMeasTotal) && (numSelected < nFree);
            iMeas++)
        {

            if (assocMeasIndx[iMeas] == NOT_ASSOCIATED)
            {
                /* The variance of the range correlates well with SNR. */
                if (measArray[iMeas].measCovVec[iRANGE] < HIGH_SNR_RVAR_THRESH) 
                {
                    selectedMeas[numSelected] = iMeas;
                    numSelected++;
                    assocMeasIndx[iMeas] = IS_ASSOCIATED;
                }
            }
        }
    }


    for (iMeas = 0;
        (iMeas < numMeasTotal) && (numSelected < nFree);
        iMeas++)
    {

        if (assocMeasIndx[iMeas] == NOT_ASSOCIATED)
        {
            selectedMeas[numSelected] = iMeas;
            numSelected++;
        }
    }

    return numSelected;
}

/**
 *  @b Description
 *  @n
 *      This function creates new tracks.
 *
 *  @param[in, out]   dataPathObj            Data path object - for the 'trackerState' struct array.    
 *  @param[in]   measArray              List of input measurements 
 *  @param[in]   freeTrackerIndexArray  Number of free tracks. 
 *  @param[in]   selectedIndxArr        An array of the selected measurements for new tracks.
 *  @param[in]   numSelected.           number of selected measurements.  
 * 
 *  @retval
 *      not applicable
 */
void createNewTracks(DSS_DataPathObj * restrict dataPathObj, 
                     trackingInputReport_t const * restrict measArray, 
                     int16_t const * restrict freeTrackerIndxArray, 
                     int16_t const * restrict selectedIndxArr, 
                     const int32_t numSelected)
{

    trackingInputReport_t const * restrict currMeas;
    KFstate_t * restrict currTrack;
    int32_t iTrack;
    int32_t freeTrackIndx;
    int32_t selectedMeasIndx; 


    for (iTrack = 0; iTrack < numSelected; iTrack++) 
    {
        freeTrackIndx = freeTrackerIndxArray[iTrack]; 
        selectedMeasIndx = selectedIndxArr[iTrack];
        currTrack = &(dataPathObj->trackerState[freeTrackIndx]);
        currMeas = &(measArray[selectedMeasIndx]);

        initNewTracker(currTrack, currMeas);
        currTrack->age = 0; 
        currTrack->validity = IS_VALID; 
        currTrack->tick = 0;
        currTrack->xSize = currMeas->xSize;
        currTrack->ySize = currMeas->ySize;        
    }
}

/**
 *  @b Description
 *  @n
 *      This function initialzes a new tracker.
 *
 *  @param[out]   obj           pointer to the new tracker's state.    
 *  @param[in]   meas           set of measurements to initialize the tracker.  
 * 
 *  @retval
 *      not applicable
 */
void initNewTracker(KFstate_t* restrict obj, trackingInputReport_t const * restrict meas)
{
    int32_t w_ik;
#if KF_2D
    obj->vec[iX] = meas->measVec[iRANGE] * meas->measVec[iSIN_AZIM];
    obj->vec[iY] = sqrtsp((meas->measVec[iRANGE] * meas->measVec[iRANGE]) - (obj->vec[iX] * obj->vec[iX]));

    obj->vec[iXd] = 0;
    obj->vec[iYd] = meas->measVec[iRANGE_RATE];

    for (w_ik = 0; w_ik < N_UNIQ_ELEM_IN_SYM_COVMAT; w_ik++) {
        obj->covmat[w_ik] = 0.0f;
    }

    /* The covariance mat doesn't really matter at init, but we want some reasonabble numbers in it */ 
    obj->covmat[iXX] = (1 + meas->measCovVec[iRANGE]) * (1 + meas->measCovVec[iSIN_AZIM]);
    obj->covmat[iYY] = (1 + meas->measCovVec[iRANGE]) * (1 + meas->measCovVec[iSIN_AZIM]);

    obj->covmat[iXdXd] = (3 * meas->measVec[iRANGE_RATE] * meas->measVec[iRANGE_RATE]) + ((1 + meas->measCovVec[iRANGE_RATE]) * (1 + meas->measCovVec[iSIN_AZIM]));
    obj->covmat[iYdYd] = (3 * meas->measVec[iRANGE_RATE] * meas->measVec[iRANGE_RATE]) + ((1 + meas->measCovVec[iRANGE_RATE]) * (1 + meas->measCovVec[iSIN_AZIM]));

#elif KF_3D
    obj->vec[iX] = meas->measVec[iRANGE] * meas->measVec[iCOS_ELEV_SIN_AZIM];
    obj->vec[iZ] = meas->measVec[iRANGE] * meas->measVec[iSIN_ELEV];
    obj->vec[iY] = sqrtsp((meas->measVec[iRANGE] * meas->measVec[iRANGE]) - ((obj->vec[iX] * obj->vec[iX]) + (obj->vec[iZ] * obj->vec[iZ])));

    obj->vec[iXd] = meas->measVec[iRANGE_RATE] * meas->measVec[iCOS_ELEV_SIN_AZIM];
    obj->vec[iZd] = meas->measVec[iRANGE_RATE] * meas->measVec[iSIN_ELEV];
    obj->vec[iYd] = sqrtsp((meas->measVec[iRANGE_RATE] * meas->measVec[iRANGE_RATE]) - ((obj->vec[iXd] * obj->vec[iXd]) + (obj->vec[iZd] * obj->vec[iZd])));

    for (w_ik = 0; w_ik < N_UNIQ_ELEM_IN_SYM_COVMAT; w_ik++) {
       obj->covmat[w_ik] = 0;
    }

    /* The covariance mat doesn't really matter at init, but we want some reasonabble numbers in it */ 
    obj->covmat[iXX] = (meas->measCovVec[iRANGE] * meas->measCovVec[iCOS_ELEV_SIN_AZIM]) + meas->measCovVec[iRANGE] + meas->measCovVec[iCOS_ELEV_SIN_AZIM];
    obj->covmat[iZZ] = meas->measCovVec[iRANGE] * meas->measCovVec[iSIN_ELEV] + meas->measCovVec[iSIN_ELEV] + meas->measCovVec[iRANGE];
    obj->covmat[iYY] = meas->measCovVec[iRANGE] + obj->covmat[iXX] + obj->covmat[iZZ];

    obj->covmat[iXdXd] = (meas->measCovVec[iRANGE_RATE] * meas->measCovVec[iCOS_ELEV_SIN_AZIM]) + meas->measCovVec[iRANGE_RATE] + meas->measCovVec[iCOS_ELEV_SIN_AZIM];
    obj->covmat[iZdZd] = (meas->measCovVec[iRANGE_RATE] * meas->measCovVec[iSIN_ELEV]) + meas->measCovVec[iRANGE_RATE] + meas->measCovVec[iSIN_ELEV];
    obj->covmat[iYdYd] = meas->measCovVec[iRANGE_RATE] + obj->covmat[iXdXd] + obj->covmat[iZdZd];
#endif
}

/**
 *  @b Description
 *  @n
 *      This function selects a 'process noise matrix' based on the age of the tracker. If
 *     the tracker is new, then the 'process noise' is assumed to be high, so as to allow 
 *     faster convergence. If the tracker is old, the process noise is computed as per the 
 *     the unmodeled parameters. 
 *     We also allow higher 'process noise' for closer objects, assuming that their higher 
 *     SNR will compensate for the noisier measurements.
 *
 *  @param[in]   QvecList      3 arrays [N_STATES] long. A list of possible Qvec matrices. 
 *  @param[in]   tick          the number of valid association of the tracker.   
 *  @param[in]   age           the age of the tracker (i.e. the number of ticks before the 
 *                             last associated measurement).    
 *  @param[in]   range         the distance of the target being tracked.   
 * 
 *  @retval
 *      pointer to the 'process noise diagonal'
 */
float * select_QVec(float const * restrict QvecList, uint8_t tick, uint8_t age, const float range)
{
    float * Qvec = (float *)QvecList;
    /* The QvecList comprises of 3 different QvecArrays.
     * The first two are used to aid convergence. The
     * third represents the regular unmodeled acceleration of the
     * model.  */

    if ((tick < 2) || (range < 10.0f) || (age > 1))
    {
        Qvec = Qvec;
    }
    else if ((tick < 4) || (range < 15.0f) || (age > 0))
    {
        Qvec = Qvec + N_STATES;
    }
    else
    {
        Qvec = Qvec + 2 * N_STATES;
    }

    return Qvec;
}


/**
 *  @b Description
 *  @n
 *      This function validates a state by checking whether it is within a specific bounding box. 
 *    For e.g  if an object is behind the radar, there is no need to track it. 
 *
 *  @param[out]  currTrack       pointer to the  tracker's state.    
 *  @param[in]   dataPathObj   data path object -> for max range.  
 * 
 *  @retval
 *      true/false
 */
uint32_t isWithinBoundingBox(KFstate_t* restrict currTrack, DSS_DataPathObj * restrict dataPathObj)
{
    int32_t isValid = 0;
    float maxRange = ((float)dataPathObj->maxRange) * dataPathObj->invOneQFormat; 

    
    if ((currTrack->vec[iX] > -maxRange) && (currTrack->vec[iX] < maxRange))
    {
        isValid = IS_VALID; 
    }
    else
    {
        return IS_INVALID;
    }

    if ((currTrack->vec[iY] < maxRange) && (currTrack->vec[iY] > 0.0f))
    {
        isValid = IS_VALID; 
    }
    else
    {
        return IS_INVALID;
    }
    
    /* The following code checks whether the state or the state covariance matrix has any NaNs, 
     * which should 'never ever' happen. But 'never ever' is a too long time to be that confident. 
     */
    {
        int32_t ik;
        for (ik = 0; ik < N_STATES; ik++)
        {
            if (isnan(currTrack->vec[ik]))
            {
                return IS_INVALID;
            }
        }

        for (ik = 0; ik < N_UNIQ_ELEM_IN_SYM_COVMAT; ik++)
        {
            if (isnan(currTrack->covmat[ik]))
            {
                return IS_INVALID;    
            }
        }        
    }
    
    return isValid;
}

/**
 *  @b Description
 *  @n
 *      This function invalidates a track by marking it as invalid, marking its index as invalid, 
 *    incrementing the number of free tracks. 
 *
 *  @param[in,out]  currTrack       pointer to the  tracker's state.    
 *  @param[in,out]  freeTrackerIndxArray   List of unassociated trackers.  
 *  @param[in]      nFree        current number of free trackers.  
 *  @param[in]      iTrack       current track Index.  
 * 
 *  @retval
 *      updated number of free trackers. 
 */
int32_t invalidateCurrTrack(KFstate_t* restrict currTrack, int16_t * restrict freeTrackerIndxArray, int32_t nFree, int32_t iTrack)
{
    currTrack->validity  =  IS_INVALID;
    freeTrackerIndxArray[nFree] = iTrack;
    nFree++;
    
    return nFree;
}
