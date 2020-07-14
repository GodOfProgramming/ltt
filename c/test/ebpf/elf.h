#ifndef ELF_H
#define ELF_H
#include <gelf.h>
#include <stddef.h>

int load_bfp_file(const char* path);
#endif