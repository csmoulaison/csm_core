#ifndef csm_build_h_INCLUDED
#define csm_build_h_INCLUDED

#define BUILD_COMMON_PREFIX "gcc -std=c99 -I code/ -I extern/ "
#define BUILD_RELEASE_FLAGS " "
#define BUILD_DEBUG_FLAGS   "-g "
#define BUILD_WARNING_FLAGS "-Wall -Werror -Wno-unused "
#define BUILD_GL3W_FLAGS    "extern/GL/gl3w.c "
#define BUILD_X11_FLAGS     "-lX11 -lX11-xcb -lGL -lm -lxcb -lXfixes "
#define BUILD_MATH_FLAGS    "-lm "

#define BUILD_FLAG_RELEASE    1 << 0
#define BUILD_FLAG_DEBUG      1 << 1
#define BUILD_FLAG_WARNINGS   1 << 2
#define BUILD_FLAG_LINK_GL3W  1 << 3
#define BUILD_FLAG_TARGET_X11 1 << 4
#define BUILD_FLAG_LINK_MATH  1 << 5

typedef enum {
    BUILD_RESULT_SUCCESS,
    BUILD_RESULT_ERROR
} BuildResult;

BuildResult build_static(String name, String main_path, u64 flags, Stack* stack);
BuildResult build_dynamic(String name, String main_path, u64 flags, Stack* stack);

#ifdef CSM_IMPLEMENTATION

BuildResult build_static(String name, String main_path, u64 flags, Stack* stack) {
    system("mkdir -p bin");
    system("mkdir -p build");
    String cmd = string_from_stack(stack, 8196);
    string_cat(&cmd, string_new(BUILD_COMMON_PREFIX));

    if(flags & BUILD_FLAG_RELEASE) 
        string_cat(&cmd, string_new(BUILD_RELEASE_FLAGS));
    if(flags & BUILD_FLAG_DEBUG) 
        string_cat(&cmd, string_new(BUILD_DEBUG_FLAGS));
    if(flags & BUILD_FLAG_WARNINGS) 
        string_cat(&cmd, string_new(BUILD_WARNING_FLAGS));
    if(flags & BUILD_FLAG_LINK_GL3W) 
        string_cat(&cmd, string_new(BUILD_GL3W_FLAGS));

    if(flags & BUILD_FLAG_TARGET_X11) {
        string_cat(&cmd, string_new(BUILD_X11_FLAGS));
    } else if(flags & BUILD_FLAG_LINK_MATH) {
        string_cat(&cmd, string_new(BUILD_MATH_FLAGS));
    }

    string_cat(&cmd, main_path);
    string_cat(&cmd, " -o bin/");
    string_cat(&cmd, name);

    string_cat(&cmd, string_new("\0"));
    system(cmd.text);
}

// NOW: We are adapting the following lines from the old comet Makefile.
// mkdir -p bin
// mkdir -p build
// gcc $(FLAGS) -c -fPIC -o build/comet.o code/game.c $(INCLUDE)
// gcc $(FLAGS) -shared build/comet.o -o bin/comet_tmp.so $(INCLUDE) -lm
// mv bin/comet_tmp.so bin/comet.so
// -rm bin/comet_*
BuildResult build_dynamic(String name, String main_path, u64 flags, Stack* stack) {
    system("mkdir -p bin");
    system("mkdir -p build");
    String cmd = string_from_stack(stack, 8196);
    string_cat(&cmd, string_new(BUILD_COMMON_PREFIX));

    // TODO: first command

    string_cat(&cmd, string_new("\0"));
    system(cmd.text);

    string_clear(&cmd);

    // TODO: second command

    string_cat(&cmd, string_new("\0"));
    system(cmd.text);
}

#endif
#endif
