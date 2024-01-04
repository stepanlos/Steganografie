//
// Created by Štěpán on 03.01.2024.
//

#ifndef PROJEKT_PNG_PROCESS_H
#define PROJEKT_PNG_PROCESS_H

//#include "lzw.h"
//#include "steg_manager.h"
#include "png.h"
#include "lzw.h"
#include "steg_manager.h"

#define RGB 3
#define SIGNATURE_BIT_SIZE_P 192
#define SIGNATURE_SIZE_P 6
#define COMPRESSED_BIT_SIZE_P 12
#define INT_BIT_SIZE_P 32

#define FALSE 0
#define EXIT_MEMORY_ALLOCATION 6
#define EXIT_FILE_NOT_FOUND 2
#define EXIT_FILE_READING 3

//#pragma pack(push, 1)




typedef struct {
    int width;
    int height;
    png_bytep row_pointers;
} png_organised;

png_organised *read_image_png(const char *filename);

int save_image_png(png_organised *image, const char *filename);

void free_image_png(png_organised **image);

int change_blue_png(png_organised *image, compressed *compressed_data);

hidden_content *unload_blue_png(png_organised *image);




//#pragma pack(pop)


#endif //PROJEKT_PNG_PROCESS_H
