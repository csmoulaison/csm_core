#ifndef stack_h_INCLUDED
#define stack_h_INCLUDED

#define DEBUG_STACK true
#if DEBUG_STACK
	#define DEBUG_LOGGING false
	#define DEBUG_CAPACITY_WARNING false
#endif

typedef struct {
	u64 index;
	u64 size;
	u8* data;
	bool initialized;
#if DEBUG_STACK
	char debug_name[64];
#endif
} Stack;

Stack stack_init(u8* memory, u64 size, const char* debug_name);
void stack_clear(Stack* stack);
void stack_clear_to_zero(Stack* stack);
void* stack_alloc(Stack* stack, u64 size);
void* stack_head(Stack* stack);

#ifdef CSM_IMPLEMENTATION

Stack stack_init(u8* memory, u64 size, const char* debug_name)
{
	Stack stack;
	stack.data = memory;
	stack.index = 0;
	stack.size = size;
	stack.initialized = true;

#if DEBUG_STACK
	sprintf(stack.debug_name, "%s", debug_name);
#if DEBUG_LOGGING
	printf("%s: Stack initialized with size %lu.\n", debug_name, size);
#endif
#endif

	return stack;
}

void stack_clear(Stack* stack)
{
	stack->index = 0;
#if DEBUG_LOGGING
	printf("%s: Stack cleared.\n", stack->debug_name);
#endif
}

void stack_clear_to_zero(Stack* stack)
{
	memset(stack->data, 0, stack->index);
	stack_clear(stack);
}

void* stack_head(Stack* stack)
{
	return (void*)&stack->data[stack->index];
}

void* stack_alloc(Stack* stack, u64 size)
{
	assert(stack->data != NULL);
	if(stack->index + size > stack->size) {
#if DEBUG_STACK
		printf("%s: Stack overflow! Size: %lu, Requested: %lu\n", stack->debug_name, stack->size, stack->index + size);
#endif
		panic();
	}

#if DEBUG_LOGGING
	printf("%s: Stack allocation from %lu-%lu (%lu bytes)\n", stack->debug_name, stack->index, stack->index + size, size);
#endif

	stack->index += size;

#if DEBUG_CAPACITY_WARNING
	if(stack->index > stack->size / 2) {
		printf("%s: Stack more than half full!\n", stack->debug_name);
	}
#endif

	return &stack->data[stack->index - size];
}

#endif // CSM_IMPLEMENTATION
#endif // stack_h_INCLUDED
