// LED pins
const int redLedPin = 12;
const int greenLedPin = 13;
const int buttonPin = 11;
bool mqttConnected = false;

void setup() {
  // Start serial communication at 9600 baud rate
  Serial.begin(9600);

  // Initialize LEDs
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, LOW);

  delay(1000);
}

void loop() {
  // Request MQTT connection via PC
  if (!mqttConnected) {
    Serial.println("CONNECT_MQTT");

    // Wait for response
    if (Serial.available() > 0) {
      String response = Serial.readStringUntil('\n');

      if (response == "MQTT_CONNECTED") {
        mqttConnected = true;
        digitalWrite(greenLedPin, HIGH);
        delay(2000);
        digitalWrite(greenLedPin, LOW);
      }
    } else {
      digitalWrite(redLedPin, HIGH);
      delay(200);
      digitalWrite(redLedPin, LOW);
    }
  }

  if (mqttConnected && digitalRead(buttonPin) == HIGH) {
    Serial.println("PUBLISH_MQTT");
    digitalWrite(redLedPin, HIGH);
    delay(1000);
    digitalWrite(redLedPin, LOW);
  }

  delay(5000);
}
