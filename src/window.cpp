#include "window.hpp"

#include <boost/bind.hpp>
#include <iostream>

using namespace std;

Window::Window(GPU &gpu, int width, int height){

  gpu.vblank_callback = boost::bind(&Window::gpuData, this, _1);

  last_frame = boost::posix_time::microsec_clock::local_time();

  if( SDL_Init(SDL_INIT_VIDEO) < 0){
    cout << "Unable to init SDL: " << SDL_GetError() << endl;
    return;
  }
  screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE);
  if ( screen == NULL ){
    cout << "Unable to set resolution: " << SDL_GetError() << endl;
    return;
  }

  atexit(SDL_Quit);


}
Window::~Window(){

}

void Window::gpuData(color* data) {



  if ( SDL_MUSTLOCK(screen) ){
    if ( SDL_LockSurface(screen) < 0 ){
      return;
    }
  }

  for (int y = 0; y < 240; y++) {
    for (int x = 0; x < 256; x++) {
      color* c = &data[y*256 + x];
      DrawPixel(2*x, 2*y, c->r, c->g, c->b);
      DrawPixel(2*x + 1, 2*y, c->r, c->g, c->b);
      DrawPixel(2*x, 2*y + 1, c->r, c->g, c->b);
      DrawPixel(2*x + 1, 2*y + 1, c->r, c->g, c->b);
    }
  }

  if ( SDL_MUSTLOCK(screen) ){
    SDL_UnlockSurface(screen);
  }

  SDL_Flip(screen);

  boost::posix_time::ptime start = boost::posix_time::microsec_clock::local_time();

  cout << "FPS: " << (1000.0/(start - last_frame).total_milliseconds()) << endl;

  last_frame = start;
}

void Window::DrawPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B){
  Uint32 color = SDL_MapRGB(screen->format, R, G, B);

  Uint8 *bufp8;
  Uint16 *bufp16;
  Uint32 *bufp32;

  switch (screen->format->BytesPerPixel){
    case 1: // Assuming 8-bpp
      bufp8 = (Uint8 *)screen->pixels + y*screen->pitch + x;
      *bufp8 = color;
      break;

    case 2: // Probably 15-bpp or 16-bpp
      bufp16 = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
      *bufp16 = color;
      break;

    case 3: // Slow 24-bpp mode, usually not used
      bufp8 = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;
      if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
      {
        bufp8[0] = color;
        bufp8[1] = color >> 8;
        bufp8[2] = color >> 16;
      }
      else {
        bufp8[2] = color;
        bufp8[1] = color >> 8;
        bufp8[0] = color >> 16;
      }
      break;

    case 4: // Probably 32-bpp
      bufp32 = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
      *bufp32 = color;
      break;
    default:
      break;
  }
}


bool Window::check_exit() {

  //  message processing loop
  SDL_Event event;
  if(SDL_PollEvent(&event)){
      // check for messages
    switch (event.type){
      // exit if the window is closed
      case SDL_QUIT:
        return true;
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) return true;
  
    } // end switch
  } // end of message processing
  return false;
}
