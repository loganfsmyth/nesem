
#ifndef __INCLUDE_INPUT_H
#define __INCLUDE_INPUT_H

#include <SDL/SDL.h>
#include <boost/cstdint.hpp>

class Input {
  private:
    Uint8* key_state;
    int numkeys;

    int up, down, left, right, abutton, bbutton, select, start;
    uint8_t pressed;

  public:
    Input(int controller);

    uint8_t read();
    void write(uint8_t value);

};


#endif
