#include <cstdio>
#include <cstring>

#include "cpp.h"
#include <mosquittopp.h>



mqtt_tempconv::mqtt_tempconv(const char *id, const char *host, int port, std::string pubTopic) 
	: mosquittopp(id), pub_topic{pubTopic}, sub_topic{id}
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	int keepalive = 60;

	/* Connect immediately. This could also be done by calling
	 * mqtt_tempconv->connect(). */
	connect(host, port, keepalive);
};

mqtt_tempconv::~mqtt_tempconv()
{
	// Optional:  Delete all global objects allocated by libprotobuf.
  	google::protobuf::ShutdownProtobufLibrary();
}

void mqtt_tempconv::on_connect(int rc)
{
	printf("Connected with code %d.\n", rc);
	if(rc == 0){
		/* Only attempt to subscribe on a successful connect. */
		subscribe(NULL, sub_topic.c_str());
	}
}

void mqtt_tempconv::on_message(const struct mosquitto_message *message)
{
	tutorial::Protobuf protocol;
	if(!strcmp(message->topic, sub_topic.c_str())){
		using namespace std::chrono_literals;
		/* Prevent a storm of messages  */
		std::this_thread::sleep_for(500ms); 
		
		if (!protocol.ParseFromArray( message->payload,  message->payloadlen) ) {
      		std::cerr << "Failed to parse." << std::endl;
      		return ;
    	}
    	auto former = protocol.temperature();
		protocol.set_temperature( protocol.temperature() + 1);

		fmt::print( "{}  payload: {}  after: {}\n", 
			format(fg(fmt::color::floral_white) | bg(fmt::color::slate_gray) 
				| fmt::emphasis::underline, sub_topic), 
			format(fg(fmt::color::crimson), std::to_string(former)),
			format(fg(fmt::color::green_yellow) | fmt::emphasis::bold, std::to_string( protocol.temperature() ) ) );
		
		std::string buffer;
		if (!protocol.SerializeToString(&buffer)) {
      		std::cerr << "Failed to SerializeToString." << std::endl;
      		return ;
    	}    	
		publish(NULL, pub_topic.c_str(), protocol.ByteSizeLong(), buffer.c_str() );
	}
}

void mqtt_tempconv::on_subscribe(int , int , const int *)
{
	fmt::print("Subscription succeeded.\n");
}



