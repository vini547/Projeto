#include <Wire.h>
long x, y, z;


void setupADXL345(){
  Wire.beginTransmission(0x53);
  Wire.write(0x2D);
  Wire.write(0);
  Wire.endTransmission();
  Wire.beginTransmission(0x53);
  Wire.write(0x2D);
  Wire.write(16);
  Wire.endTransmission();
  Wire.beginTransmission(0x53);
  Wire.write(0x2D);
  Wire.write(8);
  Wire.endTransmission();
}

void readADXL345() {    
  Wire.beginTransmission(0x53);
  Wire.write(0x32);
  Wire.endTransmission();
  Wire.requestFrom(0x53, 6);
  while(Wire.available() < 6);
  x = Wire.read()<<8|Wire.read(); 
  y = Wire.read()<<8|Wire.read(); 
  z = Wire.read()<<8|Wire.read(); 
}

void setup(){
    Serial.begin(115200);
    Wire.begin();
    setupADXL345();

}
void loop(){

recordAccelRegisters();
delay(100);
Serial.println(x);

}
