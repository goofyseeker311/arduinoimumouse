//#include <Mouse.h>
#include <Arduino_LSM6DS3.h>
#define grav 9.82f
unsigned long pstime=0, cstime=0;
float x=0, y=0, z=0, f=IMU.accelerationSampleRate(), dt=1.0f/f;
float dx=0, dy=0, dz=0;
bool ledon = false;
void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("led: begin");
  pstime = micros(); cstime = pstime;
  pinMode(LED_BUILTIN, OUTPUT);
  if (!IMU.begin()) {Serial.println("IMU.begin() failed."); while (1);}
  //Mouse.begin();
}
void loop() {
  bool mousemove = false, mouselift = false;
  pstime = cstime; cstime = micros();
  unsigned long ds = cstime - pstime;
  if (!ledon) {
    digitalWrite(LED_BUILTIN, HIGH);
    ledon = true;
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    ledon = false;
  }
  delay(100);
  if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(y, x, z);
      dx = grav*x;
      dy = grav*y;
      dz = grav*z;
      int dxi = (int)(dx*1000000);
      int dyi = (int)(dy*1000000);
      int dzi = (int)(dz*1000000);
      int dti = (int)(dt*1000000);
      char buffer[100];
      sprintf(buffer, "ds: %luus, dx: %dum/s^2, dy: %dum/s^2, dz: %dum/s^2, dt: %dus",ds,dxi,dyi,dzi,dti);
      Serial.println(buffer);
  }
}