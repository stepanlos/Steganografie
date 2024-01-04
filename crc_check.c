//
// Created by Štěpán on 02.01.2024.
//
#include "crc_check.h"

/**
 * Funkce pro výpočet CRC32
 * @param crc CRC32
 * @param byte byte
 * @return nové CRC32
 */
uint32_t update_crc(uint32_t crc, unsigned char byte) {
    crc = crc ^ (uint32_t) byte;
    for (int i = 0; i < 8; i++) {
        crc = (crc >> 1) ^ ((crc & 1) * 0xEDB88320);
    }
    return crc;
}

/**
 * Funkce pro výpočet CRC32 pro pole ukazaratelů na int
 * @param array pole
 * @param start počáteční index
 * @param length délka
 * @return součet CRC32
 */
int sumr_crc(int *array, int start, int length) {
    uint32_t crc = 0xFFFFFFFF;
    for (int i = 0; i < length; i++) {
        crc = update_crc(crc, (unsigned char) array[i]);
    }
    return crc ^ 0xFFFFFFFF;
}