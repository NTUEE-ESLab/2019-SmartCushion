#include <mbed.h>
#include "FSR.h"
#include <stdio.h>
FSR fsr1(A0, 10); // Pin 20 is used as the AnalogIn pin and a 10k resistor is used as a voltage divider
FSR fsr2(A1, 10);
Serial pc(USBTX, USBRX);
int main(){
    pc.baud(115200);
    printf("Hello~\r\n");
    while (1)
    {
        printf("******----******\n");
        printf("The raw data is %f\n", fsr1.readRaw());
        printf("The resistance of the FSR is %f\n", fsr1.readFSRResistance());
        printf("The weight on the FSR is %f\n\n", fsr1.readWeight());
        printf("The raw data is %f\n", fsr2.readRaw());
        printf("----\n");
        printf("The resistance of the FSR is %f\n", fsr2.readFSRResistance());
        printf("The weight on the FSR is %f\n\n", fsr2.readWeight());
        wait(1); //just here to slow down the output for easier reading
    }
}