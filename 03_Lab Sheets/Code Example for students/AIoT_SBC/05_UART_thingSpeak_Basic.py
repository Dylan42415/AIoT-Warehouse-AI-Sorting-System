#! /usr/bin/python3
#import time library
import time
import serial
import time
# Import important librarys for web service
import paho.mqtt.publish as publish
import psutil
import string

serial1 = serial.Serial('/dev/ttyACM1',9600)

# The ThingSpeak Channel ID.
# Replace <YOUR-CHANNEL-ID> with your channel ID.
channel_ID = "1927000"
# The hostname of the ThingSpeak MQTT broker.
mqtt_host = "mqtt3.thingspeak.com"
# Your MQTT credentials for the device
mqtt_client_ID = "BhwPBBs3KhQsPCcRMDk0JT0"
mqtt_username  = "BhwPBBs3KhQsPCcRMDk0JT0"
mqtt_password  = "JRWLSjth5bcthriA3lp8cvZN"
t_transport = "websockets"
t_port = 80
# Create the topic string.
topic = "channels/" + channel_ID + "/publish"
index = 1
my_float = 0.23

while True:
  dataString = serial1.readline()
  person_score = (int)(dataString)
  # check if person_score is more than 50 then send to cloud
  if(person_score > 50):
    # build the payload string.
    payload = "field1=" + str(person_score)
  
    # attempt to publish this data to the topic.
    try:
      print ("Writing Payload = ", payload," to host: ", mqtt_host, " clientID= ", mqtt_client_ID, " User ", mqtt_username, " PWD ", mqtt_password)
      publish.single(topic, payload, hostname=mqtt_host, transport=t_transport, port=t_port, \
      client_id=mqtt_client_ID, auth={'username':mqtt_username,'password':mqtt_password})
    except Exception as e:
      print(e)
    time.sleep(15)
ser.close()#end of while loop, close the serial interface
