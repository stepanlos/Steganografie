//
// Created by Štěpán on 28.12.2023.

#include "lzw.h"
#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include "crc_check.h"
#include "io_manager.h"


/**
 * Funkce pro uvolnění komprimovaného pole
 * @param c komprimované pole
 * @return compresed pole, nebo NULL v případě chyby
 */
compressed *compressed_create() {
    compressed *c = (compressed *) malloc(sizeof(compressed));
    if (c == NULL) {
        exit_value = MALLOC_FAIL;
        return NULL;
    }

    c->compressed = (int *) malloc(COMPRESSED_SIZE * sizeof(int));
    if (c->compressed == NULL) {
        free(c);
        exit_value = MALLOC_FAIL;
        return NULL;
    }
    c->current_length = COMPRESSED_SIZE;
    c->last_item = 0;

    return c;
}

/**
 * Funkce pro přidání prvku do komprimovaného pole
 * @param c komprimované pole
 * @param item prvek který se má přidat
 * @return 1 v případě úspěchu, nebo 0 v případě chyby
 */
int compressed_add(compressed *c, int item) {
    if (c->last_item >= c->current_length) {
        int *tmp = realloc(c->compressed, (c->current_length + COMPRESSED_INCREMENT_SIZE) * sizeof(int));
        if (!tmp){
            printf("Chyba při realokaci paměti.\n");
            compressed_free(&c);
            exit_value = MALLOC_FAIL;
            return FALSE;
        }
        c->current_length += COMPRESSED_INCREMENT_SIZE;
        c->compressed = tmp;
    }

    c->compressed[c->last_item] = item;
    c->last_item++;

    return TRUE;
}


/**
 * Funkce pro uvolnění komprimovaného pole
 * @param c komprimované pole
 */
void compressed_free(compressed **c) {
    if (*c != NULL) {
        free((*c)->compressed);
        free(*c);
        *c = NULL;
        printf("Komprimovane pole uvolneno\n");
    }
}


/**
 * Funkce pro vytvoření dekomprimovaného pole
 * @return dekomprimované pole, nebo NULL v případě chyby
 */
decompressed *decompressed_create() {
    decompressed *tmp = (decompressed *) malloc(sizeof(decompressed));
    if (tmp == NULL) {
        return NULL;
        exit_value = MALLOC_FAIL;
    }

    tmp->decompressed = (unsigned char *) malloc(COMPRESSED_SIZE * sizeof(unsigned char));
    if (tmp->decompressed == NULL) {
        free(tmp);
        exit_value = MALLOC_FAIL;
        return NULL;
    }

    tmp->current_length = COMPRESSED_SIZE;
    tmp->last_item = 0;

    return tmp;
}

/**
 * Funkce pro přidání prvku do dekomprimovaného pole
 * @param decomp dekomprimované pole
 * @param c prvek který se má přidat
 * @param c_length délka prvku
 * @return 1 v případě úspěchu, nebo 0 v případě chyby
 */
int decompressed_add(decompressed *decomp, unsigned char *c, int c_length) {
    int i;
    if (decomp->last_item + c_length >= decomp->current_length) {
        unsigned char *tmp = realloc(decomp->decompressed, (decomp->current_length + COMPRESSED_INCREMENT_SIZE) * sizeof(unsigned char));
        if (!tmp){
            printf("Chyba při realokaci paměti.\n");
            decompressed_free(&decomp);
            exit_value = MALLOC_FAIL;
            return FALSE;
        }
        decomp->current_length += COMPRESSED_INCREMENT_SIZE;
        decomp->decompressed = tmp;
    }

    for (i = 0; i < c_length; i++) {
        decomp->decompressed[decomp->last_item] = c[i];
        decomp->last_item++;
    }

    return TRUE;
}

/**
 * Funkce pro uvolnění dekomprimovaného pole
 * @param decomp dekomprimované pole
 */
void decompressed_free(decompressed **decomp) {
    if (!*decomp){
        return;
    }

    free((*decomp)->decompressed);
    free(*decomp);
    *decomp = NULL;
    printf("Dekomprimovane pole uvolneno\n");
}





















compressed *compress_lzw(char *filename){
    int pref, suf, indx;
    int i = 0;

    if (filename == NULL || !*filename){
        return NULL;
    }

    //initialize slovníku
    dictionary *dictionary = NULL;
    dictionary = initialize_dictionary();

    //ošetření chyby při inicializaci slovníku
    if (dictionary == NULL) {
        printf("Chyba při inicializaci slovníku.\n");
        return NULL;

    }

    //inicializace komprimovaného pole
    compressed *c = compressed_create();
    //ošetření chyby při inicializaci komprimovaného pole
    if (c == NULL){
        printf("Chyba při inicializaci komprimovaného pole.\n");
        free_dictionary(&dictionary);
        return NULL;
    }

    printf("Komprimuji soubor %s\n", filename);

    //otevření souboru
    FILE *file = fopen(filename, "rb");
    if (file == NULL){
        printf("Chyba při otevření souboru56.\n");
        compressed_free(&c);
        free_dictionary(&dictionary);
        exit_value = FILE_OPEN_FAIL;
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long input_size = ftell(file);

    if (input_size <= 0){
        printf("Soubor je prázdný.\n");
        compressed_free(&c);
        free_dictionary(&dictionary);
        exit_value = INVALID_ARGS_OR_FILES;
        return NULL;
    }
    fseek(file, 0, SEEK_SET);

    //vytvoření bufferu
    unsigned char *input_buf = (unsigned char *) malloc(input_size * sizeof(unsigned char));
    if (input_buf == NULL){
        printf("Chyba při alokaci paměti.\n");
        compressed_free(&c);
        free_dictionary(&dictionary);
        fclose(file);
        exit_value = MALLOC_FAIL;
        return NULL;
    }

    //načtení souboru do bufferu
    size_t read = fread(input_buf, sizeof(unsigned char), input_size, file);
    if (read != input_size){
        printf("Chyba při čtení ze souboru.\n");
        compressed_free(&c);
        free_dictionary(&dictionary);
        fclose(file);
        free(input_buf);
        exit_value = FILE_READ_FAIL;
        return NULL;
    }

    //signatura
    compressed_add(c, 'K');
    compressed_add(c, 'A');
    compressed_add(c, 'R');
    compressed_add(c, 'I');

    c->last_item ++; //posunutí ukazatele na první volnou pozici (později přidávám délku dat)
    c->last_item ++; //posunutí ukazatele na první volnou pozici (místo pro crc součet)

    printf("zacatek komprese\n");

    pref = input_buf[0];
    i++;

    while (i < input_size){

        suf = (int) input_buf[i];
        //printf("pref: %d, suf: %d\n", pref, suf);
        indx = find_in_dictionary(dictionary, (unsigned char) suf, pref);


        if (indx != -1){
            pref = indx;
        } else {
            add_to_dictionary(dictionary, (char) suf, pref);

            if(!compressed_add(c, pref)){
                printf("Chyba při kompresi.\n");
                compressed_free(&c);
                free_dictionary(&dictionary);
                fclose(file);
                free(input_buf);
                return NULL;
            }


            pref = suf;
        }
        i++;
    }
    compressed_add(c, pref);

    printf("Konec komprese.\n");

    //uložení délky dat
    c->compressed[LENGTH_OF_SIGNATURE - 2] = c->last_item - LENGTH_OF_SIGNATURE;
    
    int *raw_data = (int *) malloc((c->last_item - LENGTH_OF_SIGNATURE) * sizeof(int));
    for (int j = 0; j < c->last_item - LENGTH_OF_SIGNATURE; j++) {
        raw_data[j] = c->compressed[j + LENGTH_OF_SIGNATURE];
//        printf("%d\n", j);
    }
    //vypis raw dat
//    printf("raw data: ");
//    for (int j = 0; j < c->last_item - LENGTH_OF_SIGNATURE; j++) {
//        printf("%d ", raw_data[j]);
//    }

//    printf("\n");


//    printf("c last intem: %d\n", c->last_item);
    int crc = sumr_crc(raw_data,0, c->last_item - LENGTH_OF_SIGNATURE);

    c->compressed[LENGTH_OF_SIGNATURE - 1] = crc;


    free_dictionary(&dictionary);
    fclose(file);
    free(input_buf);
    free(raw_data);
    return c;

}


/**
 * Funkce pro dekompresi
 * @param data komprimovaná data
 * @param start počáteční index
 * @param data_length délka dat
 * @return dekomprimované pole, nebo NULL v případě chyby
 */
decompressed *decompress_lzw(int *data, int start ,int data_length) {
    int oldcd, newcd, i, len = 0;
    unsigned char code, *str;
    str = malloc(sizeof(unsigned char));
    if (str == NULL) {
        printf("Chyba při alokaci paměti.\n");
        exit_value = MALLOC_FAIL;
        return NULL;
    }
    if (data == NULL || !data_length){
        printf("Chyba při dekompresi.\n");
        return NULL;
    }

    //inicializace slovníku
    dictionary *dictionary = NULL;
    dictionary = initialize_dictionary();
    if (dictionary == NULL) {
        printf("Chyba při inicializaci slovníku.\n");
        free(str);
        return NULL;
    }

    //inicializace dekomprimovaného pole
    decompressed *decomp = decompressed_create();
    if (decomp == NULL){
        printf("Chyba při inicializaci dekomprimovaného pole.\n");
        free(str);
        free_dictionary(&dictionary);
        return NULL;
    }

    printf("Dekomprimuji data.\n");
    //dekompresní algoritmus
    oldcd = data[start];
    decompressed_add(decomp, (unsigned char *) &oldcd, 1);
//    printf("datalength: %d\n", data_length);
//    printf("code: %d\n", code);

    for(i = start + 1; i < data_length /*+ LENGTH_OF_SIGNATURE*/; i++){
        newcd = data[i];
        //printf("i: %d -> znak %d \n", i, newcd);
        if (find_in_dictionary_decompress(dictionary, newcd) == -1){
            free(str);
            str = (unsigned char *) string_from_index(dictionary, oldcd, &len);
            if(str == NULL){
                free_dictionary(&dictionary);
                decompressed_free(&decomp);
                return NULL;
            }
            //printf("string from index");

            str = (unsigned char *) realloc(str, (len + 1) * sizeof(unsigned char));
            if(str == NULL){
                free_dictionary(&dictionary);
                decompressed_free(&decomp);
                return NULL;
            }
            str[len] = code;
            len++;

        } else {
            //printf("else");
            free(str);
            str = (unsigned char *) string_from_index(dictionary, newcd, &len);
            //printf("po stringu");
        }
        if (decompressed_add(decomp, str, len) == 0){
            free_dictionary(&dictionary);
            decompressed_free(&decomp);
            return NULL;
        }

        code = str[0];

        //přidání do slovníku
        add_to_dictionary(dictionary, code, oldcd);

        oldcd = newcd;
        len = 0;
        //vypis vsech promennych
//        printf("oldcd: %d\n", oldcd);
//        printf("newcd: %d\n", newcd);
//        printf("code: %d\n", code);
//        printf("len: %d\n", len);
//        printf("str: %s\n", str);
//        printf("i: %d\n", i);
//        printf("data_length: %d\n", data_length);
//        printf("decomp->last_item: %d\n", decomp->last_item);
//        printf("decomp->current_length: %d\n", decomp->current_length);
//        printf("decomp->decompressed: %s\n", decomp->decompressed);
//        printf("dictionary->size: %d\n", dictionary->size);

    }
    free(str);
    free_dictionary(&dictionary);
    return decomp;
}
