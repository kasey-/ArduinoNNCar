#include "Tinn.h"
#include <stdio.h>

int main() {
   float in[] = { 0.05, 0.10 };
   float tg[] = { 0.01, 0.99 };
    Tinn tinn = xtbuild(2, 2, 2);

    /* Train */
    float error;
    for(int i = 0; i < 5000; i++)
        error = xttrain(tinn, in, tg, 0.5);
    printf("Error   : %f\n",error);

    /* Display biases */
    printf("Biases  : ");
    for (int i = 0; i < tinn.nb; i++)
        printf("%f, ", tinn.b[i]);
    printf("\n");

    /* Display weights */
    printf("Weights : ");
    for (int i = 0; i < tinn.nw; i++)
        printf("%f, ", tinn.w[i]);
    printf("\n");

    /* Display result */
    float *output = xtpredict(tinn,in);
    printf("Output  : %f %f\n",output[0],output[1]);

    return 0;
}
