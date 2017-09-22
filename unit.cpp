#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "wifi.h"
#include "dynamicstring.h"

TEST_CASE( "Wifi connect command is created correctly", "[wifi]" ) {
    auto wifi_command = wifi_connect_cmd("user", "password");

    auto command_str = std::string(wifi_command.get());

    REQUIRE(command_str == "AT+CWJAP=\"user\",\"password\"");
}


TEST_CASE("Basic string usage", "[dynamicstring]") {
    DynamicString string("yoloswag");

    auto cppstring = std::string(string.get());
    REQUIRE(cppstring == "yoloswag");

    string.add_char('1');

    cppstring = std::string(string.get());
    REQUIRE(cppstring == "yoloswag1");
}

TEST_CASE("String char addition", "[dynamicstring]") {
    DynamicString string;

    std::string message = "WIFI CONNECTED\n\rWIFI GOT IP\n\rOK\n\r";

    for(auto c : message) {
        string.add_char(c);
    }

    auto cppstring = std::string(string.get());
    REQUIRE(cppstring == message);
}

TEST_CASE("Shadowing strings does not cause problems", "[dynamicstring]") {
    DynamicString string;

    string = DynamicString();

    REQUIRE(string.get()[0] == '\0');
}



TEST_CASE("ATStatus can parse replies", "[atstatus]") {
    AtStatusListener sl;

    std::string message = "WIFI CONNECTED\n\rWIFI GOT IP\n\rOK\n\r";

    for(auto c : message) {
        sl.receive_char(c);
    }
    REQUIRE(sl.current_status() == AtStatus::OK);

    message = "\n\rERROR\n\r";

    sl = AtStatusListener();
    for(auto c : message) {
        sl.receive_char(c);
    }
    REQUIRE(sl.current_status() == AtStatus::ERROR);

    message = "busy p...\n\rWIFI DISCONNECT\n\rAT+\n\rbusy p...\n\rAT\n\rbusy p...\n\r+CWJAP:1\n\r \n\rFAIL\n\r";

    sl = AtStatusListener();
    for(auto c : message) {
        sl.receive_char(c);
    }
    REQUIRE(sl.current_status() == AtStatus::FAIL);
}
