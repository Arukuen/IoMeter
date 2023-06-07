#include "RGB.h"


RGB::RGB(byte pin_red, byte pin_green, byte pin_blue) {
    this->pin_red = pin_red;
    this->pin_green = pin_green;
    this->pin_blue = pin_blue;
    init();
}
void RGB::green() {
    analogWrite(pin_red,   0);
    analogWrite(pin_green, 255);
    analogWrite(pin_blue,  0);
}
void RGB::yellow() {
    analogWrite(pin_red,   255);
    analogWrite(pin_green, 63);
    analogWrite(pin_blue,  0);
}
void RGB::red() {
    analogWrite(pin_red,   255);
    analogWrite(pin_green, 0);
    analogWrite(pin_blue,  0);
}
void RGB::init() {
    pinMode(pin_red, OUTPUT);
    pinMode(pin_green, OUTPUT);
    pinMode(pin_blue, OUTPUT);
}
void RGB::on(Status status) {
    switch (status) {
        case Low:
            green();
            break;
        case Mid:
            yellow();
            break;
        case High:
            red();
            break;
    }
}
void RGB::off() {
    analogWrite(pin_red,   0);
    analogWrite(pin_green, 0);
    analogWrite(pin_blue,  0);
}
