#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  void (*function)(void *);
  void *data;
  void *next;
} queue_entry_t;

typedef struct {
  queue_entry_t *head;
  queue_entry_t *tail;
} queue_t;

int enqueue(queue_t *queue, void (*function)(), void *data) {
  // calloc is free on most modern systems
  queue_entry_t *new_entry = calloc(1, sizeof(queue_entry_t));
  if (new_entry == NULL)
    return 0;
  
  new_entry->function = function;
  new_entry->data = data;

  // Push
  if (queue->head == NULL) {
    queue->head = queue->tail = new_entry;
  }
  else {
    queue->tail->next = new_entry;
    queue->tail = new_entry;
  }

  return 1;
}

int run_one(queue_t *queue) {
  // Empty?
  if (queue->head != NULL) {
    queue_entry_t *last_entry = queue->head;

    // Pop
    if (queue->head == queue->tail) {
      queue->head = queue->tail = NULL;
    }
    else {
      queue->head = queue->head->next;
    }

    // Run
    last_entry->function(last_entry->data);
    free(last_entry);
  }

  // Return empty-p
  return queue->head != NULL;
}

void f(void *ptr) {
  printf("%s\n", (char *)ptr);
}

int main(int argc, char **argv) {
  queue_t queue = {0};
  
  enqueue(&queue, f, "A");
  enqueue(&queue, f, "B");
  enqueue(&queue, f, "C");

  // Imagine this is your inner event loop
  while (run_one(&queue))
    sleep(1);

  return 0;
};
