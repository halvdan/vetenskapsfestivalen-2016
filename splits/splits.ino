#include <Bounce2.h>

#define LED 13
#define ARDUINO_SIGNAL_OUT 8
#define ARDUINO_SIGNAL_IN 7
#define SENSOR_IN 2

int arduino_signal;

unsigned long split_time = 0;
unsigned long start_time = 0;

float best_time = 0;

Bounce debouncer = Bounce();

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(ARDUINO_SIGNAL_OUT, OUTPUT);
  pinMode(ARDUINO_SIGNAL_IN, INPUT);
  
  digitalWrite(ARDUINO_SIGNAL_OUT, LOW);

  debouncer.attach(SENSOR_IN);
  debouncer.interval(5);
  
  Serial.begin(9600);
}

void loop() {
  debouncer.update();
  
  int value = debouncer.read();
  arduino_signal = digitalRead(ARDUINO_SIGNAL_IN);


  if (value == LOW) {
    split_time = millis();
    
    digitalWrite(LED, HIGH);
    digitalWrite(ARDUINO_SIGNAL_OUT, HIGH);
    delay(10);
    digitalWrite(ARDUINO_SIGNAL_OUT, LOW);

    if (start_time > 0) {
      float final_time = calc_time(start_time, split_time);
      Serial.println("-----");
      Serial.print(final_time, 3);
      Serial.println(" s");
      Serial.println("-----");
      start_time = 0;

      if (best_time == 0 || final_time < best_time) {
        best_time = final_time;
        Serial.println("!!! NEW RECORD !!!");
        // SAVE TO EEPROM
      }
    }
  }

  float some_time = calc_time(start_time, millis());
  if (best_time != 0 && start_time != 0 && some_time > best_time) {
    if (millis() / 200 % 2 == 0) {
      digitalWrite(LED, HIGH);
    } else {
      digitalWrite(LED, LOW);
    }
  }

  if (arduino_signal == HIGH) {
    Serial.println("ZING!!!!!!!");
    start_time = millis();
    digitalWrite(LED, LOW);
  }
}

float calc_time(long start_millis, long stop_millis) {
  long ms = stop_millis - start_millis;
  
  return ms / 1000.0;
}


