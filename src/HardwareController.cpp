#include "HardwareController.h"

float HardwareController::getMCUTemperature(){
    float Vref = 1.0, Rup=32000, Rntc_base = 10000, Vcc=3.3, T1=298.15, Beta=3350;

    float Vntc = (float) analogRead(0)/1023*Vref; // NTC voltage


    float Rntc = (Vntc * Rup)/(Vcc - Vntc);

    float T2 = Beta/( log(Rntc/(Rntc_base*exp(-Beta/T1))) );

    return T2-273.15; // gradi celsius

}


void HardwareController::loop() {

    this->searchDs18b20Sensors();

    this->powerMeasurementController.loop();

    if (this->temperatureTimer.elapsed(10000)) {
        Ds18x20EverySecond();
        Ds18x20Show();
    }

    auto new_state = digitalRead(INPUT_1_PIN);


    if (new_state != this->pinState) {
        this->inputTimer.start();
        this->pinState = new_state;
        this->inputHandled = false;
    }

    if (!this->inputHandled && \
             this->inputTimer.elapsed(INPUT_DEBOUNCE_TIME_MS, false)) {

        Shelly1PMManager::getInstance().inputChanged(1, this->pinState);
        this->inputHandled = true;
    }

}

void HardwareController::searchDs18b20Sensors() const {
    static FingerTimer ds18b20SearchTimer;
    static int tries {0};

    if (!Ds18x20FoundSensors() && tries < 5) {
        if (ds18b20SearchTimer.elapsed(5000)) {
            Ds18x20Init();
            ++tries;
        }
    }
}
