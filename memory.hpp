#ifndef __INLCUDE_MEMORY_HPP
#define __INLCUDE_MEMORY_HPP

#include <boost/shared_ptr.hpp>
#include "bus.hpp"
#include <iostream>

class Memory : public BusItem{
  private:
    uint8_t bus_read(uint16_t addr);
    void bus_write(uint16_t addr, uint8_t val);
  
    boost::scoped_array<uint8_t> data;
    uint16_t size;
  public:
    Memory(uint16_t size);
};

#endif
