#ifndef __MD6502_H__
#define __MD6502_H__

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;




typedef struct _reg6502 {
	uchar a;
	uchar x;
	uchar y;
	union {
		uchar sr;
		struct {
			uchar carry : 1;
			uchar zero : 1;
			uchar intr : 1;
			uchar dec : 1;
			uchar brk : 1;
			uchar resv : 1;
			uchar ovfl : 1;
			uchar sign : 1;
		};
	} sr;
	ushort sp;
	ushort pc;
} reg6502;


typedef struct _memargstr {
	ushort staddr;				// 開始アドレス
	ushort edaddr;				// 終了アドレス
	int access;					// アクセス方法のフラグ
	char *comment;				// コメントの内容
} memargstr;


// 返り値は、ステート数
typedef int (*op6502)(reg6502 *);



typedef struct {
	char *nimonic;
	int type;
//	op6502 func;
} dis6502str;

// dis6502str.type = Address Type | Access Type;
// Address Type
enum {
	ADT_NONE = 0,
	ADT_IMM,				// 即値
	ADT_AREG,				// A Register
	ADT_ZEROP,				// Zero Page
	ADT_ZEROPX,				// Zero Page,X
	ADT_ZEROPY,				// Zero Page,Y
	ADT_REL,				// 相対アドレス
	ADT_ABS,				// 絶対アドレス
	ADT_ABSX,				// 絶対アドレス,X
	ADT_ABSY,				// 絶対アドレス,Y
	ADT_ABSIND,				// 間接絶対アドレス
	ADT_PREIND,				// 前置間接 (Zero Page,X)
	ADT_POSTIND,			// 後置間接 (Zero Page),Y
};

#define ADT_MASK 0xff

// Access Type
enum {
	ACT_NL = 0x000,
	ACT_RD = 0x100,
	ACT_WT = 0x200,
};

#define ACT_MASK ( ACT_RD | ACT_WT )


// Name Type
enum {
	NMT_UND = 0,
	NMT_ADC,
	NMT_AND,
	NMT_ASL,
	NMT_BCC,
	NMT_BCS,
	NMT_BEQ,
	NMT_BIT,
	NMT_BMI,
	NMT_BNE,
	NMT_BPL,
	NMT_BRK,
	NMT_BVC,
	NMT_BVS,
	NMT_CLC,
	NMT_CLD,
	NMT_CLI,
	NMT_CLV,
	NMT_CMP,
	NMT_CPX,
	NMT_CPY,
	NMT_DEC,
	NMT_DEX,
	NMT_DEY,
	NMT_EOR,
	NMT_INC,
	NMT_INX,
	NMT_INY,
	NMT_JMP,
	NMT_JSR,
	NMT_LDA,
	NMT_LDX,
	NMT_LDY,
	NMT_LSR,
	NMT_NOP,
	NMT_ORA,
	NMT_PHA,
	NMT_PHP,
	NMT_PLA,
	NMT_PLP,
	NMT_ROL,
	NMT_ROR,
	NMT_RTI,
	NMT_RTS,
	NMT_SBC,
	NMT_SEC,
	NMT_SED,
	NMT_SEI,
	NMT_STA,
	NMT_STX,
	NMT_STY,
	NMT_TAX,
	NMT_TAY,
	NMT_TSX,
	NMT_TXA,
	NMT_TXS,
	NMT_TYA,
};



#endif // __MD6502_H__
