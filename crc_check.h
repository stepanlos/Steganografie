//
// Created by Štěpán on 02.01.2024.
//

#ifndef PROJEKT_CRC_CHECK_H
#define PROJEKT_CRC_CHECK_H

#include <stdint.h>

#define CHAR_SIZE 8


/**
 * Funkce pro výpočet CRC32
 * @param crc CRC32
 * @param byte byte
 * @return nové CRC32
*/
uint32_t update_crc(uint32_t crc, unsigned char byte);

/**
 * Funkce pro výpočet CRC32 pro pole ukazaratelů na int
 * @param array pole
 * @param start počáteční index
 * @param length délka
 * @return součet CRC32
 */
int sumr_crc(int *array, int start, int length);



#endif //PROJEKT_CRC_CHECK_H
