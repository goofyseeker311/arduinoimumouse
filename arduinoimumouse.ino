//#include <Mouse.h>
#include <Arduino_LSM6DS3.h>
#define grav 9.82f
#define bufs 10
#define movelim 0.1f
#define liftlim 0.1f
char buffer[100];
unsigned long pstime=0, cstime=0;
float x=0, y=0, z=0, f=IMU.accelerationSampleRate(), dt=1.0f/f;
float dx=0, dy=0, dz=0;
float dxarray[bufs], dyarray[bufs], dzarray[bufs];
int arrayind = 0;
void setup() {
  for (int i=0;i<bufs;i++) {dxarray[i]=0;dyarray[i]=0;dzarray[i]=0;}
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
  if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(y, x, z);
      dx = grav*x;
      dy = grav*y;
      dz = grav*z;
      dxarray[arrayind] = dx;
      dyarray[arrayind] = dy;
      dzarray[arrayind] = dz;
      if (++arrayind>=bufs) {arrayind=0;}
      float xm = mean(dxarray, bufs);
      float ym = mean(dyarray, bufs);
      float zm = mean(dzarray, bufs);
      float xs = stdev(dxarray, bufs);
      float ys = stdev(dyarray, bufs);
      float zs = stdev(dzarray, bufs);
      if ((xs>movelim)||(ys>movelim)) {
        mousemove = true;
      }
      if (zs>movelim) {
        mouselift = true;
      }
      if (mouselift) {
        digitalWrite(LED_BUILTIN, HIGH);
        sprintf(buffer,"%lu: mouselift",cstime);
        Serial.println(buffer);
      } else if (mousemove) {
        digitalWrite(LED_BUILTIN, HIGH);
        sprintf(buffer,"%lu: mousemove",cstime);
        Serial.println(buffer);
      } else {
        digitalWrite(LED_BUILTIN, LOW);
        sprintf(buffer,"%lu: .........",cstime);
        Serial.println(buffer);
      }
      //sprintf(buffer, "ds: %luus, dxm: %d(%d)um/s^2, dym: %d(%d)um/s^2, dzm: %d(%d)um/s^2, dt: %dus",ds,(int)(xm*1000000),(int)(xs*1000000),(int)(ym*1000000),(int)(ys*1000000),(int)(zm*1000000),(int)(zs*1000000),(int)(dt*1000000));
      //Serial.println(buffer);
  }
}
float mean(float* data, int size) {
  float meanval = 0;
  for (int i=0;i<size;i++) {
    meanval += data[i];
  }
  meanval /= (float)size;
  return meanval;
}
float stdev(float* data, int size) {
  float stdval = 0;
  float meanval = mean(data, size);
  float varval = 0;
  for (int i=0;i<size;i++) {
    float difval = data[i]-meanval;
    varval += pow(difval,2);
  }
  varval /= ((float)size);
  stdval = sqrt(varval);
  return stdval;
}