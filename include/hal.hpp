#pragma once
#include <Arduino.h>
#include <ESP32Servo.h>

const int BUZZER_PIN = 9;
constexpr int PIN_VALUES[] = {
    5, 6, 7, 8, 4, 3, 2, 1};
constexpr const char *PIN_NAMES[] = {
    "L1", "L2", "L3", "L4", "L5", "L6", "L7", "L8"};

class HAL
{
public:
    HAL();

    enum class SoundType
    {
        NONE,
        LOWHIGH,
        HIGHLOW,
        ENABLETEST,
        DISABLETEST
    };

    bool enable();
    void disable();
    void estop();

    void applyPower(int index, int percent);
    void requestSound(SoundType state);

    void update(int latest_heartbeat);

    const char *getPinLabel(int i);
    int getPinPercent(int i);
    bool isEnabled();

private:
    bool global_enable = false;
    bool emergency_stopped = false;
    struct Pin
    {
        const char *label;
        int gpio;
        Servo servo;
    };
    Pin pins[8];
    int duty[8];

    SoundType requestedSound = SoundType::NONE;

    void playSound();
};