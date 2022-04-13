#ifndef FINGER_ESP_FINGERSTATE_H
#define FINGER_ESP_FINGERSTATE_H

//#define LARGE_JSON_BUFFERS 1

#define SMALL_JSON_DOCUMENT_SIZE 4096
#define LARGE_JSON_DOCUMENT_SIZE 4096

#include "Thing.h"
#include "WebThingAdapter.h"
#include <memory>

class Shelly1PMManager {

public:
    Shelly1PMManager(Shelly1PMManager &&) = delete;

    Shelly1PMManager &operator=(const Shelly1PMManager &) = delete;

    Shelly1PMManager &operator=(Shelly1PMManager &&) = delete;

    void loop();

    virtual ~Shelly1PMManager(){
        delete  adapter;
        delete  shelly1pm;
        delete  relayProp;
        delete  inputProp;
        delete  power1Prop;
        delete  energy1Prop;
        delete  rssiProp;
        delete  mcuTemperatureProp;
        delete  ambientTemperatureProp;
        delete  ipAddressProp;
        delete  gatewayAddressProp;
        delete  apMACAddressProp;
    };

    void inputChanged(int inputNumber, bool value);

    void outputChanged(int outputNumber, bool value);

    void setNetworkInfo(const String &ip, const String &gw, const String &apMacAddress);

    void setTemperature(float temperature);

    void reportAsyncEnergyAndPower(float power, float energy);

    void reportPeriodicEnergyAndPower(float power1, float energy1);

    void setRSSI(int32_t rssi);

    void setMCUTemperature(float mcu_temperature);

    static Shelly1PMManager &getInstance() {
        static Shelly1PMManager instance;
        return instance;
    }

    const String &getMacAddress() const;

    bool validTemperature() const {
        return this->temperature > -100;
    }


    const String &getApMacAddress() const;
    const int32_t &getRssi() const;

    const String &getDeviceName() const;


    static void relay1PropChanged(ThingPropertyValue newValue);


protected:

    String deviceName {""};

    Shelly1PMManager();



    String macAddress;

    bool input1{false}, output1{false};

    float power1{0.0}, energy1{0.0};
    float mcuTemperature {-100};
    float temperature {-100};

    // network info
    String ipAddress {}, gateway{}, apMacAddress{};
    int32_t rssi {};

    // Web Things
    WebThingAdapter *adapter;
    ThingDevice* shelly1pm;
    ThingProperty* relayProp;
    ThingProperty* inputProp;
    ThingProperty* power1Prop;
    ThingProperty* energy1Prop;


    ThingProperty* rssiProp;
    ThingProperty* mcuTemperatureProp;
    ThingProperty* ambientTemperatureProp;
    ThingProperty* ipAddressProp;
    ThingProperty* gatewayAddressProp;
    ThingProperty* apMACAddressProp;


};


#endif //FINGER_ESP_FINGERSTATE_H
