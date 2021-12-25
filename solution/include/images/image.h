#include  <stdint.h>

struct image {
    uint64_t width, height;
    struct pixel *data;
};

struct pixel {
    uint8_t b, g, r;
};

struct image create_image(uint32_t width, uint32_t height);

void destroy_image(struct image source);

struct pixel* find_pixel_in_image(uint32_t x_coord, uint32_t y_coord, struct image source);