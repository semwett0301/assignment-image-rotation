#include "../../include/images/image.h"

struct image create_image(uint32_t width, uint32_t height) {
    return (struct image) {
            width,
            height,
            malloc(sizeof (struct pixel) * width * height)
    };
}

void destroy_image(struct image source) {
    free(source.data);
};

struct pixel* find_pixel_in_image(uint32_t x_coord, uint32_t y_coord, struct image source) {
    size_t number = x_coord + y_coord * source.width;
    return source.data[number];
};


