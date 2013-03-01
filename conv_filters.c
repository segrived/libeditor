#include <stdlib.h>
#include <math.h>

#include "editor_headers.h"


CONV_FILTER * cf_motion_blur(int radius) {
    CONV_FILTER *filter = create_conv_filter(radius, radius, radius, 0.0f);
    for(int i = 0; i < filter->height; i++) {
        for(int j = 0; j < filter->width; j++) {
            filter->kernel[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
    return filter;
}


CONV_FILTER * cf_simple_blur(int radius) {
    CONV_FILTER *filter = create_conv_filter(radius, radius, radius * radius / 2 + 1, 0.0f);
    unsigned int center = (unsigned int)(radius / 2);
    for (int i = 0; i < filter->height; i++) {
        for (int j = 0; j < filter->width; j++) {
            int c = abs(center - i);
            filter->kernel[i][j] = (c - 1.0f < j && j < radius - c) ? 1.0f : 0.0f;
        }
    }
    return filter;
}


CONV_FILTER * cf_soft_blur(int radius) {
    CONV_FILTER *filter = create_conv_filter(radius, radius, 1.0f, 0.0f);
    int hr = radius / 2;
    double a = -2.0 * hr * hr / log(0.01);
    double sum = 0.0;
    for (int i = 0; i < filter->height; i++) {
        for (int j = 0; j < filter->width; j++) {
            double dist = sqrt((i - hr) * (i - hr) + (j - hr) * (j - hr));
            filter->kernel[i][j] = exp(-dist * dist / a);
            sum += filter->kernel[i][j];
        }
    }
    for (int i = 0; i < filter->height; i++) {
        for (int j = 0; j < filter->width; j++) {
            filter->kernel[i][j] /= sum;
        }
    }
    return filter;
}


CONV_FILTER * cf_simple_edge_detection() {
    CONV_FILTER *filter = create_conv_filter(3, 3, 1.0f, 0.0f);

    float f[3][3] = {
        { -1.0f, -1.0f, -1.0f },
        { -1.0f,  8.0f, -1.0f },
        { -1.0f, -1.0f, -1.0f }
    };

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            filter->kernel[i][j] = f[i][j];
        }
    }

    return filter;
}

CONV_FILTER * cf_soft_sharpen() {
    CONV_FILTER *filter = create_conv_filter(5, 5, 8.0f, 0.0f);

    float f[5][5] = {
        { -1.0f, -1.0f, -1.0f, -1.0f, -1.0f },
        { -1.0f,  2.0f,  2.0f,  2.0f, -1.0f },
        { -1.0f,  2.0f,  8.0f,  2.0f, -1.0f },
        { -1.0f,  2.0f,  2.0f,  2.0f, -1.0f },
        { -1.0f, -1.0f, -1.0f, -1.0f, -1.0f }
    };

    for(int i = 0; i < filter->width; i++) {
        for(int j = 0; j < filter->height; j++) {
            filter->kernel[i][j] = f[i][j];
        }
    }

    return filter;
}