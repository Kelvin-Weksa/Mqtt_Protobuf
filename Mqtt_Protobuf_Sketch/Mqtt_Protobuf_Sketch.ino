#include "pb_common.h"
#include "pb.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "addressbook.pb.h"


#if defined(ESP8266)
  #include <ESP8266WiFi.h> // Enables the ESP8266 to connect to the local network (via WiFi)
#endif


#include <PubSubClient.h> // Allows us to connect to, and publish to the MQTT broker

const int ledPin = LED_BUILTIN; // This code uses the built-in led for visual feedback that a message has been received

// WiFi
// Make sure to update this for your own WiFi network!
const char* ssid = "wwddOhYeah!";
const char* wifi_password = "wawadudu";

// MQTT
// Make sure to update this for your own MQTT Broker!
const char* mqtt_server = "192.168.1.9";
const char* mqtt_username = "username";
const char* mqtt_password = "password";
const char* mqtt_topic = "ESP8266_1";

const char* publish_to = "rasp3b";
 

// Initialise the WiFi and MQTT Client objects
WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient); // 1883 is the listener port for the Broker


void ReceivedMessage(char* topic, byte* payload, unsigned int length) {
  digitalWrite(ledPin, HIGH);
  // Output the first character of the message to serial (debug)
  bool status = false;
  /* Allocate space on the stack to store the message data.
   *
   * Nanopb generates simple struct definitions for all the messages.
   * - check out the contents of simple.pb.h!
   * It is a good idea to always initialize your structures
   * so that you do not have garbage data from RAM in there.
   */
  tutorial_Protobuf protobuf = tutorial_Protobuf_init_zero;
  /* But because we are lazy, we will just decode it immediately. */
    {
                
        /* Create a stream that reads from the payload. */
        pb_istream_t stream = pb_istream_from_buffer(payload, length);
        
        /* Now we are ready to decode the message. */
        status = pb_decode(&stream, tutorial_Protobuf_fields, &protobuf);
        
        /* Check for errors... */
        if (!status)
        {
            Serial.print("Decoding failed:");
            Serial.print (  PB_GET_ERROR(&stream) );
            Serial.print("\n");
            return ;
        }
        
        /* Print the data contained in the message. */
        Serial.print("decoding succeded ");
        Serial.print(protobuf.temperature);
        Serial.print("\n");
    }
  
  protobuf.temperature += 1;
  uint8_t buffer[128];
  size_t message_length = 0;
  /* Encode our message */
    {
               
        /* Create a stream that will write to our buffer. */
        pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
        
                
        /* Now we are ready to encode the message! */
        status = pb_encode(&stream, tutorial_Protobuf_fields, &protobuf);
        message_length = stream.bytes_written;
        
        /* Then just check for any errors.. */
        if (!status)
        {
            Serial.print("Encoding failed:");
            Serial.print(PB_GET_ERROR(&stream));
            return ;
        }
    }
  /*publish*/
    {
      if (client.publish(publish_to, buffer, message_length)) {
         Serial.println("published!");
      }
      // Again, client.publish will return a boolean value depending on whether it succeded or not.
      // If the message failed to send, we will try again, as the connection may have broken.
      else {
        Serial.println("Message failed to send. Reconnecting to MQTT Broker and trying again");
        client.connect(mqtt_topic, mqtt_username, mqtt_password);
        delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
        client.publish(publish_to, buffer , message_length);
      }
    }
  
  digitalWrite(ledPin, LOW);
}

bool Connect() {
  // Connect to MQTT Server and subscribe to the topic
  if (client.connect(mqtt_topic, mqtt_username, mqtt_password)) {
      client.subscribe(mqtt_topic);
      return true;
    }
    else {
      return false;
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  // Switch the on-board LED off to start with
  digitalWrite(ledPin, HIGH);

	Serial.println("");
  Serial.println("STARTING");
  Serial.println("Connecting to ");
  Serial.println(ssid);
  Serial.println("with password: ");
  Serial.println(wifi_password);

  // Connect to the WiFi
  WiFi.begin(ssid, wifi_password);

  // Wait until the connection has been confirmed before continuing
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Debugging - Output the IP Address of the ESP8266
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Connect to MQTT Broker
  // setCallback sets the function to be called when a message is received.
  client.setCallback(ReceivedMessage);
  if (Connect()) {
    Serial.println("Connected Successfully to MQTT Broker!");  
  }
  else {
    Serial.println("Connection Failed!");
  }
}
 
void loop() {
  // If the connection is lost, try to connect again
  if (!client.connected()) {
    Connect();
  }
  // client.loop() just tells the MQTT client code to do what it needs to do itself 
  // (i.e. check for messages, etc.)
  client.loop();
  // Once it has done all it needs to do for this cycle, 
  // go back to checking if we are still connected.
}

// run this command in the build folder
// ./../arduino-cli compile --fqbn esp8266:esp8266:generic  ../Mqtt_Protobuf_Sketch/
// ./../arduino-cli upload -p /dev/ttyUSB0 --fqbn esp8266:esp8266:generic ../Mqtt_Protobuf_Sketch/ && stty -F /dev/ttyUSB0 raw 115200 && cat /dev/ttyUSB0
//  stty -F /dev/ttyUSB0 raw 115200
//  cat /dev/ttyUSB0
//  docker run -it --device=/dev/ttyUSB0 --device=/dev/ttyACM0 --net=host kelvinwekesa/debian-mosquitto:version2
// 
// 
// git add .
// git commit -m "comment"
// git push origin master
// 
// 
// run these commands from the build folder
// 
// service mosquitto status
// 
// 
// ../python/paho_mqtt.py localhost pycode thinkbig
// ./temperature_conversion/temperature_conversion  127.0.0.1 thinkbig rasp3b
// ./temperature_conversion/temperature_conversion  127.0.0.1 rasp3b thinkbig
// mosquitto_pub -d -u username -P password -t thinkbig -m "@@"
// 
// 
// apt install git cmake make g++ autoconf automake libtool curl unzip python 
// python-dev python-pip  python3 python3-dev python3-pip 
// mosquitto mosquitto-dev libmosquittopp-dev arduino
// 
// pip install python3-protobuf protobuf paho-mqtt
// 
// git clone https://github.com/protocolbuffers/protobuf.git
// 
// cd protobuf
// git submodule update --init --recursive
// ./autogen.sh
// 
// ./configure --prefix=/usr
// make
// make check
// make install
// ldconfig # refresh shared library cache.
// 
// 
// 