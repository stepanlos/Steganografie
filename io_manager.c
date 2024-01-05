//
// Created by Štěpán on 03.01.2024.
//
#include "io_manager.h"
#include <stdio.h>
#include "string.h"
#include <stdlib.h>
#include <unistd.h>


#define ARG_COUNT 4


/**
 * Funkce pro zjištění pozice přepínače mezi argumenty
 * přepínač buď "-x" nebo "-h"
 * @param argc počet argumentů
 * @param argv pole argumentů
 * @return pozice přepínače, -1 pokud přepínač není nalezen
 */
int switcher_pos(int argc, char *argv[], char *switcher) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'x' || argv[i][1] == 'h') {
                *switcher = argv[i][1];
                return i;
            }
        }
    }
    return -1;

}


void arg_check(int argc, char *argv[], char **image, char *switcher, char **payload, char *image_type) {
    if (argc != ARG_COUNT) {
        printf("ERROR: Invalid number of arguments!\n");
        exit_value = INVALID_ARGS_OR_FILES;
        return;
    }

    // Zjištění pozice přepínače a přiřazení cest k souborům
    int switcher_position = switcher_pos(argc, argv, switcher);
    if (switcher_position == -1) {
        printf("ERROR: Invalid switcher!\n");
        exit_value = INVALID_ARGS_OR_FILES;
        return;
    } else if (switcher_position == 1) {
        *image = argv[2];
        *payload = argv[3];
    } else if (switcher_position == 2) {
        *image = argv[1];
        *payload = argv[3];
    } else if (switcher_position == 3) {
        *image = argv[1];
        *payload = argv[2];
    } else {
        printf("ERROR: Invalid switcher position!\n");
        exit_value = INVALID_ARGS_OR_FILES;
        return;
    }

    // Zjištění typu obrázku
    int image_len = strlen(*image);
    if (image_len < 5) {
        printf("ERROR: Invalid image format!\n");
        exit_value = INVALID_IMAGE_FORMAT;
        return;
    }
    if ((*image)[image_len - DOT_POS] == '.') {
        if ((*image)[image_len - 3] == 'p' && (*image)[image_len - 2] == 'n' && (*image)[image_len - 1] == 'g') {
            *image_type = 'p';
        } else if ((*image)[image_len - 3] == 'b' && (*image)[image_len - 2] == 'm' && (*image)[image_len - 1] == 'p') {
            *image_type = 'b';
        } else {
            printf("ERROR: Invalid image format!\n");
            exit_value = INVALID_IMAGE_FORMAT;
            return;
        }
    } else {
        printf("ERROR: Invalid image format!\n");
        exit_value = INVALID_IMAGE_FORMAT;
        return;
    }



    // ověření existence souborů
    if (access(*image, F_OK) == -1) {
        printf("ERROR: Image file does not exist!\n");
        exit_value = INVALID_ARGS_OR_FILES;
        return;
    }
    if (*switcher == 'h' && access(*payload, F_OK) == -1) {
        printf("ERROR: Payload file does not exist!\n");
        exit_value = INVALID_ARGS_OR_FILES;
        return;
    }


}







