#ifndef H_WIFI
#define H_WIFI

#include <memory>
#include <cstring>

#include "dynamicstring.h"

std::unique_ptr<char[]> wifi_connect_cmd(const char* ssid, const char* password);

enum class AtStatus {
    WAITING,
    OK,
    FAIL,
    ERROR,
};

class AtStatusListener {
    public:
        void receive_char(const char c);

        AtStatus current_status() const;

        const char * const current_reply() const;
    private:
        void update_status();

        DynamicString reply;

        AtStatus status = AtStatus::WAITING;
};

#endif
