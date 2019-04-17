#define USER_BUTTON_PIN     13
#define LED_RIGHT_PIN       4
#define LED_LEFT_PIN        3
#define UTRASONIC_TRIG_PIN  A0
#define UTRASONIC_ECHO_PIN  A1
#define LINE_SENS_RIGHT_PIN A2
#define LINE_SENS_LEFT_PIN  A3
#define DIST_SENS_RIGHT_PIN A4 // http://irsensor.wizecode.com
#define DIST_SENS_LEFT_PIN  A5

float measure_ultrasonic_distance() {
  // Note: check if we could avoid using Analog pin to do such a thing
  digitalWrite(UTRASONIC_TRIG_PIN, LOW);   // set trig port low level for 2μs
  delayMicroseconds(2);
  digitalWrite(UTRASONIC_TRIG_PIN, HIGH);  // set trig port high level for 10μs (at least 10μs)
  delayMicroseconds(10);
  digitalWrite(UTRASONIC_TRIG_PIN, LOW);   // set trig port low level
  return pulseIn(UTRASONIC_ECHO_PIN, HIGH) / 58.273;
  /*
  Read echo port high level time(unit:μs)
  Distance(m) =(time(s) * 343.21(m/s)) / 2
  speed of sound at 20C 344.21 m/s.
  ==> 2*Distance(cm) = time(μs) * 0.034321(cm/μs)
  ==> Distance(cm) = time(μs) * 0.0171605 = time(μs) / 58.273
  */
}

void setup() {
  pinMode(LED_RIGHT_PIN, OUTPUT);           // Set led right as output
  pinMode(LED_LEFT_PIN, OUTPUT);            // Set led left as output
  pinMode(USER_BUTTON_PIN, INPUT_PULLUP);   // Set button as input and internal pull-up mode

  pinMode(LINE_SENS_RIGHT_PIN, INPUT);      // Set Right Line Walking Infrared sensor as input
  pinMode(LINE_SENS_LEFT_PIN, INPUT);       // Set left Line Walking Infrared sensor as input
  
  pinMode(DIST_SENS_RIGHT_PIN, INPUT);      // Set Distance Infrared sensor as input
  pinMode(DIST_SENS_LEFT_PIN, INPUT);       // Set Distance Infrared sensor as input

  pinMode(UTRASONIC_ECHO_PIN, INPUT);       // Set Ultrasonic echo port as input
  pinMode(UTRASONIC_TRIG_PIN, OUTPUT);      // Set Ultrasonic trig port as input

  Serial.begin(115200);
}

void loop() {
  Serial.print(digitalRead(USER_BUTTON_PIN)); Serial.print("\t");
  Serial.print(digitalRead(LINE_SENS_RIGHT_PIN)); Serial.print("\t");
  Serial.print(digitalRead(LINE_SENS_LEFT_PIN)); Serial.print("\t");
  Serial.print(digitalRead(DIST_SENS_RIGHT_PIN)); Serial.print("\t");
  Serial.print(digitalRead(DIST_SENS_LEFT_PIN)); Serial.print("\t");
  Serial.println(measure_ultrasonic_distance());
  delay(250);
}