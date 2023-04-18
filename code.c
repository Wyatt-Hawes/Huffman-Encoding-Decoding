#include "defines.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "code.h"

Code code_init(void) {

  Code c;

  c.top = 0;
  //setting entire code to be all 0
  memset(&c.bits, 0, sizeof(c.bits));

  return c;
}

uint32_t code_size(Code *c) { return c->top; }

bool code_empty(Code *c) { return (c->top == 0); }

bool code_full(Code *c) { return (c->top == ALPHABET); }

bool code_set_bit(Code *c, uint32_t i) {

  if (i > ALPHABET) {
    return false;
  }

  uint8_t key = 1;
  int index = i / 8;
  int position = i % 8;
  key = key << position;

  //Or'ing bits to set bit
  c->bits[index] = c->bits[index] | key;
  return true;
}

bool code_clr_bit(Code *c, uint32_t i) {

  if (i > ALPHABET) {
    return false;
  }

  uint8_t key = 1;
  int index = i / 8;
  int position = i % 8;
  key = ~(key << position);
  
  //AND'ing bits together to clear bit
  c->bits[index] = c->bits[index] & key;
  return true;
}

bool code_get_bit(Code *c, uint32_t i) {

  if (i > ALPHABET) {
    return false;
  }

  int index = ((int)i / (int)8);
  int bit = (i % 8);

  uint8_t result = c->bits[index] << (7 - bit);
  result = result >> 7;
  return (result == 1);
}

bool code_push_bit(Code *c, uint8_t bit) {
  if (c->top >= ALPHABET || bit > 1) {
    return false;
  }

  if (bit != 0) {
    code_set_bit(c, c->top);
  }
  c->top++;
  return true;
}

bool code_pop_bit(Code *c, uint8_t *bit) {
  if (c == NULL || c->top == 0) {
    return false;
  }
  c->top--;
  if (code_get_bit(c, c->top)) {
    code_clr_bit(c, c->top);
    *bit = 1;
  } else {
    //no need to clear bit if its already 0
    *bit = 0;
  }
  return true;
}

void code_print(Code *c) {

  for (uint32_t i = 0; i < c->top; i++) {

    if (code_get_bit(c, i)) {
      printf("1");
    } else {
      printf("0");
    }
    if ((i + 1) % 32 == 0) {
      printf("\n");
    }
  }
  printf("\n");
}
