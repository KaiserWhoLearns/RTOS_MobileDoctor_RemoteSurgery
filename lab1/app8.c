#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "app8_fun.h"

int main(void) {
    int time1, time2;
    // Define the pointers
    int* time1Ptr = &time1;
    int* time2Ptr = &time2;
    //  prompt for data
	printf("Please enter two single digits between 0-9 \n");
	//  get the data
	scanf("%d%d", &time1, &time2);
	f1Data(time1Ptr, time2Ptr);
	return 0;
}