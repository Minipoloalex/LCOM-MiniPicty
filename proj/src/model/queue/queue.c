#include "queue.h"

struct queue {
  unsigned int count; // count of elements being used in the queue
  unsigned int size; // maximum size of the queue

  unsigned int front; // index of the front element
  unsigned int back; // index of the back element

  int element_size;   // size of each element for pointer arithmetic
  void **elements; // array of elements

};


queue_t *(create_queue)(unsigned int size, int element_size) {
    queue_t *queue = (queue_t *) malloc(sizeof(queue_t));
    if (queue == NULL) {
      return NULL;
    }
    queue->size = size == 0 ? 1 : size;
    queue->element_size = element_size;

    queue->elements = (void *) malloc(queue->size * element_size);
    if (queue->elements == NULL) {
        free(queue);
        return NULL;
    }
    queue->back = queue->front = queue->count = 0;
    return queue;
}

void (delete_queue)(queue_t *queue) {
  free(queue->elements);
  free(queue);
}

int (is_full_queue)(queue_t *q) {
  return q->count == q->size;
}
int (is_empty_queue)(queue_t *q) {
  return q->count == 0;
}

int (push_queue)(queue_t *q, void *element) {
  if (is_full_queue(q)) {
    return EXIT_FAILURE;
  }
  memcpy(q->elements + q->back * q->element_size, element, q->element_size);
  q->back = (q->back + 1) % q->size;
  q->count++;
  return EXIT_SUCCESS;
}

int (pop_queue)(queue_t *q, void *element) {
  if (is_empty_queue(q)) {
    return EXIT_FAILURE;
  }
  memcpy(element, q->elements + q->front * q->element_size, q->element_size);
  q->front = (q->front + 1) % q->size;
  q->count--;
  return EXIT_SUCCESS;
}

int (queue_get_back)(queue_t *q, void *back) {
  if (is_empty_queue(q)) {
    return EXIT_FAILURE;
  }
  memcpy(back, q->elements + ((q->back - 1) % q->size) * q->element_size, q->element_size);
  return EXIT_SUCCESS;
}
