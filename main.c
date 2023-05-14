//
// Created by noams on 5/8/2023.
//
#include "IsraeliQueue.h"
#include "HackEnrollment.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>


char* stringDuplicator(char* s, int times)
{
    if(!s || times <= 0)
    {
        return NULL;
    }
    unsigned long long LEN = strlen(s);
    char* out = malloc(LEN*times+1);
    if(!out)
    {
        return NULL;
    }
    char* temp = out;
    for (int i=0; i < times; i++)
    {
        strcpy(out, s);
        out = out + LEN;
    }
    out = temp;
    return out;
}

int main()
{
    printf("%s\n", stringDuplicator("Yeah mister white yeah science", 6));
}

