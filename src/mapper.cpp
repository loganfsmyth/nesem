
#include "mapper.hpp"

Mapper::Mapper(boost::shared_ptr<Rom> r) {
  rom = r;
  
}

uint8_t Mapper::bus_read(uint16_t addr) {
  return rom->prg_read(addr);
}
void Mapper::bus_write(uint16_t addr, uint8_t val) {
  rom->prg_write(addr, val);
}

int Mapper::vram_read(int addr) {
  return rom->chr_read(addr);
}
void Mapper::vram_write(int addr, int val) {
  rom->chr_write(addr, val);
}