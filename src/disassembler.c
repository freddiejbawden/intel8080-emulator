#include <stdio.h>
#include <stdlib.h>
#include <emu_utils.h>

void Disassemble8080OP(unsigned char *codebuffer, int pc) {
  unsigned char *code = &codebuffer[pc];
  int opbytes = 1;
  if (GetFirstPart(code[0]) >= 4 && GetFirstPart(code[0]) <= 7){
    if (GetFirstPart(code[0]) == 7 && GetLastPart(code[0]) == 6) {
      printf("HLT");
    } else {
      printf("MOV ");
      GetSource(code[0]);
      printf(" ");
      printf("%c", GetDest(code[0]));
    }
  } else if (GetFirstPart(code[0]) == 8) {
    if (GetLastPart(code[0]) < 8) {
      printf("ADD ");
     printf("%c", GetDest(code[0]));

    } else {
      printf("ADC ");
     printf("%c", GetDest(code[0]));

    }
  } else if (GetFirstPart(code[0]) == 9) {
     if (GetLastPart(code[0]) < 8) {
      printf("SUB ");
     printf("%c", GetDest(code[0]));

    } else {
      printf("SBB ");
     printf("%c", GetDest(code[0]));

    }
  } else if (GetFirstPart(code[0]) == 10) {
     if (GetLastPart(code[0]) < 8) {
      printf("ANA ");
     printf("%c", GetDest(code[0]));

    } else {
      printf("XRA ");
     printf("%c", GetDest(code[0]));

    } 
  } else if (GetFirstPart(code[0]) == 11) {
     if (GetLastPart(code[0]) < 8) {
      printf("ORA ");
     printf("%c", GetDest(code[0]));

    } else {
      printf("CMP ");
     printf("%c", GetDest(code[0]));

    } 
  } else {
    switch(*code) 
    {
      case 0x00: printf("NOP"); break;
      case 0x01: printf("LXI %02x%02x", code[2],code[1]); opbytes=3; break;
      case 0x02: printf("STAX   B"); break; // move (BC)<-A, move accumulator register to (BC) pair
      case 0x03: printf("INX    B"); break;    // increment register pair
      case 0x04: printf("INR  B");break;    // increment register
      case 0x05: printf("DCR B"); break;     // decrement register
      case 0x06: printf("MVI %02x", code[1]); opbytes=2; break;    // B <- byte 2
      case 0x07: printf("RLC"); break;  // shift A left 1 bit, wrap shifted last bit to the end
      case 0x08: printf("Nothing"); break;
      case 0x09: printf("DAD B"); break;     // Add BC to HL and set HL to result
      case 0x0a: printf("LDAX B"); break;     // A <- (BC)
      case 0x0b: printf("DCX B"); break;  // BC = BC - 1
      case 0x0c: printf("INR C"); break;
      case 0x0d: printf("DCR C"); break;
      case 0x0e: printf("MVI C, %02x", code[1]); opbytes=2; break;
      case 0x0f: printf("RRC"); break;     // shift A right 1 bit, wrap shifted first bit to the end
      
      case 0x10: printf("Nothing"); break;
      case 0x11: printf("LXI %02x%02x", code[2],code[1]); opbytes=3; break;     // load into (DE)
      case 0x12: printf("STAX D"); break;
      case 0x13: printf("INX    D"); break;
      case 0x14: printf("INR  D");break;
      case 0x15: printf("DCR D"); break;
      case 0x16: printf("MVI %02x", code[1]); opbytes=2; break; // D <- byte 2
      case 0x17: printf("RAL"); break;
      case 0x18: printf("Nothing"); break;
      case 0x19: printf("DAD D"); break;
      case 0x1a: printf("LDAX D"); break;    // A <- (BC)
      case 0x1b: printf("DCX D"); break;     // BC = BC - 1
      case 0x1c: printf("INR E"); break;
      case 0x1d: printf("DCR E"); break;
      case 0x1e: printf("MVI %02x", code[1]); opbytes=2; break;
      case 0x1f: printf("RAR"); break; // // shift A right 1 bit, set bit 7 to previoous bit 7

      case 0x20: printf("RIM"); break; // Special instruction
      case 0x21: printf("LXI %02x%02x", code[2],code[1]); opbytes=3; break;     // load into (DE)
      case 0x22: printf("STAX H"); break;
      case 0x23: printf("INX  H"); break;
      case 0x24: printf("INR  H");break;
      case 0x25: printf("DCR H"); break;
      case 0x26: printf("MVI %02x", code[1]); opbytes=2; break; // D <- byte 2
      case 0x27: printf("DAA"); break; // Special
      case 0x28: printf("Nothing");
      case 0x29: printf("DAD H"); break;
      case 0x2a: printf("LHLD $%02x%02x", code[2], code[1]); opbytes=3; break; // load HL pair
      //  using Direct addressing, 16 bit address so 2 registers
      case 0x2b: printf("DCX H"); break;     // BC = BC - 1
      case 0x2c: printf("INR L"); break;
      case 0x2d: printf("DCR L"); break;
      case 0x2e: printf("MVI L, %02x", code[1]); opbytes=2; break;
      case 0x2f: printf("CMA"); break; // A <- !A 

      case 0x30: printf("SIM"); break;
      case 0x31: printf("LXI %02x%02x", code[2], code[1]); opbytes=3; break; // set stack pointer
      case 0x32: printf("STA $%02x%02x", code[2], code[1]); opbytes=3; break;
      case 0x33: printf("INX SP"); break;
      case 0x34: printf("INR  SP");break;
      case 0x35: printf("DCR SP"); break;
      case 0x36: printf("MVI %02x", code[1]); opbytes=2; break; // D <- byte 2
      case 0x37: printf("STC"); break; // CY =1
      case 0x38: printf("Nothing"); break;
      case 0x39: printf("DAD H"); break;
      case 0x3a: printf("LDA $%02x%02x", code[2], code[1]); opbytes=3; break; // load A from memory
      //  using Direct addressing, 16 bit address so 2 registers
      case 0x3b: printf("DCX SP"); break;     // BC = BC - 1
      case 0x3c: printf("INR A"); break;
      case 0x3d: printf("DCR A"); break;
      case 0x3e: printf("MVI A, %02x", code[1]); opbytes=2; break;
      case 0x3f: printf("CMC"); break; // CY <- !CY

      case 0xc0: printf("RNZ"); break; // if zero flag not set return (NZ)
      case 0xc1: printf("POP B"); break; 
      case 0xc2: printf("JNZ $%02x%02x", code[2], code[1]); opbytes = 3; break; // if NZ, PC <- adr
      case 0xc3: printf("JMP $%02x%02x", code[2], code[1]); opbytes = 3; break; // PC <- adr
      case 0xc4: printf("CNZ $%02x%02x", code[2], code[1]); opbytes = 3; break; // PC <- adr
      case 0xc5: printf("PUSH B"); break;
      case 0xc6: printf("ADI %02x", code[1]); opbytes = 2; break; // A <- A + byt
      case 0xc7: printf("RST 0"); break; // reset 
      case 0xc8: printf("RZ"); break; // if zero flag set, return 
      case 0xc9: printf("RET"); break; //sp +2 
      case 0xca: printf("JZ $%02x%02x", code[2], code[1]); opbytes = 3; break;
      case 0xcb: printf("Nothing"); break;
      case 0xcc: printf("CZ $%02x%02x",  code[2], code[1]); opbytes=3; break; // if Z, call adr
      case 0xcd: printf("CALL $%02x%02x",  code[2], code[1]); opbytes=3; break; // sp<-pc, sp+2, pc=adr
      case 0xce: printf("ACI %02x",  code[1]); opbytes=2; break; // A <- A + data+ CY
      case 0xcf: printf("RST 1"); break; //call $8
      
      case 0xd0: printf("RNC"); break; // if NCY, RET
      case 0xd1: printf("POP D"); break;
      case 0xd2: printf("JNC $%02x%02x", code[2], code[1]); opbytes=3; break; // if NCY, PC<-adr
      case 0xd3: printf("OUT %02x", code[1]); opbytes=2; break;
      case 0xd4: printf("CNC $%02x%02x", code[2], code[1]); opbytes=3; break; // if NCY, call adr
      case 0xd5: printf("PUSH D"); break;
      case 0xd6: printf("SUI %02x", code[1]); opbytes=2; break;
      case 0xd7: printf("RST 2"); break; //call $8
      case 0xd8: printf("RC"); break; // if CY; RET
      case 0xd9: printf("Nothing"); break;
      case 0xda: printf("JC $%02x%02x", code[2], code[1]); opbytes=3; break; // if CY, PC<-adr
      case 0xdb: printf("IN %02x", code[1]); opbytes=2; break;
      case 0xdc: printf("CC $%02x%02x", code[2], code[1]); opbytes=3; break; // if CY, call adr
      case 0xdd: printf("Nothing"); break;
      case 0xde: printf("SBI %02x", code[1]); opbytes=2; break;
      case 0xdf: printf("RST 3"); break; //call $8

      case 0xe0: printf("RPO"); break; // if NCY, RET
      case 0xe1: printf("POP H"); break;
      case 0xe2: printf("JPO $%02x%02x", code[2], code[1]); opbytes=3; break; // if PO, PC<-adr
      case 0xe3: printf("XTHL"); break; // swap HL with top of stack
      case 0xe4: printf("CPO $%02x%02x", code[2], code[1]); opbytes=3; break; // if PO, call adr
      case 0xe5: printf("PUSH H"); break;
      case 0xe6: printf("ANI %02x", code[1]); opbytes=2; break;
      case 0xe7: printf("RST 4"); break; //call $20
      case 0xe8: printf("RPE"); break; // if PE; RET
      case 0xe9: printf("PCHL"); break; // jump to address HL
      case 0xea: printf("JPE $%02x%02x", code[2], code[1]); opbytes=3; break; // if CY, PC<-adr
      case 0xeb: printf("XCHG"); break; // Swap DE and HL
      case 0xec: printf("CC $%02x%02x", code[2], code[1]); opbytes=3; break; // if CY, call adr
      case 0xed: printf("Nothing"); break;
      case 0xee: printf("SBI %02x", code[1]); opbytes=2; break;
      case 0xef: printf("RST 3"); break; //call $8

      case 0xf0: printf("RP"); break; // if NCY, RET
      case 0xf1: printf("POP PSW"); break;
      case 0xf2: printf("JPO $%02x%02x", code[2], code[1]); opbytes=3; break; // if PO, PC<-adr
      case 0xf3: printf("DI"); break; // swap HL with top of stack
      case 0xf4: printf("CP $%02x%02x", code[2], code[1]); opbytes=3; break; // if PO, call adr
      case 0xf5: printf("PUSH PSW"); break;
      case 0xf6: printf("ORI %02x", code[1]); opbytes=2; break;
      case 0xf7: printf("RST 6"); break; //call $30
      case 0xf8: printf("RM"); break; // if PE; RET
      case 0xf9: printf("SPHL"); break; // jump to address HL
      case 0xfa: printf("JM $%02x%02x", code[2], code[1]); opbytes=3; break; // if CY, PC<-adr
      case 0xfb: printf("EI"); break; // Swap DE and HL
      case 0xfc: printf("JM $%02x%02x", code[2], code[1]); opbytes=3; break; // if CY, call adr
      case 0xfd: printf("Nothing"); break;
      case 0xfe: printf("CPI %02x", code[1]); opbytes=2; break;
      case 0xff: printf("RST 7"); break; //call $38
      default:
        printf("unknown %c %c", GetFirstPart(code[1]),GetLastPart(code[1]));
        break;
    }
  }
  printf("\n");
  // switch statement here
}

 