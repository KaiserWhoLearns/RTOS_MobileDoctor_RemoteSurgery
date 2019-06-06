#include<stdio.h>
#include "tcb.h"

// the number of recording time; be int, count how many 0.5 sec has passed
int halfsec = 0;


// Time values, to determine whether to blink
unsigned long timef;



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
    if(sys > 60.5) {
        if(float(sys - 60.5)/60.5 > 0.2) { 
          
          return true; 
        }
        else { return false; }
    } else if(sys < 55.5) {
        if(float(55.5 - sys)/55.5 > 0.2) { 
          
          return true; 
        }
        else { return false; }
    }
    if(dia > 49.3) {
        if(float(dia - 49.3)/49.3 > 0.2) { 
          
          return true; 
        }
        else { return false; }
    } else {
        if(float(42.7 - dia)/42.7 > 0.2) { 
          
          return true; 
        }
        else { return false; }
    }
    return false;
}

/*
*   @param: float Temperature t
*   return true if Temperature high; false otherwise
*/
bool isTHight(float t) {
    return t > 43.7 && (t - 43.7)/43.7 > 0.15 
            || t < 41.5 && (41.5 - t)/41.5 > 0.15;
}

/*
*   @param: float pulse rate pr
*    return true if it's too out of range(0.15); false otherwise
*/
bool isPRHigh(float pr) {
    return (pr > 30.7) && (pr - 30.7) / 30.7 > 0.15
            || (pr < 17.3) && (17.3 - pr) / 17.3 > 0.15;
}

/*
*   @param: float respiration rate rr
*   return true if it's too out of range(0.15); false otherwise
*/
bool isRRHigh(float rr) {
    return (rr > 6) && (rr - 6) / 6 > 0.15
        || (rr < 1.67) && (1.67 - rr) / 1.67 > 0.15;
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
        tft.fillRect(x, y, 260, 60, RED);
    } else {
        tft.fillRect(x, y, 260, 60, GREEN);
    }
    // See Line595 of Elegoo_GFX.cpp
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
//    tft.setCursor(x + 10, y + 10);
     tft.setCursor(x + 10, y + 10);
    if(y == 0) {
        tft.print("BloodPressure");
    } else if(y == 60) {
        tft.print("PulseRate");
    } else if(y == 120) {
        tft.print("Temperature");
    } else {
        tft.print("RespirationRate");
    }
}

bool moreThan15(int val, unsigned int* Buf) {
    return float(abs(val - *Buf))/(*(Buf))> 0.15;
}
