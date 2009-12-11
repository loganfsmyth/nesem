#ifndef INCLUDE_GPU_H
#define INCLUDE_GPU_H

#include <cstdint>
#include <iostream>
#include "Window.h"
#include "Rom.h"


#define GPU_REG_CTRL		0
#define GPU_REG_MASK		1
#define GPU_REG_STATUS	2
#define GPU_REG_OAM_ADDR	3
#define GPU_REG_OAM_DATA	4
#define GPU_REG_SCROLL	5
#define GPU_REG_ADDR		6
#define GPU_REG_DATA		7

struct color{
	uint8_t r;
	uint8_t g;
	uint8_t b;
};


class GPU{
	
	public:
		GPU();
		~GPU();
		
		
		uint8_t reg_read(uint8_t location);
		void reg_write(uint8_t location, uint8_t data);
		
		uint8_t mem_read(uint16_t location);
		void mem_write(uint16_t location, uint8_t data);
		void setWindow(Window* w);
		void setCHRRom(Rom* r);
		
	
		int executeCycle();
		void display();
		void display_full();
		void printPT(int y);
		void printNT(int y);
	
		void render_sprites();
	
	private:
		uint8_t pattern_table[0x2000];
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
	
		Window* win;
		Rom* rom;
		
		uint8_t pix_count;
		
		int wait_frame;
		int cycle, scanline;
		
		color paletteLookup(int num);
		
};

#endif
