//
// Created by noams on 5/8/2023.
//
#include "HackEnrollment.h"
#include <stdlib.h>
#include <stdio.h>



// argc is 6 or 7
// argv is ,name of file. ,flag. ,students file. ,courses file. ,hackers file. ,queues file. ,output file

FILE** openFiles (char** argv)
{
    FILE** files = malloc(sizeof(FILE*) * 5);
    if (files == NULL)
    {
        return NULL;
    }
    int i = 1;
    if(argv[1] == "-i")
    {
        i++;
    }
    int j;
    for (j = 0; j < 4; ++j)
    {
        files[j] = fopen(argv[j+i], "r");
        if(files[j] == NULL)
        {
            for (int k = i; k < j; ++k)
            {
                fclose(files[k]);
            }
            free(files);
            return NULL;
        }
    }
    files[j] = fopen(argv[j+i], "w");
    if(files[j] == NULL)
    {
        for (int k = i; k < j; ++k)
        {
            fclose(files[k]);
        }
        free(files);
        return NULL;
    }
    return files;
}

int main(int argc, char** argv)
{
    if (argc != 6 && argc != 7)
    {
        return 0;
    }

    FILE** files = openFiles(argv);

    EnrollmentSystem  enrollmentSystem = createEnrollment(files[0], files[1], files[2]);
    if(enrollmentSystem == NULL)
    {
        for (int i = 0; i < 5; ++i)
        {
            fclose(files[i]);
        }
        free(files);
        return 0;
    }
    if(argc == 7)
    {
        unCapital(enrollmentSystem);
    }

    enrollmentSystem = readEnrollment(enrollmentSystem, files[3]);

    hackEnrollment(enrollmentSystem, files[4]);

    for (int i = 0; i < 5; ++i)
    {
        fclose(files[i]);
    }
    free(files);
    return 0;

}

