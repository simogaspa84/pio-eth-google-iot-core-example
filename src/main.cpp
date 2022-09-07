#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include "Ethernet_pinout_setting.h"
#include <PubSubClient.h>
#include <SSL_Utility/EthernetWebServer_SSL.h>
#include "AWS_security.h"

void MQTTPublish(const char *topic, char *payload);
void updateThing();
void callback(char *topic, byte *payload, unsigned int length);

SPIClass ethernetSPI(ETH_SPI_BUS);

EthernetClient ethClient;
EthernetSSLClient ethClientSSL(ethClient, TAs, (size_t)TAs_NUM, 1);
PubSubClient mqtt_istance(mqttServer, 8883, callback, ethClientSSL);
SSLClientParameters mTLS = SSLClientParameters::fromPEM(my_cert, sizeof my_cert, my_key, sizeof my_key);

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
        Serial.println(subscribeTopic[i]);
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
  /*authentication on aws*/
  ethClientSSL.setMutualAuthParams(mTLS);

  Ethernet.init(ETH_CS, &ethernetSPI, ETH_SCLK, ETH_MISO, ETH_MOSI);
  Serial.begin(115200);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Ethernet.begin(mac);
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
}

void loop()
{
  if (!mqtt_istance.connected())
  {
    reconnect();
  }

  mqtt_istance.loop();
}