
#include <cstring>
#include "gpu.hpp"


using namespace std;

#define GPU_MEM_MAP(location, var, GPU_FUNC)             \
  if (!(location & 0x2000)) {                            \
    GPU_FUNC(map->vram[location & 0x1FFF], var);         \
  }                                                      \
  else if( (location & 0x3F00) == 0x3F00) {              \
    GPU_FUNC(palette_table[location & 0x001F], var);     \
  }                                                      \
  else{                                                  \
    GPU_FUNC(name_attrib_table[location & 0x0FFF], var); \
  }

#define GPU_SET(ram, var) ram = var
#define GPU_GET(ram, var) var = ram
#define GPU_MEM_WRITE(location, var) GPU_MEM_MAP(location, var, GPU_SET)
#define GPU_MEM_READ(location, var) GPU_MEM_MAP(location, var, GPU_GET)


GPU::GPU(Mapper& m) : map(&m){
  reg_ctrl = 0;
  reg_mask = 0;
  reg_status = 0x80;
  wait_frame = 0;
  cycle = 0;
  scanline = 0;
  addr_toggle = false;

  vram_address_temp = 0x0000;
  vram_address_reg = 0x0000;

  for(int i = 0; i < 0x20; i++) palette_table[i] = i;

  for(int i = 0x03C0; i < 0x0400; i++){
    name_attrib_table[i] = 0x00;
  }

  for(int i = 0; i < 0x03C0; i++){
    name_attrib_table[i] = 0x00;
  }

  vbuffer = new color[VRAM_BUFFER_SIZE];
}


GPU::~GPU(){
  delete[] vbuffer;
}


bool GPU::interrupt_requested() {
  return ((reg_status&0x80) != 0) && (reg_ctrl & 0x80) == 0;
}


int GPU::step(int steps){
        //~ cout << "AHH";



  for (int i = 0; i < steps; i++) {

//        cout << "X:" << (int)cycle << " Y:" << (int)scanline << endl;
    if(wait_frame < 15){
      if(cycle == 341){
        if(scanline == 261){
          wait_frame++;
          scanline = 0;
        }
        else{ 
          scanline++;
        }
        cycle = 0;
      }
      else cycle++;

      return 0;
    }
    offset_x = cycle & 0x7;

    if(cycle < 256 && scanline > 0 && scanline < 241 && (reg_mask & 0x08) != 0 ){

      //~ cout << dec << "(" << cycle << ", " << scanline << ") " << hex << "Temp:0x" << (int)vram_address_temp << " Reg:0x" << (int)vram_address_reg << " - " << pix_color << dec << endl;
      //~ cout << "(" << cycle << ", " << (int)(scanline-1) << ") ";
      vbuffer[scanline*256 + cycle] = paletteLookup(get_pixel_color());
    }

    if(cycle == 341){
      if(scanline == 0){
        reg_status &= 0x7F;        //disable vblank flag

        vram_address_reg = vram_address_temp;
      }
      else if(scanline == 241){
        reg_status &= 0x7F;        //disable vblank flag
        reg_status |= 0x80;
        
        if((reg_mask&0x10) != 0) render_sprites();
        
        vblank_callback(vbuffer);

        //~ cout << "Set Flag 0x" << hex << (int)vram_address_temp << " 0x" << (int)vram_address_reg << dec << endl;

      }
      else if(scanline > 0 && scanline < 241){
        vram_address_reg &= 0xFBE0;        //reset bit 10 (The X NT) and bits 0-4(the x loc)
        vram_address_reg |= vram_address_reg&0x041F; //reset bits 0-4 and 10

        int vram_temp = vram_address_reg;

        if(((vram_address_reg >> 12) & 0x7) == 0x7){
          vram_address_reg &= 0x0C1F;        //clear bit 5-9 ( the y loc) and 12-15

          if((vram_temp&0x3E0) >> 5 == 29){        //if Y loc is 29
            vram_address_reg &= 0xF4EF;        //clear bit 11 and 5-9
            vram_address_reg |= ((vram_temp&0x0800) == 0)?0x800:0x0; //toggles bit 11
          }
          else vram_address_reg |= ((vram_temp&0x03E0)+0x0020) & 0x03E0;        //increment 5-9
        }
        else{
          vram_address_reg &= 0x0FFF; //clear 12-15
          vram_address_reg |= ((vram_temp&0x7000)+0x1000) & 0x7000;        //increment 12-14
        }
        
      }

      if(scanline == 261) scanline = 0;
      else scanline++;

      cycle = 0;
    }
    else{
      if(cycle < 256 && ((cycle&0x7) == 0x7) && (scanline > 0 && scanline < 241)){
        int vram_temp = vram_address_reg;

        if((vram_address_reg&0x1F) == 0x1F){
          vram_address_reg &= 0xFBFF;
          vram_address_reg |= ((vram_temp&0x0400) == 0)?0x400:0x0;
        }

        vram_address_reg &= 0xFFE0;
        vram_address_reg |= ((vram_temp&0x1F)+1) & 0x1F;
      }

      //~ offset_x = (offset_x + 1) & 0x7;
      cycle++;
    }

  }

  //~ cout << " OHH" << endl;
  
  //~ cout << " => 0x" << (int)vram_address_reg << dec << endl;
  
  return (reg_status & 0x80) != 0 && (reg_ctrl & 0x80) == 0;
}

uint8_t GPU::read(uint8_t location){
  uint8_t temp;

  switch(location){
    case GPU_REG_STATUS:
      temp = reg_status;
      reg_status &= 0x7F;
      addr_toggle = false;
      return temp;

    case GPU_REG_OAM_DATA:
      return sprite_ram[reg_oam_addr];

    case GPU_REG_DATA:
      if(location >= 0x3F00 && location < 0x4000) {
        GPU_MEM_READ(vram_address_reg, temp);
        return temp;
      }
      else {
        temp = vram_read_delay;
        GPU_MEM_READ(vram_address_reg, vram_read_delay);
        return temp;        //first read is discarded incorrect data
      }

    case GPU_REG_OAM_ADDR:
    case GPU_REG_CTRL:
    case GPU_REG_MASK:
    case GPU_REG_SCROLL:
    case GPU_REG_ADDR:
    default:
      return 0;
  }

  return 0;
}


void GPU::write(uint8_t location, uint8_t data){
  switch(location){
    case GPU_REG_CTRL:
      reg_ctrl = data;
      vram_address_temp &= 0xF3FF;
      vram_address_temp |= ((data & 0x03) << 10);
      break;
    case GPU_REG_MASK:
      reg_mask = data;
      break;
    case GPU_REG_STATUS:
      break;
    case GPU_REG_OAM_ADDR:
      reg_oam_addr = data;
//      cout << "Write SPR addr = 0x" <<  hex << (unsigned int)data << dec << endl;
      
      break;

    case GPU_REG_OAM_DATA:
      sprite_ram[reg_oam_addr] = data;
//      cout << "Write SPR data = 0x" <<  hex << (unsigned int)data << " at 0x" << (int)reg_oam_addr << dec << endl;
      
      reg_oam_addr++;
      break;

    case GPU_REG_SCROLL:
      if(addr_toggle == false){
        offset_x = data & 0x07;
        vram_address_temp &= 0xFFE0;
        vram_address_temp |= (data >> 3);
      }
      else{
        vram_address_temp &= 0x8C1F;
        vram_address_temp |= ((data & 0x07) << 12);
        vram_address_temp |= ((data & 0xF8) << 2);
      }
      addr_toggle = !addr_toggle;
      
//      cout << "Write Reg 5: 0x" << hex << (unsigned int)vram_address_temp << " :: 0x" <<  (unsigned int)data << dec << endl;
      break;
    case GPU_REG_ADDR:
      if(addr_toggle == false){
        vram_address_temp &= 0x00FF;
        vram_address_temp |= (((uint16_t)data & 0x3F) << 8);
      }
      else{
        vram_address_temp &= 0xFF00;
        vram_address_temp |= data;
        vram_address_reg = vram_address_temp;
      }

//      cout << "Write Reg 6: 0x" << hex << (unsigned int)vram_address_temp << " :: 0x" <<  (unsigned int)data << dec << endl;

      addr_toggle = !addr_toggle;
      break;
    case GPU_REG_DATA:
      GPU_MEM_WRITE(vram_address_reg, data)
      //~ cout << "Write: 0x" << hex << (unsigned int)vram_address_reg << " :: 0x" <<  (unsigned int)data << dec << endl;
//      cout << hex << "mem_write(0x" << (unsigned int)vram_address_reg << ",0x" << (unsigned int)data << ");" << dec << endl;
      if((reg_ctrl & 0x04) == 0){
              vram_address_reg += 1;
      }
      else vram_address_reg += 32;


    //        printPT();
    //        cout << "Line" << endl;
    //        display_full();
      break;

    default:

      break;

  }

}
void GPU::printPT(int y){

  //        int found = 0x0;

  cout << "PT: 0x"<< hex << y << dec << endl;

  for(int i = 0; i < 8; i++){
    for(int x = 0; x < 8; x++){

      int patterndat_one, patterndat_two;
      GPU_MEM_READ(((y << 4)+0x0+i), patterndat_one);
      GPU_MEM_READ(((y << 4)+0x8+i), patterndat_two);

      patterndat_one >>= 7-x;
      patterndat_one &= 0x1;
      patterndat_two >>= 7-x;
      patterndat_two &= 0x1;
      int pix_index = (patterndat_one << 1)|(patterndat_two);
    //                        int pix_color = mem_read(0x3F00|pix_index);


      cout << hex << pix_index << "";
    }
    cout << endl;
  }
  cout << endl;


}
void GPU::printNT(int y){
  int i, j;
  uint8_t value;
  cout << hex;

  for(i = 0; i < 30; i++){
    for(j = 0; j < 32; j++){
      GPU_MEM_READ(((i<< 5)|j | 0x2000), value);
      cout.width(2);
      cout << right << (unsigned int)value;
      cout << " ";

    }
    cout << endl;
  }
  cout << dec << endl;
}

int GPU::get_pixel_color(){
  int pt_num;
  GPU_MEM_READ(((vram_address_reg&0x0FFF)|0x2000), pt_num);



  
  int y_offset = (vram_address_reg >> 12)&0x7;

  int po = (pt_num << 4)+0x0 + y_offset;
  int pt = (pt_num << 4)+0x8 + y_offset;
  
  
  int patterndat_one;
  int patterndat_two;

  GPU_MEM_READ(po, patterndat_one);
  GPU_MEM_READ(pt, patterndat_two);

  patterndat_one >>= 7-offset_x;
  patterndat_one &= 0x1;
  patterndat_two >>= 7-offset_x;
  patterndat_two &= 0x1;
  int pix_index = (patterndat_one)|(patterndat_two << 1);

  int result;
  GPU_MEM_READ((0x3F00|pix_index), result);

  return result;
}

void GPU::render_sprites(){
  
  for(int i = 0; i < 0x100; i += 0x04){

    for(unsigned int y = 0; y < 8; y++){
      for(unsigned int x = 0; x < 8; x++){


        unsigned int y_offset = sprite_ram[i]-1;
        int pt_num = sprite_ram[i+1];

        unsigned int x_offset = sprite_ram[i+3];
        
        int po = (pt_num << 4) + 0x0 + y;
        int pt = (pt_num << 4) + 0x8 + y;
        if((reg_ctrl&0x08) != 0) {
          po += 0x1000;
          pt += 0x1000;
        }
        
        
        int patterndat_one;
        int patterndat_two;

        GPU_MEM_READ(po, patterndat_one);
        GPU_MEM_READ(pt, patterndat_two);

        int shift = x&0x7;

        patterndat_one >>= 7-shift;
        patterndat_one &= 0x1;
        patterndat_two >>= 7-shift;
        patterndat_two &= 0x1;
        int pix_index = (patterndat_one)|(patterndat_two << 1)|(sprite_ram[i+2]&0x3 << 2);
        int pix_color;

        GPU_MEM_READ((0x31F0|pix_index), pix_color);
        
        //~ cout << hex << "S: 0x" << po << " 0x" << pt << " - 0x" << pix_index << "(0x" << x_offset << ",0x" << y_offset << ")0x" << pt_num << dec << endl;

        //~ cout << "(" << (unsigned int)(x+x_offset) << "," << (int)(y+y_offset) << ")" << "(" << (int)(x_offset) << "," << (int)(y_offset) << ")" << endl;
        
        if((x+x_offset) < 256 && (y+y_offset) < 240) {
        
          vbuffer[(y + y_offset)*256 + (x + x_offset)] = paletteLookup(pix_color);
        }
      }
    }
  }
}

color GPU::paletteLookup(int num){
  switch(num){
    case 0x0: return (color){0x75,0x75,0x75};
    case 0x1: return (color){0x27,0x1B,0x8F};
    case 0x2: return (color){0x00,0x00,0xAB};
    case 0x3: return (color){0x47,0x00,0x9F};
    case 0x4: return (color){0x8F,0x00,0x77};
    case 0x5: return (color){0xAB,0x00,0x13};
    case 0x6: return (color){0xA7,0x00,0x0};
    case 0x7: return (color){0x7F,0x0B,0x0};
    case 0x8: return (color){0x43,0x2F,0x0};
    case 0x9: return (color){0x00,0x47,0x0};
    case 0xA: return (color){0x00,0x51,0x0};
    case 0xB: return (color){0x00,0x3F,0x17};
    case 0xC: return (color){0x1B,0x3F,0x5F};
    case 0xD: return (color){0x00,0x00,0x0};
    case 0xE: return (color){0x00,0x00,0x1};
    case 0xF: return (color){0x00,0x00,0x0};
    case 0x10: return (color){0xBC,0xBC,0xBC};
    case 0x11: return (color){0x00,0x73,0xEF};
    case 0x12: return (color){0x23,0x3B,0xEF};
    case 0x13: return (color){0x83,0x00,0xF3};
    case 0x14: return (color){0xBF,0x00,0xBF};
    case 0x15: return (color){0xE7,0x00,0x5B};
    case 0x16: return (color){0xDB,0x2B,0x0};
    case 0x17: return (color){0xCB,0x4F,0x0F};
    case 0x18: return (color){0x8B,0x73,0x0};
    case 0x19: return (color){0x00,0x97,0x0};
    case 0x1A: return (color){0x00,0xAB,0x0};
    case 0x1B: return (color){0x00,0x93,0x3B};
    case 0x1C: return (color){0x00,0x83,0x8B};
    case 0x1D: return (color){0x00,0x00,0x0};
    case 0x1E: return (color){0x00,0x00,0x0};
    case 0x1F: return (color){0x00,0x00,0x0};
    case 0x20: return (color){0xFF,0xFF,0xFF};
    case 0x21: return (color){0x3F,0xBF,0xFF};
    case 0x22: return (color){0x5F,0x97,0xFF};
    case 0x23: return (color){0xA7,0x8B,0xFD};
    case 0x24: return (color){0xF7,0x7B,0xFF};
    case 0x25: return (color){0xFF,0x77,0xB7};
    case 0x26: return (color){0xFF,0x77,0x63};
    case 0x27: return (color){0xFF,0x9B,0x3B};
    case 0x28: return (color){0xF3,0xBF,0x3F};
    case 0x29: return (color){0x83,0xD3,0x13};
    case 0x2A: return (color){0x4F,0xDF,0x4B};
    case 0x2B: return (color){0x58,0xF8,0x98};
    case 0x2C: return (color){0x00,0xEB,0xDB};
    case 0x2D: return (color){0x00,0x00,0x0};
    case 0x2E: return (color){0x00,0x00,0x0};
    case 0x2F: return (color){0x00,0x00,0x0};
    case 0x30: return (color){0xFF,0xFF,0xFF};
    case 0x31: return (color){0xAB,0xE7,0xFF};
    case 0x32: return (color){0xC7,0xD7,0xFF};
    case 0x33: return (color){0xD7,0xCB,0xFF};
    case 0x34: return (color){0xFF,0xC7,0xFF};
    case 0x35: return (color){0xFF,0xC7,0xDB};
    case 0x36: return (color){0xFF,0xBF,0xB3};
    case 0x37: return (color){0xFF,0xDB,0xAB};
    case 0x38: return (color){0xFF,0xE7,0xA3};
    case 0x39: return (color){0xE3,0xFF,0xA3};
    case 0x3A: return (color){0xAB,0xF3,0xBF};
    case 0x3B: return (color){0xB3,0xFF,0xCF};
    case 0x3C: return (color){0x9F,0xFF,0xF3};
    case 0x3D: return (color){0x00,0x00,0x0};
    case 0x3E: return (color){0x00,0x00,0x0};
    case 0x3F: return (color){0x00,0x00,0x0};
  }
  return (color){0xFF, 0xFF, 0xFF};
}


