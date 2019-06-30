#include <stdio.h>
#include <stdlib.h>

#include "buffer_mgt.h"
#include "struct_buffer_element.h"
#include "struct_boucle.h"

struct element* current_elt;

unsigned char jump_code = 0; // If we are in a boucle with a pointer pointing 0.
struct boucle boucle_data;
struct boucle current_boucle;

int eval_char(char instruction, unsigned char first_pass)
{
  unsigned int value;
  unsigned int new_size; // New size of the buffer
  struct element* temp_elt;
  // printf("%c\n", instruction);
  if (first_pass)
  {
    if(!add_boucle_instruction(&boucle_data, instruction))
    {
      return 5;
    }
  }
  if (!jump_code)
  {
    switch (instruction)
    {
      case '+':
        ++(*current_elt).value;
        break;
      case '-':
        --(*current_elt).value;
        break;
      case '.':
        printf("%c", (*current_elt).value);
        break;
      case '>':
        temp_elt = extends_buffer_right(current_elt);
        if (temp_elt == NULL)
        {
          return 2;
        }
        current_elt = temp_elt;
        break;
      case '<':
        temp_elt = extends_buffer_left(current_elt);
        if (temp_elt == NULL)
        {
          return 2;
        }
        current_elt = temp_elt;
        break;
      case '^':
        current_elt = extends_buffer_up(current_elt);
        if (current_elt == NULL)
        {
          return 2;
        }
        break;
      case 'v':
        current_elt = extends_buffer_down(current_elt);
        if (current_elt == NULL)
        {
          return 2;
        }
        break;
      case '[':
        if ((*current_elt).value)
        {
          if (!add_boucle(&boucle_data))
          {
            return 3;
          }
        }
        else
        {
          jump_code = 1;
        }
        break;
      case ']':
        if (!boucle_data.levels)
        {
          return 1;
        }
        if ((*current_elt).value)
        {
          if (!copy_boucle_last_level(&boucle_data, &current_boucle))
          {
            return 4;
          }
        }
        else
        {
          remove_boucle(&boucle_data);
        }
        break;
      case ',':
        value = getchar();
        (*current_elt).value = value;
        break;
      default:
        return 0;
    }
  }
  else if(instruction == ']')
  {
    jump_code = 0;
  }

  return 0;
}

int readFromFile(FILE* file)
{
  current_elt = create_element(NULL, NULL, NULL, NULL);
  if (current_elt == NULL)
  {
    return 2;
  }
  jump_code = 0;
  clear_boucle(&boucle_data);
  clear_boucle(&current_boucle);
  char i;

  while((i = fgetc(file)) != EOF)
  {
    int status = eval_char(i, 1);

    if (status)
    {
      return status;
    }
    
    while (current_boucle.levels)
    {
      char instruction;
      next_boucle_character(&current_boucle, &instruction);
      status = eval_char(instruction, 0);
      if (status)
      {
        return status;
      }
    }
  }

  printf("\n");

  return 0;
}

int main(int argc, char** argv)
{
  if (argc <= 1)
  {
    printf("Usage : %s [filenames...]\n", argv[0]);
    return 0;
  }
  for (int i = 1; i < argc; ++i)
  {
    FILE* file = fopen(argv[i], "r");
    int status = (file == NULL) ? -1 : readFromFile(file);
    delete_buffer(current_elt);
    clear_boucle(&boucle_data);
    clear_boucle(&current_boucle);
    if (status)
    {
      switch (status)
      {
        case -1:
          printf("Unable to open file %s (code: %d).\n", argv[i], status);
          break;
        case 1:
          printf("Syntax error: Closing brace without corresponding opening brace (%s) (code: %d).\n", argv[i], status);
          break;
        case 2:
          printf("Memory error: Failed to allocate enough Memory for the buffer (%s) (code: %d).\n", argv[i], status);
          break;
        case 3:
        case 4:
        case 5:
          printf("Memory error: Failed to allocate enough Memory for the loop buffer (%s) (code: %d).\n", argv[i], status);
          break;
        default:
          printf("Unknown error (%s) (code: %d).\n", argv[i], status);
          break;
      }
    }
  }

  return 0;
} 
