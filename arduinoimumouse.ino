#include <Mouse.h>
#include <Arduino_LSM6DS3.h>
#define grav 9.82f
#define bufs 4
#define movelim 0.1f
#define liftlim 0.1f
#define movemult 100.0f
//char buffer[200];
unsigned long pstime=0, cstime=0;
float mousex=0, mousey=0;
float x=0, y=0, z=0, f=IMU.accelerationSampleRate(), dt=1.0f/f;
float dx=0, dy=0, dz=0;
float sx=0, sy=0;
float dxarray[bufs], dyarray[bufs], dzarray[bufs];
int arrayind = 0;
void setup() {
  for (int i=0;i<bufs;i++) {dxarray[i]=0;dyarray[i]=0;dzarray[i]=0;}
  //Serial.begin(9600);
  //while(!Serial);
  //Serial.println("led: begin");
  pstime = micros(); cstime = pstime;
  pinMode(LED_BUILTIN, OUTPUT);
  if (!IMU.begin()) {
    //Serial.println("IMU.begin() failed.");
    while (1);
  }
  Mouse.begin();
}
void loop() {
  bool mousemove = false, mouselift = false;
  pstime = cstime; cstime = micros();
  float ds = (cstime - pstime)/1000000.0f;
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
      dx -= xm;
      dy -= ym;
      dz -= zm;
      sx += (dx*ds);
      sy += (dy*ds);
      if (zs>movelim) {
        mouselift = true;
        sx = 0; sy = 0;
        digitalWrite(LED_BUILTIN, HIGH);
      } else if ((xs>movelim)||(ys>movelim)) {
        digitalWrite(LED_BUILTIN, HIGH);
        mousemove = true;
      } else {
        sx = 0; sy = 0;
        digitalWrite(LED_BUILTIN, LOW);
      }
      float mousedx = sx*ds*movemult;
      float mousedy = sy*ds*movemult;
      Mouse.move((char)mousedx,(char)mousedy,0);
      //sprintf(buffer,"%lu: mousemove mousedx: %d, mousedy: %d",cstime,(int)(mousedx*1000000.0f),(int)(mousedy*1000000.0f));
      //Serial.println(buffer);
      //sprintf(buffer, "ds: %dus, dx: %d(%d)um/s^2, dy: %d(%d)um/s^2, dz: %d(%d)um/s^2, dt: %dus",(int)(ds*1000000.0f),(int)(dx*1000000.0f),(int)(xs*1000000.0f),(int)(dy*1000000.0f),(int)(ys*1000000.0f),(int)(dz*1000000.0f),(int)(zs*1000000.0f),(int)(dt*1000000.0f));
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