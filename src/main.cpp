#include <Arduino.h>
#include "HardwareController.h"

// TODO: Hardcode your wifi credentials here (and keep it private)
const char *ssid = "";
const char *password = "";

const int ledPin = 0; // manually configure LED pin

Shelly1PMManager *pmManagerInstance{nullptr};
HardwareController *hardwareControllerInstance{nullptr};


void setup(void) {
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);
    Serial.begin(115200);
    Serial.println("");
    Serial.print("Connecting to \"");
    Serial.print(ssid);
    Serial.println("\"");
#if defined(ESP8266) || defined(ESP32)
    WiFi.mode(WIFI_STA);
#endif
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    bool blink = true;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        digitalWrite(ledPin, blink ? LOW : HIGH); // active low led
        blink = !blink;
    }

    digitalWrite(ledPin, HIGH); // active low led
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    pmManagerInstance = &Shelly1PMManager::getInstance();
    hardwareControllerInstance = &HardwareController::getInstance();
    hardwareControllerInstance->init();

    Serial.println("HTTP server started");
    Serial.println(WiFi.localIP());

    Serial.println("Device id: ");

    Serial.println(pmManagerInstance->getDeviceName());

}

void updateNetworkStatus() {

    if (WiFi.getMode() != WiFiMode::WIFI_STA || !WiFi.isConnected()) {
        return;
    }

    auto bssid = WiFi.BSSIDstr();
    bssid.replace(":", "");
    bssid.toLowerCase();

    pmManagerInstance->setNetworkInfo(WiFi.localIP().toString(), WiFi.gatewayIP().toString(), bssid);
    pmManagerInstance->setRSSI(WiFi.RSSI());
    pmManagerInstance->setMCUTemperature(HardwareController::getInstance().getMCUTemperature());

}


void loop() {
    pmManagerInstance->loop();

    static FingerTimer networkTimer{};

    if (networkTimer.elapsed(10000))
        updateNetworkStatus();

    hardwareControllerInstance->loop();


}

