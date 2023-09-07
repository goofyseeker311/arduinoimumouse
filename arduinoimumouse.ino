#include <Mouse.h>
#include <Arduino_LSM6DS3.h>
float ax=0.0f, ay=0.0f, az=1.01f, tol=0.01f, ltol=0.1f, dec=0.99f, mlt=2000000;
unsigned long pstime=0, lstime=0, ldtime=100000;
void setup() {
  pstime=micros(); lstime=pstime;
  pinMode(LED_BUILTIN, OUTPUT);
  if (!IMU.begin()) {
      while (1);
  }
  Mouse.begin();
}
void loop() {
  float x, y, z; float fps; float dx, dy, dz, dt, dt2; int mx, my; bool mousemove = false; bool mouselift = false; unsigned long ntime = micros();
  if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(y, x, z); fps = IMU.accelerationSampleRate(); dt=1.0f/fps; dt2=dt*dt; x=-x;
      ax=dec*ax+(1.0f-dec)*x; ay=dec*ay+(1.0f-dec)*y; az=dec*az+(1.0f-dec)*z; dx=x-ax; dy=y-ay; dz=z-az;
      if (abs(dx)>tol) { mx=mlt*0.5*dx*dt2; mousemove = true; }
      if (abs(dy)>tol) { my=mlt*0.5*dy*dt2; mousemove = true; }
      if (abs(dz)>ltol) { mouselift = true; lstime = micros(); }
      if ((mouselift) || (ntime<(lstime+ldtime))) {
        digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
      } else {
        digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
        if (mousemove) {
          Mouse.move(mx, my, 0);
        }
      }
  }
}
