#include "file.h"
#include "bmp.h"
#include "rotate.h"
#include <stdio.h>

int main( int argc, char** argv ) {
    if (argc < 3) {
        fprintf(stderr, "Не введены необходимые параметры");
        return 1;
    }

    FILE *input;
    struct image* input_image;
    enum open_status openStatus = file_open(&input, argv[1], "rb");
    if (openStatus == OPEN_FILE_NOT_EXIST) {
        fprintf(stderr, "Файла ввода не существует");
        return 1;
    }

    if (openStatus == OPEN_FILE_PERM_DENIED) {
        fprintf(stderr, "Не хватает прав для открытия файла ввода");
        return 1;
    }

    if (openStatus == OPEN_OTHER_ERROR) {
        fprintf(stderr, "При открытии файла ввода возникла ошибка");
        return 1;
    }

    enum read_status readStatus = from_bmp(input, &input_image);
    if (readStatus == READ_INVALID_SIGNATURE || readStatus == READ_INVALID_BITS) {
        fprintf(stderr, "При чтении картинки возникла ошибка (не тот формат)");
        return 1;
    }

    if (readStatus == READ_FAILED) {
        fprintf(stderr, "При чтении картинки возникла ошибка (не удалось считать)");
        return 1;
    }

    enum close_status closeStatus = file_close(&input);
    if (closeStatus == CLOSE_OTHER_ERROR || closeStatus == CLOSE_FILE_NOT_EXIST) {
        fprintf(stderr, "При закрытии файла ввода возникла ошибка");
        return 1;
    }


    struct image* output_image = rotate(input_image);

    destroy_image(input_image);

    FILE* output;

    enum open_status open_status_output = file_open(&output, argv[2], "wb");

    if (open_status_output == OPEN_FILE_NOT_EXIST) {
        fprintf(stderr, "Файла вывода не существует");
        return 1;
    }
    if (open_status_output == OPEN_FILE_PERM_DENIED) {
        fprintf(stderr, "Не хватает прав для открытия файла вывода");
        return 1;
    }
    if (open_status_output == OPEN_OTHER_ERROR) {
        fprintf(stderr, "При открытии файла вывода возникла ошибка");
        return 1;
    }

    enum write_status write_stat = to_bmp(output, output_image);
    if (write_stat == WRITE_ERROR) {
        fprintf(stderr, "При записи файла вывода возникла ошибка");
        return 1;
    }

    enum close_status close_status_output = file_close(&output);
    if (close_status_output == CLOSE_OTHER_ERROR || close_status_output == CLOSE_FILE_NOT_EXIST) {
        fprintf(stderr, "При закрытии файла вывода возникла ошибка");
        return 1;
    }

    destroy_image(output_image);
    return 0;
}
