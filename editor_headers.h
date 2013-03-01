#ifndef EDITOR_HEADERS
#define EDITOR_HEADERS

// byte = unsigned char
typedef unsigned char byte;

#pragma pack(2)
// Структура заголовка BMP-файла
typedef struct
{
    unsigned short bfType;      // "Магические" значения (для проверки валидности файла)
    unsigned int   bfSize;      // Общий размер BMP-файла
    unsigned short bfReserved1; // Заразервированных байты
    unsigned short bfReserved2; // Заразервированных байты
    unsigned int   bfOffBits;   // Оффсет, с которого начинаются данные
} BMPFILEHEADER;
#pragma pack()

#pragma pack(2)
// Структура заголовка информации об изображении
typedef struct
{
    unsigned int   biSize;          // Размер данного заголовка в байтах
    unsigned int   biWidth;         // Ширина изображения
    unsigned int   biHeight;        // Высота изображения
    unsigned short biPlanes;        // Количество плоскостей на изображении
    unsigned short biBitCount;      // Количество  бит на пиксел
    unsigned int   biCompression;   // Используемый алгоритм сжатия
    unsigned int   biSizeImage;     // Размер изображения без учёта заголовков
    int            biXPelsPerMeter; // Пикселов по горизонтали на метр
    int            biYPelsPerMeter; // Пикселов по вертикали на метр
    unsigned int   biClrUsed;       // Размер цветовой палитры
    unsigned int   biClrImportant;  // Количество "важных" цветов
} BITMAPINFOHEADER;
#pragma pack()

// Структура, представляющая пиксел из трех компонент
typedef struct
{
    byte r; // Красная компонента
    byte g; // Зелёная компонента
    byte b; // Синяя компонента
} PIXEL;

typedef struct {
    float ** kernel;
    float factor;
    float offset;
    unsigned int width;
    unsigned int height;
} CONV_FILTER;

// Струтура, представляющая BMP-файл
typedef struct
{
    BMPFILEHEADER file_header;    // Заголовок файла
    BITMAPINFOHEADER info_header; // Заголовок изображения
    PIXEL** pixels;               // Набор пикселов
} BMPFILE;

CONV_FILTER * create_conv_filter(int, int, float, float);
void sort_pixels_by_components(PIXEL*, size_t);

// Выводит заголовок файла
void debug_file_header(BMPFILE*);
// Выводит заголовок изображения
void debug_info_header(BMPFILE*);
// Выводит значения пикселов на экран
void debug_pixels(BMPFILE*);

// Градиации серого
void effect_greyscale(BMPFILE*);
// Эффект сепии
void effect_sepia(BMPFILE*, unsigned int);
// Инвертирование цветов
void effect_invert(BMPFILE*);
// Применяет фильтр свертки к изображению
void effect_conv_filter(BMPFILE*, CONV_FILTER*);
void effect_oil(BMPFILE*, unsigned int);
void effect_pen(BMPFILE*, unsigned int);

CONV_FILTER * cf_motion_blur(int);
CONV_FILTER * cf_simple_blur(int);
CONV_FILTER * cf_soft_blur(int);
CONV_FILTER * cf_simple_edge_detection();
CONV_FILTER * cf_soft_sharpen();

// Считывает BMP файл в структуру
void read_bmp_file(char*, BMPFILE*);
// Создаёт BMP-файл на основании структуры
void write_bmp_file(BMPFILE*, char*);

inline byte norm_comp(int source) {
    byte result;
    if(source < 0) result = 0;
    else if(source > 255) result = 255;
    else result = source;
    return result;
}

 #endif