#include <stdlib.h>
#include <string.h>

#include "editor_headers.h"


void effect_greyscale(BMPFILE *bitmap)
{
    PIXEL *p;
    for(int i = 0; i < bitmap->info_header.biHeight; i++) {
        for(int j = 0; j < bitmap->info_header.biWidth; j++) {
            p = &bitmap->pixels[i][j];
            p->r = p->g = p->b = (byte)((p->r + p->g + p->b) / 3);
        }
    }
}

void effect_sepia(BMPFILE *bitmap, unsigned int depth)
{
    PIXEL *p;
    for(int i = 0; i < bitmap->info_header.biHeight; i++) {
        for(int j = 0; j < bitmap->info_header.biWidth; j++) {
            p = &bitmap->pixels[i][j];
            byte median = (byte)((p->r + p->g + p->b) / 3);
            p->r = norm_comp(median + depth * 2);
            p->g = norm_comp(median + depth);
            p->b = median;
        }
    }
}

void effect_invert(BMPFILE *bitmap)
{
    PIXEL *p;
    for(int i = 0; i < bitmap->info_header.biHeight; i++) {
        for(int j = 0; j < bitmap->info_header.biWidth; j++) {
            p = &bitmap->pixels[i][j];
            p->r ^= 0xff;
            p->g ^= 0xff;
            p->b ^= 0xff;
        }
    }
}

void effect_conv_filter(BMPFILE *bitmap, CONV_FILTER *filter)
{
    PIXEL** new_image;
    new_image = malloc(sizeof(PIXEL *) * bitmap->info_header.biHeight);

    for(int i = 0; i < bitmap->info_header.biHeight; i++) {
        new_image[i] = malloc(sizeof(PIXEL) * bitmap->info_header.biWidth);
    }

    unsigned int imageHeight = bitmap->info_header.biHeight;
    unsigned int imageWidth = bitmap->info_header.biWidth;
    int ix, iy;
    float cr, cg, cb;
    unsigned int fhd2 = (unsigned int)(filter->height / 2);
    unsigned int fwd2 = (unsigned int)(filter->width / 2);
    
    for (int x = 0; x < imageHeight; x++) {
        for (int y = 0; y < imageWidth; y++) {
            cr = 0.0f, cg = 0.0f, cb = 0.0f;
            for (int fx = 0; fx < filter->height; fx++) {
                for (int fy = 0; fy < filter->width; fy++) {
                    // Вычисление используемых координат изображения
                    ix = (x - fhd2 + fx + imageHeight) % imageHeight;
                    iy = (y - fwd2 + fy + imageWidth) % imageWidth;

                    cr += bitmap->pixels[ix][iy].r * filter->kernel[fx][fy];
                    cg += bitmap->pixels[ix][iy].g * filter->kernel[fx][fy];
                    cb += bitmap->pixels[ix][iy].b * filter->kernel[fx][fy];
                }
            }
            new_image[x][y].r = (byte)norm_comp((unsigned int)(filter->factor * cr + filter->offset));
            new_image[x][y].g = (byte)norm_comp((unsigned int)(filter->factor * cg + filter->offset));
            new_image[x][y].b = (byte)norm_comp((unsigned int)(filter->factor * cb + filter->offset));
        }
    }

    for(int i = 0; i < imageHeight; i++) {
        memcpy(&bitmap->pixels[i], &new_image[i], sizeof(new_image[i]));
    }
    
    free(new_image);
}


void effect_oil(BMPFILE *bitmap, unsigned int radius)
{
    PIXEL** new_image;
    new_image = malloc(sizeof(PIXEL *) * bitmap->info_header.biHeight);

    for(int i = 0; i < bitmap->info_header.biHeight; i++) {
        new_image[i] = malloc(sizeof(PIXEL) * bitmap->info_header.biWidth);
    }

    unsigned int imageHeight = bitmap->info_header.biHeight;
    unsigned int imageWidth = bitmap->info_header.biWidth;

    int ix, iy;

    PIXEL* tmp = malloc(radius * radius * sizeof(PIXEL *));

    for (int x = 0; x < bitmap->info_header.biHeight; x++) {
        for (int y = 0; y < bitmap->info_header.biWidth; y++) {
            for (int fx = 0; fx < radius; fx++) {
                for (int fy = 0; fy < radius; fy++) {
                    ix = (x - radius / 2 + fx + imageHeight) % imageHeight;
                    iy = (y - radius / 2 + fy + imageWidth) % imageWidth;
                    tmp[radius * fx + fy] = bitmap->pixels[ix][iy];
                }
            }
            sort_pixels_by_components(tmp, radius * radius);
            new_image[x][y] = tmp[radius * radius / 2];
        }
    }

    for(int i = 0; i < imageHeight; i++) {
        memcpy(&bitmap->pixels[i], &new_image[i], sizeof(new_image[i]));
    }

    free(new_image);
}


void effect_pen(BMPFILE *bitmap, unsigned int radius)
{
    PIXEL** new_image;
    new_image = malloc(sizeof(PIXEL *) * bitmap->info_header.biHeight);

    for(int i = 0; i < bitmap->info_header.biHeight; i++) {
        new_image[i] = malloc(sizeof(PIXEL) * bitmap->info_header.biWidth);
    }

    unsigned int imageHeight = bitmap->info_header.biHeight;
    unsigned int imageWidth = bitmap->info_header.biWidth;

    int ix, iy;
    unsigned int nr, ng, nb;

    PIXEL* tmp = malloc(radius * radius * sizeof(PIXEL *));

    for (int x = 0; x < bitmap->info_header.biHeight; x++) {
        for (int y = 0; y < bitmap->info_header.biWidth; y++) {
            for (int fx = 0; fx < radius; fx++) {
                for (int fy = 0; fy < radius; fy++) {
                    ix = (x - radius / 2 + fx + imageHeight) % imageHeight;
                    iy = (y - radius / 2 + fy + imageWidth) % imageWidth;
                    tmp[radius * fx + fy] = bitmap->pixels[ix][iy];
                }
            }
            sort_pixels_by_components(tmp, radius * radius);
            new_image[x][y].r = (byte)((tmp[0].r + tmp[radius * radius - 1].r) / 2);
            new_image[x][y].g = (byte)((tmp[0].g + tmp[radius * radius - 1].g) / 2);
            new_image[x][y].b = (byte)((tmp[0].b + tmp[radius * radius - 1].b) / 2);
        }
    }

    for(int i = 0; i < imageHeight; i++) {
        memcpy(&bitmap->pixels[i], &new_image[i], sizeof(new_image[i]));
    }
    
    free(new_image);
}