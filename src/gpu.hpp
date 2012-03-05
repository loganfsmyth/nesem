#ifndef INCLUDE_GPU_H
#define INCLUDE_GPU_H

#include <boost/cstdint.hpp>
#include <boost/function.hpp>
#include <iostream>
#include "rom.hpp"
#include "mapper.hpp"

#define VRAM_BUFFER_SIZE  0x10000 // 256*256

#define GPU_REG_CTRL      0x0
#define GPU_REG_MASK      0x1
#define GPU_REG_STATUS    0x2
#define GPU_REG_OAM_ADDR  0x3
#define GPU_REG_OAM_DATA  0x4
#define GPU_REG_SCROLL    0x5
#define GPU_REG_ADDR      0x6
#define GPU_REG_DATA      0x7

#define GPU_CYCLES_PER_FRAME  (341 * 262)

struct color{
  uint8_t r;
  uint8_t g;
  uint8_t b;
};


class GPU {
  public:
    GPU(Mapper &m);
    ~GPU();

    bool interrupt_requested();

    int executeCycle();
    int get_pixel_color();
    void printPT(int y);
    void printNT(int y);

    void render_sprites();

    uint8_t read(uint8_t reg);
    void    write(uint8_t reg, uint8_t val);

    int step(int steps);
    uint8_t sprite_ram[0x100];

    boost::function<void(color*)> vblank_callback;

  private:
    Mapper* map;

    color* vbuffer;

    //~ uint8_t pattern_table[0x2000];
    uint8_t name_attrib_table[0x1000];
    uint8_t palette_table[0x20];



    uint8_t reg_ctrl;
    uint8_t reg_mask;
    uint8_t reg_status;

    uint8_t reg_oam_addr;

    bool addr_toggle;
    uint16_t vram_address_reg;
    uint16_t vram_address_temp;

    uint8_t vram_read_delay;

    uint8_t offset_x;

    uint8_t pix_count;

    int wait_frame;
    int cycle, scanline;

    color paletteLookup(int num);

    

};

#endif
