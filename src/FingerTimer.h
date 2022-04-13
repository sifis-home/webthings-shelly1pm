
#ifndef FINGER_ESP_FINGERTIMER_H
#define FINGER_ESP_FINGERTIMER_H

#include <cstdint>
#include <Arduino.h>

class FingerTimer {
protected:
    unsigned long startMillis;
    bool running {true};
public:

    explicit FingerTimer(bool startEnabled = true);

    void reset();
    void stop();

    void start();

    bool isRunning() const;

    bool elapsed(unsigned long ms,  bool autoreset=true);

};


#endif //FINGER_ESP_FINGERTIMER_H
