#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
//#include <SSLClient.h>
//#include <esp32-mqtt.h>
//#include <certificates_google.h>
#include <HexDump.h>
#include <PubSubClient.h>
#include "AWS_Root_CA.h" // This file is created using AmazonRootCA1.pem from https://www.amazontrust.com/repository/AmazonRootCA1.pem
#include "EthernetWebServer_SSL-impl.h"

#define ETH_SPI_BUS HSPI
#define ETH_MOSI 13
#define ETH_MISO 12
#define ETH_SCLK 14
#define ETH_CS 15

#define ANALOG_PIN_FOR_ENTROPY 36
#define MAX_SSL_SESSIONS 1
#define SSL_DEBUG_LEVEL SSLClient::DebugLevel::SSL_DUMP

#define THING_NAME "<Thing_Name>"
#define MQTT_PACKET_SIZE 1024

const char my_cert[] =
    "-----BEGIN CERTIFICATE-----\n"
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
    "-----END CERTIFICATE-----\n";

const char my_key[] =
    "-----BEGIN RSA PRIVATE KEY-----\n"
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
    "-----END RSA PRIVATE KEY-----\n";

void callback(char *topic, byte *payload, unsigned int length);

SSLClientParameters mTLS = SSLClientParameters::fromPEM(my_cert, sizeof my_cert, my_key, sizeof my_key);

const char *mqttServer = "xxxxxxxxxxxx-ats.iot.us-east-1.amazonaws.com";
const char publishShadowUpdate[] = "$aws/things/" THING_NAME "/shadow/update";
char publishPayload[MQTT_PACKET_SIZE];

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
unsigned long lastMillis = 0;

/*google*/
SPIClass ethernetSPI(ETH_SPI_BUS);
EthernetClient ethClient;
// SSLClient ethSSLClient(ethClient, TAs_GOOGLE, (size_t)TAs_NUM, ANALOG_PIN_FOR_ENTROPY, MAX_SSL_SESSIONS, SSL_DEBUG_LEVEL);

/*amazon*/
EthernetClient ethClientAmazon;
EthernetSSLClient ethClientSSL(ethClientAmazon, TAs, (size_t)TAs_NUM, 1);
PubSubClient mqtt_istance(mqttServer, 8883, callback, ethClientSSL);

extern "C" void C_HexDump(const uint8_t *buff, size_t len)
{
  Serial.println("Decrypted received data");
  HexDump(Serial, (void *)buff, len);
}

void setup_eth_google()
{
  Ethernet.init(ETH_CS, &ethernetSPI, ETH_SCLK, ETH_MISO, ETH_MOSI);
  Serial.begin(115200);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Google IoT Core Ethernet Example");
  Serial.println("Trying to recognize hardware and join via DHCP...");
  Ethernet.begin(mac /*, ip, dns, gateway*/);
  Serial.println("Got out of Ethernet.begin.");
  Serial.flush();
  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware)
  {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true)
    {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF)
  {
    Serial.println("Ethernet cable is not connected.");
  }

  Serial.print("Joined LAN with IP ");
  Serial.println(Ethernet.localIP());
  Serial.flush();
  delay(2000);
  // setupCloudIoT(&ethSSLClient);
}

void loop_eth_google()
{
  /*   mqtt->loop();
    delay(10); // <- fixes some issues with WiFi stability

    if (!mqttClient->connected())
    {
      connect();
    }

    if (millis() - lastMillis > 6000)
    {
      lastMillis = millis();
      Serial.print("Entro");
      publishTelemetry(getDefaultSensor());
}*/
}

void MQTTPublish(const char *topic, char *payload);
void updateThing();

char *subscribeTopic[5] =
    {
        "$aws/things/" THING_NAME "/shadow/update/accepted",
        "$aws/things/" THING_NAME "/shadow/update/rejected",
        "$aws/things/" THING_NAME "/shadow/update/delta",
        "$aws/things/" THING_NAME "/shadow/get/accepted",
        "$aws/things/" THING_NAME "/shadow/get/rejected"};

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }

  Serial.println();
}

void reconnect()
{
  while (!mqtt_istance.connected())
  {
    Serial.print("Attempting MQTT connection...");

    if (mqtt_istance.connect("arduinoClient"))
    {
      Serial.println("connected");

      for (int i = 0; i < 5; i++)
      {
        // Serial.println(subscribeTopic[i]);
        mqtt_istance.subscribe(subscribeTopic[i]);
      }

      Serial.println("Started updateThing ");
      updateThing();
      Serial.println("Done updateThing ");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(mqtt_istance.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void updateThing()
{
  strcpy(publishPayload, "{\"state\": {\"reported\": {\"powerState\":\"ON\"}}}");
  MQTTPublish(publishShadowUpdate, publishPayload);
}

void MQTTPublish(const char *topic, char *payload)
{
  mqtt_istance.publish(topic, payload);
  Serial.print("Published [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(payload);
}

void setup()
{
  Ethernet.init(ETH_CS, &ethernetSPI, ETH_SCLK, ETH_MISO, ETH_MOSI);
  Serial.begin(115200);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop()
{
  if (!mqtt_istance.connected())
  {
    reconnect();
  }

  mqtt_istance.loop();
}