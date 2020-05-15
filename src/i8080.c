#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emu_utils.h>
#include <disassembler.h>
#include <i8080.h>
#include <machine.h>

#define SET_Z_FLAG  (1 << 7)
#define SET_S_FLAG  (1 << 6)
#define SET_P_FLAG  (1 << 5)
#define SET_CY_FLAG (1 << 4)
#define SET_AC_FLAG (1 << 3)
#define SET_ALL_FLAGS (SET_Z_FLAG | SET_S_FLAG | SET_P_FLAG | SET_CY_FLAG | SET_AC_FLAG)

int half_carry_table[] = { 0, 0, 1, 0, 1, 0, 1, 1 };
int sub_half_carry_table[] = { 0, 1, 1, 1, 0, 0, 0, 1 };

uint8_t Parity(uint16_t value) {
  uint8_t x = value & 0xff;
  int size = 8;
  int i;
  int p = 0;
  x = (x & ((1 << size) - 1));
  for (i = 0; i < size; i++) {
      if (x & 0x1) {
          p++;
      }
      x = x >> 1;
  }
  return (0 == (p & 0x1));
}

uint8_t AuxCarry(uint16_t value) {
 return 0;  
}

uint8_t Zero(uint16_t value) {
  return ((value & 0xff) == 0);
}

uint8_t Sign(uint16_t value) {
  return ((value & 0x80) == 0x80);
}

uint8_t Carry(uint16_t value) {
    return (value > 0xff); 
}

void SetArithFlags(State8080 *state, uint16_t value, uint8_t flagstoset) {
    // remove trailing bits
    uint8_t cleaned = flagstoset & 0b11111000;
    if (cleaned & SET_Z_FLAG) {
        state->cc.z = Zero(value);
           

    }
    if (cleaned & SET_S_FLAG) {
        state->cc.s = Sign(value);
    }
    if (cleaned & SET_P_FLAG) {
        state->cc.p = Parity(value);
    }
    if (cleaned & SET_CY_FLAG) {
        state->cc.cy = Carry(value);
    }
    if (cleaned & SET_AC_FLAG) {
      state->cc.ac = AuxCarry(value); 
  }
}

void SetLogicFlags(State8080 *state, uint8_t res, uint8_t flagstoset) {
    // remove trailing bits
    uint8_t cleaned = flagstoset & 0b11111000;
    uint16_t answer = (uint16_t) res;
    if (cleaned & SET_Z_FLAG) {
        state->cc.z = Zero(answer);
    }
    if (cleaned & SET_S_FLAG) {
        state->cc.s = Sign(answer);
    }
    if (cleaned & SET_P_FLAG) {
        state->cc.p = Parity(answer);
    }

    if (cleaned & SET_CY_FLAG) {
        state->cc.cy = 0;
    }
    if (cleaned & SET_AC_FLAG) {
        state->cc.ac = 0; 
  }
}



void increment(State8080* state, uint8_t *ptr) {
  uint16_t value = (uint16_t) *ptr + 1;
  // INR does not affect carry bit
  uint8_t flags = SET_Z_FLAG | SET_S_FLAG | SET_P_FLAG | SET_AC_FLAG;
  SetArithFlags(state, value, flags);
  *ptr = value & 0xff;
}


void inx_xy(uint8_t *x, uint8_t *y) {
  (*y)++;
    if (*y == 0) {
        (*x)++;
    }
}

void dcx_xy(uint8_t *x, uint8_t *y) {
  Add16Bit(x,y,-1);
}
void dad_xy(State8080 *state, uint8_t *x, uint8_t *y) {
  uint16_t value = MakeWord(*x,*y);
  uint32_t result = Add16Bit(&state->h, &state->l, value);
  state->cc.cy = ((result & 0xffff0000) != 0);
}

void decrement(State8080* state, uint8_t *ptr) {
  uint16_t value = (uint16_t) *ptr - 1;
  // INR does not affect carry bit
  uint8_t flags = SET_Z_FLAG | SET_S_FLAG | SET_P_FLAG | SET_AC_FLAG;
  SetArithFlags(state, value, flags);
  *ptr = value & 0xff;
}



uint16_t read_hl_addr(State8080 *state) 
{
  return MakeWord(state->h, state->l);
}
uint8_t read_hl(State8080 *state) 
{
  uint16_t offset = read_hl_addr(state);
  uint8_t m = state->memory[offset];
  return m;
}

void write_state(State8080 *state, FILE *pFile) {
    fprintf(pFile, "A: 0x%02x \n", state->a);
    fprintf(pFile,"B: 0x%02x ", state->b);
    fprintf(pFile,"C: 0x%02x ", state->c);
    fprintf(pFile,"D: 0x%02x ", state->d);
    fprintf(pFile,"E: 0x%02x ", state->e);
    fprintf(pFile,"H: 0x%02x ", state->h);
    fprintf(pFile,"L: 0x%02x \n", state->l);
    fprintf(pFile,"Z: %d", state->cc.z);
    fprintf(pFile,"S: %d", state->cc.s);
    fprintf(pFile,"P: %d", state->cc.p);
    fprintf(pFile,"CY: %d", state->cc.cy);
    fprintf(pFile,"AC %d\n", state->cc.ac);
    fprintf(pFile,"SP: 0x%04x\n", state->sp);
    fprintf(pFile,"PC: 0x%04x\n", state->pc);
    fprintf(pFile,"Interrupt enable: %d\n", state->int_enable);
    fprintf(pFile,"Memory Current: 0x%04x ", read_hl(state));
    fprintf(pFile,"\n");
}

void print_state(State8080 *state) {
    printf("\n");
    printf("----------------------------------\n");
    printf(" A    B    C    D    E    H    L  \n");
    printf("0x%02x ", state->a);
    printf("0x%02x ", state->b);
    printf("0x%02x ", state->c);
    printf("0x%02x ", state->d);
    printf("0x%02x ", state->e);
    printf("0x%02x ", state->h);
    printf("0x%02x ", state->l);
    printf("\n");
    printf("----------------------------------\n");
    printf(" Z S P CY AC \n");
    printf(" %d", state->cc.z);
    printf(" %d", state->cc.s);
    printf(" %d", state->cc.p);
    printf(" %d", state->cc.cy);
    printf("  %d", state->cc.ac);
    printf("\n\n");
    printf(" SP: 0x%04x\n", state->sp);
    printf(" PC: 0x%04x\n", state->pc);
    printf(" Interrupt enable: %d\n", state->int_enable);
    printf("----------------------------------\n");
    printf("\n");
}

void set_hl(State8080* state, uint8_t val)
{
  uint16_t offset = read_hl_addr(state);
  state->memory[offset] = val;
}

void inr_hl(State8080* state){
  uint16_t offset = read_hl_addr(state);
  uint8_t *m_ptr = &state->memory[offset];
  increment(state, m_ptr);
}
void dcr_hl(State8080* state){
  uint16_t offset = read_hl_addr(state);
  uint8_t *m_ptr = &state->memory[offset];
  decrement(state, m_ptr);
}


uint8_t GetValueOfRegister(State8080* state, unsigned char code) 
{
  char dest = GetDest(code);
  switch(dest) {
    case 'B': return state->b;
    case 'C': return state->c;
    case 'D': return state->d;
    case 'E': return state->e;
    case 'H': return state->h;
    case 'L': return state->l;
    case 'M': return read_hl(state);
    case 'A': return state->a;
  }
  return 0;
}


void MoveRegister(unsigned char code, State8080* state) 
{
  char target = GetSource(code);
  switch(target) {
    case 'B': state->b = GetValueOfRegister(state, code); break;
    case 'C': state->c = GetValueOfRegister(state, code); break;
    case 'D': state->d = GetValueOfRegister(state, code); break;
    case 'E': state->e = GetValueOfRegister(state, code); break;
    case 'H': state->h = GetValueOfRegister(state, code); break;
    case 'L': state->l = GetValueOfRegister(state, code); break;
    case 'M': set_hl(state, GetValueOfRegister(state, code)); break;
    case 'A': state->a = GetValueOfRegister(state, code); break;
  }

}


void GenericAdd(State8080* state, uint16_t value) {
  uint16_t answer = state->a + value;
  SetArithFlags(state, answer, SET_ALL_FLAGS);
  state->a = answer & 0xff; 
}

void AddI(State8080* state, unsigned char code) {
  GenericAdd(state, (uint16_t) code);
  state->pc += 1;
}

void AddCI(State8080* state, unsigned char code) {
  GenericAdd(state, (uint16_t) (((uint8_t) code) + state->cc.cy));
  state->pc += 1;
}

void Add(State8080* state, unsigned char code) {
  GenericAdd(state, (uint16_t) GetValueOfRegister(state, code));
}

void AddC(State8080* state, unsigned char code) {
  GenericAdd(state, (uint16_t) (GetValueOfRegister(state, code) + state->cc.cy));
}

void Sub(State8080* state, unsigned char code) {
  GenericAdd(state,  (uint16_t) -GetValueOfRegister(state, code));
}

void SubI(State8080* state, unsigned char code ) {
  GenericAdd(state, (uint16_t) -code);
}

void SubCI(State8080* state, unsigned char code) {
  SubI(state, (uint16_t) (((uint8_t) code) + state->cc.cy));
  state->pc += 1;
}

void SubB(State8080* state, unsigned char code) {
  GenericAdd(state, (uint16_t) (-GetValueOfRegister(state, code) - state->cc.cy));
}
void SubBI(State8080* state, unsigned char code) {
  GenericAdd(state, (uint16_t) (-code - state->cc.cy));
}

void Ana(State8080* state, uint8_t x) {
  uint16_t result = state->a & x;
  SetLogicFlags(state, result, SET_ALL_FLAGS);
  state->a = result & 0xff;
}

void Ori(State8080* state, uint8_t x) {
  uint16_t result = (uint16_t) state->a | x;
  SetLogicFlags(state, result, SET_ALL_FLAGS);
  state->a = result & 0xff;
}

void Xra(State8080 *state, uint8_t x) {
  uint16_t result = state->a ^ x;
  SetLogicFlags(state, result, SET_ALL_FLAGS);
  state->a = result & 0xff;
}

void Pop(State8080* state, uint8_t *right, uint8_t *left) {
  uint16_t sp_addr;
  sp_addr = state->sp;
  *right = state->memory[sp_addr];
  *left = state->memory[sp_addr + 1];
  state->sp += 2;
}

void Push(State8080 *state, uint8_t hi, uint8_t lo) {
  uint16_t sp_addr;
  sp_addr = state->sp;
  state->memory[sp_addr - 1] = hi;
  state->memory[sp_addr - 2] = lo;
  state->sp -= 2;
}

void mvi(State8080 *state, uint8_t *ptr, uint8_t load) {
  uint16_t value = (uint16_t) load;
  *ptr = value & 0xff;
  state->pc += 1;
}

void jmp(State8080 *state, uint16_t adr) {
  state->pc = adr;
}

void cmp(State8080 *state, uint8_t x) {
  uint16_t answer;
  answer =  state->a - x;
  SetArithFlags(state, answer, SET_ALL_FLAGS ^ SET_CY_FLAG);
  state->cc.cy = state->a < x;

}

void jmp_cond(State8080 *state, uint16_t adr, uint8_t cond) {
  if (cond) {
    jmp(state, adr);
  } else {
    state->pc += 2;
  }
}

void call(State8080 *state, uint16_t adr){
  // cache return adress on the stack
  uint16_t ret_addr = state->pc + 2;
  uint8_t hi, lo;
  hi = (ret_addr >> 8) & 0xff;
  lo = ret_addr & 0xff;
  state->sp -= 1;
  state->memory[state->sp] = hi;
  state->sp -= 1;
  state->memory[state->sp] = lo;
  state->pc = adr;
}

void condCall(State8080 *state, uint16_t adr, uint8_t cond) {
  if (cond) {
    call(state, adr);
  } else {
    state->pc += 2;
  }
  // this is 3 opcodes but incrementing the PC for the return is handled
  // in the call function
}

void ret(State8080 *state) {
  uint16_t sp_addr;
  sp_addr = state->sp;
  uint8_t lo = state->memory[sp_addr];
  uint8_t hi = state->memory[sp_addr + 1];
  uint16_t target = MakeWord(hi, lo);
  state->pc = target;
  state->sp += 2;
}

void daa(State8080* state){
  // first step, split A into two parts
  uint8_t upper = state->a >> 4;
  uint8_t lower = state->a & 0x0F;

  if (lower >= 10) {
    state->a += 6;
  }
  if (upper > 0x90)
  {
      uint16_t result = (uint16_t) state->a + 0x60;
      state->a = result & 0xff;
      SetArithFlags(state, result, SET_ALL_FLAGS);
  }
}

void condRet(State8080 *state, uint8_t cond) {
  if (cond) {
    ret(state);
  }
}


void UnimplementedInstruction(State8080* state)
{
  printf("Error: Unimplemented instruction %02x\n", state->memory[state->pc-1]);
  print_state(state);
  exit(1);
}

void UnusedInstruction(State8080* state)
{
  return;
}

void swp(uint8_t *p1, uint8_t *p2) {
  uint8_t tmp;
  tmp = *p1;
  *p1 = *p2;
  *p2 = tmp;
}


int Emulate8080Op(State8080* state, int debug)
{

  unsigned char *opcode = &state->memory[state->pc];
  if (debug) {
    printf("%04x %02x ", state->pc, opcode[0]);
    Disassemble8080OP(state->memory,state->pc);
  }
  
  

  state->pc += 1;

  int first = GetFirstPart(opcode[0]);
  int last  = GetLastPart(opcode[0]);
  if (first >= 4 && first <= 7) {
    if (first == 7 && last == 6) {
     printf("HALT");
    } else {
      MoveRegister(opcode[0], state);
    }
  } else if (first == 8 && last < 8) {
    // ADD A X
    Add(state, opcode[0]);
  } else if (first == 8 && last >= 8) {
    AddC(state, opcode[0]);
  } else if (first == 9) {
    if (last < 8) {
      Sub(state, GetValueOfRegister(state, opcode[0]));
    } else {
      SubB(state, GetValueOfRegister(state, opcode[0]));
    }

  } else if (first == 10) {
    if (last < 8) {
      Ana(state, GetValueOfRegister(state, opcode[0]));
    } else {
      Xra(state, GetValueOfRegister(state, opcode[0]));
    }
  } else if (first == 11) {
    if (last < 8) {
      Ori(state, GetValueOfRegister(state, opcode[0]));
    } else {
      cmp(state, GetValueOfRegister(state, opcode[0]));
    }
  } else {
    switch(*opcode) 
  {
    case 0x00: break;
    case 0x01:
      state->c = opcode[1];  // c <- byte 2
      state->b = opcode[2];  // b <- byte 3
      state->pc += 2;  // advance two more bytes
      break;
    case 0x02: 
      {
        uint16_t offset = MakeWord(state->b, state->c);
        state->memory[offset] = state->a;
      }
      break; // move (BC)<-A, move accumulator register to (BC) pair
    case 0x03: inx_xy(&state->b, &state->c); break;    // increment register pair
    case 0x04: increment(state, &state->b); break;
    case 0x05: decrement(state, &state->b); break;    // decrement register
    case 0x06: {
      state->b = opcode[1];  // b <- byte 2
      state->pc += 1;
    } 
    break;    // B <- byte 2
    case 0x07: {
      state->cc.cy = state->a >> 7;
      state->a = (state->a << 1) | state->cc.cy;
    }break;  // shift A left 1 bit, wrap shifted last bit to the end
    case 0x08: UnusedInstruction(state); break;
    case 0x09: dad_xy(state, &state->b, &state->c); break;     // Add BC to HL and set HL to result
    case 0x0a: {
      uint16_t offset = MakeWord(state->b, state->c);
      state->a = state->memory[offset];
    }
    break;     // A <- (BC)
    case 0x0b: dcx_xy(&state->b, &state->c);break;  // BC = BC - 1
    case 0x0c: increment(state, &state->c); break;
    case 0x0d: decrement(state, &state->c); break;
    case 0x0e: mvi(state, &state->c, opcode[1]); break;
    case 0x0f: {
        uint8_t val = state->a;
        state->cc.cy = state->a & 1;
        uint8_t rotated = (val >> 1) | (state->cc.cy << 7);
        state->a = rotated;
      }
      break;     // shift A right 1 bit, wrap shifted first bit to the end
    
    case 0x10:  UnusedInstruction(state); break;
    case 0x11:
      state->e = opcode[1]; 
      state->d = opcode[2]; 
      state->pc += 2;  
      break;
    case 0x12: {
        uint16_t offset = MakeWord(state->d, state->e);
        state->memory[offset] = state->a;
    } break;
    case 0x13: inx_xy(&state->d, &state->e); break;
    case 0x14: increment(state, &state->d);break;    // increment register
    case 0x15: decrement(state, &state->d);break;
    case 0x16: mvi(state, &state->d, opcode[1]); break; // D <- byte 2
    case 0x17: {
      int cy = state->cc.cy;
      state->cc.cy = state->a >> 7;
      state->a = (state->a << 1) | cy;
    }
    break;
    case 0x18:  UnusedInstruction(state);break;
    case 0x19: dad_xy(state, &state->d, &state->e);break;
    case 0x1a: 
      {
        uint16_t offset = MakeWord(state->d, state->e);
        uint8_t memval = state->memory[offset];
        state->a = memval;
      }
    break;    // A <- (BC)
    case 0x1b: dcx_xy(&state->d, &state->e); break;     // BC = BC - 1
    case 0x1c: increment(state, &state->e); break;    // increment register
    case 0x1d: decrement(state, &state->e); break;
    case 0x1e: mvi(state, &state->e, opcode[1]);  break;
    case 0x1f: {
    int cy = state->cc.cy;
    state->cc.cy = state->a & 1;
    state->a = (state->a >> 1) | (cy << 7);
    }break; // // shift A right 1 bit, set bit 7 to previoous bit 7

    case 0x20: UnusedInstruction(state); break; // Special instruction
    case 0x21: 
      state->l = opcode[1];  // c <- byte 2
      state->h = opcode[2];  // b <- byte 3
      state->pc += 2;  // advance two more bytes
      break;     // load into (DE)
    case 0x22: {
        uint16_t addr = MakeWord(opcode[2], opcode[1]);
        state->memory[addr] = state->l;
        state->memory[addr + 1] = state->h;    
        state->pc += 2;
    };break;
    case 0x23: inx_xy(&state->h, &state->l);break;
    case 0x24: increment(state, &state->h);break;    // increment register
    case 0x25: decrement(state, &state->h); break;
    case 0x26: mvi(state, &state->h, opcode[1]); break; // D <- byte 2
    case 0x27: daa(state);break; // Special
    case 0x28: UnusedInstruction(state); break;
    case 0x29: dad_xy(state, &state->h, &state->l);break;
    case 0x2a: {
            uint16_t addr = MakeWord(opcode[2], opcode[1]);
            uint8_t lo, hi;
            lo = state->memory[addr];
            hi = state->memory[addr + 1]; 
            state->l = lo;
            state->h = hi;
            state->pc += 2;
    } break; // load HL pair
    //  using Direct addressing, 16 bit address so 2 registers
    case 0x2b: dcx_xy(&state->h, &state->l); break;     // BC = BC - 1
    case 0x2c: increment(state, &state->l);break;    
    case 0x2d: decrement(state, &state->l); break;
    case 0x2e: mvi(state, &state->l, opcode[1]); break; 
    case 0x2f: {
      state->a = ~state->a;
    }break; // A <- !A 
    case 0x30: UnusedInstruction(state); break;
    case 0x31: {
        uint8_t byte2;
        uint8_t byte3;
        byte3 = opcode[2];
        byte2 = opcode[1];
        state->sp = MakeWord(byte3, byte2);
        state->pc += 2;
      }
      break; // set stack pointer
    
    case 0x32: {
        uint16_t addr = MakeWord(opcode[2], opcode[1]);
        state->memory[addr] = state->a;
        state->pc += 2;  
      }
      break;
    case 0x33: state->sp = state->sp + 1; break;
    case 0x34: inr_hl(state); break;    // increment registerbreak;
    case 0x35: dcr_hl(state); break;
    case 0x36: {
      set_hl(state, opcode[1]); state->pc+=1; 
      }break; 
    case 0x37: {
      state->cc.cy = 1;
    }break; // CY =1
    case 0x38: UnusedInstruction(state); break;
    case 0x39: 
      {
        uint32_t answer;
        
        answer = Add16Bit(
            &state->h, &state->l, state->sp);
        state->cc.cy = ((answer & 0xffff0000) != 0);
      }
      break;
    case 0x3a:{
      uint16_t addr = MakeWord(opcode[2], opcode[1]);
      uint8_t val = state->memory[addr];
      state->a = val;
      state->pc += 2;
      }
      break; // load A from memory
    //  using Direct addressing, 16 bit address so 2 registers
    case 0x3b: {
        uint16_t curr_sp = state->sp;
        state->sp = curr_sp - 1;
      }
      break;     // BC = BC - 1
    case 0x3c: increment(state, &state->a); break;    // increment registerbreak;
    case 0x3d: decrement(state, &state->a); break;
    case 0x3e: mvi(state, &state->a, opcode[1]); break;
    case 0x3f: {
      state->cc.cy = !state->cc.cy;
    }break; // CY <- !CY

    case 0xc0: condRet(state, state->cc.z == 0); break; // if zero flag not set return (NZ)
    case 0xc1: Pop(state, &state->c, &state->b); break; 
    case 0xc2: jmp_cond(state, MakeWord(opcode[2], opcode[1]), state->cc.z == 0);break; 
    case 0xc3: jmp(state, MakeWord(opcode[2], opcode[1])); return 0; break; // PC <- adr
    case 0xc4: condCall(state, MakeWord(opcode[2], opcode[1]), (state->cc.z == 0)); break;
    case 0xc5: Push(state, state->b, state->c); break;
    case 0xc6: AddI(state, opcode[1]); break; // A <- A + byte
    case 0xc7: call(state, 0x0000); break; // reset 
    case 0xc8: condRet(state, state->cc.z); break;
    case 0xc9: ret(state); break; //sp +2 
    case 0xca: jmp_cond(state, MakeWord(opcode[2], opcode[1]), state->cc.z);break;
    case 0xcb: UnusedInstruction(state); break;
    case 0xcc: condCall(state, MakeWord(opcode[2], opcode[1]), state->cc.z); break; // if Z, call adr
    case 0xcd: call(state, MakeWord(opcode[2], opcode[1])); break; // sp<-pc, sp+2, pc=adr
    case 0xce: AddCI(state, opcode[1]);  break; // A <- A + data+ CY
    case 0xcf: call(state, 0x0008); break; //call $8
    
    case 0xd0: condRet(state, state->cc.cy == 0); break;
    case 0xd1: Pop(state, &state->e, &state->d); break; 
    case 0xd2: jmp_cond(state, MakeWord(opcode[2], opcode[1]), (state->cc.cy == 0)); break; 
    case 0xd3: break; // Handled In Upper Layer
    case 0xd4: condCall(state, MakeWord(opcode[2], opcode[1]), (state->cc.cy == 0)); break; 
    case 0xd5: Push(state, state->d, state->e); break;
    case 0xd6: SubI(state, opcode[1]);  break;
    case 0xd7: call(state, 0x0010); break; //call $8
    case 0xd8: condRet(state, state->cc.cy); break;
    case 0xd9: UnusedInstruction(state); break;
    case 0xda: jmp_cond(state, MakeWord(opcode[2], opcode[1]), state->cc.cy); break; // if CY, PC<-adr
    case 0xdb: break; // In
    case 0xdc: condCall(state, MakeWord(opcode[2], opcode[1]), state->cc.cy); break; // if CY, call adr
    case 0xdd: UnusedInstruction(state); break;
    case 0xde: SubCI(state, opcode[1]);  break;
    case 0xdf: call(state, 0x0018); break; //call $8

    case 0xe0: condRet(state, state->cc.p == 0); break;
    case 0xe1: Pop(state, &state->l, &state->h); break; 
    case 0xe2: jmp_cond(state, MakeWord(opcode[2], opcode[1]), state->cc.p == 0);  break; // if PO, PC<-adr
    case 0xe3: {
      uint16_t sp = state->sp;
      uint8_t *sp_h, *sp_l;
      sp_h = &state->memory[sp + 1];
      sp_l = &state->memory[sp]; 
      swp(&state->l, sp_l);
      swp(&state->h, sp_h);
    } break;
    case 0xe4: {
      condCall(state, MakeWord(opcode[2], opcode[1]), state->cc.p == 0); 
      
      }break; // if PO, call adr
    case 0xe5: Push(state, state->h, state->l); break;
    case 0xe6: {
        Ana(state, opcode[1]);
        state->pc++;
      }
      break;
    case 0xe7: call(state, 0x0020); break; //call $20
    case 0xe8: condRet(state, state->cc.p); break;
    case 0xe9: {
      state->pc = MakeWord(state->h, state->l);
    }break; // jump to address HL
    case 0xea: jmp_cond(state, MakeWord(opcode[2], opcode[1]), state->cc.p);  break; // if CY, PC<-adr
    case 0xeb: 
      {
        swp(&state->h, &state->d);
        swp(&state->l, &state->e);
      }
      break; // Swap DE and HL
    case 0xec:  condCall(state,MakeWord(opcode[2], opcode[1]), state->cc.p); break; // if CY, call adr
    case 0xed: UnusedInstruction(state); break;
    case 0xee: Xra(state, opcode[1]); state->pc += 1; break;
    case 0xef: call(state, 0x0028); break; //call $8

    case 0xf0: condRet(state, state->cc.s == 0); break;
    case 0xf1: {
            uint16_t sp_addr = state->sp;
            uint8_t sp_val = state->memory[sp_addr];

            // (CY) <- ((SP))O
            state->cc.cy = sp_val & 1;

            // (P) <- ((SP))2
            state->cc.p = (sp_val & (1 << 2)) > 0;

            // (AC) <- ((SP))4
            state->cc.ac = (sp_val & (1 << 4)) > 0;

            // (Z) <- ((SP))6
            state->cc.z = (sp_val & (1 << 6)) > 0;

            // (S) <- ((SP))7
            state->cc.s = (sp_val & (1 << 7)) > 0;

            // (A) <- ((SP) +1)
            state->a = state->memory[sp_addr + 1];

            // (SP) <- (SP) + 2
            state->sp += 2;
      }
      break;
    case 0xf2: jmp_cond(state, MakeWord(opcode[2], opcode[1]), (state->cc.s == 0)); break; // if PO, PC<-adr
    case 0xf3: 
      {
        state->int_enable = 0;
      } break; // swap HL with top of stack
    case 0xf4: condCall(state, MakeWord(opcode[2], opcode[1]), (state->cc.s == 0)); break; // if PO, call adr
    case 0xf5: 
      {
        uint16_t sp_adr = state->sp;
            
            state->memory[sp_adr - 1] = state->a;

            uint8_t sp_flags = 0;

            sp_flags |= state->cc.cy; 

            sp_flags |= (1 << 1);

            sp_flags |= (state->cc.p << 2);

            sp_flags |= (state->cc.ac << 4);

            sp_flags |= (state->cc.z << 6);

            sp_flags |= (state->cc.s << 7);
            state->memory[sp_adr - 2] = sp_flags;

            state->sp -= 2;
      }
      break;
    case 0xf6: Ori(state, opcode[1]); state->pc += 1; break;
    case 0xf7: call(state, 0x0030); break; //call $30
    case 0xf8: condRet(state, state->cc.s); break; // if PE; RET
    case 0xf9: {
      state->sp = MakeWord(state->h, state->l);
    } break; // jump to address HL
    case 0xfa: jmp_cond(state, MakeWord(opcode[2], opcode[1]), state->cc.s); break; // if CY, PC<-adr
    case 0xfb: 
      {
        state->int_enable = 1;
      }
      break;
    case 0xfc: condCall(state, MakeWord(opcode[2], opcode[1]), state->cc.s);  break; // if CY, call adr
    case 0xfd: UnusedInstruction(state); break;
    case 0xfe: 
      {
        cmp(state, opcode[1]);
        state->pc += 1;
      }
      break;
    case 0xff: call(state, 0x0038); break;
    default: 
      UnimplementedInstruction(state);
      break;
    }
  }
  return state->pc;
} 
