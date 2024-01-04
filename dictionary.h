//
// Created by Štěpán on 26.12.2023.
// Header file pro slovník LZW komprese
//

#ifndef DICTIONARY_H
#define DICTIONARY_H

#define DICTIONARY_SIZE 4096  // Velikost slovníku TODO 4096 (12bit)

#define TRUE 1
#define FALSE 0


typedef struct {
    int parent;
    unsigned char last;
} Entry;

// Struktura slovníku
typedef struct {
    Entry entries[DICTIONARY_SIZE];
    int size;
} dictionary;

// Inicializace slovníku
dictionary *initialize_dictionary();

// Přidání záznamu do slovníku
int add_to_dictionary(dictionary *dictionary, char last, int parent); //TODO unsined chary všude


// Hledání kódu ve slovníku
int find_in_dictionary(const dictionary *dictionary, const unsigned char last, const int parent);

//Hledani kódu ve slovníku při dekompresi
int find_in_dictionary_decompress(dictionary *dictionary, int key);

char *string_from_index(dictionary *d, int index, int *len);
char *reverse_string(char *str, int len);

//uvolnění slovníku
int free_dictionary(dictionary **dictionary);


#endif
