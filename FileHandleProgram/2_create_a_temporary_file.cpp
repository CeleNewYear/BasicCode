// C++ program to create a temporary file
// read and write a temporary file

/*
Temporary file in C++ can be created using the tmpfile() method
defined in the <cstdio> header file. The temporary file created
has a unique auto-generated filename. The file created is opened
in binary mode and has access mode “wb+”. These temporary files
are automatically deleted when the program is terminated or when
they are closed in the program using fclose().
*/
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <windows.h>
using namespace std;

int main()
{
    // create a file pointer which points to the temporary file
    // created by tmpfile() method
    FILE* fp = tmpfile();

    // Content to be written in temporary file
    char write[] = "Welcome to China";

    // If file pointer is NULL then file could not be created
    if (fp == NULL) {
        perror("Error in creating temporary file");
        exit(1);
    }

    // write in temporary file using fputs()
    fputs(write, fp);
    // rewind the file pointer to the beginning of the file
    rewind(fp);

    // read from temporary file using fgets()
    char read[100];
    fgets(read, 100, fp);
    cout << read << endl;

    // close the file pointer
    // file will be deleted automatically
    fclose(fp);
    return 0;
}