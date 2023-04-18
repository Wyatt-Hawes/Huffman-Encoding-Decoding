#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "node.h"

Node *node_create(uint8_t symbol, uint64_t frequency) {

  Node *n = (Node *)malloc(sizeof(Node));

  n->left = NULL;
  n->right = NULL;

  n->symbol = symbol;
  n->frequency = frequency;

  return n;
}

void node_delete(Node **n) {

  free(*n);
  *n = NULL;
}

Node *node_join(Node *left, Node *right) {

  Node *parent = node_create('$', left->frequency + right->frequency);
  parent->left = left;
  parent->right = right;

  return parent;
}

void node_print(Node *n) {

  // print node and its children if they exist
  if (n != NULL) {
    printf("Node: (%c:%lu)\n", n->symbol, n->frequency);
  }

  if (n->left != NULL) {
    printf("Left: %c:%lu  ", n->left->symbol, n->left->frequency);
  }

  if (n->right != NULL) {
    printf("Right: %c:%lu", n->right->symbol, n->right->frequency);
  }
  printf("\n----------\n");
  return;
}
//Compare n > m based on frequency
bool node_cmp(Node *n, Node *m) {
  if (n->frequency > m->frequency) {
    return true;
  }
  return false;
}

void node_print_sym(Node *n) { printf("%c", n->symbol); }
