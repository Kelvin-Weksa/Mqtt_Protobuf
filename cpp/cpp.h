#ifndef TEMPERATURE_CONVERSION_H
#define TEMPERATURE_CONVERSION_H
#include <chrono>
#include <thread>
#include <string>

#include <fmt/color.h>
#include <fmt/core.h>

#include <mosquittopp.h>

#include <google/protobuf/util/time_util.h>

#include "addressbook.pb.h"

using google::protobuf::util::TimeUtil;

class mqtt_tempconv : public mosqpp::mosquittopp
{	std::string pub_topic;
	std::string sub_topic;
	public:
		mqtt_tempconv(const char *id, const char *host, int port, std::string);
		~mqtt_tempconv();

		void on_connect(int rc);
		void on_message(const struct mosquitto_message *message);
		void on_subscribe(int mid, int qos_count, const int *granted_qos);
};

#endif
