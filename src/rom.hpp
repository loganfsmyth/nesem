#ifndef INCLUDE_ROM_H
#define INCLUDE_ROM_H

#include <iostream>
#include <fstream>
#include <string>
#include <boost/cstdint.hpp>



#define RAMBANK_SIZE      0x1000
#define CHRROMBANK_SIZE   0x2000
#define PRGROMBANK_SIZE   0x4000
#define TRAINER_SIZE      0x200

#define ROM_LOAD_SUCCESS  0x1
#define ROM_LOAD_ERROR    0x0

#define ROM_MIRROR_VERT   0x0
#define ROM_MIRROR_HORIZ  0x1
#define ROM_MIRROR_ALL    0x2

#define ROM_WRITE_ERROR   0x0
#define ROM_WRITE_SUCCESS 0x1

struct ines_header {
  char identifier[3];
  uint8_t id_num;
  uint8_t prgrom_banks;
  uint8_t chrrom_banks;
  uint8_t ctrl_one;
  uint8_t ctrl_two;
  uint8_t ram_banks;
  uint8_t blank[7];

};

class Rom {
  private:
    ines_header head;
    uint8_t* trainer_data;
    uint8_t* prgrom_data;
    uint8_t* chrrom_data;
    std::string title;


  public:
    Rom(std::string filename);
    ~Rom();
    int num_prg_banks();
    int num_chr_banks();
    int num_ram_banks();
    int mirror_type();
    bool battery_ram();
    bool trainer_present();
    int mapper_number();
    void print_stats();
    std::string get_title();

    uint8_t* prg(int num);
    uint8_t* chr(int num);
};

#endif
