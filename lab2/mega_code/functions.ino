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
                *(md.temperatureRawPtr) -= 1;
            } else {
                *(md.temperatureRawPtr) += 1;
            }
            if(!prIsReverse) {
                *(md.pulseRateRawPtr) += 3;
            } else {
                *(md.pulseRateRawPtr) -= 3;
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
*    @para: generic pointer dataPtr;
*    Assume the data pointer is of type ComputeData
*    Compute the corrected values of data
*    April 23, 2019 by Kaiser Sun
*    Change the type of corrected to char[]
*    April 24, 2019 by Kaiser Sun
*/
void Compute(void* dataPtr) {
    ComputeData comd = *((ComputeData*) dataPtr);
    *(comd.tempCorrectedPtr) = (*(comd.temperatureRawPtr)) * 0.75 + 5;
    *(comd.sysPressCorrectedPtr) = (*(comd.systolicPressRawPtr)) * 2 + 9;
    *(comd.diasCorrectedPtr) = (*(comd.diastolicPressRawPtr)) * 1.5 + 6;
    *(comd.prCorrectedPtr) = (*(comd.pulseRateRawPtr)) * 3 + 8;
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
    tft.fillScreen(GREY);
    tft.setCursor(0, 0);
    // Pointer dereference
    DisplayData dd = *((DisplayData*) dataPtr);
    // Starter
    tft.setTextColor(CYAN);
    tft.setTextSize(2);
    tft.println("   Mobile Doctor");
    tft.setTextSize(1.7);
    // Display Pressure
    if(bpOutOfRange == 0) {
        tft.setTextColor(GREEN);
    } else {
        tft.setTextColor(RED);
    }
    tft.print("Systolic Pressure: ");
    tft.print(*(dd.sysPressCorrectedPtr));
    tft.println(" mmHg");
    tft.print("Diastolic Pressure: ");
    tft.print(*(dd.diastolicPressCorrectedPtr));
    tft.println(" mmHg");

    // print temperature
    if(tempOutOfRange == 0) {
        tft.setTextColor(GREEN);
    } else {
        tft.setTextColor(RED);
    }
    tft.print("Temperature: ");
    tft.print(*(dd.tempCorrectedPtr));
    tft.println(" C");

    // Display pulse
    if(pulseOutOfRange == 0) {
        tft.setTextColor(GREEN);
    } else {
        tft.setTextColor(RED);
    }
    tft.print("Pulse Rate: ");
    tft.print(*(dd.pulseRateCorrectedPtr));
    tft.println("BPM");

    // Display battery status
    if(*(dd.batteryStatePtr) > 20) {
        tft.setTextColor(GREEN);
    } else {
        tft.setTextColor(RED);
    }
    tft.print("Battery: ");
    tft.println(*(dd.batteryStatePtr)); 
    // Display too high or low warnings
    tft.setTextSize(1.5);
    tft.setTextColor(RED);
    if(tempHigh) {
        tft.println("Your body temperature is too high! Calm down QWQ");
    }

    if (bpHigh) {
        tft.println("Your blood pressure is too high! Calm down QWQ");
    }
    
    if(pulseLow) {
        tft.println("Your heart beat is too slow. Do something exciting 0w0");
    }

    // End
    tft.setTextSize(1.4);
    tft.setTextColor(CYAN);
    tft.println("                  CSE 474 Inc.");
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
        bpHigh = TRUE;
    } else {
        bpOutOfRange = 0;
        bpHigh = FALSE;
    }
    if(*(wad.pulseRateRawPtr) < 60 || *(wad.pulseRateRawPtr) > 100) {
        pulseOutOfRange = 1;
    } else {
        pulseOutOfRange = 0;
    }
    
    // TODO : Make change to the warnings(comfirm the values)
    if(*(wad.temperatureRawPtr) > 37.8) {
        tempHigh = TRUE;
    } else {
        tempHigh = FALSE;
    }

    if(*(wad.pulseRateRawPtr) < 60) {
        pulseLow = TRUE;
    } else {
        pulseLow = FALSE;
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
    if(*(sd.batteryStatePtr) > 0) {
        *(sd.batteryStatePtr) -= 1;
    }
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
