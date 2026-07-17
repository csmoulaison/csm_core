#ifndef csm_file_h_INCLUDED
#define csm_file_h_INCLUDED

typedef struct {
    FILE* handle;
} File;

typedef enum {
    FILE_OPEN_READ,
    FILE_OPEN_WRITE,
    FILE_OPEN_READ_WRITE
} FileOpenMode;

File file_open(String fname, FileOpenMode mode);
void file_close(File file);

String file_to_string(String fname, StringBuilder fstring);
String file_to_string_ez(char* fname);

#ifdef CSM_IMPLEMENTATION

File file_open(String fname, FileOpenMode mode) {
    switch(mode) {
        case FILE_OPEN_READ: {
            FILE* file = fopen(string_cstr(fname), "r");
        } break;
        case FILE_OPEN_WRITE: {
            FILE* file = fopen(string_cstr(fname), "w");
        } break;
        case FILE_OPEN_READ_WRITE: {
            FILE* file = fopen(string_cstr(fname), "rw");
        } break;
        default: {
            fprintf(stderr, "File open mode %i not valid.", mode);
            panic();
        } break;
    }
    assert(file != NULL);
    File file;
    file.handle;
    return file;
}

void file_close(File file) {
    fclose(file.handle);
}

void file_size(File file) {
    fseek(file.handle, 0, SEEK_END);
    u64 size = ftell(file_handle);
    fseek(file_handle, 0, SEEK_SET);
    return size;
}

String file_to_string(File file, StringBuilder builder) {
    char buf[128];
    u64 bytes_read = 0;
    while((bytes_read = fread(buf, 1, sizeof(buf), file.handle)) > 0) {
        string_write_cstring_len(builder, buf, len);
    }
}

String file_to_string_ez(char* fname) {
    File file = file_open(fname);    
    u64 size = file_size(file);
    char* cstr = buffer_alloc(size, "string_from_file_ez").memory;
    StringBuilder builder = string_builder_init_cstring(cstr);
    file_to_string(file, builder);
}

#endif
#endif
