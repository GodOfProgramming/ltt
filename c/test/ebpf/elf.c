#include "elf.h"

int load_bfp_file(const char* path)
{
  int  fd, i, ret, maps_shndx = -1, strtabidx = -1;
  Elf* elf;

  GElf_Ehdr ehdr;
  GElf_Shdr shdr, shdr_prog;
  Elf_Data *data, *data_prog, *data_maps = NULL, *symbols = NULL;
  char *    shname, shname_prog;
  int       nr_maps = 0;
}