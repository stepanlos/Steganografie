//
// Created by Štěpán on 26.12.2023.
// Header file pro slovník LZW komprese
//

#ifndef DICTIONARY_H
#define DICTIONARY_H

#define DICTIONARY_SIZE 4096  // Velikost slovníku TODO 4096 (12bit)
#define ASCII_SIZE 256

#define TRUE 1
#define FALSE 0

/**
 * \brief Struktura pro záznam ve slovníku
 */
typedef struct {
    int parent;
    unsigned char last;
} Entry;

/**
 * \brief Struktura pro slovník
 */
typedef struct {
    Entry entries[DICTIONARY_SIZE];
    int size;
} dictionary;

/**
 * \brief Inicializace slovníku
 * @return ukazatel na slovník
 */
dictionary *initialize_dictionary();

/**
 * \brief Přidání záznamu do slovníku
 * @param dictionary slovník
 * @param last poslední znak
 * @param parent rodičovský záznam
 * @return 1 v případě úspěchu, nebo 0 v případě chyby
 */
int add_to_dictionary(dictionary *dictionary, char last, int parent); //TODO unsined chary všude


/**
 * \brief Funkce pro hledání kódu ve slovníku
 * @param dictionary slovník ve kterém se má hledat
 * @param last poslední znak
 * @param parent rodičovský záznam
 * @return index nalezeného záznamu, nebo -1 v případě nenalezení
 */
int find_in_dictionary(const dictionary *dictionary, const unsigned char last, const int parent);


/**
 * \brief Funkce pro hledání kódu ve slovníku při dekompresi
 * @param dictionary slovník ve kterém se má hledat
 * @param key klíč podle kterého se má hledat
 * @return index nalezeného záznamu, nebo -1 v případě nenalezení
 */
int find_in_dictionary_decompress(dictionary *dictionary, int key);

/**
 * \brief Funkce pro získání řetězce ze slovníku
 * @param d slovník
 * @param index index záznamu
 * @param len délka řetězce
 * @return řetězec
 */
char *string_from_index(dictionary *d, int index, int *len);

/**
 * \brief Funkce pro otočení řetězce
 * @param str řetězec
 * @param len délka řetězce
 * @return otočený řetězec
 */
char *reverse_string(char *str, int len);

/**
 * \brief Funkce pro uvolnění slovníku
 * @param dictionary slovník
 * @return 1 v případě úspěchu, nebo 0 v případě chyby
 */
int free_dictionary(dictionary **dictionary);


#endif
