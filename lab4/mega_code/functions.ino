#include "tcb.h"
#include "helpers.h"

bool dispBP = TRUE;
bool dispT = TRUE;
bool dispPR = TRUE;
bool dispRR = TRUE;

// By high we mean 15% out of range
bool bpHigh = FALSE;
bool tempHigh = FALSE;
bool prHigh = FALSE;
bool rrLow = FALSE;
bool rrHigh = FALSE;

unsigned char bpOutOfRange = 0;
unsigned char tempOutOfRange = 0;
unsigned char pulseOutOfRange = 0;
unsigned char rrOutOfRange = 0;

bool trIsReverse = FALSE, prIsReverse = FALSE, isEven = TRUE;



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
void Measure(void* dataPtr)
{
    MeasureData md = *((MeasureData*) dataPtr);
    Serial1.write('s');
    if(dispT) { 
        Serial1.write('t'); 
        // Uncomment the delay if we are measuring using interrupt
        // delay(500);
        int newTemp = Serial1.read();
        shift(newTemp, 8, (md.temperatureRawBuf));
    }

    if(dispBP) { 
        Serial1.write('b'); 
        int newSys = Serial1.read();
        int newDia = Serial1.read();
        shift(newSys, 16, (md.bloodPressRawBuf));
        shift(newDia, 16, (md.bloodPressRawBuf));
    }
    if(dispPR) { 
        Serial1.write('p');
        int newPr = Serial1.read();
        shift(newPr, 8, (md.pulseRateRawBuf)); 
    }
    if(dispRR) { 
        Serial1.write('r'); 
        int newRR = Serial1.read();
        shift(newRR, 8, (md.respirationRateRawBuf));
    }
    Serial1.write('e');
    
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
    // TODO : change by disp values
  ComputeData comd = *((ComputeData*) dataPtr);
      int correctedTemp = (*(comd.temperatureRawBuf)) * 0.75 + 5;
      int correctedDia = (*(comd.bloodPressRawBuf)) * 1.5 + 6;
      int correctedSys = (*(comd.bloodPressRawBuf + 1)) * 2 + 9;
      int correctedPr = (*(comd.pulseRateRawBuf)) * 3 + 8; 
      Serial.print("Corrected pulse rate");
      Serial.println(correctedPr);
      shiftChar(correctedTemp, 8, (comd.tempCorrectedBuf));
      shiftChar(correctedPr, 8, (comd.prCorrectedBuf));
      shiftChar(correctedSys, 16, (comd.bloodPressCorrectedBuf));
      shiftChar(correctedDia, 16, (comd.bloodPressCorrectedBuf));
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
    // TODO: change the color of display!

    // Setup of tft display
    tft.fillScreen(GREY);
    tft.setCursor(0, 0);
    // Pointer dereference
    DisplayData dd = *((DisplayData*) dataPtr);
    // Starter
    tft.setTextColor(CYAN);
    tft.setTextSize(2);
    tft.println("   Mobile Doctor");
    tft.setTextSize(2);
    // Display Pressure
    if(dispBP) {
        if(bpOutOfRange == 0) {
            tft.setTextColor(GREEN);
        } else {
            if(bpHigh) {
            tft.setTextColor(RED);
            } else {
                tft.setTextColor(YELLOW);
            }
        }
        tft.print("Systolic Pressure: ");
        tft.print(*(dd.bloodPressCorrectedBuf + 1));
        tft.println(" mmHg");
        tft.print("Diastolic Pressure: ");
        tft.print(*(dd.bloodPressCorrectedBuf));
        tft.println(" mmHg");
    }

    // print temperature
    if(dispT) {
        if(tempOutOfRange == 0) {
            tft.setTextColor(GREEN);
        } else {
            if(tempHigh) {
            tft.setTextColor(RED);
            } else {
            tft.setTextColor(YELLOW);
            }
        }
        tft.print("Temperature: ");
        tft.print(*(dd.tempCorrectedBuf));
        tft.println(" C");
    }

    // Display pulse
    if(dispPR) {
        if(pulseOutOfRange == 0) {
            tft.setTextColor(GREEN);
        } else {
            if(prHigh) {
            tft.setTextColor(RED);
            } else {
            tft.setTextColor(YELLOW);
            }
        }
        tft.print("Pulse Rate: ");
        tft.print(*(dd.prCorrectedBuf));
        tft.println("BPM");
    }

    // Display respiration rate
    if(dispRR) {
        if(rrOutOfRange == 0) {
            tft.setTextColor(GREEN);
        } else {
            if(rrHigh) {
            tft.setTextColor(RED);
            } else {
            tft.setTextColor(YELLOW);
            }
        }
    }
    tft.print("Respiration Rate: ");
    tft.print(*(dd.respirationRateCorrectedBuf));
    tft.println(" /s");

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
    
    if(prHigh) {
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
    if (*(wad.temperatureRawBuf) > 37.8 || *(wad.temperatureRawBuf) < 36.1) {
        tempOutOfRange = 1;
        tempHigh = isTHight(float(*(wad.temperatureRawBuf)));
    } else {
        tempOutOfRange = 0;
    }
    if(*(wad.bloodPressRawBuf + 1) < 130 || *(wad.bloodPressRawBuf) > 80 || *(wad.bloodPressRawBuf + 1) < 120 || *(wad.bloodPressRawBuf) < 70) {
        bpOutOfRange = 1;
        //sys: 1, Dia: 0
        bpHigh = isBPHigh(*(wad.bloodPressRawBuf + 1), *(wad.bloodPressRawBuf));
    } else {
        bpOutOfRange = 0;
    }
    if(*(wad.pulseRateRawBuf) < 60 || *(wad.pulseRateRawBuf) > 100) {
        pulseOutOfRange = 1;
        prHigh = isPRHigh(float(*(wad.pulseRateRawBuf)));
    } else {
        pulseOutOfRange = 0;
    }
    if(*(wad.rrRawBuf) < 12 || *(wad.rrRawBuf) > 25) {
        rrOutOfRange = 1;
        rrHigh = isRRHigh(float(*(wad.rrRawBuf)));
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
*   This function is called when the TFT is in menu mode
*   May 9, 2019 by Kaiser
*/
void menu(KeypadData* dataPtr) {
    KeypadData d = *dataPtr;
    // // Test 
    // Serial.print("Entered menu mode");
    // Serial.print("Measure: ");
    // Serial.println(*(d.measurementSelectionPtr));
    // Serial.println(*(d.alarmAcknowledgePtr));
    // Draw menu
    tft.setCursor(0, 0);
    tft.fillScreen(BLACK);
    drawSub(70, 0, dispBP);
    drawSub(70, 80, dispPR);
    drawSub(70, 160, dispT);
    tft.fillRect(0, 0, 70, 240, MAGENTA);
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.setCursor(5, 100);
    tft.print("Exit");
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
        if(QUIT1(p.x, p.y)) {
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
    tft.fillRect(0, 180, 330, 60, RED);
    tft.setTextSize(2);
    tft.setTextColor(BLUE);
    tft.setCursor(150, 200);
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
        if(QUIT2(p.x, p.y)) {
            *(d.alarmAcknowledgePtr) = 0;
        }
    }
    return;
}

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
        tft.fillRect(0, 0, 165, 120, GREEN);
        // Not used button
        tft.fillRect(0, 120, 165, 240, BLUE);
        // Upper: not used button
        tft.setTextSize(2);
        tft.setTextColor(BLACK);
        tft.setCursor(10, 100);
        tft.print("MENU");
        tft.fillRect(165, 0, 165, 120, YELLOW);
        // Not used button
        tft.fillRect(165, 120, 165, 240, CYAN);
        // Upper: not used button
        tft.setCursor(230, 100);
        tft.print("Announ");
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
*   @param: data pointer of communications data
*   Receive the command from remote subsystem
*   change the display status; 's' is start of message
*   'e' is end of message
*   Order: temperature, pressure, pulserate, respiration rate
*   May 29th, 2019 by Kaiser
*/
char t = 'q';
void Communications(void* dataPtr) {
    CommunicationsData cd = *((CommunicationsData*) dataPtr);
    if(Serial.available() > 0) {
        if(Serial.read() == 's') {
            int i = 0;
            t = Serial.read();
            while(t != 'e') {
                // When they did not read end of message, keep loop
                if(t == 't') {
                    Serial.write("Temperature: ");
                    Serial.println(*(cd.tempCorrectedBuf));
                    Serial.println(" ");
                    if(dispT) { dispT = false; } else {dispT = true; }
                }
                if(t == 'b') {
                    Serial.write("Systolic Pressure: ");
                    Serial.println(*(cd.bloodPressCorrectedBuf + 1));
                    Serial.println(" ");
                    Serial.write("Diastolic Pressure: ");
                    Serial.println(*(cd.bloodPressCorrectedBuf));
                    Serial.println(" ");
                    if(dispBP) { dispBP = false; } else { dispBP = true; }
                }
                if(t == 'p') {
                    Serial.write("Pulse Rate: ");
                    Serial.println(*(cd.prCorrectedBuf));
                    Serial.println(" ");
                    if(dispPR) { dispPR = false; } else { dispPR = true; }
                }
                if(t == 'r') {
                    Serial.write("Respiration Rate: ");
                    Serial.println(*(cd.respirationRateCorBufPtr));
                    Serial.println(" ");
                    if(dispRR) {dispRR = false; } else { dispRR = true; }
                }
                t = Serial.read();
            }
        }
        t = 'e';
    }
    return;
}


/*
*    @param: array of TCB taskQ
*    pre: assume length(taskQ) = 5
*    helper method of scheduler, used to measure the time
*    that each task takes to execute; should be commented
*    after development;
*    April 23, 2019 by Kaiser Sun
*/
void sechdulerTest() {
        TCB* taskQueue = front;

        while (taskQueue != NULL) {
            run(taskQueue);
            taskQueue = taskQueue->next;
        }
}


/*
*    Execute only at the Setup function once
*    Start the system timer, arrange the taskQueue
*    May 22, 2019 by Kaiser
*/
void startUp() {
    // Setup the data structs
  meaD = MeasureData{temperatureRawPtrr, bloodPressRawPtrr, pulseRateRawPtrr, respirationRateRawPtr, measurementSelectionPtr};
  cD = ComputeData{temperatureRawPtrr, bloodPressRawPtrr, pulseRateRawPtrr, respirationRateRawPtr,tempCorrectedPtrr, bloodPressCorrectedPtrr, pulseRateCorrectedPtrr, respirationRateCorPtr,measurementSelectionPtr};
  dDa = DisplayData{tempCorrectedPtrr, bloodPressCorrectedPtrr, pulseRateCorrectedPtrr, respirationRateCorPtr, batteryStatePtrr};
  wAD = WarningAlarmData{temperatureRawPtrr, bloodPressRawPtrr, pulseRateRawPtrr, respirationRateRawPtr, batteryStatePtrr};
  sD = StatusData{batteryStatePtrr};
  kD = KeypadData{localFunctionSelectPtr, measurementSelectionPtr, alarmAcknowledgePtr, commandPtr, remoteFunctionSelectPtr, measurementResultSelectionPtr};
  comD = CommunicationsData{tempCorrectedPtrr, bloodPressCorrectedPtrr, pulseRateCorrectedPtrr, respirationRateCorPtr};
// Setup the TCBs
  meas = {&Measure, &meaD};
  comp = {&Compute, &cD};
  disp = {&Display, &dDa};
  alar = {&WarningAlarm, &wAD};
  stat = {&Status, &sD};
  keyp = {&Select, &kD};
  com = {&Communications, &comD};
  
  // Setup task queue
  insertLast(&meas);
  insertLast(&comp);
  insertLast(&stat);
  insertLast(&alar);
  insertLast(&com);
  //insertLast(&keyp);
  time1 = millis();
  timeb = time1;
  // taskQueue[4] = disp;
  run(&keyp);

}
