int temperatureRaw, systolicPressRaw, diastolicPressRaw, pulseRateRaw, batteryState;

typedef struct
{
    void (*myTask)(void*);
    void* taskDataPtr; 
} TCB;


typedef struct
{
    int* temperatureRawPtr;
    int* systolicPressRawPtr;
    int* diastolicPressRawPtr;
    int* pulseRateRawPtr;
} MeasureData;

typedef struct
{
    int* temperatureRawPtr;
    int* systolicPressRawPtr;
    int* diastolicPressRawPtr;
    int* pulseRateRawPtr;
    int* tempCorrectedPtr;
    int* sysPressCorrectedPtr;
    int* diasCorrectedPtr;
    int* prCorrectedPtr;
} ComputeData;

typedef struct
{
    int* tempCorrectedPtr;
    int* sysPressCorrectedPtr;
    int* diasCorrectedPtr;
    int* prCorrectedPtr;
    int* batteryStatePtr;
} DisplayData;

typedef struct
{
    int* temperatureRawPtr;
    int* systolicPressRawPtr;
    int* diastolicPressRawPtr;
    int* pulseRateRawPtr;
    int* batteryStatePtr; 
} WarningAlarmData;

typedef struct
{
    int* batteryStatePtr;
} Status;

typedef struct
{

} SchedulerData;


void setup()
{
  // running on the mega - connect to tx1 and rx1 on the mega and to rx and tx on the uno
  // start serial1 port at 9600 bps and wait for serial1 port to open:
  Serial1.begin(9600);

}

void loop()
{
  //  send some test characters from the mega to the uno
//   Serial1.println("A");
//   delay(1000);
//   Serial1.println("B");
//   delay(1000);
//   Serial1.println("C");
//   delay(1000);

}


/*
*    @para: void* dataPtr, we assume it's MeasureData pointer; integer isEven, check if it's even;
*    isEven range: 0 or 1;
*    Increase temperatureRaw by 2 even number time, decrease by 1 odd times called before reach 50; 0 is even;
*    Then reverse the process until temperatureRaw falls below 15
*    
*    systolicPressRaw  - even: Increase by 3; - odd: decrease by 1; Range: no larger than 100
*
*    diastolicPressRaw -even: decrease by 2; - odd: increase by 1; Range: no less than 40
*
*    pulseRateRaw - even: decrease by 1; - odd: increase by 3; Range: 15-40
*
*    April, 22, 2019 by Kaiser
*/
int trIsReverse = 0, prIsReverse = 0;

void Measure(void* dataPtr, int isEven, int trIsReverse, int )
{
    // dereference the data pointer;
    md = *((MeasureData*) dataPtr);  
        // When the function is executed even times;
        // Update the data;
        if(isEven == 1) {
            if(*(md.systolicPressRawPtr) <= 100) {
                *(md.systolicPressRawPtr) += 3;
            }
            if(*(md.diastolicPressRawPtr) >= 40) {
                *(md.diastolicPressRawPtr) -= 2;
            }
            if(trIsReverse == 0) {
                *(md.temperatureRawPtr) += 2;
            } else {
                *(md.temperatureRawPtr) -= 2;
            }
            if(prIsReverse == 0) {
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
            if(trIsReverse == 0) {
                *(md.temperatureRawPtr) -= 2;
            } else {
                *(md.temperatureRawPtr) += 2;
            }
            if(prIsReverse == 0) {
                *(md.pulseRateRawPtr) += 1;
            } else {
                *(md.pulseRateRawPtr) -= 1;
            }
        }

        // Check if should reverse the process;
        if(*(md.temperatureRawPtr) > 50) {
            trIsReverse = 1;
        } else if (*(md.temperatureRawPtr) < 15)
        {
            trIsReverse = 1;
        }

        if(*(md.pulseRateRawPtr) > 40) {
            prIsReverse = 1;
        } else if (*(md.pulseRateRawPtr) < 15)
        {
            prIsReverse = 0;
        }

        return;
}

/*
*    @para: generic pointer dataPtr;
*/
void Compute(void* dataPtr) {

}


