#pragma once

#include "wifiModule.hpp"
#include "espNowConfigurations.hpp"

class EspNowModule{
    public:

    void init();
    void deinit();
    void addPeer();
    void deletePeer();
    void sendData();
    void setRole();
    private:
};

