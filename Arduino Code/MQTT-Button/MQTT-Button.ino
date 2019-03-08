
/**********************************************************************************************
  MQTT Button
  Written by: Jay Collett jay.collett AT gmail.com
  Description: 
  
  Sketch includes unmodified code libraries from the following awesome sources:
      PubSubClient  https://github.com/knolleary/pubsubclient
      ESP8266WiFi   https://github.com/esp8266/Arduino
 ***********************************************************************************************/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define phyButton1 5
#define phyButton2 12

#define WIFI_SSID "xxxxxxxxxxxxxx"
#define WIFI_PASS "xxxxxxxxxxxx"
#define MQTT_PORT 1883

char  fmversion[7] = "v1.0";                  // firmware version
char  mqtt_server[] = "192.168.0.x";          // MQTT broker IP address
char  mqtt_username[] = "xxxxxxxxxxxxxxx";    // username for MQTT broker (USE ONE)
char  mqtt_password[] = "xxxxxxxxxxxxx";      // password for MQTT broker
char  mqtt_clientid[] = "mqttLabButton";      // client id for connections to MQTT broker


const String HOSTNAME  = "mqttLabButton";
const String baseTopic = "mqttLabButton";
const String buttonOneTopic = baseTopic + "/" + "button1";
const String buttonTwoTopic = baseTopic + "/" + "button2";

IPAddress ip;

WiFiClient WiFiClient;
PubSubClient mqttclient(WiFiClient);
Adafruit_Si7021 sensor = Adafruit_Si7021();


ADC_MODE(ADC_VCC);


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
