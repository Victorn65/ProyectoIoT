#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"
#define DHTPIN 5 // PIN D1
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Iphone del guapo ese";  // Enter SSID here
const char* password = "ladrando1";  //Enter Password here
const char *host = "http://thservers.herokuapp.com/api/data/get";// URL aqui

int buz = 4; // GPIO13 PIN D2

LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup() {
  Wire.begin(2,0);
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Iniciando...");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Conectando WiFi!");
  
  pinMode(buz, OUTPUT);
  digitalWrite(buz, LOW);
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println("DHT22 Output!");
  dht.begin();
  Serial.println("\nWiFi station setting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting...");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi ready");
  Serial.print("My ip address: ");
  Serial.println(WiFi.localIP()); 

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bienvenido!");  
  delay(2000);
}

void peligro(String alarma){
  if (alarma == "ON"){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Alarma!!");
    digitalWrite(buz, HIGH);
    delay(50);
    digitalWrite(buz, LOW);
    delay(500);
    digitalWrite(buz, HIGH);
    delay(50);
    digitalWrite(buz, LOW);
    delay(500);
    digitalWrite(buz, HIGH);
    delay(50);
    digitalWrite(buz, LOW);
    delay(500);
    digitalWrite(buz, HIGH);
    delay(50);
    digitalWrite(buz, LOW);
    delay(500);
    digitalWrite(buz, HIGH);
    delay(100);
    digitalWrite(buz, LOW);
    delay(250);
    digitalWrite(buz, HIGH);
    delay(100);
    digitalWrite(buz, LOW);
    delay(250);
    digitalWrite(buz, HIGH);
    delay(100);
    digitalWrite(buz, LOW);
    delay(250);
    digitalWrite(buz, HIGH);
    delay(100);
    digitalWrite(buz, LOW);
    delay(250);
    digitalWrite(buz, HIGH);
  }
  else{
    digitalWrite(buz, LOW);
  }
  }

void loop() {
  String alarma;
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();  
  if(isnan(temperature) || isnan(humidity)){
    Serial.println("Failed to read DHT22");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Failed DHT22");
  }else{
    if (temperature > 26.00 || humidity > 70.00){
      alarma = "1";
      peligro("ON");
    }
    else{
      alarma = "0";
      peligro("OFF");
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T=");
    lcd.setCursor(2, 0);
    lcd.print(temperature);
    lcd.setCursor(8, 0);
    lcd.print("H=");
    lcd.setCursor(10, 0);
    lcd.print(humidity);
    Serial.print("Alarma: ");
    Serial.print(alarma);
    Serial.print(" \t");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C");
    delay(500);
  }
  if(WiFi.status() != WL_CONNECTED){
    lcd.setCursor(0, 1);
    lcd.print("WiFi=OFF");
    Serial.println("WiFi OFF");
  }
  else{
    lcd.setCursor(0, 1);
    lcd.print("WiFi=ON ");
    Serial.println("WiFi ON");
  }
  WiFiClient client;
  HTTPClient http;
  
  //http://thservers.herokuapp.com/api/data/get
  String getData, link;
  getData = ("?temperature=") + String(temperature) + ("&humidity=") + String(humidity) + ("&alarm=") + String(alarma);
  link = host + getData;

  http.begin(client, link);
  int httpCode = http.GET();
  lcd.setCursor(9, 1);
  lcd.print("RC=");
  lcd.setCursor(12, 1);
  lcd.print(httpCode);
  Serial.print("Response Code: ");
  Serial.println(httpCode);
  http.end();
  delay((1000*60));
}
