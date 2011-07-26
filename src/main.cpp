

#include <string>
#include <iostream>
using namespace std;

#include "cpu.hpp"
#include "gpu.hpp"
#include "mapper.hpp"
#include "window.hpp"
#include "input.hpp"

int main(int argc, char** argv) {

  int steps;

  std::string filename("roms/PONG.NES");
//  std::string filename("roms/MarioBro.nes");

  cout << "AW" << endl;
  Rom rom(filename);

  rom.print_stats();

  Mapper m;

  CPU cpu(m);
  GPU gpu(m);

  Window win(gpu, 512, 512);
  Input in0(0);
  Input in1(1);

  m.setRom(rom);
  m.setGPU(gpu);
  m.setCPU(cpu);
  m.setInput(in0, 0);
  m.setInput(in1, 1);

  cpu.reset();
  

  int count = 0;

//  boost::posix_time::ptime start_time = boost::posix_time::microsec_clock::local_time();
  do {
    steps = cpu.step();
    gpu.step(steps);

    if (gpu.interrupt_requested()) {
      cpu.raise_interrupt();
    }
    else {
      cpu.drop_interrupt();
    }

    count += steps;
    if (count > 200000) count = 0;

  } while ((count != 0) || !win.check_exit());  // Check for window close every frame or so

  return 0;
}
