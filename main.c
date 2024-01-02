#include <stdio.h>
#include <stdlib.h>
#include <libpng16/png.h>
#include "lzw.h"
//#include "dictionary.h"

//int main() {
//    printf("Hello, World!\n");
//
//    int a = png_access_version_number();
//    printf("%d", a);
//    return 0;


//
//
//    Dictionary *d = NULL;
//    d = initialize_dictionary();
//
//    if (d == NULL) {
//        printf("Chyba při inicializaci slovníku.\n");
//        return 1;
//    }
//
//    for (int i = 0; i < 770; i++) {
//        add_to_dictionary(d, (char)i, i);
//        printf("slovnik ma velikost %d\n", d->size);
//    }
//
//    printf("znak A najdeme ve slovniku na pozici %d\n", find_in_dictionary(d, 'A', -1) );
//    printf("znak B najdeme ve slovniku na pozici %d\n", find_in_dictionary(d, 'B', -1) );
//    printf("znak AB najdeme ve slovniku na pozici %d\n", find_in_dictionary(d, 'B', 66) );
//
//
//    printf("uvolnuji slovnik\n");
//    printf("%d\n", free_dictionary(d));
//    printf("velikost slovniku je %d\n", d->size);


int main() {
    char input_array[14] = {'A', 'B', 'C', 'A', 'B', 'C', 'A', 'B', 'C', 'D', 'A', 'B', 'C', '\0'}; //"ABCABCABCDABC"
    int output_array[512];

//    printf("%d\n", input_array[0]);


     int i = compress_lzw(input_array, output_array);
     if (i == 1){
         printf("Chyba při kompresi.\n");
         return 1;
     }


//    for (int i = 0; i < 512; i++) {
//        printf("%d\n", output_array[i]);
//
//    }




    return 0;
}


