#ifndef INCLUDE_MEMORY_H
#define INCLUDE_MEMORY_H

#include <cstdint>
#include "Rom.h"
#include "GPU.h"


#define MEMORY_LOAD_ERROR	0
#define MEMORY_LOAD_SUCCESS	1

class Memory{
	public:
		Memory();
		~Memory();
		
		int load(Rom* r);
		int write(uint16_t location, uint8_t data);
		uint8_t read(uint16_t location);
	
		void setGPU(GPU* g);
	
	private:
		Rom* rom;
		GPU* gpu;
		uint8_t ram[0x800];
};


#endif
