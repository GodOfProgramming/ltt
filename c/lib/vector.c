#include <stdlib.h>
#include <cvector.h>

int cvec_create(cvector* cvec, size_t def_size)
{
  cvec->_buf = malloc(sizeof(char) * def_size);
  cvec->_indx = 0;
  cvec->_size = def_size;
  return cvec_buf != NULL;
}

size_t cvec_size(cvector* cvec)
{
  return cvec->_size;
}

void cvec_push(cvector* cvec, char c)
{
  if(cvec->_buf == NULL) {
    return;
  }

  if(cvec->_indx == cvec->_size) {
    cvec_resize(cvec, cvec->_size * 0.6f);
  }

  cvec->_buf[cvec->_indx++] = c;
}

char cvec_pop(cvector* cvec)
{
  if(cvec->_buf == NULL) {
    return;
  }

  if(!cvec->_indx) {
    return;
  }

  return cvec_get(cvec, cvec->_indx--);
}

char cvec_get(cvector* cvec, size_t indx)
{

}

struct string {
    const char* chars;
    size_t length;
};

struct string_vector {
    struct string* arr;
    size_t current_index;
    size_t size;
};

void vec_sizeup(struct string_vector* vec) {
    size_t new_size = vec->size * 1.4; // 40% increase in size
    struct string* old = vec->arr;
    struct string* next = malloc(sizeof(struct string) * new_size);
    memcpy(next, old, sizeof(struct string) * vec->size);
    free(old);
    vec->arr = next;
}

void vec_push(struct string_vector* vec, struct string str) {
    if (vec->current_index >= vec->size) {
        vec_sizeup(vec);
    }

    vec->arr[vec->current_index++] = str;
}
