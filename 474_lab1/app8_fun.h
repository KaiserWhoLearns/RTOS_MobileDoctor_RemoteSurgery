#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
void f2Clear(int* delay2);
// pre: take in a pointer of int delay1
void f1Data(int* delay1, int printab, int printcd){
    f2Clear(delay1);
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

// pre: take in a pointer of int delay2
void f2Clear(int* delay2) {
    // sleep for the value of the pointer time
    sleep(*delay2);
	system("clear");
}