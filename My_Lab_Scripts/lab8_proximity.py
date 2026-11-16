import serial
import time
import paho.mqtt.publish as publish

channel_ID = "3520288"
mqtt_host  = "mqtt3.thingspeak.com"
mqtt_client_ID = "LAIyFCQEOjoLEigXAS0EMjM"
mqtt_username  = "LAIyFCQEOjoLEigXAS0EMjM"
mqtt_password  = "itEEIw2lk2UEz6sgPURuVfq1"

t_transport = "websockets"
t_port = 80
topic = f"channels/{channel_ID}/publish"

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=2)
print("Connected to Arduino Proximity Sensor!")
print("Wave your hand over the sensor!\n")

last_send_time = 0

while True:
    if ser.in_waiting > 0:
        raw_line = ser.readline().decode('utf-8', errors='ignore').strip()
        if raw_line:
            try:
                hand_val = int(raw_line)
                print(f"Hand Proximity: {hand_val}")
                
                # Publish to Cloud every 15 seconds
                current_time = time.time()
                if current_time - last_send_time >= 15:
                    payload = f"field1={hand_val}"
                    print(f"--> Publishing Live Hand Score: {payload} to ThingSpeak...")
                    
                    publish.single(
                        topic,
                        payload=payload,
                        hostname=mqtt_host,
                        transport=t_transport,
                        port=t_port,
                        client_id=mqtt_client_ID,
                        auth={'username': mqtt_username, 'password': mqtt_password}
                    )
                    print("   -> Plotted on ThingSpeak Graph!\n")
                    last_send_time = current_time
            except ValueError:
                pass
            except Exception as e:
                print(f"Publish error: {e}")
    time.sleep(0.1)
