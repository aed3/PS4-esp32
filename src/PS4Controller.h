#ifndef PS4Controller_h
#define PS4Controller_h

#include <inttypes.h>

#include "Arduino.h"

extern "C" {
#include  "include/ps4.h"
}


class PS4Controller
{
    public:
        typedef void(*callback_t)();

        ps4_t data;
        ps4_event_t event;
        ps4_cmd_t output;

        PS4Controller();

        bool begin();
        bool begin(char *mac);
        bool end();

        bool isConnected();

        void setLed(int r, int g, int b);
        void setRumble(int small, int large);
        void setFlashRate(int onTime, int offTime);

        void sendToController();

        void attach(callback_t callback);
        void attachOnConnect(callback_t callback);
        void attachOnDisconnect(callback_t callback);

    private:
        static void _event_callback(void *object, ps4_t data, ps4_event_t event);
        static void _connection_callback(void *object, uint8_t is_connected);

        callback_t _callback_event = nullptr;
        callback_t _callback_connect = nullptr;
        callback_t _callback_disconnect = nullptr;

};

#if !defined(NO_GLOBAL_INSTANCES)
extern PS4Controller PS4;
#endif

#endif
