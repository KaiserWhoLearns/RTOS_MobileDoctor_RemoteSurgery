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

  if(time1 != time2) {
    //specifiy that time1 != time2
   while(1) {
      for (int i = 0; i < time2-time1-1; i ++) {
        system("clear");
        for (int j = 0; j < time1-1; j ++) {
          sleep(1);
        }
        printf("A B\n");
        sleep(1);
      }
      system("clear");
      printf("C D\n");
      sleep(1);

    }
  } else {
    //When time1 = time2
    while(1) {
        printf("%s","A B C D\n");
        // must need new line character; because the string is first print to a buffer.
        sleep(1);
        system("clear");
        sleep(time1);
    }
  }
    return 0;
}
