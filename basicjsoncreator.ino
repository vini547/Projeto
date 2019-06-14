#include <ArduinoJson.h>
float accelX, accelY, accelZ;
String a;
float gyroX, gyroY, gyroZ;

String createJsonResponse() {
  
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& object = jsonBuffer.createObject();
  JsonObject& data = object.createNestedObject("data");
   
  JsonObject& accel = data.createNestedObject("accel");

  JsonObject& gyro = data.createNestedObject("gyro");
   
  JsonObject& accelXl = accel.createNestedObject("accelX");
  JsonObject& accelYl = accel.createNestedObject("accelY");
  JsonObject& accelZl = accel.createNestedObject("accelZ");
 
  JsonObject& gyroXl = gyro.createNestedObject("gyroX");
  JsonObject& gyroYl = gyro.createNestedObject("gyroY");
  JsonObject& gyroZl = gyro.createNestedObject("gyroZ");

  object["nodeID"] = "mcu1";
 
  accel["accelX"] = accelX;
  accel["accelY"] = accelY;
  accel["accelZ"] = accelZ;
  gyro["gyroX"] = gyroX;
  gyro["gyroY"] = gyroY;
  gyro["gyroZ"] = gyroZ;

  String json;
  object.prettyPrintTo(json);
  return json;
}

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println(createJsonResponse());
delay(3000);
}