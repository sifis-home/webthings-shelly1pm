
#ifndef FINGER_ESP_UTILS_H
#define FINGER_ESP_UTILS_H

template<class T>
inline T deadzone(T val, T threshold) {
    return (val > threshold) ? (val) : (0);
};

#endif //FINGER_ESP_UTILS_H
