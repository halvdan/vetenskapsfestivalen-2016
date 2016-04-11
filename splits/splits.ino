#include <Bounce2.h>

#define LED 13
#define ARDUINO_SIGNAL_OUT 8
#define ARDUINO_SIGNAL_IN 7
#define SENSOR_IN 2

int sensor;
int arduino_signal;

unsigned long split_time = 0;
unsigned long start_time = 0;

float best_time = 0;


float previous_time = 0;

unsigned long ms = 0;

Bounce debouncer = Bounce();

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(ARDUINO_SIGNAL_OUT, OUTPUT);
  pinMode(ARDUINO_SIGNAL_IN, INPUT);
  pinMode(SENSOR_IN, INPUT);

  debouncer.attach(SENSOR_IN);
  debouncer.interval(5);
  
  digitalWrite(ARDUINO_SIGNAL_OUT, LOW);
  
  Serial.begin(9600);
}

void loop() {
  debouncer.update();

  int sensor = debouncer.read();

  arduino_signal = digitalRead(ARDUINO_SIGNAL_IN);

  // Fix sensor trigger
  if (sensor == LOW) {
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

      if (best_time == 0 || final_time < best_time) {
        best_time = final_time;
        Serial.println("!!! NEW RECORD !!!");
        // SAVE TO EEPROM
      }
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


