#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <HexDump.h>
#include <PubSubClient.h>
#include "AWS_Root_CA.h" // This file is created using AmazonRootCA1.pem from https://www.amazontrust.com/repository/AmazonRootCA1.pem
#include <EthernetWebServer_SSL.h>

//#include <SSLClient.h>
//#include <esp32-mqtt.h>
//#include <certificates_google.h>

#define ETH_SPI_BUS HSPI
#define ETH_MOSI 13
#define ETH_MISO 12
#define ETH_SCLK 14
#define ETH_CS 15

#define ANALOG_PIN_FOR_ENTROPY 36
#define MAX_SSL_SESSIONS 1
#define SSL_DEBUG_LEVEL SSLClient::DebugLevel::SSL_DUMP

#define THING_NAME "ESP32_TEMP"
#define MQTT_PACKET_SIZE 1024

const char my_cert[] =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDWTCCAkGgAwIBAgIUG2rXcAUBk1U/Q/2s2kfmg53EzOgwDQYJKoZIhvcNAQEL\n"
    "BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n"
    "SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIyMDkwMTA5NDcx\n"
    "MFoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n"
    "ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALxdgbNeuAEJGGd+lYK9\n"
    "uYc1BxXobXBWgNgJe/nSGYOj/35JIB/vPbJMRkRExusl5xnwfCyyCTTpYSmeK72I\n"
    "33RGLzuGPzJxVtHhAUh0VN3aCg7KOebGJD1qMWSZtbgHiJGr4tlZ9K21MSWs1lH1\n"
    "oiC58aE58g/98avKdl2O3n+j/NStAjy42QKzug2mdwEZdNLsS5mLZ31UE12rb2Wf\n"
    "ZUskZgOVuiACp6KjNa9Dah4Fy9dqM5esIdCRL9QxCZnv8yFD/pQkAIyu+7rQdWDN\n"
    "G7gl+4pmBXPfXQuhC4tPY0ccQWfPKglsPf2xmEpQgnfihihijlvdGP90fgz/lITe\n"
    "Q1ECAwEAAaNgMF4wHwYDVR0jBBgwFoAUCH8ptygk40UMDIpT3bpV83xPwNswHQYD\n"
    "VR0OBBYEFPOcyr5N1NX1wurbccC+F1isZvvQMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n"
    "AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQAX9H/0DKTDRJMglUS2puM8kKa2\n"
    "pZe31MJAhFkxgj//H2wtMXa6Wf3iTelN6p3iW70V5tL/+Kb5okhSertmfvaH3txs\n"
    "YiFkdos7WWk7s0UyrzfagPtnm89sRAFJyBuM5Blu/MPCmNTwucTDybg9JYazJhYq\n"
    "yzVJslShU/I3w8czVxl/i3fpePtmRt5Jxf4h9wZOvxtEy6v0Vr/Cu8QlQzwjIllR\n"
    "x6KguEitPYhsBzQarevvi521lssTHpb5YN7R01vtU11s7Gk0qef9nny2AG6AS0Hq\n"
    "25rKNUEBgfiOri6y87csVQmNYfeLvN5dItPzda3xC2Ti+sYmi1Zvig4C0hLa\n"
    "-----END CERTIFICATE-----\n";

const char my_key[] =
    "-----BEGIN RSA PRIVATE KEY-----\n"
    "MIIEpQIBAAKCAQEAvF2Bs164AQkYZ36Vgr25hzUHFehtcFaA2Al7+dIZg6P/fkkg\n"
    "H+89skxGRETG6yXnGfB8LLIJNOlhKZ4rvYjfdEYvO4Y/MnFW0eEBSHRU3doKDso5\n"
    "5sYkPWoxZJm1uAeIkavi2Vn0rbUxJazWUfWiILnxoTnyD/3xq8p2XY7ef6P81K0C\n"
    "PLjZArO6DaZ3ARl00uxLmYtnfVQTXatvZZ9lSyRmA5W6IAKnoqM1r0NqHgXL12oz\n"
    "l6wh0JEv1DEJme/zIUP+lCQAjK77utB1YM0buCX7imYFc99dC6ELi09jRxxBZ88q\n"
    "CWw9/bGYSlCCd+KGKGKOW90Y/3R+DP+UhN5DUQIDAQABAoIBADzmqpmYxTsoeqhz\n"
    "h9lmiAaMAYd2x3NrtGqk3l2OLUufiWqGMAnDKxpriMhnyDyc25RKaYt14H4n+o8f\n"
    "IZs9FqocSwcex24CsQRYkDahzhKQfXH8ti3D4FGu9NtGyzb90Izwmv6VFAPZslMx\n"
    "B3YmSc308IelGXtMKofVhOGI4K05hOhuoKS93CqP6iqxTiGy+aZfXfNSmtEmpg1t\n"
    "lezlLv1SbAbdXFW1RvGFROnSWWuTNgEtED83HJS6pDLwb1gD+wkd82hDlvHe5L+m\n"
    "E0cALme+IDyBUDu7oucAsHGbzDdBHYH7CEznuuEA4NYN8FLRcKWVNmSbzrigvE90\n"
    "EU60T/UCgYEA4WTtdC1BgFrticqyJXvdl0IQRAADKZRaqwpwY8CPfFYQgHuyJ4+T\n"
    "LeRC1M5hrKSp0zmJR2rk/VylieuaUgnvmcCPjZXX12x6bBAnfOntJ6e04p3arChX\n"
    "7iBJwoHZhIkMzMPy+PY8yFHgcHPW83BhqSYeb0mEwLUZvAVIEbAqYkcCgYEA1fFk\n"
    "FUlwvRwk/0ly2kYLiMi5qlIDf4E5A/L3u9z8ub90OFkB0qIByiZr/u5Ay91Eg3SV\n"
    "F6pypSlZbHPuj1laCxlWkm7Z3Wdqw7VOYe/hRwfMA0D6GvPjwRdxuhHzcydlpk6U\n"
    "pv5U+d9u4noyfUWnmpVKXjBXdXyb6l5pLQqLIacCgYEAmnQ9PDWfA0RMLvaV6ZXe\n"
    "Hb8Jwm93u1CMaVU3+0ODkCKyIDmjtfRvWS1olsfuqw4TyklNR4F69hw83Cx1je59\n"
    "IJGSTeX3CQqvQco4JSNwfpIwJHMj3IGw7990bDPrSSVZfiBxKiYjNOrWsHjRPfUv\n"
    "F2ilxoFM1XnE7tF3t5dPFBUCgYEAwqI28P69J+5A4lcJu/XZbHlgH8k5EXiYTDVw\n"
    "l2J2MO4yZ0jiVaO5iRTnTyeln1IPkKAxOzR/1C4IAph9wcACIPLORa2/WqNJB/8b\n"
    "7hFzm1FGkvc8EeUroHEZr7PBDVQcL+5xYnMjD6frkvKkmD8UOZfz7jiAeIwCtHdr\n"
    "q+RZqyUCgYEAgSfV0pRbbWL1+CvsyrzYqYoLY3w6Xy7NF7dNbll8OaLcaAyUEfsi\n"
    "D04COWLTJ4rAoTbTJtHyiUSoaV929yPfg1P5MlL6vogJTJrV8MeTTFo3RyIaQoF8\n"
    "8+OjBsMa4hrxj45has7BDgc4WD615El8b9o7brd4um7iEctt3nh+Qn8=\n"
    "-----END RSA PRIVATE KEY-----\n";

void callback(char *topic, byte *payload, unsigned int length);

SSLClientParameters mTLS = SSLClientParameters::fromPEM(my_cert, sizeof my_cert, my_key, sizeof my_key);

const char *mqttServer = "a3ei8e16py0910-ats.iot.us-east-1.amazonaws.com";
const char publishShadowUpdate[] = "$aws/things/" THING_NAME "/shadow/update";
char publishPayload[MQTT_PACKET_SIZE];

// Enter a MAC address and IP address for your controller below.a3ei8e16py0910-ats.iot.us-east-1.amazonaws.com
// The IP address will be dependent on your local network:
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
unsigned long lastMillis = 0;
SPIClass ethernetSPI(ETH_SPI_BUS);

/*google*/
/*EthernetClient ethClient;
SSLClient ethSSLClient(ethClient, TAs_GOOGLE, (size_t)TAs_NUM, ANALOG_PIN_FOR_ENTROPY, MAX_SSL_SESSIONS, SSL_DEBUG_LEVEL); */

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
  /*  Ethernet.init(ETH_CS, &ethernetSPI, ETH_SCLK, ETH_MISO, ETH_MOSI);
   Serial.begin(115200);
   while (!Serial)
   {
     ; // wait for serial port to connect. Needed for native USB port only
   }
   Serial.println("Google IoT Core Ethernet Example");
   Serial.println("Trying to recognize hardware and join via DHCP...");
   Ethernet.begin(mac);
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
   setupCloudIoT(&ethSSLClient); */
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