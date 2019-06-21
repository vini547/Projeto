#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <FS.h>
#include <HTTPClient.h>

int LED_BUILTIN = 2;
long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;

long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;

const char* ssid = "BetinaFernanda";
const char* password = "betina1802";

WebServer server(5555);


void setupMPU(){
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
  Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s 
  Wire.endTransmission(); 
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00000000); //Setting the accel to +/- 2g
  Wire.endTransmission(); 
}

void recordAccelRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Accel Registers (3B - 40)
  while(Wire.available() < 6);
  accelX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  accelY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  accelZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
 
}

void recordGyroRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Gyro Registers (43 - 48)
  while(Wire.available() < 6);
  gyroX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  gyroY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  gyroZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  
}

void handleRoot() {
 

  String textoHTML;

  textoHTML =  "<center>RAW DATA FROM MPU6050 REGISTERS</center>";
  textoHTML += "<html><head><style>table, th, td {border: 1px solid black;}</style></head><body><table><tr><th>Modulo</th><th>Eixo X</th>  <th>Eixo Y</th>  <th>Eixo Z</th>   </tr>    <td>Acelerometro</td>    <td><script></script></td>    <td>accelY</td>    <td>accelZ</td>  </tr>  <tr>    <td>Giroscopio</td>    <td>gyroX</td>    <td>gyroY</td>    <td>gyroZ</td></tr></table></body></html>";
  textoHTML += "<html><body><p><a href=https://youtu.be/5y19QMU2GS0>Maratona DEV</a></p>";  
  textoHTML += "<html><body><p><a href=http://www.portalburn.com.br/downloads/operacao_prato/fotos/>Aliens no Brasil?</a></p>";
  textoHTML += "<html><body><p><a href=https://youtu.be/PRgoisHRmUE>The Nimitz Encounters</a></p>";
  textoHTML += "<html><body><p><a href=https://github.com/vini547>Github</a></p>";
  textoHTML +=  createJsonResponse();  
    
  
  
   
  server.send(200, "text/html", textoHTML);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300);
  digitalWrite(LED_BUILTIN, LOW);
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
void setup(void){
  
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
}

File jedi = SPIFFS.open("/jedi.jpg", "r");
server.streamFile(jedi, "image/jpeg");

  Wire.begin();
  setupMPU();
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
  HTTPClient http;
  http.begin("s3.private.ap.cloud-object-storage.appdomain.cloud");


  server.handleClient();
  recordAccelRegisters();
  recordGyroRegisters();
  Serial.println(createJsonResponse());
  delay(100);
}
