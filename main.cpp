#include"ELFHeaderParser.h"
#include"disassem.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdbool.h>
#include <cstdlib>
#include <iostream>





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
    if (argc != 4){
        printf("Usage: ELFParser [FilePath] [ResultPath] [disassemblePath]\n");
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

        // node* now = myELFinfo->filePhdr->next;
        // for(;now != NULL;){
        //     printf("%x, ",((Elf64_Phdr*)now->info)->p_type);
        //     now = now->next;
        // }
        // printf("\n");
        // now = myELFinfo->fileShdr->next;
        // for(;now != NULL;){
        //     printf("%x, ",((Elf64_Shdr*)now->info)->sh_name);
        //     now = now->next;
        // }
    }
    createJson(myELFinfo,argv[2]);
    //char disassFileName[] = "./disassemble.txt";
    FILE* outputFp = fopen(argv[3], "w");
    disassTextSection(myELFinfo,outputFp);
    fclose(outputFp);
}
