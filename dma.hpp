#ifndef __INLCUDE_DMA_HPP
#define __INLCUDE_DMA_HPP

#include <boost/shared_ptr.hpp>
#include "bus.hpp"
#include <iostream>

class DMA : public BusItem{
  private:
    uint8_t bus_read(uint16_t addr);
    void bus_write(uint16_t addr, uint8_t val);
  
    boost::shared_ptr<Bus> bus;
  public:
    DMA(boost::shared_ptr<Bus> b);
};

#endif
