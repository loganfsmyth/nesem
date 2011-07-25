#include "rom.hpp"
using namespace std;


Rom::Rom(string filename){
  trainer_data = NULL;
  prgrom_data = NULL;
  chrrom_data = NULL;
  title = "";

  // Load file in binary mode at end of file.
  ifstream file(filename.c_str(), ios::in | ios::binary | ios::ate);

  if (!file.is_open()) throw exception();

  streampos length = file.tellg();  // Read position to get file size
  file.seekg(0);

  // Read the file header into the header struct
  file.read((char*)&head, sizeof(head));

  if(head.ram_banks == 0) head.ram_banks = 1;

  // Load the trailer if it exists
  if(head.ctrl_one & 0x04){
    trainer_data = new uint8_t[TRAINER_SIZE];
    file.read((char*)trainer_data, TRAINER_SIZE);
  }
  else {
    trainer_data = NULL;
  }

  // Load program rom blocks
  if(head.prgrom_banks != 0){
    prgrom_data = new uint8_t[PRGROMBANK_SIZE*head.prgrom_banks];
    file.read((char*)prgrom_data, PRGROMBANK_SIZE*head.prgrom_banks);
  }
  else {
    prgrom_data = NULL;
  }

  // Load character rom blocks
  if(head.chrrom_banks != 0){
    chrrom_data = new uint8_t[ CHRROMBANK_SIZE*head.chrrom_banks];
    file.read((char*)chrrom_data, CHRROMBANK_SIZE*head.chrrom_banks);
  }
  else {
    chrrom_data = new uint8_t[ CHRROMBANK_SIZE ];
  }

  // Check for and load a title from the end of the file
  if (length - file.tellg()) {
    while(!file.eof() && file.peek() != '\0'){
      title += file.get();
    }
  }

  file.close();
}


Rom::~Rom(){
  if(trainer_data != NULL) delete[] trainer_data;
  if(prgrom_data != NULL) delete[] prgrom_data;
  if(chrrom_data != NULL) delete[] chrrom_data;
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

int Rom::num_prg_banks(){
  return (int)head.prgrom_banks;
}

int Rom::num_chr_banks(){
  return (int)head.chrrom_banks;
}

int Rom::num_ram_banks(){
  return (int)head.ram_banks;
}

int Rom::mirror_type(){
  if(head.ctrl_one & 0x04) return ROM_MIRROR_ALL;
  else if(head.ctrl_one & 0x01) return ROM_MIRROR_VERT;
  else  return ROM_MIRROR_HORIZ;
}

bool Rom::battery_ram(){
  if(head.ctrl_one & 0x02) return true;
  else return false;
}

bool Rom::trainer_present(){
  if(head.ctrl_one & 0x04) return true;
  else return false;
}

int Rom::mapper_number(){
  return ((head.ctrl_one >> 4) | (head.ctrl_two & 0xF0));
}

string Rom::get_title(){
  return title;
}

uint8_t* Rom::prg(int num) {
  return &prgrom_data[num & (head.prgrom_banks-1)];
}

uint8_t* Rom::chr(int num) {
  return &chrrom_data[num & (head.chrrom_banks-1)];
}

