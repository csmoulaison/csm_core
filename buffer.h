#ifndef buffer_h_INCLUDED
#define buffer_h_INCLUDED

// TODO: Debug build code for keeping a lookaside table of buffers and
// suballocations to visualize the state of the program.

#ifndef BUFFER_BOUNDS_CHECKING
#define BUFFER_BOUNDS_CHECKING true
#endif

#ifndef BUFFER_LABELS
#define BUFFER_LABELS false
#define BUFFER_LABEL_MAX_LENGTH 64
#endif

typedef struct {
    u8* memory;
    u64 size;
#if BUFFER_LABELS
    char label[BUFFER_LABEL_MAX_LENGTH];
#endif
} Buffer;

Buffer buffer_alloc(u64 size, char* debug_label);
Buffer buffer_suballoc(Buffer* buffer, u64 byte_index, u64 size, char* debug_label);
void buffer_free(Buffer* buffer);

#ifdef CSM_IMPLEMENTATION

Buffer buffer_create(void* memory, u64 size, char* debug_label) {
    Buffer buffer;
    buffer.memory = (u8*)memory;
    buffer.size = size;
#if BUFFER_LABELS
    strncpy(buffer.label, debug_label, BUFFER_LABEL_MAX_LENGTH);
#endif
    return buffer;
}

Buffer buffer_malloc(u64 size, char* debug_label) {
    void* memory = malloc(size);
    return buffer_create(memory, memory, debug_label);
}

Buffer buffer_suballoc(Buffer* buffer, u64 byte_index, u64 size, char* debug_label) {
#if BUFFER_BOUNDS_CHECKING
    if(byte_index + size > buffer->size) {
        log_fail("suballoc_buffer: overrun!");
    }
#endif
    u8* memory = &buffer->memory[byte_index];
    return _buffer_create(size, memory, debug_label);
}

void buffer_free(Buffer* buffer) {
    free(buffer->memory);
}

#endif // CSM_IMPLEMENTATION
#endif // buffer_h_INCLUDED
