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
    int time1 = 0, time2 = 0;
    printf("Please enter two numbers:");
    scanf("%d%d", &time1, &time2);
    //assume time1 <= time2
    int printab = 0;
    int printcd = 0;
    while(1) {
      for(int i = 0; i < time2; i = i + time1) {
        for(int j = 0; j < time1; j ++) {
          sleep(1);
        }
         if(printab == 0) {
          system("clear");
          printf("A B\n");
          printab = 1;
        } else {
          system("clear");
          printab = 0;
        }
        if(printcd == 1) {
          printf("C D\n");
        }
      }
      if(printcd == 0) {
        system("clear");
        printf("C D\n");
        printcd = 1;
      } else {
        system("clear");
        printcd = 0;
      }
    }
    return 0;
}
