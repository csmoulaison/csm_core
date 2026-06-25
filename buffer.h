#ifndef buffer_h_INCLUDED
#define buffer_h_INCLUDED

// TODO: Debug build code for keeping a lookaside table of buffers and
// suballocations to visualize the state of the program.

#ifndef BUFFER_BOUNDS_CHECKING
#define BUFFER_BOUNDS_CHECKING true
#endif

#ifndef BUFFER_LABELS
#define BUFFER_LABELS false
#define BUFFER_LABEL_MAX_LENGTH 16
#endif

typedef struct {
    u8* memory;
    u64 size;
#if BUFFER_LABELS
    char label[BUFFER_LABEL_MAX_LENGTH];
#endif
} Buffer;

Buffer alloc_buffer(u64 size, char* debug_label);
Buffer suballoc_buffer_at_byte(Buffer* buffer, u64 byte, u64 size, char* debug_label);
Buffer suballoc_buffer_at_pointer(Buffer* buffer, void* pointer, u64 size, char* debug_label);

#ifdef CSM_IMPLEMENTATION

Buffer _create_buffer(u64 size, void* memory, char* debug_label) {
    Buffer buffer;
    buffer.memory = (u8*)memory;
    buffer.size = size;
#if BUFFER_LABELS
    strncpy(buffer.label, debug_label, BUFFER_LABEL_MAX_LENGTH);
#endif
    return buffer;
}

Buffer alloc_buffer(u64 size, char* debug_label) {
    void* memory = malloc(size);
    return _create_buffer(size, memory, debug_label);
}

Buffer suballoc_buffer(Buffer* buffer, u64 byte_index, u64 size, char* debug_label) {
#if BUFFER_BOUNDS_CHECKING
    if(byte_index + size > buffer->size) {
        log_fail("suballoc_buffer: overrun!");
    }
#endif
    u8* memory = &buffer->memory[byte_index];
    return _create_buffer(size, memory, debug_label);
}

#endif // CSM_IMPLEMENTATION
#endif // buffer_h_INCLUDED
