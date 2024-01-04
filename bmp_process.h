//
// Created by Štěpán on 03.01.2024.
//

#ifndef PROJEKT_BMP_PROCESS_H
#define PROJEKT_BMP_PROCESS_H

#include <stdint.h>
#include "lzw.h"
#include "steg_manager.h"

#define BMP_SIGNATURE 0x4D42
#define IMAGE_BIT_DEPTH 24
#define BGR 3



#define SIGNATURE_BIT_SIZE 192
#define SIGNATURE_SIZE 6
#define COMPRESSED_BIT_SIZE 12
#define INT_BIT_SIZE 32

#pragma pack(push, 1)

/** Struktura pro hlavičku BMP souboru */
typedef struct {
    uint16_t signature;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t dataOffset;
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitDepth;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t colorsImportant;
} bmp_inf_props;



/** Struktura pro uložení BMP obrázku */
typedef struct {
    bmp_inf_props *info_properties;
    uint8_t *pixel_data;
    int pixel_data_size;
} bmp_organised;



bmp_organised *read_image_bmp(const char *filename);

int save_image_bmp(bmp_organised *image, const char *filename);

void free_image_bmp(bmp_organised **image);

int change_blue_bmp(bmp_organised *image, compressed *data);

hidden_content *unload_blue_bmp(bmp_organised *image);











#pragma pack(pop)

#endif //PROJEKT_BMP_PROCESS_H
