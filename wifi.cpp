#include "wifi.h"

////////////////////////////////////////////////////////////////////////////////
// Standalone functions
////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<char[]> wifi_connect_cmd(const char* ssid, const char* password)
{
    auto connect_command = "AT+CWJAP=";
    const std::size_t ADDITIONAL_CHARACTERS = 5; // One comma, 4 "

    std::size_t buffer_size = strlen(connect_command)
        + strlen(ssid)
        + strlen(password) 
        + ADDITIONAL_CHARACTERS
        + 1; // Because the NULL terminator needs to be stored

    char* buffer = new char[buffer_size];

    snprintf(buffer, buffer_size, "%s\"%s\",\"%s\"", connect_command, ssid, password);

    return std::unique_ptr<char[]>(buffer);
}



////////////////////////////////////////////////////////////////////////////////
// ATListener definition
////////////////////////////////////////////////////////////////////////////////

void AtStatusListener::receive_char(char c) {
    if(status == AtStatus::WAITING) {
        this->reply.add_char(c);

        update_status();
    }
}

AtStatus AtStatusListener::current_status() const {
    return this->status;
}


void AtStatusListener::update_status() {
    auto reply_size = this->reply.size();
    if(reply_size >= 3) {

        if(
                reply.get()[reply_size-3] == '\r' &&
                reply.get()[reply_size-2] == 'O' &&
                reply.get()[reply_size-1] == 'K'
            ) {
            this->status = AtStatus::OK;
        }
    }
    if(reply_size >= 5) {
        if(
                reply.get()[reply_size-5] == '\r' &&
                reply.get()[reply_size-4] == 'F' &&
                reply.get()[reply_size-3] == 'A' &&
                reply.get()[reply_size-2] == 'I' &&
                reply.get()[reply_size-1] == 'L'
            ) {
            this->status = AtStatus::FAIL;
        }
    }
    if(reply_size >= 6) {
        if(
                reply.get()[reply_size-6] == '\r' &&
                reply.get()[reply_size-5] == 'E' &&
                reply.get()[reply_size-4] == 'R' &&
                reply.get()[reply_size-3] == 'R' &&
                reply.get()[reply_size-2] == 'O' &&
                reply.get()[reply_size-1] == 'R'
            ) {
            this->status = AtStatus::ERROR;
        }
    }
}


const char * const AtStatusListener::current_reply() const {
    return this->reply.get();
}
