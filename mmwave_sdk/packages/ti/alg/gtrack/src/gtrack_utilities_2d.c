/**
 *   @file  gtrack_utilities_2d.c
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

#ifdef GTRACK_2D

#define PI 3.14159265358979323846f
#define RAD2DEG (180.f/PI)

const float pInit[] = {0.f, 0.f, 0.5f, 0.5f, 1.f, 1.f};
const float spreadInit[] = {0.5f, 2*PI/180.f, 0.5f};
const float spreadMin[] = {0.1f, PI/180.f, 0.1f};

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
    float sinAngle, cosAngle;

    gtrack_sincosd(v->angle*RAD2DEG,&sinAngle, &cosAngle);
    c->posX = v->range*sinAngle;
    c->posY = v->range*cosAngle;
}

void gtrack_calcMeasurementVar(GTRACK_measurement_vector *v, GTRACK_varParams *varP, GTRACK_measurement_vector *var)
{
    float angleErr;
    
    var->range = varP->depthStd*varP->depthStd;
    angleErr = 2*atanf(0.5f*varP->widthStd/v->range);
    var->angle = angleErr*angleErr;
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
    tDim[2] = mSpread[2];
}

/**
*  @b Description
*  @n
*		This function is used to calculate a distance between two points defined in measurement coordinates
*
*  @param[in]  p1
*		Pointer to measurements (spheriacl form) vector
*  @param[in]  p2
*		Pointer to measurements (spheriacl form) vector
*
*  \ingroup GTRACK_ALG_UTILITY_FUNCTION
*
*  @retval
*      distance, m
*/

float gtrack_calcDistance(GTRACK_measurement_vector *p1, GTRACK_measurement_vector *p2)
{
    //  d = u1(1)^2 + u2(1)^2 - 2*u1(1)*u2(1)*cos(u1(2)-u2(2));
    float sinAngle, cosAngle;
    gtrack_sincosd((p1->angle-p2->angle)*RAD2DEG,&sinAngle, &cosAngle);
    return p1->range*p1->range + p2->range*p2->range - 2*p1->range*p2->range*cosAngle;
}

/**
*  @b Description
*  @n
*		This function is used to multiply two matrices.
*		First matrix P is of size 6x6, the second one is of the size 3x6.
*		The second matrix is being transposed first.
*		Matrices are all real, single precision floating point.
*		Matrices are in row-major order
*
*  @param[in]  P
*		Matrix P
*  @param[in]  J
*		Matrix J
*  @param[out]  PJ
*		Matrix PJ = P(6,6) X J(3,6)T
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
	/* PJ[6x3] = P[6x6] x J[3x6]' */
	for (i = 0U, k = 0U; i < 18U; i+= 3U, k+=6U)
	{
		for (j = 0U, n = 0U; j < 3U; j++, n+=6U)
		{
            PJ[i + j] = (P[k + 0U] * J[n + 0U]) +
						(P[k + 1U] * J[n + 1U]) +
                        (P[k + 2U] * J[n + 2U]) +
                        (P[k + 3U] * J[n + 3U]) +
                        (P[k + 4U] * J[n + 4U]) +
                        (P[k + 5U] * J[n + 5U]);	
		}
	}
}

/**
*  @b Description
*  @n
*		This function is used to multiply two matrices of the size 6x6. 
*		Matrices are all real, single precision floating point.
*		Matrices are in row-major order
*
*  @param[in]  A
*		Matrix A
*  @param[in]  B
*		Matrix B
*  @param[out]  C
*		Matrix C(6x6x) = A(6x6) X B(6x6)
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
*		This function is used to multiply two matrices of size 6x6. 
*		Second Matrix is getting transposed first
*		Matrices are all real, single precision floating point.
*		Matrices are in row-major order
*
*  @param[in]  A
*		Matrix A
*  @param[in]  B
*		Matrix B
*  @param[out]  C
*		Matrix C(6x6) = A(6x6) X B(6x6)T
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
*		This function computes the determinant of 3x3 matrix.
*		Matrix is real, single precision floating point.
*		Matrix is in row-major order
*
*  @param[in]  A
*		Matrix A
*  @param[out]  det
*		det = det(A);
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_matrixDet(float *A, float *det)
{
	*det = A[0] * (A[4]*A[8] - A[7]*A[5]) -
		A[1] * (A[3]*A[8] - A[5]*A[6]) +
		A[2] * (A[3]*A[7] - A[4]*A[6]);
}

/**
*  @b Description
*  @n
*		This function computes the inverse of 3x3 matrix.
*		Matrix is real, single precision floating point.
*		Matrix is in row-major order
*
*  @param[in]  A
*		Matrix A
*  @param[out]  det_out
*		det_out = determinant;
*  @param[out]  inv
*		inv = inverse(A);
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/

void gtrack_matrixInv(const float *A, float *det_out, float *inv)
{
	float det;
	float invdet;

	det = A[0] * (A[4]*A[8] - A[7]*A[5]) -
		A[1] * (A[3]*A[8] - A[5]*A[6]) +
		A[2] * (A[3]*A[7] - A[4]*A[6]);

	invdet = 1 / det;

	inv[0] = (A[4] * A[8] - A[7] * A[5]) * invdet;
	inv[1] = (A[2] * A[7] - A[1] * A[8]) * invdet;
	inv[2] = (A[1] * A[5] - A[2] * A[4]) * invdet;
	inv[3] = (A[5] * A[6] - A[3] * A[8]) * invdet;
	inv[4] = (A[0] * A[8] - A[2] * A[6]) * invdet;
	inv[5] = (A[3] * A[2] - A[0] * A[5]) * invdet;
	inv[6] = (A[3] * A[7] - A[6] * A[4]) * invdet;
	inv[7] = (A[6] * A[1] - A[0] * A[7]) * invdet;
	inv[8] = (A[0] * A[4] - A[3] * A[1]) * invdet;

    *det_out = det;
}


void gtrack_computeMahalanobis(float *d, float *S, float *chi2)
{
	*chi2 = 
		d[0]*(d[0]*S[0]+d[1]*S[3]+d[2]*S[6])+
		d[1]*(d[0]*S[1]+d[1]*S[4]+d[2]*S[7])+
		d[2]*(d[0]*S[2]+d[1]*S[5]+d[2]*S[8]);
}

float gtrack_gateCreateLim (float volume, float *EC, float range, float *gateLim)
{
	float a,b,c, v;
	float gConst;
	float LQ[GTRACK_MEASUREMENT_VECTOR_SIZE*GTRACK_MEASUREMENT_VECTOR_SIZE]; /* Lower triangular decomposition */

	float W[GTRACK_MEASUREMENT_VECTOR_SIZE*GTRACK_MEASUREMENT_VECTOR_SIZE];
	float W_det;
	float sWidth;
	float gLimit;
	float gMin;

	/* Compute unitary volume */
 	a = 1/sqrtf(EC[0]);
	b = 1/sqrtf(EC[4]);
	c = 1/sqrtf(EC[8]);
	v = 4*PI/3*a*b*c;

	/* Compute constant volume expansion factor */
	gConst = powf((float)(volume/v), (float)(2./3.));

	/* Compute limiters */
	gtrack_matrixCholesky(GTRACK_MEASUREMENT_VECTOR_SIZE, EC, LQ);

    gtrack_matrixInv(LQ, &W_det, W);

    gMin = gConst;

	/* Orthogonal projections onto Range, Angle and Doplper dimentions */

	/* Orthogonal projections onto Range is the RSS of the first column of W */
	/* s[0]=2*sqrtf(w[0]*w[0] + w[3]*w[3] + w[6]*w[6]) */
	/* gLimit = (gateLim[0]/s[0])^2=gateLim[0]^2/(4*(w[0]*w[0] + w[3]*w[3] + w[6]*w[6]) */
	if(gateLim[0] != 0) {
		gLimit = (gateLim[0]*gateLim[0])/(4*(W[0]*W[0] + W[3]*W[3] + W[6]*W[6]));
		if(gMin > gLimit)
			gMin = gLimit;
	}

	/* Orthogonal projections onto Angle is the RSS of the second column of W */
	/* s[1] = 2*sqrtf(w[4]*w[4] + w[7]*w[7]) */
	/* Converitng Angle span to the the width */
	/* sWidth  = 2*Range*tan(0.5*s[1])=2*Range*tan(sqrtf(w[4]*w[4] + w[7]*w[7])) */
	/* gLimit = (gateLim[0]/sWidth)^2 */
	if(gateLim[1] != 0) {
		sWidth = 2*range*tanf(sqrtf(W[4]*W[4] + W[7]*W[7]));
		gLimit = (gateLim[1]*gateLim[1])/(sWidth*sWidth);
		if(gMin > gLimit)
			gMin = gLimit;
	}
	
	/* Note the doppler limit is the forth (not third) parameter in gateLimit array */
	/* Orthogonal projections onto Doppler is the last element of W */
	/* s[2] = 2*abs(w[8]) */
	if(gateLim[3] != 0) {
		gLimit = (gateLim[3]*gateLim[3])/(4*(W[8]*W[8]));
		if(gMin > gLimit)
			gMin = gLimit;
	}
	return gMin;
}

uint8_t gtrack_isPointBehindTarget(GTRACK_measurement_vector *p, GTRACK_measurement_vector *target)
{
    if( (fabsf(p->angle - target->angle) < 2*PI/180) && (p->range > target->range))        
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
*		pointer to a 2D box object
*
*  \ingroup GTRACK_ALG_UTILITY_FUNCTION
*
*  @retval
*      1 if inside, 0 otherwise
*/
uint8_t gtrack_isPointInsideBox(GTRACK_cartesian_position *c, GTRACK_boundaryBox *box)
{
    if( (c->posX > box->x1) && (c->posX < box->x2) &&
        (c->posY > box->y1) && (c->posY < box->y2) )
        return 1U;
    else
        return 0;
}
#endif
