/**
 *   @file  gtrack_utilities_3d.c
 *
 *   @brief
 *      This is a set of 2d utilities functions used by GTRACK Algorithm
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

#include <ti/alg/gtrack/gtrack.h>
#include <ti/alg/gtrack/include/gtrack_int.h>

#ifdef GTRACK_3D

#define PI 3.14159265358979323846f
#define RAD2DEG (180.f/PI)

const float pInit[] = {0.f, 0.f, 0.f, 0.5f, 0.5f, 0.5f, 1.f, 1.f, 1.f};
const float spreadInit[] = {1.0f, 5*PI/180.f, 5*PI/180.f, 1.0f};
const float spreadMin[] = {0.1f, PI/180.f, PI/180.f, 0.5f};

/**
*  @b Description
*  @n
*		This function is used to convert a vector from sherical to cartesian
*
*  @param[in]  v
*		Pointer to measurements (spheriacl form) vector
*  @param[out]  c
*		Pointer to state vector (cartesian form)
*
*  \ingroup GTRACK_ALG_UTILITY_FUNCTION
*
*  @retval
*      None
*/
void gtrack_sph2cart(GTRACK_measurement_vector *v, GTRACK_cartesian_position *c)
{
    float sinAzim, cosAzim;
    float sinElev, cosElev;

    gtrack_sincosd(v->azimuth*RAD2DEG,&sinAzim, &cosAzim);
    gtrack_sincosd(v->elev*RAD2DEG,&sinElev, &cosElev);

    c->posX = v->range*cosElev*sinAzim;
    c->posY = v->range*cosElev*cosAzim;
    c->posZ = v->range*sinElev;
}

void gtrack_calcMeasurementVar(GTRACK_measurement_vector *v, GTRACK_varParams *varP, GTRACK_measurement_vector *var)
{
    float azimErr;
    float elevErr;
    
    var->range = varP->depthStd*varP->depthStd;
    azimErr = 2*atanf(0.5f*varP->widthStd/v->range);
    elevErr = 2*atanf(0.5f*varP->heightStd/v->range);
    var->azimuth = azimErr*azimErr;
    var->elev = elevErr*elevErr;
    var->doppler = varP->dopplerStd*varP->dopplerStd;
}

/**
*  @b Description
*  @n
*		This function computes target dimension estimations based on estimated measurement spread and centroid range
*		Matrix is real, single precision floating point.
*		Matrix is in row-major order
*
*  @param[in]  mSpread
*		Vector S
*  @param[in]  R
*		scalar Range
*  @param[out]  tDim
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_calcDim(float *mSpread, float R, float *tDim)
{
    tDim[0] = mSpread[0];
    tDim[1] = 2*R*tanf(mSpread[1]/2);
    tDim[2] = 2*R*tanf(mSpread[2]/2);
    tDim[3] = mSpread[3];
}


float gtrack_calcDistance(GTRACK_measurement_vector *p1, GTRACK_measurement_vector *p2)
{
    //  d = u1(1)^2 + u2(1)^2 - 2*u1(1)*u2(1)*(cos(u1(3))*cos(u2(3))*cos(u1(2)-u2(2))+sin(u1(3))*sin(u2(3)));
    float sinAzim, cosAzim;
    float sinElev1, cosElev1;
    float sinElev2, cosElev2;

    gtrack_sincosd((p1->azimuth-p2->azimuth)*RAD2DEG,&sinAzim, &cosAzim);
    gtrack_sincosd(p1->elev*RAD2DEG,&sinElev1, &cosElev1);
    gtrack_sincosd(p2->elev*RAD2DEG,&sinElev2, &cosElev2);

    return p1->range*p1->range + p2->range*p2->range - 2*p1->range*p2->range*(cosElev1*cosElev2*cosAzim+sinElev1*sinElev2);
}

void gtrack_computeMahalanobis(float *d, float *S, float *chi2)
{
	*chi2 = 
		d[0]*(d[0]*S[0]+d[1]*S[4]+d[2]*S[8]+d[3]*S[12])+
		d[1]*(d[0]*S[1]+d[1]*S[5]+d[2]*S[9]+d[3]*S[13])+
		d[2]*(d[0]*S[2]+d[1]*S[6]+d[2]*S[10]+d[3]*S[14])+
		d[3]*(d[0]*S[3]+d[1]*S[7]+d[2]*S[11]+d[3]*S[15]);
}

/**
*  @b Description
*  @n
*		This function is used to multiply two matrices of the size 9x9. 
*		Matrices are all real, single precision floating point.
*		Matrices are in row-major order
*
*  @param[in]  A
*		Matrix A
*  @param[in]  B
*		Matrix B
*  @param[out]  C
*		Matrix C(9x9) = A(9x9) X B(9x9)
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/

void gtrack_matrixMultiply66(float *A, float *B, float *C)
{
	uint16_t i,j;
    for (i = 0U; i < 36U; i += 6U) 
	{
        for (j = 0U; j < 6U; j++) {
            C[i + j] = 	(A[i + 0U] * B[j +  0U]) +
						(A[i + 1U] * B[j +  6U]) +
                        (A[i + 2U] * B[j + 12U]) +
                        (A[i + 3U] * B[j + 18U]) +
                        (A[i + 4U] * B[j + 24U]) +
                        (A[i + 5U] * B[j + 30U]);	
		}
	}
}

/**
*  @b Description
*  @n
*		This function is used to multiply two matrices of size 9x9. 
*		Second Matrix is getting transposed first
*		Matrices are all real, single precision floating point.
*		Matrices are in row-major order
*
*  @param[in]  A
*		Matrix A
*  @param[in]  B
*		Matrix B
*  @param[out]  C
*		Matrix C(9x9) = A(9x9) X B(9x9)T
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/

void gtrack_matrixMultiply66T(float *A, float *B, float *C)
{
	uint16_t i,j, k;
    for (i = 0; i < 36; i += 6) 
	{
        for (j = 0U, k = 0U; j < 6U; j++, k+=6U) {
            C[i + j] = 	(A[i + 0U] * B[k + 0U]) +
						(A[i + 1U] * B[k + 1U]) +
                        (A[i + 2U] * B[k + 2U]) +
                        (A[i + 3U] * B[k + 3U]) +
                        (A[i + 4U] * B[k + 4U]) +
                        (A[i + 5U] * B[k + 5U]);	
		}
	}
}


/**
*  @b Description
*  @n
*		This function is used to multiply two matrices.
*		First matrix P is of size 9x9, the second one is of the size 4x9.
*		The second matrix is being transposed first.
*		Matrices are all real, single precision floating point.
*		Matrices are in row-major order
*
*  @param[in]  P
*		Matrix P
*  @param[in]  J
*		Matrix J
*  @param[out]  PJ
*		Matrix PJ = P(9,9) X J(4,9)T
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/

void gtrack_matrixComputePJT(float *P, float *J, float *PJ)
{
	/* We compute PJ' first because it is re-used in Kalman Gain calculations */
	uint16_t i,j, k, n;
	/* PJ[9x4] = P[9x9] x J[4x9]' */
	for (i = 0U, k = 0U; i < 36U; i+= 4U, k+=9U)
	{
		for (j = 0U, n = 0U; j < 4U; j++, n+=9U)
		{
            PJ[i + j] = (P[k + 0U] * J[n + 0U]) +
						(P[k + 1U] * J[n + 1U]) +
                        (P[k + 2U] * J[n + 2U]) +
                        (P[k + 3U] * J[n + 3U]) +
                        (P[k + 4U] * J[n + 4U]) +
                        (P[k + 5U] * J[n + 5U]) +
                        (P[k + 6U] * J[n + 6U]) +
                        (P[k + 7U] * J[n + 7U]) +
                        (P[k + 8U] * J[n + 8U]);	
		}
	}
}


void gtrack_matrixInv(const float *A, float *det_out, float *B)
{
    float inv[16];
    float det;
    float invdet;
    int i;

    inv[0] = A[5]  * A[10] * A[15] - 
             A[5]  * A[11] * A[14] - 
             A[9]  * A[6]  * A[15] + 
             A[9]  * A[7]  * A[14] +
             A[13] * A[6]  * A[11] - 
             A[13] * A[7]  * A[10];

    inv[4] = -A[4]  * A[10] * A[15] + 
              A[4]  * A[11] * A[14] + 
              A[8]  * A[6]  * A[15] - 
              A[8]  * A[7]  * A[14] - 
              A[12] * A[6]  * A[11] + 
              A[12] * A[7]  * A[10];

    inv[8] = A[4]  * A[9] * A[15] - 
             A[4]  * A[11] * A[13] - 
             A[8]  * A[5] * A[15] + 
             A[8]  * A[7] * A[13] + 
             A[12] * A[5] * A[11] - 
             A[12] * A[7] * A[9];

    inv[12] = -A[4]  * A[9] * A[14] + 
               A[4]  * A[10] * A[13] +
               A[8]  * A[5] * A[14] - 
               A[8]  * A[6] * A[13] - 
               A[12] * A[5] * A[10] + 
               A[12] * A[6] * A[9];

    inv[1] = -A[1]  * A[10] * A[15] + 
              A[1]  * A[11] * A[14] + 
              A[9]  * A[2] * A[15] - 
              A[9]  * A[3] * A[14] - 
              A[13] * A[2] * A[11] + 
              A[13] * A[3] * A[10];

    inv[5] = A[0]  * A[10] * A[15] - 
             A[0]  * A[11] * A[14] - 
             A[8]  * A[2] * A[15] + 
             A[8]  * A[3] * A[14] + 
             A[12] * A[2] * A[11] - 
             A[12] * A[3] * A[10];

    inv[9] = -A[0]  * A[9] * A[15] + 
              A[0]  * A[11] * A[13] + 
              A[8]  * A[1] * A[15] - 
              A[8]  * A[3] * A[13] - 
              A[12] * A[1] * A[11] + 
              A[12] * A[3] * A[9];

    inv[13] = A[0]  * A[9] * A[14] - 
              A[0]  * A[10] * A[13] - 
              A[8]  * A[1] * A[14] + 
              A[8]  * A[2] * A[13] + 
              A[12] * A[1] * A[10] - 
              A[12] * A[2] * A[9];

    inv[2] = A[1]  * A[6] * A[15] - 
             A[1]  * A[7] * A[14] - 
             A[5]  * A[2] * A[15] + 
             A[5]  * A[3] * A[14] + 
             A[13] * A[2] * A[7] - 
             A[13] * A[3] * A[6];

    inv[6] = -A[0]  * A[6] * A[15] + 
              A[0]  * A[7] * A[14] + 
              A[4]  * A[2] * A[15] - 
              A[4]  * A[3] * A[14] - 
              A[12] * A[2] * A[7] + 
              A[12] * A[3] * A[6];

    inv[10] = A[0]  * A[5] * A[15] - 
              A[0]  * A[7] * A[13] - 
              A[4]  * A[1] * A[15] + 
              A[4]  * A[3] * A[13] + 
              A[12] * A[1] * A[7] - 
              A[12] * A[3] * A[5];

    inv[14] = -A[0]  * A[5] * A[14] + 
               A[0]  * A[6] * A[13] + 
               A[4]  * A[1] * A[14] - 
               A[4]  * A[2] * A[13] - 
               A[12] * A[1] * A[6] + 
               A[12] * A[2] * A[5];

    inv[3] = -A[1] * A[6] * A[11] + 
              A[1] * A[7] * A[10] + 
              A[5] * A[2] * A[11] - 
              A[5] * A[3] * A[10] - 
              A[9] * A[2] * A[7] + 
              A[9] * A[3] * A[6];

    inv[7] = A[0] * A[6] * A[11] - 
             A[0] * A[7] * A[10] - 
             A[4] * A[2] * A[11] + 
             A[4] * A[3] * A[10] + 
             A[8] * A[2] * A[7] - 
             A[8] * A[3] * A[6];

    inv[11] = -A[0] * A[5] * A[11] + 
               A[0] * A[7] * A[9] + 
               A[4] * A[1] * A[11] - 
               A[4] * A[3] * A[9] - 
               A[8] * A[1] * A[7] + 
               A[8] * A[3] * A[5];

    inv[15] = A[0] * A[5] * A[10] - 
              A[0] * A[6] * A[9] - 
              A[4] * A[1] * A[10] + 
              A[4] * A[2] * A[9] + 
              A[8] * A[1] * A[6] - 
              A[8] * A[2] * A[5];

    det = A[0] * inv[0] + A[1] * inv[4] + A[2] * inv[8] + A[3] * inv[12];

    invdet = 1.0f / det;

    for (i = 0; i < 16; i++)
        B[i] = inv[i] * invdet;
    
    *det_out = det;
}

void gtrack_matrixDet(float *A, float *det)
{
    float inv[4];

    inv[0] = A[5]  * A[10] * A[15] - 
             A[5]  * A[11] * A[14] - 
             A[9]  * A[6]  * A[15] + 
             A[9]  * A[7]  * A[14] +
             A[13] * A[6]  * A[11] - 
             A[13] * A[7]  * A[10];

    inv[1] = -A[4]  * A[10] * A[15] + 
              A[4]  * A[11] * A[14] + 
              A[8]  * A[6]  * A[15] - 
              A[8]  * A[7]  * A[14] - 
              A[12] * A[6]  * A[11] + 
              A[12] * A[7]  * A[10];

    inv[2] = A[4]  * A[9] * A[15] - 
             A[4]  * A[11] * A[13] - 
             A[8]  * A[5] * A[15] + 
             A[8]  * A[7] * A[13] + 
             A[12] * A[5] * A[11] - 
             A[12] * A[7] * A[9];

    inv[3] = -A[4]  * A[9] * A[14] + 
               A[4]  * A[10] * A[13] +
               A[8]  * A[5] * A[14] - 
               A[8]  * A[6] * A[13] - 
               A[12] * A[5] * A[10] + 
               A[12] * A[6] * A[9];

    *det = A[0] * inv[0] + A[1] * inv[1] + A[2] * inv[2] + A[3] * inv[3];
}

float gtrack_gateCreateLim (float volume, float *EC, float range, float *gateLim)
{
    /* Simplified Gate Construction (no need for SVD) */
	/* We build a gate based on a constant volume */
	/* In addition, we impose a limiter: under no circumstances the gate will */
	/* allow to reach points beyond gateLimits */
	float a,b,c,d, v;
	float gConst;
	float LQ[GTRACK_MEASUREMENT_VECTOR_SIZE*GTRACK_MEASUREMENT_VECTOR_SIZE]; /* Lower triangular decomposition */

	float W[GTRACK_MEASUREMENT_VECTOR_SIZE*GTRACK_MEASUREMENT_VECTOR_SIZE];
	float W_det;
    float sWidth;
	float gLimit;
	float gMin;

	/* Compute unitary volume */
    a = 1/sqrtf(EC[0]);
	b = 1/sqrtf(EC[5]);
	c = 1/sqrtf(EC[10]);
	d = 1/sqrtf(EC[15]);
	v = PI*PI/2*a*b*c*d;


	/* Compute constant volume expansion factor */
	gConst = sqrtf(volume/v);

	/* Compute limiters */
	gtrack_matrixCholesky(GTRACK_MEASUREMENT_VECTOR_SIZE, EC, LQ);

    gtrack_matrixInv(LQ, &W_det, W);

    gMin = gConst;

	/* Orthogonal projections onto Range, Azimuth, Elevation and Doplper dimensions */

	/* Orthogonal projections onto Range axis is the RSS of the first column of W */
	/* s[0]=2*sqrtf(w[0]*w[0] + w[4]*w[4] + w[8]*w[8] + w[12]*w[12]) */
	/* gLimit = (gateLim[0]/s[0])^2=gateLim[0]^2/(4*(w[0]*w[0] + w[4]*w[4] + w[8]*w[8] + w[12]*w[12]) */
	if(gateLim[0] != 0) {
		gLimit = (gateLim[0]*gateLim[0])/(4*(W[0]*W[0] + W[4]*W[4] + W[8]*W[8]+ W[12]*W[12]));
		if(gMin > gLimit)
			gMin = gLimit;
	}

	/* Orthogonal projections onto Azimuth axis is the RSS of the second column of W */
	/* s[1] = 2*sqrtf(w[5]*w[5] + w[9]*w[9] + w[13]*w[13]) */
	/* Converitng Azimuth span to the the width */
	/* sWidth  = 2*Range*tan(0.5*s[1])=2*Range*tan(sqrtf(w[5]*w[5] + w[9]*w[9] + w[13]*w[13])) */
	/* gLimit = (gateLim[0]/sWidth)^2 */
	if(gateLim[1] != 0) {
		sWidth = 2*range*tanf(sqrtf(W[5]*W[5] + W[9]*W[9] + W[13]*W[13]));
		gLimit = (gateLim[1]*gateLim[1])/(sWidth*sWidth);
		if(gMin > gLimit)
			gMin = gLimit;
	}

	/* Orthogonal projections onto Elevation axis is the RSS of the third column of W */
    /* s[2] = 2*sqrtf(w[10]*w[10] + w[14]*w[14]) */
	/* Converitng Azimuth span to the the width */
	/* sWidth  = 2*Range*tan(0.5*s[1])=2*Range*tan(sqrtf(w[10]*w[10] + w[14]*w[14])) */
	/* gLimit = (gateLim[0]/sWidth)^2 */
	if(gateLim[2] != 0) {
		sWidth = 2*range*tanf(sqrtf(W[10]*W[10] + W[14]*W[14]));
		gLimit = (gateLim[1]*gateLim[1])/(sWidth*sWidth);
		if(gMin > gLimit)
			gMin = gLimit;
	}
	
	/* Orthogonal projections onto Doppler axis is the last element of W */
	/* s[3] = 2*abs(w[15]) */
	if(gateLim[3] != 0) {
		gLimit = (gateLim[2]*gateLim[2])/(4*(W[15]*W[15]));
		if(gMin > gLimit)
			gMin = gLimit;
	}
	return gMin;
}


uint8_t gtrack_isPointBehindTarget(GTRACK_measurement_vector *p, GTRACK_measurement_vector *target)
{
    if( (fabsf(p->azimuth - target->azimuth) < 2*PI/180) && 
        (fabsf(p->elev - target->elev) < 2*PI/180) &&
        (p->range > target->range) )        
        return 1U;
    else
        return 0;
}

/**
*  @b Description
*  @n
*		This function checks whether the point is inside the box boundary or not
*
*  @param[in]  c
*		pointer to a position in cartesian space
*  @param[in] box
*		pointer to a 3D box object
*
*  \ingroup GTRACK_ALG_UTILITY_FUNCTION
*
*  @retval
*      1 if inside, 0 otherwise
*/
uint8_t gtrack_isPointInsideBox(GTRACK_cartesian_position *c, GTRACK_boundaryBox *box)
{
    if( (c->posX > box->x1) && (c->posX < box->x2) && 
        (c->posY > box->y1) && (c->posY < box->y2) && 
        (c->posZ > box->z1) && (c->posZ < box->z2) )
        return 1U;
    else
        return 0;
}
#endif
