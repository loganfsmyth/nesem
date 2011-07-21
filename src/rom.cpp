#include "rom.hpp"
using namespace std;


Rom::Rom(){
	trainer_data = NULL;
	prgrom_data = NULL;
	chrrom_data = NULL;
	title = "__BLANK__";
}
Rom::~Rom(){
	if(trainer_data != NULL) delete trainer_data;
	if(prgrom_data != NULL) delete prgrom_data;
	if(chrrom_data != NULL) delete chrrom_data;
}
void Rom::print_stats(){
	cout << "Rom Title:\t" <<  get_title() << endl;
	cout << "Ram banks:\t" << num_ram_banks()<< endl;
	cout << "Prg Ram banks:\t" << num_prg_banks() << endl;
	cout << "Chr Ram banks:\t" << num_chr_banks() << endl;

	cout << "Mirror Type:\t" << mirror_type() << endl;
	cout << "Battery:\t" << battery_ram() << endl;
	cout << "Trainer:\t" << trainer_present() << endl;
	cout << "Mapper:\t" << mapper_number() << endl;
}
int Rom::load(string filename){
	ifstream file(filename.c_str());

	file.read(identifier, 3);
	file.read(&id_num, 1);
	file.read(&prgrom_banks, 1);
	file.read(&chrrom_banks, 1);
	file.read(&ctrl_one, 1);
	file.read(&ctrl_two, 1);
	file.read(&ram_banks, 1);
	file.read(blank, 7);

	if(ram_banks == 0) ram_banks = 1;

	if(ctrl_one & 0x04){
		trainer_data = new char[TRAINER_SIZE];
		file.read(trainer_data, TRAINER_SIZE);
	}
	else trainer_data = NULL;
	if(prgrom_banks != 0){
		prgrom_data = new char[PRGROMBANK_SIZE*prgrom_banks];
		file.read(prgrom_data, PRGROMBANK_SIZE*prgrom_banks);
	}
	else prgrom_data = NULL;

	if(chrrom_banks != 0){
		chrrom_data = new char[ CHRROMBANK_SIZE*chrrom_banks];
		file.read(chrrom_data, CHRROMBANK_SIZE*chrrom_banks);
	}
	else chrrom_data = new char[ CHRROMBANK_SIZE ];

	if(!file.eof()) title = "";
	while(!file.eof() && file.peek() != '\0'){
		title += file.get();
	}

	file.close();


	//~ ofstream out("nestress.rom");
	//~ out.write(prgrom_data, PRGROMBANK_SIZE*prgrom_banks);
	//~ out.close();

	return ROM_LOAD_SUCCESS;
}



int Rom::num_prg_banks(){
	return (int)prgrom_banks;
}
int Rom::num_chr_banks(){
	return (int)chrrom_banks;
}
int Rom::num_ram_banks(){
	return (int)ram_banks;
}
int Rom::mirror_type(){
	if(ctrl_one & 0x04) return ROM_MIRROR_ALL;
	else if(ctrl_one & 0x01) return ROM_MIRROR_VERT;
	else  return ROM_MIRROR_HORIZ;
}
bool Rom::battery_ram(){
	if(ctrl_one & 0x02) return true;
	else return false;
}
bool Rom::trainer_present(){
	if(ctrl_one & 0x04) return true;
	else return false;
}
int Rom::mapper_number(){
	return ((ctrl_one >> 4) | (ctrl_two & 0xF0));
}
string Rom::get_title(){
	return title;
}

int Rom::prg_write(int location, char data){
	if(prgrom_banks == 1) location &= 0x3FFF;

//	cout << hex << "Loc: " << location << "  Dat: " << data << dec << endl;

	prgrom_data[location] = data;
	return ROM_WRITE_SUCCESS;
}
char Rom::prg_read(int location){
//	cout << "OMG" << endl;

	if(prgrom_banks == 1) location &= 0x3FFF;
//	cout << hex << "Loc: " << location << dec << endl;

	return prgrom_data[location];
}
int Rom::chr_write(int location, char data){
	chrrom_data[location] = data;

	return ROM_WRITE_SUCCESS;
}
char Rom::chr_read(int location){
	return chrrom_data[location];
}
