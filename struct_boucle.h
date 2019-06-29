#ifndef STRUCT_BOUCLE_SEEN
#define STRUCT_BOUCLE_SEEN

struct boucle {
  char** buffer;
  unsigned int* sizes;
  unsigned int levels;
  unsigned int* current_indexes;
};

#endif
