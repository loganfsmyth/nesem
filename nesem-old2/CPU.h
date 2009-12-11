#ifndef INCLUDE_CPU_H
#define INCLUDE_CPU_H

#include <cstdint>
#include <vector>
#include <string>
#include <sstream>
#include "Memory.h"



#define ADC_IMM 0x69
#define ADC_ZP 0x65
#define ADC_ZPX 0x75
#define ADC_AB 0x6D
#define ADC_ABX 0x7D
#define ADC_ABY 0x79
#define ADC_INX 0x61
#define ADC_INY 0x71

#define SBC_IMM 0xE9
#define SBC_ZP 0xE5
#define SBC_ZPX 0xF5
#define SBC_AB 0xED
#define SBC_ABX 0xFD
#define SBC_ABY 0xF9
#define SBC_INX 0xE1
#define SBC_INY 0xF1

#define AND_IMM 0x29
#define AND_ZP 0x25
#define AND_ZPX 0x35
#define AND_AB 0x2D
#define AND_ABX 0x3D
#define AND_ABY 0x39
#define AND_INX 0x21
#define AND_INY 0x31

#define CMP_IMM 0xC9
#define CMP_ZP 0xC5
#define CMP_ZPX 0xD5
#define CMP_AB 0xCD
#define CMP_ABX 0xDD
#define CMP_ABY 0xD9
#define CMP_INX 0xC1
#define CMP_INY 0xD1

#define EOR_IMM 0x49
#define EOR_ZP 0x45
#define EOR_ZPX 0x55
#define EOR_AB 0x4D
#define EOR_ABX 0x5D
#define EOR_ABY 0x59
#define EOR_INX 0x41
#define EOR_INY 0x51

#define ORA_IMM 0x09
#define ORA_ZP 0x05
#define ORA_ZPX 0x15
#define ORA_AB 0x0D
#define ORA_ABX 0x1D
#define ORA_ABY 0x19
#define ORA_INX 0x01
#define ORA_INY 0x11

#define LDA_IMM 0xA9
#define LDA_ZP 0xA5
#define LDA_ZPX 0xB5
#define LDA_AB 0xAD
#define LDA_ABX 0xBD
#define LDA_ABY 0xB9
#define LDA_INX 0xA1
#define LDA_INY 0xB1

#define STA_ZP 0x85
#define STA_ZPX 0x95
#define STA_AB 0x8D
#define STA_ABX 0x9D
#define STA_ABY 0x99
#define STA_INX 0x81
#define STA_INY 0x91

#define TXS_IMP 0x9A
#define TSX_IMP 0xBA
#define PHA_IMP 0x48
#define PLA_IMP 0x68
#define PHP_IMP 0x08
#define PLP_IMP 0x28

#define BPL_REL 0x10
#define BMI_REL 0x30
#define BVC_REL 0x50
#define BVS_REL 0x70
#define BCC_REL 0x90
#define BCS_REL 0xB0
#define BNE_REL 0xD0
#define BEQ_REL 0xF0

#define BRK_IMP 0x00

#define CLC_IMP 0x18
#define SEC_IMP 0x38
#define CLI_IMP 0x58
#define SEI_IMP 0x78
#define CLV_IMP 0xB8
#define CLD_IMP 0xD8
#define SED_IMP 0xF8

#define NOP_IMP 0xEA

#define TAX_IMP 0xAA
#define TXA_IMP 0x8A
#define DEX_IMP 0xCA
#define INX_IMP 0xE8
#define TAY_IMP 0xA8
#define TYA_IMP 0x98
#define DEY_IMP 0x88
#define INY_IMP 0xC8

#define RTI_IMP 0x40

#define RTS_IMP 0x60

#define LSR_AC 0x4A
#define LSR_ZP 0x46
#define LSR_ZPX 0x56
#define LSR_AB 0x4E
#define LSR_ABX 0x5E

#define ROL_AC 0x2A
#define ROL_ZP 0x26
#define ROL_ZPX 0x36
#define ROL_AB 0x2E
#define ROL_ABX 0x3E

#define ROR_AC 0x6A
#define ROR_ZP 0x66
#define ROR_ZPX 0x76
#define ROR_AB 0x6E
#define ROR_ABX 0x7E

#define ASL_AC 0x0A
#define ASL_ZP 0x06
#define ASL_ZPX 0x16
#define ASL_AB 0x0E
#define ASL_ABX 0x1E

#define LDX_IMM 0xA2
#define LDX_ZP 0xA6
#define LDX_ZPY 0xB6
#define LDX_AB 0xAE
#define LDX_ABX 0xBE

#define LDY_IMM 0xA0
#define LDY_ZP 0xA4
#define LDY_ZPX 0xB4
#define LDY_AB 0xAC
#define LDY_ABX 0xBC

#define DEC_ZP 0xC6
#define DEC_ZPX 0xD6
#define DEC_AB 0xCE
#define DEC_ABX 0xDE

#define INC_ZP 0xE6
#define INC_ZPX 0xF6
#define INC_AB 0xEE
#define INC_ABX 0xFE

#define STX_ZP 0x86
#define STX_ZPY 0x96
#define STX_AB 0x8E

#define STY_ZP 0x84
#define STY_ZPX 0x94
#define STY_AB 0x8C

#define CPX_IMM 0xE0
#define CPX_ZP 0xE4
#define CPX_AB 0xEC

#define CPY_IMM 0xC0
#define CPY_ZP 0xC4
#define CPY_AB 0xCC

#define BIT_ZP 0x24
#define BIT_AB 0x2C

#define JMP_AB 0x4C
#define JMP_IN 0x6C

#define JSR_AB 0x20


#define MEM_IMM	0
#define MEM_ZP	1
#define MEM_ZPX	2
#define MEM_AB		3
#define MEM_ABX	4
#define MEM_ABY	5
#define MEM_INX	6
#define MEM_INY	7
#define MEM_IMP	8
#define MEM_AC		9
#define MEM_IN		10
#define MEM_ZPY	11
#define MEM_REL	12

#define SET_ZERO(in)  	\
	SR &= 0xFD;		\
	SR |= (((in) == 0) << 1) & 0x02;	\

#define SET_SIGN(in)  	\
	SR &= 0x7F;		\
	SR |= ((in) & 0x80);	\

#define SET_CARRY(in)  	\
	SR &= 0xFE;		\
	SR |= ((in) != 0) &0x01 ;	\

#define SET_OVERFLOW(in)  	\
	SR &= 0xBF;		\
	SR |= (((in) != 0) << 6) & 0x40;	\

#define SET_INTERRUPT(in)  	\
	SR &= 0xFB;		\
	SR |= (((in) != 0) << 2) & 0x04;	\

#define SET_BREAK(in)  	\
	SR &= 0xEF;		\
	SR |= (((in) != 0) << 5) & 0x10;	\

#define SET_DECIMAL(in)  	\
	SR &= 0xF7;		\
	SR |= (((in) != 0) << 3) & 0x80;	\

#define IF_CARRY() (SR & 0x01)
#define IF_OVERFLOW() (SR & 0x40)
#define IF_SIGN() (SR & 0x80)
#define IF_ZERO() (SR & 0x02)
#define IF_DECIMAL() (SR & 0x08)

//#define PUSH(val) mem->write(SP--, val)
//#define PULL() mem->read(++SP)

#define PUSH(val) mem->write(SP + 0x100, val); SP--; //cout << "PUSH: " << ((int)val) << endl;
#define PULL() mem->read(((++SP) & 0xFF) + 0x100)

struct instruction_info{
	int length;
	int type;
	int time;
	char plus;
};

class CPU{
	public:
		CPU();
		~CPU();
		
		
		int executeInst();
		void setMemory(Memory* m);
		void reset();
		void trigger_interrupt();
		void raise_interrupt();
		void drop_interrupt();
	
//		void write(int type, uint16_t addr, uint8_t arg1, uint8_t arg2, uint8_t data);
		void write(int type,  uint8_t arg1, uint8_t arg2, uint8_t data);
//		uint8_t read(int type, uint16_t addr, uint8_t arg1, uint8_t arg2, uint8_t data);
		uint8_t read(int type, uint8_t arg1, uint8_t arg2);
//		uint16_t get_address(int type, uint8_t arg1, uint8_t arg2);
		void print_stack();
	
		void print_trace();
	
		string get_inst_str(int inst);
	
	private:
		vector<string> trace;
		uint8_t XR,YR,AC;
		uint8_t SP, SR;
		uint16_t PC;
	
		Memory* mem;
	
		bool interrupt_status, interrupt_status_last;
	
		struct instruction_info get_instruction_info(uint8_t inst);
	
	
	
};


#endif
