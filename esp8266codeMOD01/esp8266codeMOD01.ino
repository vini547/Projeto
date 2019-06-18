#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <ArduinoJson.h>

#define DEVICE (0x53)
#define ADXL345_MG2G_MULTIPLIER (0.004)
#define SENSORS_GRAVITY_STANDARD          (SENSORS_GRAVITY_EARTH)
#define SENSORS_GRAVITY_EARTH             (9.80665F)              /**< Earth's gravity in m/s^2 */
byte _buff[6];
char POWER_CTL = 0x2D;    //Power Control Register
char DATA_FORMAT = 0x31;  //
char DATAX0 = 0x32;       //X-Axis Data 0
char DATAX1 = 0x33;       //X-Axis Data 1
char DATAY0 = 0x34;       //Y-Axis Data 0
char DATAY1 = 0x35;       //Y-Axis Data 1
char DATAZ0 = 0x36;       //Z-Axis Data 0
char DATAZ1 = 0x37;       //Z-Axis Data 1

float readAccel() 
{
  Serial.print("readAccel");
  uint8_t howManyBytesToRead = 6; //6 for all axes
  readFrom( DATAX0, howManyBytesToRead, _buff); //read the acceleration data from the ADXL345
  short x =0;
   x = (((short)_buff[1]) << 8) | _buff[0];
  short y = (((short)_buff[3]) << 8) | _buff[2];
  short z = (((short)_buff[5]) << 8) | _buff[4];
  Serial.println(x * ADXL345_MG2G_MULTIPLIER * SENSORS_GRAVITY_STANDARD);
  return x * ADXL345_MG2G_MULTIPLIER * SENSORS_GRAVITY_STANDARD;
  /*x = x + cal_x;

  
  Serial.print("x: "); 
  Serial.print( x*2./512 );
  Serial.print(" y: ");
  Serial.print( y*2./512 );
  Serial.print(" z: ");
  Serial.print( z*2./512 );
  Serial.print("X: "); 
  Serial.print( x);

  Serial.println( sqrtf(x*x+y*y+z*z)*2./512 );

getX() = read16(ADXL345_REG_DATAX0);
x = getX() * ADXL345_MG2G_MULTIPLIER * SENSORS_GRAVITY_STANDARD;
  */
}

void writeTo(byte address, byte val) 
{
  Wire.beginTransmission(DEVICE); // start transmission to device
  Wire.write(address); // send register address
  Wire.write(val); // send value to write
  Wire.endTransmission(); // end transmission
}

void readFrom(byte address, int num, byte _buff[]) 
{
  Wire.beginTransmission(DEVICE); // start transmission to device
  Wire.write(address); // sends address to read from
  Wire.endTransmission(); // end transmission
  Wire.beginTransmission(DEVICE); // start transmission to device
  Wire.requestFrom(DEVICE, num); // request 6 bytes from device

  int i = 0;
  while(Wire.available()) // device may send less than requested (abnormal)
  {
    _buff[i] = Wire.read(); // receive a byte
    i++;
  }
  Wire.endTransmission(); // end transmission
}




const char* ssid = "Millennium Falcon";
const char* password = "19072008";

ESP8266WebServer server(4444);

/*String createJsonResponse() {
  
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
} */




void handleRoot() {
 

  String textoHTML;

  textoHTML = "<center>RAW DATA FROM MPU6050 REGISTERS</center>";
  textoHTML += "<html><head><style>table, th, td {border: 1px solid black;}</style></head><body><table><tr><th>Modulo</th><th>Eixo X</th>  <th>Eixo Y</th>  <th>Eixo Z</th>   </tr>    <td>Acelerometro</td>    <td><script></script></td>    <td>accelY</td>    <td>accelZ</td>  </tr>  <tr>    <td>Giroscopio</td>    <td>gyroX</td>    <td>gyroY</td>    <td>gyroZ</td></tr></table></body></html>";
  textoHTML +=  "<html><body><p><a href=https://youtu.be/5y19QMU2GS0>LINKAO</a></p>"; 
  textoHTML +=  "<html><body><p><a href=http://127.0.0.1:5500/nodejs-server/public/index.html>LINKAO</a></p>";
  textoHTML +=  readAccel();
   
  server.send(200, "text/html", textoHTML);
 
}

void handleNotFound(){
 
  
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
 
}

void setup(void){

  Serial.begin(115200);
  Wire.begin();

// Put the ADXL345 into +/- 2G range by writing the value 0x01 to the DATA_FORMAT register.
  // FYI: 0x00 = 2G, 0x01 = 4G, 0x02 = 8G, 0x03 = 16G
  writeTo(DATA_FORMAT, 0x00);
  
  // Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL register.
  writeTo(POWER_CTL, 0x08);

  /*int i =0;
  for(i=0; i<11; i++)
  {
    uint8_t howManyBytesToRead = 6;
    readFrom( DATAX0, howManyBytesToRead, _buff);
    float calib_x ;//= (((short)_buff[1]) << 8) | _buff[0];
    calib_x = readAccel();
    if(i==0)
     cal_x = x;
    if(i>0)
     cal_x = cal_x + calib_x;
    Serial.println(calib_x);
    delay(100);
  }

  cal_x = cal_x/10;
  Serial.print("cal_x: ");Serial.println(cal_x); */

  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
 

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  
 /* current_value = readAccel();  // read ONLY x, for the y and x modify the readAccel function
  
  if((current_value - cal_x) > max_x)
    max_x = current_value - cal_x;
  if((current_value - cal_x) < min_x)
    min_x = current_value - cal_x;

  Serial.print("x: ");Serial.print(current_value);  Serial.print(" x(corrected): ");Serial.print(current_value - cal_x);    
  Serial.print(" Min:" );Serial.print(min_x); Serial.print(" Max:" ); Serial.println(max_x);    
  Serial.print("Trigger value:"); Serial.print(trigger_value); Serial.print(" Count:"); Serial.println(trigger_count);*/
  server.handleClient(); 
  
  delay(100);   // only read every 100ms
  
  delay(100);
}
