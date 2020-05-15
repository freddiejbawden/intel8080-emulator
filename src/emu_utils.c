#include <stdint.h>


int GetFirstPart(unsigned char code) {
  return (int)((code & 0xF0) >> 4);
}

int GetLastPart(unsigned char code) {
  return (int) (code & 0x0F);
}
char GetDest(unsigned char code) 
{
  int last = GetLastPart(code);
  last = last % 8;
  switch (last)
  {
    case 0: return 'B'; break;
    case 1: return 'C'; break;
    case 2: return 'D'; break;
    case 3: return 'E'; break;
    case 4: return 'H'; break;
    case 5: return 'L'; break;
    case 6: return 'M'; break;
    case 7: return 'A'; break;
  }
  return 0;
}

char GetSource(unsigned char code) 
{
  // split character into two
  int last = GetLastPart(code);
  int first = GetFirstPart(code);
  switch (first) {
    case 4: return (last < 8) ? 'B' : 'C'; break;
    case 5: return (last < 8) ? 'D' : 'E'; break;
    case 6: return (last < 8) ? 'H' : 'L'; break;
    case 7: return (last < 8) ? 'M' : 'A'; break;
  }
  return 0;
}
uint16_t MakeWord(uint8_t x, uint8_t y) 
{
  uint16_t word;
  word = (x << 8) | y;
  return word;
}
uint32_t Add16Bit(uint8_t *left_ptr, uint8_t *right_ptr, uint16_t to_add)
{
  uint8_t left, right;
  left = *left_ptr;
  right = *right_ptr;
  uint16_t cur_val = MakeWord(left, right);
  uint32_t result = cur_val + to_add;
  // split the result into two and store each part, mask out over flow
  *left_ptr = (result & 0xff00) >> 8;
  *right_ptr = (result & 0xff);
  return result;
}