//
// Created by noams on 5/3/2023.
//
#include "IsraeliQueue.h"
#include <stdlib.h>
#include <stdio.h>

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
    if (friendshipFunction == NULL)
    {
        return NULL;
    }

    IsraeliQueue newQueue = malloc(sizeof (*newQueue));
    if (newQueue == NULL)
    {
        free (newQueue);
        return NULL;
    }
    newQueue->compareFunction = comparisonFunction;
    newQueue->rivalry_th = rivalry_th;
    newQueue->friendship_th = friendship_th;
    newQueue->head = NULL;
    newQueue->tail = NULL;
    int arrLen = friendArraySize(friendshipFunction) + 1;
    newQueue->friendFunction = malloc(arrLen * sizeof(FriendshipFunction));
    for (int i = 0; i < arrLen; ++i)
    {
        newQueue->friendFunction[i] = friendshipFunction[i];
    }
    return newQueue;

}

Node* nodeCreate (void* value)
{
    Node* newNode = malloc(sizeof *newNode);
    if (newNode == NULL || value == NULL)
    {
        free(newNode);
        return  NULL;
    }
    newNode->next = NULL;
    newNode->friends = 0;
    newNode->rivals = 0;
    newNode->value = value;
    return newNode;
}

void PrintIsraeliQueue (IsraeliQueue q)
{
    if(q == NULL || q->head == NULL)
    {
        printf("NULL\n");
    }
    else
    {
        Node* temp = q->head;
        while(temp != NULL)
        {
            printf("%d -> " , *(int*)temp->value);
            //printf("has %d friends" , temp->rivals);
            temp = temp->next;
        }
        printf("\n");
    }
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
        if(q->friendFunction[i] != NULL)
        {
            count += q->friendFunction[i](personInLine, person);
            if (q->friendFunction[i](personInLine, person) > q->friendship_th) {
                return -1;
            }
        }
    }

    return  count;
}

bool hasRivalsBehind (Node* person, IsraeliQueue q, Node* position, bool rivalCheck)
{
    if(person == NULL || q == NULL)
    {
        return  NULL;
    }
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
        temp = temp->next;
    }

    return false;
}

IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue q, void * value)
{
    bool rivalCheck = true;
    bool hasEntered = false;
    if(q == NULL || value == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    Node* person = nodeCreate(value);
    Node* temp = q->head;
    while (temp != NULL && !hasEntered)
    {
        if(temp->friends < 5 && areFriends(temp, person, q) == -1)
        {
            if(!hasRivalsBehind(person,q,temp,rivalCheck))
            {
                temp->friends ++;
                person->next = temp->next;
                temp->next = person;
                hasEntered = true;
            }
            else
            {
                rivalCheck = false;
            }

        }
        temp = temp->next;
    }
    if(!hasEntered)
    {
        QueueEnqueue(q,person);
    }
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError QueueEnqueue(IsraeliQueue queue, Node* person)
{

    if(person->value == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    Node* newPerson = malloc(sizeof (*newPerson));
    if(newPerson == NULL)
    {
        free(newPerson);
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    newPerson->friends = person->friends;
    newPerson->rivals = person->rivals;
    newPerson->value = person->value;
    newPerson->next = NULL;
    if(queue->head == NULL)
    {
        queue->head = newPerson;
        queue->tail = newPerson;
    }
    else
    {
        queue->tail->next = newPerson;
        queue->tail = newPerson;
    }
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueue IsraeliQueueClone(IsraeliQueue q)
{
    IsraeliQueue newQueue = malloc(sizeof (*newQueue));
    if(newQueue == NULL || q == NULL || q->head == NULL)
    {
        free(newQueue);
        return NULL;
    }
    newQueue = IsraeliQueueCreate(q->friendFunction,q->compareFunction,q->friendship_th,q->rivalry_th);
    Node* temp = q->head;
    while(temp!=NULL)
    {
        QueueEnqueue(newQueue, temp);
        temp = temp->next;
    }

    return  newQueue;

}

void* IsraeliQueueDequeue(IsraeliQueue q)
{
    if(q == NULL || q->head == NULL || q->head->next == NULL)
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

bool IsraeliQueueContains(IsraeliQueue q, void * item)
{
    if (q == NULL || q->head == NULL || q->compareFunction)
    {
        return false;
    }
    Node* temp = q->head;
    while(temp != NULL)
    {
        if(q->compareFunction(temp->value, item))
        {
            return true;
        }

    }
    return  false;
}

IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue q, FriendshipFunction friendshipFunction)
{
    if(q == NULL || q->friendFunction == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    int arrSize = friendArraySize(q->friendFunction);
    FriendshipFunction* newFriendshipFunction = malloc(sizeof(FriendshipFunction) * (arrSize + 2));
    if(newFriendshipFunction == NULL)
    {
        free(newFriendshipFunction);
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    int i = 0;
    for ( i = 0; i < arrSize; i++)
    {
        newFriendshipFunction[i] = q->friendFunction[i];
    }
    newFriendshipFunction[i] = friendshipFunction;
    newFriendshipFunction[i+1] = NULL;
    FriendshipFunction* temp = q->friendFunction;
    q->friendFunction = newFriendshipFunction;
    free(temp);
    return ISRAELIQUEUE_SUCCESS;

}

int qArraySize (IsraeliQueue* qarr)
{
    int i = 0;
    for (i = 0; qarr[i] != NULL; ++i);
    return i;
}

bool IsEmpty (IsraeliQueue* qarr)
{
    for (int i = 0; i < qArraySize(qarr); ++i)
    {
        if(qarr[i]->head != NULL)
        {
            return false;
        }
    }
    return  true;
}

IsraeliQueue IsraeliQueueMerge(IsraeliQueue* qarr,ComparisonFunction comparisonFunction)
{
    if(qarr == NULL)
    {
        return NULL;
    }
    int friendship_th_sum = 0;
    int rivalry_th_sum = 0;
    int ffSizeTotal = 0;
    for (int i = 0; i < qArraySize(qarr); ++i)
    {
        if(qarr[i]!=NULL)
        {
            friendship_th_sum += qarr[i]->friendship_th;
            rivalry_th_sum += qarr[i]->rivalry_th;
            if(qarr[i]->friendFunction != NULL)
            {
                ffSizeTotal += friendArraySize(qarr[i]->friendFunction);
            }
        }
    }
    FriendshipFunction* newff = malloc((sizeof (FriendshipFunction) * ffSizeTotal)+1);
    int c = 0;
    for (int i = 0; i < qArraySize(qarr); ++i)
    {
        for (int p = 0; qarr[i]->friendFunction[p] != NULL ; ++p)
        {
            newff[c] = qarr[i]->friendFunction[p];
            c++;
        }
    }
    IsraeliQueue newQueue = IsraeliQueueCreate(newff, comparisonFunction, friendship_th_sum/qArraySize(qarr), rivalry_th_sum/qArraySize(qarr));
    int j = 0;
    while (!IsEmpty(qarr))
    {
        if(qarr[j]->head != NULL)
        {
            QueueEnqueue(newQueue, qarr[j]->head);
            IsraeliQueueDequeue(qarr[j]);
        }
        j++;
    }
    for (int i = 0; i < qArraySize(qarr); ++i)
    {
        IsraeliQueueDestroy(qarr[i]);
    }
    return  newQueue;

}

bool isInLine (Node* person, Node* lineHead)
{
    Node* temp = lineHead;
    while (temp != NULL)
    {
        if(temp->value == person->value)
        {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

int countLine (Node* head)
{
    int count = 0;
    Node* temp = head;
    while (temp != NULL)
    {
        count++;
        temp = temp->next;
    }
    return count;
}

bool allInLine (IsraeliQueue q, Node* used)
{
    Node* temp = q->head;
    while (temp != NULL)
    {
        if(!isInLine(temp, used))
        {
            return false;
        }
        temp = temp->next;
    }
    return true;
}

IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue q)
{
    if(q == NULL || q->head == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    if(q->head->next != NULL && q->tail != NULL)
    {
        Node *current = q->tail;
        Node *temp1 = q->head;
        Node *used = nodeCreate(q->tail->value);
        Node *temp2 = used;
        while (allInLine(q, used))
        {
            temp1 = q->head;
            while (temp1->next->next != current)
            {
                temp1 = temp1->next;
            }
            current = temp1->next;
            if (!isInLine(current, used))
            {
                temp1->next = current->next;
                current->next = NULL;
                IsraeliQueueEnqueue(q, current->value);
                temp2->next = nodeCreate(current->value);
                temp2 = temp2->next;
                Node* memoryTemp = current;
                current = temp1;
                free(memoryTemp);
            }


        }
        free(used);
    }
    return ISRAELIQUEUE_SUCCESS;

}