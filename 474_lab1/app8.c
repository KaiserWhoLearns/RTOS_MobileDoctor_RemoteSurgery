#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "app8_fun.h"

int main(void) {
    int time1, time2;
    int printab = 0, printcd = 0;
    // 0 stand for did not print, 1 stand for printed;
    // if printed, then we clear the screen;
    // Define the pointers
    int* time1Ptr = &time1;
    int* time2Ptr = &time2;
    //  prompt for data
	printf("Please enter a single digit between 0-9 \n");
	//  get the data
	scanf("%d%d", &time1, &time2);
    int times = *time2Ptr/(*time1Ptr);
    while(1) {
		for(int i = 0; i < times; i ++) {
			f1Data(time1Ptr, printab, printcd);
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