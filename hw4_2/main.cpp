#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

Ticker servo_ticker;
Ticker encoder_ticker;
Timer t;

PwmOut pin5(D5), pin6(D6);
DigitalIn encoder(D11);
BufferedSerial uart(D1,D0);     //tx,rx
BufferedSerial pc(USBTX,USBRX); //tx,rx
BBCar car(pin5, pin6, servo_ticker);

volatile int steps;
volatile int last;
char lr = '0';

int main() {
    uart.set_baud(9600);

    while(1){
        if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            lr = recv[0];
            //printf("%c", lr);
        }
        if (lr == '0') {
            car.go(100, 100);
            ThisThread::sleep_for(10ms);
            car.stop();
            //printf("go\n");
        }
        else if (lr == '1') {
            car.go(50, 100);
            ThisThread::sleep_for(10ms);
            car.stop();
            //printf("right\n");
        }
        else if (lr == '2') {
            car.go(100, 50); 
            ThisThread::sleep_for(10ms); 
            car.stop();
            //printf("left\n");
        }
    }
}