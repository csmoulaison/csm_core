#ifndef stack_h_INCLUDED
#define stack_h_INCLUDED

#define DEBUG_STACK true
#if DEBUG_STACK
	#define DEBUG_LOGGING false
	#define DEBUG_CAPACITY_WARNING false
#endif

typedef struct {
    union {
        Buffer buffer;
        struct {
            u8* memory;
            u64 size;
        };
    };
	u64 head;
} Stack;

Stack stack_init(Buffer buffer);
Stack stack_from_memory(u8* memory, u64 size, String label);
Stack stack_from_buffer(Buffer* buffer, u64 at_byte, u64 size, String label);
Stack stack_from_stack(Stack* stack, u64 size, String label);

void stack_clear(Stack* stack);
void stack_clear_to_zero(Stack* stack);
void* stack_alloc(Stack* stack, u64 size);
void* stack_alloc_labeled(Stack* stack, u64 size, String label);
void* stack_alloc_typed(Stack* stack, u64 size, String label, BufferType type);

#ifdef CSM_IMPLEMENTATION

Stack stack_init(Buffer buffer) {
    Stack stack = {};
    stack.buffer = buffer;
    stack.head = 0;

#if DEBUG_STACK
	sprintf(stack.label, "%s", label);

    #if DEBUG_LOGGING
	printf("%s: Stack initialized with size %lu.\n", label, size);
    #endif
#endif
	return stack;
}

Stack stack_from_memory(u8* memory, u64 size, String label)
{
	stack_init(buffer_from_memory_typed(memory, size, label, BUFFER_TYPE_RAW | BUFFER_TYPE_STACK));
}

Stack stack_from_buffer(Buffer* buffer, u64 at_byte, u64 size, String label) {
    stack_init(buffer_alloc_typed(buffer, at_byte, size, label, BUFFER_TYPE_SUB | BUFFER_TYPE_STACK));
}

Stack stack_from_stack(Stack* stack, u64 size, String label) {
    stack_init(stack_alloc_typed(stack, size, label, BUFFER_TYPE_SUB | BUFFER_TYPE_STACK));
}

void stack_clear(Stack* stack)
{
	stack->head = 0;
	buffer_clear_suballocations(&stack->buffer);

#if DEBUG_LOGGING
	printf("%s: Stack cleared.\n", stack->label);
#endif
}

void stack_clear_to_zero(Stack* stack)
{
	memset(stack->data, 0, stack->head);
	stack_clear(stack);
}

void* stack_alloc(Stack* stack, u64 size)
{
    stack_alloc_typed(stack, size, "stack_alloc", BUFFER_TYPE_SUB);
}

void* stack_alloc_labeled(Stack* stack, u64 size, String label) {
    stack_alloc_typed(stack, size, label, BUFFER_TYPE_SUB);
}

void* stack_alloc_typed(Stack* stack, u64 size, String label, BufferType type) {
	assert(stack->data != NULL);
	if(stack->head + size > stack->size) {
#if DEBUG_STACK
		printf("%s: Stack overflow! Size: %lu, Requested: %lu\n", stack->label, stack->size, stack->head + size);
#endif
		panic();
	}

#if DEBUG_LOGGING
	printf("%s: Stack allocation from %lu-%lu (%lu bytes)\n", stack->label, stack->head, stack->head + size, size);
#endif

	buffer_alloc_typed(&stack->buffer, stack->head, size, label, type);
	stack->head += size;

#if DEBUG_CAPACITY_WARNING
	if(stack->head > stack->size / 2) {
		printf("%s: Stack more than half full!\n", stack->label);
	}
#endif

	return &stack->data[stack->head - size];
}


#endif
#endif
