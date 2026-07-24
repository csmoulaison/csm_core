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
void file_close(File* file);
u64 file_size(File* file);
u64 file_last_modified(File* file);

void file_write(File* file, void* data, u64 len);
void file_write_char(File* file, char c);
void file_write_string(File* file, String string);
void file_print_uint(File* file, u64 n);
void file_print_int(File* file, i64 n);
void file_print_float(File* file, f64 n);

u64  file_read(File* file, void* dst, u64 len);
u64  file_read_all(File* file, void* dst, u64 dst_size);
char file_read_char(File* file);
// dst can be NULL in these string related functions.
void file_read_line(File* file, String* dst);
u64  file_read_string_token(File* file, String* dst, char delimiter);
i64  file_read_int_token(File* file, char delimiter);
f64  file_read_float_token(File* file, char delimiter);

void file_seek(File* file, f64 offset);
void file_seek_start(File* file);
void file_seek_end(File* file);
bool file_at_end(File* file);
char file_peek_char(File* file);
void file_peek_string_token(File* file, String* dst, char delimiter);

#ifdef CSM_IMPLEMENTATION

File file_open(String fname, FileOpenMode mode) {
    FILE* handle;
    String fname_cstring = string_init((char[fname.len+1]){}, fname.len+1);
    string_cat(&fname_cstring, fname);
    string_write_null_terminator(&fname_cstring);
    switch(mode) {
        case FILE_OPEN_READ: {
            handle = fopen(fname_cstring.text, "r");
        } break;
        case FILE_OPEN_WRITE: {
            handle = fopen(fname_cstring.text, "w");
        } break;
        case FILE_OPEN_READ_WRITE: {
            handle = fopen(fname_cstring.text, "rw");
        } break;
        default: {
            fprintf(stderr, "File open mode %i not valid.", mode);
            panic();
        } break;
    }
    if(handle == NULL) {
        fprintf(stderr, "File '%s' could not be opened.\n", fname_cstring.text);
        panic();
    }
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

u64 file_last_modified(File* file) {
    i32 descriptor = fileno(file->handle);
    struct stat stat;
    assert(fstat(descriptor, &stat) == 0);
    return (u64)stat.st_mtime;
}

void file_write(File* file, void* data, u64 len) {
    fwrite(data, len, 1, file->handle);
}

void file_write_char(File* file, char c) {
    file_write(file, &c, 1);
}

void file_write_string(File* file, String string) {
    file_write(file, string.text, string.len);
}

void file_print_uint(File* file, u64 n) {
    fprintf(file->handle, "%u", n);
}

void file_print_int(File* file, i64 n) {
    fprintf(file->handle, "%d", n);
}

void file_print_float(File* file, f64 n) {
    fprintf(file->handle, "%lf", n);
}

u64 file_read(File* file, void* dst, u64 len) {
    return fread(dst, len, 1, file->handle);
}

u64 file_read_all(File* file, void* dst, u64 dst_size) {
    u64 size = file_size(file);
    assert(size < dst_size);
    return file_read(file, dst, size);
}

char file_read_char(File* file) {
    return fgetc(file->handle);
}

void file_read_line(File* file, String* dst) {
    char c;
    while((c = file_read_char(file)) != '\n') {
        if(c == EOF) {
            return;
        }
        if(dst != NULL) {
            string_write_char(dst, c);
        }
    }
}

u64 file_read_string_token(File* file, String* dst, char delimiter) {
    u64 len = 0;
    char c = 0;
    while((c = fgetc(file->handle)) != EOF && c != '\n' && c != delimiter) {
        if(dst != NULL) {
            string_write_char(dst, c);
        }
        len++;
    }
    return len;
}

// TODO: factor int/float conversions into string_to_* functions.
i64 file_read_int_token(File* file, char delimiter) {
    String tmp = string_init((char[256]){}, 256);
    file_read_string_token(file, &tmp, delimiter);
    string_write_null_terminator(&tmp);
    char* end;
    i64 n = strtol(tmp.text, &end, 10);
    errno = 0;
    if (end == tmp.text) {
        fprintf(stderr, "Could not read int token. No digits found.\n");
        panic();
    } else if (errno == ERANGE || n > INT_MAX || n < INT_MIN) {
        fprintf(stderr, "Could not read int token. Value out of range for an int.\n");
        panic();
    }
    return n;
}

f64 file_read_float_token(File* file, char delimiter) {
    String tmp = string_init((char[256]){}, 256);
    file_read_string_token(file, &tmp, delimiter);
    string_write_null_terminator(&tmp);
    char* end;
    f64 n = strtof(tmp.text, &end);
    if (end == tmp.text) {
        fprintf(stderr, "Could not read float token.\n");
        panic();
    }
    return n;
}

void file_seek(File* file, f64 offset) {
    fseek(file->handle, offset, SEEK_CUR);
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
    if(c != EOF) {
        ungetc(c, file->handle);
    }
    return c;
}

void file_peek_string_token(File* file, String* dst, char delimiter) {
    u64 len = file_read_string_token(file, dst, delimiter);
    file_seek(file, -len);
}

#endif
#endif
