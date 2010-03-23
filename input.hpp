#ifndef __INLCUDE_INPUT_HPP
#define __INLCUDE_INPUT_HPP

#include <boost/shared_ptr.hpp>
#include "bus.hpp"
#include <iostream>

class Input : public BusItem{
  private:
    uint8_t bus_read(uint16_t addr);
    void bus_write(uint16_t addr, uint8_t val);
  
    boost::shared_ptr<Bus> bus;
  public:
    Input(boost::shared_ptr<Bus> b);
};

#endif
