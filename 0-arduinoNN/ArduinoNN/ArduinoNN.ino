extern "C"{
  #include "Tinn.h"
};

float biases[] = {
  0.179296, 0.434693
};

float weights[] = {
  -0.274637, 0.082248, 0.559791, 0.567023,
  -3.573742, -4.020966, 2.633797, 3.380774
};

Tinn tinn ;
   
void setup() {
  Serial.begin(115200);

  tinn = xtbuild(2, 2, 2);
  for(int i=0; i < tinn.nb; i++)
    tinn.b[i] = biases[i];
  for(int i=0; i < tinn.nw; i++)
    tinn.w[i] = weights[i];
}

void loop() {
  float in[2] = { 0.01, 0.95 };
  float *out = xtpredict(tinn,in);
  Serial.println(out[0], DEC);
  Serial.println(out[1], DEC);
  delay(10000);
}