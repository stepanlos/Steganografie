//
// Created by Štěpán on 27.12.2023.
//

#ifndef LZW_H
#define LZW_H

#define COMPRESSED_SIZE 2048 //Velikost komprimovaného pole
#define COMPRESSED_INCREMENT_SIZE 4096 //Velikost o kterou se zvětšuje komprimované pole

#define LENGTH_OF_SIGNATURE 6 //Délka signatury
#define LENGTH_OF_DATA 4 //Délka datové části
#define CRC_POSITON 5


/** \brief Struktura pro komprimovaná data */
typedef struct {
    int current_length;
    int last_item;
    int *compressed;
}compressed;

/** \brief Struktura pro dekomprimovaná data */
typedef struct {
    int current_length;
    int last_item;
    unsigned char *decompressed;
}decompressed;

/**
 * \brief Funkce pro kompresi dat
 * @param filename název souboru
 * @return ukazatel na strukturu compressed
 */
compressed *compress_lzw(char *filename);

/**
 * \brief Funkce pro dekompresi dat
 * @param data ukazatel na komprimovaná data
 * @param start počáteční index dat
 * @param data_length délka dat
 * @return ukazatel na strukturu decompressed
 */
decompressed *decompress_lzw(int *data, int start ,int data_length);

/**
 * \brief Funkce pro vytvoření komprimované struktury
 * @return ukazatel na strukturu compressed
 */
compressed *compressed_create();

/**
 * \brief Funkce pro přidání prvku do komprimované struktury
 * @param c ukazatel na komprimovanou strukturu
 * @param item přidávaný prvek
 * @return 1 v případě úspěchu, jinak 0
 */
int compressed_add(compressed *c, int item);

/**
 * \brief Funkce pro uvolnění komprimované struktury
 * @param c ukazatel na komprimovanou strukturu
 */
void compressed_free(compressed **c);


/**
 * \brief Funkce pro vytvoření dekomprimované struktury
 * @return ukazatel na dekomprimovanou strukturu
 */
decompressed *decompressed_create();

/**
 * \brief Funkce pro přidání prvku do dekomprimované struktury
 * @param decomp ukazatel na dekomprimovanou strukturu
 * @param c ukazatel na pole znaků
 * @param c_length délka pole znaků
 * @return 1 v případě úspěchu, jinak 0
 */
int decompressed_add(decompressed *decomp, unsigned char *c, int c_length);

/**
 * \brief Funkce pro uvolnění dekomprimované struktury
 * @param decomp ukazatel na dekomprimovanou strukturu
 */
void decompressed_free(decompressed **decomp);





#endif //PROJEKT_LZW_H
