
#include <stdio.h>
#include "io_manager.h"
#include "lzw.h"
#include <stdlib.h>
#include "bmp_process.h"
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



    //********************************ARGCHECK********************************









    //********************************KOMPRESE********************************
    compressed *result = compress_lzw(payload);
    if (!result) {
        printf("ERROR: Compression failed!\n");
        return 1;
    }
    printf("delka dat: %d\n", result->compressed[4]);



    bmp_organised *image_bmp = read_image_bmp(image);
    if (!image_bmp) {
        printf("ERROR: Image read failed!\n");
        return 1;
    }
    change_blue_bmp(image_bmp, result);

    save_image_bmp(image_bmp, "output.bmp");

    bmp_organised  *image_bmp2 = read_image_bmp("output.bmp");

    hidden_content *hic_c  = unload_blue_bmp(image_bmp2);

//    for (int i = 0; i < hic_c->hidden_data_size; i++) {
//        printf("%d ", hic_c->hidden_data[i]);
//    }

    decompressed *decomp = decompress_lzw(hic_c->hidden_data, LENGTH_OF_SIGNATURE, hic_c->hidden_data_size);

    printf("dekomprese dokoncena \n");
    FILE *f = fopen("vystup.bmp", "wb");
    if (!f) {
        printf("ERROR: File open failed!\n");
        return 1;
    }
    fwrite(decomp->decompressed, sizeof(char), decomp->last_item, f);
    fclose(f);
    printf("pamet uvolnena\n");

   // printf("délka dat: %d\n", result->last_item);

//   // printf("pro dekompresi musim delku %d \n", result->compressed[5 - 1]);
//    decompressed *result2 = decompress_lzw(result->compressed, LENGTH_OF_SIGNATURE,result->compressed[LENGTH_OF_SIGNATURE - 2]);
//  //  printf("dekompresovana delka %d\n" ,result->compressed[LENGTH_OF_SIGNATURE - 2]);
//    if (!result2) {
//        printf("ERROR: Decompression failed!\n");
//        return 1;
//    }
//    printf("dekomprese dokoncena \n");
//
//
//    FILE *f = fopen(payload, "wb");
//    if (!f) {
//        printf("ERROR: File open failed!\n");
//        return 1;
//    }
//    fwrite(result2->decompressed, sizeof(char), result2->last_item, f);
//    fclose(f);
//    printf("pamet uvolnena\n");
//
//
//    decompressed_free(&result2);
//    compressed_free(&result);



//
//    free(image);
//    free(payload);



    return exit_value;
}

