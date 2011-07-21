
#include <SDL/SDL.h>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

#include "mapper.hpp"
#include "rom.hpp"
#include "dma.hpp"
#include "memory.hpp"
#include "cpu.hpp"
#include "gpu.hpp"
#include "window.hpp"
#include "bus.hpp"

#define NESEM_DEFAULT_FPS 30

using namespace std;


bool process_args(int argc, char** argv, string &filename);
void main_loop();

int main ( int argc, char** argv ){
  string filename = "roms/PONG.NES";
  
  //~ if(!process_args(argc, argv, filename)) return 1;
  
  boost::shared_ptr<Window> win(new Window(512,480));
  boost::shared_ptr<Bus> bus(new Bus());
  
  boost::shared_ptr<Rom> r(new Rom());
  r->load(filename);
  r->print_stats();
  
  boost::shared_ptr<Mapper> map(new Mapper(r));
  boost::shared_ptr<Memory> ram(new Memory(0x2000));
  
  boost::shared_ptr<GPU> gpu(new GPU(map, win));
  boost::shared_ptr<DMA> dma(new DMA(bus));
//  boost::shared_ptr<Input> controller1(new Input());
  
  bus->register_item(boost::dynamic_pointer_cast<BusItem>(ram), 0x0, 0x1FFF);
  bus->register_item(boost::dynamic_pointer_cast<BusItem>(gpu), 0x2000, 0x3FFF);
  bus->register_item(boost::dynamic_pointer_cast<BusItem>(dma), 0x4014, 0x4014);
  bus->register_item(boost::dynamic_pointer_cast<BusItem>(map), 0x8000, 0xFFFF);
  //~ bus->register_item(boost::dynamic_pointer_cast<BusItem>(controller1), 0x4000, 0x4000);
  
  boost::shared_ptr<CPU> cpu(new CPU(bus));

  cpu->reset();
  
  //~ bus->write(0x2006, 0x20);
  //~ bus->write(0x2006, 0x00);
  
  //~ for(int i = 0; i < 0x3C0; i++) {
    
    //~ bus->write(0x2007, i);
    
  //~ }
  
  //~ bus->write(0x2006, 0x00);
  //~ bus->write(0x2006, 0x00);
  
  do {
    int time = cpu->executeInst();
    //~ int time = 1;
    for(int i = 0; i < 3*time; i++ ){
      gpu->executeCycle();
    }
    
    if(gpu->interrupt_requested()) cpu->raise_interrupt();
    else cpu->drop_interrupt();
    //~ boost::this_thread::sleep(boost::posix_time::milliseconds(10));
  } while(!win->check_exit());

  // all is well ;)
  printf("Exited cleanly\n");
  return 0;
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