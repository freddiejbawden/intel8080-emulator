#include <machine.h>
#include <i8080.h>
#include <stdio.h>

int shift_register_offset = 0;
int shift0 = 0;
int shift1 = 0;

void SetShiftRegister(int offset) {
  shift_register_offset = offset;
}
void printBin(uint8_t n) {
  while (n) {
    if (n & 1)
        printf("1");
    else
        printf("0");

    n >>= 1;
}
printf("\n");
}
int Shift() {
  uint16_t value = (shift1<<8) | shift0;    
  uint8_t a = ((value >> (8-shift_register_offset)) & 0xff);   
  return a;
}

void MachineOutPort(State8080 *state, uint8_t port, uint8_t value) {
  // printf("Machine Out %02x\n", port);
  switch(port)
  {
    case 2:
      shift_register_offset = value  &0x7;
      break;
    case 4:
      shift0 = shift1;
      shift1 = value;
      break;
  }
}

void MachineInPort(State8080 *state, uint8_t port) {
  // printf("Machine In %02x\n", port);
  uint8_t value = 0;

  if (port == 1) {
      value = 1 << 3;
  }
  else if (port == 2) {
      value = 0;
  }
  else if (port == 3) {
     value = Shift();
  }
  else {
      fprintf(stderr, "error: unknown IN port %02X\n", port);
  }
  state->a = value;
}

int count = 0;
int NextInstruction(State8080 *state) {
  uint8_t *opcode = &state->memory[state->pc];

    if (*opcode == 0xdb) //machine specific handling for IN    
    {    
        uint8_t port = opcode[1];    
        MachineInPort(state, port);    
        state->pc+=2;    
    }    
    else if (*opcode == 0xd3)  //OUT    
    {   

        uint8_t port = opcode[1];  
        MachineOutPort(state, port, state->a);    
        state->pc+=2;    
    }    
    else  {
      int debug = 0;
      return Emulate8080Op(state, debug);
        // printf("Current A: %02x\n", state->a);  
    }
    return 1;  
  }    

