#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>
#include "Status.h"

class Buzzer {
    private:
    byte pin;
    byte prev_status;
    void toMid();
    void toHigh();
    
  public:
    Buzzer(byte pin);
    void init();
    void on(Status status);
};

#endif