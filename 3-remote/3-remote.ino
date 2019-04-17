#include <IRremote.h>

#define KEYCODE_UP    0x00FD8877
#define KEYCODE_RIGHT 0x00FD6897
#define KEYCODE_LEFT  0x00FD28D7
#define KEYCODE_DOWN  0x00FD9867
#define IR_PIN 2

IRrecv irrecv(IR_PIN);
unsigned long last = millis();

void setup() {
  Serial.begin(115200);	
  irrecv.enableIRIn();  // Start the receiver
}

void decode_action(decode_results *results) {
  switch(results->value) {
    case KEYCODE_UP:
      Serial.println("up");
    break;
    case KEYCODE_RIGHT:
      Serial.println("right");
    break;
    case KEYCODE_LEFT:
      Serial.println("left");
    break;
    case KEYCODE_DOWN:
      Serial.println("down");
    break;
  }
}

void loop() {
  decode_results  results;        // Somewhere to store the results
  if (irrecv.decode(&results)) {  // Grab an IR code
    decode_action(&results);
    irrecv.resume();              // Prepare for the next value
    last = millis();
  }
  if (millis() - last > 110) {    // wait 110 ms to declare no new command
    Serial.println("stop");
  }
}