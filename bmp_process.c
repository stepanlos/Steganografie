//
// Created by Štěpán on 03.01.2024.
//

#include "bmp_process.h"
#include <stdlib.h>
#include <stdio.h>
#include "io_manager.h"
#include "crc_check.h"




bmp_organised *read_image_bmp(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error while opening file\n");
        exit_value = INVALID_ARGS_OR_FILES;
        return NULL;
    }

    bmp_inf_props *info_properties = malloc(sizeof(bmp_inf_props));
    if (info_properties == NULL) {
        printf("Error while allocating memory for bmp info properties\n");
        exit_value = MALLOC_FAIL;
        fclose(file);
        return NULL;
    }

    bmp_organised *image = malloc(sizeof(bmp_organised));
    if (image == NULL) {
        printf("Error while allocating memory for bmp image\n");
        exit_value = MALLOC_FAIL;
        fclose(file);
        free(info_properties);
        return NULL;
    }

    //načtení hlavičky
    fread(info_properties, sizeof(bmp_inf_props), 1, file);

    if (info_properties->signature != BMP_SIGNATURE) {
        printf("Error while reading bmp file\n");
        exit_value = INVALID_IMAGE_FORMAT;
        fclose(file);
        free(info_properties);
        free(image);
        return NULL;
    }

    if (info_properties->bitDepth != IMAGE_BIT_DEPTH) {
        printf("Error while reading bmp file\n");
        exit_value = INVALID_IMAGE_FORMAT;
        fclose(file);
        free(info_properties);
        free(image);
        return NULL;
    }

    //načtení velikosti pixelů
    int width = abs(info_properties->width);
    int height = abs(info_properties->height);
    image->pixel_data_size = width * height;
    image->pixel_data = (uint8_t *) malloc(image->pixel_data_size * BGR * sizeof(uint8_t));

    if (image->pixel_data == NULL) {
        printf("Error while allocating memory for bmp image\n");
        exit_value = MALLOC_FAIL;
        fclose(file);
        free(info_properties);
        free(image);
        return NULL;
    }

    // načtení pixelů
    for (int i = 0; i < height; i++) {
        fread(image->pixel_data + i * width * BGR, sizeof(uint8_t), width * BGR, file);
        fseek(file, (4 - (3 * width) % 4) % 4, SEEK_CUR);
    }

    if(ferror(file)) {
        printf("Error while reading bmp file\n");
        exit_value = FILE_READ_FAIL;
        fclose(file);
        free(info_properties);
        free(image->pixel_data);
        free(image);
        return NULL;
    }

    image->info_properties = info_properties;
    fclose(file);

    return image;
}


int save_image_bmp(bmp_organised *image, const char *filename) {
    FILE *file = fopen(filename, "wb");

    //kontrola otevření souboru
    if (file == NULL) {
        printf("Error while opening file\n");
        exit_value = INVALID_ARGS_OR_FILES;
        return 0;
    }

    //zapsání hlavičky
    fwrite(image->info_properties, sizeof(bmp_inf_props), 1, file);
    if (ferror(file)) {
        printf("Error while writing bmp file\n");
        exit_value = FILE_WRITE_FAIL;
        fclose(file);
        return 0;
    }

    //zapsání pixelů
    int width = (4 - (3 * abs(image->info_properties->width)) % 4) % 4;

    uint8_t *row = (uint8_t *) malloc(image->info_properties->width * BGR);

    if(row == NULL) {
        printf("Error while allocating memory for bmp image\n");
        exit_value = MALLOC_FAIL;
        fclose(file);
        return 0;
    }


    // zapsání pixelů
    int irow, icol, pos;
    for (irow = 0; irow < abs(image->info_properties->height); irow++) {
        for (icol = 0; icol < image->info_properties->width; icol++) {
            pos = irow * image->info_properties->width + icol;
            row[icol * BGR] = image->pixel_data[pos * BGR];
            row[icol * BGR + 1] = image->pixel_data[pos * BGR + 1];
            row[icol * BGR + 2] = image->pixel_data[pos * BGR + 2];
        }
        fwrite(row, sizeof(uint8_t), image->info_properties->width * BGR, file);
        fwrite("\0", sizeof(uint8_t), width, file);

        if (ferror(file)) {
            printf("Error while writing bmp file\n");
            exit_value = FILE_WRITE_FAIL;
            fclose(file);
            free(row);
            return 0;
        }

    }

    fclose(file);
    free(row);
    return 1;
}


void free_image_bmp(bmp_organised **image) {
    if (image == NULL || *image == NULL) {
        return;
    }

    free((*image)->info_properties);
    free((*image)->pixel_data);
    free(*image);
    *image = NULL;
}

int change_blue_bmp(bmp_organised *image, compressed *data) {
    //velikost signatury a dat v bitech
    int signature_size_b = SIGNATURE_BIT_SIZE;
    int data_size = (data->last_item - SIGNATURE_SIZE) * COMPRESSED_BIT_SIZE + signature_size_b;

    //kontrola velikosti obrázku
    if (data_size > image->pixel_data_size) {
        printf("Error while hiding data in bmp file\n");
        exit_value = IMAGE_TOO_SMALL;
        return 0;
    }

    printf("compresed %d\n", data->compressed[6]);


    int  i;

    //uložení signatury

    for (i = 0; i < signature_size_b * BGR; i += BGR) {
//        printf("Blue: %d =>", image->pixel_data[i]);
//
//        for (int bit = 7; bit >= 0; bit--) {
//            int bit_value = (image->pixel_data[i] >> bit) & 0x1;
//            printf(" %d", bit_value);
//        }
//        printf(" => ");

//        image->pixel_data[i] &= ~(0x01);  // Nulování nejméně významného bitu
        image->pixel_data[i] &= 0xFE;
        image->pixel_data[i] |= (data->compressed[i / (BGR * INT_BIT_SIZE)] >> ((i / BGR) % INT_BIT_SIZE) ) & 0x1;
//        printf(" %d => ", image->pixel_data[i]);

//        for (int bit = 7; bit >= 0; bit--) {
//            int bit_value = (image->pixel_data[i] >> bit) & 0x1;
//            printf(" %d", bit_value);
//        }
//        printf("\n");
    }
    int temp, shift;
    shift = i;


    //uložení dat
    for (i = SIGNATURE_SIZE; i < data->last_item; i++) {
        temp = data->compressed[i];
        temp &= 0xFFF;
        for(int j = 0; j < COMPRESSED_BIT_SIZE * BGR ; j += BGR) {
            image->pixel_data[shift + j] &= 0xFE;
            image->pixel_data[shift + j] |= (temp >> (j / BGR)) & 0x1;
        }
        shift += COMPRESSED_BIT_SIZE * BGR;
    }

    return 1;
}



hidden_content *unload_blue_bmp(bmp_organised *image) {
    int *extracted_data_temp = malloc(SIGNATURE_SIZE * sizeof(int));
    if (extracted_data_temp == NULL) {
        printf("Error while allocating memory for hidden content\n");
        exit_value = MALLOC_FAIL;
        return NULL;
    }
    for (int i = 0; i < SIGNATURE_SIZE; i++) {
        extracted_data_temp[i] = 0;
    }

    //načtení signatury z obrázku do extracted_data_temp
    int i = 0;
    int shift = 0;
    for(; i < SIGNATURE_SIZE; i ++) {
//        extracted_data_temp[i / (3 * 32)] |= (image->pixel_data[i] & 0x1) << ((i / 3) % 32);
        for (int j = 0; j < BGR * INT_BIT_SIZE; j += BGR) {
            extracted_data_temp[i] |= (image->pixel_data[shift + j] & 0x1) << (j / BGR);
        }
        shift += BGR * INT_BIT_SIZE;
    }

    //výpis extracted_data_temp
    for (int j = 0; j < SIGNATURE_SIZE; j++) {
        printf("%d ",  extracted_data_temp[j]);
    }

    //kontrola hlavičky, zda je na prvních 4 místech "KARI"
    if (extracted_data_temp[0] != 75 || extracted_data_temp[1] != 65 || extracted_data_temp[2] != 82 || extracted_data_temp[3] != 73) {
        printf("Error while extracting data from bmp file\n");
        exit_value = NO_HIDDEN_DATA;
        free(extracted_data_temp);
        return NULL;
    }


    hidden_content *hidden = hidden_content_create(extracted_data_temp[4] + SIGNATURE_SIZE);
    printf("hidden->hidden_data_size: %d\n", hidden->hidden_data_size);

    //načtení dat z extracted_data_temp do hidden_content
    for (int j = 0; j < SIGNATURE_SIZE; j++) {
        hidden->hidden_data[j] = extracted_data_temp[j];
    }

//    printf("vypis hidden->hidden_data:\n");
//    for (int j = 0; j < hidden->hidden_data_size; j++) {
//        printf("%d ", hidden->hidden_data[j]);
//    }

    free(extracted_data_temp);
    printf("shift = %d\n", shift);
    printf("i = %d\n", i);
    //načtení dat z obrázku do hidden_content
    for(; i < hidden->hidden_data_size; i ++) {
        for (int j = 0; j < BGR * COMPRESSED_BIT_SIZE; j += BGR) {
            hidden->hidden_data[i] |= (image->pixel_data[shift + j] & 0x1) << (j / BGR);
        }
        shift += BGR * COMPRESSED_BIT_SIZE;
    }
    printf("i = %d\n", i);


    int *raw_data = (int *) malloc((hidden->hidden_data_size - SIGNATURE_SIZE) * sizeof(int));
    for (int j = 6; j < hidden->hidden_data_size; j++) {
        raw_data[j - SIGNATURE_SIZE] = hidden->hidden_data[j];
    }

    printf("hidden data size: %d\n", hidden->hidden_data_size);
    int crc_data = hidden->hidden_data[5];
    hidden->hidden_data[5] = 0;

    //kontrola crc
    int crc_cntl = sumr_crc(raw_data, 0, hidden->hidden_data_size - SIGNATURE_SIZE);
    printf("crc_cntl: %d\n", crc_cntl);


    printf("crc_data: %d\n", crc_data);
    if(crc_cntl != crc_data) {
        printf("Error while extracting data from png file\n");
        exit_value = DAMAGED_HIDDEN_CONTENT;
        hidden_content_free(&hidden);
        return NULL;
    }

    free(raw_data);





    return hidden;
}


