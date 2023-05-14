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
void unCapital (EnrollmentSystem sys);


EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers);

EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues);

void hackEnrollment(EnrollmentSystem sys, FILE* out);

#endif //EX1_HACKENROLLMENT_H
