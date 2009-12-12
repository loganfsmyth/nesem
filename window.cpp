#include "Window.hpp"

using namespace std;

Window::Window(int width, int height){

	if( SDL_Init(SDL_INIT_VIDEO) < 0){
		cout << "Unable to init SDL: " << SDL_GetError() << endl;
		return;
	}
	screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
	if ( screen == NULL ){
		cout << "Unable to set resolution: " << SDL_GetError() << endl;
		return;
	}

    cout << "OMG" << endl;

    atexit(SDL_Quit);


}
Window::~Window(){

	unsigned int i;
	for(i = 0; i < hooks.size(); i++){
		hooks[i]();
	}

}
void Window::registerExitHook(void (*hook_ptr)()){
	hooks.push_back(hook_ptr);
}
void Window::registerLoopHook(void (*hook_ptr)()){
	loop_hooks.push_back(hook_ptr);
}

void Window::executeLoopHooks(){
    int i;
    for(i = 0; i < loop_hooks.size(); i++){
        loop_hooks[i]();
    }
}

void Window::lock(){
	if ( SDL_MUSTLOCK(screen) ){
		if ( SDL_LockSurface(screen) < 0 ){
			return;
		}
	}
}

void Window::unlock(){
	if ( SDL_MUSTLOCK(screen) ){
		SDL_UnlockSurface(screen);
	}
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

void Window::flip(){
	SDL_Flip(screen);
}


void Window::main_loop(){

    // program main loop
    bool done = false;
    while (!done)
    {
        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // check for messages
            switch (event.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                done = true;
                break;

                // check for keypresses
            case SDL_KEYDOWN:
                {
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    break;
                }
            } // end switch
        } // end of message processing

        executeLoopHooks();

        // clear screen
//        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

  //      SDL_Flip(screen);
    } // end main loop

}
