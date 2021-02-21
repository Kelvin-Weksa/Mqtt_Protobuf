import paho.mqtt.client as mqtt
import sys
from time import sleep
sys.path.insert(0, '../build/messages')
import addressbook_pb2

client = mqtt.Client()
client.username_pw_set(username="username",password="password")

if len(sys.argv) != 4:
	print ("""

		usage: app <broker_ip> <subscribe_to>  <publish_to>!
		""")
	sys.exit(0)

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
	print("Connected with result code "+str(rc))

	# Subscribing in on_connect() means that if we lose the connection and
	# reconnect then subscriptions will be renewed.
	client.subscribe(sys.argv[2])

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
	protocol = addressbook_pb2.Protobuf()
	if msg.topic == sys.argv[2]:
		sleep(0.5)
		protocol.ParseFromString ( msg.payload )
		former = protocol.temperature
		protocol.temperature = ( protocol.temperature + 1)
		print("{}  payload: {}  after: {}".format(msg.topic,former,protocol.temperature))
		client.publish(sys.argv[3], protocol.SerializeToString())

client.on_connect = on_connect
client.on_message = on_message

client.connect(sys.argv[1], 1883, 60) 
# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()