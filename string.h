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
} StringBuilder;

String string_init(char* text);

#ifdef CSM_IMPLEMENTATION

String string_init(char* text) {
    String string;
    string.len = strlen(text);
    string.text = text;
    return string;
}

#endif
#endif
