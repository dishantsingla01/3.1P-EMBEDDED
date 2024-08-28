#include <WiFiNINA.h>
#include <Wire.h>
#include <BH1750.h>

char wifiSSID[] = "Tushar";
char wifiPass[] = "87654321";

WiFiClient wifiClient;

BH1750 sensor; // Instance of the BH1750 light sensor library
int ledPin = 2;
char serverName[] = "maker.ifttt.com"; // IFTTT server
String webhookURL = "https://maker.ifttt.com/trigger/dishant/with/key/m_wIhOiboWdUSFgTG97XzzD4FUIguSK0TbxLhsOcbvN"; // Webhook URL

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  Wire.begin();
  sensor.begin();

  Serial.println(F("Light sensor initialization complete"));

  // Connect to WiFi network
  initializeWiFi();
}

void loop()
{
  float lightLevel = sensor.readLightLevel();
  Serial.print("Light Level: ");
  Serial.println(lightLevel);

  if (lightLevel >= 360)
  {
    sendNotification("Daylight");
  }
  else if (lightLevel < 80)
  {
    sendNotification("Nighttime");
  }
  delay(1000); // Pause for a while before reading the light level again
}

void initializeWiFi()
{
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin(wifiSSID, wifiPass);
    delay(5000); // Retry every 5 seconds
    Serial.print(".");
  }
  Serial.println("\nSuccessfully connected to WiFi");
}

void sendNotification(String event)
{
  if (WiFi.status() != WL_CONNECTED)
  {
    initializeWiFi();
  }

  String eventData = "?value1=" + event;
  if (wifiClient.connect(serverName, 80))
  {
    wifiClient.println("GET " + webhookURL + eventData + " HTTP/1.1");
    wifiClient.println("Host: " + String(serverName));
    wifiClient.println("Connection: close");
    wifiClient.println();
    delay(500);
    wifiClient.stop();
    Serial.println("Notification sent successfully");
  }
  else
  {
    Serial.println("Failed to connect to IFTTT server");
  }
}