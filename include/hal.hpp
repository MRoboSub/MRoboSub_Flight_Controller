#pragma once
#include <Arduino.h>

#define NUM_MOTORS 8

const int BUZZER_PIN = 9;

namespace MotorConstants {
  const int FREQUENCY    = 50;   // Hz, T = 20ms = 20_000us
  const int RESOLUTION   = 14;   // resolution 0 to (2^(14)-1)=16383

  // (Pulse Width / Period Time) * (2^resolution - 1)
  // e.g. for neutral at 1500us period:
  //      (1500us / 20_000us) * 16383 = 1228.7 ~= 1229
  const int NEUTRAL_DUTY = 1229; // 1500us
  const int FORWARD_MAX  = 1556; // 1900us
  const int REVERSE_MAX  = 901;  // 1100us

  constexpr int PIN_VALUES[] = {
      5, 6, 7, 8, 4, 3, 2, 1};

  constexpr const char *PIN_NAMES[] = {
      "L1", "L2", "L3", "L4", "L5", "L6", "L7", "L8"};
}

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
    bool globalEnable = false;
    bool emergencyStopped = false;

    struct Pin
    {
        const char *label;
        int gpio;
    };
    Pin pins[8];
    int duty[8];

    SoundType requestedSound = SoundType::NONE;
    void playSound();
};
