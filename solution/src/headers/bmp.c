#include "bmp.h"

static const uint16_t BMP_FILE_SIGNATURE = 0x4d42;
static const uint32_t HEADER_INFO_SIZE = 40;
static const uint16_t BITS_PER_PIXEL = 24;

static uint32_t get_padding(uint32_t width) {
    return width % 4;
//    if (width % 4 == 0) {
//        return 0;
//    } else {
//        return 4 - (3 * width % 4);
//    }
}

static size_t get_image_size(const struct image *image) {
    return (image->width * sizeof(struct pixel) + get_padding(image->width)) * image->height;
}

static struct bmp_header create_header(const struct image* img) {
    return (struct bmp_header) {
            .bfType = BMP_FILE_SIGNATURE,
            .bfileSize = sizeof(struct bmp_header) + sizeof(struct pixel) * img->width * img->height,
            .bfReserved = 0,
            .bOffBits = sizeof(struct bmp_header),
            .biSize = HEADER_INFO_SIZE,
            .biWidth = img->width,
            .biHeight = img->height,
            .biPlanes = 1,
            .biBitCount = BITS_PER_PIXEL,
            .biCompression = 0,
            .biSizeImage = get_image_size(img),
            .biXPelsPerMeter = 0,
            .biYPelsPerMeter = 0,
            .biClrUsed = 0,
            .biClrImportant = 0,
    };
}

enum read_status from_bmp(FILE *in, struct image **img) {
    struct bmp_header header;

    if(fread(&header, sizeof(struct bmp_header), 1, in) == 1) {
        if (header.bfType != BMP_FILE_SIGNATURE) return READ_INVALID_SIGNATURE;
        if (header.biBitCount != BITS_PER_PIXEL) return READ_INVALID_BITS;

        *img = create_image(header.biWidth, header.biHeight);
        const uint32_t padding = get_padding(header.biWidth);

        for (size_t i = 0; i < (*img)->height; ++i) {
            for (size_t j = 0; j < (*img)->width; ++j) {
                if (fread(&((*img)->data[(*img)->width * i + j]), sizeof(struct pixel), 1, in) != 1) {
                    return READ_FAILED;
                }
            }
            if (fseek(in, padding, SEEK_CUR) != 0) {
                return READ_FAILED;
            }
        }

        return READ_OK;
    } else {
        return READ_FAILED;
    }
}
enum write_status to_bmp(FILE *out, struct image const *img) {
    struct bmp_header bmpHeader = create_header(img);
    const uint32_t padding = get_padding(img->width);

    if (fwrite(&bmpHeader, sizeof(struct bmp_header), 1, out) < 1) return WRITE_ERROR;
    if (fseek(out, bmpHeader.bOffBits, SEEK_SET) != 0) {
        return WRITE_ERROR;
    }

    const uint8_t pad_bytes = 0;
    if(!img->data) {
        return WRITE_ERROR;
    }

    for (uint32_t i = 0; i < img->height; i++) {
        if (fwrite(img->data + i * img->width, img->width * sizeof(struct pixel), 1, out) < 1) {
            return WRITE_ERROR;
        };
        for (size_t j = 0; j < padding; ++j) {
            if (fwrite(&pad_bytes, 1, 1, out) < 1) {
                return WRITE_ERROR;
            }
        }
    }

    return WRITE_OK;
}
