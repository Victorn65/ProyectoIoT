// Librerias para la conexion de la pantalla lcd 16x2 con I2C
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Librerias para la conexion del NodeMCU a internet
#include <ESP8266WiFi.h>
// Librerias para la conexion del NodeMCU a un link receptor de datos
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
// Libreria para la conexion del sensor Temp/Hum
#include "DHT.h"
// Definicion de pin en el GPIO del NodeMCU
#define DHTPIN 5 // PIN D1
// Definicion del tipo de sensor DHT en este caso el 22
#define DHTTYPE DHT22
// Declaracion para el tipo de variable del sensor
DHT dht(DHTPIN, DHTTYPE);
// Declaracion de variables para la conexion a wifi
// Nota: solo admite red 2.4 
const char* ssid = "Iphone del guapo ese";  // Nombre de la red
const char* password = "ladrando1";  // Contrasena de la red
// Declaracion de variable host que contiene parte del link
const char *host = "http://thservers.herokuapp.com/api/data/get";// URL aqui
// Definicion de variable del pin en el GPIO del NodeMCU del Buzzer
int buz = 4; // GPIO13 PIN D2
// Declaracion de variable para la lcd
LiquidCrystal_I2C lcd(0x3F, 16, 2);
// Configuracion que se realiza unica vez cada encendido o reset del Hardware
void setup() {
  // Inicializacion de conexion del I2C 
  Wire.begin(2,0);
  // Prendido de lcd
  lcd.init();   
  // Prendido de la luz del lcd
  lcd.backlight();
  // Funcion para limpiar de caracteres la pantalla
  lcd.clear();
  // Funcion para colocar el puntero para escribr en el lcd
  lcd.setCursor(0, 0); // Cordenada en (x, y)
  // Funcion para impresion de caracteres en la pantalla
  lcd.print("Iniciando...");
  // Funcion para implementar una pausa en milisegundos
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Conectando WiFi!");
  // Declaracion del tipo de funcionamiento del pin, en este caso salida
  pinMode(buz, OUTPUT);
  // Funcion para declarar lo que sale en el pin siendo:
  // LOW = 0 volts
  // HIGH = 3.3 volts
  digitalWrite(buz, LOW);
  // Funcion para iniciar el puerto serie
  Serial.begin(115200);
  // Funcion para imprimir en el puerto serie con un enter
  Serial.println();
  Serial.println();
  Serial.println("DHT22 Output!");
  // Funcion para iniciar el DHT
  dht.begin();
  Serial.println("\nWiFi station setting");
  // Funcion necesaria para hacer funcionar la conexion a wifi
  WiFi.mode(WIFI_STA);// Tipo de conexion de modo
  // Funcion para iniciar la conexion a wifi
  WiFi.begin(ssid, password);
  Serial.println("Connecting...");
  // Loop, Mientras que no conecte a internet muestra un punto en el puerto serie
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi ready");
  Serial.print("My ip address: ");
  // Imprime el LocalIP de la conexion WiFi
  Serial.println(WiFi.localIP()); 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bienvenido!");  
  delay(2000);
}
// Funcion que implementa la alarma sonora
// Recibe un string (ON / OFF)
void peligro(String alarma){
  // Si es on inicia la funcion sonora
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
  // En caso de no ser ON apaga el buzzer
  else{
    digitalWrite(buz, LOW);
  }
}
// Funcion que se repite indefinidamente mientras el dispositivo tenga energia
void loop() {
  // Creacion de variables
  String alarma;
  float temperature = dht.readTemperature();// Funcion para obtener el valor de temp del DHT
  float humidity = dht.readHumidity();// Funcion para obtener el valor de hum del DHT
  // Condicional que pregunta si existen valores del sensor o no
  // En caso de desconectar el sensor aparecen valores nan
  // En caso no estar funcionando el sensor aparecen valores nan
  if(isnan(temperature) || isnan(humidity)){
    // En caso de tener valores nan: -->
    Serial.println("Failed to read DHT22");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Failed DHT22");
  // En caso de existir valores: -->
  }else{
    // Dependiendo de los valores checa si hay un sobre paso de los declarados
    // para saber si inicia la alarma
    if (temperature > 26.00 || humidity > 70.00){ // Rangos para la alarma
      alarma = "1";
      peligro("ON");
    }
    else{
      alarma = "0";
      peligro("OFF");
    }
    // Inicia la impresion de valores en pantalla y puerto
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
  // Pregunta si hay conexion a internet
  if(WiFi.status() != WL_CONNECTED){// Si no hay conexion entonces:
    lcd.setCursor(0, 1);
    lcd.print("WiFi=OFF");
    Serial.println("WiFi OFF");
  }
  else{// Si hay conexion entonces: 
    lcd.setCursor(0, 1);
    lcd.print("WiFi=ON ");
    Serial.println("WiFi ON");
  }
  // Inicializamos los tipos de variable de conexion a nuestro host
  WiFiClient client;
  HTTPClient http;
  // Primera parte del link = host
  //http://thservers.herokuapp.com/api/data/get
  // Variables para creacion de link completo
  String getData, link;
  // Variable que se compone de la estructura necesaria para mandar los datos al server
  getData = ("?temperature=") + String(temperature) + ("&humidity=") + String(humidity) + ("&alarm=") + String(alarma);
  // Agrupacion de los strings para la cracion del link completo
  link = host + getData;
  // Inicializacion de conexion al servidor con envio de datos
  http.begin(client, link);
  // Variable para reconocer el codigo de conexion
  int httpCode = http.GET();
  // Impresion del Response code
  lcd.setCursor(9, 1);
  lcd.print("RC=");
  lcd.setCursor(12, 1);
  lcd.print(httpCode);
  Serial.print("Response Code: ");
  Serial.println(httpCode);
  // Cerrado de conexion al servidor para no tener errores o problemas
  http.end();
  // Pausa de tiempo de 60 Segundos 
  delay((1000*60));// Cambiar dependiendo de la necesidad de datos
}
