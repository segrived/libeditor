#include <stdio.h>

#include "editor_headers.h"

void debug_file_header(BMPFILE *bitmap)
{
    printf("BMPFILEHEADER (size: %d bytes)\n", sizeof(bitmap->file_header));
    printf("bfType:      %d\n", bitmap->file_header.bfType);
    printf("bfSize:      %d\n", bitmap->file_header.bfSize);
    printf("bfReserved1: %d\n", bitmap->file_header.bfReserved1);
    printf("bfReserved2: %d\n", bitmap->file_header.bfReserved2);
    printf("bfOffBits:   %d\n", bitmap->file_header.bfOffBits);
    printf("\n");
}


void debug_info_header(BMPFILE *bitmap)
{
    printf("BITMAPINFOHEADER (size: %d bytes)\n", sizeof(bitmap->info_header));
    printf("biSize:          %d\n", bitmap->info_header.biSize);
    printf("biWidth:         %d\n", bitmap->info_header.biWidth);
    printf("biHeight:        %d\n", bitmap->info_header.biHeight);
    printf("biPlanes:        %d\n", bitmap->info_header.biPlanes);
    printf("biBitCount:      %d\n", bitmap->info_header.biBitCount);
    printf("biCompression:   %d\n", bitmap->info_header.biCompression);
    printf("biSizeImage:     %d\n", bitmap->info_header.biSizeImage);
    printf("biXPelsPerMeter: %d\n", bitmap->info_header.biXPelsPerMeter);
    printf("biYPelsPerMeter: %d\n", bitmap->info_header.biYPelsPerMeter);
    printf("biClrUsed:       %d\n", bitmap->info_header.biClrUsed);
    printf("biClrImportant:  %d\n", bitmap->info_header.biClrImportant);
    printf("\n");
}


void debug_pixels(BMPFILE *bitmap)
{
    for(int i = 0; i < bitmap->info_header.biWidth; i++) {
        for(int j = 0; j < bitmap->info_header.biHeight; j++) {
        printf("[ %02x %02x %02x ] ",
            bitmap->pixels[i][j].r,
            bitmap->pixels[i][j].g,
            bitmap->pixels[i][j].b);
        }
        printf("\n");
    }
}