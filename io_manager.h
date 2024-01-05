//
// Created by Štěpán on 03.01.2024.
//

#ifndef PROJEKT_IO_MANAGER_H
#define PROJEKT_IO_MANAGER_H

#define RUN_OK 0
#define INVALID_ARGS_OR_FILES 1
#define INVALID_IMAGE_FORMAT 2
#define IMAGE_TOO_SMALL 3
#define NO_HIDDEN_DATA 4
#define DAMAGED_HIDDEN_CONTENT 5
#define MALLOC_FAIL 6
#define FILE_OPEN_FAIL 7
#define FILE_READ_FAIL 8
#define FILE_WRITE_FAIL 9

#define DOT_POS 4


extern int exit_value;

/**
 * Funkce pro kontrolu argumentů
 * @param argc počet argumentů
 * @param argv pole argumentů
 * @param image ukazatel na cestu k obrázku
 * @param switcher ukazatel na přepínač
 * @param payload ukazatel na cestu k payloadu
 */
void arg_check(int argc, char *argv[], char **image, char *switcher, char **payload, char *image_type);





#endif //PROJEKT_IO_MANAGER_H
