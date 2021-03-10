#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

WiFiClient wifi;
PubSubClient mqtt;
SoftwareSerial p1s;

constexpr char MQTT_SERVER[] = "test.mosquitto.org";
constexpr int MQTT_PORT = 1883;

constexpr char WIFI_SSID[] = "Ties Netwerk 2.4";
constexpr char WIFI_PASS[] = "TiesWifi2.4";

void connectMQTT()
{
  while (!mqtt.connected())
  {
    mqtt.connect("iwsn-wemos-client-ties");

    if (mqtt.connected())
    {
      Serial.println("MQTT Connected");
      mqtt.publish("iwsn-wemos-ties", "Wemos Connected!");
      break;
    }
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASS); // Connect to the Wi-Fi (if not known use WifiManager from tzapu!)
  Serial.print("Setup Wi-Fi:");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  Serial.println();
  Serial.print("Connected:");
  Serial.println(WiFi.localIP());

  mqtt.setClient(wifi);    // Setup the MQTT client
  mqtt.setBufferSize(256); // override MQTT_MAX_PACKET_SIZE
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  connectMQTT();

  Serial.flush();

  p1s.begin(9600, SWSERIAL_7E1);

  delay(2000);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (p1s.available())
  {
    Serial.println(p1s.read());
  }
}