#ifndef INCLUDE_MEMORY_H
#define INCLUDE_MEMORY_H

#include <cstdint>
#include "rom.hpp"
#include "gpu.hpp"
#include <boost/smart_ptr.hpp>


#define MEMORY_LOAD_ERROR	0
#define MEMORY_LOAD_SUCCESS	1

class Memory{
	public:
		Memory();
		~Memory();

		int load(boost::shared_ptr<Rom> r);
		int write(uint16_t location, uint8_t data);
		uint8_t read(uint16_t location);

		void setGPU(boost::shared_ptr<GPU> g);

	private:
		boost::shared_ptr<Rom> rom;
		boost::shared_ptr<GPU> gpu;
		uint8_t ram[0x800];
};


#endif
