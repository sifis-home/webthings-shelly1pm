#include <FS.h>
#include <ESP8266WiFi.h>
#include <algorithm>
#include "utils.h"
#include "defines.h"
#include "Shelly1PMManager.h"
#include "HardwareController.h"

void Shelly1PMManager::setRSSI(int32_t rssi) {
    this->rssi=rssi;

    ThingPropertyValue valueOfProp;
    valueOfProp.number = this->rssi;
    this->rssiProp->setValue(valueOfProp);

}

void Shelly1PMManager::setMCUTemperature(float mcu_temperature){
    this->mcuTemperature = mcu_temperature;

    ThingPropertyValue valueOfProp;
    valueOfProp.number = this->mcuTemperature;
    this->mcuTemperatureProp->setValue(valueOfProp);
}

void Shelly1PMManager::setNetworkInfo(const String &ip, const String &gw, const String &apMacAddress) {
    this->ipAddress = ip;
    this->gateway = gw;
    this->apMacAddress = apMacAddress;

    ThingPropertyValue valueOfProp;
    valueOfProp.string = &this->ipAddress;
    this->ipAddressProp->setValue(valueOfProp);

    valueOfProp.string = &this->gateway;
    this->gatewayAddressProp->setValue(valueOfProp);

    valueOfProp.string = &this->apMacAddress;
    this->apMACAddressProp->setValue(valueOfProp);


}

void Shelly1PMManager::inputChanged(int inputNumber, bool value) {

    if (inputNumber == 1) {
        if (value == this->input1)
            return;
        this->input1 = value;
    } else
        return;

    ThingPropertyValue valueOfProp;
    valueOfProp.boolean = this->input1;
    this->inputProp->setValue(valueOfProp);

}

void Shelly1PMManager::outputChanged(int outputNumber, bool value) {

    if (outputNumber == 1) {

        if (value == this->output1)
            return;

        this->output1 = value;

    } else
        return;

}


Shelly1PMManager::Shelly1PMManager() {

    this->macAddress = WiFi.macAddress();
    this->macAddress.toLowerCase();
    this->macAddress.replace(":", "");

    const char *shelly1PMTypes[] = {"OnOffSwitch", "Light", nullptr};

    this->deviceName = "shelly1pm-" + this->macAddress;

    shelly1pm = new ThingDevice(deviceName.c_str(), "Shelly 1 PM", shelly1PMTypes);
    this->relayProp = new ThingProperty("relay", "", BOOLEAN, "OnOffProperty",
                                        &Shelly1PMManager::relay1PropChanged);

    this->inputProp = new ThingProperty("input", "", BOOLEAN, "OnOffProperty");

    this->power1Prop = new ThingProperty("power1", "", NUMBER, "LevelProperty");

    this->energy1Prop = new ThingProperty("energy1", "", NUMBER, "LevelProperty");


    this->rssiProp = new ThingProperty("rssi", "", NUMBER, "LevelProperty");

    this->mcuTemperatureProp = new ThingProperty("mcuTemperature", "", NUMBER,
                                                 "LevelProperty");
    this->ambientTemperatureProp = new ThingProperty("ambientTemperature", "", NUMBER,
                                                     "LevelProperty");
    this->ipAddressProp = new ThingProperty("ipAddress", "", STRING, nullptr);

    this->gatewayAddressProp = new ThingProperty("gatewayAddress", "", STRING, nullptr);

    this->apMACAddressProp = new ThingProperty("apMACAddress", "", STRING, nullptr);

    adapter = new WebThingAdapter("w25", WiFi.localIP());

    shelly1pm->addProperty(relayProp);
    shelly1pm->addProperty(inputProp);
    shelly1pm->addProperty(power1Prop);
    shelly1pm->addProperty(energy1Prop);
    shelly1pm->addProperty(rssiProp);
    shelly1pm->addProperty(mcuTemperatureProp);
    shelly1pm->addProperty(ambientTemperatureProp);
    shelly1pm->addProperty(ipAddressProp);
    shelly1pm->addProperty(gatewayAddressProp);
    shelly1pm->addProperty(apMACAddressProp);


    adapter->addDevice(shelly1pm);
    adapter->begin();

}

const String &Shelly1PMManager::getMacAddress() const {
    return macAddress;
}


void Shelly1PMManager::reportAsyncEnergyAndPower(float power, float energy) {

    power = deadzone(power, POWER_MEASUREMENT_DEADZONE_W);
    energy = deadzone(energy, ENERGY_MEASUREMENT_DEADZONE_W);
    this->energy1 = energy;
    this->power1 = power;

    ThingPropertyValue valueOfPower1Prop, valueOfEnergy1Prop;
    valueOfPower1Prop.number = this->power1;
    valueOfEnergy1Prop.number = this->energy1;

    this->power1Prop->setValue(valueOfPower1Prop);
    this->energy1Prop->setValue(valueOfEnergy1Prop);

}

void Shelly1PMManager::reportPeriodicEnergyAndPower(float power1, float energy1) {

    auto power = deadzone(power1, POWER_MEASUREMENT_DEADZONE_W);
    auto energy = deadzone(energy1, ENERGY_MEASUREMENT_DEADZONE_W);
    this->energy1 = energy;
    this->power1 = power;

    ThingPropertyValue valueOfPower1Prop, valueOfEnergy1Prop;
    valueOfPower1Prop.number = this->power1;
    valueOfEnergy1Prop.number = this->energy1;

    this->power1Prop->setValue(valueOfPower1Prop);
    this->energy1Prop->setValue(valueOfEnergy1Prop);

}

void Shelly1PMManager::setTemperature(float temperature) {
    this->temperature = temperature;
}

const int32_t &Shelly1PMManager::getRssi() const {
    return rssi;
}

const String &Shelly1PMManager::getApMacAddress() const {
    return apMacAddress;
}

void Shelly1PMManager::loop() {
    adapter->update();
}

void Shelly1PMManager::relay1PropChanged(ThingPropertyValue newValue) {
    Serial.println("Changing relay");
    HardwareController::getInstance().setRelay(1, newValue.boolean);
}

const String &Shelly1PMManager::getDeviceName() const {
    return deviceName;
}
