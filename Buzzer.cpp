#include "Buzzer.h"

Buzzer::Buzzer(byte pin) {
    this->pin = pin;
    this->prev_status = Low;
    init();
}
void Buzzer::toMid() {
    tone(pin, 500);
    delay(500);
    noTone(pin);
    delay(500);

    tone(pin, 500);
    delay(500);
    noTone(pin);
    delay(500);

    tone(pin, 500);
    delay(500);
    noTone(pin);
    delay(500);
}

void Buzzer::toHigh() {
    tone(pin, 600);
    delay(200);
    noTone(pin);
    delay(200);
    tone(pin, 600);
    delay(200);
    noTone(pin);
    delay(400);

    tone(pin, 600);
    delay(200);
    noTone(pin);
    delay(200);
    tone(pin, 600);
    delay(200);
    noTone(pin);
    delay(400);

    tone(pin, 600);
    delay(200);
    noTone(pin);
    delay(200);
    tone(pin, 600);
    delay(200);
    noTone(pin);
    delay(400);
}
void Buzzer::init() {
    pinMode(pin, OUTPUT);
}
void Buzzer::on(Status status) {
    if (prev_status == Low && status == Mid) {
        toMid();
        prev_status = Mid;
    }
    else if (prev_status == Mid && status == High) {
        toHigh();
        prev_status = High;
    }
    else if (status == Low)
        prev_status = Low;
}