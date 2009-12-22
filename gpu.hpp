#ifndef INCLUDE_GPU_H
#define INCLUDE_GPU_H

#include <cstdint>
#include <iostream>
#include <boost/smart_ptr.hpp>
//~ #include "window.hpp"
#include "rom.hpp"
#include "bus.hpp"
#include "mapper.hpp"


#define GPU_REG_CTRL		0
#define GPU_REG_MASK		1
#define GPU_REG_STATUS		2
#define GPU_REG_OAM_ADDR	3
#define GPU_REG_OAM_DATA	4
#define GPU_REG_SCROLL		5
#define GPU_REG_ADDR		6
#define GPU_REG_DATA		7

struct color{
  uint8_t r;
  uint8_t g;
  uint8_t b;
};


class GPU : public BusItem{

  public:
    GPU(boost::shared_ptr<Mapper> m);
    ~GPU();


    uint8_t bus_read(uint16_t location);
    void bus_write(uint16_t location, uint8_t data);

    int executeCycle();
    void display();
    void display_full();
    void printPT(int y);
    void printNT(int y);

    void render_sprites();

  private:
    boost::shared_ptr<Mapper> map;
    
    //~ uint8_t pattern_table[0x2000];
    uint8_t name_attrib_table[0x1000];
    uint8_t palette_table[0x20];

    uint8_t sprite_ram[0x100];

    uint8_t reg_ctrl;
    uint8_t reg_mask;
    uint8_t reg_status;

    uint8_t reg_oam_addr;

    bool addr_toggle;
    uint16_t vram_address_reg;
    uint16_t vram_address_temp;

    uint8_t vram_read_delay;

    uint8_t offset_x;

    //~ boost::shared_ptr<Window> win;
    //~ boost::shared_ptr<Rom> rom;

    uint8_t pix_count;

    int wait_frame;
    int cycle, scanline;

    color paletteLookup(int num);
    
    uint8_t mem_read(uint16_t location);
    void mem_write(uint16_t location, uint8_t data);

};

#endif
