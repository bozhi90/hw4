#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

Ticker servo_ticker;
Ticker encoder_ticker;

PwmOut pin5(D5), pin6(D6);
DigitalIn encoder(D11);
BufferedSerial xbee(D1, D0);
BBCar car(pin5, pin6, servo_ticker);

volatile int steps;
volatile int last;

void encoder_control() {
    int value = encoder;
    if (!last && value) steps++;
    last = value;
}

int main() {
    char buf[256], outbuf[256];
    FILE *devin = fdopen(&xbee, "r");
    FILE *devout = fdopen(&xbee, "w");
    while (1) {
        memset(buf, 0, 256);
        for( int i = 0; ; i++ ) {
            char recv = fgetc(devin);
            if(recv == '\n') {
                printf("\r\n");
                break;
            }
            buf[i] = fputc(recv, devout);
        }
    RPC::call(buf, outbuf);
    }
}

void Go_Straight (Arguments *in, Reply *out) {
    int dist = in->getArg<double>();

    encoder_ticker.attach(&encoder_control, 1ms);
    steps = 0;
    last = 0;
    car.goStraight(100);
    while(steps*6.5*3.14/32 < dist) {
        ThisThread::sleep_for(10ms);
    }
    car.stop();
}
RPCFunction go_Straight(&Go_Straight, "Go_Straight");

void Go_Turn (Arguments *in, Reply *out) {
    int ang = in->getArg<double>();
    car.turn(100, 0);
    for (int i = 0; i < ang; i++) {
        ThisThread::sleep_for(75ms);
    }
    car.stop();
}
RPCFunction go_Turn(&Go_Turn, "Go_Turn");