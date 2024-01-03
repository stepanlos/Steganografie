//
// Created by Štěpán on 02.01.2024.
//

#ifndef PROJEKT_CRC_H
#define PROJEKT_CRC_H
#include <stdint.h>

uint32_t update_crc(uint32_t crc, unsigned char byte);

int sum_crc(int *array, int start, int length);



#endif //PROJEKT_CRC_H
