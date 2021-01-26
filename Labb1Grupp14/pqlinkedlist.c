/*
 * File: pqlinkedlist.c
 * --------------
 * This file imlements a pqueue with the use of a linked list.
 *
 * Jonathan Gustafsson, Algoritmer och Datastrukturer 1, vt2019.
 */

#include "pqueue.h"


typedef struct listCell {
	int data;
	struct listCell *next;
} listCell;


struct pqueueCDT {
	listCell *head;
};

/*Exported functions*/

pqueueADT NewPQueue(void)
{
	pqueueADT pqueue;

	pqueue = New(pqueueADT);
	pqueue->head = NULL;
	return (pqueue);
}

void FreePQueue(pqueueADT pqueue)
{
	listCell *nextElement, *currentPointer;

	currentPointer = pqueue->head;

	while (currentPointer != NULL)
	{
		nextElement = currentPointer->next;
		FreeBlock(currentPointer);
		currentPointer = nextElement;
	}
	FreeBlock(pqueue);
}

bool IsEmpty(pqueueADT pqueue)
{
	return (pqueue->head == NULL);
}

bool IsFull(pqueueADT pqueue)
{
	return (FALSE);
}

/*
 * Implementation notes: Enqueue
 * -----------------------------
 * This function creates a new node for the value which is being inserted into the linked list.
 * If the pqueue is empty, the new node is set as head. If the pqueue isnt empty, the function
 * searches for the correct spot to insert the new element. The elements are sorted in descending
 * order. 
 */

void Enqueue(pqueueADT pqueue, int newValue)
{
	listCell *newNode; 
	listCell *currentNode = pqueue->head;
	
	newNode = (listCell*)malloc(sizeof(listCell));
	newNode->data = newValue;
	newNode->next = NULL;

	if (currentNode==NULL || currentNode->data <= newNode->data)
	{
		newNode->next = currentNode;
		pqueue->head = newNode;
		printf("Element %d queued\n", newNode->data);
	}
	else
	{
		while (currentNode->next != NULL && currentNode->next->data > newNode->data)
		{
			currentNode = currentNode->next;
		}
		newNode->next = currentNode->next;
		currentNode->next = newNode;
		printf("Element %d queued\n", newNode->data);
	}
}

/*
 * Implementation notes: DequeueMax
 * -------------------------------------------------
 * Function starts by cheking if the pqueue is empty. If not, it removes the head of the linked
 * list, redirect the head to the next element, free the memory allocated for the node which is being removed
*  and returns the largest element.
 */

int DequeueMax(pqueueADT pqueue)
{
	int i, maxValue;
	listCell *currentPointer = pqueue->head;

	if (IsEmpty(pqueue))
	{
		Error("Priority queue is empty, can't dequeue from an empty queue!");
	}

 	maxValue = currentPointer->data;
	pqueue->head = currentPointer->next;
	printf("%d removed from queue\n", maxValue);
	FreeBlock(currentPointer);
	return (maxValue);
}