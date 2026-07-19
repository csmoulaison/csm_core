#ifndef csm_texture_h_INCLUDED
#define csm_texture_h_INCLUDED

#ifndef TEXTURE_MAX_PIXELS
#define TEXTURE_MAX_PIXELS 16000000
#endif

typedef struct {
    union {
        u32 pixel;
        struct {
            u8 r, g, b, a;
        } components;
    };
} PixelData;

typedef struct {
    u64 width;
    u64 height;
    PixelData pixels[TEXTURE_MAX_PIXELS];
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

void texture_from_bmp(String* bmp, TextureData* data);

#ifdef CSM_IMPLEMENTATION

void texture_from_bmp(String* bmp, TextureData* data) {
    assert(bmp->len >= sizeof(TextureBmpInfo));
    TextureBmpInfo* info = (TextureBmpInfo*)bmp->text;

    assert(info->signature[0] == 'B' && info->signature[1] == 'M');
    assert(info->width * info->height <= TEXTURE_MAX_PIXELS);
    assert(info->bits_per_pixel == 32);
    assert(info->compression == 0);

    data->width = info->width;
    data->height = info->height;
    for(i32 i = 0; i < info->width * info->height; i++) {
        u8* pixel = bmp->text + info->data_offset + i * 4;
        data->pixels[i].components.r = *(pixel+3);
        data->pixels[i].components.g = *(pixel+2);
        data->pixels[i].components.b = *(pixel+1);
        data->pixels[i].components.a = *(pixel+0);
    }
}

#endif
#endif
