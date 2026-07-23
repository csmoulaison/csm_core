#ifndef csm_font_h_INCLUDED
#define csm_font_h_INCLUDED

typedef struct {
} FontData;

FontData* font_from_ttf(File* file, Stack* stack);
TextureData* texture_from_ttf(File* file, Stack* stack);
u64 font_size(FontData* data);

#ifdef CSM_IMPLEMENTATION

#endif
#endif
