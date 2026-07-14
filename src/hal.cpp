#include "hal.hpp"

HAL::HAL()
{
  for (unsigned int i = 0; i < NUM_MOTORS; ++i)
  {
    duty[i] = MotorConstants::NEUTRAL_DUTY;
    pins[i].gpio = MotorConstants::PIN_VALUES[i];
    pins[i].label = MotorConstants::PIN_NAMES[i];

    ledcSetup(i, MotorConstants::FREQUENCY, MotorConstants::RESOLUTION);
    ledcAttachPin(pins[i].gpio, i);
    ledcWrite(i, MotorConstants::NEUTRAL_DUTY);
  }
}

void HAL::applyPower(int index, int percent)
{
  if (index < 0 || index > 7)
    return;

  int duty = map(constrain(percent, -100, 100),
                 -100, 100,
                 MotorConstants::REVERSE_MAX, MotorConstants::FORWARD_MAX);

  this->duty[index] = duty;
}

void HAL::requestSound(SoundType state)
{
  requestedSound = state;
}

void HAL::update(int latest_update)
{
  if (globalEnable)
  {
    for (int i = 0; i < 8; i++)
    {
      ledcWrite(i, duty[i]);
    }
  }
  else
  {
    for (int i = 0; i < 8; i++)
    {
      ledcWrite(i, MotorConstants::NEUTRAL_DUTY);
    }
  }
}

bool HAL::enable()
{
  if (emergencyStopped)
    return false;

  for (int i = 0; i < 8; i++)
  {
    duty[i] = MotorConstants::NEUTRAL_DUTY;
  }

  globalEnable = true;
  return true;
}

void HAL::disable()
{
  globalEnable = false;
}

void HAL::estop()
{
  globalEnable = false;
  emergencyStopped = true;
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
  return map(duty[i],
             MotorConstants::REVERSE_MAX, MotorConstants::FORWARD_MAX,
             -100, 100);
}

bool HAL::isEnabled()
{
  return globalEnable && !emergencyStopped;
}
