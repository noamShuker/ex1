//
// Created by noams on 5/3/2023.
//
#include "IsraeliQueue.h"
#include <stdlib.h>

typedef struct Node
{
    void* value;
    struct Node* next;
    int friends;
    int rivals;
}Node;

typedef struct IsraeliQueue_t
{
    Node* head;
    Node* tail;
    FriendshipFunction* friendFunction;
    ComparisonFunction compareFunction;
    int friendship_th;
    int rivalry_th;

};

int friendArraySize (FriendshipFunction * friendshipFunction);

IsraeliQueue IsraeliQueueCreate(FriendshipFunction * friendshipFunction, ComparisonFunction comparisonFunction, int
friendship_th , int rivalry_th)
{
    if (comparisonFunction == NULL || friendship_th == NULL || rivalry_th == NULL || friendshipFunction == NULL)
    {
        return NULL;
    }

    IsraeliQueue newQueue = malloc(sizeof (*newQueue));
    if (newQueue == NULL)
    {
        return NULL;
    }
    newQueue->compareFunction = comparisonFunction;
    newQueue->rivalry_th = rivalry_th;
    newQueue->friendship_th = friendship_th;
    newQueue->head = NULL;
    newQueue->tail = NULL;
    int arrLen = friendArraySize(friendshipFunction+1);
    FriendshipFunction* ffarr = malloc(arrLen * sizeof(*ffarr));
    for (int i = 0; i < arrLen; ++i)
    {
        newQueue->friendFunction[i] = friendshipFunction[i];
    }
    return newQueue;

}

int friendArraySize (FriendshipFunction * friendshipFunction)
{
    int i = 0;
    for (i = 0; friendshipFunction[i] != NULL; ++i);
    return i;
}

IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue, void *)
{

}



IsraeliQueueError QueueEnqueue(IsraeliQueue queue, void* value)
{

}

IsraeliQueue IsraeliQueueClone(IsraeliQueue q)
{

}
