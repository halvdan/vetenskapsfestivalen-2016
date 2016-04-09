#define LED 13
#define ARDUINO_SIGNAL_OUT 8
#define ARDUINO_SIGNAL_IN 7
#define SENSOR_IN 0

#define SIGNAL_PIN 7

enum split_state {
  READY,
  START,
  RUNNING,
  DONE
};

split_state current_state = RUNNING;

int sensor;
int arduino_signal;

unsigned long start_time = 0;
unsigned long foo_time = 0;
unsigned long bar_time = 0;

float previous_time = 0;
float best_time = 0;

unsigned long ms = 0;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(ARDUINO_SIGNAL_OUT, OUTPUT);
  pinMode(ARDUINO_SIGNAL_IN, INPUT);
  pinMode(SENSOR_IN, INPUT);
  
  Serial.begin(9600);

  foo_done();
}

void loop() {
  sensor = digitalRead(SENSOR_IN);
  arduino_signal = digitalRead(ARDUINO_SIGNAL_IN);

  Serial.println(sensor);

  if (sensor == LOW) {
    if (current_state == RUNNING) {
      //sensor_activated();

      analogWrite(LED, 255);
      delay(1000);
    }
  }

  delay(1000);

/*

  if (arduino_signal == LOW) {
    switch (arduino_signal) {
      case READY:
        foo_ready();
        break;
      case START:
        foo_start();
        break;
      case RUNNING:
        foo_running();
        break;
      case DONE:
        foo_done();
        break;
    }
  }
  */
}

/**
 * Received signal from while in state ready. This means that the 
 * lap has started and thus we start the timer. And update our 
 * state. And notify the next controller in the chain.
 */
void foo_ready() {
  start_time = millis();
  current_state = START;
  digitalWrite(ARDUINO_SIGNAL_OUT, LOW);
  delay(10);
  digitalWrite(ARDUINO_SIGNAL_OUT, HIGH);
}

/**
 * Received signal while in state START.
 * 
 * The runner has reached the previous sensor and is en route to 
 * this sensor. Therefore we note the time since start. Update 
 * state to RUNNING.
 */
void foo_start() {
  foo_time = millis();
  current_state = RUNNING;
}

/**
 * Received signal while in state RUNNING.
 */
void foo_running() {
  // DO NOTHING
}

/**
 * Received signal while in state DONE.
 * 
 * Indicating that we have come full circle and are wrapping 
 * up stuff and are preparing for a new race. Update state 
 * to READY.
 */
void foo_done() {
  current_state = READY;
  float final_time = calc_time(start_time, millis());

  Serial.println("-----");
  Serial.print(final_time, 3);
  Serial.println(" s");
  Serial.println("-----");
}

/**
 * Received signal from our sensor.
 * 
 * Update time, notify next Arduino. Update state.
 */
void sensor_activated() {
  bar_time = millis();
  current_state = DONE;
  digitalWrite(ARDUINO_SIGNAL_OUT, LOW);
  delay(10);
  digitalWrite(ARDUINO_SIGNAL_OUT, HIGH);
}
/*
void start_stuff() {
  ms = millis();
  is_running = true;
  analogWrite(RED, 0);
  analogWrite(GRN, 255);
  delay(1000);
}

void stop_stuff() {
  float current_time = calc_time(ms, millis());
  
  if (current_time < best_time) {
    best_time = current_time;
  }

  previous_time = current_time;
  is_running = false;

  Serial.print(current_time, 3);
  Serial.println("s");
  
  analogWrite(GRN, 0);
  analogWrite(RED, 255);
  delay(1000);
}*/

float calc_time(long start_millis, long stop_millis) {
  long ms = stop_millis - start_millis;
  
  return ms / 1000.0;
}


