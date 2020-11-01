#define dis 0.200
#define pi 3.1415926

const byte micA = 4;
const byte micB = 5;
const byte micC = 6;
volatile float t1 = 0;
volatile float t2 = 0;
volatile float t3 = 0;

volatile bool ts1 = false;
volatile bool ts2 = false;
volatile bool ts3 = false;

float soundVel;
float temperature;

void setup() {
  for(int m = 4; m < 7; m++){
    pinMode(m, INPUT_PULLUP);
    }
  attachInterrupt(digitalPinToInterrupt(micA), Time1, FALLING);
  attachInterrupt(digitalPinToInterrupt(micB), Time2, FALLING);
  attachInterrupt(digitalPinToInterrupt(micC), Time3, FALLING);

  temperature = 25.0;
  soundVel = 331.3 + 0.606 * temperature;
}

void loop() {
  float *pose = new float[3];
  
  if ((ts1 && ts2 && ts3)||(abs(t1-t2)>1000)||(abs(t2-t3)>1000)||(abs(t1-t3)>1000)){
    Serial.println("t1: " + String(t1) + " t2: " + String(t2) + " t3: " + String(t3));
    source(pose, t1, t2, t3);
    Serial.println("angel1: " + String(pose[0]) + " angel2: " + String(pose[1]) + " angel3: " + String(pose[2]));
    ts1=false;
    ts2=false;
    ts3=false;
    delay(2000);
    }

  delete[] pose;
  
}

void Time1() {
  if(!ts1){
    t1 = micros();
    ts1 = true;
    }
}

void Time2() {
  if(!ts2){
    t2 = micros();
    ts2 = true;
    }
}

void Time3() {
  if(!ts3){
    t3 = micros();
    ts3 = true;
    }
}

void source(float p[3], float time1, float time2, float time3){
  p[0] = angel(time1, time2);
  p[1] = angel(time2, time3);
  p[2] = angel(time1, time3);
  }

float angel(float t1, float t2){
  float ang = 0;
  float dt = t1 - t2;
  ang = 180 / pi * asin((dt * 0.000001 * soundVel) / dis);
  return ang;
  }
