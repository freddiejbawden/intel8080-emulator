// Intel 8080 (KR580VM80A) microprocessor core model
//
// Copyright (C) 2012 Alexander Demin <alexander@demin.ws>
//
// Credits
//
// Viacheslav Slavinsky, Vector-06C FPGA Replica
// http://code.google.com/p/vector06cc/
//
// Dmitry Tselikov, Bashrikia-2M and Radio-86RK on Altera DE1
// http://bashkiria-2m.narod.ru/fpga.html
//
// Ian Bartholomew, 8080/8085 CPU Exerciser
// http://www.idb.me.uk/sunhillow/8080.html
//
// Frank Cringle, The origianal exerciser for the Z80.
//
// Thanks to zx.pk.ru and nedopc.org/forum communities.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <i8080.h>
#include <emu_utils.h>
#include <emu.h>

void load_file(const char* name, unsigned char* load_to) {
    FILE* f = fopen(name, "r+b");
    int sz;
    if (!f) {
        fprintf(stderr, "Unable to open file \"%s\"\n", name);
        exit(1);
    }
    sz = 0;
    while (!feof(f)) {
        int const read = fread((void *)load_to, 1, 1024, f);
        if (read == 0) break;
        sz += read;
        load_to += read;
    }
    printf("\n*********************************\n");
}

void execute_test(char* filename, int success_check) {

    int success = 0;
    State8080 state = SetUpEmu();
    LoadROM(&state, filename, 0x0100);
    state.pc = 0x100;
    int i_count = 0;
    state.memory[0x0005] = 0xC9;  // Inject RET at 0x0005 to handle "CALL 5".
    int j = 0;
    while (1) {
        j++;
        int const pc = state.pc;
        if (state.memory[pc] == 0x76) {
            printf("HLT at %04X\n", pc);
            exit(1);
        }
        if (pc == 0x0005) {
            if (state.c == 9) {
                int i;
                  
                for (i = MakeWord(state.d, state.e); state.memory[i] != '$'; i += 1)
                    putchar(state.memory[i]);
                success = 1;
            }
            
            if (state.c == 2) {
              putchar((char)state.e);
            }
        }
       
        /* if (j >= 10) {
          i_count += 10;
          printf("I count: %d\n",i_count);
          print_state(&state);
          getchar();
          j=0;
          
          for (int k = 0; k < 0x100; k++) {
            printf("%02x ", state.memory[k]);
          }
          printf("\n");
        } */
        
         Emulate8080Op(&state, 0);
  
        if (state.pc == 0) {
            printf("\nJump to 0000 from %04X\n", pc);
            if (success_check && !success) {
                printf("success");
                exit(1);
            }
              print_state(&state);

            return;
        }
    }
}

int main() {
  execute_test("./tests/CPUTEST.COM", 0);
  execute_test("./tests/TEST.COM", 0);
  execute_test("./tests/8080PRE.COM", 0);
  execute_test("./tests/8080EX1.COM", 0);

    return 0;
}
