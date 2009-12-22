#include "memory.hpp"

using namespace std;

Memory::Memory(uint16_t size) : data(new uint8_t[size]){
  this->size = size;
}

uint8_t Memory::bus_read(uint16_t addr){
  if(addr < size) 
    return data[addr];
  else
    return 0;
}

void Memory::bus_write(uint16_t addr, uint8_t val) {
  if(addr < size) data[addr] = val;
}
