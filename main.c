
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

    //TODO FREe


    //********************************ARGCHECK********************************

//    compressed *result_comp = compress_lzw(payload);
//    if (!result_comp) {
//        printf("ERROR: Compression failed!\n");
//        return 1;
//    }
//    printf("delka dat: %d\n", result_comp->compressed[4]);
//
//
//
//    bmp_organised *image_png = read_image_bmp(image);
//    if (!image_png) {
//        printf("ERROR: Image read failed!\n");
//        return 1;
//    }
//    change_blue_bmp(image_png, result_comp);
//
//    save_image_bmp(image_png, "output.bmp");
//
//    bmp_organised  *image_png2 = read_image_bmp("output.bmp");
//
//    hidden_content *hic_c  = unload_blue_bmp(image_png2);
////
//////    for (int i = 0; i < hic_c->hidden_data_size; i++) {
//////        printf("%d ", hic_c->hidden_data[i]);
//////    }
////
//    decompressed *decomp = decompress_lzw(hic_c->hidden_data, LENGTH_OF_SIGNATURE, hic_c->hidden_data_size);
////
//    printf("dekomprese dokoncena \n");
//    FILE *f = fopen("vystup.txt", "wb");
//    if (!f) {
//        printf("ERROR: File open failed!\n");
//        return 1;
//    }
//    fwrite(decomp->decompressed, sizeof(char), decomp->last_item, f);
//    fclose(f);







    //********************************KOMPRESE********************************
//    compressed *result_comp = compress_lzw(payload);
//    if (!result_comp) {
//        printf("ERROR: Compression failed!\n");
//        return 1;
//    }
//    printf("delka dat: %d\n", result_comp->compressed[4]);
//
//
//
//    png_organised *image_png = read_image_png(image);
//    if (!image_png) {
//        printf("ERROR: Image read failed!\n");
//        return 1;
//    }
//    change_blue_png(image_png, result_comp);
//
//    save_image_png(image_png, "output.png");
//
//    png_organised  *image_png2 = read_image_png("output.png");
//
//    hidden_content *hic_c  = unload_blue_png(image_png2);
////
//////    for (int i = 0; i < hic_c->hidden_data_size; i++) {
//////        printf("%d ", hic_c->hidden_data[i]);
//////    }
////
//    decompressed *decomp = decompress_lzw(hic_c->hidden_data, LENGTH_OF_SIGNATURE, hic_c->hidden_data_size);
////
//    printf("dekomprese dokoncena \n");
//    FILE *f = fopen("vystup.txt", "wb");
//    if (!f) {
//        printf("ERROR: File open failed!\n");
//        return 1;
//    }
//    fwrite(decomp->decompressed, sizeof(char), decomp->last_item, f);
//    fclose(f);
//    printf("pamet uvolnena\n");








   // printf("délka dat: %d\n", result_comp->last_item);

//   // printf("pro dekompresi musim delku %d \n", result_comp->compressed[5 - 1]);
//    decompressed *result2 = decompress_lzw(result_comp->compressed, LENGTH_OF_SIGNATURE,result_comp->compressed[LENGTH_OF_SIGNATURE - 2]);
//  //  printf("dekompresovana delka %d\n" ,result_comp->compressed[LENGTH_OF_SIGNATURE - 2]);
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
//    compressed_free(&result_comp);



//
//    free(image);
//    free(payload);



    return exit_value;
}

