#ifndef __INCLUDE_MAPPER_H
#define __INCLUDE_MAPPER_H

#include <boost/cstdint.hpp>

class Rom;
class GPU;
class CPU;

class Mapper {
  private:
    uint8_t* ram;
    uint8_t* sram;
    uint8_t* bank1;
    uint8_t* bank2;

    Rom* rom;
    GPU* gpu;
    CPU* cpu;

  public:
    Mapper();
    ~Mapper();

    void setRom(Rom &r);
    void setGPU(GPU &g);
    void setCPU(CPU &c);
  

    // CPU Read/Write
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t value);

    // GPU Memory
    uint8_t* vram;
};

#endif
