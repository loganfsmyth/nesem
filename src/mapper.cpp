
#include <cstring>

#include "mapper.hpp"

#include "rom.hpp"
#include "gpu.hpp"
#include "cpu.hpp"

Mapper::Mapper() {
  ram = new uint8_t[0x800];
  sram = new uint8_t[0x2000];
  gpu = NULL;
  cpu = NULL;
}

Mapper::~Mapper() {
  delete[] sram;
  delete[] ram;
}

void Mapper::setRom(Rom &r) {
  rom = &r;

  bank1 = rom->prg(0);
  bank2 = rom->prg(1);

  vram = rom->chr(0);
}
void Mapper::setGPU(GPU &g) {
  gpu = &g;
}
void Mapper::setCPU(CPU &c) {
  cpu = &c;
}

uint8_t Mapper::read(uint16_t addr) {

  switch (addr&0xE000) {

    // RAM: [0x0000 - 0x2000)
    case 0x0000:
      return ram[addr&0x7FF];

    // GPU: [0x2000 - 0x4000)
    case 0x2000:
      return gpu->read(addr & 0x7);

    // Expansion + IO: [0x4000 - 0x6000)
    case 0x4000:
      if (addr < 0x4020) {

      }
      else {

      }
      return 0;

    // SRAM: [0x6000 - 0x8000)
    case 0x6000:
      return sram[addr & 0x3FFF];

    // Bank 1: [0x8000 - 0xC000)
    case 0x8000:
    case 0xA000:
      return bank1[addr & 0x3FFF];

    // Bank 2: [0xC000 - 0x10000)
    case 0xC000:
    case 0xE000:
      return bank2[addr & 0x3FFF];
  }
}

void Mapper::write(uint16_t addr, uint8_t value) {

  // RAM
  if (addr >= 0x0000 && addr < 0x2000) {
    ram[addr & 0x7FF] = value;
  }

  // GPU Register access
  else if (addr >= 0x2000 && addr < 0x4000) {
    gpu->write(addr & 0x7, value);
  }

  // Non-GPU Register access
  else if (addr >= 0x4000 && addr < 0x4020) {

    // DMA from PRG-RAM to SPR-RAM
    if (addr == 0x4014) {
      uint8_t* src;
      value *= 0x100;

      // From RAM
      if (addr < 0x20) {
        src = &ram[value];
      }

      // From SRAM
      else if (addr < 0x80) {
        src = &sram[value & 0x1F00];
      }

      else if (addr < 0xC0) {
        src = &bank1[value & 0x3F00];
      }

      else {
        src = &bank2[value & 0x3F00];
      }

      memcpy(gpu->sprite_ram, src, 0x100);
    }
  }

  // Expansion ROM
  else if (addr >= 0x4020 && addr < 0x6000) {

  }

  // SRAM
  else if (addr >= 0x6000 && addr < 0x8000) {
    sram[addr & 0x1FFF] = value;
  }

  // PRG-ROM Bank 1
  else if (addr >= 0x8000 && addr < 0xC000) {
    bank1[addr & 0x3FFF] = value;
  }

  // PRG-ROM Bank 2
  else if (addr >= 0xC000 && addr < 0x10000) {
    bank2[addr & 0x3FFF] = value;
  }

}
