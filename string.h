#ifndef csm_string_h_INCLUDED
#define csm_string_h_INCLUDED

#include <ctype.h>

typedef struct {
    char* text;
    u64 capacity;
    u64 len;
} String;

#undef CSM_IMPLEMENTATION
#include "./buffer.h"
#include "./stack.h"
#define CSM_IMPLEMENTATION

// Return a new string directly from a memory location.
String string_init(void* buffer, u64 capacity);
// Return a new string from a cstring. Capacity is set to the string length.
String string_new(char* literal);
// Return a new string by sub-allocating within a buffer.
String string_from_buffer(Buffer* buffer, u64 byte_index, u64 capacity);
// Return a new string by allocating from a stack.
String string_from_stack(Stack* stack, u64 capacity);

// Return true if both strings have equal length and the same characters.
bool string_equals(String a, String b);

// Set the string length to 0, leaving its capacity intact.
void string_clear(String* string);
// Convert all characters in string to uppercase.
void string_to_upper(String* string);
// Convert all characters in string to lowercase.
void string_to_lower(String* string);
// Convert all instances of a character in string to another.
void string_replace_char(String* string, char original, char replacement);
// Convert all instances of a substring in string to another.
void string_replace_substring(String* dst, String original, String replacement);

// Write a buffer of chars to dst.
void string_write(String* string, char* chars, u64 len);
// Write src to the end of dst.
void string_cat(String* dst, String* src);
// Write a single char to dst
void string_write_char(String* dst, char c) {}
// Write a string representation of an integer to dst.
void string_write_int(String* string, i64 n);
// Write a string representation of an f32 to dst.
void string_write_f32(String* string, f32 n);
// Write a string representation of an f64 to dst.
void string_write_f64(String* string, f64 n);

void string_write_file(String* string, 

typedef struct {
    String* string;
    u64 head;
} StringReader;

StringReader string_reader_init(String* string);
char string_read_char(StringReader* reader);
void string_read_line(StringReader* reader, String* dst);
void string_read_string_token(StringReader* reader, char delimiter);
i64  string_read_int_token(StringReader* reader, char delimiter);
f32  string_read_f32_token(StringReader* reader, char delimiter);
f64  string_read_f64_token(StringReader* reader, char delimiter);

#ifdef CSM_IMPLEMENTATION

String string_init(void* memory, u64 capacity) {
    String string;
    string.text = memory;
    string.len = 0;
    string.capacity = capacity;
    return string;
}

String string_new(char* cstring) {
    u64 len = strlen(cstring);
    String string;
    string.text = cstring;
    string.len = len;
    string.capacity = len;
    return string;
}

String string_from_buffer(Buffer* buffer, u64 byte_index, u64 len) {
    String string;
    string.text = buffer_suballoc(buffer, byte_index, len, "string_from_buffer").memory;
    string.len = len;
    return string;
}

String string_from_stack(Stack* stack, u64 len) {
    String string;
    string.text = stack_alloc(stack, len);
    string.len = len;
    return string;
}

bool string_equals(String a, String b) {
    if(a.len != b.len) {
        return false;
    }
    for(i32 i = 0; i < a.len; i++) {
        if(a.text[i] != b.text[i]) {
            return false;
        }
    }
    return true;
}

void string_clear(String* string) {
    string->len = 0;
}

void string_to_upper(String* string) {
    for(i32 i = 0; i < string->len; i++) {
        string->text[i] = toupper(string->text[i]);
    }
}

void string_to_lower(String* string) {
    for(i32 i = 0; i < string->len; i++) {
        string->text[i] = tolower(string->text[i]);
    }
}

void string_replace_char(String* string, char original, char replacement) {
    for(i32 i = 0; i < string->len; i++) {
        if(string->text[i] == original) {
            string->text[i] = replacement;
        }
    }
}

void string_replace_substring(String* dst, String original, String replacement) {
    i32 string_delta = replacement.len - original.len;
    String tmp = string_init((char[dst->len]), dst->len);
    for(i32 i = 0; i < dst->len; i++) {
        bool match = true;
        for(i32 j = 0; j < original.len; j++) {
            if(dst->text[i + j] == original.text[j]) {
                match = false;
                break;
            }
        }
        if(match) {
            string_cat(&tmp, replacement);
            i += original.len;
        } else {
            string_write_char(&tmp, dst->text[i]);
            i += 1;
        }
    }
    string_clear(dst);
    string_cat(dst, &tmp);
}

void string_write(String* dst, char* src, u64 len) {
    if(dst->len + len > dst->capacity) {
        fprintf(stderr, "String capacity overflow. Capacity: %u, Write len: %u\n", dst->capacity, len);
        panic();
    }
    memcpy(&dst->text[dst->len], src, len);
    string->len += len;
}

void string_cat(String* dst, String src) {
    string_write(dst, src.text, src.len);
}

void string_write_char(String* dst, char c) {
    string_write(dst, c, 1);
}

void string_print_int(String* dst, i64 n) {
    char buf[256];
    u64 len = snprintf(buf, 256, "%ld", n);
    assert(len < 256);
    string_write(dst, buf, len)
}

void string_print_f32(String* dst, f32 n) {
    char buf[256];
    u64 len = snprintf(buf, 256, "%f", n);
    assert(len < 256);
    string_write(dst, buf, len)
}

void string_print_f64(String* dst, f64 n) {
    char buf[256];
    u64 len = snprintf(buf, 256, "%lf", n);
    assert(len < 256);
    string_write(dst, buf, len)
}

StringReader string_reader_init(String* string) {
    StringReader reader;
    reader.string = string;
    reader.head = 0;
    return reader;
}

char string_read_char(StringReader* reader) {
    // NOW: bounds check. return end of string something
    reader->head++;
    return reader->string[reader->head - 1];
}

void string_read_line(StringReader* reader, String* dst) {

}

void string_read_string_token(StringReader* reader, String dst, char delimiter) {
    u64 len = 0;
    char c = 0;
    // NOW: adapt from file. fgetc becomes string_read_char
    while((c = fgetc(file->handle) != EOF || c != '\n' || c != delimiter) {
        string_write_char(dst, c);
        len++;
    }
    return len;
}

i64 string_read_int_token(StringReader* reader, char delimiter) {
    String tmp = string_new(char[256], 256);
    string_read_string_token(reader, &tmp, delimiter);
    string_write_char(&tmp, '\0');
    char* end;
    i64 n = strtol(tmp->text, &endptr, 10);
    errno = 0;
    if (end == tmp->text) {
        fprintf(stderr, "Could not read int token. No digits found.\n");
        panic();
    } else if (errno == ERANGE || n > INT_MAX || n < INT_MIN) {
        fprintf(stderr, "Error: Value out of range for an int.\n");
        panic();
    }
    return n;
}

f32 string_read_f32_token(StringReader* reader, char delimiter) { 
    // NOW: implement
    panic(); 
}

f64 string_read_f64_token(StringReader* reader, char delimiter) { 
    // NOW: implement
    panic(); 
}

#endif
#endif
