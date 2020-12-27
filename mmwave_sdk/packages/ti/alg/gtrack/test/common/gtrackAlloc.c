#include <stdlib.h>
#include <ti/drivers/osal/MemoryP.h>

#ifndef far 
#define far /* Empty */
#endif

far unsigned int memoryBytesUsed = 0;

void *gtrack_alloc(unsigned int numElements, unsigned int sizeInBytes)
{
	memoryBytesUsed += numElements*sizeInBytes;
    return MemoryP_ctrlAlloc(numElements*sizeInBytes, 0);
}
void gtrack_free(void *pFree, unsigned int sizeInBytes)
{
	memoryBytesUsed -= sizeInBytes;
	MemoryP_ctrlFree(pFree,sizeInBytes);
}
