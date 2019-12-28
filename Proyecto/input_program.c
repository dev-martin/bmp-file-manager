 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "input_program.h"


//ERRORS

#define SUCCESS_INPUT 0
#define ERR_NO_NUMBERS_INPUT -1
#define ERR_FURTHER_CHARS_INPUT -2
#define ERR_CTRLD_INPUT -3

#define NUM_INPUT_ERROR_MSGS 5

//ARRAY ERRORS
static const char *error_map[NUM_INPUT_ERROR_MSGS +1] =  {  "Success" ,"No digits were found","Further characters after number", "CTRL+D was pressed (EOF)", "Unknown error"};
//VARIABLE GLOBAL

char *cadena=NULL;
size_t n_alloc=0;

//FUNCION
void clean_module_input_program(){
  if (cadena!=NULL){
    free(cadena);
    n_alloc=0;
    cadena=NULL;
  }
}
//PRINT ERRORS
const char *get_error_msg_input(int error){
  if(errno==0 && (error<0 || error>-4)){
    const char* c;
    
    c=error_map[-error];
    return c;
  }else{
		 return	strerror(error);	
   }
	
	
	
}
// ASK FOR A STRING
char *get_string(int *error){
	errno=0;
	
	ssize_t n_bits=0; // number of bits 
	n_bits=getline(&cadena,&n_alloc,stdin); 
	
	if (n_bits==-1){// getline returns -1 in case of a CTRLD or a error 
		if(errno==0){
			//CTR+D
			*error=ERR_CTRLD_INPUT;
			clean_module_input_program();
			
		}else{
			*error=-4;//introducimos -4 como unkonown error por que no esta definido	
		} 
		cadena=NULL;
	}else if(n_bits==1){
		cadena[strlen(cadena)-1]='\0';	//notese que recortamos aqui la cadena 

		puts("Intro introduced");
		*error=-4;//introducimos -4 como unkonown error por que no esta definido
	  
    }else if(n_bits>1){
		cadena[strlen(cadena)-1]='\0';		//notese que recortamos aqui la cadena 
		*error=SUCCESS_INPUT;
    }

	return cadena;

}

long get_number(int base, int *error){

	char *cpy; //char* auxiliar para el strtol
	long val=0;
	ssize_t n_bits;// number of bits
	
	errno=0;
	n_bits=getline(&cadena,&n_alloc,stdin);// call to getline

	if (n_bits==-1){// getline returns -1 in case of a CTRLD or a error 
		if(errno==0){
			//CTR+D
			*error=ERR_CTRLD_INPUT;
			clean_module_input_program();
		}else{
			*error=-4;//introducimos -4 como unkonown error por que no esta definido
		} 
  }else if(n_bits==1){
		puts("Intro introduced");
		cadena[strlen(cadena)-1]='\0';	
		*error=-4;//introducimos -4 como unkonown error por que no esta definido
		

  }else if(n_bits>1){
    cadena[strlen(cadena)-1]='\0';	
    val=strtol(cadena,&cpy,base);
    if (strcmp(cpy,cadena)==0){//ambas son iguales es decir no ha extraido ningun numero de la cadena
		*error=ERR_NO_NUMBERS_INPUT; 
		 val=0;
    }else{
      if(strlen(cpy)==0){
		*error=SUCCESS_INPUT;				
      }else if(strlen(cpy)>0){
		*error=ERR_FURTHER_CHARS_INPUT;
		 val=0;
      }	
    }
  }			
	
  return val;				
}
	
	
	



