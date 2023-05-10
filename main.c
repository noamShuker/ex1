//
// Created by noams on 5/8/2023.
//
#include "IsraeliQueue.h"
#include <stdlib.h>
#include <stdio.h>

int friendFunc1(void* a, void* b)
{
    //int aa = *(int*)a;
    // bb = *(int*)b;
    return 101;
}
int friendFunc2(void* a, void* b)
{
    //int aa = *(int*)a;
    // bb = *(int*)b;
    return 202;
}
int friendFunc3(void* a, void* b)
{
    //int aa = *(int*)a;
    // bb = *(int*)b;
    return 303;
}

int main()
{
    FriendshipFunction fFunction[] = {friendFunc1, NULL};
    //fFunction[4] = NULL;
    ComparisonFunction cFunction = NULL;
    IsraeliQueue q = IsraeliQueueCreate(fFunction, cFunction, 60, 20);
    int arr[] =  {1,2,3,4,5,6,7};
    for (int i = 0; i < 7; ++i)
    {
        Node* x = nodeCreate( &arr[i]);
        QueueEnqueue(q, x);
    }

    IsraeliQueue dup = IsraeliQueueClone(q);
    PrintIsraeliQueue(q);
    PrintIsraeliQueue(dup);
    printf("%d\n", fFunction[0](NULL, NULL));
    IsraeliQueueAddFriendshipMeasure(q,friendFunc2);
    if(fFunction[1] == NULL)
    {
        printf("SHOELALALALALALA\n");
    }
    printf("%d\n", fFunction[0](NULL, NULL));
    //printf("%d\n", fFunction[1](NULL, NULL));

}

