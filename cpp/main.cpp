#include "cpp.h"

int main(int argc , char ** argv )
{
	// Check the number of parameters
    if (argc != 4) {
        fmt::print("usage: app <broker_ip> <subscribe_to>  <publish_to>!\n");
        return 1;
    }
	class mqtt_tempconv *tempconv;
	
	mosqpp::lib_init();

	tempconv = new mqtt_tempconv(argv[2], argv[1], 1883, argv[3] );
	tempconv->username_pw_set("username", "password");
	tempconv->loop_forever();
	
	mosqpp::lib_cleanup();

	return 0;
}

