#include <Mouse.h>
#include <Arduino_LSM6DS3.h>
unsigned long pstime=0, cstime=0;
bool ledon = false;
void setup() {
  Serial.begin(9600);
  delay(3000);
  Serial.println("led: begin");
  pstime = micros(); cstime = pstime;
  pinMode(LED_BUILTIN, OUTPUT);
  if (!IMU.begin()) {while (1);}
  Mouse.begin();
}
void loop() {
  float x, y, z, dt;
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
      dt = 1.0f/IMU.accelerationSampleRate();
      int xi = (int)(x*1000000);
      int yi = (int)(y*1000000);
      int zi = (int)(z*1000000);
      int dti = (int)(dt*1000000);
      char buffer[50];
      sprintf(buffer, "ds: %luus, x: %d, y: %d, z: %d, dt: %dus",ds,xi,yi,zi,dti);
      Serial.println(buffer);
  }
}