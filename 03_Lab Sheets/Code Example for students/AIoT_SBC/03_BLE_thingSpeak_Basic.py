#! /usr/bin/python3
#import time library
import time
import asyncio
from bleak import BleakClient

# Import important librarys for web service
import paho.mqtt.publish as publish
import psutil
import string

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

#address = input("Device Address (e.g. 12:34:56:78:9A:BC): ")
address = '1F:1A:D1:87:80:FA'    #prototype


TEMPERATURE_UUID = "7299a27d-8110-4eb7-b67b-ad03cec64c02"

async def run(address):
  async with BleakClient(address) as client:
    global index
    temperature = await client.read_gatt_char(TEMPERATURE_UUID)
    index = int.from_bytes(temperature,'little')
    #print("Temperature: {0}C".format(int.from_bytes(temperature, byteorder='little', signed=True)/100))
      
loop = asyncio.get_event_loop()

while True:
  #loop = asyncio.get_event_loop()
  loop.run_until_complete(run(address))
  
  #Index = 211.11 #value_int is the number to be uploaded
  # build the payload string.
  my_float = float(index)
  payload = "field1=" + str(my_float)

  # attempt to publish this data to the topic.
  try:
    print ("Writing Payload = ", payload," to host: ", mqtt_host, " clientID= ", mqtt_client_ID, " User ", mqtt_username, " PWD ", mqtt_password)
    publish.single(topic, payload, hostname=mqtt_host, transport=t_transport, port=t_port, client_id=mqtt_client_ID, auth={'username':mqtt_username,'password':mqtt_password})
  except Exception as e:
    print(e)
  time.sleep(5)
ser.close()#end of while loop, close the serial interface
