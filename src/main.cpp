#include <Arduino.h>

const int buzzerPin = 3;
const int sensorPin = 2; // pin, který se spojí s GND

bool lastState = HIGH;

unsigned long lastTriggerTime = 0;
const unsigned long cooldown = 30000; // 30 sekund

void playJingle()
{
  // Frekvence pro Samsung písknutí: C6, E6, C7, B6, G6
  int frequencies[] = {1047, 1319, 2093, 1976, 1568};

  // Délka trvání jednotlivých tónů v milisekundách
  int durations[] = {100, 100, 140, 140, 250};

  // Krátká pauza po každém tónu pro čisté oddělení zvuku
  int pauses[] = {50, 50, 50, 140, 10};

  for (int i = 0; i < 5; i++)
  {
    // Spustí tón o dané frekvenci a délce
    tone(buzzerPin, frequencies[i], durations[i]);

    // delay musí pokrýt délku tónu + pauzu před dalším tónem,
    // protože funkce tone() neblokuje běh programu
    delay(durations[i] + pauses[i]);
  }
  noTone(buzzerPin); // Pro jistotu vypne bzučák
}

void setup()
{
  pinMode(buzzerPin, OUTPUT);
  pinMode(sensorPin, INPUT_PULLUP); // interní pull-up rezistor

  // Nastavíme čas posledního spuštění hluboko do minulosti,
  // aby byl cooldown splněn hned při prvním spuštění.
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