#ifndef BUFFER_MGT_SEEN
#define BUFFER_MGT_SEEN

#include <stdlib.h>
#include <stdio.h>

#include "struct_boucle.h"

unsigned int extends_buffer_right(int** buffer_ptr, unsigned int size, unsigned int extension);
unsigned int extends_buffer_left(int** buffer_ptr, unsigned int size, unsigned int extension);

unsigned int add_boucle(struct boucle* boucle_data);
unsigned int remove_boucle(struct boucle* boucle_data);

void add_boucle_instruction(struct boucle* boucle_data, char to_add);
unsigned int last_boucle_stack(struct boucle* boucle_data, char** output_stack_ptr, unsigned int* output_size);
void copy_boucle_last_level(struct boucle* boucle_data, struct boucle* current_boucle);
void next_boucle_character(struct boucle* current_boucle, char* character);
void clear_boucle(struct boucle* boucle_data);

#endif
