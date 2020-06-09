#ifndef AR_M_BUFFER_H
#define AR_M_BUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define AR_BUFFER_RESIZE_AUTO -1

// buffer flags
#define AR_BUFFER_INITIALIZED 0x01
#define AR_BUFFER_UNTRACKED   0x02

typedef enum {
    AR_BUFFER_NONE,
    AR_BUFFER_STATIC,
    AR_BUFFER_DYNAMIC,
} ar_buffer_type_t;

typedef struct ar_buffer_s {
    uint16_t flags;
    
    ar_buffer_type_t type;

    void *data;
    // object size in bytes
    uint32_t obj_sz;
    uint32_t index;
    // allocated size in objects
    uint32_t size;
    
    size_t (*resize_func)(struct ar_buffer_s *);
} ar_buffer_t;

int  ar_buffer_init(ar_buffer_t *buffer, ar_buffer_type_t type, uint32_t obj_sz, uint32_t start_size, uint16_t flags);
bool ar_buffer_is_initialized(ar_buffer_t *buffer);
void *ar_buffer_push(ar_buffer_t *buffer, void *obj);
void *ar_buffer_new_item(ar_buffer_t *buffer);
void *ar_buffer_get(ar_buffer_t *buffer, unsigned index);
ar_buffer_t ar_buffer_duplicate(ar_buffer_t *buffer, ar_buffer_type_t type);
ar_buffer_t ar_buffer_from_data(ar_buffer_type_t type, void *data, uint32_t obj_sz, uint32_t data_size, uint16_t flags);
void ar_buffer_copy_data(ar_buffer_t *buffer, void *data, int data_size);
void ar_buffer_resize(ar_buffer_t *buffer, int size);
void ar_buffer_reduce_to_data(ar_buffer_t *buffer);
void ar_buffer_destroy(ar_buffer_t *buffer);

// resize functions
size_t ar_buffer_resize_func_double(ar_buffer_t *buffer);


#endif