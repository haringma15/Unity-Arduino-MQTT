#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <Controllino.h>
#include <Encoder.h>

const char* ID = "dmt09"; 

// Network Settings
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xBD };
IPAddress ip(192, 168, 2, 24);

// MQTT Settings
const char* mqttServer = "dmt.fh-joanneum.at";
const int mqttPort = 1883;
const char* mqttUser = "dmt";
const char* mqttPassword = "ss2021";
const char* mqttTopic = "cosa/room3/habitat/solar";

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

int reConnect = 0;
int mqttCnt   = 0;

void setup() {
  Serial.begin(9600);
  delay(200);

  Serial.println("Exhibit /habitat/power starting...");

  mqttClient.setServer(mqttServer, mqttPort);

  Ethernet.begin(mac, ip);
  delay(2000);

  Serial.print("Exhibit IP address: ");

  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    Serial.print(Ethernet.localIP()[thisByte], DEC);

    if (thisByte < 3) Serial.print(".");
    if (thisByte == 3) Serial.println("");
  }

  if (mqttClient.connect("cosa-clientId-101010101", mqttUser, mqttPassword)) {
    Serial.println("Connected to MQTT broker");
  } else {
    Serial.print("MQTT connection failed with state: ");
    Serial.println(mqttClient.state());
  }
}

void loop() {
  // Reconnect to the MQTT broker if disconnected
  // if (!mqttClient.connected()) {
  //   reconnect();
  // }
  mqttClient.loop();
  if (!mqttClient.connected()) {  
    Serial.print("MQTT Disconnected!!! Try new MQTT connection. ");
    setup_mqtt();  // MQTT reconnection
  }
  mqttClient.loop();
  // Delay before sending the next MQTT message
  delay(5000);
}

void setup_mqtt() 
{
  long randNumber = random(10000000);
  char mqttCLIENT[50];
  sprintf(mqttCLIENT,"%s_%08d_%s", "dmt01", randNumber, ID);
  reConnect++;

    while (!mqttClient.connected()) {

        if (reConnect == 1) 
          sprintf("Data init","MQTT first connection... Client: %s",mqttCLIENT);       
        else
          sprintf("Data init","MQTT reconnecting... [%04d] Client: %s ",reConnect ,mqttCLIENT);
        Serial.print("Data init");
        
        // boolean connect (clientID, [username, password], [willTopic, willQoS, willRetain, willMessage], [cleanSession])
        sprintf("Data init","MQTT died: %s",ID);
        if (!mqttClient.connect(mqttCLIENT, mqttUser, mqttPassword, mqttTopic, 2, true, "Data init")) {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" retrying in some seconds");
            delay(1500);
          }
    }
    Serial.println();
    
    mqttClient.subscribe(mqttTopic, 1);

    // now when WiFi and MQTT works > Info also to MQTT
    // sprintf("Data init","WiFi [%s]: %s @ %s", ID, WiFi.localIP().toString().c_str(), mySSID); 
    mqttClient.publish(mqttTopic, "Data init", true); 
    Serial.println("Data init"); 
    
    sprintf("Data init","MQTT connected OK: %s - #%d", mqttServer, reConnect); 
    Serial.println("=NIS=MQTT=............................................................=OK=");    
}

// Function to connect to MQTT broker
void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "cosaClient-";
    clientId += String(random(0xffff), HEX);

    if (mqttClient.connect("cosa-clientId-101010101", mqttUser, mqttPassword)) {
      Serial.println("connected");
      mqttClient.subscribe(mqttTopic);

      // Publish a test message
      mqttClient.publish(mqttTopic, "Hello World from solar!");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}