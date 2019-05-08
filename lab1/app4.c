#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//main
int main(void)
{
    int time1 = 0, time2 = 0;
    printf("Please enter two numbers:");
    scanf("%d%d", &time1, &time2);
    //assume time1 <= time2
    int printab = 0;
    int printcd = 0;
    // 0 stand for did not print, 1 stand for printed;
    // if printed, then we clear the screen;
    while(1) {
      for(int i = 0; i < time2; i = i + time1) {
        for(int j = 0; j < time1; j ++) {
          // inner for-loop for the shorter sleep
          sleep(1);
        }
        if(printab == 0) {
          system("clear");
          printf("A C\n");
          printab = 1;
        } else {
          system("clear");
          printab = 0;
        }
        if(printcd == 1) {
          printf("B D\n");
        }
      }
      if(printcd == 0) {
        system("clear");
        printf("B D\n");
        printcd = 1;
      } else {
        system("clear");
        printcd = 0;
      }
    }
    return 0;
}
