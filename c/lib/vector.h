struct cvector {
 char* _buf;
 size_t _indx;
 size_t _size;
}

int cvec_create(cvector* cvec, size_t def_size = 10);
void cvec_push(cvector* cvec, char c);
char cvec_pop(cvector* cvec);
char cvec_get(cvector* cvec, size_t indx);
size_t cvec_size(cvector* cvec);
void cvec_resize(cvector* cvec, size_t new_size);

