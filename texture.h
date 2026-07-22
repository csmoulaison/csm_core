#ifndef csm_texture_h_INCLUDED
#define csm_texture_h_INCLUDED

typedef struct {
    union {
        u32 pixel;
        struct {
            u8 r, g, b, a;
        } components;
    };
} TexturePixelData;

typedef struct {
    u64 width;
    u64 height;
    TexturePixelData pixels[];
} TextureData;

#pragma pack(push, 1)
typedef struct {
    char signature[2];
    u32 file_size;
    u32 reserved;
    u32 data_offset;
    u32 info_size;
    u32 width;
    u32 height;
    u16 planes;
    u16 bits_per_pixel;
    u32 compression;
    u32 image_size;
    u32 x_pixels_per_meter;
    u32 y_pixels_per_meter;
    u32 colors_used;
    u32 important_colors;
} TextureBmpInfo;
#pragma pack(pop)

TextureData* texture_from_bmp(File* file, Stack* stack);
u64 texture_size_from_dimensions(u64 width, u64 height);
u64 texture_size(TextureData* texture);

#ifdef CSM_IMPLEMENTATION

TextureData* texture_from_bmp(File* file, Stack* stack) {
    assert(sizeof(TextureBmpInfo) == 54);
    TextureBmpInfo info = {};
    file_read(file, info, sizeof(TextureBmpInfo));

    assert(info.signature[0] == 'B' && info.signature[1] == 'M');
    assert(info.width * info.height <= TEXTURE_MAX_PIXELS);
    assert(info->bits_per_pixel == 32);
    assert(info.compression == 0);

    TextureData* data = (TextureData*)stack_alloc(
        &stack, texture_size_from_dimensions(info->width, info->height));
    data->width = info.width;
    data->height = info.height;
    for(i32 i = 0; i < info.width * info.height; i++) {
        u8 pixels[4];
        file_read(file, pixels, sizeof(pixels));
        // BMP files store pixels in ABGR order.
        data->pixels[i].components.r = *(pixels[3]);
        data->pixels[i].components.g = *(pixels[2]);
        data->pixels[i].components.b = *(pixels[1]);
        data->pixels[i].components.a = *(pixels[0]);
    }
}

u64 texture_size_from_dimensions(u64 width, u64 height) {
    return sizeof(TextureData) + width * height * sizeof(TexturePixelData));
}

u64 texture_size(TextureData* data) {
    return texture_size_from_dimensions(data->width, data->height);
}

#endif
#endif
