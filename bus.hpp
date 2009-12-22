#ifndef __INCLUDE_BUS_HPP
#define __INCLUDE_BUS_HPP

#include <boost/smart_ptr.hpp>
#include <cstdint>
#include <list>
#include <iostream>

class BusItem {
  public:
    virtual void bus_write(uint16_t addr, uint8_t val) = 0;
    virtual uint8_t bus_read(uint16_t addr) = 0;
};

struct BusObj{
  std::pair<int,int> range;
  boost::shared_ptr<BusItem> item;
};

class Bus {
  private:
    std::list<BusObj> items;
    
  public:
    void write(uint16_t addr, uint8_t val);
    uint8_t read(uint16_t addr);
    void register_item(boost::shared_ptr<BusItem> it, int start, int end);
};

#endif
