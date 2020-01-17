#include<elf.h>

typedef struct{
    int filefd;
    int fileSize;
    int is_64;
    char* fileBuf;
    void* fileEhdr;
    void* filePhdr;
}ELFinfo;

typedef struct firstDetectELF{
  unsigned char	e_ident[EI_NIDENT];
  Elf32_Half	e_type;
  Elf32_Half	e_machine;
} ElfArch;
