// #include <Elegoo_GFX.h>    // Core graphics library
// #include <Elegoo_TFTLCD.h> // Hardware-specific library
#include "tcb.h"


// initialize raw values
unsigned int temperatureRaw = 30;
    // there're problem of initial value of temp!
unsigned int systolicPressRaw = 80;
unsigned int diastolicPressRaw = 80;
unsigned int pulseRateRaw = 70;

// initialize raw value pointers
unsigned int* temperatureRawPtrr = &temperatureRaw;
unsigned int* systolicPressRawPtrr = &systolicPressRaw;
unsigned int* diastolicPressRawPtrr = &diastolicPressRaw;
unsigned int* pulseRateRawPtrr = &pulseRateRaw;

Bool trIsReverse = FALSE, prIsReverse = FALSE, isEven = TRUE;


//MeasureData meaD;

//set up data struct
MeasureData meaD = MeasureData{temperatureRawPtrr, systolicPressRawPtrr, diastolicPressRawPtrr, pulseRateRawPtrr};
 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(4800);

} 

char incoming;

void loop() {
  // put your main code here, to run repeatedly:
//  Measure(&meaD);
//  char incoming = Serial.read();
//  if (incoming == 0x00) {
//    Serial.write(*temperatureRawPtrr);
//  } else if (incoming == 0x01) {
//    Serial.write(*systolicPressRawPtrr);
//  } else if (incoming == 0x02) {
//    Serial.write(*diastolicPressRawPtrr);
//  } else {
//    Serial.write(*pulseRateRawPtrr);
//  }
//  Measure(&meaD);
//  char incoming = Serial.read();
//  switch (incoming) {
//    case 0x00:
//      Serial.write(*temperatureRawPtrr);
//      Measure(&meaD);
//      
//      break;
//  
//    case 0x01:
//      Serial.write(*systolicPressRawPtrr);
//      break;
//  
//    case 0x02:
//      Serial.write(*diastolicPressRawPtrr);
//      break;
//     
//    case 0x03:
//      Serial.write(*pulseRateRawPtrr);
//      break;
//  
//    default: 
//      break;
//  }
  Measure(&meaD);
  //Serial.println(*pulseRateRawPtrr);
  //Serial.print(*temperatureRawPtrr);
  //Serial.println(*systolicPressRawPtrr);
  //Serial.print(*diastolicPressRawPtrr);
  incoming = Serial.read();
  if (incoming == (char)0x03) {
    Serial.write(*pulseRateRawPtrr);
    //Serial.println(*pulseRateRawPtrr);
    incoming = Serial.read();
  }
  if (incoming == (char)0x00) {
    Serial.write(*temperatureRawPtrr);
    Serial.println(*pulseRateRawPtrr);
    incoming = Serial.read();
  } 
  if (incoming == (char)0x01) {
    Serial.write(*systolicPressRawPtrr);
    incoming = Serial.read();
  } 
  if (incoming == (char)0x02) {
    Serial.write(*diastolicPressRawPtrr);
    incoming = Serial.read();
  } 
   
}

// dereference the data pointer;
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
