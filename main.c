
#include <stdio.h>
#include "io_manager.h"
#include "lzw.h"
#include <stdlib.h>
#include "bmp_process.h"
#include "png_process.h"
#include "steg_manager.h"
//#include <libpng16/png.h>
//#include <png.h>
int exit_value = RUN_OK;

int main(int argc, char *argv[]) {

    char *image = NULL;
    char *payload = NULL;
    char switcher = 0;
    char image_type = 0;

    arg_check(argc, argv, &image, &switcher, &payload, &image_type);

    printf("image: %s\n", image);
    printf("payload: %s\n", payload);
    printf("switcher: %c\n", switcher);
    printf("image_type: %c\n\n", image_type);


    if (exit_value != RUN_OK) {
        return exit_value;
    }

    manage_steg(payload, image, switcher, image_type);


//    free(image);
//    free(payload);



    return exit_value;
}

