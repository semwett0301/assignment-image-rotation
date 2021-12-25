#include "../../include/headers/bmp.h"

static const uint16_t BMP_FILE_SIGNATURE = 0x4d42;
static const uint32_t HEADER_INFO_SIZE = 40;
static const uint16_t BITS_PER_PIXEL = 24;

static struct bmp_header create_header(uint32_t width, uint32_t height) {
    return (struct bmp_header) {
        .bfType = BMP_FILE_SIGNATURE,
        .bfileSize = sizeof(struct bmp_header) + sizeof(struct pixel) * width * height,
        .bfReserved = 0,
        .bOffBits = sizeof(struct bmp_header),
        .biSize = HEADER_INFO_SIZE,
        .biWidth = width,
        .biHeight = height,
        .biPlanes = 1,
        .biBitCount = BITS_PER_PIXEL,
        .biCompression = 0,
        .biSizeImage = 0,
        .biXPelsPerMeter = 0,
        .biYPelsPerMeter = 0,
        .biClrUsed = 0,
        .biClrImportant = 0,
    };
}

static uint32_t get_padding_from_input(uint32_t width) {
    if (width % 4 == 0) {
        return 0;
    } else {
        return 4 - (3 * width % 4);
    }
}

static uint32_t get_padding_to_output(uint32_t width) {
    if (width % 4 == 0) {
        return 0;
    } else {
        return (4 - (width * sizeof(struct pixel) % 4)) % 4;
    }
}

enum read_status from_bmp(FILE *in, struct image **img) {
    struct bmp_header header;

    fread(&header, sizeof(struct bmp_header), 1, in);
    if (header.bfType != BMP_FILE_SIGNATURE) return READ_INVALID_SIGNATURE;
    if (header.biBitCount != BITS_PER_PIXEL) return READ_INVALID_BITS;

    *img = create_image(header.biWidth, header.biHeight);
    uint32_t padding = get_padding_from_input(header.biWidth);

    for (size_t i = 0; i < header.biHeight; i++) {
        struct pixel tmp_pixel;

        for (size_t j = 0; j < header.biWidth; j++) {
            fread(&tmp_pixel, sizeof(struct pixel), 1, in);
            (*img)->data[header.biWidth * i + j] = tmp_pixel;
        }

        for (size_t j = 0; j < padding; j++) {
            fread(&tmp_pixel, sizeof(unsigned char), 1, in);
        }
    }

    return READ_OK;
}
enum write_status to_bmp(FILE *out, struct image const *img) {
    struct bmp_header bmpHeader = create_header(img->width, img->height);
    uint32_t padding = get_padding_to_output(img->width);

    if (fwrite(&bmpHeader, sizeof(struct bmp_header), 1, out) < 1) return WRITE_ERROR;
    fseek(out, bmpHeader.bOffBits, SEEK_SET);

    struct pixel pixel;
    for (uint32_t i = 0; i < img->height; i++) {
        for (uint32_t j = 0; j < img->width; j++) {
            pixel = img->data[img->width * i + j];
            if (fwrite(&pixel, sizeof(struct pixel), 1, out) < 1) return WRITE_ERROR;
        }

        for (int j = 0; j < padding; j++) {
            pixel.b = 0;
            pixel.g = 0;
            pixel.r = 0;
            if (fwrite(&pixel, sizeof(struct pixel), 1, out) < 1) return WRITE_ERROR;
        }
    }

    return WRITE_OK;
}
