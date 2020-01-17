#include"ELFHeaderParser.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdbool.h>
#include <cstdlib>
#include <iostream>



bool RegisterAllHeader(ElfArch* fArch, ELFinfo* myELFinfo){

    int16_t phoff;
    int16_t phentsize;
    int16_t phnum;

    int16_t shoff;
    int16_t shentsize;
    int16_t shnum;

    if (fArch->e_ident[EI_CLASS] == ELFCLASS32){
        myELFinfo->fileEhdr = myELFinfo->fileBuf;
        myELFinfo->is_64 = 0;

        phoff = ((Elf32_Ehdr*)myELFinfo->fileEhdr)->e_phoff;
        phentsize = ((Elf32_Ehdr*)myELFinfo->fileEhdr)->e_phentsize;
        phnum = ((Elf32_Ehdr*)myELFinfo->fileEhdr)->e_phnum;
        
        shoff = ((Elf32_Ehdr*)myELFinfo->fileEhdr)->e_shoff;
        shentsize = ((Elf32_Ehdr*)myELFinfo->fileEhdr)->e_shentsize;
        shnum = ((Elf32_Ehdr*)myELFinfo->fileEhdr)->e_shnum;
    }
    else if (fArch->e_ident[EI_CLASS] == ELFCLASS64) {
        myELFinfo->fileEhdr = myELFinfo->fileBuf;
        myELFinfo->is_64 = 1;

        phoff =((Elf64_Ehdr*)myELFinfo->fileEhdr)->e_phoff;
        phentsize = ((Elf64_Ehdr*)myELFinfo->fileEhdr)->e_phentsize;
        phnum =((Elf64_Ehdr*)myELFinfo->fileEhdr)-> e_phnum;

        shoff =((Elf64_Ehdr*)myELFinfo->fileEhdr)->e_shoff;
        shentsize = ((Elf64_Ehdr*)myELFinfo->fileEhdr)->e_shentsize;
        shnum =((Elf64_Ehdr*)myELFinfo->fileEhdr)-> e_shnum;
    }
    else{printf("not match 32 or 64-bit format");return false;}

    myELFinfo->filePhdr = (node*)malloc(sizeof(node));
    node* now = myELFinfo->filePhdr;
    for(int ind=0 ; ind < phnum ; ind+=1){
        node* pNode = (node*)malloc(sizeof(node));
        pNode->info = &(myELFinfo->fileBuf[phoff]);
        pNode->next = NULL;
        
        printf("program headers startAddr: %x, size: %x, num: %x\n",phoff,phentsize,phnum);
    
        now->next = pNode;
        now = pNode;
        phoff += phentsize;
    }
    
    myELFinfo->fileShdr = (node*)malloc(sizeof(node));
    now = myELFinfo->fileShdr;
    for(int ind=0 ; ind < shnum ; ind+=1){
        node* sNode = (node*)malloc(sizeof(node));
        sNode->info = &(myELFinfo->fileBuf[shoff]);
        sNode->next = NULL;
        
        printf("Section headers startAddr: %x, size: %x, num: %x\n",shoff,shentsize,shnum);
    
        now->next = sNode;
        now = sNode;
        shoff += shentsize;
    }

    
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

    // check elf magic number and executable
    if (!memcmp(fArch->e_ident,"\x7fELF",4) || ((fArch->e_type) != ET_EXEC )){
        printf("Not a ELF file\n");
        return false;
    }
    if (!RegisterAllHeader(fArch,myELFinfo)){
        return false;
    }
    if (myELFinfo->is_64){
        printf("%x\n",((Elf64_Ehdr*)myELFinfo->fileEhdr)->e_type);
        printf("%x\n",((Elf64_Ehdr*)myELFinfo->fileEhdr)->e_shstrndx);
        printf("%x\n",(((Elf64_Phdr*)myELFinfo->filePhdr)->p_type));

        node* now = myELFinfo->filePhdr->next;
        for(;now != NULL;){
            printf("%x, ",((Elf64_Phdr*)now->info)->p_type);
            now = now->next;
        }
        printf("\n");
        now = myELFinfo->fileShdr->next;
        for(;now != NULL;){
            printf("%x, ",((Elf64_Shdr*)now->info)->sh_name);
            now = now->next;
        }
    }
    createJson(myELFinfo);
}
