#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "stack.h"

typedef struct PriorityQueue {

  uint32_t capacity;
  uint32_t size;

  Node **arr;
} PriorityQueue;

PriorityQueue *pq_create(uint32_t capacity) {

  PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  pq->capacity = capacity;
  pq->size = 0;

  pq->arr = (Node **)calloc(capacity, sizeof(Node *));

  return pq;
}

void pq_delete(PriorityQueue **q) {
  free((*q)->arr);
  free(*q);

  *q = NULL;
}

bool pq_empty(PriorityQueue *q) { return (q->size == 0); }

bool pq_full(PriorityQueue *q) { return (q->size == q->capacity); }

uint32_t pq_size(PriorityQueue *q) { return q->size; }

bool enqueue(PriorityQueue *q, Node *n) {

  uint32_t pointer = 0;

  if (q->size >= q->capacity) {
    return false;
  }

  while (true) {
    if ((pointer == q->size) || !node_cmp(n, q->arr[pointer])) {
      memcpy(&(q->arr[pointer + 1]), &(q->arr[pointer]),
             (q->size - pointer) * sizeof(Node *));
      q->arr[pointer] = n;
      q->size++;
      break;
    }
    pointer++;
  }

  return true;
}

bool dequeue(PriorityQueue *q, Node **n) {

  if (!(q->size > 0)) {
    return false;
  }

  *n = q->arr[0];
  //copy array down one index essentially removing the first node.
  memcpy(&(q->arr[0]), &(q->arr[1]), (q->size - 1) * sizeof(Node *));
  q->size--;
  return true;
}

void pq_print(PriorityQueue *q) {
  for (uint32_t i = 0; i < q->size; i++) {
    node_print(q->arr[i]);
  }
}
