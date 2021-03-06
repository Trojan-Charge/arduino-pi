

"Run vm_subscriber.py in a separate terminal on your VM."""

import paho.mqtt.client as mqtt
import time

#Ultrasonic ranger callback - receives and prints ranger output
def servo_callback(client, userdata, message):
    print("servo moves" + message.payload.decode("utf-8","strict"))

#Button callback - prints "Button pressed!" if button is pressed
def button_callback(client, userdata, message):
    print(message.payload.decode("utf-8", "strict"))

def on_connect(client, userdata, flags, rc):
    print("Connected to server (i.e., broker) with result code "+str(rc))

    #subscribe to the servo topic here
    client.subscribe("trojancharge/servo")
     #specify what to do when messages are received on this topic
    client.message_callback_add("rachelan/servo", servo_callback)

    
#Default message callback. Please use custom callbacks.
def on_message(client, userdata, msg):
    print("on_message: " + msg.topic + " " + str(msg.payload, "utf-8"))

if __name__ == '__main__':
    #this section is covered in publisher_and_subscriber_example.py
    client = mqtt.Client()
    client.on_message = on_message
    client.on_connect = on_connect
    client.connect(host="eclipse.usc.edu", port=11000, keepalive=60)
    client.loop_start()

    while True:
        time.sleep(1)
