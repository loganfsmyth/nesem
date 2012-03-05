

#include <string>
#include <iostream>
using namespace std;

#include "cpu.hpp"
#include "gpu.hpp"
#include "mapper.hpp"
#include "window.hpp"
#include "input.hpp"

#define FRAMERATE   5

int main(int argc, char** argv) {

  if (argc != 2) return 1;

  int steps;
  float extratime;
  long cputime;

  std::string filename(argv[1]);

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

  boost::posix_time::ptime frame_start = boost::posix_time::microsec_clock::local_time();
  do {
    steps = cpu.step();
    gpu.step(3*steps);

    if (gpu.interrupt_requested()) {
      cpu.raise_interrupt();
    }
    else {
      cpu.drop_interrupt();
    }

    count += 3*steps;

    if (count > GPU_CYCLES_PER_FRAME) {
      boost::posix_time::ptime frame_end = boost::posix_time::microsec_clock::local_time();

      extratime = ((1000000L/FRAMERATE) - (frame_end - frame_start).total_microseconds());

      frame_start = boost::posix_time::microsec_clock::local_time();
      count = 0;
    }

  } while ((count != 0) || !win.check_exit());  // Check for window close every frame or so

  return 0;
}
