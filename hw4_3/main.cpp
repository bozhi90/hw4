#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

Ticker servo_ticker;
Ticker encoder_ticker;
Timer t;

PwmOut pin5(D5), pin6(D6);
DigitalInOut ping(D10);
DigitalIn encoder(D11);
BufferedSerial uart(D1,D0);     //tx,rx
BufferedSerial pc(USBTX,USBRX); //tx,rx
BBCar car(pin5, pin6, servo_ticker);

volatile int steps;
volatile int last;
char lr = '0';

Thread thread_ping;

void encoder_control() {
    int value = encoder;
    if (!last && value) steps++;
    last = value;
}

void function_ping() {
    float val;

    while(1) {
        ping.output();
        ping = 0; wait_us(200);
        ping = 1; wait_us(5);
        ping = 0; wait_us(5);

        ping.input();
        while(ping.read() == 0);
        t.start();
        while(ping.read() == 1);
        val = t.read();
        printf("Ping = %lf\r\n", val*17700.4f);
        t.stop();
        t.reset();

        ThisThread::sleep_for(1s);
    }
}

int main() {
    uart.set_baud(9600);
    thread_ping.start(function_ping);

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