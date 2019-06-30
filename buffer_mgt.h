#ifndef BUFFER_MGT_SEEN
#define BUFFER_MGT_SEEN

#include <stdlib.h>
#include <stdio.h>

#include "struct_boucle.h"
#include "struct_buffer_element.h"

struct element* extends_buffer_right(struct element* buffer_elt);
struct element* extends_buffer_left(struct element* buffer_elt);
struct element* extends_buffer_up(struct element* buffer_elt);
struct element* extends_buffer_down(struct element* buffer_elt);

struct element* create_element(struct element* left, struct element* right, struct element* up, struct element* down);

void delete_element(struct element* buffer_elt);
void delete_buffer(struct element* buffer_elt);

unsigned int add_boucle(struct boucle* boucle_data);
unsigned int remove_boucle(struct boucle* boucle_data);

unsigned int add_boucle_instruction(struct boucle* boucle_data, char to_add);
unsigned int last_boucle_stack(struct boucle* boucle_data, char** output_stack_ptr, unsigned int* output_size);
unsigned int copy_boucle_last_level(struct boucle* boucle_data, struct boucle* current_boucle);
void next_boucle_character(struct boucle* current_boucle, char* character);
void clear_boucle(struct boucle* boucle_data);

#endif
