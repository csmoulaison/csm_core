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

File file_open(String* fname, FileOpenMode mode);
void file_close(File* file);
void file_write(File* file, void* data, u64 len);
void string_write_file(File* file, StringWriter* writer);

#ifdef CSM_IMPLEMENTATION

File file_open(String* fname, FileOpenMode mode) {
    FILE* handle;
    char fname_cstring[4096];
    string_to_cstring(fname, fname_cstring, 4096);
    switch(mode) {
        case FILE_OPEN_READ: {
            handle = fopen(fname_cstring, "r");
        } break;
        case FILE_OPEN_WRITE: {
            handle = fopen(fname_cstring, "w");
        } break;
        case FILE_OPEN_READ_WRITE: {
            handle = fopen(fname_cstring, "rw");
        } break;
        default: {
            fprintf(stderr, "File open mode %i not valid.", mode);
            panic();
        } break;
    }
    //printf("handle %s\n\n", fname_cstring);
    assert(handle != NULL);
    File file;
    file.handle = handle;
    return file;
}

void file_close(File* file) {
    fclose(file->handle);
}

void file_write(File* file, void* data, u64 len) {
    fwrite(data, len, 1, file->handle);
}

u64 file_size(File* file) {
    fseek(file->handle, 0, SEEK_END);
    u64 size = ftell(file->handle);
    fseek(file->handle, 0, SEEK_SET);
    return size;
}

void string_write_file(File* file, StringWriter* writer) {
    char buf[128];
    u64 bytes_read = 0;
    while((bytes_read = fread(buf, 1, sizeof(buf), file->handle)) > 0) {
        string_write_char_buffer(writer, buf, bytes_read);
    }
}

#endif
#endif
