// joystick-array.ino
// mixing ideas from...
//        https://www.prometec.net/8x8-max7219/
//        https://www.luisllamas.es/arduino-joystick/

#include "LedControlMS.h"

const unsigned DataIn = 12, // pin 12
               CLK = 11,    // pin 11
               LOAD = 10,   // pin 10
               SINGLE = 1,  // only a single max72xx
               ADDR = 0,    // max72xx address
               BUTTOM = 4,  // joystick buttom conected to pin 4
               COLS = 8,    // number of columns
               ROWS = 8,    // number of rows
               DELAY = 64;  // delay in ms

LedControl lc = LedControl(DataIn, CLK, LOAD, SINGLE);
unsigned x = 4, y = 4;

void setup()
{
  lc.shutdown(ADDR, false);      // power on max72xx
  lc.setIntensity(ADDR, 2);      // intensity in [0, 15]
  lc.clearDisplay(ADDR);         // clear display
  
  pinMode(BUTTOM, INPUT_PULLUP); // pull-up resistance activation
}

void boom()
{
  while (digitalRead(BUTTOM) == LOW)
    for (unsigned x = 0; x < COLS; ++x)  
      for (unsigned y = 0; y < ROWS; ++y)
        lc.setLed(ADDR, x, y, HIGH);
  lc.clearDisplay(ADDR);
}

void loop()
{
  unsigned old_x = x, old_y = y;
    
  x = constrain(x + map(analogRead(A1), 0, 1024, -1, 2), 0, COLS - 1);
  delay(2);  // small pause between analog reads

  y = constrain(y + map(analogRead(A0), 0, 1024, -1, 2), 0, ROWS - 1);
  delay(2);  // small pause between analog reads

  if (x != old_x || y != old_y)
    lc.setLed(ADDR, old_x, old_y, LOW); // clear only if <x,y> changes
    
  lc.setLed(ADDR, x, y, HIGH); // activate <x,y> led
  delay(DELAY);                // for some time

  if (digitalRead(BUTTOM) == LOW) // has buttom been clicked?
    boom();                       // then do something
}
