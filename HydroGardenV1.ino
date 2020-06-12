#include <WiFi.h>
#include <NTPClient.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define FloatSensorL1  14
#define FloatSensorL2 27
#define FloatSensorR1  12
#define FloatSensorR2 13
#define WaterPumpRight 25
#define WaterPumpLeft 26
#define AirPump 33
#define Light 32
 
const char* ssid = "HP-Print-B4-ENVY";
const char* password =  "adams1892-2";
const long utcOffsetInSeconds = -14400;
int lightON = 9;
int lightOFF=21;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
 
void connectToNetwork() {
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Establishing connection to WiFi..");
  }
 
  Serial.println("Connected to network");
 
}

void lightControl(){
  timeClient.update();
  Serial.print(timeClient.getHours());
  if (timeClient.getHours()>=lightON && timeClient.getHours()<=lightOFF){
    digitalWrite(2,HIGH);
  }
  else{
    digitalWrite(2,LOW);
  }
}

void CheckWaterLevelLeft(){
  double float1=digitalRead(FloatSensorL1);
  Serial.print("Float L1:");
  Serial.println(digitalRead(FloatSensorL1));
  double float2=digitalRead(FloatSensorL2);
  Serial.print("Float L2:");
  Serial.println(digitalRead(FloatSensorL2));
  double avgFloat=(float1+float2)/2;
  Serial.print("Average L=");
  Serial.println(avgFloat);

  if (avgFloat==0){
    digitalWrite(WaterPumpRight,LOW);
  }
  else{
    digitalWrite(WaterPumpRight,HIGH);
  }
}

void CheckWaterLevelRight(){
  double float1=digitalRead(FloatSensorR1);
  Serial.print("Float R1:");
  Serial.println(digitalRead(FloatSensorR1));
  double float2=digitalRead(FloatSensorR2);
  Serial.print("Float R2:");
  Serial.println(digitalRead(FloatSensorR2));
  double avgFloat=(float1+float2)/2;
  Serial.print("Average R=");
  Serial.println(avgFloat);

  if (avgFloat==0){
    digitalWrite(WaterPumpRight,LOW);
  }
  else{
    digitalWrite(WaterPumpRight,HIGH);
  }
}
 
void setup() {
 
  Serial.begin(115200);
  connectToNetwork();
 
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());
  pinMode(2,OUTPUT);
  timeClient.begin();
  //Set PinMode
  pinMode(FloatSensorL1, INPUT_PULLUP);
  pinMode(FloatSensorL2, INPUT_PULLUP);
  pinMode(FloatSensorR1, INPUT_PULLUP);
  pinMode(FloatSensorR2, INPUT_PULLUP);
  pinMode(WaterPumpRight,OUTPUT);
  pinMode(WaterPumpLeft,OUTPUT);
  pinMode(AirPump,OUTPUT);
  pinMode(Light,OUTPUT);
  //Turn off all relays
  digitalWrite(WaterPumpRight,HIGH);
  digitalWrite(WaterPumpLeft,HIGH);
  digitalWrite(Light,HIGH);
  digitalWrite(AirPump,HIGH);
 
}

 
void loop() {
    lightControl();
    CheckWaterLevelLeft();
    CheckWaterLevelRight();
    delay(1000);
  }
