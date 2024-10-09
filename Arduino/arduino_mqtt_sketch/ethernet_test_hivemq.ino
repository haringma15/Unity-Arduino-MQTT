// #include <SPI.h>
// #include <Ethernet.h>
// #include <PubSubClient.h>

// // HiveMQ public broker address
// const char* mqtt_server = "broker.hivemq.com"; // Replace with your private HiveMQ broker if needed
// const int mqtt_port = 1883;                    // MQTT port

// // Ethernet settings
// byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xBD };
// IPAddress ip(192, 168, 2, 24);

// // Create Ethernet client
// EthernetClient ethClient;
// // Create MQTT client
// PubSubClient mqttClient(ethClient);

// // Function to handle incoming messages from the broker
// void callback(char* topic, byte* payload, unsigned int length) {
//   Serial.print("Message arrived [");
//   Serial.print(topic);
//   Serial.print("]: ");
//   for (int i = 0; i < length; i++) {
//     Serial.print((char)payload[i]);
//   }
//   Serial.println();
// }

// // Function to reconnect to the MQTT broker
// void reconnect() {
//   // Loop until reconnected
//   while (!mqttClient.connected()) {
//     Serial.print("Attempting MQTT connection...");
//     // Attempt to connect
//     if (mqttClient.connect("ArduinoClient")) {
//       Serial.println("Connected");
//       // Subscribe to the topic once connected
//       mqttClient.subscribe("test/topic");
//     } else {
//       Serial.print("Failed, rc=");
//       Serial.print(mqttClient.state());
//       Serial.println(" try again in 5 seconds");
//       delay(5000);
//     }
//   }
// }

// void setup() {
//   // Initialize Serial for debug output
//   Serial.begin(9600);

//   // Start Ethernet connection
//   if (Ethernet.begin(mac) == 0) {
//     Serial.println("Failed to configure Ethernet using DHCP");
//     Ethernet.begin(mac, ip);
//   }
//   delay(1500); // Give time for Ethernet to initialize

//   // Set the server address and the callback function
//   mqttClient.setServer(mqtt_server, mqtt_port);
//   mqttClient.setCallback(callback);
// }

// void loop() {
//   // Ensure Ethernet connection
//   if (Ethernet.linkStatus() == LinkOFF) {
//     Serial.println("Ethernet cable disconnected");
//   }

//   // Ensure MQTT connection
//   if (!mqttClient.connected()) {
//     reconnect();
//   }
//   mqttClient.loop();

//   // Example: Publish message to topic every 5 seconds
//   static unsigned long lastMsg = 0;
//   unsigned long now = millis();
//   if (now - lastMsg > 5000) {
//     lastMsg = now;
//     String message = "Hello from Arduino!";
//     Serial.print("Publishing message: ");
//     Serial.println(message);
//     mqttClient.publish("test/topic", message.c_str());
//   }
// }