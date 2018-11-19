// Import required libraries
#include "ESP8266WiFi.h"
#include "DHT.h"

// WiFi parameters
const char *ssid = "";
const char *password = "";

// Pin
#define DHTPIN 5

// Use DHT11 sensor
#define DHTTYPE DHT11

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE, 15);

// Host
const char* host = "dweet.io";

float count = 0;

void setup() {

  // Start Serial
  Serial.begin(115200);
  delay(10);

  // Init DHT
  dht.begin();

  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA); // <<< Station
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
  }

  // Reading temperature and humidity
  float humidity = dht.readHumidity();
  float temp = dht.readTemperature(true); // true for Fahrenheit

  if (!isnan(temp) && !isnan(humidity)) {
  // Send the request to the server
  count++;
  client.print(String("GET /dweet/for/<Dweet_ID>?Temperature=") + String(temp) + "&Humidity=" + String(humidity) + "&Count=" + String(count) + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  }

  delay(10000); // Repeat every 10 seconds

}
