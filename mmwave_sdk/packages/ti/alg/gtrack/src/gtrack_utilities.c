/**
 *   @file  gtrack_utilities.c
 *
 *   @brief
 *      This is a set of utility functions used by GTRACK Algorithm
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

#define PI 3.14159265358979323846f
#define RAD2DEG (180.f/PI)
/**
*  @b Description
*  @n
*		This function is used to convert a vector from sherical to cartesian
*
*  @param[in]  format
*		Format used
*  @param[in]  sph
*		Pointer to measurements (spheriacl form) vector
*  @param[in]  cart
*		Pointer to state vector (cartesian form)
*
*  \ingroup GTRACK_ALG_UTILITY_FUNCTION
*
*  @retval
*      None
*/
void gtrack_spherical2cartesian(GTRACK_STATE_VECTOR_TYPE format, float *sph, float *cart)
{

	float range; 
	float angle; 
	float azim; 
	float elev; 
	float doppler;
    float sinAngle, cosAngle;
    float sinAzim, cosAzim;
    float sinElev, cosElev;

	switch (format) {
        default:
		case GTRACK_STATE_VECTORS_2DA:
			cart[4] = 0; cart[5] = 0;
			/* fall through */
		case GTRACK_STATE_VECTORS_2DV:
            range = sph[0]; angle = sph[1]; doppler = sph[2];
            gtrack_sincosd(angle*RAD2DEG,&sinAngle, &cosAngle);
			cart[0] = range*sinAngle;
			cart[1] = range*cosAngle;
			cart[2] = doppler*sinAngle;
			cart[3] = doppler*cosAngle;
			break;

		case GTRACK_STATE_VECTORS_3DA:
            cart[6] = cart[7] = 0;
			/* fall through */
		case GTRACK_STATE_VECTORS_3DV:
            range = sph[0]; azim = sph[1]; elev = sph[2]; doppler = sph[3];
            gtrack_sincosd(azim*RAD2DEG,&sinAzim, &cosAzim);
            gtrack_sincosd(elev*RAD2DEG,&sinElev, &cosElev);
			cart[0] = range*cosElev*sinAzim;
			cart[1] = range*cosElev*cosAzim;
			cart[2] = range*sinElev;
			
            cart[3] = doppler*cosElev*sinAzim;
			cart[4] = doppler*cosElev*cosAzim;
			cart[5] = doppler*sinElev;
            break;
	}
}
/**
*  @b Description
*  @n
*		This function is used to convert a vector from cartesian to sherical to cartesian
*
*  @param[in]  format
*		Format used
*  @param[in]  cart
*		Pointer to state vector (cartesian form)
*  @param[in] sph 
*		Pointer to measurements (spheriacl form) vector
*
*  \ingroup GTRACK_ALG_UTILITY_FUNCTION
*
*  @retval
*      None
*/

void gtrack_cartesian2spherical(GTRACK_STATE_VECTOR_TYPE format, float *cart, float *sph)
{
	float posx, posy, posz; 
	float velx, vely, velz;

	switch (format) {
        default:
		case GTRACK_STATE_VECTORS_2DV:	
			/* cart = [posx posy velx vely] */
			/* sph = [range azimuth doppler] */
		case GTRACK_STATE_VECTORS_2DA:
			/* cart = [posx posy velx vely accx accy] */
			/* sph = [range azimuth doppler] */
			posx = cart[0]; posy = cart[1]; 
			velx = cart[2]; vely = cart[3];

			/* calc range */
			sph[0] = sqrtf(posx*posx + posy*posy); 
	    
			/* calc azimuth */
			if (posy == 0)
				sph[1] = PI/2;
			else if (posy > 0)
				sph[1] = atanf(posx/posy);
			else
				sph[1] = atanf(posx/posy) + PI;
    
			/* calc doppler */
			sph[2] = (posx*velx+posy*vely)/sph[0];	
			break;

		case GTRACK_STATE_VECTORS_3DV:	
			/* cart = [posx posy posz velx vely velz accx accy accz] */
			/* sph = [range azimuth elevation doppler] */
		case GTRACK_STATE_VECTORS_3DA:	
			/* cart = [posx posy posz velx vely velz] */
			/* sph = [range azimuth elevation doppler] */
            posx = cart[0]; posy = cart[1]; posz = cart[2];
            velx = cart[3]; vely = cart[4]; velz = cart[5];
			/* calc range */
			sph[0] = sqrtf(posx*posx + posy*posy + posz*posz); 
			/* calc azimuth */
			if (posy == 0)
				sph[1] = PI/2;
			else if (posy > 0)
				sph[1] = atanf(posx/posy);
			else
				sph[1] = atanf(posx/posy) + PI;

            /* calc elevation */
            sph[2] = atanf(posz/sqrtf(posx*posx+posy*posy));
			/* calc doppler */
			sph[3] = (posx*velx+posy*vely+posz*velz)/sph[0];	
			break;
	}
}

/**
*  @b Description
*  @n
*		This function computes partial derivatives of the cartesian vector
*
*  @param[in]  format
*		Format used
*  @param[in]  cart
*		Pointer to state vector (cartesian form)
*  @param[in] jac 
*		Pointer to jacobian parial derivatives) vector
*
*  \ingroup GTRACK_ALG_UTILITY_FUNCTION
*
*  @retval
*      None
*/

void gtrack_computeJacobian(GTRACK_STATE_VECTOR_TYPE format, float *cart, float *jac)
{    
    uint16_t ssize;
    float posx, posy, posz;
    float velx, vely, velz;
	float range, range2, range3;
    float rangeXY2, rangeXY;

	switch (format) {
		case GTRACK_STATE_VECTORS_2DV:
			/* cart = [posx posy velx vely] */
            ssize = 4;
            break;
        case GTRACK_STATE_VECTORS_2DA:
			/* cart = [posx posy velx vely accx accy] */
            ssize = 6;
            break;

		case GTRACK_STATE_VECTORS_3DV:
			/* cart = [posx posy posz velx vely velz] */
            ssize = 6;
            break;
        case GTRACK_STATE_VECTORS_3DA:
			/* cart = [posx posy posz velx vely velz accx accy accz] */
            ssize = 9;
            break;
    }


	switch (format) {
        default:
		case GTRACK_STATE_VECTORS_2DV:	    		
            /* jacobian is 3x4 */
            // falls through
        case GTRACK_STATE_VECTORS_2DA:
			/* jacobian is 3x6 */

            posx = cart[0]; posy = cart[1];
            velx = cart[2]; vely = cart[3];

            range2 = (posx*posx + posy*posy);
	        range = sqrtf(range2);
	        range3 = range*range2;

            /* dR */
		    jac[0] = posx/range; /* dx */
		    jac[1] = posy/range; /* dy */

            /* dPhi */
		    jac[ssize+0] = posy/range2; /* dx */
		    jac[ssize+1] = -posx/range2; /* dy */

            /* dR' */
		    jac[2*ssize+0] = (posy*(velx*posy - vely*posx))/range3; /* dx */
			jac[2*ssize+1] = (posx*(vely*posx - velx*posy))/range3; /* dy */
			jac[2*ssize+2] = posx/range; /* dx' */
			jac[2*ssize+3] = posy/range; /* dy' */
			break;


        case GTRACK_STATE_VECTORS_3DV:
			/* jacobian is 4x6 */
            // falls through
		case GTRACK_STATE_VECTORS_3DA:
			/* jacobian is 4x9 */
            posx = cart[0]; posy = cart[1]; posz = cart[2];
            velx = cart[3]; vely = cart[4]; velz = cart[5];

	        range2 = posx*posx + posy*posy + posz*posz;
	        range = sqrtf(range2);

            rangeXY2 = posx*posx + posy*posy;
	        rangeXY = sqrtf(rangeXY2);

			/* dR */
		    jac[0] = posx/range;                /* dx */
			jac[1] = posy/range;                /* dy */
			jac[2] = posz/range;                /* dz */

            /* dAzim */
			jac[ssize+0] = posy/rangeXY2;     /* dx */
			jac[ssize+1] = -posx/rangeXY2;    /* dy */
			jac[ssize+2] = 0;                 /* dz */

            /* dElev */
		    jac[2*ssize+0] = -posz*posz/(range2*rangeXY);  /* dx */
		    jac[2*ssize+1] = -posy*posz/(range2*rangeXY);  /* dy */
			jac[2*ssize+2] = rangeXY/range2;               /* dz */

			/* dR' */
		    jac[3*ssize+0] = (posy*(velx*posy - vely*posx)+posz*(velx*posz-velz*posx))/(range2*range); /* dx */
			jac[3*ssize+1] = (posx*(vely*posx - velx*posy)+posz*(vely*posz-velz*posy))/(range2*range); /* dy */
			jac[3*ssize+2] = (posx*(velz*posx - velx*posz)+posy*(velz*posy-vely*posz))/(range2*range); /* dz */
			jac[3*ssize+3] = posx/range; /* dx' */
			jac[3*ssize+4] = posy/range; /* dy' */
			jac[3*ssize+5] = posz/range; /* dz' */
			break;
	}
}

/**
*  @b Description
*  @n
*		This function is used to unroll radial velocity from +/- rvMax form based on expectation velocity value
*
*  @param[in]  rvMax
*		Unambigious radial velocity
*  @param[in]  rvExp
*		Expected radial velocity value
*  @param[in] rvIn 
*		Measured radial velocity value
*
*  \ingroup GTRACK_ALG_UTILITY_FUNCTION
*
*  @retval
*      rvOut Unrolled radial velocity value
*/

float gtrack_unrollRadialVelocity(float rvMax, float rvExp, float rvIn)
{
	float distance;
	uint16_t factor;
	float rvOut;

	distance = rvExp - rvIn;
	if(distance >= 0) {
		/* going right */
		factor = (int)((distance+rvMax)/(2*rvMax));
		rvOut = rvIn + 2*rvMax*factor;
	} 
	else {
		/* going left */
		factor = (int)((rvMax-distance)/(2*rvMax));
		rvOut = rvIn - 2*rvMax*factor;
	}
	return rvOut;
}

