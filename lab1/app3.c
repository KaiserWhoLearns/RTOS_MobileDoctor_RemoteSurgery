#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//main
void delay1s()
{
  sleep(1);
}

int main(void)
{
     while(1) {
        system("clear");
        delay1s();
        printf("A C\n");
        delay1s();
        system("clear");
        delay1s();
        printf("A B C D\n");
        delay1s();
      }
    return 0;
}
