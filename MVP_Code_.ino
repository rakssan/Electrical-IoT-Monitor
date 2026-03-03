#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "ElectricalMonitor"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <PZEM004Tv30.h>
#include <DHT.h>
#include <SoftwareSerial.h>

char ssid[] = "YOUR_WIFI";
char pass[] = "YOUR_PASS";

// DHT Setup
#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// PZEM Setup
SoftwareSerial pzemSW(D6, D5);  // RX, TX
PZEM004Tv30 pzem(pzemSW);

BlynkTimer timer;

void sendData() {
  
  // PZEM readings
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power   = pzem.power();
  float energy  = pzem.energy();
  float frequency = pzem.frequency();
  float pf = pzem.pf();

  // DHT readings
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Send to Blynk (Virtual Pins)
  Blynk.virtualWrite(V0, voltage);
  Blynk.virtualWrite(V1, current);
  Blynk.virtualWrite(V2, power);
  Blynk.virtualWrite(V3, energy);
  Blynk.virtualWrite(V4, temperature);
  Blynk.virtualWrite(V5, humidity);

  // Serial Monitor
  Serial.println("------ Sensor Data ------");
  Serial.print("Voltage: "); Serial.println(voltage);
  Serial.print("Current: "); Serial.println(current);
  Serial.print("Power: "); Serial.println(power);
  Serial.print("Energy: "); Serial.println(energy);
  Serial.print("Temp: "); Serial.println(temperature);
  Serial.print("Humidity: "); Serial.println(humidity);
}

void setup() {
  Serial.begin(9600);
  dht.begin();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(2000L, sendData);  // Send every 2 sec
}

void loop() {
  Blynk.run();
  timer.run();
}