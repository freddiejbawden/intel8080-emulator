#include <machine.h>
#include <i8080.h>
#include <stdio.h>

int shift_register_offset;
int shift0;
int shift1;

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

uint8_t MachineInPort(State8080 *state, uint8_t port) {
  // printf("Machine In %02x\n", port);
  if (port == 0) {
    return 0x01;
  }
  if (port == 1) {
    return 0x01;
  }
  if (port == 2) {
    return 0x00;
  } 
  if (port == 3) {
    int shifted_value = Shift();
    state->a = shifted_value;
  }
  return 0;
}
int count = 0;
void NextInstruction(State8080 *state) {
  uint8_t *opcode = &state->memory[state->pc];

    if (*opcode == 0xdb) //machine specific handling for IN    
    {    
        uint8_t port = opcode[1];    
        state->a = MachineInPort(state, port);    
        state->pc+=2;    
    }    
    else if (*opcode == 0xd3)  //OUT    
    {   

        uint8_t port = opcode[1];  
        MachineOutPort(state, port, state->a);    
        state->pc+=2;    
    }    
    else  {
      int debug = 1;
        Emulate8080Op(state, debug);
  
        // printf("Current A: %02x\n", state->a);  
    }  
  }    

