/**
 *   @file  gtrack_delete.c
 *
 *   @brief
 *      This function deletes the instance of the GTRACK Algorithm
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

/**
*  @b Description
*  @n
*	   Algorithm level delete funtion
*      Application calls this function to delete an instance of GTRACK algorithm
*
*  @param[in]  handle
*      Handle to GTRACK module
*
*  \ingroup GTRACK_ALG_EXTERNAL_FUNCTION
*
*  @retval
*      None
*/
void gtrack_delete(void *handle)
{
    GtrackModuleInstance *inst;
	uint16_t n;

	if(handle) 
	{
		inst = (GtrackModuleInstance *)handle;
	
		for(n=0; n< inst->maxNumTracks; n++) {
			if(inst->hTrack[n] != NULL)
				gtrack_unitDelete(inst->hTrack[n]);
		}
		if(inst->hTrack)
			gtrack_free(inst->hTrack, inst->maxNumTracks * sizeof(GtrackUnitInstance *));
		if(inst->params.F)
            gtrack_free(inst->params.F, GTRACK_STATE_VECTOR_SIZE*GTRACK_STATE_VECTOR_SIZE*sizeof(float));
        if(inst->params.Q)
            gtrack_free(inst->params.Q, GTRACK_STATE_VECTOR_SIZE*GTRACK_STATE_VECTOR_SIZE*sizeof(float));
		if(inst->bestScore)
			gtrack_free(inst->bestScore, inst->maxNumPoints * sizeof(float));
		if(inst->bestIndex)
			gtrack_free(inst->bestIndex, inst->maxNumPoints *sizeof(uint8_t));
		if(inst->allocIndex)
			gtrack_free(inst->allocIndex, inst->maxNumPoints * sizeof(uint16_t));
		if(inst->uidElem)
			gtrack_free(inst->uidElem, inst->maxNumTracks * sizeof(GTrack_ListElem));

		gtrack_free(inst, sizeof(GtrackModuleInstance));
	}
}
