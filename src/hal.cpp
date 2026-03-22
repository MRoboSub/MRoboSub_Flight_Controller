#include "hal.hpp"

HAL::HAL()
{
    for (int i = 0; i < 8; i++)
    {
        duty[i] = 1500;
        pins[i].gpio = PIN_VALUES[i];
        pins[i].label = PIN_NAMES[i];
        pins[i].servo.attach(pins[i].gpio);
    }
}

void HAL::applyPower(int index, int percent)
{
    if (index < 0 || index > 7)
        return;

    int duty = map(constrain(percent, -100, 100), -100, 100, 1100, 1900);
    this->duty[index] = duty;
}

void HAL::requestSound(SoundType state)
{
    requestedSound = state;
}

void HAL::update(int latest_update)
{
    if (false)
    {
        global_enable = false;
        emergency_stopped = true;
    }

    if (global_enable)
    {
        for (int i = 0; i < 8; i++)
        {
            pins[i].servo.writeMicroseconds(duty[i]);
        }
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            pins[i].servo.writeMicroseconds(1500);
        }
    }
}

bool HAL::enable()
{

    if (emergency_stopped)
        return false;
    for (int i = 0; i < 8; i++)
    {
        duty[i] = 1500;
    }
    return global_enable = true;
}

void HAL::disable()
{
    global_enable = false;
}

void HAL::estop()
{
    global_enable = false;
    emergency_stopped = true;
}

void HAL::playSound()
{

    // if (requestedSound == HIGHLOW)
    // {
    //     int firstTone = 3500;
    //     int secondTone = 2500;

    //     ledcChangeFrequency(BUZZER_PIN, firstTone, 10);
    //     ledcWrite(BUZZER_PIN, 256);
    //     delay(100);

    //     ledcChangeFrequency(BUZZER_PIN, secondTone, 10);
    //     ledcWrite(BUZZER_PIN, 256);
    //     delay(100);

    //     ledcWrite(BUZZER_PIN, 0);
    // }
    // else if (req == DISABLETEST)
    // {

    //     ledcChangeFrequency(BUZZER_PIN, 1000, 10);
    //     ledcWrite(BUZZER_PIN, 256);
    //     delay(200);

    //     ledcWrite(BUZZER_PIN, 0);
    // }
    // else if (state == ENABLETEST)
    // {
    //     ledcChangeFrequency(BUZZER_PIN, 300, 10);
    //     ledcWrite(BUZZER_PIN, 256);
    //     delay(200);

    //     ledcWrite(BUZZER_PIN, 0);
    // }
}

const char *HAL::getPinLabel(int i)
{
    return pins[i].label;
}

int HAL::getPinPercent(int i)
{
    return map(duty[i], 1100, 1900, -100, 100);
}

bool HAL::isEnabled()
{
    return global_enable && !emergency_stopped;
}