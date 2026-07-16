#ifndef csm_texture_h_INCLUDED
#define csm_texture_h_INCLUDED

#ifndef TEXTURE_MAX_PIXELS
#define TEXTURE_MAX_PIXELS 65535
#endif

typedef struct {
    union {
        f32 pixel;
        struct {
            u8 r, g, b, a;
        } components;
    };
} PixelData;

typedef struct {
    PixelData pixels[TEXTURE_MAX_PIXELS];
} TextureData;

TextureData texture_from_bmp(char* bmp);

#ifdef CSM_IMPLEMENTATION

TextureData texture_from_bmp(String bmp) {
    // String and file helpers first.
}

#endif
#endif
