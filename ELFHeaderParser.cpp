#include"ELFHeaderParser.h"

int parseMachine(void* myfileEhdr,int is_64, char* result)
{
    int machID = 0;
    if (is_64){
        machID = ((Elf32_Ehdr*)myfileEhdr)->e_machine;
    }
    else{
        machID = ((Elf64_Ehdr*)myfileEhdr)->e_machine;
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


int createJson(ELFinfo* myELFinfo)
{
    char machineName[30];
    parseMachine(myELFinfo->fileEhdr,myELFinfo->is_64,machineName);
    printf("%s",machineName);

    json_object *json_obj = NULL;
	json_object *tmp_obj = NULL;

    //new a base object
	json_obj = json_object_new_object();
    tmp_obj = json_object_new_string("string");
	json_object_object_add(json_obj, "name", tmp_obj);
    tmp_obj = json_object_new_string(machineName);
    json_object_object_add(json_obj, "type", tmp_obj);

    json_object_to_file("test.json", json_obj);

    return true;
}