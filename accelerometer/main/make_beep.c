#include "accelerometer.h"

void make_beep_sound() {
    int arr[100];
    int p = 0;
    int t = 0;

    for (int i = 0; i < 100; ++i) {
        p = (i % 100);
        arr[i] = p < 50 ? 100 + p * 2 : p * 2 - 100; 
    }
    dac_output_enable(DAC_CHANNEL_1);
    while (t < 15) {
        for (int i = 0; i < 100; i++) {
            dac_output_voltage(DAC_CHANNEL_1, arr[i]);
        }
        usleep(2000);
        t++;
    }
}
