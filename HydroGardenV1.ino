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
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
 
const char* ssid = "SSID";
const char* password =  "Password";
const long utcOffsetInSeconds = -14400;
int lightON = 9;
int lightOFF=21;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

 
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
    digitalWrite(Light,LOW);
  }
  else{
    digitalWrite(Light,HIGH);
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
    digitalWrite(WaterPumpLeft,LOW);
  }
  else{
    digitalWrite(WaterPumpLeft,HIGH);
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

void DisplayText(){
  // Display Text
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.println("Herb Garden IOT");
  display.setCursor(0,8);
  display.print("IP: ");
  display.println(WiFi.localIP());
  display.setCursor(0,16);
  display.print("Time: ");
  display.println(timeClient.getFormattedTime());
  display.setCursor(0,24);
  display.print("L1:");
  display.print(digitalRead(FloatSensorL1));
  display.print("  L2:");
  display.println(digitalRead(FloatSensorL2));
  display.setCursor(0,32);
  display.print("R1:");
  display.print(digitalRead(FloatSensorR1));
  display.print("  R2:");
  display.println(digitalRead(FloatSensorR2));
  display.display();
  
  
}
 
void setup() {
 
  Serial.begin(115200);
  connectToNetwork();
 
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());
  //pinMode(2,OUTPUT);
  timeClient.begin();
  //Set PinMode
  
  // initialize with the I2C addr 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  

  // Clear the buffer.
  display.clearDisplay();
  
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
    //lightControl();
    digitalWrite(AirPump,LOW);
    lightControl();
    CheckWaterLevelLeft();
    CheckWaterLevelRight();
    DisplayText();
    delay(1000);
  }
