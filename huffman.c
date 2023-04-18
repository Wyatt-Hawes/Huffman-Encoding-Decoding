#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "code.h"
#include "defines.h"
#include "io.h"
#include "node.h"
#include "pq.h"
#include "stack.h"

Node *build_tree(uint64_t hist[static ALPHABET]) {

  PriorityQueue *pq = pq_create(ALPHABET);

  for (int i = 0; i < ALPHABET; i++) {

    if (hist[i] > 0) {
      enqueue(pq, node_create(i, hist[i]));
    }
  }

  // pq_print(pq);

  Node *L = NULL;
  Node *R = NULL;
  
  //Dequeue two node and enqueue the joined node of each
  while (pq_size(pq) > 1) {
    dequeue(pq, &L);
    dequeue(pq, &R);

    enqueue(pq, node_join(L, R));
  }

  Node *root = (Node *)NULL;

  //Last node is the root
  dequeue(pq, &root);
  
  pq_delete(&pq);

  return root;
}

static bool is_leaf(Node *n) {

  if ((n->left == NULL) && (n->right == NULL)) {

    return true;
  }
  return false;
}

static Code c;
static bool flag = true;
void build_codes(Node *root, Code table[static ALPHABET]) {
  
  //Only initialize code once
  if (flag) {
    c = code_init();
    flag = false;
  }


  if (is_leaf(root)) {
    table[root->symbol] = c;
    return;
  }

  uint8_t void_bit;

  code_push_bit(&c, 0);
  build_codes(root->left, table);
  code_pop_bit(&c, &void_bit);

  code_push_bit(&c, 1);
  build_codes(root->right, table);
  code_pop_bit(&c, &void_bit);
  return;
}

void dump_tree(int outfile, Node *root) {

  uint8_t bytes[2] = {0};
  
  //If node is a leaf, print L and the symbol
  if (is_leaf(root)) {
    bytes[0] = 'L';
    bytes[1] = root->symbol;
    write_bytes(outfile, bytes, 2);
    return;
  }

  dump_tree(outfile, root->left);
  dump_tree(outfile, root->right);

  bytes[0] = 'I';
  write_bytes(outfile, bytes, 1);
  return;
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
  Stack *s = stack_create(nbytes);

  int i = 0;
  while (i < nbytes) {
    
    //push nodes onto the stack
    if (tree[i] == 'L') {
      stack_push(s, node_create(tree[i + 1], 0));
      i += 2;

    } else if (tree[i] == 'I') {
      Node *left;
      Node *right;

      stack_pop(s, &right);
      stack_pop(s, &left);

      stack_push(s, node_join(left, right));
      i++;
    } else {
      //fprintf(stderr,
      //        " This should not happen\n Bad character in huffman.c (rebuild_tree:%c)\n",
      //        tree[i]);
      return NULL;
    }
  }

  Node *root;
  stack_pop(s, &root);
  stack_delete(&s);

  return root;
}

void delete_tree(Node **root) {

  if (*root == NULL) {
    return;
  }

  if ((*root)->left != NULL) {

    delete_tree(&((*root)->left));
  }
  if ((*root)->right != NULL) {

    delete_tree(&((*root)->right));
  }
  node_delete(root);
}
