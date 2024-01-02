//
// Created by Štěpán on 27.12.2023.
//

#ifndef LZW_H
#define LZW_H

#define COMPRESSED_SIZE 2048 //Velikost komprimovaného pole
#define COMPRESSED_INCREMENT_SIZE 4096 //Velikost o kterou se zvětšuje komprimované pole

#define LENGTH_OF_SIGNATURE 5 //Délka signatury

/** storing compressed data **/
typedef struct {
    int current_length;
    int last_item;
    unsigned char *compressed;
}compressed;

/** storing decompressed data **/
typedef struct {
    int current_length;
    int last_item;
    unsigned char *decompressed;
}decompressed;

/** function for compressing data **/
compressed *compress_lzw(char input_array[], int output_array[]); //TODO vstup název souboru

decompressed *decompress_lzw(int *data, int start ,int data_length);


compressed *compressed_create();
int compressed_add(compressed *c, int item);
void compressed_free(compressed **c);

decompressed *decompressed_create();
int decompressed_add(decompressed *decomp, unsigned char *c, int c_length);
void decompressed_free(decompressed **decomp);





#endif //PROJEKT_LZW_H
