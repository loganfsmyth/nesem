#include "cpu.hpp"

using namespace std;

CPU::CPU(boost::shared_ptr<Bus> b){
  bus = b;
  XR = 0;
  YR = 0;
  AC = 0;
  SR = 0;
  SP = 0xFF;
  PC = 0;

  interrupt_status = false;
  interrupt_status_last = false;

  wait_time = 0;
}
CPU::~CPU(){

}

void CPU::reset(){
  PC = bus->read(0xFFFC) | (bus->read(0xFFFD) << 8);
}

void CPU::trigger_interrupt(){

  //~ print_stack();

  PUSH(PC >> 8);
  PUSH(PC & 0xFF);
  PUSH(SR);

  PC = bus->read(0xFFFA) | (bus->read(0xFFFB) << 8);

  //~ print_stack();
}
void CPU::raise_interrupt(){
  interrupt_status = true;
}
void CPU::drop_interrupt(){
  interrupt_status = false;
}

void CPU::print_stack(){
  cout << "SP: " << hex <<  (int)SP << endl;
  cout << "----------------" << endl;
  cout << hex;

  int i;
  for(i = 0xFF; i > SP; i--){
    cout << (i + 0x100) << ": " << (int)bus->read(i + 0x100) << endl;
  }

  cout << "----------------" << endl;
  cout << dec;
}

int CPU::executeInst(){

  if(interrupt_status == true && interrupt_status_last == false){
    trigger_interrupt();
  }
  interrupt_status_last = interrupt_status;

  ostringstream address;
  string address_str;
  int time = 0;

  uint8_t inst = bus->read(PC);
   
  
  //~ cout << hex << "0x" << PC << ": " << "0x" << (int)inst << " " << get_inst_str(inst) << endl;
//  cout << get_inst_str(inst) << endl;
//  cout << hex << "0x" << (unsigned int)AC << dec  << endl;
//  cout << (int)bus->read(0x1ff) << endl;

//~ if(PC >= 0xB10A && PC <= 0xB110){
//~ cout << hex << "0x" << PC << ": " << "0x" << (int)inst << " " << get_inst_str(inst) << endl;
//~ print_stack();
//~ }

  struct instruction_info inst_info = get_instruction_info(inst);
  
//  cout << "Len: " << inst_info.length << endl;
//  cout << "Type: " << inst_info.type << endl;

  uint8_t arg1 = 0, arg2 = 0;
  if(inst_info.length > 1) arg1 = bus->read(PC + 1);
  if(inst_info.length > 2) arg2 = bus->read(PC + 2);

  PC += inst_info.length;
  time += inst_info.time;

//  uint16_t addr = get_address(inst_info.type, arg1, arg2);


  uint8_t src = read(inst_info.type, arg1, arg2);
  uint16_t temp;

  switch(inst){
    case ADC_IMM:
    case ADC_ZP:
    case ADC_ZPX:
    case ADC_AB:
    case ADC_ABX:
    case ADC_ABY:
    case ADC_INX:
    case ADC_INY:
      temp = src + AC + (IF_CARRY() ? 1 : 0);
      SET_ZERO(temp & 0xff);	/* This is not valid in decimal mode */
      if (IF_DECIMAL()) {
	if (((AC & 0xf) + (src & 0xf) + (IF_CARRY() ? 1 : 0)) > 9) temp += 6;
	SET_SIGN(temp);
	SET_OVERFLOW(!((AC ^ src) & 0x80) && ((AC ^ temp) & 0x80));
	if (temp > 0x99) temp += 96;
	SET_CARRY(temp > 0x99);
      } else {
	SET_SIGN(temp);
	SET_OVERFLOW(!((AC ^ src) & 0x80) && ((AC ^ temp) & 0x80));
	SET_CARRY(temp > 0xff);
      }

      AC = temp;
      break;

    case SBC_IMM:
    case SBC_ZP:
    case SBC_ZPX:
    case SBC_AB:
    case SBC_ABX:
    case SBC_ABY:
    case SBC_INX:
    case SBC_INY:
      temp = AC - src - (IF_CARRY() ? 0 : 1);
      SET_SIGN(temp);
      SET_ZERO(temp & 0xff);	/* Sign and Zero are invalid in decimal mode */
      SET_OVERFLOW(((AC ^ temp) & 0x80) && ((AC ^ src) & 0x80));
      if (IF_DECIMAL()) {
	if ( ((AC & 0xf) - (IF_CARRY() ? 0 : 1)) < (src & 0xf)) /* EP */ temp -= 6;
	if (temp > 0x99) temp -= 0x60;
      }
      SET_CARRY(temp < 0x100);
      AC = (temp & 0xff);
      break;

    case AND_IMM:
    case AND_ZP:
    case AND_ZPX:
    case AND_AB:
    case AND_ABX:
    case AND_ABY:
    case AND_INX:
    case AND_INY:
      src &= AC;
      SET_SIGN(src);
      SET_ZERO(src);
      AC = src;
      break;

    case CMP_IMM:
    case CMP_ZP:
    case CMP_ZPX:
    case CMP_AB:
    case CMP_ABX:
    case CMP_ABY:
    case CMP_INX:
    case CMP_INY:
      temp = AC - src;
      SET_CARRY(temp < 0x100);
      SET_SIGN(temp);
      SET_ZERO(temp & 0xff);
      break;

    case EOR_IMM:
    case EOR_ZP:
    case EOR_ZPX:
    case EOR_AB:
    case EOR_ABX:
    case EOR_ABY:
    case EOR_INX:
    case EOR_INY:
      src ^= AC;
      SET_SIGN(src);
      SET_ZERO(src);
      AC = src;
      break;

    case ORA_IMM:
    case ORA_ZP:
    case ORA_ZPX:
    case ORA_AB:
    case ORA_ABX:
    case ORA_ABY:
    case ORA_INX:
    case ORA_INY:
      src |= AC;
      SET_SIGN(src);
      SET_ZERO(src);
      AC = src;
      break;

    case LDA_IMM:
    case LDA_ZP:
    case LDA_ZPX:
    case LDA_AB:
    case LDA_ABX:
    case LDA_ABY:
    case LDA_INX:
    case LDA_INY:
      SET_SIGN(src);
      SET_ZERO(src);
      AC = src;
      break;

    case STA_ZP:
    case STA_ZPX:
    case STA_AB:
    case STA_ABX:
    case STA_ABY:
    case STA_INX:
    case STA_INY:
      write(inst_info.type, arg1, arg2, AC);
      if(((arg2 << 8) | arg1) == 0x4014) time += 512;
    //	update_value = true;
      break;

    case TXS_IMP:
      SP = XR;
      break;

    case TSX_IMP:
      SET_SIGN(SP);
      SET_ZERO(SP);
      XR = SP;
      break;

    case PHA_IMP:
      src = AC;
      PUSH(src);
      break;

    case PLA_IMP:
      src = PULL();
      SET_SIGN(src);	/* Change sign and zero flag accordingly. */
      SET_ZERO(src);
      AC = src;
      break;

    case PHP_IMP:
      src = SR;
      PUSH(src);
      break;

    case PLP_IMP:
      SR = PULL();
      break;

    case BPL_REL:
      if (!IF_SIGN()) {
	time += ((PC & 0xFF00) != ((PC + (int8_t)src) & 0xFF00) ? 2 : 1);
	PC = PC + (int8_t)src;
      }
      break;

    case BMI_REL:
      if (IF_SIGN()) {
	time += ((PC & 0xFF00) != ((PC + (int8_t)src) & 0xFF00) ? 2 : 1);
	PC = PC + (int8_t)src;
      }
      break;

    case BVC_REL:
      if (!IF_OVERFLOW()) {
	time += ((PC & 0xFF00) != ((PC + (int8_t)src) & 0xFF00) ? 2 : 1);
	PC = PC + (int8_t)src;
      }
      break;

    case BVS_REL:
      if (IF_OVERFLOW()) {
	time += ((PC & 0xFF00) != ((PC + (int8_t)src) & 0xFF00) ? 2 : 1);
	PC = PC + (int8_t)src;
      }
      break;

    case BCC_REL:
      if (!IF_CARRY()) {
	time += ((PC & 0xFF00) != ((PC + (int8_t)src) & 0xFF00) ? 2 : 1);
	PC = PC + (int8_t)src;
      }
      break;

    case BCS_REL:
      if (IF_CARRY()) {
	time += ((PC & 0xFF00) != ((PC + (int8_t)src) & 0xFF00) ? 2 : 1);
	PC = PC + (int8_t)src;
      }
      break;

    case BNE_REL:
      if (!IF_ZERO()) {
	time += ((PC & 0xFF00) != ((PC + (int8_t)src) & 0xFF00) ? 2 : 1);
	PC = PC + (int8_t)src;
      }
      break;
    case BEQ_REL:
      if (IF_ZERO()) {
	time += ((PC & 0xFF00) != ((PC + (int8_t)src) & 0xFF00) ? 2 : 1);
	PC = PC + (int8_t)src;
      }
      break;

    case BRK_IMP:
      PC++;
      PUSH((PC >> 8) & 0xff);	/* Push return address onto the stack. */
      PUSH(PC & 0xff);
      SET_BREAK((1));             /* Set BFlag before pushing */
      PUSH(SR);
      SET_INTERRUPT((1));
      PC = (bus->read(0xFFFE) | (bus->read(0xFFFF) << 8));


    case CLC_IMP:	SET_CARRY(0); break;
    case SEC_IMP:	SET_CARRY(1); break;
    case CLI_IMP:	SET_INTERRUPT(0); break;
    case SEI_IMP:	SET_INTERRUPT(1); break;
    case CLV_IMP:	SET_OVERFLOW(0); break;
    case CLD_IMP:	SET_DECIMAL(0); break;
    case SED_IMP:	SET_DECIMAL(1); break;

    case NOP_IMP:
      break;

    case TAX_IMP:
      SET_SIGN(AC);
      SET_ZERO(AC);
      XR = AC;
      break;

    case TXA_IMP:
      SET_SIGN(XR);
      SET_ZERO(XR);
      AC = XR;
      break;

    case DEX_IMP:
      XR = (XR - 1) & 0xff;
      SET_SIGN(XR);
      SET_ZERO(XR);
      break;

    case INX_IMP:
      XR = (XR + 1) & 0xff;
      SET_SIGN(XR);
      SET_ZERO(XR);
      break;

    case TAY_IMP:
      SET_SIGN(AC);
      SET_ZERO(AC);
      YR = AC;
      break;

    case TYA_IMP:
      SET_SIGN(YR);
      SET_ZERO(YR);
      AC = YR;
      break;

    case DEY_IMP:
      YR = (YR - 1) & 0xff;
      SET_SIGN(YR);
      SET_ZERO(YR);
      break;

    case INY_IMP:
      YR = (YR + 1) & 0xff;
      SET_SIGN(YR);
      SET_ZERO(YR);
      break;

    case RTI_IMP:

      src = PULL();
      SR = src;
      temp = PULL();
      temp |=  (PULL() << 8);	/* Load return address from stack. */
      PC = temp;

      break;

    case RTS_IMP:

    //print_stack();

      temp = PULL();
      temp |= ((PULL()) << 8);	/* Load return address from stack and add 1. */
      PC = temp + 1;

    //print_stack();
    //trace.pop_back();

      break;

    case LSR_AC:
    case LSR_ZP:
    case LSR_ZPX:
    case LSR_AB:
    case LSR_ABX:
      SET_CARRY(src & 0x01);
      src >>= 1;
      SET_SIGN(src);
      SET_ZERO(src);
      write(inst_info.type, arg1, arg2, src);
    //update_value = true;
      break;

    case ROL_AC:
    case ROL_ZP:
    case ROL_ZPX:
    case ROL_AB:
    case ROL_ABX:
      temp = src;
      temp <<= 1;
      if (IF_CARRY()) temp |= 0x1;
      SET_CARRY(temp > 0xff);
      temp &= 0xff;
      SET_SIGN(temp);
      SET_ZERO(temp);
      src = temp;
      write(inst_info.type, arg1, arg2, src);
    //update_value = true;
      break;

    case ROR_AC:
    case ROR_ZP:
    case ROR_ZPX:
    case ROR_AB:
    case ROR_ABX:
      temp = src;
      if (IF_CARRY()) temp |= 0x100;
      SET_CARRY(temp & 0x01);
      temp >>= 1;
      SET_SIGN(temp);
      SET_ZERO(temp);
      src = temp;
      write(inst_info.type, arg1, arg2, src);
    //update_value = true;
      break;

    case ASL_AC:
    case ASL_ZP:
    case ASL_ZPX:
    case ASL_AB:
    case ASL_ABX:
      SET_CARRY(src & 0x80);
      src <<= 1;
      SET_SIGN(src);
      SET_ZERO(src);
      write(inst_info.type, arg1, arg2, src);
    //update_value = true;
      break;

    case LDX_IMM:
    case LDX_ZP:
    case LDX_ZPY:
    case LDX_AB:
    case LDX_ABX:
      SET_SIGN(src);
      SET_ZERO(src);
      XR = src;
      break;

    case LDY_IMM:
    case LDY_ZP:
    case LDY_ZPX:
    case LDY_AB:
    case LDY_ABX:
      SET_SIGN(src);
      SET_ZERO(src);
      YR = src;
      break;

    case DEC_ZP:
    case DEC_ZPX:
    case DEC_AB:
    case DEC_ABX:
      src = (src - 1) & 0xff;
      SET_SIGN(src);
      SET_ZERO(src);
      write(inst_info.type, arg1, arg2, src);
    //update_value = true;
      break;

    case INC_ZP:
    case INC_ZPX:
    case INC_AB:
    case INC_ABX:
      src = (src + 1) & 0xff;
      SET_SIGN(src);
      SET_ZERO(src);
      write(inst_info.type, arg1, arg2, src);
    //update_value = true;
      break;

    case STX_ZP:
    case STX_ZPY:
    case STX_AB:
      write(inst_info.type, arg1, arg2, XR);
      if(((arg2 << 8) | arg1) == 0x4014) time += 512;
    //update_value = true;
      break;

    case STY_ZP:
    case STY_ZPX:
    case STY_AB:
      write(inst_info.type, arg1, arg2, YR);
      if(((arg2 << 8) | arg1) == 0x4014) time += 512;
    //update_value = true;
      break;

    case CPX_IMM:
    case CPX_ZP:
    case CPX_AB:
      temp = XR - src;
      SET_CARRY(temp < 0x100);
      SET_SIGN(temp);
      SET_ZERO(temp & 0xff);
      break;

    case CPY_IMM:
    case CPY_ZP:
    case CPY_AB:
      temp = YR - src;
      SET_CARRY(temp < 0x100);
      SET_SIGN(temp);
      SET_ZERO(temp & 0xff);
      break;

    case BIT_ZP:
    case BIT_AB:
      SET_SIGN(src);
      SET_OVERFLOW(0x40 & src);	/* Copy bit 6 to OVERFLOW flag. */
      SET_ZERO(src & AC);
      break;

    case JMP_AB:
      PC = (arg2 << 8) | arg1;
      break;
    case JMP_IN:
      PC = bus->read((arg2 << 8) | arg1) | (bus->read(((arg2 << 8) | arg1) + 1) << 8);
      break;

    case JSR_AB:
      temp = PC;
      //~ print_stack();

      PC--;
      PUSH((PC >> 8) & 0xff);	/* Push return address onto the stack. */
      PUSH(PC & 0xff);
      PC = (arg2 << 8) | arg1;

      //~ cout << "OMG" << endl;
      //~ address << "From " << hex << (int)temp << " to " << (int)PC;
      //~ cout << "From " << hex << (int)temp << " to " << (int)PC << endl;
      //~ cout << "OMG" << endl;
      //~ address_str = address.str();
      //~ address_str = "OMG";
      //~ cout << "OMG" << endl;
      //~ cout << trace.size() << "!!" << endl;
      //~ cout << "OMG" << endl;
      //~ trace.push_back(address_str);
      //~ cout << "OMG" << endl;

      //~ print_stack();

      break;

    default:
      cout << "Invalid Instruction: " << hex << "PC: 0x" << (int)PC << ": " << " Inst 0x" << (int)inst << " " << endl;
      print_trace();
      throw;
      break;
  }

  return time;
}

void CPU::print_trace(){

  vector<string>::iterator it;

  //~ cout << "myvector contains:";
  //~ for ( it=trace.begin() ; it < trace.end(); it++ ){
	  //~ cout << " " << *it << endl;
  //~ }

  cout << endl;



}


void CPU::write(int type, uint8_t arg1, uint8_t arg2, uint8_t data){

  uint16_t temp;
  uint8_t lower, upper;

  switch(type){
    case MEM_IMM:
      break;

    case MEM_ZP:
      bus->write(arg1, data);
      break;
    case MEM_ZPX:
      bus->write((arg1 + (uint8_t)XR) & 0x00FF, data);
      break;
    case MEM_ZPY:
      bus->write((arg1 + (uint8_t)YR) & 0x00FF, data);
      break;
    case MEM_AB:
      bus->write((arg2 << 8) | arg1, data);
      break;
    case MEM_ABX:
      bus->write(((arg2 << 8) | arg1) + (uint8_t)XR, data);
      break;
    case MEM_ABY:
      bus->write(((arg2 << 8) | arg1) + (uint8_t)YR, data);
      break;
    case MEM_INX:
      lower = bus->read(arg1 + (uint8_t)XR);
      upper = bus->read(arg1 + (uint8_t)XR + 1);
      bus->write((upper << 8) | lower, data);
      break;
    case MEM_INY:
      lower = bus->read(arg1);
      upper = bus->read(arg1 + 1);
      bus->write(((upper << 8) | lower) + (uint8_t)YR, data);
      break;
    case MEM_IMP:
      break;
    case MEM_AC:
      AC = data;
      break;

    case MEM_IN:
      temp = (arg2 << 8) & arg1;
      lower = bus->read(temp);
      upper = bus->read(temp + 1);
      bus->write((upper << 8) | lower, data);
      break;
    case MEM_REL:
      break;

    default:
      cout << "CPU: WTF Line:" << __LINE__ << endl;
      break;
  }
}
uint8_t CPU::read(int type, uint8_t arg1, uint8_t arg2){

  uint16_t temp;
  uint8_t lower, upper;

  switch(type){
    case MEM_IMM:
      return arg1;

    case MEM_ZP:
      return bus->read(arg1);

    case MEM_ZPX:
      return bus->read((arg1 + (uint8_t)XR) & 0x00FF);

    case MEM_ZPY:
      return bus->read((arg1 + (uint8_t)YR) & 0x00FF);

    case MEM_AB:
      return bus->read((arg2 << 8) | arg1);

    case MEM_ABX:
      return bus->read(((arg2 << 8) | arg1) + (uint8_t)XR);

    case MEM_ABY:
      return bus->read(((arg2 << 8) | arg1) + (uint8_t)YR);

    case MEM_INX:
      lower = bus->read(arg1 + (uint8_t)XR);
      upper = bus->read(arg1 + (uint8_t)XR + 1);
      return bus->read((upper << 8) | lower);

    case MEM_INY:
      lower = bus->read(arg1);
      upper = bus->read(arg1 + 1);

    //cout << "INY 0x" << hex << (unsigned int)(((upper << 8) | lower) + (uint8_t)YR) << dec << endl;

      return bus->read(((upper << 8) | lower) + (uint8_t)YR);

    case MEM_IMP:
      return 0;

    case MEM_AC:
      return AC;

    case MEM_IN:
      temp = (arg2 << 8) & arg1;
      lower = bus->read(temp);
      upper = bus->read(temp + 1);
      return bus->read((upper << 8) | lower);

    case MEM_REL:
      return arg1;

    default:
      cout << "CPU: WTF Line:" << __LINE__ << endl;
      return 0;
      break;
  }

}


struct instruction_info CPU::get_instruction_info(uint8_t inst){
  switch(inst){
    case ADC_IMM: return  (struct instruction_info){2, MEM_IMM,2,'2'};
    case ADC_ZP: return  (struct instruction_info){2, MEM_ZP,3,'3'};
    case ADC_ZPX: return  (struct instruction_info){2, MEM_ZPX,4,'4'};
    case ADC_AB: return  (struct instruction_info){3, MEM_AB,4,'4'};
    case ADC_ABX: return  (struct instruction_info){3, MEM_ABX,4,'+'};
    case ADC_ABY: return  (struct instruction_info){3, MEM_ABY,4,'+'};
    case ADC_INX: return  (struct instruction_info){2, MEM_INX,6,'6'};
    case ADC_INY: return  (struct instruction_info){2, MEM_INY,5,'+'};

    case SBC_IMM: return  (struct instruction_info){2, MEM_IMM,2,'2'};
    case SBC_ZP: return  (struct instruction_info){2, MEM_ZP,3,'3'};
    case SBC_ZPX: return  (struct instruction_info){2, MEM_ZPX,4,'4'};
    case SBC_AB: return  (struct instruction_info){3, MEM_AB,4,'4'};
    case SBC_ABX: return  (struct instruction_info){3, MEM_ABX,4,'+'};
    case SBC_ABY: return  (struct instruction_info){3, MEM_ABY,4,'+'};
    case SBC_INX: return  (struct instruction_info){2, MEM_INX,6,'6'};
    case SBC_INY: return  (struct instruction_info){2, MEM_INY,5,'+'};

    case AND_IMM: return  (struct instruction_info){2, MEM_IMM,2,'2'};
    case AND_ZP: return  (struct instruction_info){2, MEM_ZP,2,'2'};
    case AND_ZPX: return  (struct instruction_info){2, MEM_ZPX,3,'3'};
    case AND_AB: return  (struct instruction_info){3, MEM_AB,4,'4'};
    case AND_ABX: return  (struct instruction_info){3, MEM_ABX,4,'+'};
    case AND_ABY: return  (struct instruction_info){3, MEM_ABY,4,'+'};
    case AND_INX: return  (struct instruction_info){2, MEM_INX,6,'6'};
    case AND_INY: return  (struct instruction_info){2, MEM_INY,5,'+'};

    case CMP_IMM: return  (struct instruction_info){2, MEM_IMM,2,'2'};
    case CMP_ZP: return  (struct instruction_info){2, MEM_ZP,3,'3'};
    case CMP_ZPX: return  (struct instruction_info){2, MEM_ZPX,4,'4'};
    case CMP_AB: return  (struct instruction_info){3, MEM_AB,4,'4'};
    case CMP_ABX: return  (struct instruction_info){3, MEM_ABX,4,'+'};
    case CMP_ABY: return  (struct instruction_info){3, MEM_ABY,4,'+'};
    case CMP_INX: return  (struct instruction_info){2, MEM_INX,6,'6'};
    case CMP_INY: return  (struct instruction_info){2, MEM_INY,5,'+'};

    case EOR_IMM: return  (struct instruction_info){2, MEM_IMM,2,'2'};
    case EOR_ZP: return  (struct instruction_info){2, MEM_ZP,3,'3'};
    case EOR_ZPX: return  (struct instruction_info){2, MEM_ZPX,4,'4'};
    case EOR_AB: return  (struct instruction_info){3, MEM_AB,4,'4'};
    case EOR_ABX: return  (struct instruction_info){3, MEM_ABX,4,'+'};
    case EOR_ABY: return  (struct instruction_info){3, MEM_ABY,4,'+'};
    case EOR_INX: return  (struct instruction_info){2, MEM_INX,6,'6'};
    case EOR_INY: return  (struct instruction_info){2, MEM_INY,5,'+'};

    case ORA_IMM: return  (struct instruction_info){2, MEM_IMM,2,'2'};
    case ORA_ZP: return  (struct instruction_info){2, MEM_ZP,2,'2'};
    case ORA_ZPX: return  (struct instruction_info){2, MEM_ZPX,3,'3'};
    case ORA_AB: return  (struct instruction_info){3, MEM_AB,4,'4'};
    case ORA_ABX: return  (struct instruction_info){3, MEM_ABX,4,'+'};
    case ORA_ABY: return  (struct instruction_info){3, MEM_ABY,4,'+'};
    case ORA_INX: return  (struct instruction_info){2, MEM_INX,6,'6'};
    case ORA_INY: return  (struct instruction_info){2, MEM_INY,5,'+'};

    case LDA_IMM: return  (struct instruction_info){2, MEM_IMM,2,'2'};
    case LDA_ZP: return  (struct instruction_info){2, MEM_ZP,3,'3'};
    case LDA_ZPX: return  (struct instruction_info){2, MEM_ZPX,4,'4'};
    case LDA_AB: return  (struct instruction_info){3, MEM_AB,4,'4'};
    case LDA_ABX: return  (struct instruction_info){3, MEM_ABX,4,'+'};
    case LDA_ABY: return  (struct instruction_info){3, MEM_ABY,4,'+'};
    case LDA_INX: return  (struct instruction_info){2, MEM_INX,6,'6'};
    case LDA_INY: return  (struct instruction_info){2, MEM_INY,5,'+'};

    case STA_ZP: return  (struct instruction_info){2, MEM_ZP,3,'3'};
    case STA_ZPX: return  (struct instruction_info){2, MEM_ZPX,4,'4'};
    case STA_AB: return  (struct instruction_info){3, MEM_AB,4,'4'};
    case STA_ABX: return  (struct instruction_info){3, MEM_ABX,5,'5'};
    case STA_ABY: return  (struct instruction_info){3, MEM_ABY,5,'5'};
    case STA_INX: return  (struct instruction_info){2, MEM_INX,6,'6'};
    case STA_INY: return  (struct instruction_info){2, MEM_INY,6,'6'};

    case TXS_IMP: return  (struct instruction_info){1, MEM_IMP,2,'2'};
    case TSX_IMP: return  (struct instruction_info){1, MEM_IMP,2,'2'};
    case PHA_IMP: return  (struct instruction_info){1, MEM_IMP,3,'3'};
    case PLA_IMP: return  (struct instruction_info){1, MEM_IMP,4,'4'};
    case PHP_IMP: return  (struct instruction_info){1, MEM_IMP,3,'3'};
    case PLP_IMP: return  (struct instruction_info){1, MEM_IMP,4,'4'};

    case BPL_REL: return  (struct instruction_info){2, MEM_REL,2,' '};
    case BMI_REL: return  (struct instruction_info){2, MEM_REL,2,' '};
    case BVC_REL: return  (struct instruction_info){2, MEM_REL,2,' '};
    case BVS_REL: return  (struct instruction_info){2, MEM_REL,2,' '};
    case BCC_REL: return  (struct instruction_info){2, MEM_REL,2,' '};
    case BCS_REL: return  (struct instruction_info){2, MEM_REL,2,' '};
    case BNE_REL: return  (struct instruction_info){2, MEM_REL,2,' '};
    case BEQ_REL: return  (struct instruction_info){2, MEM_REL,2,' '};

    case BRK_IMP: return  (struct instruction_info){1, MEM_IMP,7,'7'};

    case CLC_IMP: return  (struct instruction_info){1, MEM_IMP,2,'2'};
    case SEC_IMP: return  (struct instruction_info){1, MEM_IMP,2,'2'};
    case CLI_IMP: return  (struct instruction_info){1, MEM_IMP,2,'2'};
    case SEI_IMP: return  (struct instruction_info){1, MEM_IMP,2,'2'};
    case CLV_IMP: return  (struct instruction_info){1, MEM_IMP,2,'2'};
    case CLD_IMP: return  (struct instruction_info){1, MEM_IMP,2,'2'};
    case SED_IMP: return  (struct instruction_info){1, MEM_IMP,2,'2'};

    case NOP_IMP: return  (struct instruction_info){1, MEM_IMP,2,'2'};

    case TAX_IMP: return  (struct instruction_info){1, MEM_IMP,2,'2'};
    case TXA_IMP: return  (struct instruction_info){1, MEM_IMP,2,'2'};
    case DEX_IMP: return  (struct instruction_info){1, MEM_IMP,2,'2'};
    case INX_IMP: return  (struct instruction_info){1, MEM_IMP,2,'2'};
    case TAY_IMP: return  (struct instruction_info){1, MEM_IMP,2,'2'};
    case TYA_IMP: return  (struct instruction_info){1, MEM_IMP,2,'2'};
    case DEY_IMP: return  (struct instruction_info){1, MEM_IMP,2,'2'};
    case INY_IMP: return  (struct instruction_info){1, MEM_IMP,2,'2'};

    case RTI_IMP: return  (struct instruction_info){1, MEM_IMP,6,'6'};

    case RTS_IMP: return  (struct instruction_info){1, MEM_IMP,6,'6'};

    case LSR_AC: return  (struct instruction_info){1, MEM_AC,2,'2'};
    case LSR_ZP: return  (struct instruction_info){2, MEM_ZP,5,'5'};
    case LSR_ZPX: return  (struct instruction_info){2, MEM_ZPX,6,'6'};
    case LSR_AB: return  (struct instruction_info){3, MEM_AB,6,'6'};
    case LSR_ABX: return  (struct instruction_info){3, MEM_ABX,7,'7'};

    case ROL_AC: return  (struct instruction_info){1, MEM_AC,2,'2'};
    case ROL_ZP: return  (struct instruction_info){2, MEM_ZP,5,'5'};
    case ROL_ZPX: return  (struct instruction_info){2, MEM_ZPX,6,'6'};
    case ROL_AB: return  (struct instruction_info){3, MEM_AB,6,'6'};
    case ROL_ABX: return  (struct instruction_info){3, MEM_ABX,7,'7'};

    case ROR_AC: return  (struct instruction_info){1, MEM_AC,2,'2'};
    case ROR_ZP: return  (struct instruction_info){2, MEM_ZP,5,'5'};
    case ROR_ZPX: return  (struct instruction_info){2, MEM_ZPX,6,'6'};
    case ROR_AB: return  (struct instruction_info){3, MEM_AB,6,'6'};
    case ROR_ABX: return  (struct instruction_info){3, MEM_ABX,7,'7'};

    case ASL_AC: return  (struct instruction_info){1, MEM_AC,2,'2'};
    case ASL_ZP: return  (struct instruction_info){2, MEM_ZP,5,'5'};
    case ASL_ZPX: return  (struct instruction_info){2, MEM_ZPX,6,'6'};
    case ASL_AB: return  (struct instruction_info){3, MEM_AB,6,'6'};
    case ASL_ABX: return  (struct instruction_info){3, MEM_ABX,7,'7'};

    case LDX_IMM: return  (struct instruction_info){2, MEM_IMM,2,'2'};
    case LDX_ZP: return  (struct instruction_info){2, MEM_ZP,3,'3'};
    case LDX_ZPY: return  (struct instruction_info){2, MEM_ZPY,4,'4'};
    case LDX_AB: return  (struct instruction_info){3, MEM_AB,4,'4'};
    case LDX_ABX: return  (struct instruction_info){3, MEM_ABX,4,'+'};

    case LDY_IMM: return  (struct instruction_info){2, MEM_IMM,2,'2'};
    case LDY_ZP: return  (struct instruction_info){2, MEM_ZP,3,'3'};
    case LDY_ZPX: return  (struct instruction_info){2, MEM_ZPX,4,'4'};
    case LDY_AB: return  (struct instruction_info){3, MEM_AB,4,'4'};
    case LDY_ABX: return  (struct instruction_info){3, MEM_ABX,4,'+'};

    case DEC_ZP: return  (struct instruction_info){2, MEM_ZP,5,'5'};
    case DEC_ZPX: return  (struct instruction_info){2, MEM_ZPX,6,'6'};
    case DEC_AB: return  (struct instruction_info){3, MEM_AB,6,'6'};
    case DEC_ABX: return  (struct instruction_info){3, MEM_ABX,7,'7'};

    case INC_ZP: return  (struct instruction_info){2, MEM_ZP,5,'5'};
    case INC_ZPX: return  (struct instruction_info){2, MEM_ZPX,6,'6'};
    case INC_AB: return  (struct instruction_info){3, MEM_AB,6,'6'};
    case INC_ABX: return  (struct instruction_info){3, MEM_ABX,7,'7'};

    case STX_ZP: return  (struct instruction_info){2, MEM_ZP,3,'3'};
    case STX_ZPY: return  (struct instruction_info){2, MEM_ZPY,4,'4'};
    case STX_AB: return  (struct instruction_info){3, MEM_AB,4,'4'};

    case STY_ZP: return  (struct instruction_info){2, MEM_ZP,3,'3'};
    case STY_ZPX: return  (struct instruction_info){2, MEM_ZPX,4,'4'};
    case STY_AB: return  (struct instruction_info){3, MEM_AB,4,'4'};

    case CPX_IMM: return  (struct instruction_info){2, MEM_IMM,2,'2'};
    case CPX_ZP: return  (struct instruction_info){2, MEM_ZP,3,'3'};
    case CPX_AB: return  (struct instruction_info){3, MEM_AB,4,'4'};

    case CPY_IMM: return  (struct instruction_info){2, MEM_IMM,2,'2'};
    case CPY_ZP: return  (struct instruction_info){2, MEM_ZP,3,'3'};
    case CPY_AB: return  (struct instruction_info){3, MEM_AB,4,'4'};

    case BIT_ZP: return  (struct instruction_info){2, MEM_ZP,3,'3'};
    case BIT_AB: return  (struct instruction_info){3, MEM_AB,4,'4'};

    case JMP_AB: return  (struct instruction_info){3, MEM_AB,3,'3'};
    case JMP_IN: return  (struct instruction_info){3, MEM_IN,5,'5'};

    case JSR_AB: return  (struct instruction_info){3, MEM_AB,6,'6'};


    default:
      return (struct instruction_info){0,0,0,'0'};
  }

}

string CPU::get_inst_str(int inst){
  switch(inst){

    case 0x69:   return "ADC";
    case 0x65:   return "ADC";
    case 0x75:   return "ADC";
    case 0x6D:   return "ADC";
    case 0x7D:   return "ADC";
    case 0x79:   return "ADC";
    case 0x61:   return "ADC";
    case 0x71:   return "ADC";

    case 0xE9:   return "SBC";
    case 0xE5:   return "SBC";
    case 0xF5:   return "SBC";
    case 0xED:   return "SBC";
    case 0xFD:   return "SBC";
    case 0xF9:   return "SBC";
    case 0xE1:   return "SBC";
    case 0xF1:   return "SBC";

    case 0x29:   return "AND";
    case 0x25:   return "AND";
    case 0x35:   return "AND";
    case 0x2D:   return "AND";
    case 0x3D:   return "AND";
    case 0x39:   return "AND";
    case 0x21:   return "AND";
    case 0x31:   return "AND";

    case 0xC9:   return "CMP";
    case 0xC5:   return "CMP";
    case 0xD5:   return "CMP";
    case 0xCD:   return "CMP";
    case 0xDD:   return "CMP";
    case 0xD9:   return "CMP";
    case 0xC1:   return "CMP";
    case 0xD1:   return "CMP";

    case 0x49:   return "EOR";
    case 0x45:   return "EOR";
    case 0x55:   return "EOR";
    case 0x4D:   return "EOR";
    case 0x5D:   return "EOR";
    case 0x59:   return "EOR";
    case 0x41:   return "EOR";
    case 0x51:   return "EOR";

    case 0x09:   return "ORA";
    case 0x05:   return "ORA";
    case 0x15:   return "ORA";
    case 0x0D:   return "ORA";
    case 0x1D:   return "ORA";
    case 0x19:   return "ORA";
    case 0x01:   return "ORA";
    case 0x11:   return "ORA";

    case 0xA9:   return "LDA";
    case 0xA5:   return "LDA";
    case 0xB5:   return "LDA";
    case 0xAD:   return "LDA";
    case 0xBD:   return "LDA";
    case 0xB9:   return "LDA";
    case 0xA1:   return "LDA";
    case 0xB1:   return "LDA";

    case 0x85:   return "STA";
    case 0x95:   return "STA";
    case 0x8D:   return "STA";
    case 0x9D:   return "STA";
    case 0x99:   return "STA";
    case 0x81:   return "STA";
    case 0x91:   return "STA";

    case 0x9A:   return "TXS";
    case 0xBA:   return "TSX";
    case 0x48:   return "PHA";
    case 0x68:   return "PLA";
    case 0x08:   return "PHP";
    case 0x28:   return "PLP";

    case 0x10:   return "BPL";
    case 0x30:   return "BMI";
    case 0x50:   return "BVC";
    case 0x70:   return "BVS";
    case 0x90:   return "BCC";
    case 0xB0:   return "BCS";
    case 0xD0:   return "BNE";
    case 0xF0:   return "BEQ";

    case 0x00:   return "BRK";

    case 0x18:   return "CLC";
    case 0x38:   return "SEC";
    case 0x58:   return "CLI";
    case 0x78:   return "SEI";
    case 0xB8:   return "CLV";
    case 0xD8:   return "CLD";
    case 0xF8:   return "SED";

    case 0xEA:   return "NOP";

    case 0xAA:   return "TAX";
    case 0x8A:   return "TXA";
    case 0xCA:   return "DEX";
    case 0xE8:   return "INX";
    case 0xA8:   return "TAY";
    case 0x98:   return "TYA";
    case 0x88:   return "DEY";
    case 0xC8:   return "INY";

    case 0x40:   return "RTI";

    case 0x60:   return "RTS";

    case 0x4A:   return "LSR";
    case 0x46:   return "LSR";
    case 0x56:   return "LSR";
    case 0x4E:   return "LSR";
    case 0x5E:   return "LSR";

    case 0x2A:   return "ROL";
    case 0x26:   return "ROL";
    case 0x36:   return "ROL";
    case 0x2E:   return "ROL";
    case 0x3E:   return "ROL";

    case 0x6A:   return "ROR";
    case 0x66:   return "ROR";
    case 0x76:   return "ROR";
    case 0x6E:   return "ROR";
    case 0x7E:   return "ROR";

    case 0x0A:   return "ASL";
    case 0x06:   return "ASL";
    case 0x16:   return "ASL";
    case 0x0E:   return "ASL";
    case 0x1E:   return "ASL";

    case 0xA2:   return "LDX";
    case 0xA6:   return "LDX";
    case 0xB6:   return "LDX";
    case 0xAE:   return "LDX";
    case 0xBE:   return "LDX";

    case 0xA0:   return "LDY";
    case 0xA4:   return "LDY";
    case 0xB4:   return "LDY";
    case 0xAC:   return "LDY";
    case 0xBC:   return "LDY";

    case 0xC6:   return "DEC";
    case 0xD6:   return "DEC";
    case 0xCE:   return "DEC";
    case 0xDE:   return "DEC";

    case 0xE6:   return "INC";
    case 0xF6:   return "INC";
    case 0xEE:   return "INC";
    case 0xFE:   return "INC";

    case 0x86:   return "STX";
    case 0x96:   return "STX";
    case 0x8E:   return "STX";

    case 0x84:   return "STY";
    case 0x94:   return "STY";
    case 0x8C:   return "STY";

    case 0xE0:   return "CPX";
    case 0xE4:   return "CPX";
    case 0xEC:   return "CPX";

    case 0xC0:   return "CPY";
    case 0xC4:   return "CPY";
    case 0xCC:   return "CPY";

    case 0x24:   return "BIT";
    case 0x2C:   return "BIT";

    case 0x4C:   return "JMP";
    case 0x6C:   return "JMP";

    case 0x20:   return "JSR";

    default:	return "Error";
  }

}
