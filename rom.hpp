#ifndef INCLUDE_ROM_H
#define INCLUDE_ROM_H

#include <iostream>
#include <fstream>
#include <string>



#define RAMBANK_SIZE		(8*1024)
#define CHRROMBANK_SIZE	(8*1024)
#define PRGROMBANK_SIZE	(16*1024)
#define TRAINER_SIZE		512

#define ROM_LOAD_SUCCESS	1
#define ROM_LOAD_ERROR		0

#define ROM_MIRROR_VERT		0
#define ROM_MIRROR_HORIZ		1
#define ROM_MIRROR_ALL		2

#define ROM_WRITE_ERROR	0
#define ROM_WRITE_SUCCESS	1

class Rom{
	private:
		char identifier[3];
		char id_num;
		char prgrom_banks;
		char chrrom_banks;
		char ctrl_one;
		char ctrl_two;
		char ram_banks;
		char blank[7];
		char* trainer_data;
		char* prgrom_data;
		char* chrrom_data;
		std::string title;
	public:
		Rom();
		~Rom();
		int load(std::string filename);
		int num_prg_banks();
		int num_chr_banks();
		int num_ram_banks();
		int mirror_type();
		bool battery_ram();
		bool trainer_present();
		int mapper_number();
		void print_stats();
		std::string get_title();
		int prg_write(int location, char data);
		char prg_read(int location);
		int chr_write(int location, char data);
		char chr_read(int location);
};

#endif
