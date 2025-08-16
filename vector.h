#ifndef VECTOR_HEADER_H
#define VECTOR_HEADER_H

#include <stdlib.h>

/**
    Functions:
    - vec_add(vec, value): Adds an element to the end of the  vector
    - vec_pop(vec): Removes the last element of the vector and returns it
    - vec_remove(vec, index): Removes the element at the given index
    - vec_fill_empty(vec, index): Grows the vector so there are indices for every index up until the given one. It is filled with darnom data
    - vec_fill_empty_void_ptr(vec, index, data_size): Grows a void* vector so there are indices for every index up until the given one. It is filled with random data.
    - vec_free(vec): Cleans up the memory related to the vector
    - vec_length(vec): Returns the length of the vector
    - vec_cap(vec): Returns the current capacity of the vector

    Statements:
    - vec_for_each_cpy(variable_dec, vec): For each loop over the elements of the vector as a copy
    - vec_for_each_ptr(variable_dec, vec): For each loop over the elements of the vector as a (const) ptr
*/

#define CONCAT(a,b) a##b
#define EXPAND_AND_CONCAT(a,b) CONCAT(a,b)


typedef struct {
    size_t cap;
    size_t len;
} vector_header_t;

// util

#define vec_to_base(VEC) (void*)(&((vector_header_t*)VEC)[-1])
#define base_to_vec(BASE) (void*)((char*)BASE + sizeof(vector_header_t))
#define type_size(vec) sizeof(*vec)

#define vec_grow(vec) do { \
vector_header_t* header = &((vector_header_t*)vec)[-1]; \
header->cap *= 2;\
header = realloc(header, header->cap * sizeof(*vec) + sizeof(vector_header_t));\
vec = (void*)((char*)header + sizeof(vector_header_t));\
} while(0)

#define vec_grow_void_ptr(vec, DATA_SIZE) do { \
vector_header_t* header = &((vector_header_t*)vec)[-1]; \
header->cap *= 2;\
header = realloc(header, header->cap * DATA_SIZE + sizeof(vector_header_t));\
vec = (void*)((char*)header + sizeof(vector_header_t));\
} while(0)

#define vec_init(vec) do { vector_header_t* header = malloc(10 * type_size(vec) + sizeof(vector_header_t)); header->cap = 10; header->len = 0; vec = (void*)((char*)header + sizeof(vector_header_t));} while(0)
#define vec_init_size(vec, SIZE) do { vector_header_t* header = malloc(SIZE * type_size(vec) + sizeof(vector_header_t)); header->cap = SIZE; header->len = 0; vec = (void*)((char*)header + sizeof(vector_header_t));} while(0)
#define vec_init_void_ptr(vec, DATA_SIZE) do { vector_header_t* header = malloc(10 * DATA_SIZE + sizeof(vector_header_t)); header->cap = 10; header->len = 0; vec = void*)((char*)header + sizeof(vector_header_t));} while(0)

#define vec_add(vec, value) \
do{\
    if((vec) == NULL) vec_init((vec));\
    vector_header_t* header = vec_to_base((vec));\
    if(header->len >= header->cap) vec_grow((vec));\
    header = ((vector_header_t*)(vec)) - 1;\
    (vec)[header->len] = value;\
    header->len++; \
}while(0)

#define vec_insert(VEC, INDEX, VALUE)                           \
do {                                                            \
    if(VEC == NULL) vec_init(VEC);                              \
    vector_header_t* header = ((vector_header_t*)(VEC)) - 1;      \
    if(header->len >= header->cap) vec_grow(VEC);             \
    header = ((vector_header_t*)VEC) - 1;                      \
    for(int i = vec_length(VEC)-1;i >= INDEX;i--){              \
        VEC[i+1] = VEC[i];                                      \
    }                                                           \
    VEC[INDEX] = VALUE;                                         \
    header->len++;                                              \
} while(0);
#define vec_pop(vec) (((vector_header_t*)vec - 1)->len--, vec[((vector_header_t*)vec - 1)->len])
#define vec_peek(vec) (vec[((vector_header_t*)vec - 1)->len-1])

#define vec_remove(VEC, INDEX) do { for(int i = INDEX;i < vec_length(VEC)-1;i++) {VEC[i] = VEC[i+1];} ((vector_header_t*)VEC - 1)->len--;}while(0)
#define vec_length(vec) (vec == NULL ? 0 : ((vector_header_t*)vec)[-1].len)
#define vec_empty(VEC) (vec_length(VEC) == 0)
#define vec_cap(vec) (vec == NULL ? 0 : ((vector_header_t*)vec)[-1].cap)

#define vec_free(vec) do { if(vec != NULL) {free(((vector_header_t*)vec) - 1); vec = NULL;} }while(0)

#define vec_fill_empty(VEC, SIZE) do { \
if(VEC == NULL) { vec_init(VEC); }\
while((vec_length(VEC)) < SIZE){\
    vec_grow(VEC);\
    ((vector_header_t*)VEC - 1)->len = ((vector_header_t*)VEC - 1)->cap; \
}\
((vector_header_t*)VEC - 1)->len = SIZE; \
} while(0);

#define vec_fill_empty_void_ptr(VEC, SIZE, DATA_SIZE)                           \
do {                                                                            \
    if(VEC == NULL) { vec_init_void_ptr(VEC, DATA_SIZE); }                      \
    while((vec_cap(VEC)) < SIZE){                                               \
        vec_grow_void_ptr(VEC, DATA_SIZE);                                      \
        ((vector_header_t*)VEC - 1)->len = ((vector_header_t*)VEC - 1)->cap;    \
    }                                                                           \
    ((vector_header_t*)VEC - 1)->len = SIZE;                                    \
} while(0);

#define vec_for_each_ptr(VAR, VEC) \
    for(int EXPAND_AND_CONCAT(a,__LINE__) = 1, EXPAND_AND_CONCAT(b,__LINE__) = 0; EXPAND_AND_CONCAT(b,__LINE__) < vec_length(VEC);EXPAND_AND_CONCAT(a,__LINE__) = !EXPAND_AND_CONCAT(a,__LINE__), EXPAND_AND_CONCAT(b,__LINE__)++) for(VAR = &VEC[EXPAND_AND_CONCAT(b,__LINE__)];EXPAND_AND_CONCAT(a,__LINE__);EXPAND_AND_CONCAT(a,__LINE__) = !EXPAND_AND_CONCAT(a,__LINE__))

#define vec_enum_each_ptr(INDEX, VAR, VEC) \
    for(int EXPAND_AND_CONCAT(a,__LINE__) = 1, INDEX = 0; INDEX < vec_length(VEC);EXPAND_AND_CONCAT(a,__LINE__) = !EXPAND_AND_CONCAT(a,__LINE__), INDEX++) for(VAR = &VEC[INDEX];EXPAND_AND_CONCAT(a,__LINE__);EXPAND_AND_CONCAT(a,__LINE__) = !EXPAND_AND_CONCAT(a,__LINE__))

#define vec_for_each_cpy(VAR, VEC) \
    for(int EXPAND_AND_CONCAT(a,__LINE__) = 1, EXPAND_AND_CONCAT(b,__LINE__) = 0; EXPAND_AND_CONCAT(b,__LINE__) < vec_length(VEC);EXPAND_AND_CONCAT(a,__LINE__) = !EXPAND_AND_CONCAT(a,__LINE__), EXPAND_AND_CONCAT(b,__LINE__)++) for(VAR = VEC[EXPAND_AND_CONCAT(b,__LINE__)];EXPAND_AND_CONCAT(a,__LINE__);EXPAND_AND_CONCAT(a,__LINE__) = !EXPAND_AND_CONCAT(a,__LINE__))

#define vec_enum_each_cpy(INDEX, VAR, VEC) \
    for(int EXPAND_AND_CONCAT(a,__LINE__) = 1, INDEX = 0; INDEX < vec_length(VEC);EXPAND_AND_CONCAT(a,__LINE__) = !EXPAND_AND_CONCAT(a,__LINE__), INDEX++) for(VAR = VEC[INDEX];EXPAND_AND_CONCAT(a,__LINE__);EXPAND_AND_CONCAT(a,__LINE__) = !EXPAND_AND_CONCAT(a,__LINE__))





#endif // VECTOR_HEADER_H
