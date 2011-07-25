#ifndef INCLUDE_WINDOW_H
#define INCLUDE_WINDOW_H

#include <SDL/SDL.h>
#include <iostream>
#include <vector>

#include <boost/date_time/posix_time/posix_time_types.hpp>

#include "gpu.hpp"


typedef void (*hook_ptr)();

class Window{

  public:
    Window(GPU &gpu, int width, int height);
    ~Window();

    void DrawPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);

    void gpuData(color* data);

    bool check_exit();
  private:

    SDL_Surface* screen;

    boost::posix_time::ptime last_frame;

};


#endif
