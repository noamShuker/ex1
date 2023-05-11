//
// Created by noams on 5/10/2023.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

IsraeliQueueError expandMemory (int strSize, void** str)
{
    strSize += STR_Size;
    void** temp = realloc(str, sizeof(*str) * strSize);
    if (temp == NULL)
    {
        free(str);
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    str = temp;

    return ISRAELIQUEUE_SUCCESS;
}

char* getLine(FILE* file)
{
    int memSize  = STR_Size;
    char* str = malloc(sizeof(char) * memSize);
    int c = fgetc(file);
    if(c == '\n')
    {
        c = fgetc(file);
    }
    int i = 0;
    while (c != '\n' && c != EOF)
    {
        if(i == memSize-1)
        {
            memSize += STR_Size;
            char* temp = realloc(str, sizeof(*str) * memSize);
            if (temp == NULL)
            {
                free(str);
                return NULL;
            }
            str = temp;
        }
        str[i] = c;
        i++;
        c = fgetc(file);
    }
    if(c == EOF)
    {
        ungetc(c, file);
    }
    str[i] = '\0';
    return str;
}

void getWords (char* str, char** arr)
{
    int i = 0;
    int j = 0;
    int m = 0;
    while (str[i] != '\0')
    {
        if(str[i] == ' ')
        {
            arr[j][m] = '\0';
            m = 0;
            j++;
        }
        arr[j] = malloc(sizeof(char) * strlen(str) + 1);
        arr[j][m] = str[i];
        m++;
        i++;
    }


}


int howManyWords (FILE* file, int lineNum) // before file's changed
{
    int LineCounter = 1;
    int c = fgetc(file);
    while(LineCounter < lineNum)
    {
        if (c == '\n')
        {
            LineCounter ++;
        }
        c = fgetc(file);
    }
    int counter = 1;
    while(c != '\n')
    {
        if (c == ' ')
        {
            counter ++;
        }
        c = fgetc(file);
    }
    rewind(file);
    return counter;
}
char* strCpy (char* str)
{
    char* newStr = malloc(sizeof(char) * (strlen(str) + 1));
    char temp = str[0];
    int i = 0;
    while (temp != '\0')
    {
        newStr[i] = temp;
        i++;
        temp = str[i];
    }
    str[i+1] = '\0';
    return newStr;
}


student studentCreate (char* ID, int credits, int GPA, char* name, char* surName, char* city, char* department)
{
    student newStudent = malloc(sizeof *newStudent);
    newStudent->GPA = GPA;
    newStudent->city = strCpy(city);
    newStudent->studentID = strCpy(ID);
    newStudent->name = strCpy(name);
    newStudent->surname = strCpy(surName);
    newStudent->department = strCpy(department);
    newStudent->totalCredits = credits;
    return newStudent;
}

course courseCreate (IsraeliQueue q, int courseNum, int size)
{
     course newCourse = malloc(sizeof *newCourse);
     newCourse->q = IsraeliQueueClone(q);
     newCourse->courseNumber = courseNum;
     newCourse->size = size;
     return  newCourse;

}

hacker  hackerCreate (char* studentID, int courseNumbers [], char** friends,  char** rivals)
{
    int courseNumberSize = STR_Size;
    int friendsSize = STR_Size;
    int rivalsSize = STR_Size;
    hacker newHacker = malloc(sizeof *newHacker);
    newHacker->studentID = strCpy(studentID);
    newHacker->courseNumbers = malloc(sizeof(int) * STR_Size);
    newHacker->friends = malloc(sizeof(char*) * STR_Size);
    newHacker->rivals = malloc(sizeof(char*) * STR_Size);
    int i;
    for (i = 0; courseNumbers[i] != (int)NULL; ++i)
    {
        if(i == courseNumberSize)
        {
            expandMemory(courseNumberSize, (void**)courseNumbers);
        }
       newHacker->courseNumbers[i] = courseNumbers[i];
    }
    newHacker->courseNumbers[i] = (int)NULL;
    for (i = 0; friends[i] != NULL; ++i)
    {
        if(i == friendsSize)
        {
            expandMemory(friendsSize, (void**)friends);
        }
        newHacker->friends[i] = strCpy(friends[i]);
    }
    newHacker->friends[i] = (char*)NULL;
    for (i = 0; rivals[i] != NULL; ++i)
    {
        if(i == rivalsSize)
        {
            expandMemory(rivalsSize, (void**)rivals);
        }
        newHacker->rivals[i] = strCpy(rivals[i]);
    }
    newHacker->rivals[i] = (char*)NULL;
    return newHacker;
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
    int strSize = STR_Size;
    if(!(c == ' ' || c == '\n' || c == EOF))
    {
        ungetc(c,students);
    }
    while(c != ' ' && c != EOF && c!='\n')
    {
        if( i == strSize)
        {
            strSize += STR_Size;
            char* temp = realloc(str, sizeof(*str) * strSize);
            if (str == NULL)
            {
                free(str);
                return NULL;
            }
            str = temp;
        }
        c = fgetc(students);
        if(c!='\n' && c != ' ' && c!= EOF)
        {
            str[i] = (char) c;
            i++;
        }

    }
    str[i] = '\0';
    if (c == EOF || c == '\n')
    {
        ungetc(c,students);
    }
    return str;
}

student * cpyStudents (FILE* students)
{
    int lineCount = howManyLines(students);
    char* studentID;
    char* name;
    char* surname;
    char* city;
    char* department;
    int TotCredits, GPA;
    student* studentsArr = malloc(sizeof (student_t) * howManyLines(students));
    int c = fgetc(students);
    ungetc(c,students);
    int i = 0;

    while (i < lineCount)
    {

            studentID = getStr(students);
            fscanf(students, " %d %d ", &TotCredits, &GPA);
            name = getStr(students);
            surname = getStr(students);
            city = getStr(students);
            department = getStr(students);
            studentsArr[i] = studentCreate(studentID, TotCredits, GPA, name,surname, city, department);
            i++;
    }

    return studentsArr;

}


course * cpyCourses (FILE* courses)
{
    int lineCount = howManyLines(courses);
    int courseNum, size;
    course* coursesArr = malloc(sizeof (course_t) * howManyLines(courses));
    int c = fgetc(courses);
    ungetc(c,courses);
    int i = 0;
    while (i < lineCount)
    {
        fscanf(courses, " %d %d ", &courseNum, &size);
        coursesArr[i] = courseCreate(NULL, courseNum, size);
        i++;
    }
    return coursesArr;
}

int longestLine (FILE* file)
{
    bool done = false;;
    int max = 0;
    for (int i = 0; i < howManyLines(file); ++i)
    {
        char* line = getLine(file);
        int count = 1;
        for (int j = 0; line[j] != '\0'; ++j)
        {
            if(line[j] == ' ')
            {
                count++;
            }
        }
        if(count > max)
        {
            max = count;
        }
    }

    rewind(file);
    return max;
}

hacker* cpyHackers (FILE* hackers)
{
    int lineCount = howManyLines(hackers);
    char* studentID;
    int* courseNumbers = malloc(sizeof (int) * (howManyLines(hackers)/4));
    char** friends = malloc(sizeof (char*) * (howManyLines(hackers)/4));
    char** rivals = malloc(sizeof (char*) * (howManyLines(hackers)/4));
    hacker* hackersArr = malloc(sizeof (hacker_t) * howManyLines(hackers));
    int i = 0;
    while (i < lineCount / 4)
    {
        studentID = getStr(hackers);
        char* a = getLine(hackers);
        getWords(getLine(hackers) , friends);
        getWords(getLine(hackers), rivals);
        hackersArr[i] = hackerCreate(studentID, courseNumbers, friends, rivals);
        i++;
    }
    return hackersArr;
}

EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers)
{
     EnrollmentSystem  newEnrollment = malloc(sizeof *newEnrollment);
     newEnrollment->students = cpyStudents(students);
     newEnrollment->courses = cpyCourses(courses);
     newEnrollment->hackers = cpyHackers(hackers);
     return newEnrollment;
}

