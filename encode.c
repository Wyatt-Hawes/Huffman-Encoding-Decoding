#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "code.h"
#include "defines.h"
#include "header.h"
#include "huffman.h"
#include "io.h"
#include "node.h"
#include "pq.h"
#include "stack.h"

static void print_usage() {

  fprintf(stderr, "SYNOPSIS\n"
                  "  A Huffman encoder.\n"
                  "  Compresses a file using the Huffman coding algorithm.\n"
                  "\n"
                  "USAGE\n"
                  "  ./encode [-h] [-i infile] [-o outfile]\n"
                  "\n"
                  "OPTIONS\n"
                  "  -h             Program usage and help.\n"
                  "  -v             Print compression statistics.\n"
                  "  -i infile      Input file to compress.\n"
                  "  -o outfile     Output of compressed data.\n");
}

int main(int argc, char **argv) {

  int value;
  char *in_file = "stdin";
  char *out_file = "stdout";
  bool statistics = false;
  //Reading input values
  while ((value = getopt(argc, argv, "i:o:hv")) != -1) {

    switch (value) {

    case 'v':
      statistics = true;
      break;

    case 'h':
      // Print usage and program synopsis
      print_usage();
      return 0;
      break;

    case 'i':
      in_file = (optarg);
      break;

    case 'o':
      out_file = optarg;
      break;

    default:
      // printf("invalid input");
      print_usage();
      return 1;
      break;
    }
  }

  uint64_t hist[ALPHABET] = {0};
  uint8_t character = 0;
  int unique = 0;
  int input;
  int output;
  
  //Opening and preparing files
  
  if (strcmp(out_file, "stdout") == 0) {
    output = 1;
  } else {
    output = open(out_file, O_WRONLY | O_TRUNC | O_CREAT);
    printf("%d\n", output);
  }

  FILE *temp_file;
  temp_file = fopen("temp", "w+");
  if (strcmp(in_file, "stdin") == 0) {

    int c = fgetc(stdin);
    while (c != EOF) {

      fputc(c, temp_file);

      if (hist[c] == 0) {
        unique++;
      }
      hist[c] += 1;

      c = fgetc(stdin);
    }
    fclose(temp_file);
    input = open("temp", O_RDONLY);

  } else {
    FILE *inn = fopen(in_file, "r");
    
    if(inn == NULL){
    	fprintf(stderr, "Input file does not exist\n");
    	return 1;
    }
    
    int c = fgetc(inn);
    while (c != EOF) {

      if (hist[c] == 0) {
        unique++;
      }
      hist[c] += 1;

      c = fgetc(inn);
    }
    fclose(inn);
    input = open(in_file, O_RDONLY);
  }
  if(input == -1){
  	fprintf(stderr, "Input file does not exist\n");
  	return 1;
  }

  //Setting hist[0] and hist[1] if they are not already incremented
  if (hist[0] == 0) {
    hist[0] = 1;
    unique++;
  }
  if (hist[1] == 0) {
    hist[1] = 1;
    unique++;
  }

  // step 3: building huffman tree
  Node *root = build_tree(hist);

  // step 4: building code table
  Code table[ALPHABET] = {code_init()};

  build_codes(root, table);


  Header h;
  struct stat s;

  struct stat in;
  stat("/dev/stdin", &in);

  if (strcmp(in_file, "stdin") != 0) {
    stat(in_file, &s);
  } else {
    stat("temp", &s);
  }


  h.magic = MAGIC;

  if (strcmp(in_file, "stdin") != 0) {
    h.permissions = s.st_mode;
  } else {
    h.permissions = in.st_mode;
  }
  h.permissions = s.st_mode;

  h.tree_size = (3 * unique) - 1;
  h.file_size = s.st_size;

  // step 6: Writing header to file
  write_bytes(output, (uint8_t *)&h, sizeof(Header));

  // step 7: Dumping tree to file
  dump_tree(output, root);

  //Closing and re-opening file to reset file pointer
  close(input);
  if (strcmp(in_file, "stdin") == 0) {
    input = open("temp", O_RDONLY);
  } else {

    input = open(in_file, O_RDONLY);
  }

  while (read_bytes(input, &character, 1) > 0) {

    write_code(output, &table[character]);
  }

  flush_codes(output);
 
 
  //Print statistics if they're enabled
  if (statistics) {

    double saving = 100 * (1 - ((float)bytes_written / ((float)bytes_read)));

    fprintf(stderr,
            "Uncompressed file size: %lu bytes\n"
            "Compressed file size: %lu bytes\n"
            "Space saving: %.2f%%\n",
            bytes_read, bytes_written, saving);
  }

  	close(input);
  
  if (strcmp(out_file, "stdout") != 0) {
    close(output);
  }
  remove("temp");

  delete_tree(&root);
  return 0;
}
