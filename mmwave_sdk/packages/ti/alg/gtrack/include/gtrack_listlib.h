/**
 *   @file  gtrack_listlib.h
 *
 *   @brief   
 *      Header file for a double linked list 
 *      implementation.
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

#ifndef GTRACK_LIST_LIB_H__
#define GTRACK_LIST_LIB_H__

/**
 * @brief 
 *  GTrack ListElement
 *
 * @details
 *  The structure describes a list node which has links to the previous
 *  and next element in the list.
 */
typedef struct GTrack_ListElem_t
{
	uint32_t					data;
	struct GTrack_ListElem_t	*prev;
	struct GTrack_ListElem_t	*next;
} GTrack_ListElem;

/**
 * @brief 
 *  GTrack List Object
 *
 * @details
 *  The structure describes the list object 
 */
typedef struct
{
    uint32_t			count;
    GTrack_ListElem		*begin;
    GTrack_ListElem		*end;
} GTrack_ListObj;

/**********************************************************************
 **************************** EXPORTED API ****************************
 **********************************************************************/

extern void gtrack_listInit (GTrack_ListObj *list);
extern int32_t gtrack_isListEmpty (GTrack_ListObj *list);
extern void gtrack_listEnqueue (GTrack_ListObj *list, GTrack_ListElem *elem);
extern GTrack_ListElem *gtrack_listDequeue (GTrack_ListObj *list);
extern GTrack_ListElem* gtrack_listGetFirst (GTrack_ListObj *list);
extern GTrack_ListElem* gtrack_listGetNext (GTrack_ListElem *elem);
extern uint32_t gtrack_listGetCount (GTrack_ListObj *list);
extern int32_t gtrack_listRemoveElement (GTrack_ListObj *list, GTrack_ListElem *elem);

#endif /* GTRACK_LIST_LIB_H__ */

