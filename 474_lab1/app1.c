#include <stdio.h>
#include <unistd.h>
//main

int main(void)
{
     while(1) {
        printf("A B C D\n");
        // must need new line character; because the string is first print to a buffer.
        sleep(1);
      }
    return 0;
}
