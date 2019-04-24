extern "C"{
  #include "Tinn.h"
};

// Car state
#define CAR_STATE_FORWARD  0
#define CAR_STATE_RIGHT    1
#define CAR_STATE_BACKWARD 2
#define CAR_STATE_LEFT     3
#define CAR_STATE_STOP    -1

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


void setup() {
  Serial.begin(115200);	
  // Setup the neural network
  tinn = xtbuild(NN_INPUT, NN_HIDDEN, NN_OUTPUT);
  for(int i=0; i < tinn.nb; i++)
    tinn.b[i] = biases[i];
  for(int i=0; i < tinn.nw; i++)
    tinn.w[i] = weights[i];
}

void predict(int command, float sensor1, float sensor2) {
    float NN_input[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    if(command != CAR_STATE_STOP)
      NN_input[command] = 1.0;
    NN_input[4] = sensor1;
    NN_input[5] = sensor2;
    float *out = xtpredict(tinn,NN_input);
    Serial.print(out[0],DEC);
    Serial.print(" ");
    Serial.println(out[1],DEC);
}

void loop() {
  predict(CAR_STATE_STOP,    0.0, 0.0);
  predict(CAR_STATE_FORWARD, 0.0, 0.0);
  predict(CAR_STATE_RIGHT,   0.0, 0.0);
  predict(CAR_STATE_BACKWARD,0.0, 0.0);
  predict(CAR_STATE_LEFT,    0.0, 0.0);
  predict(CAR_STATE_FORWARD, 1.0, 0.0);
  predict(CAR_STATE_FORWARD, 0.0, 1.0);
  predict(CAR_STATE_FORWARD, 1.0, 1.0);
  delay(10000);
}
