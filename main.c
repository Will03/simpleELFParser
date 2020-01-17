#include"ELFHeaderParser.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdbool.h>
#include <string.h>
#include <cstdlib>
#include <iostream>


bool RegisterHeader(ElfArch* fArch,void** fileEhdr,char* fileBuf,int& is_64){
    if (fArch->e_ident[EI_CLASS] == ELFCLASS32){
        *fileEhdr = (Elf32_Ehdr*)malloc(sizeof(Elf32_Ehdr));
        memcpy(fileEhdr,fileBuf,(size_t)sizeof(Elf32_Ehdr));
        is_64 = 0;
    }
    else if (fArch->e_ident[EI_CLASS] == ELFCLASS64) {
        *fileEhdr = (Elf64_Ehdr*)malloc(sizeof(Elf64_Ehdr));
        memcpy(*fileEhdr,fileBuf,(size_t)sizeof(Elf64_Ehdr));
        is_64=1;
    }
    else{printf("not match 32 or 64-bit format");return false;}
    
    return true;
}

bool readBinFile(const char fileName[], char** bufPtr, int& length) {
	if (FILE* fp = fopen(fileName, "rb")) {
		fseek(fp, 0, SEEK_END);
		length = ftell(fp);
		*bufPtr = (char*) malloc(sizeof(char)*length + 1);
		fseek(fp, 0, SEEK_SET);
		fread(*bufPtr, sizeof(char), length, fp);
		return true;
	}
	else return false;
}

int main(int argc, char *argv[]){
    
    ELFinfo* myELFinfo = (ELFinfo*)malloc(sizeof(ELFinfo));
    ElfArch* fArch = (ElfArch*)malloc(sizeof(ElfArch));
    if (argc != 2){
        printf("Usage: ELFParser [Filepath]\n");
        return false;
    }
    if (!readBinFile(argv[1],&myELFinfo->fileBuf, myELFinfo->fileSize)){
        printf("error reading file");
        return false;
    }
    memcpy(fArch,myELFinfo->fileBuf,(size_t)sizeof(ElfArch));
    if (!memcmp(fArch->e_ident,"\x7fELF",4)){
        printf("Not a ELF file\n");
        return false;
    }
    if(!RegisterHeader(fArch,&myELFinfo->fileEhdr,myELFinfo->fileBuf,myELFinfo->is_64)){
        return false;
    }
    
    if (myELFinfo->is_64){
    printf("%x\n",((Elf64_Ehdr*)myELFinfo->fileEhdr)->e_type);
    printf("%x\n",((Elf64_Ehdr*)myELFinfo->fileEhdr)->e_machine);}
}
