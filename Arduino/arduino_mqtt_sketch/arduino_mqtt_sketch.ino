// LED pins
const int redLedPin = 12;
const int greenLedPin = 13;

void setup() {
  // Start serial communication at 9600 baud rate
  Serial.begin(9600);

  // Initialize LEDs
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);

  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, LOW);

  delay(1000);
}

void loop() {
  // Request MQTT connection via PC
  Serial.println("CONNECT_MQTT");

  // Wait for response
  if (Serial.available() > 0) {
    String response = Serial.readStringUntil('\n');

    if (response == "MQTT_CONNECTED") {
      digitalWrite(greenLedPin, HIGH);
    }
  }

  delay(30000);
}
