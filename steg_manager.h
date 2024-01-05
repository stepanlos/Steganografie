//
// Created by Štěpán on 04.01.2024.
//

#ifndef PROJEKT_STEG_MANAGER_H
#define PROJEKT_STEG_MANAGER_H



typedef struct {
    int *hidden_data;
    int hidden_data_size;
} hidden_content;


hidden_content *hidden_content_create(int data_size);

void hidden_content_free(hidden_content **content);

void hide_data(char *payload, char *image, char file_type);

void unload_data(char *image, char *payload, char file_type);

void manage_steg(char *payload, char *image, char switcher, char file_type);



#endif //PROJEKT_STEG_MANAGER_H
