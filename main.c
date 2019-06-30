#include <stdio.h>
#include <stdlib.h>

#include "buffer_mgt.h"

int* buffer; // Brainfuck row
unsigned int current_pos = 0; // Current position in the array
unsigned int max_pos = 0; // Maximum position in the array

unsigned char jump_code = 0; // If we are in a boucle with a pointer pointing 0.
struct boucle boucle_data;
struct boucle current_boucle;

void eval_char(char instruction, unsigned char first_pass)
{
  unsigned int value;
  // printf("%c\n", instruction);
  if (first_pass)
  {
    add_boucle_instruction(&boucle_data, instruction);
  }
  if (!jump_code)
  {
    switch (instruction)
    {
      case '+':
        ++(buffer[current_pos]);
        break;
      case '-':
        --(buffer[current_pos]);
        break;
      case '.':
        printf("%c", buffer[current_pos]);
        break;
      case '>':
        if (current_pos >= max_pos)
        {
          max_pos = extends_buffer_right(&buffer, max_pos + 1, 1)-1;
        }
        ++current_pos;
        break;
      case '<':
        if (current_pos <= 0)
        {
          max_pos = extends_buffer_left(&buffer, max_pos + 1, 1)-1;
        }
        else
        {
          --current_pos;
        }
        break;
      case '[':
        if (buffer[current_pos])
        {
          add_boucle(&boucle_data);
        }
        else
        {
          jump_code = 1;
        }
        break;
      case ']':
        if (buffer[current_pos])
        {
          copy_boucle_last_level(&boucle_data, &current_boucle);
        }
        else
        {
          remove_boucle(&boucle_data);
        }
        break;
      case ',':
        value = getchar();
        buffer[current_pos] = value;
        break;
      default:
        return;
    }
  }
  else if(instruction == ']')
  {
    jump_code = 0;
  }
}

void readFromFile(FILE* file)
{
  buffer = malloc((max_pos + 1) * sizeof(int));
  buffer[0] = 0;
  current_pos = 0;
  max_pos = 0;
  jump_code = 0;
  clear_boucle(&boucle_data);
  clear_boucle(&current_boucle);
  char i;

  while((i = fgetc(file)) != EOF)
  {
    eval_char(i, 1);
    
    while (current_boucle.levels)
    {
      char instruction;
      next_boucle_character(&current_boucle, &instruction);
      eval_char(instruction, 0);
    }
  }

  printf("\n");
  free(buffer);
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
    readFromFile(file);
  }

  return 0;
} 
