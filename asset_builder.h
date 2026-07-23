#ifndef csm_asset_pack_h_INCLUDED
#define csm_asset_pack_h_INCLUDED

#ifndef ASSET_BUILDER_MAX_TYPES
#define ASSET_BUILDER_MAX_TYPES 64
#endif

#ifndef ASSET_BUILDER_MAX_ASSETS_PER_TYPE
#define ASSET_BUILDER_MAX_ASSETS_PER_TYPE 4096
#endif

typedef struct {
    String tag;
    u64    byte_index;
} Asset;

typedef struct {
    String name;
    Asset  assets[ASSET_BUILDER_MAX_ASSETS_PER_TYPE];
    u32    assets_len;
} AssetType;

typedef struct {
    Stack*    stack;
    AssetType types[ASSET_BUILDER_MAX_TYPES];
    u32       types_len;
} AssetBuilder;

void asset_builder_init(AssetBuilder* builder, Stack* stack);
void asset_builder_push_asset(AssetBuilder* builder, String tag, String type_name, void* data, u64 size);
void asset_builder_output_pack(AssetBuilder* builder, String path);
void asset_builder_output_header(AssetBuilder* builder, String path);

#ifdef CSM_IMPLEMENTATION

void asset_builder_init(AssetBuilder* builder, Stack* stack) {
    builder->stack = stack;
    builder->types_len = 0;
}

void asset_builder_push_asset(AssetBuilder* builder, String tag, String type_name, void* data, u64 size) {
    string_to_upper(&tag);
    string_to_upper(&type_name);

    AssetType* type = NULL;
    for(i32 i = 0; i < builder->types_len; i++) {
        if(string_equals(type_name, builder->types[i].name)) {
            type = &builder->types[i];
            break;
        }
    }

    if(type == NULL) {
        type = &builder->types[builder->types_len];
        type->name = type_name;
        assert(builder->types_len < ASSET_BUILDER_MAX_TYPES);
        builder->types_len++;
    }

    Asset* asset = &type->assets[type->assets_len];
    assert(type->assets_len < ASSET_BUILDER_MAX_ASSETS_PER_TYPE);
    type->assets_len++;
    asset->tag = tag;
    asset->byte_index = builder->stack->head;
    void* dst = stack_alloc(builder->stack, size);
    memcpy(dst, data, size);
}

void asset_builder_output_pack(AssetBuilder* builder, String path) {
    File file = file_open(path, FILE_OPEN_WRITE);
    file_write(&file, builder->stack->memory, builder->stack->head);
    file_close(&file);
}

void asset_builder_output_header(AssetBuilder* builder, String path) {
    File file = file_open(path, FILE_OPEN_WRITE);
    file_write_string(&file, string_new("// Pregenerated file. Any changes made will be erased on recompilation.\n\n"));
    for(i32 i = 0; i < builder->types_len; i++) {
        AssetType* type = &builder->types[i];
        file_write_string(&file, string_new("#define "));
        file_write_string(&file, type->name);
        file_write_string(&file, string_new("_COUNT "));
        file_print_int(&file, type->assets_len);
        file_write_string(&file, string_new("\n"));
        for(i32 j = 0; j < type->assets_len; j++) {
            Asset* asset = &type->assets[j];
            file_write_string(&file, string_new("#define "));
            file_write_string(&file, type->name);
            file_write_string(&file, string_new("_"));
            file_write_string(&file, asset->tag);
            file_write_string(&file, string_new("\n"));
        }
        file_write_string(&file, string_new("\n"));
    }
    file_close(&file);
}

#endif
#endif
