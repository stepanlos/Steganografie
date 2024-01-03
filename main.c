//#include <stdio.h>
//#include <stdlib.h>
//#include <libpng16/png.h>
//#include "lzw.h"
////#include "dictionary.h"
//
////int main() {
////    printf("Hello, World!\n");
////
////    int a = png_access_version_number();
////    printf("%d", a);
////    return 0;
//
//
////
////
////    Dictionary *d = NULL;
////    d = initialize_dictionary();
////
////    if (d == NULL) {
////        printf("Chyba při inicializaci slovníku.\n");
////        return 1;
////    }
////
////    for (int i = 0; i < 770; i++) {
////        add_to_dictionary(d, (char)i, i);
////        printf("slovnik ma velikost %d\n", d->size);
////    }
////
////    printf("znak A najdeme ve slovniku na pozici %d\n", find_in_dictionary(d, 'A', -1) );
////    printf("znak B najdeme ve slovniku na pozici %d\n", find_in_dictionary(d, 'B', -1) );
////    printf("znak AB najdeme ve slovniku na pozici %d\n", find_in_dictionary(d, 'B', 66) );
////
////
////    printf("uvolnuji slovnik\n");
////    printf("%d\n", free_dictionary(d));
////    printf("velikost slovniku je %d\n", d->size);
//
//
//int main() {
//    char input_array[14] = {'A', 'B', 'C', 'A', 'B', 'C', 'A', 'B', 'C', 'D', 'A', 'B', 'C', '\0'}; //"ABCABCABCDABC"
//    int output_array[512];
//
////    printf("%d\n", input_array[0]);
//
//
//     int i = compress_lzw(input_array, output_array);
//     if (i == 1){
//         printf("Chyba při kompresi.\n");
//         return 1;
//     }
//
//
////    for (int i = 0; i < 512; i++) {
////        printf("%d\n", output_array[i]);
////
////    }
//
//
//
//
//    return 0;
//}
//
//
#include <stdio.h>
#include <stdlib.h>
//#include "arg_cont.h"
#include "lzw.h"
//#include <libpng16/png.h>
//#include <png.h>
int main(int argc, char *argv[]) {

//    char *switcher = NULL, *imageFile = NULL, *payloadFile = NULL, imageType = 0;
//    int i = 0;
//
//
//    control_arguments(argc, argv, &switcher, &imageFile, &payloadFile, &imageType);
//    printf("%s \n",switcher);
//    printf("%s \n",imageFile);
//    printf("%s \n",payloadFile);

//    int a = png_access_version_number();
//    printf("%d \n", a);


//    int a;
//    a = png_access_version_number();
//    printf("%d", a);

    // strcmp atp -> memcmp,...

    /* MASKOVANI */
//    int test = 50000;
//
//    test &= 0xFFF;   // bitova operace AND -> vyuzije jen 12 bitÅ¯ z int
//    char temp_test= test&0x1;// ziskani bitu zleva
//    temp_test= (test >> 1)&0x1; // posunuti o 1 bit doprava a ziskani bitu zleva
//
//    for (int i = 0; i < 12; i++) {
//        printf("%d", (test >> i) & 0x1);
//    }
    /* MASKOVANI*/

//    result = lzw_compress("ABCABCABCDABC", result);

    // todo zjistit, jestli je ok ve slovniku pouzivat unsigned char misto char
    compressed *result = compress_lzw(argv[1]);
    if (!result) {
        printf("ERROR: Compression failed!\n");
        return 1;
    }

//    for (int j = 0; j < result->last_item; ++j) {
//        printf("%d \n", result->compressed[j]);
//    }
//    compressed_free(&result);

    printf("délka dat: %d\n", result->last_item);

    printf("pro dekompresi musim delku %d \n", result->compressed[5 - 1]);
    decompressed *result2 = decompress_lzw(result->compressed, LENGTH_OF_SIGNATURE,result->compressed[LENGTH_OF_SIGNATURE - 2]);
    printf("dekompresovana delka %d\n" ,result->compressed[LENGTH_OF_SIGNATURE - 2]);
    if (!result2) {
        printf("ERROR: Decompression failed!\n");
        return 1;
    }
////    for (int j = 0; j < result2->last; ++j) {
////        printf("%c \n", result2->decoded[j]);
////    }
//
    FILE *f = fopen("compressed.pdf", "wb");
    if (!f) {
        printf("ERROR: File open failed!\n");
        return 1;
    }
    printf("Compressed: ");
    fwrite(result2->decompressed, sizeof(char), result2->last_item, f);
    fclose(f);
//
//    FILE * f2 = fopen("decompressed.bmp", "wb");
//    for (int j = 0; j < result2->last_item; j++) {
//        fprintf(f2, "%c", result2->decompressed[j]);
//    }
////
////    fclose(f);
//    fclose(f2);
////    printf("\n");
//    compressed_free(&result);
//    decompressed_free(&result2);


    return 0;
}