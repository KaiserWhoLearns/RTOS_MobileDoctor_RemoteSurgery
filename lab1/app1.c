#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//main

int main(void)
{
    while(1) {
        printf("%s","A B C D\n");
        // must need new line character; because the string is first print to a buffer.
        sleep(1);
        system("clear");
        sleep(1);
    }
    return 0;
}
