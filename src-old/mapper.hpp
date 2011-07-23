#ifndef __INCLUDE_MAPPER_HPP
#define __INCLUDE_MAPPER_HPP

#include <boost/smart_ptr.hpp>
#include "rom.hpp"
#include "bus.hpp"

class Mapper : public BusItem{
  private:
    boost::shared_ptr<Rom> rom;
    
  public:
    Mapper(boost::shared_ptr<Rom> r);
    
    uint8_t bus_read(uint16_t addr);
    void bus_write(uint16_t addr, uint8_t val);
  
    int vram_read(int addr);
    void vram_write(int addr, int val);
    
  
};

#endif
