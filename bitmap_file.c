#include <stdio.h>
#include <stdlib.h>

#include "editor_headers.h"

void read_bmp_file(char *file_name, BMPFILE *bitmap) {
    FILE * f = fopen(file_name, "rb");
    if (f == NULL) {
        perror ("Error opening file");
    }

    // Чтение заголовка файла в структуру
    fread(&bitmap->file_header, sizeof(bitmap->file_header), 1, f);
    // Чтение заголовка изображения в структуру
    fread(&bitmap->info_header, sizeof(bitmap->info_header), 1, f);

    // 0x4D42 = "BM" - стандартный заголовок BMP-файла
    if(bitmap->file_header.bfType != 0x4D42) {
        perror("Invalid BMP header");
    }

    fseek(f, 0, SEEK_END);
    int content_size = ftell(f) - bitmap->file_header.bfOffBits;
    fseek(f, bitmap->file_header.bfOffBits, SEEK_SET);
    unsigned char *pixel_data = malloc(content_size * sizeof(unsigned char));
    fread(pixel_data, content_size, 1, f);
    // Количество байт, служащих для выравнивания в BMP-файле
    int padding_size = (4 - (bitmap->info_header.biWidth * 3 % 4)) % 4;
    // Размер каждой строки изображения в байтах (включая выравнивание)
    int line_size_in_bytes = bitmap->info_header.biWidth * 3 + padding_size;

    bitmap->pixels = (PIXEL**)malloc(sizeof(PIXEL *) * bitmap->info_header.biHeight);

    unsigned int start_line_index, start_pixel_index;
    for(int x = 0; x < bitmap->info_header.biHeight; x++) {
        start_line_index = line_size_in_bytes * x;
        bitmap->pixels[x] = (PIXEL*)malloc(sizeof(PIXEL) * bitmap->info_header.biWidth);
        int startIndex;
        for(int y = 0; y < bitmap->info_header.biWidth; y++) {
            start_pixel_index = start_line_index + y * 3;
            bitmap->pixels[x][y].r = pixel_data[start_pixel_index + 2];
            bitmap->pixels[x][y].g = pixel_data[start_pixel_index + 1];
            bitmap->pixels[x][y].b = pixel_data[start_pixel_index];
        }
    }
    fclose(f);
}

void write_bmp_file(BMPFILE *bitmap, char *file_name) {
    FILE *f;
    f = fopen(file_name, "wb");
    if (f == NULL)
        perror ("Error opening file");

    fwrite(&bitmap->file_header, 1, sizeof(bitmap->file_header), f);
    fwrite(&bitmap->info_header, 1, sizeof(bitmap->info_header), f);

    int padding_size = (4 - (bitmap->info_header.biWidth * 3 % 4)) % 4;
    int line_size_in_bytes = bitmap->info_header.biWidth * 3 + padding_size;

    byte *line = (byte*)malloc(line_size_in_bytes * sizeof(byte));

    int start_index;

    for(int x = 0; x < bitmap->info_header.biHeight; x++) {
        for(int y = 0; y < bitmap->info_header.biWidth; y++) {
            start_index = y * 3;
            line[start_index]     = bitmap->pixels[x][y].b;
            line[start_index + 1] = bitmap->pixels[x][y].g;
            line[start_index + 2] = bitmap->pixels[x][y].r;
        }
        fwrite(line, line_size_in_bytes, 1, f);
    }
    fclose(f);
}