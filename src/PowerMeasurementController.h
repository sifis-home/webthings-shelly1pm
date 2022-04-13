

#ifndef FINGER_ESP_POWERMEASUREMENTCONTROLLER_H
#define FINGER_ESP_POWERMEASUREMENTCONTROLLER_H

#include "HLW8012.h"
#include "FingerTimer.h"

class PowerMeasurementController {

public:

    PowerMeasurementController();

    void loop();

    double getPower();

    double getEnergy();

    HLW8012 hlw8012 {};

    uint32_t cfPulseCount;
protected:

    void _readPower();

    FingerTimer measureTimer {};

    double energySum {0};

    double lastPower {0};

    double lastUpdateTimestamp {-1};

};


#endif //FINGER_ESP_POWERMEASUREMENTCONTROLLER_H
