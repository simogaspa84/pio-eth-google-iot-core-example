#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <SSLClient.h>
// generated by  
// wget --no-check-certificate https://pki.goog/gtsltsr/gtsltsr.crt
// openssl x509 -in gtsltsr.crt -inform DER -out gtsltsr.pem -outform PEM
// python pycert_bearssl.py convert -n gtsltsr.pem
#include <certificates.h>
#include <esp32-mqtt.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

#define ETH_SPI_BUS HSPI
#define ETH_MOSI 13
#define ETH_MISO 12
#define ETH_SCLK 14
#define ETH_CS 15

SPIClass ethernetSPI(ETH_SPI_BUS);

#define ANALOG_PIN_FOR_ENTROPY 36

EthernetClient ethClient;
SSLClient ethSSLClient(ethClient, TAs, (size_t)TAs_NUM, ANALOG_PIN_FOR_ENTROPY);

void setup()
{
    // You can use Ethernet.init(pin) to configure the CS pin and SPI
    // Init with specific pin settings from above.
    Ethernet.init(ETH_CS, &ethernetSPI, ETH_SCLK, ETH_MISO, ETH_MOSI);

    // Open serial communications and wait for port to open:
    Serial.begin(115200);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("Google IoT Core Ethernet Example");

    // start the Ethernet connection and the server:
    // use static IPs. Self-assign 192.168.0.37, set gateway & DNS as 192.168.0.1
    // this should match your home network settings
    //IPAddress ip(192,168,0,37);
    //IPAddress dns(192,168,0,1);
    //IPAddress gateway(192,168,0,1);
    Serial.println("Trying to recognize hardware and join via DHCP...");
    Ethernet.begin(mac/*, ip, dns, gateway*/);
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

unsigned long lastMillis = 0;

void loop()
{
  mqtt->loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!mqttClient->connected()) {
    connect();
  }

  // TODO: replace with your code
  // publish a message roughly every second.
  if (millis() - lastMillis > 60000) {
    lastMillis = millis();
    //publishTelemetry(mqttClient, "/sensors", getDefaultSensor());
    publishTelemetry(getDefaultSensor());
  }

// delay(5000);
}
