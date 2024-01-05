//
// Created by Štěpán on 04.01.2024.
//

#ifndef PROJEKT_STEG_MANAGER_H
#define PROJEKT_STEG_MANAGER_H


/**
 * \brief Struktura pro skrytá data
 */
typedef struct {
    int *hidden_data;
    int hidden_data_size;
} hidden_content;

/**
 * \brief Funkce pro vytvoření struktury hidden_content
 * @param data_size velikost dat
 * @return ukazatel na strukturu hidden_content
 */
hidden_content *hidden_content_create(int data_size);

/**
 * \brief Funkce pro uvolnění struktury hidden_content
 * @param content ukazatel na ukazatel na strukturu hidden_content
 */
void hidden_content_free(hidden_content **content);

/**
 * \brief Funkce pro skrytí dat do obrázku
 * @param payload cesta k souboru s daty
 * @param image cesta k obrázku
 * @param file_type typ obrázku
 */
void hide_data(char *payload, char *image, char file_type);

/**
 * \brief Funkce pro extrakci dat z obrázku
 * @param image cesta k obrázku
 * @param payload cesta k souboru pro extrahovaná data
 * @param file_type typ obrázku
 */
void unload_data(char *image, char *payload, char file_type);

/**
 * \brief Funkce pro správu skrytí a extrakce dat
 * @param payload cesta k souboru s daty
 * @param image cesta k obrázku
 * @param switcher přepínač
 * @param file_type typ obrázku
 */
void manage_steg(char *payload, char *image, char switcher, char file_type);



#endif //PROJEKT_STEG_MANAGER_H
