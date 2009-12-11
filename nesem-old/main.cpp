#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif


#include "Rom.hpp"
#include "Memory.hpp"
#include "CPU.hpp"
#include "Window.hpp"


Rom *r;
Memory *mem;
GPU *gpu;
Window *win;
CPU *cpu;


void main_loop();

int main ( int argc, char** argv ){

	r = new Rom();
//	r->load("MarioBro.nes");
	r->load("overtest.nes");
//	r->load("NEStress.NES");
//	r->load("siudym.nes");
//	r->load("PONG.NES");
	r->print_stats();

    mem = new Memory();
	mem->load(r);

	gpu = new GPU();
	gpu->setCHRRom(r);
	mem->setGPU(gpu);


	win = new Window(512, 480);
	gpu->setWindow(win);
//	win->registerExitHook(&nesem_exit);


	cpu = new CPU();
	cpu->setMemory(mem);


    win->registerLoopHook(&main_loop);


    win->main_loop();



    delete cpu;
    delete win;
    delete gpu;
    delete mem;
    delete r;

    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}


void main_loop(){


		int i = 1;
		i = cpu->executeInst();

		int j = 0;
		while(j++ < i*3){
			if(gpu->executeCycle()){
				cpu->raise_interrupt();
			}
			else cpu->drop_interrupt();
		}



}
