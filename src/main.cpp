#include <Arduino.h>

const int buzzerPin = 3;
const int sensorPin = 2; // pin, který se spojí s GND

bool lastState = HIGH;

unsigned long lastTriggerTime = 0;
const unsigned long cooldown = 30000; // 30 sekund

void playJingle()
{
  // Frekvence posunuta o oktávu výš (932 Hz * 2 = 1864 Hz) pro lepší slyšitelnost
  int frequencies[] = {1864, 1864, 1864, 1864};

  // Délka trvání jednotlivých tónů v milisekundách
  int durations[] = {170, 170, 170, 170};

  // Pauzy mezi tóny: krátká uvnitř dvojice, dlouhá mezi oběma páry
  int pauses[] = {115, 690, 115, 50};

  for (int i = 0; i < 4; i++)
  {
    // Spustí tón o dané frekvenci a délce
    tone(buzzerPin, frequencies[i], durations[i]);

    // delay musí pokrýt délku tónu + pauzu před dalším tónem
    delay(durations[i] + pauses[i]);
  }
  noTone(buzzerPin); // Pro jistotu vypne bzučák
}

void setup()
{
  pinMode(buzzerPin, OUTPUT);
  pinMode(sensorPin, INPUT_PULLUP); // interní pull-up rezistor

  // Nastavíme čas posledního spuštění hluboko do minulosti
  lastTriggerTime = -cooldown;
}

void loop()
{
  bool currentState = digitalRead(sensorPin);

  // detekce změny (HIGH -> LOW = spojeno s GND)
  if (lastState == HIGH && currentState == LOW)
  {
    unsigned long now = millis();

    if (now - lastTriggerTime >= cooldown)
    {
      playJingle();
      lastTriggerTime = now;
    }
  }

  lastState = currentState;
}