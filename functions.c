#include "functions.h"
#include <math.h>

void split_line(char line[110], char** f_flechie, char** f_base, char** category, char** type)
{
    int i, type_size;
    
    char * token = strtok(line, "\t");
    *f_flechie = token;

    token = strtok(NULL, "\t");
    *f_base = token;
    token = strtok(NULL, "\t");

    *category = malloc(3 + 1);

    for(i = 0; i < 3; i++)
        (*category)[i] = token[i];
    (*category)[i] = '\0';

    type_size = strlen(token) - 3;
    *type = malloc(type_size + 1);

    for(i = 3; i <= type_size+1; i++)
        (*type)[i-3] = token[i];
    (*type)[i-3] = '\0';
    
    return;
}


int intVerify(int min, int max, char input[]){
    int integer = 0, index = strlen(input), asciiValue;
    char tmpChar;

    if(index == 0){
        printf("\nPlease type something first!\n");
        return -1;
    }

    do{
        tmpChar = input[--index];
        if(tmpChar==' '){
            printf("\nPlease don't use spaces!\n");
            return -1;
        }

        asciiValue = (int)tmpChar;
        if( asciiValue > 57 || asciiValue < 48 ) {
            printf("\nPlease use digits!\n");
            return -1;
        }

        integer += (asciiValue-48) * (int)pow(10,(int)strlen(input) - index-1) ; // integer += digit * 10^unit
    }while(index > 0);

    if(integer>max || integer<min) {
        printf("\nPlease use a number within range!\n");
        return -1;
    }else {
        return integer;
    }
}


//Function to translate a type from the .txt file to an binary code 
int conversion_type(char* type, int mode){
    int type_int = 0;
   
    char * temp = (char*)malloc((strlen(type)+1) * sizeof(char));
    strcpy(temp, type);
    char* subtype, *token = strtok(temp, "+");
   
    while(token != NULL){
        subtype = token;
        token = strtok(NULL, "+");
           
        // Binary table for type conversion :
        // +--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
        // |   10   |   9   |   8   |   7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
        // +--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
        // |  Inf   | IImp  | IPre  | SPre  |  P1   |  P2   |  P3   |  PL   |  SG   |  Fem  |  Mas  |
        // +--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
 
        if(!strcmp(subtype, "InvGen")){
            if(mode == 0){
                int random = rand() % 2;
                if(random)
                    type_int += 1;
                else
                    type_int += 2;
            }
            else
                type_int += 2 + 1;
               
           
        }
        else if(!strcmp(subtype, "Mas"))
            type_int += 1;
        else if(!strcmp(subtype, "Fem"))
            type_int += 2;
 
        else if(!strcmp(subtype, "InvPL")){
            if(mode == 0){
                int random = rand() % 2;
                if(random)
                    type_int += 4;
                else
                    type_int += 8;
            }
            else
                type_int += 4 + 8;
        }
 
        else if(!strcmp(subtype, "SG"))
            type_int += 4;
        else if(!strcmp(subtype, "PL"))
            type_int += 8;
 
        else if(!strcmp(subtype, "P3"))
            type_int += 16;
        else if(!strcmp(subtype, "P2"))
            type_int += 32;
        else if(!strcmp(subtype, "P1"))
            type_int += 64;
 
        else if(!strcmp(subtype, "SPre"))
            type_int += 128;
        else if(!strcmp(subtype, "IPre"))
            type_int += 256;
        else if(!strcmp(subtype, "IImp"))
            type_int += 512;
        else if(!strcmp(subtype, "Inf")){
            type_int += 1024;
            break;
        }
 
        else{ // for errors, unknown types, or excluded types
            type_int = -1;
            //printf("%s : Unknown subtype : %s\n", fflechie, subtype);
            break;
        }
   
    }
 
    return type_int;
}



char * conversion_string(int type, int mode){
 
    char * type_string = (char*)malloc(100 * sizeof(char));
   
 
    strcpy(type_string, "");
   
    char * MasFem = malloc(50 * sizeof(char));
    char * SGPL = malloc(50 * sizeof(char));
    char * Personne = malloc(50 * sizeof(char));
    char * Temps = malloc(50 * sizeof(char));
 
    if((type & 1) && (type & 2)){
        strcat(type_string, "InvGen+");
        strcat(MasFem, "invariable genre");
    }
    else{
    if(type & 1){
        strcat(type_string, "Mas+");
        strcat(MasFem, "masculin, ");
    }
    if(type & 2){
        strcat(type_string, "Fem+");
        strcat(MasFem, "feminin, ");
    }
    }
 
    if((type & 4) && (type & 8)){
        strcat(type_string, "InvPL+");
        strcat(SGPL, "invariable pluriel.");
    }
    else{
    if(type & 4){
        strcat(type_string, "SG+");
        strcat(SGPL, "singulier.");
    }
    if(type & 8){
        strcat(type_string, "PL+");
        strcat(SGPL, "pluriel.");
    }
    }
    if(type & 16){
        strcat(type_string, "P3+");
        strcat(Personne, "troisieme personne ");
    }
    if(type & 32){
        strcat(type_string, "P2+");
        strcat(Personne, "deuxieme personne ");
    }
    if(type & 64){
        strcat(type_string, "P1+");
        strcat(Personne, "premiere personne ");
    }
    if(type & 128){
        strcat(type_string, "SPre+");
        strcat(Temps, "subjonctif present, ");
    }
    if(type & 256){
        strcat(type_string, "IPre+");
        strcat(Temps, "infinitif present, ");
    }
    if(type & 512){
        strcat(type_string, "IImp+");
        strcat(Temps, "infinitif imparfait, ");
    }
    if(type & 1024){
        strcat(type_string, "Inf+");
        strcat(Temps, "infinitif, ");
    }
 
 
    if(mode == 0){
        if(type_string[strlen(type_string)-1] == '+')
        type_string[strlen(type_string)-1] = '\0';
        return type_string;
 
    }else{
        char * type_string_sentence = (char*)malloc(1000 * sizeof(char));
        strcpy(type_string_sentence, "");
 
        if(strlen(Temps) == 0){
            strcat(type_string_sentence, MasFem);
            strcat(type_string_sentence, SGPL);
 
        }else if(strlen(Temps) > 0){
            strcat(type_string_sentence, Temps);
            strcat(type_string_sentence, Personne);
            strcat(type_string_sentence, "du ");
            strcat(type_string_sentence, SGPL);
        }else {
            strcat(type_string_sentence, "Since it is an adverb, it doesn't have any 'formes fléchies'.\0");
           
        }
        strcat(type_string_sentence, "\0");
 
        return type_string_sentence;
         
    }
 
   
}

