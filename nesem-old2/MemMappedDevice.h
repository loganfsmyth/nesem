#include <cstdint>

#ifndef INCLUDE_MEMMAPPEDDEVICE_H
#define INCLUDE_MEMMAPPEDDEVICE_H

class MemMappedDevice{
	
	public:
		virtual uint8_t mapped_read(uint16_t location) = 0;
		virtual void mapped_write(uint16_t location, uint8_t data) = 0;
};

#endif
