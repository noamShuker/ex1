//
// Created by noams on 5/10/2023.
//
#include <stdlib.h>
#include <stdio.h>
#include "HackEnrollment.h"
#define STR_Size 1024

typedef struct student_t
{
    char* studentID;
    int totalCredits;
    int GPA;
    char* name;
    char* surname;
    char* city;
    char* department;
}student_t;

typedef struct hacker_t
{
    char* studentID;
    int* courseNumbers;
    char** friends;
    char** rivals;
}hacker_t;

typedef struct course_t
{
    IsraeliQueue q;
    int courseNumber;
    int size;
}course_t;


typedef struct EnrollmentSystem_t
{
    student* students;
    int studentsSize;
    hacker* hackers;
    int hackersSize;
    course* courses;
    int coursesSize;

}EnrollmentSystem_t;

int howManyLines (FILE* file) // before file's changed
{
    int counter = 0;
    int c = fgetc(file);
    while(c != EOF)
    {
        if (c == '\n')
        {
            counter ++;
        }
        c = fgetc(file);
    }
    rewind(file);
    return counter;
}


student studentCreate (char* ID, int credits, int GPA, char* name, char* surName, char* city, char* department)
{
    student newStudent = malloc(sizeof *newStudent);
    newStudent->GPA = GPA;
    strcpy(newStudent->city, city);
    strcpy(newStudent->studentID, ID);
    strcpy(newStudent->name, name);
    strcpy(newStudent->surname, surName);
    strcpy(newStudent->department, department);
    newStudent->totalCredits = credits;
    return newStudent;
}

course courseCreate (IsraeliQueue q, int courseNum, int size)
{
     course newCourse = malloc(sizeof *newCourse);
     newCourse->q = IsraeliQueueClone(q);
     newCourse->courseNumber = courseNum;
     newCourse->size = size;

}

hacker  hackerCreate (char* studentID, int* courseNumbers, char** friends, char** rivals)
{
    hacker newHacker = malloc(sizeof *newHacker);
    strcpy(newHacker->studentID,studentID);
    for (int i = 0; courseNumbers[i] != NULL; ++i)
    {
       newHacker->courseNumbers[i] = courseNumbers[i];
    }
    for (int i = 0; friends[i] != NULL; ++i)
    {
        strcpy(newHacker->friends[i], friends[i]);
    }
    for (int i = 0; rivals[i] != NULL; ++i)
    {
        strcpy(newHacker->rivals[i], rivals[i]);
    }
}

char* getStr (FILE* students)
{
    if(students == NULL)
    {
        return NULL;
    }
    char* str = malloc(sizeof (char) * STR_Size);
    int c = fgetc(students);
    int i = 0;
    if(!(c == ' ' || c == '\n' || c == EOF))
    {
        ungetc(c,students);
    }
    while(c != ' ' && c != EOF && c!='\n')
    {
        if( i == sizeof (*str))
        {
            char* temp = realloc(str, sizeof(*str) * 2);
            if (str == NULL)
            {
                free(str);
                return NULL;
            }
            str = temp;
        }
        str[i] = (char)c;
        fgetc(students);
        i++;
    }
    return str;
}

student * cpyStudents (FILE* students)
{
    char* studentID;
    char* name;
    char* surname;
    char* city;
    char* department;
    int TotCredits, GPA;
    student* studentsArr = malloc(sizeof (student) * howManyLines(students));
    int c = fgetc(students);
    int i = 0;

    while (c != EOF)
    {
        while(c != '\n')//ungetc(students);
        {
            studentID = getStr(students);
            fscanf(" %d %d ", &TotCredits, &GPA);
            name = getStr(students);
            surname = getStr(students);
            city = getStr(students);
            department = getStr(students);
        }

        studentsArr[i] = studentCreate(studentID, TotCredits, GPA, name,surname, city, department);
        i++;
    }

}

/*hacker* cpyHackers (FILE* hackers)
{
    char* studentID;
    int* courseNumbers;
    char** friends;
    char** rivals;

    student* studentsArr = malloc(sizeof (student) * howManyLines(students));
    int c = fgetc(students);
    int i = 0;

    while (c != EOF)
    {
        while(c != '\n')//ungetc(students);
        {
            studentID = getStr(students);
            fscanf(" %d %d ", &TotCredits, &GPA);
            name = getStr(students);
            surname = getStr(students);
            city = getStr(students);
            department = getStr(students);
        }

        studentsArr[i] = studentCreate(studentID, TotCredits, GPA, name,surname, city, department);
        i++;
    }
}


EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers)
{

}*/

