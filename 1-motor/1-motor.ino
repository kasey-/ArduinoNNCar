#include <L298N.h>

// Right Motor
#define ENA 5
#define IN1 6
#define IN2 7
// Left Motor
#define IN3 8
#define IN4 9
#define ENB 10

L298N motor1(ENA, IN1, IN2);
L298N motor2(ENB, IN3, IN4);

void setup() {
  Serial.begin(115200);
  motor1.stop();
  motor2.stop();
  motor1.setSpeed(500);
  motor2.setSpeed(500);
}

void loop() {
  Serial.println("M1+2 Stop");
  motor1.stop();
  motor2.stop();
  delay(3000);
  Serial.println("M1 Forward");
  motor1.forward();
  delay(3000);
  Serial.println("M1 Backward");
  motor1.backward();
  delay(3000);
  motor1.stop();
  Serial.println("M2 Forward");
  motor2.forward();
  delay(3000);
  Serial.println("M2 Backward");
  motor1.backward();
  delay(3000);
  Serial.println("M1+2 Forward");
  motor1.forward();
  motor2.forward();
  delay(3000);
  Serial.println("M1+2 Backward");
  motor1.backward();
  motor2.backward();
  delay(3000);
}