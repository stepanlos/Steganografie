//
// Created by Štěpán on 04.01.2024.
//

#include "steg_manager.h"
#include <stdlib.h>
#include <stdio.h>
#include "io_manager.h"
#include "lzw.h"
#include "bmp_process.h"
#include "png_process.h"


hidden_content *hidden_content_create(int data_size) {
    hidden_content *content = malloc(sizeof(hidden_content));
    if (content == NULL) {
        printf("Error while allocating memory for hidden content\n");
        exit_value = MALLOC_FAIL;
        return NULL;
    }



    content->hidden_data_size = data_size;
    content->hidden_data = malloc(data_size * sizeof(int));
    if (content->hidden_data == NULL) {
        free(content);
        printf("Error while allocating memory for hidden content\n");
        exit_value = MALLOC_FAIL;
        return NULL;
    }
    for (int i = 0; i < data_size; ++i) {
        content->hidden_data[i] = 0;
    }

    return content;
}


void hidden_content_free(hidden_content **content) {
    if (content == NULL || *content == NULL) {
        return;
    }

    free((*content)->hidden_data);
    free(*content);
    *content = NULL;
}

void hide_data (char *payload, char *image, char file_type){
    compressed *result_comp = compress_lzw(payload);
    if (!result_comp) {
        printf("ERROR: Compression failed!\n");
        return;
    }

    if (file_type == 'p') {
        png_organised *image_png = read_image_png(image);
        if (image_png == NULL) {
            printf("ERROR: Image read failed!\n");
            compressed_free(&result_comp);
            return;
        }
        if (change_blue_png(image_png, result_comp) == 0) {
            printf("ERROR: Image read failed!\n");
            compressed_free(&result_comp);
            free_image_png(&image_png);
            return;
        }
        if (save_image_png(image_png, image) == 0) {
            printf("ERROR: Image read failed!\n");
            compressed_free(&result_comp);
            free_image_png(&image_png);
            return;
        }
        free_image_png(&image_png);


    } else if (file_type == 'b') {
        bmp_organised *image_bmp = read_image_bmp(image);
        if (image_bmp == NULL) {
//            printf("ERROR: Image read failed!\n");
            compressed_free(&result_comp);
            return;
        }
        if (change_blue_bmp(image_bmp, result_comp) == 0) {
            printf("ERROR: Image read failed!\n");
            compressed_free(&result_comp);
            free_image_bmp(&image_bmp);
            return;
        }
        if (save_image_bmp(image_bmp, image) == 0) {
            printf("ERROR: Image read failed!\n");
            compressed_free(&result_comp);
            free_image_bmp(&image_bmp);
            return;
        }
        free_image_bmp(&image_bmp);


    }

    compressed_free(&result_comp);
}


void unload_data(char *image, char *payload, char file_type){
    hidden_content *hid_c = NULL;

    if (file_type == 'p') {
        png_organised *image_png = read_image_png(image);
        if (image_png == NULL) {
            printf("ERROR: Image read failed!\n");
            return;
        }
        hid_c = unload_blue_png(image_png);
        if (hid_c == NULL) {
            printf("ERROR: Image read failed!\n");
            free_image_png(&image_png);
            return;
        }
        free_image_png(&image_png);
    } else if (file_type == 'b') {
        bmp_organised *image_bmp = read_image_bmp(image);
        if (image_bmp == NULL) {
            printf("ERROR: Image read failed!\n");
            return;
        }
        hid_c = unload_blue_bmp(image_bmp);
        if (hid_c == NULL) {
            printf("ERROR: Image read failed!\n");
            free_image_bmp(&image_bmp);
            return;
        }
        free_image_bmp(&image_bmp);
    }

    if (hid_c == NULL) {
        printf("ERROR: Image read failed!\n");
        exit_value = INVALID_ARGS_OR_FILES;
        return;
    }
    decompressed *decomp = decompress_lzw(hid_c->hidden_data, LENGTH_OF_SIGNATURE, hid_c->hidden_data_size);
    if (decomp == NULL) {
        printf("ERROR: Image read failed!\n");
        hidden_content_free(&hid_c);
        return;
    }
    FILE *f = fopen(payload, "wb");
    if (f == NULL) {
        printf("ERROR: File open failed!\n");
        exit_value = FILE_WRITE_FAIL;
        hidden_content_free(&hid_c);
        decompressed_free(&decomp);
        return;
    }

    fwrite(decomp->decompressed, sizeof(char), decomp->last_item, f);
    fclose(f);
    hidden_content_free(&hid_c);
    decompressed_free(&decomp);





}


void manage_steg(char *payload, char *image, char switcher, char file_type) {
    if (switcher == 'h') {
        hide_data(payload, image, file_type);
    } else if (switcher == 'x') {
        unload_data(image, payload, file_type);
    }

}


