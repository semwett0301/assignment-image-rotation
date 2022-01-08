#include "image.h"
#include <stdlib.h>

struct image* create_image(uint32_t width, uint32_t height) {
    struct image* new_img = malloc(sizeof(struct image));
    *new_img = (struct image) {
            width,
            height,
            malloc(sizeof (struct pixel) * width * height)
    };
    return new_img;
}

void destroy_image(struct image* source) {
    free(source->data);
    free(source);
}

struct pixel* find_pixel_in_image(uint32_t x_coord, uint32_t y_coord, struct image* source) {
    const uint32_t number = x_coord + y_coord * source->width;
    return &source->data[number];
}


