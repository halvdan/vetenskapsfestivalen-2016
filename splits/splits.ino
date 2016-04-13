#include <Bounce2.h>
#include <EEPROM.h>

#define LED 13
#define ARDUINO_SIGNAL_OUT 8
#define ARDUINO_SIGNAL_IN 7
#define SENSOR_IN 2

#define BEST_TIME_ADDRESS 0

unsigned long split_time = 0;
unsigned long start_time = 0;

float best_time = 0;

Bounce sensor_debounce = Bounce();
Bounce arduino_debounce = Bounce();

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(ARDUINO_SIGNAL_OUT, OUTPUT);
  
  digitalWrite(ARDUINO_SIGNAL_OUT, LOW);

  sensor_debounce.attach(SENSOR_IN);
  sensor_debounce.interval(5);
  arduino_debounce.attach(ARDUINO_SIGNAL_IN);
  arduino_debounce.interval(20);

  float foo = 0.00f;
  EEPROM.put(BEST_TIME_ADDRESS, foo);
  
  float stored_time;
  EEPROM.get(BEST_TIME_ADDRESS, stored_time);
  if (stored_time > 0) {
    best_time = stored_time;
  }

  Serial.begin(9600);
  Serial.println(stored_time);
}

void loop() {
  sensor_debounce.update();
  arduino_debounce.update();
  
  int sensor_value = sensor_debounce.read();
  int arduino_value = arduino_debounce.read();

  if (sensor_value == HIGH) {
    split_time = millis();
    
    digitalWrite(LED, HIGH);
    if (start_time == 0) {
      notify_next();
    }

    if (start_time > 0) {
      float final_time = calc_time(start_time, split_time);
      Serial.println("-----");
      Serial.print(final_time, 3);
      Serial.println(" s");
      Serial.println("-----");
      start_time = 0;

      if (best_time == 0 || final_time < best_time) {
        Serial.println("!!! NEW RECORD !!!");
        best_time = final_time;
        store_best_time(best_time);
      }
    }
  }

  float some_time = calc_time(start_time, millis());
  if (best_time != 0 && start_time != 0 && some_time > best_time) {
    flashing_led();
  }

  if (arduino_value == HIGH) {
    Serial.println("ZING!!!!!!!");
    start_time = millis();
    digitalWrite(LED, LOW);
    notify_next();
  }
}

void store_best_time(float f) {
  EEPROM.put(BEST_TIME_ADDRESS, best_time);
}

void notify_next() {
  digitalWrite(ARDUINO_SIGNAL_OUT, HIGH);
  delay(10);
  digitalWrite(ARDUINO_SIGNAL_OUT, LOW);
}

void flashing_led() {
  if (millis() / 200 % 2 == 0) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}

float calc_time(long start_millis, long stop_millis) {
  long ms = stop_millis - start_millis;
  
  return ms / 1000.0;
}


