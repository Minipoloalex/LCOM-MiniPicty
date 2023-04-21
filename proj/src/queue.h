#include <lcom/lcf.h>
#include <stdint.h>

struct queue;
typedef struct queue queue_t;

/**
 * @brief Create a queue object
 * Constructor for the queue object
 * @param size 
 * @return queue_t* 
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
 */
int (push_queue)(queue_t *q, void *element);

/**
 * @brief Pops an element from the queue
 * 
 * @param q
 * @return void* 
 */
int (pop_queue)(queue_t *q, void *element);

/**
 * @brief Returns true if the queue is full
 * 
 */
int (is_full_queue)(queue_t *q);

/**
 * @brief Returns true if the queue is empty
 * 
 */
int (is_empty_queue)(queue_t *q);
