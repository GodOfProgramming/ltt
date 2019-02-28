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

