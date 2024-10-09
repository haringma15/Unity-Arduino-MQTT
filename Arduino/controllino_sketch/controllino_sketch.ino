/*
  Exhibit V2 /habitat/power
  (Energie V2)
*/

// Including Necessary Libraries
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <Controllino.h>
#include <Encoder.h>

//////////////////////
///// VARIABLES /////
////////////////////

///// Network & MQTT /////

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xBD };
IPAddress ip(192, 168, 2, 24);

const char* mqtt_server = "dmt.fh-joanneum.at";
const int mqtt_port = 1883;
const char* mqtt_username = "dmt";
const char* mqtt_password = "ss2021";
const char* mqtt_topic = "cosa/room3/habitat/solar";

EthernetClient eth_client;
PubSubClient mqtt_client(eth_client);

///// Misc /////

// unsigned long debounceDelay = 50;
// unsigned long lastDebounceTimeButton = 0;
// unsigned long lastDebounceTimeJoystickLeft = 0;
// unsigned long lastDebounceTimeJoystickRight = 0;
// unsigned long lastDebounceTimeJoystickButton = 0;

// int ButtonReading = 0;
// int ButtonState = 0;
// int OldButtonState = 0;
// float Voltage = 0.00;

// boolean BackupPowerAvailable = false;
// boolean MeterSequenceStarted = false;

// // Encoder WheelLeftRight(CONTROLLINO_IN0, CONTROLLINO_IN1);
// long WheelLeftRightNewPosition = 0;
// long WheelLeftRightOldPosition = 0;

// unsigned long MeterSequenceTimer = 0;
// unsigned long MeterSequenceTick = 0;
// unsigned long MeterSequenceSendTick = 0;

// float MeterONEValue = 0.00;
// float MeterTWOValue = 0.00;
// float MeterTHREEValue = 0.00;
// float MeterFOURValue = 0.00;

// int WheelUpDown = 0;
// int OldWheelUpDown = 0;
// int DeltaWheelUpDown = 0;
// int THISDeltaWheelUpDown = 0;

// unsigned long timeNow = 0;
// unsigned long SensorTimestamp = 0;
// unsigned long SensorTimestampTWO = 0;

// long DeltaWheelLeftRight = 0;

// boolean LoginState = false;
// boolean DebugState = false;

/////////////////
///// MAIN /////
///////////////

void setup() {
  setup_serial();
  Serial.println("Exhibit /habitat/power: starting setup... -----");");

  setup_pins();
  setup_ethernet();
  setup_mqtt();

  Serial.println("Exhibit /habitat/power: setup complete. -----");
}

void loop() {
  handle_ethernet_connection();
  handle_mqtt_connection();


  // // Example: Publish message to topic every 5 seconds
  // static unsigned long lastMsg = 0;
  // unsigned long now = millis();
  // if (now - lastMsg > 5000) {
  //   lastMsg = now;
  //   String message = "Hello from Arduino!";
  //   Serial.print("Publishing message: ");
  //   Serial.println(message);
  //   mqttClient.publish("test/topic", message.c_str());
  // }

  // Delay before sending the next MQTT message
  //delay(5000);

  // OSCMsgReceive();
  // ReadInputs();

  // if (MeterSequenceStarted == true) {
  //   MeterSequence();
  // }
}

//////////////////
///// SETUP /////
////////////////

void setup_serial() {
  Serial.begin(9600);
  delay(200);

  Serial.println("Exhibit /habitat/power: Serial communication established.");
}

void setup_pins() {
  Serial.println("Exhibit /habitat/power: setup pins...");
  
  pinMode(CONTROLLINO_A0, INPUT);   // StartButton
  pinMode(CONTROLLINO_A2, INPUT);   // WheelLeftRight
  pinMode(CONTROLLINO_D0, OUTPUT);  // Emergency Power Lamp Red
  pinMode(CONTROLLINO_D1, OUTPUT);  // Emergency Power  Lamp Green
  pinMode(CONTROLLINO_D2, OUTPUT);  // Solar Lamp Red
  pinMode(CONTROLLINO_D3, OUTPUT);  // Solar Lamp Green
  pinMode(CONTROLLINO_D4, OUTPUT);  // Emergency Power Meter
  pinMode(CONTROLLINO_D5, OUTPUT);  // Solar Power 1 Meter
  pinMode(CONTROLLINO_D6, OUTPUT);  // Solar Power 2 Meter
  pinMode(CONTROLLINO_D7, OUTPUT);  // Solar Power 3 Meter
  pinMode(CONTROLLINO_D8, OUTPUT);  // Solar Power 4 Meter

  digitalWrite(CONTROLLINO_D0, LOW);
  digitalWrite(CONTROLLINO_D1, LOW);
  digitalWrite(CONTROLLINO_D2, LOW);
  digitalWrite(CONTROLLINO_D3, LOW);
  analogWrite(CONTROLLINO_D4, 0);
  analogWrite(CONTROLLINO_D5, 0);
  analogWrite(CONTROLLINO_D6, 0);
  analogWrite(CONTROLLINO_D7, 0);
  analogWrite(CONTROLLINO_D8, 0);

  Serial.println("Exhibit /habitat/power: pin setup complete.");
}

void setup_ethernet() {
  Serial.println("Exhibit /habitat/power: setup ethernet...");

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Exhibit /habitat/power: Failed to configure Ethernet using DHCP, use static ip instead");
    Ethernet.begin(mac, ip);
  }
  delay(1500);

  Serial.print("Exhibit /habitat/power: ethernet setup complete, IP address: ");

  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    Serial.print(Ethernet.localIP()[thisByte], DEC);

    if (thisByte < 3) Serial.print(".");
    if (thisByte == 3) Serial.println("");
  }
}

void setup_mqtt() {
  Serial.println("Exhibit /habitat/power: setup mqtt...");

  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(handle_received_message);

  Serial.println("Exhibit /habitat/power: mqtt setup complete.");
}

/////////////////
///// LOOP /////
///////////////

void handle_ethernet_connection() {
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Exhibit /habitat/power: Ethernet cable disconnected!");
  }
}

void handle_mqtt_connection() {
  if (!mqtt_client.connected()) {
    connect();
  }
  mqttClient.loop();
}

// void OSCMsgReceive() {
//   OSCMessage msgIN;
//   int size;

//   if ((size = Udp.parsePacket()) > 0) {
//     while (size--)
//       msgIN.fill(Udp.read());

//     if (!msgIN.hasError()) {
//       msgIN.route("/server/login_accepted", Login);
//       msgIN.route("/server/logout_complete", Logout);
//       msgIN.route("/habitat/power/voltage", GetVoltage);
//       msgIN.route("/habitat/power/backup_power_available", GetBackupPower);
//       msgIN.route("/server/debug_on", DebugOn);
//       msgIN.route("/server/debug_off", DebugOff);
//     }
//   }
// }

// void Pong(OSCMessage &msg, int addrOffset ) {
//   OSCMessage msgOUT("/habitat/power/pong");

//   msgOUT.add(incPort);
//   Udp.beginPacket(outIp, destPongPort);
//   msgOUT.send(Udp);
//   Udp.endPacket();
//   msgOUT.empty();

//   Serial.print("SENT: /habitat/power/pong ");

//   for (byte thisByte = 0; thisByte < 4; thisByte++) {
//     Serial.print(Ethernet.localIP()[thisByte], DEC);

//     if (thisByte < 3) Serial.print(".");
//     if (thisByte == 3) Serial.print(" ");
//   }

//   Serial.println(incPort);
// }

// void Login(OSCMessage &msg, int addrOffset ) {
//   WheelLeftRightNewPosition = WheelLeftRight.read();
//   WheelLeftRightOldPosition = WheelLeftRightNewPosition;
//   DeltaWheelLeftRight = 0;
//   WheelUpDown = analogRead(CONTROLLINO_A2);
//   OldWheelUpDown = WheelUpDown;
//   DeltaWheelUpDown = 0;
//   THISDeltaWheelUpDown = 0;

//   BackupPowerAvailable = false;
//   LoginState = true;

//   digitalWrite(CONTROLLINO_D0, HIGH);
//   digitalWrite(CONTROLLINO_D1, LOW);
//   digitalWrite(CONTROLLINO_D2, HIGH);
//   digitalWrite(CONTROLLINO_D3, LOW);
//   analogWrite(CONTROLLINO_D4, 0);
//   analogWrite(CONTROLLINO_D5, 0);
//   analogWrite(CONTROLLINO_D6, 0);
//   analogWrite(CONTROLLINO_D7, 0);
//   analogWrite(CONTROLLINO_D8, 0);
//   analogWrite(CONTROLLINO_D4, 0);

//   Serial.println("RECEIVED: login_accepted");
// }

// void Logout(OSCMessage &msg, int addrOffset ) {
//   LoginState = false;
//   BackupPowerAvailable = false;

//   analogWrite(CONTROLLINO_D4, 0);
//   digitalWrite(CONTROLLINO_D0, LOW);
//   digitalWrite(CONTROLLINO_D1, LOW);
//   digitalWrite(CONTROLLINO_D2, LOW);
//   digitalWrite(CONTROLLINO_D3, LOW);
//   analogWrite(CONTROLLINO_D4, 0);
//   analogWrite(CONTROLLINO_D5, 0);
//   analogWrite(CONTROLLINO_D6, 0);
//   analogWrite(CONTROLLINO_D7, 0);
//   analogWrite(CONTROLLINO_D8, 0);

//   Serial.println("RECEIVED: logout_complete");
// }

// void DebugOn(OSCMessage &msg, int addrOffset ) {
//   Serial.println("RECEIVED: debug_on");

//   DebugState = true;
// }

// void DebugOff(OSCMessage &msg, int addrOffset ) {
//   Serial.println("RECEIVED: debug_off");

//   DebugState = false;
// }

// void GetVoltage(OSCMessage &msg, int addrOffset ) {
//   Voltage = msg.getFloat(0);
//   Voltage = constrain(Voltage, 0.00, 1.00);
//   Voltage = Voltage * 100;

//   int VoltageInt = (int) Voltage;
//   VoltageInt = map(VoltageInt, 0, 100, 0, 190);

//   // Serial.println(VoltageInt);

//   analogWrite(CONTROLLINO_D4, VoltageInt);

//   Serial.print("RECEIVED: /habitat/power/voltage ");
//   Serial.print(Voltage);
//   Serial.print(" [");
//   Serial.print(VoltageInt);
//   Serial.println("]");
// }

// void GetBackupPower() {
//   BackupPowerAvailable = true;

//   digitalWrite(CONTROLLINO_D0, LOW);
//   digitalWrite(CONTROLLINO_D1, HIGH);

//   Serial.println("RECEIVED: /habitat/power/backup_power_available");
// }

// void RotationLeftRight() {
//   timeNow = millis();
//   WheelLeftRightNewPosition = WheelLeftRight.read();
//   DeltaWheelLeftRight = WheelLeftRightNewPosition - WheelLeftRightOldPosition;

//   if (timeNow - SensorTimestampTWO > 200) {
//     SensorTimestampTWO = timeNow;

//     if ((DeltaWheelLeftRight > 40 || DeltaWheelLeftRight < -40)  && LoginState == true) {
//       float DeltaFloat = DeltaWheelLeftRight / 4096.00;

//       OSCBundle bndl;
//       bndl.add("/habitat/power/crank_yaw").add(DeltaFloat);

//       Udp.beginPacket(outIp, destPort);
//       bndl.send(Udp);
//       Udp.endPacket();

//       if (DebugState == true) {
//         Udp.beginPacket(debugIp, destPort);
//         bndl.send(Udp);
//         Udp.endPacket();
//       }

//       bndl.empty();

//       Serial.print("SENT: /habitat/power/crank_yaw ");
//       Serial.println(DeltaFloat);

//       WheelLeftRightOldPosition = WheelLeftRightNewPosition;
//     }
//   }
// }

// void RotationUpDown() {
//   timeNow = millis();
//   WheelUpDown = analogRead(CONTROLLINO_A2);

//   // Serial.print(WheelUpDown);
//   // Serial.print(" ");
//   // Serial.print(OldWheelUpDown);
//   // Serial.print(" ");

//   DeltaWheelUpDown = WheelUpDown - OldWheelUpDown;

//   if (DeltaWheelUpDown < 20 && DeltaWheelUpDown > -20) {
//     THISDeltaWheelUpDown = DeltaWheelUpDown;
//   } else if (DeltaWheelUpDown > 80) {
//     DeltaWheelUpDown = THISDeltaWheelUpDown - 1;
//     OldWheelUpDown = WheelUpDown;
//   } else if (DeltaWheelUpDown < -80) {
//     DeltaWheelUpDown = THISDeltaWheelUpDown + 1;
//     OldWheelUpDown = WheelUpDown;
//   }

//   // Serial.println(DeltaWheelUpDown);

//   if (timeNow - SensorTimestamp > 200) {
//     SensorTimestamp = timeNow;

//     if ((DeltaWheelUpDown > 3 || DeltaWheelUpDown < -3)  && LoginState == true) {
//       float DeltaFloat = DeltaWheelUpDown / 135.00;

//       OSCBundle bndl;
//       bndl.add("/habitat/power/crank_pitch").add(DeltaFloat);

//       Udp.beginPacket(outIp, destPort);
//       bndl.send(Udp);
//       Udp.endPacket();

//       if (DebugState == true) {
//         Udp.beginPacket(debugIp, destPort);
//         bndl.send(Udp);
//         Udp.endPacket();
//       }

//       bndl.empty();

//       Serial.print("SENT: /habitat/power/crank_pitch ");
//       Serial.println(DeltaFloat);

//       OldWheelUpDown = WheelUpDown;
//     }
//   }
// }

// void ReadInputs() {
//   ReadButton();
//   RotationUpDown();
//   RotationLeftRight();
// }

// void ReadButton() {
//   ButtonReading = digitalRead(CONTROLLINO_A0);

//   if (ButtonReading != OldButtonState) {
//     lastDebounceTimeButton = millis();
//   }

//   if ((millis() - lastDebounceTimeButton) > debounceDelay) {
//     // whatever the reading is at, it's been there for longer than the debounce
//     // delay, so take it as the actual current state:

//     // if the button state has changed:
//     if (ButtonReading != ButtonState) {
//       ButtonState = ButtonReading;

//       if (ButtonState == HIGH) {
//         if (LoginState == true) {
//           ActivatePowerPlant();
//         }

//         if (BackupPowerAvailable == true && MeterSequenceStarted == false) {
//           MeterSequenceStarted = true;
//           MeterSequenceTimer = millis();
//           MeterONEValue = 0;
//           MeterTWOValue = 0;
//           MeterTHREEValue = 0;
//           MeterFOURValue = 0;
//         }
//       }

//       if (ButtonState == LOW) {

//       }
//     }
//   }

//   OldButtonState = ButtonReading;
// }

// void MeterSequence() {
//   int MeterSequenceTimerRaise = millis() - MeterSequenceTimer;

//   if (MeterSequenceTimerRaise < 2000) {
//     MeterONEValue = MeterSequenceTimerRaise / 2000.00;
//     analogWrite(CONTROLLINO_D5, MeterONEValue * 200);
//     // Serial.println(MeterONEValue);
//   }

//   if (MeterSequenceTimerRaise > 2000 && MeterSequenceTimerRaise < 4000) {
//     MeterTWOValue = (MeterSequenceTimerRaise - 2000) / 2000.00;
//     analogWrite(CONTROLLINO_D6, MeterTWOValue * 190);
//     // Serial.println(MeterTWOValue);
//   }

//   if (MeterSequenceTimerRaise > 4000 && MeterSequenceTimerRaise < 6000) {
//     MeterTHREEValue = (MeterSequenceTimerRaise - 4000) / 2000.00;
//     analogWrite(CONTROLLINO_D7, MeterTHREEValue * 210);
//     // Serial.println(MeterTHREEValue);
//   }

//   if (MeterSequenceTimerRaise > 6000 && MeterSequenceTimerRaise < 9000) {
//     MeterFOURValue = (MeterSequenceTimerRaise - 6000) / 3000.00;
//     analogWrite(CONTROLLINO_D8, MeterFOURValue * 200);
//     // Serial.println(MeterFOURValue);
//   }

//   if (MeterSequenceTimerRaise > 9000) {
//     MeterSequenceStarted = false;
//     digitalWrite(CONTROLLINO_D2, LOW);
//     digitalWrite(CONTROLLINO_D3, HIGH);
//     // Serial.println("OUT");
//   }

//   if (millis() - MeterSequenceSendTick > 200) {
//     MeterSequenceSendTick = millis();
//     SendPanelAlignment();
//   }
// }

// void SendPanelAlignment() {
//   OSCBundle bndl;

//   bndl.add("/habitat/power/panel_alignment").add(MeterONEValue).add(MeterTWOValue).add(MeterTHREEValue).add(MeterFOURValue);
//   Udp.beginPacket(outIp, destPort);
//   bndl.send(Udp);
//   Udp.endPacket();

//   if (DebugState == true) {
//     Udp.beginPacket(debugIp, destPort);
//     bndl.send(Udp);
//     Udp.endPacket();
//   }

//   bndl.empty();

//   Serial.print("SENT: /habitat/power/panel_alignment ");
//   Serial.print(MeterONEValue);
//   Serial.print(" ");
//   Serial.print(MeterTWOValue);
//   Serial.print(" ");
//   Serial.print(MeterTHREEValue);
//   Serial.print(" ");
//   Serial.println(MeterFOURValue);
// }

// void ActivatePowerPlant() {
//   OSCBundle bndl;

//   bndl.add("/habitat/power/activate_power_plant");
//   Udp.beginPacket(outIp, destPort);
//   bndl.send(Udp);
//   Udp.endPacket();

//   if (DebugState == true) {
//     Udp.beginPacket(debugIp, destPort);
//     bndl.send(Udp);
//     Udp.endPacket();
//   }

//   bndl.empty();

//   Serial.println("SENT: /habitat/power/activate_power_plant");
// }

/////////////////
///// MQTT /////
///////////////

void handle_received_message(char* topic, byte* payload, unsigned int length) {
  Serial.print("Exhibit /habitat/power: !!! Handle Message from topic [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void connect() {
  while (!mqtt_client.connected()) {
    Serial.print("Exhibit /habitat/power: Attempting MQTT connection...");
    String client_id = "cosa_client_habitat-";
    client_id += String(random(0xffff), HEX);

    if (mqtt_client.connect(client_id, mqtt_username, mqtt_password)) {
      Serial.println("Exhibit /habitat/power: connected");
      mqtt_client.subscribe(mqtt_topic);
    } else {
      Serial.print("Exhibit /habitat/power: failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

/*
  <pre><code>
  ----------begin_max5_patcher----------
  1675.3ocyassbihCD8YmuBJp8QuL5FRv719cL0VtjsURXFLPgwYxrSM+6qPR
  3fiAiLHHopD6XrfV5nS2pOsT98Cq72l+p3nu2W89l2pU+9gUqTWp9BqLedk+
  A9q6R4GUMyeW9gChrJ+05uqR7Zk55eYWIO6Ga9E+mMeUQo3nrk7pj7rMkhcU
  ZyDAPArv0dgzv.vZOHEV+FBD.79Wyclc5PRVpnRYQXGOt1WW137SUMsFzzZd
  0tmSxdpkgiIQA01kzqceLOq5Q9NQ6Gexd0vKe62+aBxu9Z+4gGpeYsa.rhDY
  Gc.HihQAD..fHDHAPQeDnGivTnm1fiA8fNE8J3YhzM7zjmxZ2fdPPL.2lzgf
  zfZTbAvMJRgTL8aHH3twMfSwsihxWDkeYuX6om1j+3iCAbLlB3PTxxx2fPng
  gOR9FdLv11SUUxNrcXBLV0CorlWcAj.GFRHJHAQZd8rY0OlpeUHz2f+Vd1S9
  m+5VfCBedLxK4GDUhxMhL91T0cBFAxcPb7H+Ig8DtKvHzXwBnFLfxEUpQCzs
  PitQB1L5dYMSJDGndKZoVKLDMImKT7r6cEQpgjEy4xfHNv2Bi9f7sxbiqUC2
  XpdV3nwvQdLMW1SsMGRrh4Fiqes9ucAIAcCnAEeUhX.phmhtpKbMJs9VQjo9
  sb8Nl7ep6p1m7Vjoy2R4AtpORmKHuqbP0neTzBg9LFY1PexmazmvT4MFRTHd
  H3JWx4AwoDEvRo5b+AVDI3VnLLx+SBbBI53GeP3IJxM3I6SBdhCiU5o9fvyv
  nX2fmzOK3Yib0OH7TlAfSvS77imYheJM0UoFcJqfu6Gdffy+3eKj6RDhLLBo
  W2ABfCCQxYb4vac++QmPmSUk7RdZUqDH6g00w5qMx9wQKaYlfLsfnQnLYTEo
  6XZxdQ48iOZQJXkiBjglcUJF3wnRIrI426LybXXyHsISG34DcTTR4io3T0k8
  E2HkwJIfgrXUhkKqBvPPjiT.BAtVAXud1akQ4NUroH+mRavegmjpLiU3aSUG
  zH7B3baP3w6biGUYG5U97y7sIxgyWTnWSPRu+B5FczMgxL5nI3wpiFLiC5ay
  elF.zLca7nfwisDcg9NLwf8ERR7dOXLJ.RiBPAHhbYNH.byDDpIlYRWY0y3e
  JS3o92K6WolzCiz9dlMhnMZzNBBrK8nPYqcYviKnBCEyP2sMygpHFX7PQL5G
  0lbrDrYmTh0cEMU6hdzPIJ.5TxOIX1VVSGuiDSWzcMntzAuUoDrh1U+5XprI
  bwVWiuqJ4EdkvDYpHkO3d7og2lMFkPVnE0ZvWslIHlc2KpwbX3s8hT9ujtC8
  DN692mkHZK8NlLEPii8PmicZIM+IYWXirYEx9gvpnXX8T1xsYlXX7z1KyvYB
  5Rx1v2sSTTI1aExcdafQK01.GRaibn6F4HyAxUH6j1sdYSJ13EKE62fqn36F
  svtLUyZTxS75yIxrL7dLuzKovKHHvsAlLgcgiM6ZTzz2ISKJozviDhIivII7
  Ldo15wAla6wo.hQuIrGZ3mj9bJPN6fSnQXptZcMJwH83W.tuZoz4NGAGWZj8
  rndorqH7dmxshb4Lv6tVWYMc08c4Am58ecqSkV2eUqS32EyVzNmUnClAOAqX
  7rXrcUa9pe6bVA4VIixdsH4Egb8Ov.BEslTZF93Hlda1IiL.17cRLLIKI8ZV
  xxvYxUxAkgiszmCidxEcZKz0j6noLRX1H4IQyLOYwOMbF4zN3vv8AvS5Hw6I
  lOj4f4ZnInwVrM5LSSV7h56HRRjyq8QOq1TTl7lxCKVh4VklPGrfnWaAitUE
  GC8mAhdacRSsVxsX2L3HI2j4uPxXLYtKjrYe4iPVTHYjMERVci9oIYu+eEDk
  wqu9kvyw7Sk6Z.ZS5VRZ1Y6uWbrJI6rS62d6Dt4I6jcNEbWVBOjkHtxRnArD
  zUFBNfgvNyRfgFRwWzn7x5MFulyr.llbARb1zfIY5XarLZVFzVvUqOSbSeh0
  FpJyQFZHpJj5HCM3jF1AFJxFK4h3ILKriK3BXpEFB4BK07PrxmdZVxBC4DVm
  MwJvtvR1XnvtiIMsngDalybwhMXfM7PWXIT3RYonkxPzkxPPqlkBcgkrYVpg
  zLMlmMgKvNIBH1FzyEK7hWLpGhMNKoEQvKJjB+NZZsxHR0TeOWE4JZs5iIY5
  OpjD4WJdIoo8X0U3kREOUR4NmJ0xjdkp0T4eHWFCL6ThI0L4vSZRkRsZIUGK
  LUvWIn6g+7v+Cw4SvIA
  -----------end_max5_patcher-----------
  </code></pre>
*/