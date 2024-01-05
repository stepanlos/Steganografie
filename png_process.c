//
// Created by Štěpán on 03.01.2024.
//
#include "png_process.h"
#include <stdlib.h>
#include <stdio.h>
#include "io_manager.h"
#include "png.h"
#include "steg_manager.h"
#include "string.h"
#include "crc_check.h"




png_organised *read_image_png(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error while opening file\n");
        exit_value = INVALID_ARGS_OR_FILES;
        return NULL;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png == NULL) {
        printf("Error while creating png struct\n");
        exit_value = MALLOC_FAIL;
        fclose(file);
        return NULL;
    }

    png_infop info = png_create_info_struct(png);
    if (info == NULL) {
        printf("Error while creating png info struct\n");
        exit_value = MALLOC_FAIL;
        fclose(file);
        png_destroy_read_struct(&png, NULL, NULL);
        return NULL;
    }

    if (setjmp(png_jmpbuf(png))) {
        printf("Error while setting png jump buffer\n");
        exit_value = INVALID_IMAGE_FORMAT;
        fclose(file);
        png_destroy_read_struct(&png, &info, NULL);
        return NULL;
    }

    png_init_io(png, file);

    png_read_info(png, info);

    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    if (color_type != PNG_COLOR_TYPE_RGB || bit_depth != CHAR_SIZE) {
        printf("Error while reading png file\n");
        exit_value = INVALID_IMAGE_FORMAT;
        fclose(file);
        png_destroy_read_struct(&png, &info, NULL);
        return NULL;
    }

    png_bytep row_pointers = malloc(sizeof(png_bytep) * png_get_rowbytes(png, info) * png_get_image_height(png, info));
    if (row_pointers == NULL) {
        printf("Error while allocating memory for png image\n");
        exit_value = MALLOC_FAIL;
        fclose(file);
        png_destroy_read_struct(&png, &info, NULL);
        return NULL;
    }


    int width =(int) png_get_image_width(png, info);
    int rowbytes = (int) png_get_rowbytes(png, info);
    for (int i = 0; i < png_get_image_height(png, info); i++) {
        png_read_row(png, row_pointers + i * rowbytes, NULL);
    }

    png_organised *image = malloc(sizeof(png_organised));
    if (image == NULL) {
        printf("Error while allocating memory for png image\n");
        exit_value = MALLOC_FAIL;
        fclose(file);
        png_destroy_read_struct(&png, &info, NULL);
        free(row_pointers);
        return NULL;
    }
    int height = (int) png_get_image_height(png, info);


    image->width = width;
    image->height = height;
    image->row_pointers = row_pointers;

    fclose(file);
    png_destroy_read_struct(&png, &info, NULL);

    return image;
}



int save_image_png(png_organised *image, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error while opening file\n");
        exit_value = INVALID_ARGS_OR_FILES;
        return FALSE;
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png == NULL) {
        printf("Error while creating png struct\n");
        exit_value = MALLOC_FAIL;
        fclose(file);
        return FALSE;
    }

    png_infop info = png_create_info_struct(png);
    if (info == NULL) {
        printf("Error while creating png info struct\n");
        exit_value = MALLOC_FAIL;
        fclose(file);
        png_destroy_write_struct(&png, NULL);
        return FALSE;
    }

    if (setjmp(png_jmpbuf(png))) {
        printf("Error while setting png jump buffer\n");
        exit_value = INVALID_IMAGE_FORMAT;
        fclose(file);
        png_destroy_write_struct(&png, &info);
        return FALSE;
    }

    png_init_io(png, file);

    png_set_IHDR(png, info, image->width, image->height, 8, PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png, info);

    for (int i = 0; i < image->height; i++) {
        png_write_row(png, image->row_pointers + i * image->width * RGB);
    }

    png_write_end(png, NULL);

    fclose(file);
    png_destroy_write_struct(&png, &info);

    return TRUE;
}


void free_image_png(png_organised **image) {
    if (image == NULL || *image == NULL) {
        return;
    }

    free((*image)->row_pointers);
    free(*image);
    *image = NULL;
}


int change_blue_png(png_organised *image, compressed *compressed_data) {
    int signature_size_b = SIGNATURE_BIT_SIZE_P;
    int data_size = (compressed_data->last_item - SIGNATURE_SIZE_P) * COMPRESSED_BIT_SIZE_P + signature_size_b;

    if (data_size > image->width * image->height) {
        printf("Error: Image is too small for this data\n");
        exit_value = IMAGE_TOO_SMALL;
        return FALSE;
    }

//    //vypsání compressed_data
//    for (int i = 0; i < compressed_data->last_item; i++) {
//        printf("%d ", compressed_data->compressed[i]);
//    }

    //zapsání signatury
    int i;
    for(i = BLUE_SHIFT_RGB; i < signature_size_b * RGB; i += RGB) {
//        printf("Blue: %d =>", image->row_pointers[i]);
//
//        for (int bit = 7; bit >= 0; bit--) {
//            int bit_value = (image->row_pointers[i] >> bit) & 0x1;
//            printf(" %d", bit_value);
//        }
//        printf(" => ");

        image->row_pointers[i] &= 0xFE;
        image->row_pointers[i] |= (compressed_data->compressed[i / (RGB * INT_BIT_SIZE_P)] >> ((i / RGB) % INT_BIT_SIZE_P)) & 0x1;

//        printf(" %d => ", image->row_pointers[i]);

//        for (int bit = 7; bit >= 0; bit--) {
//            int bit_value = (image->row_pointers[i] >> bit) & 0x1;
//            printf(" %d", bit_value);
//        }
//        printf("\n");

    }

    int temp, shift;
    shift = i;

    //zapsání dat
    for (i = SIGNATURE_SIZE_P; i < compressed_data->last_item; i++) {
        temp = compressed_data->compressed[i];
        temp &= 0xFFF;

//        printf("temp: %d => \n", temp);
        for (int j = 0; j < COMPRESSED_BIT_SIZE_P * RGB; j += RGB) {
//            printf("Blue: %d =>", image->row_pointers[shift + j]);
            image->row_pointers[shift + j] &= 0xFE;
            image->row_pointers[shift + j] |= (temp >> (j / RGB)) & 0x1;
//            printf(" %d \n", image->row_pointers[shift + j]);
        }


        shift += COMPRESSED_BIT_SIZE_P * RGB;
    }

    return TRUE;
}


hidden_content *unload_blue_png(png_organised *image) {
    int *extracted_data_temp = malloc(SIGNATURE_SIZE_P * sizeof(int));
    if (extracted_data_temp == NULL) {
        printf("Error while allocating memory for extracted data\n");
        exit_value = MALLOC_FAIL;
        return NULL;
    }

    for (int i = 0; i < SIGNATURE_SIZE_P; i++) {
        extracted_data_temp[i] = 0;
    }

    //načtení signatury z obrázku do extracted_data_temp
    int i = 0;
    int shift = BLUE_SHIFT_RGB;
    for(; i < SIGNATURE_SIZE_P; i ++) {
        for (int j = 0; j < RGB * INT_BIT_SIZE_P; j += RGB) {
            extracted_data_temp[i] |= (image->row_pointers[shift + j] & 0x1) << (j / RGB);
        }
        shift += RGB * INT_BIT_SIZE_P;
    }
//
//    //výpis extracted_data_temp
//    for (int j = 0; j < SIGNATURE_SIZE_P; j++) {
//        printf("%d ",  extracted_data_temp[j]);
//    }
    //kontrola hlavičky, zda je na prvních 4 místech "KARI"
    if (extracted_data_temp[0] != CHAR_K || extracted_data_temp[1] != CHAR_A || extracted_data_temp[2] != CHAR_R || extracted_data_temp[3] != CHAR_I) {
        printf("Error: Invalid signature\n");
        exit_value = NO_HIDDEN_DATA;
        free(extracted_data_temp);
        return NULL;
    }



    hidden_content *hidden = hidden_content_create(extracted_data_temp[4] + SIGNATURE_SIZE_P);
//    printf("hidden->hidden_data_size: %d\n", hidden->hidden_data_size);

    //načtení dat z extracted_data_temp do hidden_content
    for (int j = 0; j < SIGNATURE_SIZE_P; j++) {
        hidden->hidden_data[j] = extracted_data_temp[j];
    }

    free(extracted_data_temp);
//    printf("shift = %d\n", shift);
//    printf("i = %d\n", i);
    //načtení dat z obrázku do hidden_content
    for(; i < hidden->hidden_data_size; i++) {
        for (int j = 0; j < RGB * COMPRESSED_BIT_SIZE_P; j += RGB) {
            hidden->hidden_data[i] |= (image->row_pointers[shift + j] & 0x1) << (j / RGB);
        }
        shift += RGB * COMPRESSED_BIT_SIZE_P;
    }

//    //výpis hidden->hidden_data
//    for (int j = 6; j < hidden->hidden_data_size; j++) {
//        printf("%d ", hidden->hidden_data[j]);
//    }

    int *raw_data = (int *) malloc((hidden->hidden_data_size - SIGNATURE_SIZE_P) * sizeof(int));
    for (int j = 6; j < hidden->hidden_data_size; j++) {
        raw_data[j - SIGNATURE_SIZE_P] = hidden->hidden_data[j];
    }

//    printf("hidden data size: %d\n", hidden->hidden_data_size);
    int crc_data = hidden->hidden_data[CRC_POSITON];
    hidden->hidden_data[CRC_POSITON] = 0;

    //kontrola crc
    int crc_cntl = sumr_crc(raw_data, 0, hidden->hidden_data_size - SIGNATURE_SIZE_P);
//    printf("crc_cntl: %d\n", crc_cntl);


//    printf("crc_data: %d\n", crc_data);
    if(crc_cntl != crc_data) {
        printf("Error while extracting data from png file\n");
        exit_value = DAMAGED_HIDDEN_CONTENT;
        hidden_content_free(&hidden);
        return NULL;
    }

    free(raw_data);

    return hidden;
}


