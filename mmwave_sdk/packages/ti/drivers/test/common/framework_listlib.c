/**
 *   @file  framework_listlib.c
 *
 *   @brief
 *      Implementation of a doubly linked list.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016 Texas Instruments, Inc.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <ti/drivers/test/common/framework_listlib.h>

/**********************************************************************
 *************************** LISTLIB FUNCTIONS ************************
 **********************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is called to add a node to the list.
 *
 *  @param[in]  ptr_list
 *      This is the list to which the node is to be added.
 *  @param[in]  ptr_node
 *      This is the node which is to be added.
 *
 *  @retval
 *      Not Applicable
 */
void TestFmk_listAdd (TestFmk_ListNode** ptr_list, TestFmk_ListNode* ptr_node)
{
    TestFmk_ListNode*    ptr_head;

    /* Check if the list is empty ? */
    if (*ptr_list == NULL)
    {
        /* YES the list is empty. Initialize the links */
        ptr_node->p_next = NULL;
        ptr_node->p_prev = NULL;

        /* Initialize the LIST */
        *ptr_list = ptr_node;
    }
    else
    {
        /* No the list was NOT empty. Add the node to the beginning of list.
         * Get the current head of the list. */
        ptr_head = *ptr_list;

        /* Initialize the new head of the list. */
        ptr_node->p_next  = ptr_head;
        ptr_node->p_prev = NULL;

        /* Update the old head to point to the new head */
        ptr_head->p_prev = ptr_node;

        /* Update the pointer to the head of the list. */
        *ptr_list = ptr_node;
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is called to concatenate the src list to the end of the
 *      destination list.
 *
 *  @param[in]  ptr_dst
 *      This is the head of the destination list.
 *  @param[in]  ptr_src
 *      This is the head of the source list.
 *
 *  @retval
 *      Not Applicable
 */
void TestFmk_listCat (TestFmk_ListNode** ptr_dst, TestFmk_ListNode** ptr_src)
{
    TestFmk_ListNode*    ptr_node;
    TestFmk_ListNode*    ptr_prev;

    /* Is the source list empty? */
    if (*ptr_src != NULL)
    {
        /* NO: Is the destination list empty? */
        if (*ptr_dst == NULL)
        {
            /* YES: Make the source now as the destination. */
            *ptr_dst = *ptr_src;
        }
        else
        {
            /* NO: Both the lists are not empty. */
            ptr_node = *ptr_dst;
            ptr_prev = NULL;

            /* Reach the end of the list. */
            while (ptr_node != NULL)
            {
                ptr_prev = ptr_node;
                ptr_node = ptr_node->p_next;
            }

            /* Link the last element to the source list. */
            ptr_prev->p_next = *ptr_src;
            (*ptr_src)->p_prev = ptr_prev;
        }
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is called to remove the head node from the list.
 *
 *  @param[in]  ptr_list
 *      This is the pointer to the list from where nodes will be removed.
 *
 *  @retval
 *      Pointer to the head of the list.
 */
TestFmk_ListNode* TestFmk_listRemove (TestFmk_ListNode** ptr_list)
{
    TestFmk_ListNode*    ptr_head;
    TestFmk_ListNode*    ptr_node = NULL;

    /* Check if the list is empty ? */
    if (*ptr_list != NULL)
    {
        /* NO: Get the head of the list. */
        ptr_node = *ptr_list;

        /* Move the head to the next element in the list. */
        ptr_head = ptr_node->p_next;
        *ptr_list = ptr_head;

        /* Did we remove the last element?*/
        if (ptr_head != NULL)
        {
            /* No; in that case update the pointers for the new head. */
            ptr_head->p_prev = NULL;
        }

        /* Kill the links before returning the OLD head. */
        ptr_node->p_next = NULL;
        ptr_node->p_prev = NULL;
    }
    return ptr_node;
}

/**
 *  @b Description
 *  @n
 *      The function is called to remove the specified node from the list.
 *
 *  @param[in]  ptr_list
 *      This is the pointer to the list from where node will be removed.
 *  @param[in]  ptr_remove
 *      This is the node which is to be removed.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   -1
 */
int32_t TestFmk_listRemoveNode (TestFmk_ListNode** ptr_list, TestFmk_ListNode* ptr_remove)
{
    TestFmk_ListNode*    ptr_next;
    TestFmk_ListNode*    ptr_prev;
    int32_t         retVal = 0;

    /* Are there any nodes in the list? */
    if (*ptr_list != NULL)
    {
        /* YES: Are we removing the head? */
        if (ptr_remove == *ptr_list)
        {
            /* Use the other API to acheive the needful. */
            TestFmk_listRemove(ptr_list);
        }
        else
        {
            /* OK; we are trying to remove a non head element; so lets get the
             * previous and next pointer of the elements that needs to be removed. */
            ptr_prev = ptr_remove->p_prev;
            ptr_next = ptr_remove->p_next;

            /* Kill the Links for element that is being removed. */
            ptr_remove->p_prev = NULL;
            ptr_remove->p_next = NULL;

            /* Are we removing the last element */
            if (ptr_next == NULL)
            {
                /* The last element points to nothing. */
                ptr_prev->p_next = NULL;
            }
            else
            {
                /* We are trying to remove an element in the middle of the list. */
                ptr_prev->p_next = ptr_next;
                ptr_next->p_prev = ptr_prev;
            }
        }
    }
    else
    {
        /* No: The list is empty. */
        retVal = -1;
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to get the head of the specific list
 *
 *  @param[in]  ptr_list
 *      This is the pointer to the list.
 *
 *  @retval
 *      Head of the list (could be NULL if the list is empty)
 */
TestFmk_ListNode* TestFmk_listGetHead (TestFmk_ListNode** ptr_list)
{
    return *ptr_list;
}

/**
 *  @b Description
 *  @n
 *      The function is used to get the next element in the list.
 *
 *  @param[in]  ptr_list
 *      This is the pointer to the node in the list.
 *
 *  @retval
 *      Next element in the list. (could be NULL if this is the last element)
 */
TestFmk_ListNode* TestFmk_listGetNext (TestFmk_ListNode* ptr_list)
{
    return ptr_list->p_next;
}

/**
 *  @b Description
 *  @n
 *      The function is used to get the previous element in the list.
 *
 *  @param[in]  ptr_list
 *      This is the pointer to the node in the list.
 *
 *  @retval
 *      Previous element in the list. (could be NULL if this is the first element)
 */
TestFmk_ListNode* TestFmk_listGetPrev (TestFmk_ListNode* ptr_list)
{
    return ptr_list->p_prev;
}

