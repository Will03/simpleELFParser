#include<elf.h>
#include <string.h>
#include<stdio.h>
#include<json-c/json.h>
typedef struct node{ 
    void* info; 
    node* next; 
}node; 
typedef struct{
    int filefd;
    int fileSize;
    int is_64;
    char* fileBuf;
    void* fileEhdr;
    node* filePhdr;
    node* fileShdr;
}ELFinfo;

typedef struct firstDetectELF{
  unsigned char	e_ident[EI_NIDENT];
  Elf32_Half	e_type;
  Elf32_Half	e_machine;
} ElfArch;

int createJson(ELFinfo* myELFinfo);