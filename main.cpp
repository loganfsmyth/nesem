
#include <SDL/SDL.h>
#include <iostream>
#include <boost/program_options.hpp>

#include "rom.hpp"
#include "memory.hpp"
#include "cpu.hpp"
#include "window.hpp"

#define NESEM_DEFAULT_FPS 30

using namespace std;


Rom *r;
Memory *mem;
GPU *gpu;
Window *win;
CPU *cpu;

bool process_args(int argc, char** argv, string &filename);
void main_loop();

int main ( int argc, char** argv ){
  string filename;
  
  if(!process_args(argc, argv, filename)) return 1;
  
  
  r = new Rom();
  r->load(filename);
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

bool process_args(int argc, char** argv, string &filename) {
  
  namespace po = boost::program_options;
  
  
  po::options_description general("General options");
  general.add_options()
    ("version,v", "print version string")
    ("help,?", "display help message");
  
  po::options_description config("Config options");
  config.add_options()
    ("fps,f", po::value<int>()->default_value(NESEM_DEFAULT_FPS), "set frames per second maximum");
  
  po::options_description hidden("Hidden options");
  hidden.add_options()
    ("filename", po::value<string>(),"set  file name");
  
  po::options_description config_opts;
  config_opts.add(general).add(config).add(hidden);
  po::options_description visible_opts("NESEM Configuration options");
  visible_opts.add(general).add(config);
  
  po::positional_options_description p;
  p.add("filename", -1);
  
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(config_opts).positional(p).run(), vm);
  //~ po::store(po::command_line_parser(argc, argv).options(visible_opts).positional(p).run(), vm);
  po::notify(vm);
  
  if(vm.count("help")) {
    cout << visible_opts << "\n";
    return false;
  }
  else if(vm.count("filename")) {
    filename = vm["filename"].as<string>();
    cout << "Loading file " << filename << endl;
  }
  else {
    cout << "No filename specified" << endl << endl;
    return false;
  }
  
  return true;
}
