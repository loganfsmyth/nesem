

#include <string>
#include <iostream>
using namespace std;

#include "cpu.hpp"
#include "gpu.hpp"
#include "mapper.hpp"
#include "window.hpp"


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
  m.setRom(rom);
  m.setGPU(gpu);
  m.setCPU(cpu);

  cpu.reset();
  do {
    steps = cpu.step();

    gpu.step(steps);

    if (gpu.interrupt_requested()) cpu.raise_interrupt();
    else cpu.drop_interrupt();
  } while (!win.check_exit());

  return 0;
}
