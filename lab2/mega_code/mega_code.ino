#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library

void setup()
{
  // running on the mega - connect to tx1 and rx1 on the mega and to rx and tx on the uno
  // start serial1 port at 9600 bps and wait for serial1 port to open:
  Serial1.begin(9600);

    // Print
  Serial.println(F("TFT LCD test"));


#ifdef USE_Elegoo_SHIELD_PINOUT
  Serial.println(F("Using Elegoo 2.4\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Elegoo 2.4\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

  tft.reset();

   //Use identifier to make sure tft works
   uint16_t identifier = tft.readID();
   if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  }else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else if(identifier==0x0101)
  {     
      identifier=0x9341;
       Serial.println(F("Found 0x9341 LCD driver"));
  }
  else if(identifier==0x1111)
  {     
      identifier=0x9328;
       Serial.println(F("Found 0x9328 LCD driver"));
  }
  else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Elegoo 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_Elegoo_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Elegoo_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    identifier=0x9328;
  
  }
  tft.begin(identifier);

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
*    @para: generic pointer dataPtr;
*    Assume the data pointer is of type DisplayData
*    Display the data on the TFT display
*    April 23, 2019 by Kaiser Sun
*/
void Display(void* dataPtr) {
    // Setup of tft display
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(GREEN);
    tft.setTextSize(3);
    // Pointer dereference
    dd = *((DisplayData*) dataPtr);
    // Display
    tft.print("Systolic Pressure: ");
    //TODO: need to make sure whether use raw or corrected
    tft.print(*(dd.systolicPressRawPtr));
    tft.print("mmHg   Diastolic Pressure: ");
    tft.print(*(dd.diastolicPressRawPtr));
    tft.println(" mmHg");
    // print temperature, pulserate, battery Status
    tft.print("Temperature: ");
    tft.print(*(dd.temperatureRawPtr));
    tft.print("C   Pulse Rate: ");
    tft.print(*(dd.pulseRateRawPtr));
    tft.print("BPM   Battery: ");
    tft.print(*(dd.batteryStatePtr)); 
}

/*
*    @param: generic pointer dataPtr;
*    assume the dataPtr is of type dataPtr;
*    if the data are out of range, diplay with red;
*/
void WarningAlarm(void* dataPtr) {
    wad = *((WarningAlarmData*) dataPtr);
    // Change all measurement value?
    
}