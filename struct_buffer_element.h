#ifndef STRUCT_BUFFER_ELEMENT_SEEN
#define STRUCT_BUFFER_ELEMENT_SEEN

struct element {
  int value;
  struct element* left;
  struct element* right;
  struct element* up;
  struct element* down;
};

#endif
