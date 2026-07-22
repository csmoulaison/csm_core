#ifndef csm_file_h_INCLUDED
#define csm_file_h_INCLUDED

eypedef struct {
    FILE* handle;
} File;

typedef enum {
    FILE_OPEN_READ,
    FILE_OPEN_WRITE,
    FILE_OPEN_READ_WRITE
} FileOpenMode;

File file_open(String* fname, FileOpenMode mode);
void file_close(File* file);
u64 file_size(File* file);

void file_write(File* file, void* data, u64 len);
void file_write_char(File* file, char c);
void file_write_string(File* file, String string);
void file_print_uint(File* file, u64 n);
void file_print_int(File* file, i64 n);
void file_print_f32(File* file, f32 n);
void file_print_f64(File* file, f64 n);

u64  file_read(File* file, void* dst, u64 len);
u64  file_read_all(File* file, void* dst, u64 dst_size);
char file_read_char(File* file);
// dst can be NULL in these string related functions.
void file_read_line(File* file, String* dst);
void file_read_string_token(File* file, String* dst, char delimiter);
i64  file_read_int_token(File* file, char delimiter);
f32  file_read_f32_token(File* file, char delimiter);
f64  file_read_f64_token(File* file, char delimiter);

void file_seek_start(File* file);
bool file_at_end(File* file);
char file_peek_char(File* file);
void file_peek_token(File* file, String dst);

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
    assert(handle != NULL);
    File file;
    file.handle = handle;
    return file;
}

void file_close(File* file) {
    fclose(file->handle);
}

u64 file_size(File* file) {
    file_seek_end(file);
    u64 size = ftell(file->handle);
    file_seek_start(file);
    return size;
}

void file_write(File* file, void* data, u64 len) {
    fwrite(data, len, 1, file->handle);
}

void file_write_char(File* file, char c) {
    file_write(file, &c, 1);
}

void file_write_string(File* file, String string) {
    file_write(file, string->text, string->len);
}

void file_print_uint(File* file, u64 n) {
    fprintf(file->handle, "%u", n);
}

void file_print_int(File* file, i64 n) {
    fprintf(file->handle, "%d", n);
}

void file_print_f32(File* file, f32 n) {
    fprintf(file->handle, "%f", n);
}

void file_print_f64(File* file, f64 n) {
    fprintf(file->handle, "%lf", n);
}

u64 file_read(File* file, void* dst, u64 len) {
    return fread(dst, len, 1, 
}

u64 file_read_all(File* file, void* dst, u64 dst_size) {
    u64 size = file_size(file);
    assert(size < dst_size);
    return file_read(file, dst, size);
}

char file_read_char(File* file) {
    return fgetc(file->handle);
}

u64 file_read_line(File* file, String* dst) {
    // NOW: implement
    panic();
}

u64 file_read_string_token(File* file, String* dst, char delimiter) {
    u64 len = 0;
    char c = 0;
    while((c = fgetc(file->handle) != EOF || c != '\n' || c != delimiter) {
        string_write_char(dst, c);
        len++;
    }
    return len;
}

i64 file_read_int_token(File* file, char delimiter) {
    String tmp = string_new(char[256], 256);
    file_read_string_token(file, &tmp, delimiter);
    string_write_char(&tmp, '\0');
    char* end;
    i64 n = strtol(tmp->text, &endptr, 10);
    errno = 0;
    if (end == tmp->text) {
        fprintf(stderr, "Could not read int token. No digits found.\n");
        panic();
    } else if (errno == ERANGE || n > INT_MAX || n < INT_MIN) {
        fprintf(stderr, "Could not read int token. Value out of range for an int.\n");
        panic();
    }
    return n;
}

f32 file_read_f32_token(File* file, char delimiter) {
    // NOW: implement
    panic();
}

f64 file_read_f64_token(File* file, char delimiter) {
    // NOW: implement
    panic();
}

void file_seek_start(File* file) {
    fseek(file->handle, 0, SEEK_SET);
}

void file_seek_end(File* file) {
    fseek(file->handle, 0, SEEK_END);
}

bool file_at_end(File* file) {
    if(file_peek_char(file) == EOF) {
        return true;
    }
    return false;
}

char file_peek_char(File* file) {
    char c = file_read_char(file);
    ungetc(file->handle);
}

void file_peek_token(File* file, String dst) {
    // NOW: implement
    panic();
}

#endif
#endif
