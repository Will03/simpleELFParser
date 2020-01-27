#ifndef stdlib
#define stdlib
#include<stdio.h>
#include <inttypes.h>
#include<elf.h>
#include <string.h>
#include<stdio.h>
#endif
#ifndef dissam
#define dissam
#include "disassem.h"
#endif 

#include<json-c/json.h>

#define MAX_BUF 256
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

bool RegisterAllHeader(ElfArch* fArch, ELFinfo* myELFinfo);
int parseMachine(void* myfileEhdr,int is_64, char* result);
int disassTextSection(ELFinfo* myELFinfo,FILE *outputFp);
int createJson(ELFinfo* myELFinfo,char* resultPath);