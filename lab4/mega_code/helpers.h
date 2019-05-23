#include<stdio.h>
#include "tcb.h"

// the number of recording time; be int, count how many 0.5 sec has passed
int halfsec = 0;

// Time values, to determine whether to blink
unsigned long timef, timeb;


void shift(int newVal,int Bufsize,unsigned int* Buf) {
  for (int i = 1; i <= Bufsize-1; i++) { 
    //if ( (*Buff)[Bufsize - (i+1)] != null)
   // if ( Buf[Bufsize - (i+1)] != NULL) { //if element 6 is not null
      *(Buf + (Bufsize - i)) =  *(Buf + (Bufsize - (i+1)));  //element 7 = element 6
    }
    *Buf = newVal;
  //}
}

void shiftChar(int newVal,int Bufsize,unsigned char* Buf) {
  for (int i = 1; i <= Bufsize-1; i++) { 
    //if ( (*Buff)[Bufsize - (i+1)] != null)
   // if ( Buf[Bufsize - (i+1)] != NULL) { //if element 6 is not null
      *(Buf + (Bufsize - i)) =  *(Buf + (Bufsize - (i+1)));  //element 7 = element 6    //}
  }
    *Buf = newVal;
  
}


/*
*    @param: take in the systolic and diastolic BloodPressure
*    return true if it's high; false otherwise
*    May23, 2019 Kaiser
*/
bool isBPHigh(int sys, int dia) {
    if(sys > 130) {
        if(float(sys - 130)/130 > 0.15) { return true; }
        else { return false; }
    } else if(sys < 120) {
        if(float(120 - sys)/120 > 0.15) { return true; }
        else { return false; }
    }
    if(dia > 80) {
        if(float(dia - 80)/80 > 0.15) { return true; }
        else { return false; }
    } else {
        if(float(70 - dia)/70 > 0.15) { return true; }
        else { return false; }
    }
    return false;
}

/*
*   @param: float Temperature t
*   return true if Temperature high; false otherwise
*/
bool isTHight(float t) {
    return t > 37.8 && (t - 37.8)/37.8 > 0.15 
            || t < 36.1 && (36.1 - 7)/36.1 > 0.15;
}

/*
*   @param: float pulse rate pr
*    return true if it's too out of range(0.15); false otherwise
*/
bool isPRHigh(float pr) {
    return (pr > 100) && (pr - 100) / 100 > 0.15
            || (pr < 60) && (60 - pr) / 60 > 0.15;
}

/*
*   @param: float respiration rate rr
*   return true if it's too out of range(0.15); false otherwise
*/
bool isRRHigh(float rr) {
    return (rr > 25) && (rr - 25) / 25 > 0.15
        || (rr < 12) && (12 - rr) / 12 > 0.15;
}

/*
*    @param: int index, TCB* taskQ;
*    pre: index < length(taskQ);
*    helper method of sechdule function;
*    April 23, 2019 by Kaiser Sun
*/
void run(TCB* taskQ) {
    // Call the function in the taskQ;
    (*taskQ->myTask)(taskQ->taskDataPtr);
}

/*
*    @para: take in x, y, the center of the rectangle
*           bool d, to see whether it is unselected
*    Draw the buttons
*    (700, 250) -> T
*    (700, 500) -> BP
*    (700, 750) -> PR
*/
void drawSub(int x, int y, bool d) {
    Serial.print("Entered drawSub");
    if(!d) {
        tft.fillRect(x, y, 260, 80, RED);
    } else {
        tft.fillRect(x, y, 260, 80, GREEN);
    }
    // See Line595 of Elegoo_GFX.cpp
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.setCursor(x + 10, y + 10);
    if(y == 0) {
        tft.print("BloodPressure");
    } else if(y == 80) {
        tft.print("PulseRate");
    } else if(y == 160) {
        tft.print("Temperature");
    }
}