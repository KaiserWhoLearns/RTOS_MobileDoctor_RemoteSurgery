#include <stdio.h>
#include <unistd.h>
//main

int main(void)
{
     while(1) {
        printf("A B C D");
        // must need new line character; because the string is first print to a buffer.
        sleep(1);
	fflush(stdout);
      }
    return 0;
}
