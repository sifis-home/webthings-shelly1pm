
#include "FingerTimer.h"

FingerTimer::FingerTimer(bool startEnabled) : startMillis(millis()), running(startEnabled) {

}

void FingerTimer::reset() {
    startMillis = millis();
}

bool FingerTimer::elapsed(unsigned long ms, bool autoreset) {
    if (!this->running)
        return true;

    auto cond = (bool) (((unsigned long) (millis() - startMillis)) > ms);
    if (cond && autoreset)
        this->reset();
    return cond;
}

void FingerTimer::stop() {
    this->running = false;

}

void FingerTimer::start() {
    this->reset();
    this->running = true;
}

bool FingerTimer::isRunning() const {
    return running;
}
