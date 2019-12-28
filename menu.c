#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "menu.h"
#include "input_program.h"
#include "comandproces.h"
#include "prints.h"

//PRINT MENU
 
	void developmentresearch(int media , int file , int total){
			char *cadena=NULL;
			int error=0;
			int correct=0;
			double pmedia=0;
			double pfile=0;
			puts("Do you want to see the statistics behind your work? (Y/N):");
			cadena=get_string(&error);
			if (error){// hay error?
			printf("Error: %s\n",get_error_msg_input(error));
		
			}else{
				if (strcmp(cadena,"Y")==0){
					correct=1;	
					if(total!=0){
					pmedia=(media/total)*100;
					pfile=(file/total)*100;
				}
				//stadisticas
				
				puts("TOTAL NUMBER OF OPERATIONS:\n");
				printf("value---------->%d\n",total);
				puts(".....................................................005................................................................\n");
				puts("TOTAL NUMBER OF MEDIA OPERATIONS:\n");
				printf("value---------->%d\n",media);
				printf("percentage---------->%f\n",pmedia);
				puts(".....................................................005................................................................\n");
				puts("TOTAL NUMBER OF FILE USAGE OPERATIONS:\n");
				printf("value---------->%d\n",file);
				printf("percentage---------->%f\n",pfile);
				puts(".....................................................005................................................................\n");
				puts("... ---ABOUT THE USER--- ...\n\n");
				if(media>file){
					puts("... ---ABOUT THE USER--- ...\n\n");
					puts("... the main usage that had been given to the app this time is focus to MEDIA ...\n");
					
					
				}else if (media<file){
					puts("... ---ABOUT THE USER--- ...\n\n");
					puts("... the main usage that had been given to the app this time is focus to FILE MANAGEMENT ...\n");
					
				}else{
					puts("... ---ABOUT THE USER--- ...\n\n");
					puts("... at this time while executing the program the user had use exactly the ...\n");	
					puts("... same number of times FILE MANAGEMENT and MEDIA operations  ...\n");	
				}
			puts(".....................................................005................................................................\n");
			puts(".....................................................END................................................................\n");	
				
			}
			else if (strcmp(cadena,"N")==0){
				correct=1;
			}else {
				while(correct!=1){
					puts ("Wrong option choose Y or N :");
					cadena=get_string(&error);// Pedimos opcion
				if (error){// hay error?
					printf("Error: %s\n",get_error_msg_input(error));
				}else {
					if (strcmp(cadena,"Y")==0){
						correct=1;
						printRESyes();
						puts("TOTAL NUMBER OF OPERATIONS:\n");
						printf("value---------->%d\n",total);
						puts(".....................................................005................................................................\n");
						puts("TOTAL NUMBER OF MEDIA OPERATIONS:\n");
						printf("value---------->%d\n",media);
						printf("percentage---------->%f\n",pmedia);
						puts(".....................................................005................................................................\n");
						puts("TOTAL NUMBER OF FILE USAGE OPERATIONS:\n");
						printf("value---------->%d\n",file);
						printf("percentage---------->%f\n",pfile);
						puts(".....................................................005................................................................\n");
						puts("... ---ABOUT THE USER--- ...\n\n");
						if(media>file){
							puts("... ---ABOUT THE USER--- ...\n\n");
							puts("... the main usage that had been given to the app this time is focus to MEDIA ...\n");
							
							
						}else if (media<file){
							puts("... ---ABOUT THE USER--- ...\n\n");
							puts("... the main usage that had been given to the app this time is focus to FILE MANAGEMENT ...\n");
							
						}else{
							puts("... ---ABOUT THE USER--- ...\n\n");
							puts("... at this time while executing the program the user had use exactly the ...\n");	
							puts("... same number of times FILE MANAGEMENT and MEDIA operations  ...\n");	
						}
					puts(".....................................................005................................................................\n");
					puts(".....................................................END................................................................\n");
					}else if (strcmp(cadena,"N")==0){
						correct=1;
						
					}else{
					correct=0;
					}
				}
			}
	    }
		}
		
	free(cadena);	
	}
	void print_help(){
		int correct=0;
		char *cadena=NULL;
		int error=0;
		printhelpS();
		cadena=get_string(&error);// Pedimos opcion
		if (error){// hay error?
			printf("Error: %s\n",get_error_msg_input(error));
		
		}else{
			if (strcmp(cadena,"Y")==0){
				correct=1;
				printhelpL();
			}
			else if (strcmp(cadena,"N")==0){
			correct=1;
			printmenu();
			}else {
			while(correct!=1){
				puts ("More information? (Y/N):");
				cadena=get_string(&error);// Pedimos opcion
				if (error){// hay error?
					printf("Error: %s\n",get_error_msg_input(error));
				}else {
					if (strcmp(cadena,"Y")==0){
						correct=1;
						printhelpL();
					}else if (strcmp(cadena,"N")==0){
						correct=1;
						printmenu();
					}else{
					correct=0;
					}
				}
			}
	    }
	}
	free(cadena);
}
		
// ASK_COMAND	

comandos_t ask_command(){
	comandos_t c;
	c.opcional.arg_opt_long=0;
	c.opcional.arg_opt_char=NULL;
	c.parameter=NULL;
	c.parameter2=NULL;
	c.id=0;
	
	char *cadena=NULL;
	int error=0;
	
	//long selection;

	do {  
		 
	puts("Enter an Option : ");
	
	error=0;
	
	cadena=get_string(&error);// Pedimos nombre
	
	if(error){
		
		printf("Error: %s\n",get_error_msg_input(error));

		if (error == ERR_CTRLD_INPUT){
			
			c.comando=EXIT;// Si se introduce un CTRLD se sale del programa
		  
		  return c;
		}
		
	}else{
		
		char **buffer=stringcut(cadena,&error);
		if(error){
			printf("Error: %s\n",get_error_msg_comp(error));

		}else{
			c=asigncomand(&error,buffer);
			
			if(error){
				
			printf("Error: %s\n",get_error_msg_comp(error));
			
			}else{
				
				return c;
			}
		}
	
	}	
	}while(error!=0);
	
	
	return c;
	
	
}

void clear_comando(comandos_t comando){
	
	if (comando.opcional.arg_opt_char!=NULL){
		free(comando.opcional.arg_opt_char);
		comando.opcional.arg_opt_char = NULL;
	}
	
	if (comando.parameter!=NULL){
		if(strcmp(comando.parameter,"")==0){
			comando.parameter = NULL;
			
		}else{
			free(comando.parameter);
			comando.parameter = NULL;
		}
	}
	if (comando.parameter2!=NULL){
		if(strcmp(comando.parameter2,"")==0){
			comando.parameter = NULL;
			
		}else{
		free(comando.parameter2);
		comando.parameter2 = NULL;
		}
	}
	
}

	



