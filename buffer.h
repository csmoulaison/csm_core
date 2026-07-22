#ifndef buffer_h_INCLUDED
#define buffer_h_INCLUDED

// TODO: Debug build code for tracking suballocations.

#ifndef BUFFER_BOUNDS_CHECKING
#define BUFFER_BOUNDS_CHECKING true
#endif

#ifndef BUFFER_TRACKING
#define BUFFER_TRACKING false
#define BUFFER_LABEL_MAX_LENGTH 64
#endif

#ifndef BUFFER_MAX_TRACKED_SUBALLOCATIONS
#define BUFFER_MAX_TRACKED_SUBALLOCATIONS 64
#endif

typedef BufferType u8
#define BUFFER_TYPE_RAW    1 << 0
#define BUFFER_TYPE_BUFFER 1 << 1
#define BUFFER_TYPE_STACK  1 << 2
#define BUFFER_TYPE_SUB    1 << 3

typedef struct {
    u8* memory;
    u64 size;
    
#if BUFFER_TRACKING
    String label;
    BufferType type;
    struct Buffer* children[BUFFER_MAX_TRACKED_SUBALLOCATIONS];
    u32 children_len;
#endif
} Buffer;

Buffer buffer_alloc(Buffer* buffer, u64 at_byte, u64 size, String label);
Buffer buffer_alloc_typed(Buffer* buffer, u64 at_byte, u64 size, String label, BufferType type);
Buffer buffer_from_memory(void* memory, u64 size, String label);
Buffer buffer_from_memory_typed(void* memory, u64 size, String label, BufferType type);
Buffer buffer_malloc(u64 size, String label);
// This is only useful for when BUFFER_TRACKING is active.
void buffer_clear_suballocations(Buffer* buffer);

#ifdef CSM_IMPLEMENTATION

Buffer buffer_alloc(Buffer* buffer, u64 at_byte, u64 size, String label) {
    return buffer_alloc_typed(buffer, at_byte, size, label, BUFFER_TYPE_BUFFER | BUFFER_TYPE_SUB);
}

Buffer buffer_alloc_typed(Buffer* parent, u64 at_byte, u64 size, String label, BufferType type) {
#if BUFFER_BOUNDS_CHECKING
    if(at_byte + size > parent->size) {
        log_fail("suballoc_buffer: overrun!");
    }
#endif

    Buffer child = {};
    child.memory = &buffer->memory[at_byte];
    child.size = size;

#if BUFFER_TRACKING
    child.label = label;
    child.type = type;
#endif
    return child;
}

Buffer buffer_from_memory(void* memory, u64 size, String label) {
    return buffer_type_from_memory(memory, size, label, BUFFER_TYPE_RAW);
}

Buffer buffer_from_memory_typed(void* memory, u64 size, String label, BufferType type) {
    Buffer buffer = {};
    buffer.memory = (u8*)memory;
    buffer.size = size;

#if BUFFER_TRACKING
    strncpy(buffer.label, label, BUFFER_LABEL_MAX_LENGTH);
    buffer.type = type;
#endif
    return buffer;
}

Buffer buffer_malloc(u64 size, String label) {
    void* memory = malloc(size);
    return buffer_from_memory(memory, size, label);
}

void buffer_clear_suballocations(Buffer* buffer) {
#if BUFFER_TRACKING
    buffer->children_len = 0;
#endif
}

#endif // CSM_IMPLEMENTATION
#endif // buffer_h_INCLUDED
