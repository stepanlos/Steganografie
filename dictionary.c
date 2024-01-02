//
// Created by Štěpán on 27.12.2023.
//
#include <stdlib.h>
#include <stdio.h>
#include "dictionary.h"

#define ASCII_SIZE 256

/**
 * Funkce pro uvolnění slovníku
 * @return vytvořený slovník s inicializovanými hodnotami ascii znaků, nebo NULL v případě chyby
 */
Dictionary *initialize_dictionary() {
    Dictionary *dictionary = (Dictionary *)malloc(sizeof(Dictionary));
    if (dictionary == NULL) {
        // handle memory allocation failure
        return NULL;
    }

    // Inicializace velikosti slovníku
    dictionary->size = 0;

    // Inicializace položek slovníku
    for (int i = 0; i < ASCII_SIZE; i++) {
        dictionary->entries[i].parent = -1;  // Výchozí hodnota pro nenastavený záznam
        dictionary->entries[i].last = (char)i;  // ASCII hodnota pro kód i
        dictionary->size++;
    }

    return dictionary;
}

/**
 * Funkce pro přidání záznamu do slovníku
 * @param dictionary slovník do kterého se má záznam přidat
 * @param last poslední znak
 * @param parent rodičovský záznam
 * @return 1 v případě úspěchu, nebo 0 v případě chyby
 */
int add_to_dictionary(Dictionary *dictionary, char last, int parent) {

    if (dictionary->size >= DICTIONARY_SIZE){
        dictionary->size = ASCII_SIZE;
        printf("Slovnik je plny, vracim se na zacatek\n");
    }

    // Přidání nového záznamu
    dictionary->entries[dictionary->size].last = last;
    dictionary->entries[dictionary->size].parent = parent;
    dictionary->size++;

    return 1;
}



/**
 * Funkce pro hledání kódu ve slovníku
 * @param dictionary slovník ve kterém se má hledat
 * @param last poslední znak
 * @param parent rodičovský záznam
 * @return index nalezeného záznamu, nebo -1 v případě nenalezení
 */
int find_in_dictionary(const Dictionary *dictionary, const char last,const int parent) {

    // Hledání záznamu
    for (int i = 0; i < dictionary->size; i++) {
        if (dictionary->entries[i].last == last && dictionary->entries[i].parent == parent) {
            return i;
        }
    }

    // Záznam nenalezen
    return -1;
}


/**
 * Funkce pro hledání kódu ve slovníku při dekompresi
 * @param dictionary slovník ve kterém se má hledat
 * @param key klíč podle kterého se má hledat
 * @return index nalezeného záznamu, nebo -1 v případě nenalezení
 */
int find_in_dictionary_decompress(Dictionary *dictionary, int key) {

    // Hledání záznamu
    if (key < dictionary->size) {
        return key;
    }

    // Záznam nenalezen
    return -1;
}


char *string_from_index(Dictionary *d, int index, int *len) {
    unsigned char *str = NULL;
    int i = index;

    while (index != -1) {
        index = d->entries[index].parent;
        (*len)++;
    }
    (*len)++;


    str = (unsigned char *)malloc(sizeof(unsigned char) * (*len));
    if (str == NULL) {
        // handle memory allocation failure
        return NULL;
    }

    // Fill the string
    for (i = 0; i < *len; i++) {
        str[i] = d->entries[index].last;
        index = d->entries[index].parent;
    }

    return (char *) reverse_string((char *)str, *len);
}


char *reverse_string(char *str, int len) {
    int i = 0;
    char tmp;

    for (i = 0; i < len / 2; i++) {
        tmp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = tmp;
    }
    return str;
}

/**
 * Funkce pro uvolnění slovníku
 * @param dictionary slovník který se má uvolnit
 * @return 1 v případě úspěchu, nebo 0 v případě chyby
 */
int free_dictionary(Dictionary **dictionary) {
    if (dictionary == NULL) {
        // TODO handle invalid dictionary
        return 0;
    }


//    for (int i = 0; i < DICTIONARY_SIZE; ++i) {
//        free((*dictionary)->entries[i]);
//
//    }
    // Uvolnění slovníku
    free((*dictionary)->entries);
    free(*dictionary);
    *dictionary = NULL;

    printf("Slovnik uvolnen\n");

    return 1;
}



