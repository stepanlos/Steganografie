//
// Created by Štěpán on 04.01.2024.
//

#include "steg_manager.h"
#include <stdlib.h>
#include <stdio.h>
#include "io_manager.h"


hidden_content *hiddenContent_create(int data_size) {
    hidden_content *content = malloc(sizeof(hidden_content));
    if (content == NULL) {
        printf("Error while allocating memory for hidden content\n");
        exit_value = MALLOC_FAIL;
        return NULL;
    }



    content->hidden_data_size = data_size;
    content->hidden_data = malloc(data_size * sizeof(int));
    if (content->hidden_data == NULL) {
        free(content);
        printf("Error while allocating memory for hidden content\n");
        exit_value = MALLOC_FAIL;
        return NULL;
    }
    for (int i = 0; i < data_size; ++i) {
        content->hidden_data[i] = 0;
    }

    return content;
}


void hidden_content_free(hidden_content **content) {
    if (content == NULL || *content == NULL) {
        return;
    }

    free((*content)->hidden_data);
    free(*content);
    *content = NULL;
}

