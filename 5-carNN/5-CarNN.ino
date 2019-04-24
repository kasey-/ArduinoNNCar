extern "C"{
  #include "Tinn.h"
};
#include <IRremote.h>
#include <L298N.h>

// Car state
#define CAR_STATE_FORWARD  0
#define CAR_STATE_RIGHT    1
#define CAR_STATE_BACKWARD 2
#define CAR_STATE_LEFT     3
#define CAR_STATE_STOP    -1

// IR setup
#define IR_PIN 2

// Sensor setup
#define DIST_SENS_RIGHT_PIN A4
#define DIST_SENS_LEFT_PIN  A5

// Motor Right
#define ENA 5
#define IN1 6
#define IN2 7
// Motor Left
#define IN3 8
#define IN4 9
#define ENB 10
// Motor Settings
#define MOTOR_SPEED 255

// IR Remote Hexa Keycode
#define KEYCODE_UP    0x00FD8877
#define KEYCODE_RIGHT 0x00FD6897
#define KEYCODE_LEFT  0x00FD28D7
#define KEYCODE_DOWN  0x00FD9867
#define KEYCODE_HOLD  0xFFFFFFFF

// Neural Network Parameters, Biases & Weights
#define NN_INPUT  6
#define NN_HIDDEN 7
#define NN_OUTPUT 2
float biases[] = {
  -0.013483, 0.397656
};

float weights[] = {
  -2.779373, -2.721625, 2.606902, 0.318619, -3.514817, -3.545094,
  -0.162132, -0.644565, -1.713277, 0.866418, -0.644662, -0.324716,
  0.212939, -2.334723, -1.849762, 0.776425, -3.585699, -3.717424,
  -1.321438, -1.825798, 1.955568, -0.006805, 5.102677, 4.917130,
  -0.473502, 0.120281, -2.841549, -0.305924, 0.046639, 0.414302,
  0.355330, 0.081733, -2.734358, -0.398892, 0.407724, -0.183350,
  -2.249321, -0.150071, 5.477276, -3.146920, -3.382013, -3.356823,
  -0.727476, 0.938150, 3.876003, -2.980558, 2.305927, 1.843021,
  -6.008123, -4.465878, 2.695777, 0.105951, -4.759139, 3.003681,
  3.371545, -0.732590
};

Tinn tinn ;

L298N motor1(ENA, IN1, IN2);
L298N motor2(ENB, IN3, IN4);

IRrecv irrecv(IR_PIN);
unsigned long last_ir = millis();

int action_state = CAR_STATE_STOP;

void setup() {
  // Setup the sensors
  pinMode(DIST_SENS_RIGHT_PIN, INPUT);
  pinMode(DIST_SENS_LEFT_PIN, INPUT);

  // Start the receiver
  irrecv.enableIRIn();  

  // Setup the neural network
  tinn = xtbuild(NN_INPUT, NN_HIDDEN, NN_OUTPUT);
  for(int i=0; i < tinn.nb; i++)
    tinn.b[i] = biases[i];
  for(int i=0; i < tinn.nw; i++)
    tinn.w[i] = weights[i];
  
  // Setup the motors
  motor1.stop();
  motor2.stop();
  motor1.setSpeed(MOTOR_SPEED);
  motor2.setSpeed(MOTOR_SPEED);
}

void drive_motor(float cmd_motor1, float cmd_motor2) {
  if(cmd_motor1 <= 0.25) {
    motor1.backward();
  } else if(cmd_motor1 > 0.25 && cmd_motor1 <= 0.75) {
    motor1.stop();
  } else {
    motor1.forward();
  }
  
  if(cmd_motor2 <= 0.25) {
    motor2.backward();
  } else if(cmd_motor2 > 0.25 && cmd_motor2 <= 0.75) {
    motor2.stop();
  } else {
    motor2.forward();
  }
}

void decode_action(decode_results *results) {
  switch(results->value) {
    case KEYCODE_UP:
      action_state = CAR_STATE_FORWARD;
    break;
    case KEYCODE_RIGHT:
      action_state = CAR_STATE_RIGHT;
    break;
    case KEYCODE_DOWN:
      action_state = CAR_STATE_BACKWARD;
    break;
    case KEYCODE_LEFT:
      action_state = CAR_STATE_LEFT;
    break;
  }
}

float* predict_output() {
  float NN_input[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
  
  if(action_state != CAR_STATE_STOP)
    NN_input[action_state] = 1.0;
  
  NN_input[4] = (float) !digitalRead(DIST_SENS_LEFT_PIN);
  NN_input[5] = (float) !digitalRead(DIST_SENS_RIGHT_PIN);

  return xtpredict(tinn,NN_input);
}

void loop() {
  // handle user remote commands
  decode_results results;         // Somewhere to store the results
  if (irrecv.decode(&results)) {  // Grab an IR code
    decode_action(&results);
    irrecv.resume();              // Prepare for the next value
    last_ir = millis();
  }

  // wait 210 ms to declare no new command. The remote send a command every 100 ms
  if (millis() - last_ir > 210) { 
    action_state = CAR_STATE_STOP;
  }

  // use neural network to predict motor command
  float *out = predict_output();
  drive_motor(out[0], out[1]);
}
