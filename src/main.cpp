#include <Arduino.h>

const int buzzerPin = 3;
const int sensorPin = 2; // pin, který se spojí s GND

bool lastState = HIGH;

unsigned long lastTriggerTime = 0;
const unsigned long cooldown = 30000; // 30 sekund

void playJingle()
{
  tone(buzzerPin, 1000, 120);
  delay(150);
  tone(buzzerPin, 1400, 120);
  delay(150);
  tone(buzzerPin, 1800, 180);
  delay(220);
  noTone(buzzerPin);
}

void setup()
{
  pinMode(buzzerPin, OUTPUT);
  pinMode(sensorPin, INPUT_PULLUP); // interní pull-up rezistor
}

void loop()
{
  bool currentState = digitalRead(sensorPin);

  // detekce změny (HIGH -> LOW = spojeno s GND)
  if (lastState == HIGH && currentState == LOW)
  {
    unsigned long now = millis();

    if (now - lastTriggerTime > cooldown || lastTriggerTime == 0)
    {
      playJingle();
      lastTriggerTime = now;
    }
  }

  lastState = currentState;
}