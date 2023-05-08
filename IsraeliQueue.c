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

}IsraeliQueue_t;

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
    newQueue->friendFunction = malloc(arrLen * sizeof(*newQueue->friendFunction));
    for (int i = 0; i < arrLen; ++i)
    {
        newQueue->friendFunction[i] = friendshipFunction[i];
    }
    return newQueue;

}

Node* nodeCreate (void* value)
{
    Node* newNode = malloc(sizeof *newNode);
    if (newNode == NULL)
    {
        return  NULL;
    }
    newNode->next = NULL;
    newNode->friends = 0;
    newNode->rivals = 0;
    newNode->value = value;
    return newNode;
}

int friendArraySize (FriendshipFunction * friendshipFunction)
{
    int i = 0;
    for (i = 0; friendshipFunction[i] != NULL; ++i);
    return i;
}

int areFriends (Node* personInLine, Node* person, IsraeliQueue q)
{
    int count = 0;
    for (int i = 0; i < friendArraySize(q->friendFunction); ++i)
    {
        count += q->friendFunction[i](personInLine,person);
        if(q->friendFunction[i](personInLine,person) > q->friendship_th)
        {
            return  -1;
        }
    }

    return  count;
}

bool hasRivalsBehind (Node* person, IsraeliQueue q, Node* position, bool rivalCheck)
{
    Node* temp = position;
    while(temp != NULL)
    {
        int friendshipNum = areFriends(temp,person,q);
        if(friendshipNum != -1 && friendshipNum < q->rivalry_th && temp->rivals < 3)
        {
            if(rivalCheck)
            {
                temp->rivals ++;
            }

            return  true;
        }
    }

    return false;
}

IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue q, void * value)
{
    bool rivalCheck = true;
    if(q == NULL || value == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    Node* person = nodeCreate(value);
    Node* temp = q->head;
    while (temp->next->next != NULL)
    {
        if(temp->friends < 5 && areFriends(temp, person, q) == -1)
        {
            if(!hasRivalsBehind(person,q,temp,rivalCheck))
            {
                temp->friends ++;
                person->next = temp->next;
                temp->next = person;
            }
            else
            {
                rivalCheck = false;
            }

        }
    }
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError QueueEnqueue(IsraeliQueue queue, Node* person)
{
    Node* newPerson = malloc(sizeof (*newPerson));
    if(newPerson == NULL)
    {
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    if(person->value == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    newPerson->friends = person->friends;
    newPerson->rivals = person->rivals;
    newPerson->value = person->value;
    newPerson->next = NULL;
    queue->tail->next = newPerson;
    queue->tail = newPerson;
    return ISRAELIQUEUE_SUCCESS;

}

IsraeliQueue IsraeliQueueClone(IsraeliQueue q)
{
    IsraeliQueue newQueue = malloc(sizeof (*newQueue));
    if(newQueue == NULL || q == NULL || q->head == NULL)
    {
        return NULL;
    }
    newQueue = IsraeliQueueCreate(q->friendFunction,q->compareFunction,q->friendship_th,q->rivalry_th);
    Node* temp = q->head;
    while(temp!=NULL)
    {
        IsraeliQueueEnqueue(newQueue, temp);
        if(temp == q->head)
        {
            newQueue->head = newQueue->tail;
        }
        temp = temp->next;
    }

    return  newQueue;

}

void* IsraeliQueueDequeue(IsraeliQueue q)
{
    if(q == NULL || q->head == NULL)
    {
        return NULL;
    }
    void* x = q->head->value;
    Node* temp = q->head;
    q->head = q->head->next;
    free(temp);
    return x;
}

void IsraeliQueueDestroy(IsraeliQueue q)
{
    Node* temp = q->head;
    while(temp!=NULL)
    {
        IsraeliQueueDequeue(q);
        temp = temp->next;
    }
    free(q->friendFunction);
    free(q);
}

IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue q, int friendshipThreshold)
{
    if(q == NULL || friendshipThreshold == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    q->friendship_th = friendshipThreshold;
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue q, int rivalryThreshold)
{
    if(q == NULL || rivalryThreshold == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    q->friendship_th = rivalryThreshold;
    return ISRAELIQUEUE_SUCCESS;
}

int IsraeliQueueSize(IsraeliQueue q)
{
    if(q == NULL || q->head == NULL || q->tail == NULL)
    {
        return 0;
    }
    int count  = 0;
    Node* temp = q->head;
    while (temp != NULL)
    {
        count++;
        temp = temp->next;
    }
    return count;
}
