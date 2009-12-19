#include "memory.hpp"

using namespace std;


Memory::Memory(){
  
}
Memory::~Memory(){

}
int Memory::load(boost::shared_ptr<Rom> r){
	rom = r;
	return MEMORY_LOAD_SUCCESS;
}
void Memory::setGPU(boost::shared_ptr<GPU> g){
	gpu = g;
}
int Memory::write(uint16_t location, uint8_t data){
//	cout << hex;
//	cout << "Write: 0x" << hex << (int)location << " val 0x" << (int)data << dec << endl;
//	cout << dec;

	if(location < 0x2000){
		ram[location & 0x07FF] = data;
	}
	else if(location >= 0x8000){
		return rom->prg_write(location & 0x7FFF, (char)data);
	}
	else if(location >= 0x2000 && location < 0x4000){
		gpu->reg_write(location & 0x0007, data);
		return 0;
	}
	else if(location == 0x4014){
		int i;
//		gpu->reg_write(GPU_REG_OAM_ADDR, 0x00);
		for(i = 0; i < 0xFF; i++){
			gpu->reg_write(GPU_REG_OAM_DATA, read((data << 8)|i));
		}
	}
	else{
		cout << "Control Reg Write: " << hex << location << dec << endl;
	}
	return 0;
}
uint8_t Memory::read(uint16_t location){
//	cout << "Read: 0x" << hex << location << dec << endl;


	if(location < 0x2000){
		return ram[location & 0x07FF];
	}
	else if(location >= 0x8000){
//		cout << "OMG" << endl;

//		\\it's crashing here for some reason!!!
		return (uint8_t)rom->prg_read(location & 0x7FFF);
	}
	else if(location >= 0x2000 && location < 0x4000){
		return gpu->reg_read(location & 0x0007);
	}
	else{
		cout << "Control Reg Read: " << hex << location << dec << endl;
	}
	return 0;
}
