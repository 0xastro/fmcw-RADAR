/*!
 *  \file   EKF_XYZ_Consts.h
 *
 *  \brief   Constants for the Extended Kalman Filter.
 *
 *  Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/
 *
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
 *
*/

#ifndef EKF_XYZ_CONSTS_H
#define EKF_XYZ_CONSTS_H

/* Universal Constants*/
#define EPS 0.00001

#define KF_2D 1
#define KF_3D 0

#if KF_2D /* The states are x, y, vx, vy*/
/* Measurement indices */
#define iRANGE 0           /* Index of Range (meters)*/
#define iRANGE_RATE 1      /* Index of Range rate (meters/sec)*/
#define iSIN_AZIM 2        /* Index of sin(azim) (x/r)*/

/* State indices  */
#define iX 0               /* Index of x-coord  (meters)*/
#define iY 1               /* Index of y-coord  (meters)*/
#define iXd 2              /* Index of Speed in x direction (meters/sec)*/
#define iYd 3              /* Index of Speed in y direction (meters/sec)*/


#define N_STATES 4
#define N_MEASUREMENTS 3

#define N_UNIQ_ELEM_IN_HMAT 6
#elif KF_3D
/* Measurement indices.*/
#define iRANGE 0           /* Index of Range (meters)*/
#define iRANGE_RATE 1      /* Index of Range rate (m/s)*/
#define iCOS_ELEV_SIN_AZIM 2 /* Index of cos(elev)sin(azim) (x/r)*/
#define iSIN_ELEV 3        /* Index of sin(elev)(z/r)*/

/* State indices. */
#define iX 0
#define iY 1
#define iZ 2
#define iXd 3
#define iYd 4
#define iZd 5

#define N_STATES 6
#define N_MEASUREMENTS 4

#define N_UNIQ_ELEM_IN_HMAT 12

#endif

/* The state covariance matrix is stored as a linear vector of size N_UNIQ_ELEM_IN_SYM_COVMAT */
#define N_UNIQ_ELEM_IN_SYM_COVMAT (N_STATES*(N_STATES+1)/2)
#define N_UNIQ_ELEM_IN_SYM_RESIDCOVMAT (N_MEASUREMENTS*(N_MEASUREMENTS+1)/2)

/* The translation of the indices of the covariance matrix  to the indices of the linear vector*/
#if KF_2D
#define iXX          iX
#define iXY          (iXX + 1)
#define iXXd  (iXY + 1)
#define iXYd  (iXXd + 1)

#define iYY          (iXYd + 1)
#define iYXd  (iYY + 1)
#define iYYd  (iYXd + 1)

#define iXdXd (iYYd + 1)
#define iXdYd (iXdXd + 1)

#define iYdYd (iXdYd + 1)

#elif KF_3D
#define iXX          iX
#define iXY          (iXX + 1)
#define iXZ          (iXY + 1)

#define iXXd  (iXZ + 1)
#define iXYd  (iXXd + 1)
#define iXZd  (iXYd + 1)

#define iYY          (iXZd + 1)
#define iYZ          (iYY + 1)

#define iYXd  (iYZ + 1)
#define iYYd  (iYXd + 1)
#define iYZd  (iYYd + 1)

#define iZZ          (iYZd + 1)

#define iZXd  (iZZ + 1)
#define iZYd  (iZXd + 1)
#define iZZd  (iZYd + 1)

#define iXdXd (iZZd + 1)
#define iXdYd (iXdXd + 1)
#define iXdZd (iXdYd + 1)

#define iYdYd (iXdZd + 1)
#define iYdZd (iYdYd + 1)

#define iZdZd (iYdZd + 1)
#endif

/* The translation of the indices of the residual covariance matrix  to the indices of the linear vector*/
#if KF_2D

#define iRR   iRANGE    
#define iRRd  (iRR + 1)
#define iRAz  (iRRd + 1)

#define iRdRd (iRAz + 1)
#define iRdAz (iRdRd + 1)

#define iAzAz (iRdAz + 1)

#elif KF_3D

#define iRR   iRANGE
#define iRRd  (iRR + 1)
#define iRAz  (iRRd + 1)
#define iREl  (iRAz + 1)

#define iRdRd (iREl  + 1)
#define iRdAz (iRdRd + 1)
#define iRdEl  (iRdAz + 1)

#define iAzAz (iRdEl + 1)
#define iAzEl (iAzAz + 1)

#define iElEl (iAzEl + 1)

#endif

/* Some #defines for use within the tracker.*/
/** @brief  object has not been associated. */
#define NOT_ASSOCIATED  (-1)
/** @brief  object has been associated. */
#define IS_ASSOCIATED   (1)
/** @brief  object is invalid. */
#define IS_INVALID      (0)
/** @brief  object is valid. */
#define IS_VALID        (1)

/** @brief  Unmodelled acceleration parameter in the Y direction (perpendicular to the antenna). */
#define MAX_ACCEL_Y_M_P_SECSQ 12 /* m/s^2*/
/** @brief  Unmodelled acceleration parameter in the X direction (parallel to the antenna). */
#define MAX_ACCEL_X_M_P_SECSQ 5 /* m/s^2*/

/* Thresholds. */
/** @brief Delete objects that haven't been associated after these many ticks. */
#define AGED_OBJ_DELETION_THRESH 10     
/** @brief Objects with range variance lower than this are considered to be high SNR tracks. */
#define HIGH_SNR_RVAR_THRESH 6  


#endif
