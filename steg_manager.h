//
// Created by Štěpán on 04.01.2024.
//

#ifndef PROJEKT_STEG_MANAGER_H
#define PROJEKT_STEG_MANAGER_H

#include "bmp_process.h"
#include "png_process.h"

typedef struct {
    int *hidden_data;
    int hidden_data_size;
} hidden_content;


hidden_content *hiddenContent_create(int data_size);

void hidden_content_free(hidden_content **content);

void hide_data(char *payload, char *image, char file_type);

void extract_data(char *image, char *payload, char file_type);

#endif //PROJEKT_STEG_MANAGER_H
