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
                  "  A Huffman decoder.\n"
                  "  Decompresses a file using the Huffman coding algorithm.\n"
                  "\n"
                  "USAGE\n"
                  "  ./decode [-h] [-i infile] [-o outfile]\n"
                  "\n"
                  "OPTIONS\n"
                  "  -h             Program usage and help.\n"
                  "  -v             Print compression statistics.\n"
                  "  -i infile      Input file to decompress.\n"
                  "  -o outfile     Output of decompressed data.\n");
}

//Checks if the node is a leaf
static bool is_leaf(Node *n) {
  if (n->left == NULL && n->right == NULL) {
    return true;
  }
  return false;
}

int main(int argc, char **argv) {

  int value;
  char *in_file = "stdin";
  char *out_file = "stdout";
  bool statistics = false;
  
  //Reading input 
  while ((value = getopt(argc, argv, "hvi:o:")) != -1) {

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

  int input;
  int output;
  //setting up and preparing files
  if (strcmp(out_file, "stdout") == 0) {
    output = 1;
  } else {
    output = open(out_file, O_WRONLY | O_TRUNC| O_CREAT);
  }
  

  if (strcmp(in_file, "stdin") == 0) {
    input = 0;
  } else {
    input = open(in_file, O_RDONLY);
  }

  if(input == -1){
  	fprintf(stderr, "Input file does not exist\n");
  	return 1;
  }

  Header h;
  //reading header from file
  read_bytes(input, (uint8_t *)&h, sizeof(Header));

  uint16_t nbytes = h.tree_size;
  uint8_t tree[nbytes];
  
  //reading tree from file and rebuilding it
  read_bytes(input, tree, nbytes);
  Node *root = rebuild_tree(nbytes, tree);

  if (h.magic != MAGIC) {
    fprintf(stderr,
            "Invalid magic number.\n");
    return 1;
  }
  
  //setting file permissions
  chmod(out_file, h.permissions);
  
  Node *current = root;

  Code table[ALPHABET] = {code_init()};

  build_codes(root, table);

  uint8_t bit = 0;
  uint64_t num_printed = 0;
  //reading bits until the file reaches the correct size
  while (num_printed < h.file_size) {
    read_bit(input, &bit);

    if (bit == 0) {
      current = current->left;
    }
    if (bit == 1) {
      current = current->right;
    }

    if (is_leaf(current)) {
      write_bytes(output, &current->symbol, 1);

      num_printed++;
      current = root;
    }
  }
  
  //print statistics if its enabled
  if (statistics) {

    double saving = 100 * (1 - ((float)bytes_read / ((float)bytes_written)));

    fprintf(stderr,
            "Compressed file size: %lu bytes\n"
            "Decompressed file size: %lu bytes\n"
            "Space saving: %.2f%%\n",
            bytes_read, bytes_written, saving);
  }

  close(input);
  close(output);
  delete_tree(&root);
  return 0;
}
