import serial
import time
import paho.mqtt.client as mqtt

# MQTT credentials
MQTT_SERVER = "dmt.fh-joanneum.at"
MQTT_PORT = 1883
MQTT_USERNAME = "dmt"
MQTT_PASSWORD = "ss2021"
MQTT_TOPIC = "Arduino-Unity-MQTT-Test"

# Arduino Initialization
ser = serial.Serial('COM4', 9600)
time.sleep(2)

# MQTT client initialization (global for publishing later)
client = mqtt.Client()

# Connect callback
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT broker")
        ser.write(b'MQTT_CONNECTED\n')
        client.subscribe(MQTT_TOPIC)
    else:
        print(f"Failed to connect, return code {rc}")
        ser.write(b'MQTT_FAILED\n')

# Receive callback
def on_message(client, userdata, msg):
    payload = msg.payload.decode()
    print(f"Message received from {msg.topic}: {payload}")
    if payload == "GREEN":
        print("Perform Green Action")
        ser.write(b'MQTT_GREEN\n')
    if payload == "RED": 
        print("Perform Red Action")
        ser.write(b'MQTT_RED\n')

def connect_to_mqtt():
    client.username_pw_set(MQTT_USERNAME, MQTT_PASSWORD)
    client.on_connect = on_connect
    client.on_message = on_message
    try:
        client.connect(MQTT_SERVER, MQTT_PORT, 60)
        client.loop_start()
        return True
    except Exception as e:
        print(f"Connection failed: {e}")
        ser.write(b'MQTT_FAILED\n')
        return False

# Serial communication loop
while True:
    if ser.in_waiting > 0:
        message = ser.readline().decode('utf-8').strip()
        
        if message == "CONNECT_MQTT":
            print("Attempting to connect to MQTT server...")
            success = connect_to_mqtt()
        
        if message.startswith("PUBLISH_MQTT"):
            try:
                payload = message.split(":")[1]
                print(f"Publishing to MQTT: {payload}")
                client.publish(MQTT_TOPIC, payload)
                ser.write(b'MQTT_PUBLISHED\n')
            except IndexError:
                print("No payload to publish. Expected format: PUBLISH_MQTT:payload")
                ser.write(b'MQTT_PUBLISH_FAILED\n')
            
    time.sleep(1)
