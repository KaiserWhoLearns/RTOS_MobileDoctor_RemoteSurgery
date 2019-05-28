//initialize values to measure frequency
const byte interruptPin = 2;  
#define delayTimeSec .5
// Pulse Rate& respRate
unsigned int freqCount = 0;
unsigned int freqPrevious = 0;

//initalize GPIO values to measure temperature
int analogPin = A0;
int analogVal = 0;

//initialize GPIO values to measure blood pressure
int buttonPin = 7; 
int switchPin = 9; 
int buttonVal = 0; //if buttonVal = 0 blood pressure doesn't change
int increment = 1; //if increment=1 buttonval=1 the cuff inflates
                   //if increment=0 and buttonval=1 the cuff deflates
char incoming;
char fun;

int temperatureRaw = 30;
int systolicPressRaw = 80;
int diastolicPressRaw = 80;
int pulseRateRaw = 70;
int respRate = 40;

void setup() {
  Serial.begin(4800);
  pinMode(interruptPin, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(switchPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), incrementFrequency, RISING);
}

void loop() {
  Measure();
  communications(); //Kaiser's Code
}
//main function that gets looped  
void Measure() {
 temperatureRaw = getTemp();
 systolicPressRaw = getBP(1);
 diastolicPressRaw = getBP(0);
 pulseRateRaw = getRawPulseRate();
 respRate = getRespirationRate();
}

void communications() {
    if(Serial.available()) {
        incoming = Serial.read();
    }
    if(incoming == 's') {
    if(Serial.available()) {
      // Uncomment this to run the version with more msg
      // msg = Serial.readString();
      // char incoming = parsePeri();
      incoming = Serial.read();
    }
      while(incoming != 'e') {
        
        if(incoming == 't') {
          fun = Serial.read();
          // Read in function name
          if(fun == 'p'){
          Serial.write(temperatureRaw);
          // responseMSG("t", "getTemp", String(temperatureRaw));
          }
        }
        if(incoming == 'b') {
          fun = Serial.read();
          // Read in function name
          if(fun == 'u'){
            // responseMSG("b", "getBP", String(systolicPressRaw));
            // responseMSG("b", "getBP", String(diastolicPressRaw));
          Serial.write(systolicPressRaw);
          Serial.write(diastolicPressRaw);
          }
        }
        if(incoming == 'p') {
          // responseMSG("p", "getRawPulseRate", String(pulseRateRaw));
          fun = Serial.read();
          // Read in function name
          if(fun == 'l'){
          Serial.write(pulseRateRaw);
          }
        }
        if(incoming == 'r') {
          // responseMSG("r", "getRespirationRate", String(respRate));
          fun = Serial.read();
          // Read in function name
          if(fun == 'l'){
          Serial.write(respRate);
          }
        }
            incoming = Serial.read();
            // Uncomment this to run the version with more msg
            incoming = 'e';
      }
    }
}



//returns the frequency value of the function generator
int measureFreq() {
    delay(1000 * delayTimeSec);
    unsigned int freq = freqCount - freqPrevious;
    freqPrevious = freqCount;
    //Serial.println(freq);
    return freq*2; 
}


//attach interrupt; increases the frequency count by 1
void incrementFrequency() {
  freqCount += 1;
}

////gets raw pulse rate by getting frequency values (in Hz) and scaling
unsigned int rawPulseRate = 0;
int getRawPulseRate() {
  int frequencyPulse = measureFreq();
  rawPulseRate = frequencyPulse;
  if (rawPulseRate > 64 ) { //upper limit corrected = 200 so upper limit raw = 64
    rawPulseRate = 64;      
  }
  if (rawPulseRate < 1) { //lower limit corrected = 10 so lower limit raw = 1
    rawPulseRate = 1;
  } 
  return rawPulseRate;
  //Serial.print(rawPulseRate);
}

//gets raw respiration rate by getting frequency values (in Hz) and scaling
unsigned int rawRespirationRate = 0;
int getRespirationRate() {
  int frequencyPulse = measureFreq();
  rawRespirationRate = frequencyPulse/4; 
  if (rawRespirationRate > 14 ) { //upper limit corrected = 50 so upper limit raw = 14
    rawRespirationRate = 14;      
  }
  if (rawRespirationRate < 1) { //lower limit corrected = 10 so lower limit raw = 1
    rawRespirationRate = 1;
  }
  return rawRespirationRate; 
}


//gets Temp by reading value on analog pin A0 which reads a value between 0 and 1024 
//and scaling the result
int getTemp() {
  analogVal = analogRead(analogPin);
  int temp = 20 + (analogVal/37); //scaled it so min raw = 20 max raw = 47
  return temp;
  
}

unsigned int SysP = 50;
unsigned int DiaP = 29;
unsigned int previousbuttonVal = 0;
int getBP (boolean sys) {
  // delay(5);
  buttonVal = digitalRead(buttonPin);
  increment = digitalRead(switchPin);
  if (previousbuttonVal == 0 & buttonVal == 1) {
    previousbuttonVal = 1;
    if (increment == 1 ) {
      SysP = SysP * 1.1;
      DiaP = DiaP * 1.1;
    } else {
      SysP = SysP * .9;
      DiaP = DiaP * .9;
    }
  }
  if (previousbuttonVal == 1 & buttonVal == 0) {
    previousbuttonVal = 0;
  }
  if (SysP < 50 ) {  //Corrected Systolic lower limit = 110, so systolic raw lower limit = 50 
    SysP = 50;
  } else if (SysP > 70) { //Corrected Systolic upper limit = 150, so systolic raw upper limit = 70 
    SysP = 70;
  }
  if (DiaP < 29 ) { //Corrected diastolic lower limit = 50, so diastolic raw lower limit = 29 
    DiaP = 29;
  } else if (DiaP > 50) { //Corrected diastolic upper limit = 80, so diastolic raw upper limit = 50 
    DiaP = 50;
  }
  if (sys == 1) {
    return SysP;
  }
  else return DiaP;
}


/*
*    @param: ident: task identifier; fun: function name; data: data requested
*    Generate the message and send the message to MEGA
*    May 28, 2019, Kaiser
*/
void responseMSG(String ident, String fun, String data) {
    Serial1.println("+" + ident + "-" + fun + ":" + data + ".");
    return;
}

/*
*   @param: msg: the request message from Mega
*   Parse the request message, return the data
*   May 28, 2019, Kaiser 
*/
char parsePeri(String msg) {
    for(int i = 0; i < sizeof(msg) - 1; i ++) {
        if(msg[i] == '+') 
            return msg[i+1];
    }
}