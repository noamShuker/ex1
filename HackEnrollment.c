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
    hacker hacker;
}student_t;

typedef struct hacker_t
{
    char* studentID;
    int* courseNumbers;
    char** friends;
    char** rivals;
    student student;
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

void unCapital (EnrollmentSystem sys)
{
    if(sys == NULL || sys->students == NULL)
    {
        return;
    }
    for (int i = 0; sys->students[i] != NULL; ++i)
    {
        char* name = sys->students[i]->name;
        if(name != NULL)
        {
            for (int j = 0; name[j] != "\0"; ++j)
            {
                if (name[j] >= 'A' && name[j] <= 'Z')
                {
                    name[j] += 'a' - 'A';
                }
            }
        }
    }
}

bool sameStr (char* str1, char* str2)
{
    if(str1 == NULL || str2 == NULL)
    {
        return NULL;
    }
    if(strlen(str1) != strlen(str2))
    {
        return false;
    }
    for(int i = 0; str1[i] != '\0' ; ++i)
    {
        if(str1[i] != str2[i])
        {
            return false;
        }
    }
    return true;
}

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
    if(str == NULL)
    {
        return NULL;
    }
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
        str[i] = (char)c;
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
    if(str == NULL || arr == NULL)
    {
        return;
    }
    int i = 0;
    int j = 0;
    int m = 0;
    arr[0] = malloc(sizeof(char) * strlen(str) + 1);
    if (arr[0] == NULL)
    {
        return;
    }
    while (str[i] != '\0')
    {
        if(str[i] == ' ')
        {
            arr[j][m] = '\0';
            m = 0;
            j++;
            arr[j] = malloc(sizeof(char) * strlen(str) + 1);
            if (arr[j] == NULL)
            {
                return;
            }
        }
        arr[j][m] = str[i];
        m++;
        i++;
    }
    arr[j][m] = '\0';
    arr[j+1] = NULL;

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
    if (newStr == NULL)
    {
        return NULL;
    }
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
    if(newStudent == NULL)
    {
        return NULL;
    }
    newStudent->GPA = GPA;
    newStudent->city = strCpy(city);
    newStudent->studentID = strCpy(ID);
    newStudent->name = strCpy(name);
    newStudent->surname = strCpy(surName);
    newStudent->department = strCpy(department);
    newStudent->totalCredits = credits;
    newStudent->hacker = NULL;
    return newStudent;
}

course courseCreate (IsraeliQueue q, int courseNum, int size)
{
     course newCourse = malloc(sizeof *newCourse);
     if(newCourse == NULL)
     {
         return NULL;
     }
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
    if(newHacker == NULL)
    {
        return NULL;
    }
    newHacker->studentID = strCpy(studentID);
    newHacker->courseNumbers = malloc(sizeof(int) * STR_Size);
    newHacker->friends = malloc(sizeof(char*) * STR_Size);
    newHacker->rivals = malloc(sizeof(char*) * STR_Size);
    if(newHacker->courseNumbers == NULL || newHacker->friends == NULL || newHacker->rivals == NULL)
    {
        return NULL;
    }
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
    newHacker->student = NULL;
    return newHacker;
}

char* getStr (FILE* students)
{
    if(students == NULL)
    {
        return NULL;
    }
    char* str = malloc(sizeof (char) * STR_Size);
    if (str == NULL)
    {
        return NULL;
    }
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
    if (studentsArr == NULL)
    {
        return NULL;
    }
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
    if (coursesArr == NULL)
    {
        return NULL;
    }
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
    int longest = longestLine(hackers);
    int lineCount = howManyLines(hackers);
    char* studentID;
    int* courseNumbers = malloc(sizeof (int) * (longest+1));
    char** friends = malloc(sizeof (char*) * (longest+1));
    char** rivals = malloc(sizeof (char*) * (longest+1));
    hacker* hackersArr = malloc(sizeof (hacker_t) * howManyLines(hackers));
    if (hackersArr == NULL || courseNumbers == NULL || friends == NULL || rivals == NULL)
    {
        return NULL;
    }
    int i = 0;
    while (i < lineCount / 4)
    {
        int j = 0;
        studentID = getStr(hackers);
        do
        {
            fscanf(hackers, " %d", &courseNumbers[j]);
            j++;
        } while (fgetc(hackers) != '\n');
        courseNumbers[j] = (int)NULL;
        getWords(getLine(hackers) , friends);
        getWords(getLine(hackers), rivals);
        hackersArr[i] = hackerCreate(studentID, courseNumbers, friends, rivals);
        i++;
    }
    return hackersArr;
}

hacker findHackerById (hacker hacker1, EnrollmentSystem sys)
{
    if(hacker1 == NULL || sys == NULL)
    {
        return NULL;
    }
    for (int i = 0; sys->hackers[i] != NULL; ++i)
    {
        if(sameStr(sys->hackers[i]->studentID, hacker1->studentID))
        {
            return sys->hackers[i];
        }
    }
    return NULL;
}

student findStudentByID (EnrollmentSystem system, char* studentID)
{
    if(system == NULL || studentID == NULL)
    {
        return NULL;
    }
    for (int i = 0; system->students[i] != NULL; ++i)
    {
        if(sameStr(system->students[i]->studentID, studentID) == 0)
        {
            return system->students[i];
        }
    }
    return NULL;
}

EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers)
{
    if(students == NULL || courses == NULL || hackers == NULL)
    {
        return NULL;
    }
     EnrollmentSystem  newEnrollment = malloc(sizeof *newEnrollment);
     if(newEnrollment == NULL)
     {
         return NULL;
     }
     newEnrollment->students = cpyStudents(students);
     newEnrollment->courses = cpyCourses(courses);
     newEnrollment->hackers = cpyHackers(hackers);
    for (int i = 0; newEnrollment->hackers[i] != NULL; ++i)
    {
        student student1 = findStudentByID(newEnrollment, newEnrollment->hackers[i]->studentID);
        if(student1 != NULL)
        {
            newEnrollment->hackers[i]->student = student1;
            student1->hacker = newEnrollment->hackers[i];

        }

    }
     return newEnrollment;
}

student findHacker (EnrollmentSystem system, hacker hacker1)
{
    if(system == NULL || hacker1 == NULL)
    {
        return NULL;
    }
    for (int i = 0; system->students[i] != NULL; ++i)
    {
        if(system->students[i]->studentID == hacker1->studentID)
        {
            return system->students[i];
        }
    }
    return NULL;
}



int friendshipFunction1 (student student1, student student2)
{
    student student;
    if(student1 == NULL || student2 == NULL)
    {
        return (int)NULL;
    }
    hacker hacker1 = student2->hacker;
    if(hacker1 == NULL )
    {
        hacker1 = student1->hacker;
        if(hacker1 == NULL)
        {
            return (int) NULL;
        }
        else
        {
            student = student2;
        }
    }
    else
    {
        student = student1;
    }
    for (int i = 0; hacker1->friends[i] != NULL; ++i)
    {
        if(hacker1->friends[i] == student->studentID)
        {
            return 20;
        }

    }
    for (int i = 0; hacker1->rivals[i] != NULL; ++i)
    {
        if(hacker1->rivals[i] == student->studentID)
        {
            return -20;
        }

    }

    return 0;
}


int friendshipFunction2 (student student1, student student2)
{
    int value1 = 0;
    for (int i = 0; student1->name[i] != '\0' ; ++i)
    {
        value1 += student1->name[i];
    }
    int value2 = 0;
    for (int i = 0; student2->name[i] != '\0' ; ++i)
    {
        value2 += student2->name[i];
    }

    return abs(value1 - value2);

}

int convertASCII (char* id)
{
    if(id == NULL)
    {
        return (int)NULL;
    }
    int value = 0;
    for (int i = 0; id[i] != '\0' ; ++i)
    {
        if(id[i] >= 48 && id[i] <= 57)
        {
            value += id[i] - 48;
        }
    }
    return value;
}

int friendshipFunction3 (student student1, student student2)
{
    if(student2 == NULL || student1 == NULL || student2->studentID == NULL || student1->studentID == NULL)
    {
        return (int)NULL;
    }
    return abs(convertASCII(student2->studentID) - convertASCII(student1->studentID));

}

void cpyQueue (EnrollmentSystem system, FILE* file)
{
    if(system == NULL || file == NULL || system->courses == NULL)
    {
        return;
    }
    FriendshipFunction* functions = {friendshipFunction1, friendshipFunction2, friendshipFunction3, NULL};
    int lineNum = howManyLines(file);
    for (int i = 0; i < lineNum; ++i)
    {
        int courseNum;
        fscanf(file, " %d", &courseNum);
        for(int j = 0; system->courses[j] != NULL; ++j)
        {
            if(system->courses[j]->courseNumber == courseNum)
            {
                system->courses[j]->q = IsraeliQueueCreate(functions,NULL,20,10);
                char* valueId;
                do
                {
                    valueId = getStr(file);
                    student temp = findStudentByID(system, valueId);
                    student value = studentCreate(temp->studentID, temp-> totalCredits, temp->GPA,  temp->name, temp->surname, temp->city, temp->department);
                    value->hacker = temp->hacker;
                    QueueEnqueue(system->courses[j]->q, nodeCreate((void*)value));

                }while (fgetc(file) != '\n');


            }
        }

    }
}

EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues)
{
    if(sys == NULL || queues == NULL)
    {
        return NULL;
    }
    cpyQueue(sys, queues);
    return sys;
}

char* cpyStr (char* str)
{
    if(str == NULL)
    {
        return NULL;
    }
    char* newStr = malloc(sizeof (char) * (strlen(str) + 1));
    if(newStr == NULL)
    {
        return NULL;
    }
    int i;
    for( i = 0; str[i] != '\0' ; ++i)
    {
        newStr[i] = str[i];
    }
    newStr[i] = '\0';
    return newStr;
}



void writeCoursesQueue (FILE* out, EnrollmentSystem sys)
{
    if(sys == NULL || out == NULL || sys->courses == NULL)
    {
        return;
    }
        for(int j = 0; sys->courses[j] != NULL; ++j)
        {
            if(sys->courses[j]->courseNumber != (int)NULL)
            {
                int courseNum = sys->courses[j]->courseNumber;
                if(sys->courses[j]->q == NULL)
                {
                    fprintf(out, " %d", courseNum);
                    fprintf(out, "\n");
                }
                else
                {
                    fprintf(out, " %d ", courseNum);
                    IsraeliQueue temp = IsraeliQueueClone(sys->courses[j]->q);
                    student student1 = (student) IsraeliQueueDequeue(temp);
                    char *value = cpyStr(student1->studentID);
                    while (temp != NULL)
                    {
                        fprintf(out, "%s ", value);
                        student1 = (student) IsraeliQueueDequeue(temp);
                        value = cpyStr(student1->studentID);
                    }
                    fprintf(out, "\n");
                    free(temp);
                }
            }

        }

}


int returnPosition (IsraeliQueue queue , char* value)
{
    if(queue == NULL)
    {
        return (int)NULL;
    }
    IsraeliQueue q = IsraeliQueueClone(queue);
    int i = 1;
    student temp = (student)IsraeliQueueDequeue(q);
    while (temp != NULL)
    {

        if(sameStr(temp->studentID, value))
        {
            return i;
        }
        temp = (student)IsraeliQueueDequeue(q);
        i++;
    }
    return (int)NULL;
}

bool hackWorked (EnrollmentSystem sys, hacker hacker1)
{
    int countRequired = 0;
    int countGot = 0;
    if(hacker1 == NULL || hacker1->courseNumbers == NULL)
    {
        return NULL;
    }
    for (int j = 0; hacker1->courseNumbers[j] != (int)NULL ; ++j)
    {
        countRequired++;
    }
    if(sys == NULL || hacker1 == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < countRequired; ++i)
    {
        int courseNum = hacker1->courseNumbers[i];
        for (int j = 0; sys->courses[j] != NULL; ++j)
        {
            if(sys->courses[j]->courseNumber == courseNum)
            {
                if(sys->courses[j]->q == NULL)
                {
                    return false;
                }
                int position = returnPosition(sys->courses[j]->q, hacker1->studentID);
                if(position != (int)NULL && position < sys->courses[j]->size)
                {
                    countGot++;
                }
            }
        }
    }
    return countGot >= 2 || countRequired == countGot;
}

course findCourseByNum (EnrollmentSystem sys, int num)
{
    if(sys == NULL || sys->courses == NULL)
    {
        return NULL;
    }
    for (int i = 0; sys->courses[i] != NULL ; ++i)
    {
        if(sys->courses[i]->courseNumber == num)
        {
            return sys->courses[i];
        }
    }
    return NULL;
}

void hackEnrollment(EnrollmentSystem sys, FILE* out)
{
    if(sys == NULL || out == NULL)
    {
        return;
    }
    for (int i = 0; sys->hackers[i] != NULL ; ++i)
    {
        for (int j = 0; sys->hackers[i]->courseNumbers[j] != (int)NULL; ++j)
        {
            course temp = findCourseByNum(sys, sys->hackers[i]->courseNumbers[j]);
            IsraeliQueueEnqueue(temp->q, (void*)sys->hackers[i]->student);
        }
        if(hackWorked(sys, sys->hackers[i]))
        {
            writeCoursesQueue(out, sys);
        }
        else
        {
            fprintf(out, "Cannot satisfy constraints for <%s>\n", sys->hackers[i]->studentID);
        }
    }
}