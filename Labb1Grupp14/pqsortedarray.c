/*
 * File: pqsortedarray.c
 * --------------
 * This file implements a priorityqueue with a dynamic memory allocation.
 * The implementation sorts element when inserted in an ascending order.
 * Dequeueing removes the largest element in the pqueue. 
 * If the pqueue becomes full, the memory allocation doubles.
 * 
 * Jonathan Gustafsson, Algoritmer och datastrukturer 1, vt2019
 */

#include "pqueue.h" 

 /* Constant: MAX_ELEMENTS
  * ----------------------
  * Den här konstanten anger antalet element i den vektor som
  * utgör representationen av prioritetskön.
  */

#define MAX_ELEMENTS 5


/* Två statiska heltal för att se till att isFull och minnesallokeringen fungerar på rätt sätt. */

static int countOne = 1;
static int countTwo = 2;

struct pqueueCDT {
	int *entries;
	int numEntries;
};

/* Exported endries */

pqueueADT NewPQueue(void)
{
	pqueueADT pqueue;

	pqueue = New(pqueueADT);
	pqueue->entries = malloc(sizeof(pqueueADT)*MAX_ELEMENTS);
	pqueue->numEntries = 0;
	return (pqueue);
}

pqueueADT pQueueExtendMemory(pqueueADT pqueue)
{
	int length = pqueue->numEntries;

	pqueueADT *pqueueTmp = (pqueueADT*)malloc(sizeof(pqueueADT)*MAX_ELEMENTS*countTwo);

	for (int i = 0; i < length; i++)
	{
		pqueueTmp[i] = pqueue->entries[i];
	}
	free(pqueue->entries);
	printf("Memory extended and old memory is freed\n");
	pqueue->entries = pqueueTmp;
	countOne++;
	countTwo++;
	return(pqueue); 
}

void FreePQueue(pqueueADT pqueue)
{
	FreeBlock(pqueue);
}

bool IsEmpty(pqueueADT pqueue)
{
	return (pqueue->numEntries == 0);
}
 
bool IsFull(pqueueADT pqueue)
{
	return (pqueue->numEntries == MAX_ELEMENTS*countOne);
}

/*
 * Implementation notes: Enqueue
 * -----------------------------
 * Elements are stored in a ascending order, when an element is inserted into the array, all elements that are larger gets
 * a new index value.
 * The function checks if the pqueue is full, and if so, the memory allocation for the pqueue is doubled.
 *
 */

void Enqueue(pqueueADT pqueue, int newValue)
{
	int i;
	if (IsFull(pqueue)) {
		printf("Queue is full, memory extended.\n");
		pqueue = pQueueExtendMemory(pqueue);
	}

	for (i = pqueue->numEntries-1; (i >= 0) && pqueue->entries[i] > newValue; i--)
	{
		pqueue->entries[i + 1] = pqueue->entries[i];
	}
	pqueue->entries[i+1] = newValue;
	pqueue->numEntries++;
}
	
/*
 * Implementation notes: DequeueMax
 * -------------------------------------------------
 * Since the elements are stored in a ascending order, dequeueing from pqueue is simple.
 * The function removes the last element in the pqueue, which is the largest.
 */

int DequeueMax(pqueueADT pqueue)
{
	int maxValue=0;

	if (IsEmpty(pqueue))
	{
		Error("Tried to dequeue max from an empty pqueue!");
	}
	maxValue = pqueue->entries[pqueue->numEntries-1];
	pqueue->numEntries--;
	return(maxValue);
}