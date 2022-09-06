#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <SSLClient.h>
#include <certificates.h>
#include <esp32-mqtt.h>
#include <HexDump.h>

#define ETH_SPI_BUS HSPI
#define ETH_MOSI 13
#define ETH_MISO 12
#define ETH_SCLK 14
#define ETH_CS 15

#define ANALOG_PIN_FOR_ENTROPY 36
#define MAX_SSL_SESSIONS 1
#define SSL_DEBUG_LEVEL SSLClient::DebugLevel::SSL_DUMP

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
unsigned long lastMillis = 0;
SPIClass ethernetSPI(ETH_SPI_BUS);
EthernetClient ethClient;
SSLClient ethSSLClient(ethClient, TAs, (size_t)TAs_NUM, ANALOG_PIN_FOR_ENTROPY, MAX_SSL_SESSIONS, SSL_DEBUG_LEVEL);

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
  setupCloudIoT(&ethSSLClient);
}

void loop_eth_google()
{
  mqtt->loop();
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
  }
}
