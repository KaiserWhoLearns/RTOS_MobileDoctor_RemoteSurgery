#include "tcb.h"

/*
*    @para: void* dataPtr, we assume it's MeasureData pointer; integer isEven, check if it's even;
*    isEven range: 0 or 1;
*    Increase temperatureRaw by 2 even number time, decrease by 1 odd times called before reach 50; 0 is even;
*    Then reverse the process until temperatureRaw falls below 15 
*    systolicPressRaw  - even: Increase by 3; - odd: decrease by 1; Range: no larger than 100
*    diastolicPressRaw -even: decrease by 2; - odd: increase by 1; Range: no less than 40
*    pulseRateRaw - even: decrease by 1; - odd: increase by 3; Range: 15-40
*    April, 22, 2019 by Kaiser
*/
Bool trIsReverse = FALSE, prIsReverse = FALSE, isEven = TRUE;
void Measure(void* dataPtr)
{
    // dereference the data pointer;
    MeasureData md = *((MeasureData*) dataPtr);  
        // When the function is executed even times;
        // Update the data;
        if(isEven) {
            if(*(md.systolicPressRawPtr) <= 100) {
                *(md.systolicPressRawPtr) += 3;
            }
            if(*(md.diastolicPressRawPtr) >= 40) {
                *(md.diastolicPressRawPtr) -= 2;
            }
            if(!trIsReverse) {
                *(md.temperatureRawPtr) += 2;
            } else {
                *(md.temperatureRawPtr) -= 2;
            }
            if(!prIsReverse) {
                *(md.pulseRateRawPtr) -= 1;
            } else {
                *(md.pulseRateRawPtr) += 1;
            }
        } else {
            if(*(md.systolicPressRawPtr) <= 100) {
                *(md.systolicPressRawPtr) -= 1;
            }
            if(*(md.diastolicPressRawPtr) >= 40) {
                *(md.diastolicPressRawPtr) += 1;
            }
            if(!trIsReverse) {
                *(md.temperatureRawPtr) -= 2;
            } else {
                *(md.temperatureRawPtr) += 2;
            }
            if(!prIsReverse) {
                *(md.pulseRateRawPtr) += 1;
            } else {
                *(md.pulseRateRawPtr) -= 1;
            }
        }

        // Update isReverse;
        if(*(md.temperatureRawPtr) > 50) {
            trIsReverse = TRUE;
        } else if (*(md.temperatureRawPtr) < 15)
        {
            trIsReverse = FALSE;
        }

        if(*(md.pulseRateRawPtr) > 40) {
            prIsReverse = TRUE;
        } else if (*(md.pulseRateRawPtr) < 15)
        {
            prIsReverse = FALSE;
        }

        // Update isEven;
        if(isEven) {
            isEven = FALSE;
        } else {
            isEven = TRUE;
        }
        return;
}

/*
*    @param: char array arr, integer val;
*    Transfer the integer value in val to arr;
*    helper method for Compute
*    April 24, 2019 by Kaiser Sun
*/
void intToChar(char* arr, int val) {
    arr[0] = (char) (val / 100 + 48);
    arr[1] = (char) (val % 100 / 10 + 48);
    arr[2] = (char) (val % 10 + 48);
    return;
}

/*
*    @para: generic pointer dataPtr;
*    Assume the data pointer is of type ComputeData
*    Compute the corrected values of data
*    April 23, 2019 by Kaiser Sun
*    Change the type of corrected to char[]
*    April 24, 2019 by Kaiser Sun
*/
void Compute(void* dataPtr) {
    ComputeData comd = *((ComputeData*) dataPtr);
    int tempC= (*(comd.temperatureRawPtr)) * 0.75 + 5;
    int sysPC =(*(comd.systolicPressRawPtr)) * 2 + 9;
    int diasC = (*(comd.diastolicPressRawPtr)) * 1.5 + 6;
    int prC = (*(comd.pulseRateRaw)) * 3 + 8;
    intToChar(comd.tempCorrectedPtr, tempC);
    intToChar(comd.sysPressCorrectedPtr, sysPC);
    intToChar(comd.diastolicPressCorrectedPtr, diasC);
    intToChar(comd.pulseRateCorrectedPtr, prC);
    return;
}


/*
*    @para: generic pointer dataPtr;
*    Assume the data pointer is of type DisplayData
*    Display the data on the TFT display
*    April 23, 2019 by Kaiser Sun
*/
void Display(void* dataPtr) {
    // Setup of tft display
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
    tft.setTextSize(2);
    // Pointer dereference
    DisplayData dd = *((DisplayData*) dataPtr);
    // Display Pressure
    if(bpOutOfRange == 0) {
        tft.setTextColor(GREEN);
    } else {
        tft.setTextColor(RED);
    }
    tft.print("Systolic Pressure: ");
    tft.print(*(dd.sysPressCorrectedPtr));
    tft.print("mmHg   Diastolic Pressure: ");
    tft.print(*(dd.diasCorrectedPtr));
    tft.println(" mmHg");

    // print temperature
    if(tempOutOfRange == 0) {
        tft.setTextColor(GREEN);
    } else {
        tft.setTextColor(RED);
    }
    tft.print("Temperature: ");
    tft.print(*(dd.tempCorrectedPtr));
    tft.print("C");

    // Display pulse
    if(pulseOutOfRange == 0) {
        tft.setTextColor(GREEN);
    } else {
        tft.setTextColor(RED);
    }
    tft.print("    Pulse Rate: ");
    tft.print(*(dd.prCorrectedPtr));
    tft.print("BPM   ");

    // Display battery status
    if(*(dd.batteryStatePtr) > 20) {
        tft.setTextColor(GREEN);
    } else {
        tft.setTextColor(RED);
    }
    tft.print("Battery: ");
    tft.print(*(dd.batteryStatePtr)); 
    return;
}

/*
*    @param: generic pointer dataPtr;
*    assume the dataPtr is of type dataPtr;
*    if the data are out of range, diplay with red;
*    April 23, 2019 by Kaiser Sun
*/
void WarningAlarm(void* dataPtr) {
    WarningAlarmData wad = *((WarningAlarmData*) dataPtr);
    if (*(wad.temperatureRawPtr) > 37.8 || *(wad.temperatureRawPtr) < 36.1) {
        tempOutOfRange = 1;
    } else {
        tempOutOfRange = 0;
    }
    if(*(wad.systolicPressRawPtr) > 120 || *(wad.diastolicPressRawPtr) > 80) {
        bpOutOfRange = 1;
    } else {
        bpOutOfRange = 0;
    }
    if(*(wad.pulseRateRawPtr) < 60 || *(wad.pulseRateRawPtr) > 100) {
        pulseOutOfRange = 1;
    } else {
        pulseOutOfRange = 0;
    }
    return;  
}

/*
*    @param: generic pointer dataPtr;
*    Assume the dataPtr is of type StatusData;
*    BatteryState shall decrease by 1 each it is called;
*    April 23, 2019 by Kaiser Sun
*/
void Status(void* dataPtr) {
    StatusData sd = *((StatusData*) dataPtr);
    *(sd.batteryStatePtr) -= 1;
    return;
}

/*
*    @param: int index, TCB* taskQ;
*    pre: index < length(taskQ);
*    helper method of sechdule function;
*    April 23, 2019 by Kaiser Sun
*/
void run(int index, TCB* taskQ) {
    // Call the function in the taskQ;
    (*taskQ[index].myTask)((taskQ[index]).taskDataPtr);
}

/*
*    @param: schedule each task time; 
*    each task has maximum 5s to execute;
*    April 23, 2019 by Kasier Sun
*/
// void scheduler(TCB* taskQ) {

// }

/*
*    @param: array of TCB taskQ
*    pre: assume length(taskQ) = 5
*    helper method of scheduler, used to measure the time
*    that each task takes to execute; should be commented
*    after development;
*    April 23, 2019 by Kaiser Sun
*/
void sechdulerTest(TCB* taskQ) {
        unsigned long time0 = micros();
        run(0, taskQ);
        unsigned long time1 = micros();
        run(1, taskQ);
        unsigned long time2 = micros();
        run(2, taskQ);
        unsigned long time3 = micros();
        run(3, taskQ);
        unsigned long time4 = micros();
        run(4, taskQ);
        unsigned long time5 = micros();
        Serial.print("First: ");
        Serial.println(time1 - time0);
        Serial.print("Second: ");
        Serial.println(time2 - time1);
        Serial.print("Third: ");
        Serial.println(time3 - time2);
        Serial.print("Fourth: ");
        Serial.println(time4 - time3);
        Serial.print("Fifth: ");
        Serial.println(time5 - time4);
        delay(1000);
}
