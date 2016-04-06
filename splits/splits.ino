#define RED 13
#define GRN 8

#define SIGNAL_PIN 7

int some_signal;
boolean is_running = false;
unsigned long ms = 0;

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GRN, OUTPUT);
  pinMode(SIGNAL_PIN, INPUT);

  Serial.begin(9600);

  stop_stuff();
}

void loop() {
  some_signal = digitalRead(SIGNAL_PIN);

  if (some_signal == LOW) {
    Serial.println("Received signal");
    if (is_running) {
      Serial.println("Stop stuff");
      stop_stuff();
    } else {
      Serial.println("Start stuff");
      start_stuff();
    }
  }
}

void start_stuff() {
  ms = millis();
  is_running = true;
  analogWrite(RED, 0);
  analogWrite(GRN, 255);
  delay(1000);
}

void stop_stuff() {
  float seconds = calc_time(ms, millis());
  is_running = false;
  
  Serial.print("Seconds: ");
  Serial.println(seconds, 3);
  
  analogWrite(GRN, 0);
  analogWrite(RED, 255);
  delay(1000);
}

float calc_time(long start_millis, long stop_millis) {
  long ms = stop_millis - start_millis;
  
  return ms / 1000.0;
}


