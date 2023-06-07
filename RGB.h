#ifndef RGB_H
#define RGB_H

#include <Arduino.h>
#include "Status.h"

class RGB {
  private:
    byte pin_red;
    byte pin_green;
    byte pin_blue;
    void green();
    void yellow();
    void red();
    
  public:
    RGB(byte pin_red, byte pin_green, byte pin_blue);
    void init();
    void on(Status status);
    void off();
};

#endif