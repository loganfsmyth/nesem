#include "bus.hpp"

using namespace std;

void Bus::write(uint16_t addr, uint8_t val) {
  for(list<BusObj>::iterator ci = items.begin(); ci != items.end(); ci++) {
    if(addr >= ci->range.first && addr <= ci->range.second) {
      ci->item->bus_write(addr, val);
    }
  }
}
uint8_t Bus::read(uint16_t addr){
  for(list<BusObj>::iterator ci = items.begin(); ci != items.end(); ci++) {
    if(addr >= ci->range.first && addr <= ci->range.second) {
      uint8_t temp = ci->item->bus_read(addr);
      //~ cout << hex << "BUS: Read 0x" << (int)temp << " from range (0x" << ci->range.first << ", 0x" << ci->range.second << ") at 0x" << (int)addr << dec << endl;
      return temp;
    }
  }
  return 0;
}

void Bus::register_item(boost::shared_ptr<BusItem> it, int start, int end) {
  BusObj obj;
  
  obj.range.first = start;
  obj.range.second = end;
  obj.item = it;
  
  items.push_back(obj);
}