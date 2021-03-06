#ifndef LAYER1_H
#define LAYER1_H

#include <unistd.h>
#include <stdint.h>

#define STDIN 0
#define STDOUT 1
#define DEBUG 0
#define SHA1_LENGTH 40

#define LORA // to use Layer1_LoRa.cpp
//#define SIM // to use Layer1_Sim.cpp

#ifdef LORA
#include <Arduino.h>
#include <SPI.h>

#define L1_DEFAULT_CS_PIN        18
#define L1_DEFAULT_RESET_PIN     23
#define L1_DEFAULT_DIO0_PIN      26

class Layer1Class {
public:
    Layer1Class();

    int debug_printf(const char* format, ...);

    // Public access to local variables
    int getTime();
    int loraInitialized();
    int loraCSPin();
    int resetPin();
    int DIOPin();

    // User configurable settings
    void setPins(int cs, int reset, int dio);
    void setSPIFrequency(uint32_t frequency);
    void setLoRaFrequency(uint32_t frequency);
    void setSpreadingFactor(uint8_t spreadingFactor);
    void setTxPower(int txPower);

    // Main public functions
    int init();
    int transmit();

private:
    // Main private functions
    static void onReceive(int packetSize);
    int sendPacket(char* data, int len);

    // Local variables
    int _loraInitialized;
    int _csPin;
    int _resetPin;
    int _DIOPin;
    uint32_t _spiFrequency;
    uint32_t _loraFrequency;
    uint8_t _spreadingFactor;
    int _txPower;
};

extern Layer1Class Layer1;

#endif

#ifdef SIM
typedef struct _serial {
  int (*printf)(const char*, ...);
} serial;

extern serial Serial;

// you must declare these in your router
int setup(); // called once on startup
int loop(); // called once per event loop iteration

class Layer1Class {
public:
    Layer1Class();
    int nsleep(unsigned int secs, useconds_t usecs);
    int simulationTime(int realTime);
    int setTimeDistortion(float newDistortion);
    int getTime();
    int debug_printf(const char* format, ...);
    //int print_err(const char* format, ...);
    int setLocalAddress(char* macString);
    uint8_t* localAddress();
    int setNodeID(char* newID);
    //char* nodeID();
    int parse_metadata(char* data, uint8_t len);
    int begin_packet();
    int send_packet(char* data, uint8_t len);

private:
    uint8_t hex_digit(char ch);
    int isHashNew(uint8_t hash[SHA1_LENGTH]);
    float timeDistortion();

private:
    int _transmitting;
    char* _nodeID;
    uint8_t _hashTable[256][SHA1_LENGTH];
    uint8_t _hashEntry;
    float _timeDistortion;

};

extern Layer1Class Layer1;
#endif
#endif
