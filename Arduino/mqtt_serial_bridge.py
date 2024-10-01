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
connected = False
ser = serial.Serial('COM4', 9600)
time.sleep(2)

# Connect callback
def on_connect(client, userdata, flags, rc):
    global connected
    if rc == 0:
        print("Connected to MQTT broker")
        ser.write(b'MQTT_CONNECTED\n')
        client.subscribe(MQTT_TOPIC)
        connected = True
    else:
        print(f"Failed to connect, return code {rc}")
        ser.write(b'MQTT_FAILED\n')

# Receive callback
def on_message(client, userdata, msg):
    print(f"Message received from {msg.topic}: {msg.payload.decode()}")

def connect_to_mqtt():
    client = mqtt.Client()
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
    if not connected:
        if ser.in_waiting > 0:
            message = ser.readline().decode('utf-8').strip()
            
            if message == "CONNECT_MQTT":
                print("Attempting to connect to MQTT server...")
                success = connect_to_mqtt()
                
        time.sleep(1)
