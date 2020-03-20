#include <f3d/engine/type/buffer.h>
#include <f3d/engine/log.h>

#include <stdlib.h>
#include <string.h>

unsigned long long buffer_total_used = 0;

int buffer_init(buffer_t *buffer, unsigned obj_sz, unsigned start_size) {
    buffer->index = 0;
    buffer->obj_sz = obj_sz;
    buffer->size = start_size;
    
    unsigned long bytes_sz = (unsigned long)obj_sz*(unsigned long)start_size;
    buffer->data = malloc(bytes_sz);
    
    if (buffer->data == NULL) {
        log_msg(
                LOG_ERROR, 
                "Error allocating buffer of size %u*%u(%lu)\n", 
                obj_sz, start_size, bytes_sz
        );
        return 1;
    }
    buffer_total_used += bytes_sz;
    return 0;
}

void buffer_push(buffer_t *buffer, void *obj) {
    if (buffer->index >= buffer->size) {
        buffer_resize(buffer);
    }
    unsigned long index = (buffer->index)*(buffer->obj_sz);
    memcpy(((unsigned char *)buffer->data)+(index), obj, buffer->obj_sz);
    buffer->index++;
}

void buffer_resize(buffer_t *buffer) {
    if (buffer->data == NULL) {
        log_msg(LOG_ERROR, "buffer->data == NULL\n", 0);
        return;
    }
    if (buffer->index > buffer->size)
        buffer->size = buffer->index;

    log_msg(LOG_INFO, "Resizing buffer from %luKB to %luKB\n", buffer->size/1024, buffer->size*2/1024);
    buffer->size *= 2;
    void *newb;
    newb = realloc(buffer->data, buffer->size*buffer->obj_sz);
    buffer->data = newb;
    if (buffer->data == NULL) {
        log_msg(LOG_ERROR, "Error resizing buffer\n", 0);
    }
    buffer_total_used += buffer->size;
}

void buffer_destroy(buffer_t *buffer) {
    buffer_total_used -= buffer->size*buffer->obj_sz;
    free(buffer->data);
    buffer->index = 0;
    buffer->size = 0;
    buffer->data = NULL;
}
