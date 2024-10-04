// C program to demostrate use of rename
#include <cstdio>

int main()
{
    // Old file name
    char old_name[] = "Input.txt";

    // New file name
    char new_name[] = "Output.txt";

    int value;

    // File name is changed here
    value = rename(old_name, new_name);

    if (!value) {
        printf("%s file renamed to %s\n", old_name, new_name);
    } else {
        perror("Error occurred");
    }
    return 0;
}