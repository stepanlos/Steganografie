//
// Created by Štěpán on 03.01.2024.
//

#ifndef PROJEKT_PNG_PROCESS_H
#define PROJEKT_PNG_PROCESS_H

//#include "lzw.h"
//#include "steg_manager.h"
#include "png.h"
#include "lzw.h"
#include "steg_manager.h"

#define RGB 3
#define SIGNATURE_BIT_SIZE_P 192
#define SIGNATURE_SIZE_P 6
#define COMPRESSED_BIT_SIZE_P 12
#define INT_BIT_SIZE_P 32
#define BLUE_SHIFT_RGB 2


#define FALSE 0
#define TRUE 1

#define CHAR_K 75
#define CHAR_A 65
#define CHAR_R 82
#define CHAR_I 73
//#define EXIT_MEMORY_ALLOCATION 6
//#define EXIT_FILE_NOT_FOUND 2
//#define EXIT_FILE_READING 3

//#pragma pack(push, 1)



/**
 * \brief Struktura pro PNG obrázek
 * Obsahuje šířku, výšku a ukazatel na pole řádků
 */
typedef struct {
    int width;
    int height;
    png_bytep row_pointers;
} png_organised;

/**
 * \brief Funkce pro načtení PNG obrázku
 * @param filename cesta k obrázku
 * @return ukazatel na strukturu png_organised
 */
png_organised *read_image_png(const char *filename);

/**
 * \brief Funkce pro uložení PNG obrázku
 * @param image ukazatel na strukturu png_organised
 * @param filename cesta k obrázku
 * @return 1 v případě úspěchu, jinak 0
 */
int save_image_png(png_organised *image, const char *filename);

/**
 * \brief Funkce pro uvolnění paměti po PNG obrázku
 * @param image ukazatel na strukturu png_organised
 */
void free_image_png(png_organised **image);

/**
 * \brief Funkce pro vložení dat do modrého kanálu PNG obrázku
 * @param image ukazatel na strukturu png_organised
 * @param compressed_data ukazatel na komprimovaná data
 * @return 1 v případě úspěchu, jinak 0
 */
int change_blue_png(png_organised *image, compressed *compressed_data);

/**
 * \brief Funkce pro extrakci dat z modrého kanálu PNG obrázku
 * @param image ukazatel na strukturu png_organised
 * @return ukazatel na strukturu hidden_content
 */
hidden_content *unload_blue_png(png_organised *image);


//#pragma pack(pop)


#endif //PROJEKT_PNG_PROCESS_H
