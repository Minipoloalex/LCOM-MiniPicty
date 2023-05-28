#pragma once
#include <lcom/lcf.h>
#include <stdint.h>

/** @defgroup queue Queue
 * @{
 *
 * @brief Module responsible for the queue
 */

struct queue;
typedef struct queue queue_t;

#define QUEUE_SIZE 100 /**< @brief maximum size of the queue */

/**
 * @brief Create a queue object\n
 *
 * Constructor for the queue object.
 * Creates a queue object with the given size and element size, returns NULL if it fails.
 * @param size 
 * @param element_size
 * @return queue_t* pointer to the queue object, null if fails
 */
queue_t *(create_queue)(unsigned int size, int element_size);

/**
 * @brief Deletes a queue object
 * Destructor for the queue object
 * @param q 
 */
void (delete_queue)(queue_t *q);

/**
 * @brief Pushes an element to the queue
 * 
 * @param q 
 * @param element 
 * @return int 0 if success, different than 0 if the queue is full
 */
int (push_queue)(queue_t *q, void *element);

/**
 * @brief Pops an element from the queue
 * 
 * @param q
 * @return int 0 if success, different than 0 if the queue is empty
 */
int (pop_queue)(queue_t *q, void *element);
/**
 * @brief Returns by parameter the last inserted element in the queue
 * @return int 0 if success, different than 0 if the queue is empty
 */
int (queue_get_back)(queue_t *q, void *back);

/**
 * @brief Returns true if the queue is full
 * @return int true if the queue is full, false otherwise
 */
int (is_full_queue)(queue_t *q);

/**
 * @brief Returns true if the queue is empty
 * @return int true if the queue is empty, false otherwise
 */
int (is_empty_queue)(queue_t *q);
