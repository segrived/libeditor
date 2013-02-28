#include <stdlib.h>

#include "editor_headers.h"

#define swap(a, b)   \
do {                 \
    int temp = a;    \
    a = b;           \
    b = temp;        \
} while(0)

CONV_FILTER *create_conv_filter(int height, int width, float factor_div, float offset) {
    CONV_FILTER *f;
    f = malloc(sizeof(CONV_FILTER));
    f->height = height;
    f->width  = width;
    f->factor = 1.0f / (float)factor_div;
    f->offset = offset;
    // Выделение памяти для хранения матрицы ядра фильтра
    f->kernel = malloc(height * sizeof(float*));
    for(int i = 0; i < height; i++)
        f->kernel[i] = malloc(width * sizeof(float));
    return f;
}

void sort_pixels_by_components(PIXEL* input, size_t size)
{
    int sf;

    for(int i = 0; i < size - 1; i++) {
        sf = 0;
        for(int j = 0; j < size - i; j++) {
            if(input[j].r > input[j + 1].r) {
                swap(input[j].r, input[j + 1].r);
                sf = 1;
            }
            if(input[j].g > input[j + 1].g) {
                swap(input[j].g, input[j + 1].g);
                sf = 1;
            }
            if(input[j].b > input[j + 1].b) {
                swap(input[j].b, input[j + 1].b);
                sf = 1;
            }
        }
        if(sf == 0) break;
    }
}