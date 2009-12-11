using namespace std;

#include "Rom.h"
#include "Memory.h"
#include "CPU.h"
#include "Window.h"

bool nesem_exit_loop;

void nesem_exit();

int main( int argc, char** argv){
	nesem_exit_loop = false;
	
	Rom r;
//	r.load("MarioBro.nes");
//	r.load("overtest.nes");
//	r.load("NEStress.NES");
//	r.load("siudym.nes");
	r.load("PONG.NES");
//	r.print_stats();
	
	Memory mem;
	mem.load(&r);
	
	GPU gpu;
	gpu.setCHRRom(&r);
	mem.setGPU(&gpu);
	
	
	Window win(512, 480);
	gpu.setWindow(&win);
//	win.registerExitHook(&nesem_exit);
	
	
	CPU cpu;
	cpu.setMemory(&mem);
	
	
//	long m_lLastTick = SDL_GetTicks();
	long counter = 0;
	while(!nesem_exit_loop){
//		long iElapsedTicks = SDL_GetTicks() - m_lLastTick;
//		m_lLastTick = SDL_GetTicks();

//		int inst = iElapsedTicks*1790;
		
//		int i = 0;
//		while(i < inst) i += cpu.executeInst();
//		int j = 0;
//		while(j++ < i*3) gpu.executeCycle();
		
		
		int i = 1;
		i= cpu.executeInst();
		
		counter += i;
		int j = 0;
		while(j++ < i*3){
			if(gpu.executeCycle()){
				cpu.raise_interrupt();
			}
			else cpu.drop_interrupt();
		}
		
		
		if(counter % 1000000 == 0) cout <<"Count: " << counter << endl;
	}
	
	char c;
	cin >> c;
}

void nesem_exit(){
	nesem_exit_loop = true;
}