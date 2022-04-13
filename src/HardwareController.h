
#ifndef FINGER_ESP_HARDWARECONTROLLER_H
#define FINGER_ESP_HARDWARECONTROLLER_H

#include <Arduino.h>
#include "Shelly1PMManager.h"
#include "FingerTimer.h"
#include "PowerMeasurementController.h"
#include "utils.h"

#include "defines.h"
#include "DS18B20Dual.h"

class HardwareController {
public:

    static HardwareController &getInstance() {
        static HardwareController instance;
        return instance;
    }

    void init() {
        pinMode(INPUT_1_PIN, INPUT);
        pinMode(RELAY_1_PIN, OUTPUT);

        this->pinState = digitalRead(INPUT_1_PIN);

        Ds18x20Init();
    }

    void setRelay(uint8_t port, bool value) {

        if (port == 1) {
            digitalWrite(RELAY_1_PIN, value);
            Shelly1PMManager::getInstance().outputChanged(port, value);
        }

    }

    void loop();

    HardwareController(HardwareController &&) = delete;

    HardwareController &operator=(const HardwareController &) = delete;

    HardwareController &operator=(HardwareController &&) = delete;

    virtual ~HardwareController() = default;

    float getMCUTemperature();

private:

    HardwareController() = default;

    FingerTimer inputTimer {};

    FingerTimer temperatureTimer {};

    PowerMeasurementController powerMeasurementController;

    bool pinState{false};
    bool inputHandled {false};

    uint8_t RELAY_1_PIN{15};

    uint8_t INPUT_1_PIN{4};

    void searchDs18b20Sensors() const;
};


#endif //FINGER_ESP_HARDWARECONTROLLER_H
