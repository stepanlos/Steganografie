//
// Created by Štěpán on 28.12.2023.

#include "lzw.h"
#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Funkce pro uvolnění komprimovaného pole
 * @param c komprimované pole
 * @return compresed pole, nebo NULL v případě chyby
 */
compressed *compressed_create() {
    compressed *c = (compressed *) malloc(sizeof(compressed));
    if (c == NULL) {
        return NULL;
    }

    c->compressed = (int *) malloc(COMPRESSED_SIZE * sizeof(int));
    if (c->compressed == NULL) {
        free(c);
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
    }

    tmp->decompressed = (unsigned char *) malloc(COMPRESSED_SIZE * sizeof(unsigned char));
    if (tmp->decompressed == NULL) {
        free(tmp);
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
            return FALSE;
        }
        decomp->current_length += COMPRESSED_INCREMENT_SIZE;
        decomp->decompressed = tmp;
    }



















int compress_lzw(char input_array[], int output_array[]){
    Dictionary *dictionary = NULL;
    dictionary = initialize_dictionary();


    if (dictionary == NULL) {
        printf("Chyba při inicializaci slovníku.\n");
        return 1;
    }

    int input_size = 15;
    int pref, suf;
    int i = 0,j = 0;

    //printf("velikost vstupu je %d\n", input_size);

//    printf ("%d\n", input_array[0]);
    pref = (int) input_array[0];
//    printf("%d\n", pref);
    i++;
    while (i < input_size){
        suf = (int) input_array[i];
        int pom = find_in_dictionary(dictionary, (char) suf, pref);
        if (pom != -1){
            pref = pom;
        } else {
            add_to_dictionary(dictionary, (char) suf, pref);
            printf("%d\n", pref);
            output_array[j] = pref;
            j++;
            pref = (char) suf;
        }
        i++;
    }


    free_dictionary(dictionary);

    return 0;

}

