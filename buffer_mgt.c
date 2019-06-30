#include "buffer_mgt.h"

unsigned int extends_buffer_right(int** buffer_ptr, unsigned int size, unsigned int extension)
{
  unsigned int new_size = size + extension;
  int *second_buffer = malloc(new_size * sizeof(int));

  if (second_buffer == NULL)
  {
    new_size = 0;
  }

  for (unsigned int i = 0; i < new_size; ++i)
  {
    if (i < size)
    {
      second_buffer[i] = (*buffer_ptr)[i];
    }
    else
    {
      second_buffer[i] = 0;
    }
  }

  free(*buffer_ptr);
  *buffer_ptr = second_buffer;
  return new_size;
}

unsigned int extends_buffer_left(int** buffer_ptr, unsigned int size, unsigned int extension)
{
  unsigned int new_size = size + extension;
  int *second_buffer = malloc(new_size * sizeof(int));

  if (second_buffer == NULL)
  {
    new_size = 0;
  }

  for (unsigned int i = 0; i < new_size; ++i)
  {
    if (i < extension)
    {
      second_buffer[i] = 0;
    }
    else
    {
      second_buffer[i] = (*buffer_ptr)[i - extension];
    }
  }
  
  free(*buffer_ptr);
  *buffer_ptr = second_buffer;
  return new_size;
}

unsigned int add_boucle(struct boucle* boucle_data)
{
  unsigned int new_size = (*boucle_data).levels + 1;
  char** second_buffer = malloc(new_size * sizeof(char*));
  unsigned int* second_sizes = malloc(new_size * sizeof(unsigned int));
  unsigned int* current_indexes = malloc(new_size * sizeof(unsigned int)); 

  if (second_buffer == NULL || second_sizes == NULL || current_indexes == NULL)
  {
    new_size = 0;
  }

  for (unsigned int i = 0; i < new_size; ++i)
  {
    // We are in existing levels. We copy data.
    if (i < (*boucle_data).levels)
    {
      second_buffer[i] = (*boucle_data).buffer[i];
      second_sizes[i] = (*boucle_data).sizes[i];
      current_indexes[i] = (*boucle_data).current_indexes[i];
    }
    else
    {
      second_buffer[i] = NULL;
      second_sizes[i] = 0;
      current_indexes[i] = 0;
    }
  }

  free((*boucle_data).buffer);
  free((*boucle_data).sizes);
  free((*boucle_data).current_indexes);
  (*boucle_data).buffer = second_buffer;
  (*boucle_data).sizes = second_sizes;
  (*boucle_data).levels = new_size;
  (*boucle_data).current_indexes = current_indexes;
  return new_size;
}

unsigned int remove_boucle(struct boucle* boucle_data)
{
  unsigned int new_size = (*boucle_data).levels - 1;
  char** second_buffer = malloc(new_size * sizeof(char*));
  unsigned int* second_sizes = malloc(new_size * sizeof(unsigned int));
  unsigned int* current_indexes = malloc(new_size * sizeof(unsigned int));
  unsigned char allocation_failed = (second_buffer == NULL || second_sizes == NULL || current_indexes == NULL);

  // If the new memory allocation failed, we keep the same memory zone, it can avoid program crashes at the end, but it will probably be problematic at the next boucle or buffer allocation.
  if (allocation_failed)
  {
    free(second_buffer);
    free(second_sizes);
    free(current_indexes);
  }
  else
  {
    for (unsigned int i = 0; i < new_size; ++i)
    {
      second_buffer[i] = (*boucle_data).buffer[i];
      second_sizes[i] = (*boucle_data).sizes[i];
      current_indexes[i] = (*boucle_data).current_indexes[i];
    }
  }

  // We free the pointers of the old level, no more used.
  free((*boucle_data).buffer[new_size]);

  if (!allocation_failed)
  {
    free((*boucle_data).buffer);
    free((*boucle_data).sizes);
    free((*boucle_data).current_indexes);
    (*boucle_data).buffer = second_buffer;
    (*boucle_data).sizes = second_sizes;
    (*boucle_data).current_indexes = current_indexes;
  }

  (*boucle_data).levels = new_size;
  return new_size;
}

unsigned int add_boucle_instruction(struct boucle* boucle_data, char to_add)
{
  // We add the instruction on each boucle stack.
  for (unsigned int i = 0; i < (*boucle_data).levels; ++i)
  {
    unsigned int new_size = (*boucle_data).sizes[i] + 1;
    char* new_buffer = malloc(new_size * sizeof(char));

    if (new_buffer == NULL)
    {
      new_size = 0;
    }

    for (unsigned int j = 0; j < new_size; ++j)
    {
      if (j < (*boucle_data).sizes[i])
      {
        // printf("a\n");
        new_buffer[j] = (*boucle_data).buffer[i][j];
      }
      else
      {
        new_buffer[j] = to_add;
      }
    }

    free((*boucle_data).buffer[i]);
    (*boucle_data).buffer[i] = new_buffer;
    (*boucle_data).sizes[i] = new_size;
    
    if (new_size == 0)
    {
      return 0;
    }
  }

  return 1;
}

unsigned int last_boucle_stack (struct boucle* boucle_data, char** output_stack_ptr, unsigned int* output_size)
{
  unsigned int size = (*boucle_data).levels;
  if (size <= 0)
  {
    return 1;
  }
  (*output_stack_ptr) = (*boucle_data).buffer[size-1];
  (*output_size) = (*boucle_data).sizes[size-1];

  return 0;
}

unsigned int copy_boucle_last_level(struct boucle* boucle_data, struct boucle* current_boucle)
{
  char* output_stack_ptr;
  unsigned int output_size;

  last_boucle_stack(boucle_data, &output_stack_ptr, &output_size);
  add_boucle(current_boucle);

  char* new_buffer = malloc(output_size * sizeof(char));

  if (new_buffer == NULL)
  {
    output_size = 0;
  }

  for (int i = 0; i < output_size; ++i)
  {
    new_buffer[i] = output_stack_ptr[i];
  }

  unsigned int new_size = (*current_boucle).levels-1;

  free((*current_boucle).buffer[new_size]);
  (*current_boucle).buffer[new_size] = new_buffer;
  (*current_boucle).sizes[new_size] = output_size;

  return output_size;
}

void next_boucle_character(struct boucle* current_boucle, char* character)
{
  unsigned int last_stack_index = (*current_boucle).levels-1;
  unsigned int read_index = (*current_boucle).current_indexes[last_stack_index];
  (*character) = (*current_boucle).buffer[last_stack_index][read_index];

  ++((*current_boucle).current_indexes[last_stack_index]);

  if ((*current_boucle).current_indexes[last_stack_index] == (*current_boucle).sizes[last_stack_index])
  {
    remove_boucle(current_boucle); 
  }
}

void clear_boucle(struct boucle* boucle_data)
{
  for (unsigned int i = 0; i < (*boucle_data).levels; ++i)
  {
    free((*boucle_data).buffer[i]);
  }

  free((*boucle_data).buffer);
  (*boucle_data).buffer = NULL;
  free((*boucle_data).current_indexes);
  (*boucle_data).current_indexes = NULL;
  free((*boucle_data).sizes);
  (*boucle_data).sizes = NULL;
  (*boucle_data).levels = 0;
}
