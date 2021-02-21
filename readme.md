# MQTT + PROTOCOL BUFFERS + ESP8266 + LINUX DOCKER

## starting from a fresh debian/ubuntu/raspbian container

begin by installing these packages as :
```
apt update

apt upgrade

apt install git cmake make g++ autoconf automake libtool curl unzip python  python-dev python-pip  python3 python3-dev python3-pip mosquitto mosquitto-dev libmosquittopp-dev arduino
```

install [protocol buffers](https://github.com/protocolbuffers/protobuf/blob/master/src/README.md) from source
```
git clone https://github.com/protocolbuffers/protobuf.git
// 
cd protobuf
git submodule update --init --recursive
./autogen.sh
 
./configure --prefix=/usr
make
make check
make install
ldconfig # refresh shared library cache.
```

then within python install:

```
pip install python3-protobuf protobuf paho-mqtt
pip3 install python3-protobuf protobuf paho-mqtt
```


## Now pull the Mqtt_Protobuf demo and build

```
cd /
git clone https://github.com/Kelvin-Weksa/Mqtt_Protobuf.git
cd Mqtt_Protobuf
git submodule init
git submodule update
mkdir build #the build folder MUST be named 'build'
cd build/
cmake ..
make
```

###### from the witin `build/` folder open terminal and access the C++ app as
```
./cpp/cpp broker_ip subscribe_to publish_to
```

###### from the witin `build/` folder open terminal and access the python3 app as
```
python3 ../python/paho_mqtt.py broker_ip subscribe_to publish_to
```

###### from the witin `build/` folder open terminal and compile the esp8266 app as
```
./../arduino-cli compile --fqbn esp8266:esp8266:generic  ../Mqtt_Protobuf_Sketch/
```

###### from the witin `build/` folder open terminal and upload/monitor the esp8266 app as
```
./../arduino-cli upload -p /dev/ttyUSB0 --fqbn esp8266:esp8266:generic ../Mqtt_Protobuf_Sketch/ && stty -F /dev/ttyUSB0 raw 115200 && cat /dev/ttyUSB0
```
###### of course assuming the esp8266 is located at: `/dev/ttyUSB0`
###### change as appropriate see [arduino-cli](https://arduino.github.io/arduino-cli/latest/getting-started/)


###### how to run the docker so as to have access to host network and chosen usb devices
```
docker run -it --device=/dev/ttyUSB0 --device=/dev/ttyACM0 --net=host kelvinwekesa/debian-mosquitto:final
```

