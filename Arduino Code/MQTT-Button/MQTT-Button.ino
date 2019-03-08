
/**********************************************************************************************
  MQTT Button
  Written by: Jay Collett jay AT gmail.com
  Description:

  Sketch includes unmodified code libraries from the following awesome sources:
      PubSubClient  https://github.com/knolleary/pubsubclient
      ESP8266WiFi   https://github.com/esp8266/Arduino
 ***********************************************************************************************/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define phyButton1 5
#define phyButton2 12

#define WIFI_SSID "xxxxxxx"
#define WIFI_PASS "xxxxxxxxx"
#define MQTT_PORT 1883

int   wifiRetryCount = 0;
char  fmversion[7] = "v1.0";                  // firmware version
char  mqtt_server[] = "192.168.0.x";          // MQTT broker IP address
char  mqtt_username[] = "mqttLabButton";      // username for MQTT broker (USE ONE)
char  mqtt_password[] = "xxxxxxxxxxxxx";      // password for MQTT broker
char  mqtt_clientid[] = "mqttLabButton";      // client id for connections to MQTT broker

const String HOSTNAME  = "mqttLabButton";
const String baseTopic = "mqttLabButton";
const String buttonOneTopic = baseTopic + "/" + "button1";
const String buttonTwoTopic = baseTopic + "/" + "button2";

String strTopic;
String strPayload;
bool button1On = false;
bool button2On = false;

IPAddress ip;
WiFiClient WiFiClient;
PubSubClient mqttclient(WiFiClient);

ADC_MODE(ADC_VCC);

void setup() {
  pinMode(phyButton1, INPUT);
  pinMode(phyButton2, INPUT);

  Serial.begin(115200);

  WiFi.hostname(HOSTNAME);
  WiFi.mode(WIFI_STA);

  mqttclient.setServer(mqtt_server, MQTT_PORT);
  mqttclient.setCallback(callback);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
    if (wifiRetryCount >= 120) { // 250mS delay x 120 iterations = 30 seconds
      ESP.restart();
    }
    wifiRetryCount++;
  }

  wifiRetryCount = 0;

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Calling MQTT Connect");
  MQTT_connect(); // connect to wifi/mqtt as needed
}

void loop() {

  // loop indefinitely, checking to see if someone has hit the doorbell button
  // doorbell sense pin will go HIGH if the doorbell button is being pushed
  if (digitalRead(phyButton1) == HIGH) {
    delay(150); // poor man's debounce
    if (digitalRead(phyButton1) == HIGH) {
      Serial.println("Somone hit button 1, send the MQTT message");
      // toggle on/off based on state of button
      if(button1On){
        mqttclient.publish(buttonOneTopic.c_str(), "OFF");
        Serial.println("Sent off payload for button 1");
      }else{
        mqttclient.publish(buttonOneTopic.c_str(), "ON");
        Serial.println("Sent on payload for button 1");
      }
      delay(1000); // give time for the payload to register, and prevent silly button mashers from being dumb.
    }
  }

}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  payload[length] = '\0';
  strTopic = String((char*)topic);
  if (strTopic == buttonOneTopic)
  {
    if (String((char*)payload) == "ON")
    {
      Serial.println("Button ONE State Is Now ON");
      button1On = true;
    }
    else
    {
      Serial.println("Button ONE State Is Now OFF");
      button1On = false;
    }
  } else {
    if (String((char*)payload) == "ON")
    {
      Serial.println("Button TWO State Is Now ON");
      button2On = true;
    }
    else
    {
      Serial.println("Button TWO State Is Now OFF");
      button2On = false;
    }
  }
}


// #############################################################################
//  mqtt Connect function
//  This function connects and reconnects as necessary to the MQTT server and
//  WiFi.
//  Should be called in the loop to ensure connectivity
// #############################################################################
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqttclient.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  // Loop until we're reconnected
  while (!mqttclient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttclient.connect(mqtt_clientid, mqtt_username, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttclient.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
  // resubscribe to our state topics
  mqttclient.subscribe(buttonOneTopic.c_str());
  mqttclient.subscribe(buttonTwoTopic.c_str());

  Serial.println("MQTT Connected!");
}
