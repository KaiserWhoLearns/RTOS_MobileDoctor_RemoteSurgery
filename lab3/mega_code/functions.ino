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

    MeasureData md = *((MeasureData*) dataPtr);
    Serial1.write((char)0x00);
    *(md.temperatureRawPtr) = Serial1.read();
    Serial1.write((char)0x03);
    *(md.pulseRateRawPtr)= Serial1.read();
    Serial1.write((char)0x01);
    *(md.systolicPressRawPtr) = Serial1.read();
    Serial1.write((char)0x02);
    *(md.diastolicPressRawPtr) = Serial1.read();
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

int countt = 0;
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
        tft.fillRect(x, y, 80, 50, RED);
    } else {
        tft.fillRect(x, y, 80, 50, GREEN);
    }
    // See Line595 of Elegoo_GFX.cpp
    tft.setTextSize(2);
    tft.setTextColor(BLUE);
    tft.setCursor(x - 1, y-1);
    if(y == 10) {
        tft.print("Te");
    } else if(y == 90) {
        tft.print("BP");
    } else if(y == 170) {
        tft.print("PR");
    }
}

/*
*   This function is called when the TFT is in menu mode
*   May 9, 2019 by Kaiser
*/
void menu(KeypadData* dataPtr) {
    KeypadData d = *dataPtr;
    // Test 
    Serial.print("Entered menu mode");
    Serial.print("Measure: ");
    Serial.println(*(d.measurementSelectionPtr));
    Serial.print("Ann");
    Serial.println(*(d.alarmAcknowledgePtr));
    // Draw menu
    tft.setCursor(0, 0);
    tft.fillScreen(BLUE);
    drawSub(150, 10, dispT);
    drawSub(150, 90, dispBP);
    drawSub(150, 170, dispPR);
    tft.fillRect(0, 0, 100, 100, GREEN);
    // Get point
    digitalWrite(13, HIGH);
    TSPoint p = ts.getPoint();
    digitalWrite(13, LOW);
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    // If we have point selected
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    // scale from 0->1023 to tft.width
        p.x = (tft.width() - map(p.x, TS_MINX, TS_MAXX, tft.width(), 0));
        p.y = (tft.height()-map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
        Serial.print(p.x);
        Serial.print(", ");
        Serial.println(p.y);
        // while(!T(p.x, p.y) && !BP(p.x, p.y) && !PR(p.x, p.y) && !QUIT(p.x, p.y)) {
        if(T(p.x, p.y)) {
            if(dispT) {
                dispT = FALSE;
            } else {
                dispT = TRUE;
            }
        }
        if(BP(p.x, p.y)) {
            if(dispBP) {
                dispBP = FALSE;
            } else {
                dispBP = TRUE;
            }
        }
        if(PR(p.x, p.y)) {
            if(dispPR) {
                dispPR = FALSE;
            } else {
                dispPR = TRUE;
            }
        }
        if(QUIT(p.x, p.y)) {
            *(d.measurementSelectionPtr) = 0;
        }
    }
    return;
}

/*
*    @param: KeypadData pointer, dataPtr
*    Submethod of select; when called, goes into announciation mode
*    May 10th by Kaiser
*/
void anno(KeypadData* dataPtr) {
    KeypadData d = *dataPtr;
    // Draw exit button
    tft.fillRect(0, 0, 100, 100, RED);
    tft.setTextSize(2);
    tft.setTextColor(BLUE);
    tft.setCursor(95, 95);
    tft.print("Exit");
    (*disp.myTask)(disp.taskDataPtr);
    // getPoint
    digitalWrite(13, HIGH);
    TSPoint p = ts.getPoint();
    digitalWrite(13, LOW);
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    // If we have point selected
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
        // scale from 0->1023 to tft.width
        p.x = (tft.width() - map(p.x, TS_MINX, TS_MAXX, tft.width(), 0));
        p.y = (tft.height()-map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
        if(QUIT(p.x, p.y)) {
            *(d.alarmAcknowledgePtr) = 0;
        }
    }
    return;
}

// int countt = 0;
/*
*   @param: generic pointer dataPtr
*   Assume the dataPtr is of type keyData
*   Goes into mode select; from mode select, we can select or 
*   get announciation data
*   May 8, 2019 by Kaiser Sun
*   May 9, 2019 rewrote by Kaiser
*/
void Select(void* dataPtr) {
    KeypadData kd = *((KeypadData*) dataPtr);
    // When it's select mode
    if(*(kd.measurementSelectionPtr) == 0 && *(kd.alarmAcknowledgePtr) == 0) {
        tft.fillScreen(BLACK);
        tft.fillRect(90, 50, 80, 80, GREEN);
        tft.fillRect(90, 135, 80, 80, YELLOW);
        digitalWrite(13, HIGH);
        TSPoint p = ts.getPoint();
        digitalWrite(13, LOW);
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
        // scale from 0->1023 to tft.width
            p.x = (tft.width() - map(p.x, TS_MINX, TS_MAXX, tft.width(), 0));
            p.y = (tft.height()-map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
            Serial.println(MENU(p.x, p.y));
            if(MENU(p.x, p.y)) {
                *(kd.measurementSelectionPtr) = 1;
                Serial.print("MENU pressed");
                Serial.print(p.x);
                Serial.print(", ");
                Serial.println(p.y);
            } else if(ANN(p.x, p.y)) {
                // If it's announciation, turn to display
                *(kd.alarmAcknowledgePtr) = 1;
                Serial.print("ANN pressed");
                Serial.print(p.x);
                Serial.print(", ");
                Serial.println(p.y);
                Serial.println(*(kd.alarmAcknowledgePtr));
        }
    }
    }
    // Check if it is measure mode or announciation mode
    if(*(kd.measurementSelectionPtr) == 1) {
        menu(&kd);
    } else if(*(kd.alarmAcknowledgePtr) == 1) {
            Serial.println("Announciation mode");
            anno(&kd);
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


