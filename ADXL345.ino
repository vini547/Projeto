#include <Wire.h>
int x, y, z;



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
  digitalWrite(BUILTIN_LED1, HIGH);
  delay(500);
  digitalWrite(BUILTIN_LED1, LOW);  
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
  digitalWrite(BUILTIN_LED1, HIGH);
  delay(500);
  digitalWrite(BUILTIN_LED1, LOW);  
}

void setup(void){
  pinMode(BUILTIN_LED1, OUTPUT); 
    Serial.begin(115200);
    Wire.begin();
    setupADXL345();
    digitalWrite(BUILTIN_LED1, HIGH);
    delay(500);
    digitalWrite(BUILTIN_LED1, LOW);  

}
void loop(void){

readADXL345();
delay(100);
Serial.println(x);
digitalWrite(BUILTIN_LED1, HIGH);
  delay(300);
  digitalWrite(BUILTIN_LED1, LOW);  digitalWrite(BUILTIN_LED1, HIGH);
  delay(300);
  digitalWrite(BUILTIN_LED1, LOW);  digitalWrite(BUILTIN_LED1, HIGH);
  delay(300);
  digitalWrite(BUILTIN_LED1, LOW);  

}
