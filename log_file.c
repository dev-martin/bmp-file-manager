#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include "input_program.h"
#include "configuration_file.h"



//ATENCION, SI USAIS LOG EN FUNCIONES MÁS ALLÁ DEL MAIN QUE ES DONDE SE HACE LA LLAMADA AL VALOR DE LOGON, TENEÍS QUE PASAROS ESE VALOR COMO ARGUMENTO EN
// LOS METODOS QUE LLAMEIS PORQUE SI NO NO SABREÍS SI LOGON ESTÁ ACTIVADO O NO

//LOGON Y LOGOFF BASICAMENTE SERIA CAMBIAR EL VALOR DE 0 A 1 Y VICEVERSA, NO NECESITA MÁS



//PARTS OF THE CODE ADAPTED FROM: https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.3.0/com.ibm.zos.v2r3.bpxbd00/vfprtf.htm
//https://cboard.cprogramming.com/c-programming/146390-trim-last-character-string.html

       //ESTE ES EL FORMATO DE LOS MENSAJES DEL LOG                                         
FILE* init_log (struct confArguments configargs, int* error); 
void log_print(FILE* stream, int debug, char* fmt, ...); 
void log_entry(FILE* stream, int debug, char* fmt, ...);    
char* LastcharDel(char* name);

FILE* init_log (struct confArguments configargs, int* error){ //SE DEVUELVE UN FILE* PARA PODER CERRAR EL ARCHIVO AL FINAL DE PROGFRAMA Y EVITAR MEMORY LEAKS
	
	int verify = 0;
	int correct = 0;
	FILE* log;
	
	if(configargs.l == NULL ) {
		verify = 1;
	}
	
	while (correct == 0){
		
		if(verify == 1){
			printf ("There has been an error managing the log file. Please, type again the path of your log file.\n");
			configargs.l = get_string (error);
			
				if(*error == ERR_CTRLD_INPUT){
					exit (-1); //OJO, NO ES EXIT SIN MÁS, ES LO QUE HAGAIS CUANDO SEA CTRL-D
				}
				
			char aux_path[PATH_MAX]; 
			realpath(configargs.l, aux_path);
			
			if(aux_path != NULL){
				configargs.l = strdup(aux_path);
			}
		}

		log = fopen(configargs.l, "w");
		
			if (log == NULL){
				verify = 1;
			}else{
				verify = 0;
				correct = 1;
				}
	}
	
	
	return log;
}
		
char* LastcharDel(char* name){
    int i = 0;
    while(name[i] != '\0')
    {
        i++;
         
    }
    name[i-1] = '\0';
    return name;
}
                                                         
void log_entry(FILE* stream, int logon, char* fmt, ...){                                                                               
   va_list arg_ptr;                                                             
                                                                                
   va_start(arg_ptr, fmt);                                                      
   vfprintf(stream, fmt, arg_ptr);  
   if(logon == 1){
	    va_list arg_ptr2;
		va_copy(arg_ptr2, arg_ptr);
		va_start(arg_ptr2, fmt); 
    	vprintf(fmt, arg_ptr2);  
    	va_end(arg_ptr2);
    }							
   va_end(arg_ptr);                                                             
}   



//ESTO NO VALE YA PA NAH, SI LO QUEREIS PARA --DEBUG OS LO REGALAMOS
/*void log_print(FILE* stream, int debug, char* fmt, ...){                                                                               
   va_list arg_ptr;                                                             
                                                                                
   va_start(arg_ptr, fmt);                                                      
   vprintf(fmt, arg_ptr);   								
   va_end(arg_ptr);                                                             
}*/
                                                                       
/*int main(){    
	int error = 0;   
	struct confArguments confargs;
	confargs.l = "/home/astt/.saucem/bitacora";          //ESTO ESTA PUESTO DE PRUEBA, PROBAD CON LO QUE QUERAÍS                                                        
   FILE *stream;     
         
   time_t curtime;

   time(&curtime);
                                                  
   stream = init_log(confargs, &error);    
   printf("ERROR: %d\n", error);                                  
                                                                                
   log_entry(stream, 0, fmt1, LastcharDel(ctime(&curtime)), "log_file.c", "log_entry", "61", "Log entry typed");  
   log_entry(stream, 1, fmt1, LastcharDel(ctime(&curtime)), "collection.c", "add_op", "101", "Operation zeros added"); //LOG ENTRY CON LOGON ENCENDIDO 
   log_entry(stream, 0, fmt1, LastcharDel(ctime(&curtime)), "bmp_ops.c", "zeros_bmp", "508", "Image set to zeros");     
   
   fclose(stream);                                     
}*/                                                                       
                                                                                
                   
