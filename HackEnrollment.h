//
// Created by noams on 5/10/2023.
//
#include <stdio.h>
#include <string.h>
#include "IsraeliQueue.h"
#ifndef EX1_HACKENROLLMENT_H
#define EX1_HACKENROLLMENT_H

typedef struct EnrollmentSystem_t* EnrollmentSystem;
typedef struct student_t* student;
typedef struct hacker_t* hacker;
typedef struct course_t* course;


EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers);

EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues);

void hackEnrollment(EnrollmentSystem sys, FILE* out);



student * cpyStudents (FILE* students);//Git rid of this
int howManyWords (FILE* file, int lineNum);// GET RID OF THIS
hacker* cpyHackers (FILE* hackers);// GET RID OF THIS
char* getLine(FILE* file);// GET RID OF THIS
void getWords (char* str, char** arr);// GET RID OF THIS
int longestLine (FILE* file);// GET RID OF THIS

#endif //EX1_HACKENROLLMENT_H
