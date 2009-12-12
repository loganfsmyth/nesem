
#include <SDL/SDL.h>

#include "rom.hpp"
#include "memory.hpp"
#include "cpu.hpp"
#include "window.hpp"


Rom *r;
Memory *mem;
GPU *gpu;
Window *win;
CPU *cpu;


void main_loop();

int main ( int argc, char** argv ){
  
  r = new Rom();
  r->load(argv[1]);
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
