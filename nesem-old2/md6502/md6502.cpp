/*
 * Minachun Disassembler for famicom(NES)
 *
 * Version 0.04a: INC/DEC $xx,x が ,y になっていたバグを Fix.
 * Version 0.04 : BN:0xb8 (マドゥーラの翼) のメッセージを付加
 *                -sを指定せず-lのみを指定したときの、ベースアドレスの不正をFix.
 *                MMC5 のメッセージを追加、変更
 * Version 0.03 : VRC2B のメッセージ、アドレスを変更
 *                NSF バンクメッセージの付加
 *                マッパー番号を複数指定できるようにした
 * Version 0.02 : VRC6 の音源メッセージを付加。
 *                Namcot106 のメッセージを追加(ExI/O Addr.)
 *                VRC7,VRC1,Sun EarlyMapper を追加
 *                fds のマッパー番号を 0x14(20) にセット＞パソファミ向け
 *                書き込み読み込み属性の追加
 * Version 0.01 : yd6502 と出力が同じになるようにした。
 *                開始アドレス、オフセット、サイズを指定できるようにした。
 *                コメントをつくようにした。
 *                割り込みベクターをそれらしくした。
 *                マッパー別のコメント出力に挑戦した。
 */


#include <stdio.h>
#include <string.h>

#include "md6502.h"


/* If you comment out below semantic , this program outputs English Messages. */
//#define MESSAGETYPE_JAPANESE


op6502 undefop(void) { return 0;}

char *adtstring[] = {
	"\t\t",
	"#$%02X\t\t",
	"a\t\t",
	"$%02X\t\t",
	"$%02X,x\t\t",
	"$%02X,y\t\t",
	"$%04X\t\t",
	"$%04X\t\t",
	"$%04X,x\t\t",
	"$%04X,y\t\t",
	"($%04X)\t\t",
	"($%02X,x)\t\t",
	"($%02X),y\t\t",
};

int opbytelength[] = {
	1,2,1,2,2,2,2,3,3,3,3,2,2,
};

char *opname[] = {
	"db",
	"adc",
	"and",
	"asl",
	"bcc",
	"bcs",
	"beq",
	"bit",
	"bmi",
	"bne",
	"bpl",
	"brk",
	"bvc",
	"bvs",
	"clc",
	"cld",
	"cli",
	"clv",
	"cmp",
	"cpx",
	"cpy",
	"dec",
	"dex",
	"dey",
	"eor",
	"inc",
	"inx",
	"iny",
	"jmp",
	"jsr",
	"lda",
	"ldx",
	"ldy",
	"lsr",
	"nop",
	"ora",
	"pha",
	"php",
	"pla",
	"plp",
	"rol",
	"ror",
	"rti",
	"rts",
	"sbc",
	"sec",
	"sed",
	"sei",
	"sta",
	"stx",
	"sty",
	"tax",
	"tay",
	"tsx",
	"txa",
	"txs",
	"tya",
};

// フラグのつけ方で迷っている：JSR , RTS , RTI は、一応スタックに対して読み書き
// を行っているから、本来なら RD,WT がつくはずだけど、今回は NL 扱いにする。
// もう一つ：JMP (ADDR) もだね。一応 NL 扱いにしておく
// さらに一つ：PHx,PLx は RD,WT にしておく。どうせスタックしか操作しないのだし、
// JSRとかと同じにすべきかなぁ？

dis6502str dis6502[256] = {
	// 00-0F
	{ opname[NMT_BRK],ACT_NL | ADT_NONE },
	{ opname[NMT_ORA],ACT_RD | ADT_PREIND },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_ORA],ACT_RD | ADT_ZEROP },
	{ opname[NMT_ASL],ACT_WT | ADT_ZEROP },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_PHP],ACT_WT | ADT_NONE },
	{ opname[NMT_ORA],ACT_NL | ADT_IMM },
	{ opname[NMT_ASL],ACT_NL | ADT_AREG },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_ORA],ACT_RD | ADT_ABS },
	{ opname[NMT_ASL],ACT_WT | ADT_ABS },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	// 10-1F
	{ opname[NMT_BPL],ACT_NL | ADT_REL },
	{ opname[NMT_ORA],ACT_RD | ADT_POSTIND },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_ORA],ACT_RD | ADT_ZEROPX },
	{ opname[NMT_ASL],ACT_WT | ADT_ZEROPX },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_CLC],ACT_NL | ADT_NONE },
	{ opname[NMT_ORA],ACT_RD | ADT_ABSY },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_ORA],ACT_RD | ADT_ABSX },
	{ opname[NMT_ASL],ACT_WT | ADT_ABSX },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	// 20-2F
	{ opname[NMT_JSR],ACT_NL | ADT_ABS },
	{ opname[NMT_AND],ACT_RD | ADT_PREIND },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_BIT],ACT_RD | ADT_ZEROP },
	{ opname[NMT_AND],ACT_RD | ADT_ZEROP },
	{ opname[NMT_ROL],ACT_WT | ADT_ZEROP },
	{ opname[NMT_UND],ACT_RD | ADT_NONE },
	{ opname[NMT_PLP],ACT_RD | ADT_NONE },
	{ opname[NMT_AND],ACT_RD | ADT_IMM },
	{ opname[NMT_ROL],ACT_NL | ADT_AREG },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_BIT],ACT_RD | ADT_ABS },
	{ opname[NMT_AND],ACT_RD | ADT_ABS },
	{ opname[NMT_ROL],ACT_WT | ADT_ABS },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	// 30-3F
	{ opname[NMT_BMI],ACT_NL | ADT_REL },
	{ opname[NMT_AND],ACT_RD | ADT_POSTIND },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_AND],ACT_RD | ADT_ZEROPX },
	{ opname[NMT_ROL],ACT_WT | ADT_ZEROPX },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_SEC],ACT_NL | ADT_NONE },
	{ opname[NMT_AND],ACT_RD | ADT_ABSY },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_AND],ACT_RD | ADT_ABSX },
	{ opname[NMT_ROL],ACT_WT | ADT_ABSX },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	// 40-4F
	{ opname[NMT_RTI],ACT_NL | ADT_NONE },
	{ opname[NMT_EOR],ACT_RD | ADT_PREIND },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_EOR],ACT_RD | ADT_ZEROP },
	{ opname[NMT_LSR],ACT_WT | ADT_ZEROP },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_PHA],ACT_WT | ADT_NONE },
	{ opname[NMT_EOR],ACT_NL | ADT_IMM },
	{ opname[NMT_LSR],ACT_NL | ADT_AREG },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_JMP],ACT_NL | ADT_ABS },
	{ opname[NMT_EOR],ACT_RD | ADT_ABS },
	{ opname[NMT_LSR],ACT_WT | ADT_ABS },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	// 50-5F
	{ opname[NMT_BVC],ACT_NL | ADT_REL },
	{ opname[NMT_EOR],ACT_RD | ADT_POSTIND },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_EOR],ACT_RD | ADT_ZEROPX },
	{ opname[NMT_LSR],ACT_WT | ADT_ZEROPX },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_CLI],ACT_NL | ADT_NONE },
	{ opname[NMT_EOR],ACT_RD | ADT_ABSY },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_EOR],ACT_RD | ADT_ABSX },
	{ opname[NMT_LSR],ACT_WT | ADT_ABSX },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	// 60-6F
	{ opname[NMT_RTS],ACT_NL | ADT_NONE },
	{ opname[NMT_ADC],ACT_RD | ADT_PREIND },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_ADC],ACT_RD | ADT_ZEROP },
	{ opname[NMT_ROR],ACT_WT | ADT_ZEROP },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_PLA],ACT_RD | ADT_NONE },
	{ opname[NMT_ADC],ACT_NL | ADT_IMM },
	{ opname[NMT_ROR],ACT_NL | ADT_AREG },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_JMP],ACT_NL | ADT_ABSIND },
	{ opname[NMT_ADC],ACT_RD | ADT_ABS },
	{ opname[NMT_ROR],ACT_WT | ADT_ABS },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	// 70-7F
	{ opname[NMT_BVS],ACT_NL | ADT_REL },
	{ opname[NMT_ADC],ACT_RD | ADT_POSTIND },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_ADC],ACT_RD | ADT_ZEROPX },
	{ opname[NMT_ROR],ACT_WT | ADT_ZEROPX },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_SEI],ACT_NL | ADT_NONE },
	{ opname[NMT_ADC],ACT_RD | ADT_ABSY },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_ADC],ACT_RD | ADT_ABSX },
	{ opname[NMT_ROR],ACT_WT | ADT_ABSX },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	// 80-8F
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_STA],ACT_WT | ADT_PREIND },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_STY],ACT_WT | ADT_ZEROP },
	{ opname[NMT_STA],ACT_WT | ADT_ZEROP },
	{ opname[NMT_STX],ACT_WT | ADT_ZEROP },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_DEY],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_TXA],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_STY],ACT_WT | ADT_ABS },
	{ opname[NMT_STA],ACT_WT | ADT_ABS },
	{ opname[NMT_STX],ACT_WT | ADT_ABS },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	// 90-9F
	{ opname[NMT_BCC],ACT_NL | ADT_REL },
	{ opname[NMT_STA],ACT_WT | ADT_POSTIND },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_STY],ACT_WT | ADT_ZEROPX },
	{ opname[NMT_STA],ACT_WT | ADT_ZEROPX },
	{ opname[NMT_STX],ACT_WT | ADT_ZEROPY },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_TYA],ACT_NL | ADT_NONE },
	{ opname[NMT_STA],ACT_WT | ADT_ABSY },
	{ opname[NMT_TXS],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_STA],ACT_WT | ADT_ABSX },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	// A0-AF
	{ opname[NMT_LDY],ACT_NL | ADT_IMM },
	{ opname[NMT_LDA],ACT_RD | ADT_PREIND },
	{ opname[NMT_LDX],ACT_NL | ADT_IMM },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_LDY],ACT_RD | ADT_ZEROP },
	{ opname[NMT_LDA],ACT_RD | ADT_ZEROP },
	{ opname[NMT_LDX],ACT_RD | ADT_ZEROP },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_TAY],ACT_NL | ADT_NONE },
	{ opname[NMT_LDA],ACT_NL | ADT_IMM },
	{ opname[NMT_TAX],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_LDY],ACT_RD | ADT_ABS },
	{ opname[NMT_LDA],ACT_RD | ADT_ABS },
	{ opname[NMT_LDX],ACT_RD | ADT_ABS },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	// B0-BF
	{ opname[NMT_BCS],ACT_NL | ADT_REL },
	{ opname[NMT_LDA],ACT_RD | ADT_POSTIND },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_LDY],ACT_RD | ADT_ZEROPX },
	{ opname[NMT_LDA],ACT_RD | ADT_ZEROPX },
	{ opname[NMT_LDX],ACT_RD | ADT_ZEROPY },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_CLV],ACT_NL | ADT_NONE },
	{ opname[NMT_LDA],ACT_RD | ADT_ABSY },
	{ opname[NMT_TSX],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_LDY],ACT_RD | ADT_ABSX },
	{ opname[NMT_LDA],ACT_RD | ADT_ABSX },
	{ opname[NMT_LDX],ACT_RD | ADT_ABSY },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	// C0-CF
	{ opname[NMT_CPY],ACT_NL | ADT_IMM },
	{ opname[NMT_CMP],ACT_RD | ADT_PREIND },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_CPY],ACT_RD | ADT_ZEROP },
	{ opname[NMT_CMP],ACT_RD | ADT_ZEROP },
	{ opname[NMT_DEC],ACT_WT | ADT_ZEROP },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_INY],ACT_NL | ADT_NONE },
	{ opname[NMT_CMP],ACT_NL | ADT_IMM },
	{ opname[NMT_DEX],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_CPY],ACT_RD | ADT_ABS },
	{ opname[NMT_CMP],ACT_RD | ADT_ABS },
	{ opname[NMT_DEC],ACT_WT | ADT_ABS },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	// D0-DF
	{ opname[NMT_BNE],ACT_NL | ADT_REL },
	{ opname[NMT_CMP],ACT_RD | ADT_POSTIND },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_CMP],ACT_RD | ADT_ZEROPX },
	{ opname[NMT_DEC],ACT_WT | ADT_ZEROPX }, /* ここ、ADT_ZEROPY から X へ修正 */
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_CLD],ACT_NL | ADT_NONE },
	{ opname[NMT_CMP],ACT_RD | ADT_ABSY },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_CMP],ACT_RD | ADT_ABSX },
	{ opname[NMT_DEC],ACT_WT | ADT_ABSX },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	// E0-EF
	{ opname[NMT_CPX],ACT_NL | ADT_IMM },
	{ opname[NMT_SBC],ACT_RD | ADT_PREIND },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_CPX],ACT_RD | ADT_ZEROP },
	{ opname[NMT_SBC],ACT_RD | ADT_ZEROP },
	{ opname[NMT_INC],ACT_WT | ADT_ZEROP },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_INX],ACT_NL | ADT_NONE },
	{ opname[NMT_SBC],ACT_NL | ADT_IMM },
	{ opname[NMT_NOP],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_CPX],ACT_RD | ADT_ABS },
	{ opname[NMT_SBC],ACT_RD | ADT_ABS },
	{ opname[NMT_INC],ACT_WT | ADT_ABS },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	// F0-FF
	{ opname[NMT_BEQ],ACT_NL | ADT_REL },
	{ opname[NMT_SBC],ACT_RD | ADT_POSTIND },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_SBC],ACT_RD | ADT_ZEROPX },
	{ opname[NMT_INC],ACT_WT | ADT_ZEROPX },/* Zeropx -> y に修正 */
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_SED],ACT_NL | ADT_NONE },
	{ opname[NMT_SBC],ACT_RD | ADT_ABSY },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_UND],ACT_NL | ADT_NONE },
	{ opname[NMT_SBC],ACT_RD | ADT_ABSX },
	{ opname[NMT_INC],ACT_WT | ADT_ABSX },
	{ opname[NMT_UND],ACT_NL | ADT_NONE }
};


#define ACT_RW ( ACT_RD | ACT_WT )

memargstr memcom7fff[] = {
#ifdef MESSAGETYPE_JAPANESE
/* Japanese */
	{ 0x2000,0x2000,ACT_RW,"PPU 制御Reg.#1"},
	{ 0x2001,0x2001,ACT_RW,"PPU 制御Reg.#2"},
	{ 0x2002,0x2002,ACT_RW,"PPU Status Reg."},
	{ 0x2003,0x2003,ACT_RW,"SPR-RAM Addr.Reg."},
	{ 0x2004,0x2004,ACT_RW,"SPR-RAM I/O Reg."},
	{ 0x2005,0x2005,ACT_RW,"BG Scrool Reg."},
	{ 0x2006,0x2006,ACT_RW,"VRAM Addr.Reg."},
	{ 0x2007,0x2007,ACT_RW,"VRAM I/O Reg."},
	{ 0x4000,0x4000,ACT_RW,"矩形波ch1 制御Reg.#1"},
	{ 0x4001,0x4001,ACT_RW,"矩形波ch1 制御Reg.#2"},
	{ 0x4002,0x4002,ACT_RW,"矩形波ch1 Freq. Reg.L"},
	{ 0x4003,0x4003,ACT_RW,"矩形波ch1 Freq. Reg.H"},
	{ 0x4004,0x4004,ACT_RW,"矩形波ch2 制御Reg.#1"},
	{ 0x4005,0x4005,ACT_RW,"矩形波ch2 制御Reg.#2"},
	{ 0x4006,0x4006,ACT_RW,"矩形波ch2 Freq. Reg.L"},
	{ 0x4007,0x4007,ACT_RW,"矩形波ch2 Freq. Reg.H"},
	{ 0x4008,0x4008,ACT_RW,"三角波ch3 制御Reg.#1"},
	{ 0x4009,0x4009,ACT_RW,"三角波ch3 制御Reg.#2"},
	{ 0x400A,0x400A,ACT_RW,"三角波ch3 Freq.Reg.L"},
	{ 0x400B,0x400B,ACT_RW,"三角波ch3 Freq.Reg.H"},
	{ 0x400C,0x400C,ACT_RW,"ノイズ波ch4 制御Reg.#1"},
	{ 0x400D,0x400D,ACT_RW,"ノイズ波ch4 制御Reg.#2"},
	{ 0x400E,0x400E,ACT_RW,"ノイズ波ch4 Freq.Reg.L"},
	{ 0x400F,0x400F,ACT_RW,"ノイズ波ch4 Freq.Reg.H"},
	{ 0x4010,0x4010,ACT_RW,"DPCM 制御Reg."},
	{ 0x4011,0x4011,ACT_RW,"DPCM 音量Reg."},
	{ 0x4012,0x4012,ACT_RW,"DPCM DMA-Addr.Reg."},
	{ 0x4013,0x4013,ACT_RW,"DPCM DMA-Length Reg."},
	{ 0x4014,0x4014,ACT_RW,"SPR-RAM DMA Reg."},
	{ 0x4015,0x4015,ACT_RW,"SoundChannel 制御Reg."},
	{ 0x4016,0x4016,ACT_RW,"Joy-Pad #1 制御Reg."},
	{ 0x4017,0x4017,ACT_RW,"Joy-Pad #2 制御Reg."},
	{ 0x408B,0x47FF,ACT_RW,"Unknown Register"},
	{ 0x6000,0x7FFF,ACT_RW,"Save/Work RAM"},
#else
/* English */
	{ 0x2000,0x2000,ACT_RW,"PPU CtrlReg.#1"},
	{ 0x2001,0x2001,ACT_RW,"PPU CtrlReg.#2"},
	{ 0x2002,0x2002,ACT_RW,"PPU Status Reg."},
	{ 0x2003,0x2003,ACT_RW,"SPR-RAM Addr.Reg."},
	{ 0x2004,0x2004,ACT_RW,"SPR-RAM I/O Reg."},
	{ 0x2005,0x2005,ACT_RW,"BG Scrool Reg."},
	{ 0x2006,0x2006,ACT_RW,"VRAM Addr.Reg."},
	{ 0x2007,0x2007,ACT_RW,"VRAM I/O Reg."},
	{ 0x4000,0x4000,ACT_RW,"SqrWave ch1 CtrlReg.#1"},
	{ 0x4001,0x4001,ACT_RW,"SqrWave ch1 CtrlReg.#2"},
	{ 0x4002,0x4002,ACT_RW,"SqrWave ch1 Freq. Reg.L"},
	{ 0x4003,0x4003,ACT_RW,"SqrWave ch1 Freq. Reg.H"},
	{ 0x4004,0x4004,ACT_RW,"SqrWave ch2 CtrlReg.#1"},
	{ 0x4005,0x4005,ACT_RW,"SqrWave ch2 CtrlReg.#2"},
	{ 0x4006,0x4006,ACT_RW,"SqrWave ch2 Freq. Reg.L"},
	{ 0x4007,0x4007,ACT_RW,"SqrWave ch2 Freq. Reg.H"},
	{ 0x4008,0x4008,ACT_RW,"TriWave ch3 CtrlReg.#1"},
	{ 0x4009,0x4009,ACT_RW,"TriWave ch3 CtrlReg.#2"},
	{ 0x400A,0x400A,ACT_RW,"TriWave ch3 Freq.Reg.L"},
	{ 0x400B,0x400B,ACT_RW,"TriWave ch3 Freq.Reg.H"},
	{ 0x400C,0x400C,ACT_RW,"NoizeWave ch4 CtrlReg.#1"},
	{ 0x400D,0x400D,ACT_RW,"NoizeWave ch4 CtrlReg.#2"},
	{ 0x400E,0x400E,ACT_RW,"NoizeWave ch4 Freq.Reg.L"},
	{ 0x400F,0x400F,ACT_RW,"NoizeWave ch4 Freq.Reg.H"},
	{ 0x4010,0x4010,ACT_RW,"DPCM CtrlReg."},
	{ 0x4011,0x4011,ACT_RW,"DPCM Vol.Reg."},
	{ 0x4012,0x4012,ACT_RW,"DPCM DMA-Addr.Reg."},
	{ 0x4013,0x4013,ACT_RW,"DPCM DMA-Length Reg."},
	{ 0x4014,0x4014,ACT_RW,"SPR-RAM DMA Reg."},
	{ 0x4015,0x4015,ACT_RW,"SoundChannel CtrlReg."},
	{ 0x4016,0x4016,ACT_RW,"Joy-Pad #1 CtrlReg."},
	{ 0x4017,0x4017,ACT_RW,"Joy-Pad #2 CtrlReg."},
	{ 0x408B,0x47FF,ACT_RW,"Unknown Register"},
	{ 0x6000,0x7FFF,ACT_RW,"Save/Work RAM"},
#endif // MESSAGETYPE
	{ 0xFFFF,0x0000,ACT_NL,NULL},
};


memargstr mapper_mmc1_com[] = {
	{ 0x8000,0x9FFF,ACT_WT,"MMC1 Reg#0" },
	{ 0xA000,0xBFFF,ACT_WT,"MMC1 Reg#1" },
	{ 0xC000,0xDFFF,ACT_WT,"MMC1 P$1000 4K BnkSlct" },
	{ 0xE000,0xFFFF,ACT_WT,"MMC1 $8000 16K BnkSlct" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_unrom_com[] = {
	{ 0x8000,0xFFFF,ACT_WT,"UNROM Select PRG-ROM Bank" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_cnrom_com[] = {
	{ 0x8000,0xFFFF,ACT_WT,"CNROM Select V-ROM Bank" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_mmc3_com[] = {
	{ 0x8000,0x8000,ACT_RW,"MMC3 Selector/Command Set" },
	{ 0x8001,0x8001,ACT_RW,"MMC3 SetPageNumber" },
	{ 0xA000,0xA000,ACT_RW,"MMC3 SelectMirroring" },
	{ 0xA001,0xA001,ACT_RW,"MMC3 SaveRAM Enabler" },
	{ 0xC000,0xC000,ACT_RW,"MMC3 IRQ Counter Reg." },
	{ 0xC001,0xC001,ACT_RW,"MMC3 IRQ Latch Reg." },
	{ 0xE000,0xE000,ACT_RW,"MMC3 IRQ dis" },
	{ 0xE001,0xE001,ACT_RW,"MMC3 IRQ en" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_mmc5_com[] = {
	{ 0x5000,0x5000,ACT_RW,"MMC5 pulse#1 CtrlReg." },
	{ 0x5002,0x5002,ACT_RW,"MMC5 pulse#1 FreqReg.L" },
	{ 0x5003,0x5003,ACT_RW,"MMC5 pulse#1 FreqReg.H" },
	{ 0x5004,0x5004,ACT_RW,"MMC5 pulse#2 CtrlReg." },
	{ 0x5006,0x5006,ACT_RW,"MMC5 pulse#2 FreqReg.L" },
	{ 0x5007,0x5007,ACT_RW,"MMC5 pulse#2 FreqReg.H" },
	{ 0x5010,0x5010,ACT_RW,"MMC5 PCMch OutputEn" },
	{ 0x5011,0x5011,ACT_RW,"MMC5 PCMch OutputData" },
	{ 0x5015,0x5015,ACT_RW,"MMC5 pulse#1/2 En/Dis" },
	{ 0x5100,0x5100,ACT_RW,"MMC5 PRG-ROM PageSize" },
	{ 0x5101,0x5101,ACT_RW,"MMC5 CHR-ROM PageSize" },
	{ 0x5102,0x5102,ACT_RW,"MMC5 BBR-RAM WrtPrtct#1" },
	{ 0x5103,0x5103,ACT_RW,"MMC5 BBR-RAM WrtPrtct#2" },
	{ 0x5104,0x5104,ACT_RW,"MMC5 $5C00 1K UsageReg." },
	{ 0x5105,0x5105,ACT_RW,"MMC5 Nametbl Slct" },
	{ 0x5106,0x5106,ACT_WT,"MMC5 FillMode Slct#1" },
	{ 0x5107,0x5107,ACT_WT,"MMC5 FillMode Slct#2" },
	{ 0x5113,0x5113,ACT_RW,"MMC5 $6000 BnkSlct" },
	{ 0x5114,0x5114,ACT_RW,"MMC5 $8000 BnkSlct" },
	{ 0x5115,0x5115,ACT_RW,"MMC5 $A000/8 BnkSlct" },
	{ 0x5116,0x5116,ACT_RW,"MMC5 $C000 BnkSlct" },
	{ 0x5117,0x5117,ACT_RW,"MMC5 $E000/C/8 BnkSlct" },
	{ 0x5120,0x5120,ACT_RW,"MMC5 A PPUBnkSlct#0" },
	{ 0x5121,0x5121,ACT_RW,"MMC5 A PPUBnkSlct#1" },
	{ 0x5122,0x5122,ACT_RW,"MMC5 A PPUBnkSlct#2" },
	{ 0x5123,0x5123,ACT_RW,"MMC5 A PPUBnkSlct#3" },
	{ 0x5124,0x5124,ACT_RW,"MMC5 A PPUBnkSlct#4" },
	{ 0x5125,0x5125,ACT_RW,"MMC5 A PPUBnkSlct#5" },
	{ 0x5126,0x5126,ACT_RW,"MMC5 A PPUBnkSlct#6" },
	{ 0x5127,0x5127,ACT_RW,"MMC5 A PPUBnkSlct#7" },
	{ 0x5128,0x5128,ACT_RW,"MMC5 B PPUBnkSlct#0" },
	{ 0x5129,0x5129,ACT_RW,"MMC5 B PPUBnkSlct#1" },
	{ 0x512A,0x512A,ACT_RW,"MMC5 B PPUBnkSlct#2" },
	{ 0x512B,0x512B,ACT_RW,"MMC5 B PPUBnkSlct#3" },
	{ 0x5130,0x5130,ACT_RW,"MMC5 ???" },
	{ 0x5200,0x5200,ACT_WT,"MMC5 Split Mode Ctrl" },
	{ 0x5201,0x5201,ACT_WT,"MMC5 Split Mode Reg0" },
	{ 0x5202,0x5202,ACT_WT,"MMC5 Split Mode Reg1" },
	{ 0x5203,0x5203,ACT_WT,"MMC5 Scanline BRKpt" },
	{ 0x5204,0x5204,ACT_RW,"MMC5 IRQ En/Dis" },
	{ 0x5205,0x5206,ACT_RW,"MMC5 Multiplier" },
	{ 0x5C00,0x5FBF,ACT_RW,"MMC5 ExGfx VRAM / WorkRam" },
	{ 0x5CF0,0x5CF0,ACT_RW,"MMC5 WorkRam" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_aorom_com[] = {
	{ 0x8000,0xFFFF,ACT_WT,"AOROM P-ROM BnkSlct/Mirror"},
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_ffef3_com[] = {
	{ 0x8000,0xFFFF,ACT_WT,"FFE F3xx V/P-ROM BnkSlct" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_mmc2_com[] = {
	{ 0xA000,0xAFFF,ACT_WT,"MMC2 $8000 8K BnkSlct" },
	{ 0xB000,0xCFFF,ACT_WT,"MMC2 P$0000 4K BnkSlct" },
	{ 0xD000,0xDFFF,ACT_WT,"MMC2 P$1000 $FD BnkSlct" },
	{ 0xE000,0xEFFF,ACT_WT,"MMC2 P$1000 $FE BnkSlct" },
	{ 0xF000,0xFFFF,ACT_WT,"MMC2 MirroringSlct" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_mmc4_com[] = {
	{ 0xA000,0xAFFF,ACT_WT,"MMC4 $8000 16K BnkSlct" },
	{ 0xB000,0xBFFF,ACT_WT,"MMC4 P$0000 $FD 4K BnkSlct" },
	{ 0xC000,0xCFFF,ACT_WT,"MMC4 P$0000 $FE 4K BnkSlct" },
	{ 0xD000,0xDFFF,ACT_WT,"MMC4 P$1000 $FD 4K BnkSlct" },
	{ 0xE000,0xEFFF,ACT_WT,"MMC4 P$1000 $FE 4K BnkSlct" },
	{ 0xF000,0xFFFF,ACT_WT,"MMC4 MirroringSlct" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_100in1_com[] = {
	{ 0x8000,0x8000,ACT_RW,"100in1 $8000 BnkSlct/Mirror/Swap" },
	{ 0x8001,0x8001,ACT_RW,"100in1 P-ROM BnkSlct/Swap" },
	{ 0x8002,0x8002,ACT_RW,"100in1 P-ROM SegmentBnkSlct "},
	{ 0x8003,0x8003,ACT_RW,"100in1 $C0P-ROM BnkSlct/Mirror/Swap" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_bandai_com[] = {
	{ 0x6000,0x6000,ACT_RW,"Bandai P$0000 1K BnkSlct" },
	{ 0x7FF0,0x7FF0,ACT_RW,"Bandai P$0000 1K BnkSlct" },
	{ 0x8000,0x8000,ACT_RW,"Bandai P$0000 1K BnkSlct" },
	{ 0x6001,0x6001,ACT_RW,"Bandai P$0400 1K BnkSlct" },
	{ 0x7FF1,0x7FF1,ACT_RW,"Bandai P$0400 1K BnkSlct" },
	{ 0x8001,0x8001,ACT_RW,"Bandai P$0400 1K BnkSlct" },
	{ 0x6002,0x6002,ACT_RW,"Bandai P$0800 1K BnkSlct" },
	{ 0x7FF2,0x7FF2,ACT_RW,"Bandai P$0800 1K BnkSlct" },
	{ 0x8002,0x8002,ACT_RW,"Bandai P$0800 1K BnkSlct" },
	{ 0x6003,0x6003,ACT_RW,"Bandai P$0C00 1K BnkSlct" },
	{ 0x7FF3,0x7FF3,ACT_RW,"Bandai P$0C00 1K BnkSlct" },
	{ 0x8003,0x8003,ACT_RW,"Bandai P$0C00 1K BnkSlct" },
	{ 0x6004,0x6004,ACT_RW,"Bandai P$1000 1K BnkSlct" },
	{ 0x7FF4,0x7FF4,ACT_RW,"Bandai P$1000 1K BnkSlct" },
	{ 0x8004,0x8004,ACT_RW,"Bandai P$1000 1K BnkSlct" },
	{ 0x6005,0x6005,ACT_RW,"Bandai P$1400 1K BnkSlct" },
	{ 0x7FF5,0x7FF5,ACT_RW,"Bandai P$1400 1K BnkSlct" },
	{ 0x8005,0x8005,ACT_RW,"Bandai P$1400 1K BnkSlct" },
	{ 0x6006,0x6006,ACT_RW,"Bandai P$1800 1K BnkSlct" },
	{ 0x7FF6,0x7FF6,ACT_RW,"Bandai P$1800 1K BnkSlct" },
	{ 0x8006,0x8006,ACT_RW,"Bandai P$1800 1K BnkSlct" },
	{ 0x6007,0x6007,ACT_RW,"Bandai P$1C00 1K BnkSlct" },
	{ 0x7FF7,0x7FF7,ACT_RW,"Bandai P$1C00 1K BnkSlct" },
	{ 0x8007,0x8007,ACT_RW,"Bandai P$1C00 1K BnkSlct" },
	{ 0x6008,0x6008,ACT_RW,"Bandai $8000 16K BnkSlct" },
	{ 0x7FF8,0x7FF8,ACT_RW,"Bandai $8000 16K BnkSlct" },
	{ 0x8008,0x8008,ACT_RW,"Bandai $8000 16K BnkSlct" },
	{ 0x6009,0x6009,ACT_RW,"Bandai MirrorSlct" },
	{ 0x7FF9,0x7FF9,ACT_RW,"Bandai MirrorSlct" },
	{ 0x8009,0x8009,ACT_RW,"Bandai MirrorSlct" },
	{ 0x600A,0x600A,ACT_RW,"Bandai IRQ Enabler" },
	{ 0x7FFA,0x7FFA,ACT_RW,"Bandai IRQ Enabler" },
	{ 0x800A,0x800A,ACT_RW,"Bandai IRQ Enabler" },
	{ 0x600B,0x600B,ACT_RW,"Bandai IRQ CntLo8" },
	{ 0x7FFB,0x7FFB,ACT_RW,"Bandai IRQ CntLo8" },
	{ 0x800B,0x800B,ACT_RW,"Bandai IRQ CntLo8" },
	{ 0x600C,0x600C,ACT_RW,"Bandai IRQ CntHi8" },
	{ 0x7FFC,0x7FFC,ACT_RW,"Bandai IRQ CntHi8" },
	{ 0x800C,0x800C,ACT_RW,"Bandai IRQ CntHi8" },
	{ 0x600D,0x600D,ACT_RW,"Bandai EPROM I/O ?" },
	{ 0x7FFD,0x7FFD,ACT_RW,"Bandai EPROM I/O ?" },
	{ 0x800D,0x800D,ACT_RW,"Bandai EPROM I/O ?" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_jalecoss_com[] = {
	{ 0x8000,0x8000,ACT_RW,"JalecoSS $8000 8K BnkSlct Lo4" },
	{ 0x8001,0x8001,ACT_RW,"JalecoSS $8000 8K BnkSlct Hi4" },
	{ 0x8002,0x8002,ACT_RW,"JalecoSS $A000 8K BnkSlct Lo4" },
	{ 0x8003,0x8003,ACT_RW,"JalecoSS $A000 8K BnkSlct Hi4" },
	{ 0x9000,0x9000,ACT_RW,"JalecoSS $C000 8K BnkSlct Lo4" },
	{ 0x9001,0x9001,ACT_RW,"JalecoSS $C000 8K BnkSlct Hi4" },
	{ 0xA000,0xA000,ACT_RW,"JalecoSS P$0000 1K BnkSlct Lo4" },
	{ 0xA001,0xA001,ACT_RW,"JalecoSS P$0000 1K BnkSlct Hi4" },
	{ 0xA002,0xA002,ACT_RW,"JalecoSS P$0400 1K BnkSlct Lo4" },
	{ 0xA003,0xA003,ACT_RW,"JalecoSS P$0400 1K BnkSlct Hi4" },
	{ 0xB000,0xB000,ACT_RW,"JalecoSS P$0800 1K BnkSlct Lo4" },
	{ 0xB001,0xB001,ACT_RW,"JalecoSS P$0800 1K BnkSlct Hi4" },
	{ 0xB002,0xB002,ACT_RW,"JalecoSS P$0C00 1K BnkSlct Lo4" },
	{ 0xB003,0xB003,ACT_RW,"JalecoSS P$0C00 1K BnkSlct Hi4" },
	{ 0xC000,0xC000,ACT_RW,"JalecoSS P$1000 1K BnkSlct Lo4" },
	{ 0xC001,0xC001,ACT_RW,"JalecoSS P$1000 1K BnkSlct Hi4" },
	{ 0xC002,0xC002,ACT_RW,"JalecoSS P$1400 1K BnkSlct Lo4" },
	{ 0xC003,0xC003,ACT_RW,"JalecoSS P$1400 1K BnkSlct Hi4" },
	{ 0xD000,0xD000,ACT_RW,"JalecoSS P$1800 1K BnkSlct Lo4" },
	{ 0xD001,0xD001,ACT_RW,"JalecoSS P$1800 1K BnkSlct Hi4" },
	{ 0xD002,0xD002,ACT_RW,"JalecoSS P$1C00 1K BnkSlct Lo4" },
	{ 0xD003,0xD003,ACT_RW,"JalecoSS P$1C00 1K BnkSlct Hi4" },
	{ 0xE000,0xE001,ACT_RW,"JalecoSS IRQ Cnt Lo8" },
	{ 0xE002,0xE003,ACT_RW,"JalecoSS IRQ Cnt Hi8" },
	{ 0xF000,0xF000,ACT_RW,"JalecoSS IRQ En" },
	{ 0xF001,0xF001,ACT_RW,"JalecoSS IRQ En/Dis" },
	{ 0xF002,0xF002,ACT_RW,"JalecoSS Mirroring" },
	{ 0xF003,0xF003,ACT_RW,"JalecoSS Ext-I/O Port? " },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_namcot106_com[] = {
	{ 0x4800,0x4800,ACT_RW,"Namcot106 ExI/O DataReg."},
	{ 0x5000,0x57FF,ACT_RW,"Namcot106 IRQ Cnt Lo8" },
	{ 0x5800,0x5FFF,ACT_RW,"Namcot106 IRQ Cnt Hi8/En" },
	{ 0x8000,0x87FF,ACT_WT,"Namcot106 P$0000 1K BnkSlct" },
	{ 0x8800,0x8FFF,ACT_WT,"Namcot106 P$0400 1K BnkSlct" },
	{ 0x9000,0x97FF,ACT_WT,"Namcot106 P$0800 1K BnkSlct" },
	{ 0x9000,0x97FF,ACT_WT,"Namcot106 P$0C00 1K BnkSlct" },
	{ 0xA000,0xA7FF,ACT_WT,"Namcot106 P$1000 1K BnkSlct" },
	{ 0xA800,0xAFFF,ACT_WT,"Namcot106 P$1400 1K BnkSlct" },
	{ 0xB000,0xB7FF,ACT_WT,"Namcot106 P$1800 1K BnkSlct" },
	{ 0xB800,0xBFFF,ACT_WT,"Namcot106 P$1C00 1K BnkSlct" },
	{ 0xC000,0xC7FF,ACT_WT,"Namcot106 P$2000 1K BnkSlct" },
	{ 0xC800,0xCFFF,ACT_WT,"Namcot106 P$2400 1K BnkSlct" },
	{ 0xD000,0xD7FF,ACT_WT,"Namcot106 P$2800 1K BnkSlct" },
	{ 0xD800,0xDFFF,ACT_WT,"Namcot106 P$2C00 1K BnkSlct" },
	{ 0xE000,0xE7FF,ACT_WT,"Namcot106 $8000 8K BnkSlct" },
	{ 0xE800,0xEFFF,ACT_WT,"Namcot106 $A000 8K BnkSlct" },
	{ 0xF000,0xF7FF,ACT_WT,"Namcot106 $C000 8K BnkSlct" },
	{ 0xF800,0xF800,ACT_WT,"Namcot106 ExI/O AddrReg." },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_vrc4_com[] = {
	{ 0x8000,0x8000,ACT_RW,"VRC4 $8/C000 8K BnkSlct" },
	{ 0x9000,0x9000,ACT_RW,"VRC4 Mirroring" },
	{ 0x9002,0x9002,ACT_RW,"VRC4 SaveRAM Enabler / Bnk$8/C" },
	{ 0x9003,0x9003,ACT_RW,"VRC4 Ext-I/O Port ?" },
	{ 0xA000,0xA000,ACT_RW,"VRC4 $A000 8K BnkSlct" },
	{ 0xB000,0xB000,ACT_RW,"VRC4 P$0000 1K BnkSlct Lo4" },
	{ 0xB001,0xB001,ACT_RW,"VRC4 P$0400 1K BnkSlct Lo4" },
	{ 0xB002,0xB002,ACT_RW,"VRC4 P$0000 1K BnkSlct Hi4" },
	{ 0xB003,0xB003,ACT_RW,"VRC4 P$0400 1K BnkSlct Hi4" },
	{ 0xB004,0xB004,ACT_RW,"VRC4 P$0400 1K BnkSlct Lo4" },
	{ 0xB006,0xB006,ACT_RW,"VRC4 P$0400 1K BnkSlct Hi4" },
	{ 0xC000,0xC000,ACT_RW,"VRC4 P$0800 1K BnkSlct Lo4" },
	{ 0xC001,0xC001,ACT_RW,"VRC4 P$0C00 1K BnkSlct Lo4" },
	{ 0xC002,0xC002,ACT_RW,"VRC4 P$0800 1K BnkSlct Hi4" },
	{ 0xC003,0xC003,ACT_RW,"VRC4 P$0C00 1K BnkSlct Hi4" },
	{ 0xC004,0xC004,ACT_RW,"VRC4 P$0C00 1K BnkSlct Lo4" },
	{ 0xC006,0xC006,ACT_RW,"VRC4 P$0C00 1K BnkSlct Hi4" },
	{ 0xD000,0xD000,ACT_RW,"VRC4 P$1000 1K BnkSlct Lo4" },
	{ 0xD001,0xD001,ACT_RW,"VRC4 P$1400 1K BnkSlct Lo4" },
	{ 0xD002,0xD002,ACT_RW,"VRC4 P$1000 1K BnkSlct Hi4" },
	{ 0xD003,0xD003,ACT_RW,"VRC4 P$1400 1K BnkSlct Hi4" },
	{ 0xD004,0xD004,ACT_RW,"VRC4 P$1400 1K BnkSlct Lo4" },
	{ 0xD006,0xD006,ACT_RW,"VRC4 P$1400 1K BnkSlct Hi4" },
	{ 0xE000,0xE000,ACT_RW,"VRC4 P$1800 1K BnkSlct Lo4" },
	{ 0xE001,0xE001,ACT_RW,"VRC4 P$1C00 1K BnkSlct Lo4" },
	{ 0xE002,0xE002,ACT_RW,"VRC4 P$1800 1K BnkSlct Hi4" },
	{ 0xE003,0xE003,ACT_RW,"VRC4 P$1C00 1K BnkSlct Hi4" },
	{ 0xE004,0xE004,ACT_RW,"VRC4 P$1C00 1K BnkSlct Lo4" },
	{ 0xE006,0xE006,ACT_RW,"VRC4 P$1C00 1K BnkSlct Hi4" },
	{ 0xF000,0xF001,ACT_RW,"VRC4 IRQ CntReg." },
	{ 0xF002,0xF002,ACT_RW,"VRC4 IRQ En/Dis" },
	{ 0xF003,0xF003,ACT_RW,"VRC4 IRQ CntReset" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_vrc42a_com[] = {
	// 資料がないので推測で書いてみる
//	{ 0x8000,0x8000,ACT_RW,"VRC42A $8/C000 8K BnkSlct" },
	{ 0x9000,0x9000,ACT_RW,"VRC42A Mirroring" },
//	{ 0x9002,0x9002,ACT_RW,"VRC42A SaveRAM Enabler / Bnk$8/C" },
//	{ 0x9003,0x9003,ACT_RW,"VRC42A Ext-I/O Port ?" },
//	{ 0xA000,0xA000,ACT_RW,"VRC42A $A000 8K BnkSlct" },
	{ 0xB000,0xB000,ACT_RW,"VRC42A P$0000 1K BnkSlct Lo4" },
	{ 0xB040,0xB040,ACT_RW,"VRC42A P$0000 1K BnkSlct Hi4" },
	{ 0xB080,0xB080,ACT_RW,"VRC42A P$0400 1K BnkSlct Lo4" },
	{ 0xB0C0,0xB0C0,ACT_RW,"VRC42A P$0400 1K BnkSlct Hi4" },
	{ 0xC000,0xC000,ACT_RW,"VRC42A P$0800 1K BnkSlct Lo4" },
	{ 0xC040,0xC040,ACT_RW,"VRC42A P$0800 1K BnkSlct Hi4" },
	{ 0xC080,0xC080,ACT_RW,"VRC42A P$0C00 1K BnkSlct Lo4" },
	{ 0xC0C0,0xC0C0,ACT_RW,"VRC42A P$0C00 1K BnkSlct Hi4" },
	{ 0xD000,0xD000,ACT_RW,"VRC42A P$1000 1K BnkSlct Lo4" },
	{ 0xD040,0xD040,ACT_RW,"VRC42A P$1000 1K BnkSlct Hi4" },
	{ 0xD080,0xD080,ACT_RW,"VRC42A P$1400 1K BnkSlct Lo4" },
	{ 0xD0C0,0xD0C0,ACT_RW,"VRC42A P$1400 1K BnkSlct Hi4" },
	{ 0xE000,0xE000,ACT_RW,"VRC42A P$1800 1K BnkSlct Lo4" },
	{ 0xE040,0xE040,ACT_RW,"VRC42A P$1800 1K BnkSlct Hi4" },
	{ 0xE050,0xE050,ACT_RW,"VRC42A P$1C00 1K BnkSlct Lo4" },
	{ 0xE0C0,0xE0C0,ACT_RW,"VRC42A P$1C00 1K BnkSlct Hi4" },
	{ 0xF000,0xF000,ACT_RW,"VRC42A $8000 8K BnkSlct??" },
	{ 0xF040,0xF040,ACT_RW,"VRC42A $A000 8K BnkSlct??" },
	{ 0xF080,0xF080,ACT_RW,"VRC42A BnkSwitch??" },
	{ 0xF0C0,0xF0C0,ACT_RW,"VRC42A SaveRAM Enabler??" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_vrc2a_com[] = {
	{ 0x8000,0x8000,ACT_RW,"VRC2A $8000 8K BnkSlct" },
	{ 0x9000,0x9000,ACT_RW,"VRC2A Mirroring" },
	{ 0xA000,0xA000,ACT_RW,"VRC2A $A000 8K BnkSlct" },
	{ 0xB000,0xB000,ACT_RW,"VRC2A P$0000 1K BnkSlct" },
	{ 0xB001,0xB001,ACT_RW,"VRC2A P$0400 1K BnkSlct" },
	{ 0xC000,0xC000,ACT_RW,"VRC2A P$0800 1K BnkSlct" },
	{ 0xC001,0xC001,ACT_RW,"VRC2A P$0C00 1K BnkSlct" },
	{ 0xD000,0xD000,ACT_RW,"VRC2A P$1000 1K BnkSlct" },
	{ 0xD001,0xD001,ACT_RW,"VRC2A P$1400 1K BnkSlct" },
	{ 0xE000,0xE000,ACT_RW,"VRC2A P$1800 1K BnkSlct" },
	{ 0xE001,0xE001,ACT_RW,"VRC2A P$1C00 1K BnkSlct" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_vrc2b_com[] = {
	{ 0x8000,0x8000,ACT_WT,"VRC2B $8/C000 8K BnkSlct" },
	{ 0x8004,0x8004,ACT_WT,"VRC2B $8/C000 8K BnkSlct" },
	{ 0x8008,0x8008,ACT_WT,"VRC2B $8/C000 8K BnkSlct" },
	{ 0x800C,0x800C,ACT_WT,"VRC2B $8/C000 8K BnkSlct" },
	{ 0x8FFF,0x8FFF,ACT_WT,"VRC2B $8/C000 8K BnkSlct" },
	{ 0x9000,0x9000,ACT_RW,"VRC2B Mirroring" },
	{ 0x9008,0x9008,ACT_WT,"VRC2B $8000/$C000 Swap" },
	{ 0xA000,0xA000,ACT_WT,"VRC2B $A000 8K BnkSlct" },
	{ 0xA004,0xA004,ACT_WT,"VRC2B $A000 8K BnkSlct" },
	{ 0xA008,0xA008,ACT_WT,"VRC2B $A000 8K BnkSlct" },
	{ 0xA00C,0xA00C,ACT_WT,"VRC2B $A000 8K BnkSlct" },
	{ 0xAFFF,0xAFFF,ACT_WT,"VRC2B $A000 8K BnkSlct" },
	{ 0xB000,0xB000,ACT_WT,"VRC2B P$0000 1K BnkSlct Lo4" },
	{ 0xB004,0xB004,ACT_WT,"VRC2B P$0000 1K BnkSlct Hi4" },
	{ 0xB008,0xB008,ACT_WT,"VRC2B P$0400 1K BnkSlct Lo4" },
	{ 0xB00C,0xB00C,ACT_WT,"VRC2B P$0400 1K BnkSlct Hi4" },
	{ 0xC000,0xC000,ACT_WT,"VRC2B P$0800 1K BnkSlct Lo4" },
	{ 0xC004,0xC004,ACT_WT,"VRC2B P$0800 1K BnkSlct Hi4" },
	{ 0xC008,0xC008,ACT_WT,"VRC2B P$0C00 1K BnkSlct Lo4" },
	{ 0xC00C,0xC00C,ACT_WT,"VRC2B P$0C00 1K BnkSlct Hi4" },
	{ 0xD000,0xD000,ACT_WT,"VRC2B P$1000 1K BnkSlct Lo4" },
	{ 0xD004,0xD004,ACT_WT,"VRC2B P$1000 1K BnkSlct Hi4" },
	{ 0xD008,0xD008,ACT_WT,"VRC2B P$1400 1K BnkSlct Lo4" },
	{ 0xD00C,0xD00C,ACT_WT,"VRC2B P$1400 1K BnkSlct Hi4" },
	{ 0xE000,0xE000,ACT_WT,"VRC2B P$1800 1K BnkSlct Lo4" },
	{ 0xE004,0xE004,ACT_WT,"VRC2B P$1800 1K BnkSlct Hi4" },
	{ 0xE008,0xE008,ACT_WT,"VRC2B P$1C00 1K BnkSlct Lo4" },
	{ 0xE00C,0xE00C,ACT_WT,"VRC2B P$1C00 1K BnkSlct Hi4" },
	{ 0xF000,0xF000,ACT_WT,"VRC2B IRQ Cnt Lo" },
	{ 0xF004,0xF004,ACT_WT,"VRC2B IRQ Cnt Hi" },
	{ 0xF008,0xF008,ACT_WT,"VRC2B IRQ 1:CntEn/Dis 0:$F00Cen/dis" },
	{ 0xF00C,0xF00C,ACT_WT,"VRC2B IRQ En/Dis" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_vrc6_com[] = {
	{ 0x8000,0x8000,ACT_RW,"VRC6 $8000 16K BnkSlct" },
	{ 0x9000,0x9000,ACT_WT,"VRC6 pulse#1 Duty/Vol/Gate" },
	{ 0x9001,0x9001,ACT_WT,"VRC6 pulse#1 Freq.Low8bit" },
	{ 0x9002,0x9002,ACT_WT,"VRC6 pulse#1 Freq.Up4bit/En" },
	{ 0xA000,0xA000,ACT_WT,"VRC6 pulse#2 Duty/Vol/Gate" },
	{ 0xA001,0xA001,ACT_WT,"VRC6 pulse#2 Freq.Low8bit" },
	{ 0xA002,0xA002,ACT_WT,"VRC6 pulse#2 Freq.Up4bit/En" },
	{ 0xB000,0xB000,ACT_WT,"VRC6 sawtooth Phase acc." },
	{ 0xB001,0xB001,ACT_WT,"VRC6 sawtooth Freq.Low8bit" },
	{ 0xB002,0xB002,ACT_WT,"VRC6 sawtooth Freq.Up4bit/En" },
	{ 0xB003,0xB003,ACT_RW,"VRC6 Mirroring" },
	{ 0xC000,0xC000,ACT_RW,"VRC6 $C000 8K BnkSlct" },
	{ 0xD000,0xD000,ACT_RW,"VRC6 P$0000 1K BnkSlct" },
	{ 0xD001,0xD001,ACT_RW,"VRC6 P$0400 1K BnkSlct" },
	{ 0xD002,0xD002,ACT_RW,"VRC6 P$0800 1K BnkSlct" },
	{ 0xD003,0xD003,ACT_RW,"VRC6 P$0C00 1K BnkSlct" },
	{ 0xE000,0xE000,ACT_RW,"VRC6 P$1000 1K BnkSlct" },
	{ 0xE001,0xE001,ACT_RW,"VRC6 P$1400 1K BnkSlct" },
	{ 0xE002,0xE002,ACT_RW,"VRC6 P$1800 1K BnkSlct" },
	{ 0xE003,0xE003,ACT_RW,"VRC6 P$1C00 1K BnkSlct" },
	{ 0xF000,0xF000,ACT_RW,"VRC6 IRQ CntReg." },
	{ 0xF001,0xF001,ACT_RW,"VRC6 IRQ En/Dis" },
	{ 0xF002,0xF002,ACT_RW,"VRC6 IRQ CntReset" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_vrc7_com[] = {
	{ 0x8000,0x8000,ACT_WT,"VRC7 $8000 8K BnkSlct" },
	{ 0x8010,0x8010,ACT_WT,"VRC7 $A000 8K BnkSlct" },
	{ 0x9000,0x9000,ACT_WT,"VRC7 $C000 8K BnkSlct" },
	{ 0x9010,0x9010,ACT_WT,"VRC7 ExSound Addr." },
	{ 0x9030,0x9030,ACT_WT,"VRC7 ExSound I/O" },
	{ 0xA000,0xA000,ACT_WT,"VRC7 P$0000 1K BnkSlct" },
	{ 0xA010,0xA010,ACT_WT,"VRC7 P$0400 1K BnkSlct" },
	{ 0xB000,0xB000,ACT_WT,"VRC7 P$0800 1K BnkSlct" },
	{ 0xB010,0xB010,ACT_WT,"VRC7 P$0C00 1K BnkSlct" },
	{ 0xC000,0xC000,ACT_WT,"VRC7 P$1000 1K BnkSlct" },
	{ 0xC010,0xC010,ACT_WT,"VRC7 P$1400 1K BnkSlct" },
	{ 0xD000,0xD000,ACT_WT,"VRC7 P$1800 1K BnkSlct" },
	{ 0xD010,0xD010,ACT_WT,"VRC7 P$1C00 1K BnkSlct" },
	{ 0xE000,0xE000,ACT_WT,"VRC7 Mirroring" },
	{ 0xE010,0xE010,ACT_WT,"VRC7 IRQ CntSet" },
	{ 0xF000,0xF000,ACT_WT,"VRC7 IRQ En/DisFlag" },
	{ 0xF010,0xF010,ACT_WT,"VRC7 IRQ En/DisPort" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_vrc1_com[] = {
	{ 0x8000,0x8FFF,ACT_WT,"VRC1 $8000 8K BnkSlct" },
	{ 0xA000,0xAFFF,ACT_WT,"VRC1 $A000 8K BnkSlct" },
	{ 0xD000,0xDFFF,ACT_WT,"VRC1 $C000 8K BnkSlct" },
	{ 0xE000,0xEFFF,ACT_WT,"VRC1 P$0000 4K BnkSlct" },
	{ 0xF000,0xFFFF,ACT_WT,"VRC1 P$1000 4K BnkSlct" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_g101_com[] = {
	{ 0x8FFF,0x8FFF,ACT_RW,"IREMG101 $8/C000 8K BnkSlct" },
	{ 0x9FFF,0x9FFF,ACT_RW,"IREMG101 Mirror/ $8/CSwitch" },
	{ 0xAFFF,0xAFFF,ACT_RW,"IREMG101 $A000 8K BnkSlct" },
	{ 0xBFF0,0xBFF0,ACT_RW,"IREMG101 P$0000 1K BnkSlct" },
	{ 0xBFF1,0xBFF1,ACT_RW,"IREMG101 P$0400 1K BnkSlct" },
	{ 0xBFF2,0xBFF2,ACT_RW,"IREMG101 P$0800 1K BnkSlct" },
	{ 0xBFF3,0xBFF3,ACT_RW,"IREMG101 P$0C00 1K BnkSlct" },
	{ 0xBFF4,0xBFF4,ACT_RW,"IREMG101 P$1000 1K BnkSlct" },
	{ 0xBFF5,0xBFF5,ACT_RW,"IREMG101 P$1400 1K BnkSlct" },
	{ 0xBFF6,0xBFF6,ACT_RW,"IREMG101 P$1800 1K BnkSlct" },
	{ 0xBFF7,0xBFF7,ACT_RW,"IREMG101 P$1C00 1K BnkSlct" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_taitotc_com[] = {
	{ 0x8000,0x8000,ACT_RW,"TaitoTC $8000 8K BnkSlct" },
	{ 0x8001,0x8001,ACT_RW,"TaitoTC $A000 8K BnkSlct" },
	{ 0x8002,0x8002,ACT_RW,"TaitoTC P$0000 2K BnkSlct" },
	{ 0x8003,0x8003,ACT_RW,"TaitoTC P$0800 2K BnkSlct" },
	{ 0xA000,0xA000,ACT_RW,"TaitoTC P$1000 1K BnkSlct" },
	{ 0xA001,0xA001,ACT_RW,"TaitoTC P$1400 1K BnkSlct" },
	{ 0xA002,0xA002,ACT_RW,"TaitoTC P$1800 1K BnkSlct" },
	{ 0xA003,0xA003,ACT_RW,"TaitoTC P$1C00 1K BnkSlct" },
	{ 0xC000,0xC000,ACT_RW,"TaitoTC RasterIRQ CntSet" },
	{ 0xC001,0xC002,ACT_RW,"TaitoTC RasterIRQ En/Dis" },
	{ 0xC001,0xC002,ACT_RW,"TaitoTC ChrPageMapSW" },
	{ 0xE000,0xE000,ACT_RW,"TaitoTC Mirroring" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_gnrom_com[] = {
	{ 0x8000,0xFFFF,ACT_WT,"GNROM P$0000 8K / $8000 32K BnkSlct" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_sunmap4_com[] = {
	{ 0x8000,0x8000,ACT_RW,"SunMap4 P$0000 2K BnkSlct" },
	{ 0x9000,0x9000,ACT_RW,"SunMap4 P$0800 2K BnkSlct" },
	{ 0xA000,0xA000,ACT_RW,"SunMap4 P$1000 2K BnkSlct" },
	{ 0xB000,0xB000,ACT_RW,"SunMap4 P$1800 2K BnkSlct" },
	{ 0xC000,0xC000,ACT_RW,"SunMap4 P$2000 1K BnkSlct" },
	{ 0xD000,0xD000,ACT_RW,"SunMap4 P$2400 1K BnkSlct" },
	{ 0xE000,0xE000,ACT_RW,"SunMap4 Mirroring" },
	{ 0xF000,0xF000,ACT_RW,"SunMap4 $8000 16K BnkSlct" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_sunearlym_com[] = {
	{ 0x8000,0xFFFF,ACT_WT,"SunEM cpppmccc c:P$0BS p:$8BS m:Mir"},
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_sunfme7_com[] = {
	{ 0x8000,0x8000,ACT_RW,"SunFME7 Reg#0" },
	{ 0xA000,0xA000,ACT_RW,"SunFME7 Reg#1 BnkSlct" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_irem74hc_com[] = {
	{ 0x8000,0xFFFF,ACT_WT,"IREM74HC $8000 16K / P$0000 8K BnkSlct" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_fds_com[] = {
#ifdef MESSAGETYPE_JAPANESE
	/* Japanese */
	{ 0x4020,0x4020,ACT_RW,"FDS TimerIRQ 制御Reg. #1"},
	{ 0x4021,0x4021,ACT_RW,"FDS TimerIRQ 制御Reg. #2"},
	{ 0x4022,0x4022,ACT_RW,"FDS TimerIRQ 制御Reg. #3"},
	{ 0x4023,0x4023,ACT_RW,"FDS TimerIRQ 制御Reg. #4"},
	{ 0x4024,0x4024,ACT_RW,"FDS Port 出力 Reg."},
	{ 0x4025,0x4025,ACT_RW,"FDS 制御Reg. 入力"},
	{ 0x4026,0x4026,ACT_RW,"FDS 拡張Port 出力"},
	{ 0x4030,0x4030,ACT_RW,"FDS DiskStatus Reg. #1"},
	{ 0x4031,0x4031,ACT_RW,"FDS PortInput Reg."},
	{ 0x4032,0x4032,ACT_RW,"FDS DiskStatus Reg. #2"},
	{ 0x4033,0x4033,ACT_RW,"FDS 拡張Port 入力"},
	{ 0x4040,0x407F,ACT_RW,"FDS PM音源 WaveData"},
	{ 0x4080,0x4080,ACT_RW,"FDS PM音源 音量制御Reg."},
	{ 0x4082,0x4082,ACT_RW,"FDS PM音源 Freq.Reg.L"},
	{ 0x4083,0x4083,ACT_RW,"FDS PM音源 Freq.Reg.H"},
	{ 0x4084,0x4084,ACT_RW,"FDS PM音源 LFO制御Reg."},
	{ 0x4085,0x4085,ACT_RW,"FDS PM音源 LFO OffsetReg."},
	{ 0x4086,0x4086,ACT_RW,"FDS PM音源 LFO SpeedReg.L"},
	{ 0x4087,0x4087,ACT_RW,"FDS PM音源 LFO SpeedReg.H"},
	{ 0x4088,0x4088,ACT_RW,"FDS PM音源 PortamentSpeed"},
	{ 0x4089,0x4089,ACT_RW,"FDS PM音源 発音制御Reg."},
	{ 0x408A,0x408A,ACT_RW,"FDS ???"},
#else
	/* English */
	{ 0x4020,0x4020,ACT_RW,"FDS TimerIRQ CtrlReg. #1"},
	{ 0x4021,0x4021,ACT_RW,"FDS TimerIRQ CtrlReg. #2"},
	{ 0x4022,0x4022,ACT_RW,"FDS TimerIRQ CtrlReg. #3"},
	{ 0x4023,0x4023,ACT_RW,"FDS TimerIRQ CtrlReg. #4"},
	{ 0x4024,0x4024,ACT_RW,"FDS Port Output Reg."},
	{ 0x4025,0x4025,ACT_RW,"FDS CtrlReg. Input"},
	{ 0x4026,0x4026,ACT_RW,"FDS ExPort Output"},
	{ 0x4030,0x4030,ACT_RW,"FDS DiskStatus Reg. #1"},
	{ 0x4031,0x4031,ACT_RW,"FDS PortInput Reg."},
	{ 0x4032,0x4032,ACT_RW,"FDS DiskStatus Reg. #2"},
	{ 0x4033,0x4033,ACT_RW,"FDS ExPort Input"},
	{ 0x4040,0x407F,ACT_RW,"FDS PMSndSrc WaveData"},
	{ 0x4080,0x4080,ACT_RW,"FDS PMSndSrc Vol.CtrlReg."},
	{ 0x4082,0x4082,ACT_RW,"FDS PMSndSrc Freq.Reg.L"},
	{ 0x4083,0x4083,ACT_RW,"FDS PMSndSrc Freq.Reg.H"},
	{ 0x4084,0x4084,ACT_RW,"FDS PMSndSrc LFOCtrlReg."},
	{ 0x4085,0x4085,ACT_RW,"FDS PMSndSrc LFO OffsetReg."},
	{ 0x4086,0x4086,ACT_RW,"FDS PMSndSrc LFO SpeedReg.L"},
	{ 0x4087,0x4087,ACT_RW,"FDS PMSndSrc LFO SpeedReg.H"},
	{ 0x4088,0x4088,ACT_RW,"FDS PMSndSrc PortamentSpeed"},
	{ 0x4089,0x4089,ACT_RW,"FDS PMSndSrc KeyonCtrlReg."},
	{ 0x408A,0x408A,ACT_RW,"FDS ???"},
#endif // MESSAGETYPE
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_madoola_com[] = {
	// SUNSOFT マドゥーラの翼で見られたマッパー
	{ 0x6000,0x6000,ACT_WT,"SUNMDL PPU BnkSlct??" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_nsf_com[] = {
	// for NSF Ripper マッパーの一種にする方がいいのかな...？
	{ 0x5FF8,0x5FF8,ACT_RW,"NSF $8000 4K BnkSlct" },
	{ 0x5FF9,0x5FF9,ACT_RW,"NSF $9000 4K BnkSlct" },
	{ 0x5FFA,0x5FFA,ACT_RW,"NSF $A000 4K BnkSlct" },
	{ 0x5FFB,0x5FFB,ACT_RW,"NSF $B000 4K BnkSlct" },
	{ 0x5FFC,0x5FFC,ACT_RW,"NSF $C000 4K BnkSlct" },
	{ 0x5FFD,0x5FFD,ACT_RW,"NSF $D000 4K BnkSlct" },
	{ 0x5FFE,0x5FFE,ACT_RW,"NSF $E000 4K BnkSlct" },
	{ 0x5FFF,0x5FFF,ACT_RW,"NSF $F000 4K BnkSlct" },
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

memargstr mapper_undefined_com[] = {
#ifdef MESSAGETYPE_JAPANESE
	{ 0x8000,0xFFFF,ACT_WT,"バンク切り替え？"},
#else
	{ 0x8000,0xFFFF,ACT_WT,"BnkSlct?"},
#endif // MESSAGETYPE
	{ 0xFFFF,0x0000,ACT_NL,NULL },
};

#define MAX_MAPPERS 8
#define MAX_MAPPERNUMS 256

memargstr *mapcom[MAX_MAPPERNUMS+1] = {
	mapper_undefined_com,		// 00 No Mapper
	mapper_mmc1_com,			// 01 MMC1
	mapper_unrom_com,			// 02 UNROM
	mapper_cnrom_com,			// 03 CNROM
	mapper_mmc3_com,			// 04 MMC3
	mapper_mmc5_com,			// 05 MMC5
	mapper_undefined_com,		// 06 FFE F4xxx
	mapper_aorom_com,			// 07 AOROM
	mapper_ffef3_com,			// 08 FFE F3xxx
	mapper_mmc2_com,			// 09 MMC2
	mapper_mmc4_com,			// 0A MMC4
	mapper_undefined_com,		// 0B Color Dreams
	mapper_undefined_com,		// 0C ?
	mapper_undefined_com,		// 0D ?
	mapper_undefined_com,		// 0E ?
	mapper_100in1_com,			// 0F 100-in-1
	mapper_bandai_com,			// 10 Bandai
	mapper_undefined_com,		// 11 FFE F8xxx
	mapper_jalecoss_com,		// 12 Jaleco SS8806
	mapper_namcot106_com,		// 13 Namcot 106
	mapper_fds_com,				// 14 Famicom Disk System (Ando? Mapper)
	mapper_vrc42a_com,			// 15 Konami VRC4 2A
	mapper_vrc2a_com,			// 16 Konami VRC2 Type A ( VRC4 1B )
	mapper_vrc2b_com,			// 17 Konami VRC2 Type B ( VRC4 1A )
	mapper_vrc6_com,			// 18 Konami VRC6
	mapper_vrc4_com,			// 19 Konami VRC4
	mapper_undefined_com,		// 1A Konami VRC6V
	mapper_undefined_com,		// 1B ?
	mapper_undefined_com,		// 1C ?
	mapper_undefined_com,		// 1D ?
	mapper_undefined_com,		// 1E ?
	mapper_undefined_com,		// 1F ?
	mapper_g101_com,			// 20 IREM G-101
	mapper_taitotc_com,			// 21 Taito TC0190
	mapper_undefined_com,		// 22 Nina-1
	mapper_undefined_com,		// 23 ?
	mapper_undefined_com,		// 24 ?
	mapper_undefined_com,		// 25 ?
	mapper_undefined_com,		// 26 ?
	mapper_undefined_com,		// 27 ?
	mapper_undefined_com,		// 28 ?
	mapper_undefined_com,		// 29 ?
	mapper_undefined_com,		// 2A ?
	mapper_undefined_com,		// 2B ?
	mapper_undefined_com,		// 2C ?
	mapper_undefined_com,		// 2D ?
	mapper_undefined_com,		// 2E ?
	mapper_undefined_com,		// 2F ?
	mapper_undefined_com,		// 30 Taito TC190V
	mapper_undefined_com,		// 31 ?
	mapper_undefined_com,		// 32 ?
	mapper_undefined_com,		// 33 ?
	mapper_undefined_com,		// 34 ?
	mapper_undefined_com,		// 35 ?
	mapper_undefined_com,		// 36 ?
	mapper_undefined_com,		// 37 ?
	mapper_undefined_com,		// 38 ?
	mapper_undefined_com,		// 39 ?
	mapper_undefined_com,		// 3A ?
	mapper_undefined_com,		// 3B ?
	mapper_undefined_com,		// 3C ?
	mapper_undefined_com,		// 3D ?
	mapper_undefined_com,		// 3E ?
	mapper_undefined_com,		// 3F ?
	mapper_undefined_com,		// 40 Tengen RAMBO-1
	mapper_undefined_com,		// 41 IREM H-3001
	mapper_gnrom_com,			// 42 GNROM
	mapper_undefined_com,		// 43 Sunsoft Mapper #3
	mapper_sunmap4_com,			// 44 Sunsoft Mapper #4
	mapper_sunfme7_com,			// 45 Sunsoft FME-7 ( Mapper #5 )
	mapper_undefined_com,		// 46 74161/32
	mapper_undefined_com,		// 47 Cameria
	mapper_undefined_com,		// 48 Jaleco Early Mapper #0
	mapper_undefined_com,		// 49 Konami VRC3
	mapper_undefined_com,		// 4A Taiwan MMC3
	mapper_vrc1_com,			// 4B Jaleco Mapper SS8805 / Konami VRC1
	mapper_undefined_com,		// 4C Namcot 109
	mapper_undefined_com,		// 4D Irem Early Mapper #0
	mapper_irem74hc_com,		// 4E Irem 74HC161/32
	mapper_undefined_com,		// 4F American Video Entertainment
	mapper_undefined_com,		// 50 Taito X-005
	mapper_undefined_com,		// 51 Taito C075
	mapper_undefined_com,		// 52 Taito X1-17
	mapper_undefined_com,		// 53 PC-Cony Mapper
	mapper_undefined_com,		// 54 PC-SMBJ2
	mapper_vrc7_com,			// 55 Konami VRC7
	mapper_undefined_com,		// 56 Jaleco Early Mapper #2
	mapper_undefined_com,		// 57 74161/32
	mapper_undefined_com,		// 58 Namco 118
	mapper_sunearlym_com,		// 59 Sunsoft Early Mapper
	mapper_undefined_com,		// 5A PC-JY-??
	mapper_undefined_com,		// 5B PC-HK-SF3
	mapper_undefined_com,		// 5C Jaleco Early Mapper #1
	mapper_undefined_com,		// 5D 74161/32
	mapper_undefined_com,		// 5E 74161/32
	mapper_undefined_com,		// 5F Namcot 1??
	mapper_undefined_com,		// 60 74161/32
	mapper_undefined_com,		// 61 IREM 74161/32
	mapper_undefined_com,		// 62 ?
	mapper_undefined_com,		// 63 ?
	mapper_undefined_com,		// 64 Nesticle Buggy MMC3
	mapper_undefined_com,		// 65 ?
	mapper_undefined_com,		// 66 ?
	mapper_undefined_com,		// 67 ?
	mapper_undefined_com,		// 68 ?
	mapper_undefined_com,		// 69 ?
	mapper_undefined_com,		// 6A ?
	mapper_undefined_com,		// 6B ?
	mapper_undefined_com,		// 6C ?
	mapper_undefined_com,		// 6D ?
	mapper_undefined_com,		// 6E ?
	mapper_undefined_com,		// 6F ?
	mapper_undefined_com,		// 70 PC-Asder
	mapper_undefined_com,		// 71 PC-Sachen/Hacker
	mapper_undefined_com,		// 72 PC-SuperGames
	mapper_undefined_com,		// 73 PC-CartSaint
	mapper_undefined_com,		// 74 PC-Reserved
	mapper_undefined_com,		// 75 PC-Future
	mapper_undefined_com,		// 76 ??????
	mapper_undefined_com,		// 77 TQROM
	mapper_undefined_com,		// 78 ?
	mapper_undefined_com,		// 79 ?
	mapper_undefined_com,		// 7A 74161/32
	mapper_undefined_com,		// 7B 
	mapper_undefined_com,		// 7C ?
	mapper_undefined_com,		// 7D ?
	mapper_undefined_com,		// 7E ?
	mapper_undefined_com,		// 7F ?
	mapper_undefined_com,		// 80 ?
	mapper_undefined_com,		// 81 ?
	mapper_undefined_com,		// 82 ?
	mapper_undefined_com,		// 83 ?
	mapper_undefined_com,		// 84 ?
	mapper_undefined_com,		// 85 ?
	mapper_undefined_com,		// 86 ?
	mapper_undefined_com,		// 87 ?
	mapper_undefined_com,		// 88 ?
	mapper_undefined_com,		// 89 ?
	mapper_undefined_com,		// 8A ?
	mapper_undefined_com,		// 8B ?
	mapper_undefined_com,		// 8C ?
	mapper_undefined_com,		// 8D ?
	mapper_undefined_com,		// 8E ?
	mapper_undefined_com,		// 8F ?
	mapper_undefined_com,		// 90 ?
	mapper_undefined_com,		// 91 ?
	mapper_undefined_com,		// 92 ?
	mapper_undefined_com,		// 93 ?
	mapper_undefined_com,		// 94 ?
	mapper_undefined_com,		// 95 ?
	mapper_undefined_com,		// 96 ?
	mapper_undefined_com,		// 97 ?
	mapper_undefined_com,		// 98 ?
	mapper_undefined_com,		// 99 ?
	mapper_undefined_com,		// 9A ?
	mapper_undefined_com,		// 9B ?
	mapper_undefined_com,		// 9C ?
	mapper_undefined_com,		// 9D ?
	mapper_undefined_com,		// 9E ?
	mapper_undefined_com,		// 9F ?
	mapper_undefined_com,		// A0 ?
	mapper_undefined_com,		// A1 ?
	mapper_undefined_com,		// A2 ?
	mapper_undefined_com,		// A3 ?
	mapper_undefined_com,		// A4 ?
	mapper_undefined_com,		// A5 ?
	mapper_undefined_com,		// A6 ?
	mapper_undefined_com,		// A7 ?
	mapper_undefined_com,		// A8 ?
	mapper_undefined_com,		// A9 ?
	mapper_undefined_com,		// AA ?
	mapper_undefined_com,		// AB ?
	mapper_undefined_com,		// AC ?
	mapper_undefined_com,		// AD ?
	mapper_undefined_com,		// AE ?
	mapper_undefined_com,		// AF ?
	mapper_undefined_com,		// B0 ?
	mapper_undefined_com,		// B1 ?
	mapper_undefined_com,		// B2 ?
	mapper_undefined_com,		// B3 ?
	mapper_undefined_com,		// B4 ?
	mapper_undefined_com,		// B5 ?
	mapper_undefined_com,		// B6 ?
	mapper_undefined_com,		// B7 ?
	mapper_madoola_com,			// B8 Sunsoft the wing of madoola
	mapper_undefined_com,		// B9 ?
	mapper_undefined_com,		// BA ?
	mapper_undefined_com,		// BB ?
	mapper_undefined_com,		// BC ?
	mapper_undefined_com,		// BD ?
	mapper_undefined_com,		// BE ?
	mapper_undefined_com,		// BF ?
	mapper_undefined_com,		// C0 ?
	mapper_undefined_com,		// C1 ?
	mapper_undefined_com,		// C2 ?
	mapper_undefined_com,		// C3 ?
	mapper_undefined_com,		// C4 ?
	mapper_undefined_com,		// C5 ?
	mapper_undefined_com,		// C6 ?
	mapper_undefined_com,		// C7 ?
	mapper_undefined_com,		// C8 ?
	mapper_undefined_com,		// C9 ?
	mapper_undefined_com,		// CA ?
	mapper_undefined_com,		// CB ?
	mapper_undefined_com,		// CC ?
	mapper_undefined_com,		// CD ?
	mapper_undefined_com,		// CE ?
	mapper_undefined_com,		// CF ?
	mapper_undefined_com,		// D0 ?
	mapper_undefined_com,		// D1 ?
	mapper_undefined_com,		// D2 ?
	mapper_undefined_com,		// D3 ?
	mapper_undefined_com,		// D4 ?
	mapper_undefined_com,		// D5 ?
	mapper_undefined_com,		// D6 ?
	mapper_undefined_com,		// D7 ?
	mapper_undefined_com,		// D8 ?
	mapper_undefined_com,		// D9 ?
	mapper_undefined_com,		// DA ?
	mapper_undefined_com,		// DB ?
	mapper_undefined_com,		// DC ?
	mapper_undefined_com,		// DD ?
	mapper_undefined_com,		// DE ?
	mapper_undefined_com,		// DF ?
	mapper_undefined_com,		// E0 ?
	mapper_undefined_com,		// E1 ?
	mapper_undefined_com,		// E2 ?
	mapper_undefined_com,		// E3 ?
	mapper_undefined_com,		// E4 ?
	mapper_undefined_com,		// E5 ?
	mapper_undefined_com,		// E6 ?
	mapper_undefined_com,		// E7 ?
	mapper_undefined_com,		// E8 ?
	mapper_undefined_com,		// E9 ?
	mapper_undefined_com,		// EA ?
	mapper_undefined_com,		// EB ?
	mapper_undefined_com,		// EC ?
	mapper_undefined_com,		// ED ?
	mapper_undefined_com,		// EE ?
	mapper_undefined_com,		// EF ?
	mapper_undefined_com,		// F0 ?
	mapper_undefined_com,		// F1 ?
	mapper_undefined_com,		// F2 ?
	mapper_undefined_com,		// F3 ?
	mapper_undefined_com,		// F4 ?
	mapper_undefined_com,		// F5 ?
	mapper_undefined_com,		// F6 ?
	mapper_undefined_com,		// F7 ?
	mapper_undefined_com,		// F8 ?
	mapper_undefined_com,		// F9 ?
	mapper_undefined_com,		// FA ?
	mapper_undefined_com,		// FB ?
	mapper_undefined_com,		// FC ?
	mapper_undefined_com,		// FD ?
	mapper_undefined_com,		// FE ?
	mapper_nsf_com,				// FF NSF Mapper ( Temporary )
	memcom7fff					// 100 Normal Comment
};





char *comment_fffa[3] = {
	"NMI(VBlank)",
	"Reset",
	"IRQ/BRK"
};




int disasm6502(uchar *buf,ulong base,ulong addr,char *stream)
{
	int i,j,ret;
	// int k;
	int st;

	// アドレスの出力
	st = sprintf(stream,"%04X :",base+addr);
	j = dis6502[buf[addr]].type & ADT_MASK;
	// k = dis6502[buf[addr]].type & ACT_MASK;
	// アドレス別処理
	if ( (base+addr+opbytelength[j]) < 0xfffb ) {
		// 通常処理
		// バイナリを出力
		for ( i = 0 ; i < opbytelength[j] ; i++ ) {
			st += sprintf(&stream[st]," %02X",buf[addr+i]);
		}
		// ニーモニックの出力
		st += sprintf(&stream[st],"\t\t%s\t",dis6502[buf[addr]].nimonic);
		// バイナリ長別の処理
		switch ( opbytelength[j] ) {
			case 1:
				if ( dis6502[buf[addr]].nimonic == opname[NMT_UND] ) {
					// 未定義命令
					st += sprintf(&stream[st],adtstring[ADT_IMM],buf[addr]);
				} else {
					// 1byte命令
					st += sprintf(&stream[st],adtstring[j]);
				}
			break;
			case 2:
				if ( (j&ADT_MASK) == ADT_REL ) {
					// 相対アドレス
					if ( buf[addr+1] < 0x80 ) {
						st += sprintf(&stream[st],adtstring[j],base+addr+2+buf[addr+1]);
					} else {
						st += sprintf(&stream[st],adtstring[j],base+addr+2-(0x100-buf[addr+1]));
					}
				} else {
					// 2byte命令
					st += sprintf(&stream[st],adtstring[j],buf[addr+1]);
				}
			break;
			case 3:
				// 3byte命令
				st += sprintf(&stream[st],adtstring[j],buf[addr+1]+buf[addr+2]*256);
				break;
		}
		// コメント処理
		// 下の関数に移した
		st += sprintf(&stream[st],";\n");
		ret = opbytelength[j];
	} else {
		// 割り込みベクトル処理
		if ( (base+addr)<0xfffa ) {
			// 未定義命令
			st += sprintf(&stream[st]," %02X\t\tdb\t#$%02X\t\t;\n",
				buf[addr],buf[addr]);
			ret = 1;
		} else {
			// 割り込みベクトル
			st += sprintf(&stream[st]," %02X %02X\t\tdw\t$%04X\t\t;%s\n",
				buf[addr],buf[addr+1],buf[addr]+buf[addr+1]*256,comment_fffa[(base+addr-0xfffa)/2]);
			ret = 2;
		}
	}
	return ret;
}


void comchk6502(uchar *buf,ulong base,ulong addr,char *stream,int mapper[])
{
	// コメント付加
	int i,j,k,l;
	int st;
	
	j = dis6502[buf[addr]].type & ADT_MASK;
	l = dis6502[buf[addr]].type & ACT_MASK;
	if ( opbytelength[j] != 3 || ((base+addr)>0xfff8) ) return;
	// コメントまで飛ばす
	for ( st = 0 ; stream[st] != ';' ; st++ );
	st++;
	if (( j == ADT_ABS || j == ADT_ABSX || j == ADT_ABSY ) && ( l != ACT_NL ) ) {
		k = buf[addr+1]+buf[addr+2]*256;
		// マッパー別のコメント(通常コメントもマッパーの一部とした Ver0.03)
		// Read と Write の区別をつけるべし
		mapper--;
		do {
			mapper++;
			for ( i = 0 ; mapcom[*mapper][i].comment != NULL ; i++ ) {
				if (( k >= mapcom[*mapper][i].staddr && k <= mapcom[*mapper][i].edaddr ) &&
					( l & mapcom[*mapper][i].access )
				   ) break;
			}
			if ( mapcom[*mapper][i].comment != NULL ) break;
		} while ( *mapper != MAX_MAPPERNUMS );
		if ( mapcom[*mapper][i].comment != NULL ) {
			st += sprintf(&stream[st],mapcom[*mapper][i].comment);
		}
	}
	st += sprintf(&stream[st],"\n");
}


int tablechk6502(uchar *buf,ulong base,ulong addr,char *stream)
{
	// 特殊テーブルのチェック
	int ret;
	int st,i,j;
	
	ret = 0;
	// まずは命令のチェック
	if ( dis6502[buf[addr]].nimonic == opname[NMT_RTS] ||
		 dis6502[buf[addr]].nimonic == opname[NMT_JMP]
		) {
		addr += opbytelength[dis6502[buf[addr]].type];
		// 音程テーブルのチェック これだと、DQ はだめなんだよなぁ...
		j = -1;
		if ( buf[addr+0]+buf[addr+1]*256 == 0x06AE &&
			  buf[addr+2]+buf[addr+3]*256 == 0x064E ) j = 1;
		if ( buf[addr+1]+buf[addr+0]*256 == 0x06AE &&
			  buf[addr+3]+buf[addr+2]*256 == 0x064E ) j = 0;
		if ( j >= 0 ) {
			for ( st = 0 ; stream[st] != 0x0a ; st++);
			st++;
			st += sprintf(&stream[st],";;多分音程テーブル\n");
			for ( i = 0 ; i < 24 ; i+=2 ) {
				st += sprintf(&stream[st],"%04X : %02X %02X\t\tdw\t$%04X\t\t;\n",
					base+addr+i,buf[addr+i],buf[addr+i+1],
					buf[addr+i+(1-j)]+buf[addr+i+j]*256);
			}
			ret = i;
		}
	}
	return ret;
}




void printhelp(void)
{
	printf("Minachun Disassembler for Famicom(NES).  Version 0.04\n"
	       "Usage:md6502.exe [input file] [output file] [options]\n"
#ifdef MESSAGETYPE_JAPANESE
	       "入力バイナリを 6502 バイナリと見なして逆アセンブルします。\n"
	       "[input file] : 入力ファイル名\n"
	       "[output file] : 出力ファイル名 指定しない場合は標準出力へ\n"
	       "[options] : -s????...逆アセンブル開始アドレスを指定する。デフォルトは、\n"
	       "                     ファイルの最後が 0xffff となるようにする。\n"
	       "            -l????...逆アセンブルするバイト長を指定する。デフォルトは、\n"
	       "                     ファイルすべてを読み込むまで出力する。\n"
	       "            -o????...逆アセンブル開始オフセットを指定する。デフォルトは、\n"
	       "                     ファイルの先頭、つまり 0 。\n"
	       "            -t...ファミコン音源周波数テーブルを探し、それらしい物があれば\n"
	       "                 テーブルにします。デフォルトではしません。\n"
	       "            -m??...マッパー番号を指定します。デフォルトでは、マッパー別の\n"
	       "                   コメントを出力しません。最大8つまで複数の指定が可能です。\n"
	       "                   Ex) -m4b -mff ... KONAMI VRC1  NSF Mapper\n"
#else
	       "DIsassemble regarding the input file as 6502 binary file.\n"
	       "[input file] : input filename want to disassemble\n"
	       "[output file] : output filename.if don't set , output stream is stdout.\n"
	       "[options] : -s????...Start Address of disassemble. Default is the value\n"
	       "                     which the last addr. is 0xffff.\n"
	       "            -l????...Byte length of disassemble. Default reads until the\n"
	       "                     whole input file is disassembled.\n"
	       "            -o????...Start offset Addr. of disassemble. Default is Zero ; \n"
	       "                     the head of the input file.\n"
	       "            -t...If Audio Source freqency table found , make the word tables.\n"
	       "                 Default don't it.\n"
	       "            -m??...Mapper number.If set, output uniq comment of each mapper.\n"
	       "                   Default is off. Multiple # is O.K. until 8 times.\n"
	       "                   Ex) -m4b -mff ... KONAMI VRC1 and NSF Mapper\n"
#endif
	);
	return;
}


int main(int argc,char *argv[])
{
	FILE *f,*g;
	uchar *buf;
	ulong siz;
	ulong base,addr;
	int i,j;
	int tablef,mapperf[MAX_MAPPERS+1],mapperidx;
	char outst[1024];
	
	if ( argv[1] == NULL ) {
		printhelp();
		return -1;
	}
	f = fopen(argv[1],"rb");
	if ( f ) {
		// ファイルサイズを求める
		fseek(f,0,SEEK_END);
		siz = ftell(f);
		// オプション判定
		i = 2;
		// デフォルト出力先は標準出力
		g = stdout;
		if ( argv[i] != NULL && argv[i][0] != '-' ) {
			// 出力ファイルを作る
			g = fopen(argv[i],"w");
			i++;
		}
		base = 0xffffffff;
		j = 0;				// オフセット
		tablef = 0;
		mapperf[0] = MAX_MAPPERNUMS;
		mapperidx = 0;
		while ( argv[i] != NULL ) {
			if ( argv[i][0] == '-' ) {
				switch ( argv[i][1] ) {
					case 's':
						sscanf(&argv[i][2],"%x",&base);
						break;
					case 'l':
						sscanf(&argv[i][2],"%x",&siz);
						break;
					case 'o':
						sscanf(&argv[i][2],"%x",&j);
						break;
					case 't':
						tablef = 1;
						break;
					case 'm':
						if ( mapperidx <= MAX_MAPPERS ) {
							sscanf(&argv[i][2],"%x",&mapperf[mapperidx]);
							if ( mapperf[mapperidx] >= MAX_MAPPERNUMS ) mapperf[mapperidx] = 0;
							mapperf[++mapperidx] = MAX_MAPPERNUMS;
						} else {
#ifdef MESSAGETYPE_JAPANESE
							fprintf(stderr,"指定したマッパーの数が多すぎます。無視します。\n");
#else
							fprintf(stderr,"Too many mapper#.Max # is 8.This # is ignored.\n");
#endif
						}
						break;
					default:
#ifdef MESSAGETYPE_JAPANESE
						fprintf(stderr,"未定義なオプション:%s\n",argv[i]);
#else
						fprintf(stderr,"Undefined Option:%s\n",argv[i]);
#endif
				}
			}
			i++;
		}
		// ベースアドレスの調整
		if ( base == 0xffffffff ) base = 0x10000 - siz;
		// メモリを確保して読み込む
		buf = new uchar[siz+2];
		fseek(f,j,SEEK_SET);
		siz = fread(buf,1,siz,f);
		fclose(f);
		// 念の為の初期化
		buf[siz] = 0xff;
		buf[siz+1] = 0xff;
#ifdef MESSAGETYPE_JAPANESE
		printf("逆アセンブル範囲:%04X - %04X\n",base,base+siz-1);
#else
		printf("Disassemble renge:%04X - %04X\n",base,base+siz-1);
#endif
		if ( g ) {
			for ( addr = 0 ; addr < siz ; ) {
				// 逆アセンブル
				j = disasm6502(buf,base,addr,outst);
				// コメントつけ加え
				comchk6502(buf,base,addr,outst,mapperf);
				// 特殊テーブル処理
				if ( tablef ) j += tablechk6502(buf,base,addr,outst);
				fprintf(g,outst);
				addr += j;
			}
			if ( g != stdout ) fclose(g);
		} else {
#ifdef MESSAGETYPE_JAPANESE
			fprintf(stderr,"出力ファイル %s が作れませんでした。\n",argv[2]);
#else
			fprintf(stderr,"Can't write file : %s .\n",argv[2]);
#endif
		}
		delete buf;
	} else {
#ifdef MESSAGETYPE_JAPANESE
		fprintf(stderr,"入力ファイル %s が開けませんでした。\n",argv[1]);
#else
		fprintf(stderr,"Can't open intput file : %s .\n",argv[1]);
#endif
	}
	return 0;
}
