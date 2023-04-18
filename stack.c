#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "stack.h"

typedef struct Stack {

  uint32_t top;
  uint32_t capacity;
  Node **items;

} Stack;

Stack *stack_create(uint32_t capacity) {

  Stack *s = (Stack *)malloc(sizeof(Stack));

  s->items = (Node **)calloc(capacity, sizeof(Node *));
  s->top = 0;
  s->capacity = capacity;

  return s;
}

void stack_delete(Stack **s) {

  free((*s)->items);
  free(*s);
  *s = NULL;
}

bool stack_empty(Stack *s) {

  if (s->top == 0) {
    return true;
  }
  return false;
}

bool stack_full(Stack *s) {
  if (s->top == s->capacity) {
    return true;
  }
  return false;
}

uint32_t stack_size(Stack *s) { return s->top; }

bool stack_push(Stack *s, Node *n) {
  if (s->top < s->capacity) {
    s->items[s->top] = n;
    s->top++;
    return true;
  }
  return false;
}

bool stack_pop(Stack *s, Node **n) {

  if (s->top > 0) {

    *n = s->items[s->top - 1];

    s->top--;
    return true;
  }
  return false;
}

void stack_print(Stack *s) {

  for (uint32_t i = 0; i < s->top; i++) {

    node_print(s->items[i]);
  }
}
