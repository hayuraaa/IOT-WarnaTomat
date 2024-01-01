#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
//I2C Library
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
WiFiClient client;
// http
HTTPClient http;

#define S0 D0
#define S1 D1
#define S2 D2
#define S3 D3
#define sensorOut D4
// wifi
const char *ssid = "Hayura";
const char *pass = "12345677";
// host
const char *host = "172.20.10.3";

int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

int redColor = 0;
int greenColor = 0;
int blueColor = 0;

void setup() 
{
  // Begins serial LCD
  Serial.begin(9600);
  delay(1000);
  // Connect to WiFi networkdelay(1000);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  Serial.println("Setup koneksi. . .");
  WiFi.mode(WIFI_STA); //This line hides the viewing of ESP as wifi hotspot
  WiFi.begin(ssid, pass);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  // jika koneksi berhasil, maka akan muncul IP address di serial monitor
  Serial.println("");
  Serial.println("Connected to");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 //inisial LCD
 lcd.begin (16,2); //LCD untuk ukuran 20x4
 lcd.backlight();
 lcd.setCursor(0,1);
 lcd.print("Warna : ");
  // Setting output
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Setting sensorout
  pinMode(sensorOut, INPUT);
  
  // Setting Frekuensi
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
}

void loop() {
  // variable
  String postData,link;

  // Setting RED Color
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  lcd.setCursor(2,0);
  lcd.print("PINTU SHARE");
  redFrequency = pulseIn(sensorOut, LOW);
  redColor = map(redFrequency, 22, 103, 255,0);
  
  Serial.print("R = ");
  Serial.print(redColor);
  delay(1000);
  
  // Setting GREEN Color
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  greenFrequency = pulseIn(sensorOut, LOW);
  greenColor = map(greenFrequency, 41, 129, 255, 0);
  Serial.print(" G = ");
  Serial.print(greenColor);
  delay(1000);
 
  // Setting BLUE Color
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  
  blueFrequency = pulseIn(sensorOut, LOW);
  blueColor = map(blueFrequency, 16, 122, 255, 0);
  
  Serial.print(" B = ");
  Serial.print(blueColor);
  delay(1000);

  // jika matang
  if(redColor > greenColor && redColor ){
    // postdata
      postData = "?status1=MATANG";
      link = "http://172.20.10.3:80/websensor/postdemo.php" + postData;
    http.begin(client,link);              //Specify request destination;    //Specify content-type header

    Serial.println(link);
    Serial.println(postData);
    int httpCode = http.GET();   //Send the request
  // httpCode will be negative on error
  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }    //Print request response payload

    http.end();  //Close connection
    Serial.println(" MATANG");
    lcd.setCursor(8, 1);
    lcd.print("Merah  "); 
  }
  //jika mentah
  if(greenColor > redColor && greenColor ){
    // postdata
      postData = "?status1=MENTAH";
      link = "http://172.20.10.3:80/websensor/postdemo.php" + postData;
    http.begin(client,link);              //Specify request destination;    //Specify content-type header
    Serial.println(postData);
    Serial.println(link);

    int httpCode = http.GET();
  // httpCode will be negative on error
  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }//Print request response payload

    http.end();  //Close connection
    Serial.println(" MENTAH");
     lcd.setCursor(8, 1);
   lcd.print("Hijau  "); 
  }
delay(1000);
}
