/*
 * File: pqchunklist.c
 * --------------
 * This file implements a pqueue with the use of a chunklist.
 *
 * Jonathan Gustafsson, Algoritmer och Datastrukturer 1, vt2019
 */

#include "pqueue.h"
#include "genlib.h"

 /* Constant: MAX_ELEMENTS_PER_BLOCK
  * ----------------------
  * This constant determines the max amount of values to be contained within a block.
  */

#define MAX_ELEMS_PER_BLOCK 5

typedef struct chunkListCell {
	int block[MAX_ELEMS_PER_BLOCK];
	int numEntries;
	struct chunkListCell *next;
} chunkListCell;


struct pqueueCDT {
	chunkListCell *head;
};

/* Exported endries */

/*Function prototypes*/

chunkListCell* newBlock(void);
void insertInBlock(chunkListCell *curreentPointer, int newValue);
void splitBlock(chunkListCell *currentPointer, int newValue);

pqueueADT NewPQueue(void)
{
	pqueueADT pqueue;

	pqueue = New(pqueueADT);
	pqueue->head = NULL;
	return (pqueue);
}

void FreePQueue(pqueueADT pqueue)								
{
	chunkListCell  *next, *currentPointer = pqueue->head;

	while (currentPointer != NULL)
	{	
		next = currentPointer->next;
		FreeBlock(currentPointer->block);
		currentPointer = next;		
	}
	FreeBlock(pqueue);
}

bool IsEmpty(pqueueADT pqueue)
{
	return(pqueue->head == NULL);
}

bool IsFull(pqueueADT pqueue)
{
	return(FALSE);
}

/*
 * Implementation notes: Enqueue
 * -----------------------------
 * Enqueue checks if the pqueue is empty, if so it creates a new block and sets it as head.
 * If the pqueue isnt empty, the function searches for the right block to insert the new element.
 * When the right block has been found, it checks if the block will become full with a new insertion.
 * If so, the function calls for a split of the block, if not, the new element is inserted.
 */

void Enqueue(pqueueADT pqueue, int newValue)
{
	chunkListCell *currentPointer = pqueue->head;

	if (IsEmpty(pqueue))
	{
		pqueue->head = newBlock();
		insertInBlock(pqueue->head, newValue);
	}
	else
	{
		while (currentPointer->next !=NULL && currentPointer->block[currentPointer->numEntries - 1] > newValue)
		{
			currentPointer = currentPointer->next;
		}

		if (currentPointer->numEntries == MAX_ELEMS_PER_BLOCK - 1)
		{
			printf("Splitting block\n");
			splitBlock(currentPointer, newValue);
		}
		else
		{
			insertInBlock(currentPointer, newValue);
		}
	}
}

/*
 * Implementation notes: DequeueMax
 * -------------------------------------------------
 * DequeueMax removes the first element in the first block since the elements are sorted in ascending order, if the pqueue is
 * empty the function prompts an error message. If the first block is empty or contains a value less than 1, the function
 * will free up the memory alocated for that block and point to the next block. When the element hase been removed,
 * the elements left in the block will be given a new index.
 */

int DequeueMax(pqueueADT pqueue)
{
	chunkListCell *currentPointer = pqueue->head, *next;
	int maxValue=0;

	if (IsEmpty(pqueue))
	{
		Error("Tried to dequeue from an empty pqueue.\n");
	}
	if (currentPointer->block[currentPointer->numEntries-1] < 1)
	{
		next = currentPointer->next;
		FreeBlock(currentPointer->block);
		currentPointer = next;
		pqueue->head = currentPointer;
		
	}
	maxValue = currentPointer->block[0];
	for (int i = 0; i < currentPointer->numEntries; i++)
	{
		currentPointer->block[i] = currentPointer->block[i + 1];
	}
	currentPointer->numEntries--;
	return(maxValue);
}

/*
 * Implementation notes: splitBlock
 * -------------------------------------------------
 * Splitblock is called upon if a block is about to become full. This function splits the almost full block into
 * two parts, it keeps the highest elements and the lower elements are sorted into a new block, the new element is
 * sorted into its appropriate block.
 */

void splitBlock(chunkListCell *currentPointer, int newValue)
{
	chunkListCell *nextBlock;
	nextBlock = newBlock();
	int i, mid = currentPointer->numEntries / 2, nrEntries = currentPointer->numEntries;

	for (i = mid; i < nrEntries; i++)
	{
		nextBlock->block[nextBlock->numEntries] = currentPointer->block[i];
		currentPointer->block[i] = NULL;
		nextBlock->numEntries++;
		currentPointer->numEntries--;
	}	
	if (currentPointer->block[currentPointer->numEntries-1] <= newValue)
	{
		insertInBlock(currentPointer, newValue);
	}
	else
	{
		insertInBlock(nextBlock, newValue);
	}
	nextBlock->next = currentPointer->next;
	currentPointer->next = nextBlock;
}

/*
 * Implementation notes: newBlock
 * -------------------------------------------------
 * Newblock creates a new block and allocates memory for it.
 */

chunkListCell* newBlock(void)
{
	chunkListCell *newBlock;

	newBlock = (chunkListCell*)malloc(sizeof(chunkListCell));
	newBlock->numEntries = 0;
	newBlock->next = NULL;
	return(newBlock);
}

/*
 * Implementation notes: insertInBlock
 * -------------------------------------------------
 * Insertinblock inserts a new element sorted into the block which the function input points to.
 * When the new element has been inserted into its right position, the elements lower than the newly inserted
 * element are given a new index. 
 */

void insertInBlock(chunkListCell *currentPointer, int newValue)
{
	int i;

	if (currentPointer->block[0] < 1)
	{
		currentPointer->block[0] = newValue;
		currentPointer->numEntries++;
	}
	else
	{
		for (i = currentPointer->numEntries - 1; (i >= 0) && currentPointer->block[i] <= newValue; i--)
		{
			currentPointer->block[i + 1] = currentPointer->block[i];
		}
		currentPointer->block[i + 1] = newValue;
		currentPointer->numEntries++;
	}
}