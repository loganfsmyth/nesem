#ifndef INCLUDE_WINDOW_H
#define INCLUDE_WINDOW_H

#include <SDL/SDL.h>
#include <iostream>
#include <vector>

typedef void (*hook_ptr)();

class Window{

  public:
    Window(int width, int height);
    ~Window();

    void registerExitHook(void (*hook_ptr)());
    void registerLoopHook(void (*hook_ptr)());

    void executeLoopHooks();

    void DrawPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);

    void lock();
    void unlock();
    void flip();

    bool check_exit();
  private:

    SDL_Surface* screen;
    std::vector<hook_ptr> hooks;
    std::vector<hook_ptr> loop_hooks;

};


#endif
