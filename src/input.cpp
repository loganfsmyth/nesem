
#include "input.hpp"

#include <iostream>
using namespace std;

Input::Input(int controller) {
  key_state = SDL_GetKeyState(&numkeys);

  if (controller == 0) {
    up = SDLK_w;
    down = SDLK_s;
    left = SDLK_a;
    right = SDLK_d;

    abutton = SDLK_SEMICOLON;
    bbutton = SDLK_QUOTE;
    select = SDLK_RSHIFT;
    start = SDLK_RETURN;
  }
  else {
    up = SDLK_UP;
    down = SDLK_DOWN;
    left = SDLK_LEFT;
    right = SDLK_RIGHT;

    abutton = SDLK_KP_ENTER;
    bbutton = SDLK_KP_PERIOD;
    select = SDLK_KP_MINUS;
    start = SDLK_KP_PLUS;
  }
}

uint8_t Input::read() {
  uint8_t value = pressed & 0x01;
  pressed >>= 1;

  return value;
}
void Input::write(uint8_t value) {
  if (value & 0x01) {
    pressed = (key_state[abutton] << 0) | (key_state[bbutton] << 1) | (key_state[select] << 2) | (key_state[start] << 3) |
              (key_state[up] << 4) | (key_state[down] << 5) | (key_state[left] << 6) | (key_state[right] << 7);
  }
}

