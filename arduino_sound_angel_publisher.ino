#define dis 0.200 // distance of 2 mics
#define wait 200 // wait 200 milliseconds after an angel is given
#define pi 3.1415926

#include <ros.h>
#include <std_msgs/Float32.h>

ros::NodeHandle nh;

std_msgs::Float32 sound_angel;
ros::Publisher pub_angel("angel", &sound_angel);

int A = 2; // mic A pin
int B = 3; // mic B pin
int C = 4; // mic C pin
float t1; // micro second time when the first mic is triggered
float t2; // micro second time when the second mic is triggered
float testDelay;
float angel; // the angel of sound source
float t; // ambient temperature
float soundVel; // velocity of sound under ambient temperature of t
int dir = 0; // indicate on which side of the mic is the sound source
bool micTriggered = false; // true if a mic is triggered

inline int digitalRead_fast(int pin)
{
  return !!(PORT->Group[g_APinDescription[pin].ulPort].IN.reg & (1ul << g_APinDescription[pin].ulPin));
}

void setup() {
  // put your setup code here, to run once:
  t = 25.0;
  soundVel = 331.3 + 0.606 * t;

  Serial.begin(115200);
  pinMode(A, INPUT);
  pinMode(B, INPUT);
  pinMode(C, INPUT);

  nh.initNode();
  nh.advertise(pub_angel);

}

void loop() {
  // put your main code here, to run repeatedly:
  //  int test1 = digitalRead(A);
  //  int test2 = digitalRead(B);
  //
  //  Serial.println(String(test1) + ", " + String(test2));

  t1 = 0;
  t2 = 0;
  angel = 0.0;
  micTriggered = false;

  if (digitalRead_fast(A) == 0) {
    t1 = micros();
    while (digitalRead_fast(B) == 1 && digitalRead_fast(C) == 1) {}
    t2 = micros();
    int z = digitalRead_fast(B) - digitalRead_fast(C);
    float t3 = t2 - t1;
    Serial.println(z);
    if (abs(t3) > 1000 || t1 == 0) {}
    else {
      angel = rad(t3, A, z);
      delay(20);
      // Serial.println(String(t3) + ", " + String(angel) + " A");
      micTriggered = true;
    }
    delay(wait);
  }

  if (digitalRead_fast(B) == 0) {
    t1 = micros();
    while (digitalRead_fast(C) == 1 && digitalRead_fast(A) == 1) {}
    t2 = micros();
    int z = digitalRead_fast(C) - digitalRead_fast(A);
    float t3 = t2 - t1;
    if (abs(t3) > 1000 || t1 == 0) {}
    else {
      angel = rad(t3, B, z);
      delay(20);
      // Serial.println(String(t3) + ", " + String(angel) + " B");
      micTriggered = true;
    }
    delay(wait);
  }

  if (digitalRead_fast(C) == 0) {
    t1 = micros();
    while (digitalRead_fast(A) == 1 && digitalRead_fast(B) == 1) {}
    t2 = micros();
    int z = digitalRead_fast(A) - digitalRead_fast(B);
    float t3 = t2 - t1;
    if (abs(t3) > 1000 || t1 == 0) {}
    else {
      angel = rad(t3, C, z);
      delay(20);
      // Serial.println(String(t3) + ", " + String(angel) + " C");
      micTriggered = true;
    }
    delay(wait);
  }

  if(micTriggered){
    sound_angel.data = angel;
    pub_angel.publish(&sound_angel);
    nh.spinOnce();
    delay(500);
  }

}

float rad(float dt, int mic, int zone) {
  float ang = 0.0;

  if (zone > 0) {
    dir = 1;
  }
  else {
    dir = -1;
  }

  if (mic == 2) {
    ang = 0 + dir * (60 - 180 / pi * asin((dt * 0.000001 * soundVel) / dis));
  }
  else if (mic == 3) {
    ang = -120 + dir * (60 - 180 / pi * asin((dt * 0.000001 * soundVel) / dis));
  }
  else {
    ang = 120 + dir * (60 - 180 / pi * asin((dt * 0.000001 * soundVel) / dis));
  }

  return ang;

}
