#include "code.h"
#include "defines.h"

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

uint64_t bytes_read;
uint64_t bytes_written;

int read_bytes(int infile, uint8_t *buf, int nbytes) {

  int position = 0;
  int bytes_r = 0;

  do {

    bytes_r = read(infile, &buf[position], nbytes);

    position += bytes_r;
    nbytes = nbytes - bytes_r;
    bytes_read += bytes_r;

  } while (nbytes > 0 && bytes_r > 0);
  return position;
}

int write_bytes(int outfile, uint8_t *buf, int nbytes) {

  int position = 0;
  int bytes_w = 0;

  //Write nbytes number of bytes to outfile
  do {
    bytes_w = write(outfile, &buf[position], nbytes);

    nbytes = nbytes - bytes_w;
    position += bytes_w;
    bytes_written += bytes_w;

  } while (nbytes > 0 && bytes_w > 0);

  return position;
}

bool read_bit(int infile, uint8_t *bit) {

  static uint8_t buffer[BLOCK] = {0};
  static int position = 0;
  static int limit = 0;
  static bool f = true;
 //read the buffer only if we have used up all the bits.
  if (position == (limit * 8) || f) {
    int t = read_bytes(infile, buffer, BLOCK);
    if (t == 0) {
      return false;
    }
    limit = t;
    f = false;
    position = 0;
  }
  
  //extracting bit at the current position.
  uint8_t pos = buffer[position/8];
  pos = pos << (7 - (position % 8));
  pos = pos >> 7;

  *bit = pos;
  position++;
  return true;
}

static uint8_t buff[BLOCK] = {0};
static int p = 0;

static void print_buffer(int outfile) {
  write_bytes(outfile, buff, p / 8);
  p = 0;
}

void write_code(int outfile, Code *c) {

  
  for (uint32_t i = 0; i < code_size(c); i++) {

    uint8_t b;
    if (code_get_bit(c, i)) {
      b = 1;
    } else {
      b = 0;
    }

    int section = p / 8;
    int index = p % 8;
    
    //Add bit to the buffer, when the buffer is full, print all the bits
    if (b == 1) {
      uint8_t key = 1 << index;

      buff[section] = buff[section] | key;
    } else {

      uint8_t key = ~(1 << index);
      buff[section] = buff[section] & key;
    }
    p++;
    if (p >= BLOCK * 8) {
      print_buffer(outfile);
    }
  }

}

void flush_codes(int outfile) {
 
  //print out remaining bits in the buffer after aligning it with a byte block.
  while ((p % 8) != 0) {
    int section = p / 8;
    int index = p % 8;

    uint8_t key = ~(1 << index);
    buff[section] = buff[section] & key;
    p++;
  }
  print_buffer(outfile);
}
