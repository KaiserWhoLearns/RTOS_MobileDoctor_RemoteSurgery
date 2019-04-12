#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void f2Clear(int* delay2);

// pre: take in a pointer of int time1, shorter one, 
// and time 2, longer one
void f1Data(int* time1, int* time2){
    // 0 stand for did not print, 1 stand for printed;
    // if printed, then we clear the screen;
    int printab = 0;
    int printcd = 0;
    while(1) {
	for(int i = 0; i < *time2; i = i + *time1) {
        f2Clear(time1);
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
}

// pre: take in a pointer of int delay2
void f2Clear(int* delay2) {
    // sleep for the value of the pointer time
    sleep(*delay2);
	system("clear");
}