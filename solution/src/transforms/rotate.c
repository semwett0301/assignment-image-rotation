#include "rotate.h"

struct image* rotate( struct image *source ) {
    struct image* new_image = create_image(source->height, source->width);
    for (size_t i = 0; i < source->width; i++) {
        for (size_t j = 0; j < source->height; j++) {
            *find_pixel_in_image(source->height - 1 - j ,i, new_image) = *find_pixel_in_image(i, j, source);
        }
    }

    return new_image;
}
