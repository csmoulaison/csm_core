#ifndef csm_string_h_INCLUDED
#define csm_string_h_INCLUDED

// TODO: Implement. Usage code first.

typedef struct {
    char* text;
    u64 len;
} String;

typedef struct {
    String string;
    u64 capacity;
} StringWriter;

String string_init(void* buffer);
String string_from_cstring(char* cstring);
String string_from_buffer(Buffer* buffer);
String string_from_stack(Stack* stack);

StringWriter string_writer_init(String string, u64 capacity);
StringWriter string_writer_from_cstring(char* cstring, u64 capacity);
StringWriter string_writer_from_buffer(Buffer* buffer, u64 capacity);
StringWriter string_writer_from_stack(Stack* stack, u64 capacity);

void string_write_cstring(StringWriter writer, char* cstring);
void string_write_cstring_len(StringWriter writer, char* cstring, u64 len);
void string_write_i32(StringWriter writer, i32 n);
void string_write_f32(StringWriter writer, f32 n);

#ifdef CSM_IMPLEMENTATION

String string_init(void* memory) {
    String string;
    string.text = memory;
    string.len = 0;
    return string;
}

String string_from_cstring(char* cstring) {
    String string;
    string.text = text;
    string.len = strlen(text);
    return string;
}

String string_from_buffer(Buffer* buffer, u64 byte_index, u64 len) {
    String string;
    string.text = buffer_suballoc(buffer, byte_index, len, "string_from_buffer");
    string.len = len;
    return string;
}

String string_from_stack(Stack* stack, u64 len) {
    String string;
    string.text = stack_alloc(stack, len);
    string.len = len;
    return string;
}

StringWriter string_writer_init(String* string, u64 capacity) {
    StringWriter writer;
    writer.string = &string;
    writer.capacity = capacity;
    return writer;
}

StringWriter string_writer_init_cstring(char* cstring, u64 capacity) {
    StringWriter writer;
    writer.string = string_init_cstring(cstring);
    writer.capacity = capacity;
    return writer;
}

StringWriter string_writer_from_stack(Stack* stack, u64 capacity) {
    StringWriter writer;
    writer.string = string_from_stack(stack);
    writer.capacity = capacity;
    return writer;
}

StringWriter string_writer_from_buffer(Buffer* buffer, u64 capacity) {
    StringWriter writer;
    writer.string = string_from_buffer(buffer);
    writer.capacity = capacity;
    return writer;
}

void string_write_cstring(StringWriter* writer, char* cstring) {
    u64 len = strlen(cstring);
    if(writer->string.len + len > writer->capacity) {
        fprintf(stderr, "String writer capacity overflow.\n");
        panic();
    }
    memcpy(writer->string.text, cstring, len);
    writer->string.len += len;
}

void string_write_cstring_len(StringWriter* writer, char* cstring, u64 len) {
    if(writer->string.len + len > writer->capacity) {
        fprintf(stderr, "String writer capacity overflow.\n");
        panic();
    }
    memcpy(writer->string.text, cstring, len);
    writer->string.len += len;
}

void string_write_i32(StringWriter* writer, i32 n) {
    TODO: implement
    panic();
}

void string_write_float(StringWriter* writer, f32 n) {
    TODO: implement
    panic();
}

#endif
#endif
