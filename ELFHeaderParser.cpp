#include"ELFHeaderParser.h"

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
int parseEndian(void* myfileEhdr,int is_64, char* result)
{
    printf("parse endian\n");
    int endian = 0;
    if (is_64){
        endian = ((Elf64_Ehdr*)myfileEhdr)->e_ident[EI_DATA];
    }
    else{
        endian = ((Elf32_Ehdr*)myfileEhdr)->e_ident[EI_DATA];
    }
    switch(endian){
        case ELFDATA2LSB:
            strncpy(result,"LSB",4);
	        break;
        case ELFDATA2MSB:
            strncpy(result,"MSB",4);
	        break;
        default:
            strncpy(result,"not found",10);
    }
}
int parseFileType(void* myfileEhdr,int is_64, char* result)
{
    printf("parse file type\n");
    int type = 0;
    if (is_64){
        type = ((Elf64_Ehdr*)myfileEhdr)->e_type;
    }
    else{
        type = ((Elf32_Ehdr*)myfileEhdr)->e_type;
    }
    switch(type){
        case ET_EXEC:
            strncpy(result,"executable file",16);
	        break;
        default:
            strncpy(result,"not a executable file",22);
    }
}

int parseBitformat(void* myfileEhdr,int is_64, char* result)
{
    printf("parse bitformat\n");
    int format = 0;
    if (is_64){
        format = ((Elf64_Ehdr*)myfileEhdr)->e_ident[EI_CLASS];
    }
    else{
        format = ((Elf32_Ehdr*)myfileEhdr)->e_ident[EI_CLASS];
    }
    switch(format){
        case ELFCLASS32:
            strncpy(result,"32-bit",7);
	        break;
        case ELFCLASS64:
            strncpy(result,"64-bit",7);
	        break;
        default:
            strncpy(result,"not found",10);
    }
}

int parseMachine(void* myfileEhdr,int is_64, char* result)
{
    printf("parse machine\n");
    int machID = 0;
    if (is_64){
        machID = ((Elf64_Ehdr*)myfileEhdr)->e_machine;
    }
    else{
        machID = ((Elf32_Ehdr*)myfileEhdr)->e_machine;
    }
    
    switch(machID)
    {
        case EM_NONE:
            strncpy(result,"no specific instruction set",28);
	        break; 

        case EM_386:
            strncpy(result,"x86",4);
            break;

        case EM_X86_64:
            strncpy(result,"x86-64",7);
            break;

        case EM_ARM:
        case EM_AARCH64:
            strncpy(result,"ARM",4);
            break;

        case EM_PPC: 
        case EM_PPC64:
            strncpy(result,"PowerPC",8);
            break;

        case EM_MIPS_RS3_LE:
        case EM_MIPS:
            strncpy(result,"MIPS",5);
            break;

        case EM_68K: 
        case EM_88K:
        case EM_RCE:
            strncpy(result,"Motorola m68k",14);
            break;

        case EM_SPARC:
        case EM_SPARCV9:
            strncpy(result,"SPARC",6);
            break;

        default:
            strncpy(result,"not support",12);

    }
    

    return 0;
}

int findSection(ELFinfo* myELFinfo,char* sName,int64_t& secOffset,int64_t& secSize)
{
    printf("find section\n");
    int16_t secSymbolInd;
    int64_t secNameBase;
    if (myELFinfo->is_64){
        secSymbolInd = ((Elf64_Ehdr*)myELFinfo->fileEhdr)->e_shstrndx;
    }
    else{
        secSymbolInd = ((Elf32_Ehdr*)myELFinfo->fileEhdr)->e_shstrndx;
    }

    printf("section name ind %d\n",secSymbolInd);

    node* now = myELFinfo->fileShdr->next;
    for (int ind=0;ind<secSymbolInd;ind++)
    {
        now=now->next;
    } 

    if (myELFinfo->is_64){
        secNameBase = ((Elf64_Shdr*)now->info)->sh_offset;
    }
    else{
        secNameBase = ((Elf32_Shdr*)now->info)->sh_offset;
    }
    printf("section name offset %x\n",(int)secNameBase);

    int32_t nameInd;
    now = myELFinfo->fileShdr->next;
    for (int ind=0;;)
    {
        if (now == NULL)break;
        if (myELFinfo->is_64){
            nameInd = ((Elf64_Shdr*)now->info)->sh_name;
            secOffset = ((Elf64_Shdr*)now->info)->sh_offset;
            secSize = ((Elf64_Shdr*)now->info)->sh_size;

        }
        else{
            nameInd = ((Elf32_Shdr*)now->info)->sh_name;
            secOffset = ((Elf32_Shdr*)now->info)->sh_offset;
            secSize = ((Elf32_Shdr*)now->info)->sh_size;
        }
        printf("section: %s, Offset: %x\n",&myELFinfo->fileBuf[secNameBase+nameInd],(int)secOffset);
        if(!strncmp(&myELFinfo->fileBuf[secNameBase+nameInd],sName,strlen(sName)))
        {
            printf("Find section !! %s",sName);
            return 1;
        }
        now = now->next;
    }   
    return 0;
}
int parseLinkType(ELFinfo* myELFinfo,char* result){

    printf("parse link type");
    int64_t secOffset;
    int64_t secSize;
    int64_t stringOffset;
    int64_t stringSize;
    if (!(findSection(myELFinfo,(char*)".dynamic",secOffset,secSize)))
    {
        strncpy(result,"static-link",12);
        return 1;
    }
    strncpy(result,"dynamic-link",13);
    return 1;
}
int parseSymbol(ELFinfo* myELFinfo,node* symboList)
{

    printf("parse symbol\n");
    int64_t secOffset;
    int64_t secSize;
    int64_t stringOffset;
    int64_t stringSize;
    int structSize = 0;
    if (!(findSection(myELFinfo,(char*)".symtab",secOffset,secSize) && \
            findSection(myELFinfo,(char*)".strtab",stringOffset,stringSize)))
    {
        printf("no symbol table found");
    }
    printf("section offset: %x, section size: %x\n",(int)secOffset,(int)secSize);

    if(myELFinfo->is_64){
        structSize=sizeof(Elf64_Sym);
    }
    else{
        structSize=sizeof(Elf32_Sym);
    }
    void* now;
    node* nowSymboList = symboList;
    char* srttmp;

    for (int ind = 0;ind<secSize;ind+=structSize){
        now = (char*)&myELFinfo->fileBuf[secOffset+ind];
        if (myELFinfo->is_64){
            srttmp= &myELFinfo->fileBuf[((Elf64_Sym*)now)->st_name+stringOffset];
        }
        else {
            srttmp= &myELFinfo->fileBuf[((Elf32_Sym*)now)->st_name+stringOffset];
        }

        if ((!(strstr(srttmp,(char*)"GLIBC"))) || (strchr(srttmp,'_') == srttmp) || (!strcmp(srttmp,""))){
                continue;
        }
        printf("%s\n",srttmp);
        node* sym = (node*)malloc(sizeof(node));
        sym->info = srttmp;
        sym->next = NULL;
        nowSymboList->next = sym;
        nowSymboList = sym;
    }
    return 0;
}

int parseDynSymbol(ELFinfo* myELFinfo,node* dynSymboList)
{
    printf("parse dynsymbol\n");
    int64_t secOffset;
    int64_t secSize;
    int64_t stringOffset;
    int64_t stringSize;
    int structSize = 0;
    if (!(findSection(myELFinfo,(char*)".dynsym",secOffset,secSize) && \
            findSection(myELFinfo,(char*)".dynstr",stringOffset,stringSize)) )
    {
        printf("no dynamic symbol table found\n");
    }
    printf("section offset: %x, section size: %x\n",(int)secOffset,(int)secSize);
    
    if(myELFinfo->is_64){
        structSize=sizeof(Elf64_Sym);
    }
    else{
        structSize=sizeof(Elf32_Sym);
    }

    void* now;
    node* nowSymboList = dynSymboList;
    char* srttmp;

    for (int ind = 0;ind<secSize;ind+=structSize){
        now = (char*)&myELFinfo->fileBuf[secOffset+ind];
        if (myELFinfo->is_64){
            srttmp= &myELFinfo->fileBuf[((Elf64_Sym*)now)->st_name+stringOffset];
        }
        else {
            srttmp= &myELFinfo->fileBuf[((Elf32_Sym*)now)->st_name+stringOffset];
        }


        if (((strstr(srttmp,(char*)"__"))) || strchr(srttmp,'_') == srttmp || (!strcmp(srttmp,"")) ){
                continue;
        }
        printf("%s\n",srttmp);
        node* sym = (node*)malloc(sizeof(node));
        sym->info = srttmp;
        sym->next = NULL;
        nowSymboList->next = sym;
        nowSymboList = sym;
    }
    return 0;
}
int parseDynLib(ELFinfo* myELFinfo,node* dynLibList){
    printf("parse dynlib\n");
    int64_t secOffset;
    int64_t secSize;
    int64_t stringOffset;
    int64_t stringSize;
    int structSize = 0;

    if (!(findSection(myELFinfo,(char*)".dynamic",secOffset,secSize) && \
            findSection(myELFinfo,(char*)".dynstr",stringOffset,stringSize)) )
    {
        printf("not a dynamic link file\n");
    }
    printf("section offset: %x, section size: %x\n",(int)secOffset,(int)secSize);
    if(myELFinfo->is_64){
        structSize=sizeof(Elf64_Dyn);
    }
    else{
        structSize=sizeof(Elf32_Dyn);
    }

    void* now;
    node* nowSymboList = dynLibList;
    char* srttmp;
    for (int ind = 0;ind<secSize;ind+=structSize){
        now = (char*)&myELFinfo->fileBuf[secOffset+ind];
        if (myELFinfo->is_64){
            if (((Elf64_Dyn*)now)->d_tag == DT_NEEDED ){
                srttmp= &myELFinfo->fileBuf[((Elf64_Dyn*)now)->d_un.d_val+stringOffset];
            }
            else{continue;}
        }
        else {
            if (((Elf64_Dyn*)now)->d_tag == DT_NEEDED ){
                srttmp= &myELFinfo->fileBuf[((Elf32_Dyn*)now)->d_un.d_val+stringOffset];
            }
            else{continue;}
        }
        printf("%s\n",srttmp);
        node* sym = (node*)malloc(sizeof(node));
        sym->info = srttmp;
        sym->next = NULL;
        nowSymboList->next = sym;
        nowSymboList = sym;
    }
    return 0;
}

int ELFtoCapstoneArch(char* arch,int is_64,cs_arch& capstArch,cs_mode& bitFormat ){

    printf("ELF to capstone\n");
// typedef enum ELFtoCapstone {
// 	CS_ARCH_ARM = 0,	// ARM architecture (including Thumb, Thumb-2)
// 	CS_ARCH_ARM64,		// ARM-64, also called AArch64
// 	CS_ARCH_MIPS,		// Mips architecture
// 	CS_ARCH_X86,		// X86 architecture (including x86 & x86-64)
// 	CS_ARCH_PPC,		// PowerPC architecture
// 	CS_ARCH_SPARC,		// Sparc architecture
// 	CS_ARCH_SYSZ,		// SystemZ architecture
// 	CS_ARCH_XCORE,		// XCore architecture
// 	CS_ARCH_MAX,
// 	CS_ARCH_ALL = 0xFFFF, // All architectures - for cs_support()
// } cs_arch;
    if (!memcmp(arch ,"x86",sizeof(arch))){
        bitFormat =  CS_MODE_32;
        capstArch = CS_ARCH_X86;
        return 1;
    }
    else if(memcmp(arch,"x86-64",sizeof(arch))){
        bitFormat =  CS_MODE_64;
        capstArch = CS_ARCH_X86;
        return 1;
    }
    else if(memcmp(arch,"ARM",sizeof(arch))){
        if (is_64){
            bitFormat =  CS_MODE_64;
            capstArch = CS_ARCH_ARM;   
        }
        else{
            bitFormat =  CS_MODE_32;
            capstArch = CS_ARCH_ARM;
        }
        return 1;
    }
    else if(memcmp(arch,"PowerPC",sizeof(arch))){
        bitFormat =  CS_MODE_32;
        capstArch = CS_ARCH_PPC;
        return 1;
    }
    else if(memcmp(arch,"MIPS",sizeof(arch))){
        if (is_64){
            bitFormat =  CS_MODE_64;
            capstArch = CS_ARCH_MIPS;
        }
        else{
            bitFormat =  CS_MODE_32;
            capstArch = CS_ARCH_MIPS;
        }
        return 1;
    }
    else if(memcmp(arch,"Motorola m68k",sizeof(arch))){
        bitFormat =  CS_MODE_32;
        capstArch = CS_ARCH_ALL;
        return 1;
    }
    else if(memcmp(arch,"SPARC",sizeof(arch))){
        bitFormat =  CS_MODE_32;
        capstArch = CS_ARCH_SPARC;
        return 1;
    }
    else{
        return 0;
    }
    return 0;
}

int disassTextSection(ELFinfo* myELFinfo,FILE *outputFp){
    printf("diss\n");
    int64_t secOffset;
    int64_t secSize;
    if (!(findSection(myELFinfo,(char*)".text",secOffset,secSize)) )
    {
        printf("not a dynamic link file\n");
    }
    uint8_t* binaddr = (uint8_t*)&(myELFinfo->fileBuf[secOffset]);


    char archBuf[MAX_BUF];
    cs_arch capstArch;
    cs_mode bitFormat;
    parseMachine(myELFinfo->fileEhdr,myELFinfo->is_64,archBuf);
    ELFtoCapstoneArch(archBuf,myELFinfo->is_64,capstArch,bitFormat);
    printf("\n---QQQ----\n");
    printf("%x, %x,%x,%x\n",binaddr,(int)secSize,(unsigned int)capstArch, (unsigned int)bitFormat );
    
    disassBinary(binaddr, secSize,capstArch, bitFormat,outputFp );
}



int createJson(ELFinfo* myELFinfo,char* resultPath)
{
    
    printf("create json\n");
    int64_t secOffset;
    node* symboList = (node*)malloc(sizeof(node));
    node* dynSymboList = (node*)malloc(sizeof(node));
    node* dynLibList = (node*)malloc(sizeof(node));
    parseSymbol(myELFinfo,symboList);
    parseDynSymbol(myELFinfo,dynSymboList);
    parseDynLib(myELFinfo,dynLibList);
    char buf[MAX_BUF];
    
    // printf("%s",buf);

    json_object *json_obj = NULL;
	json_object *tmp_obj = NULL;
	json_object *array_obj = NULL;

    //new a base object
	json_obj = json_object_new_object();
    tmp_obj = json_object_new_string("string");
	json_object_object_add(json_obj, "name", tmp_obj);

    parseFileType(myELFinfo->fileEhdr,myELFinfo->is_64,buf);
    tmp_obj = json_object_new_string(buf);
    json_object_object_add(json_obj, "type", tmp_obj);

    parseMachine(myELFinfo->fileEhdr,myELFinfo->is_64,buf);
    tmp_obj = json_object_new_string(buf);
    json_object_object_add(json_obj, "machine", tmp_obj);

    parseBitformat(myELFinfo->fileEhdr,myELFinfo->is_64,buf);
    tmp_obj = json_object_new_string(buf);
    json_object_object_add(json_obj, "bit-format", tmp_obj);

    parseEndian(myELFinfo->fileEhdr,myELFinfo->is_64,buf);
    tmp_obj = json_object_new_string(buf);
    json_object_object_add(json_obj, "endian", tmp_obj);

    parseLinkType(myELFinfo,buf);
    tmp_obj = json_object_new_string(buf);
    json_object_object_add(json_obj, "link-type", tmp_obj);

    array_obj = json_object_new_array();
    for (node* ind=symboList->next; ind!=NULL;){
        tmp_obj = json_object_new_string((char*)(ind->info));
	    json_object_array_add(array_obj, tmp_obj);
        ind=ind->next;
    }
    for (node* ind=dynSymboList->next; ind!=NULL;){
        tmp_obj = json_object_new_string((char*)(ind->info));
	    json_object_array_add(array_obj, tmp_obj);
        ind=ind->next;
    }
	//add array to the base object
	json_object_object_add(json_obj, "symbol", array_obj);

    array_obj = json_object_new_array();
    for (node* ind=dynLibList->next; ind!=NULL;){
        tmp_obj = json_object_new_string((char*)(ind->info));
	    json_object_array_add(array_obj, tmp_obj);
        ind=ind->next;
    }
	//add array to the base object
	json_object_object_add(json_obj, "dynamic link library", array_obj);

    json_object_to_file(resultPath, json_obj);
    return true;
}