//Concurrent
//codigo hijo
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "menu.h"
#include <sys/types.h>
#include "collection.h"
#include "saucem_ops.h"
#include "comandproces.h"
#include "saucem_ops.h"
void hijorun(struct collection my_collection, int pos){
	
	char **buffer=NULL;
	char* cadena =NULL;
	cadena=malloc(sizeof(char)*1000);
	int error=0;
	int nbytes=0;
	puts("sale proceso hijo concu");
	while (strcmp(cadena,"exit")!=0){
		
			nbytes = read(my_collection.tabla[pos].fd[0], cadena, strlen(cadena));
		
		if(nbytes>0){
		//metodo de llamar una ooperacion.
		puts("leealgo");
		buffer=stringcut(cadena,&error);
		comandos_t o=asigncomand(&error,buffer);
		my_collection=add_op( my_collection,o.id,o.opcional.arg_opt_char,o.parameter,o.parameter2,o.top_left , o.bottom_right ,&error);
		}
		
	}
	//liberar recursos
	free(cadena);
	puts("muere hijo");
	

}

void padrerun(struct collection my_collection,int comando,char* operation,char* parametro1,char* parametro2,POINT p1,POINT p2, int id ){	

	int position=0;
	char* cadena= NULL;
	cadena=(char*)malloc(sizeof(char)*(100));
	
	if(comando==ADD_OP){
			
			find_item_id_collection( my_collection,id,&position);
			
			if(strcmp(operation,"zero")==0){
				sprintf(cadena,"add_op %d %s %s",id,operation,parametro1);
				write(my_collection.tabla[position].fd[1],cadena,strlen(cadena));
				
			} 
			//if(strcmp(operation,"crop")==0){
				/*
				sprintf(cadena,"%s|%d|%d|%d|%d-",operation,p1.x,p1.y,p2.x,p2.y);
				write(my_collection.tabla[position].fd[1],addop,strlen(addop)+1);
				write(my_collection.tabla[position].fd[1],cadena,strlen(cadena)+1);
			}
									
			if(strcmp(operation,"hist")==0){
				sprintf(cadena," %d %s %s",id,operation,parametro1);
				write(my_collection.tabla[position].fd[1],addop,strlen(addop)+1);
				write(my_collection.tabla[position].fd[1],cadena,strlen(cadena)+1);
			}
			if(strcmp(operation,"mirror")==0){
				sprintf(cadena," %d %s %s",id,operation,parametro1);
				write(my_collection.tabla[position].fd[1],addop,strlen(addop)+1);
				write(my_collection.tabla[position].fd[1],cadena,strlen(cadena)+1);
			}
			if(strcmp(operation,"gray")==0){
				sprintf(cadena," %d %s %s",id,operation,parametro1);
				write(my_collection.tabla[position].fd[1],addop,strlen(addop)+1);
				write(my_collection.tabla[position].fd[1],cadena,strlen(cadena)+1);
			}
			if(strcmp(operation,"level")==0){
				sprintf(cadena," %d %s %s",id,operation,parametro1);
				write(my_collection.tabla[position].fd[1],addop,strlen(addop)+1);
				write(my_collection.tabla[position].fd[1],cadena,strlen(cadena)+1);
			}*/
	}else if(comando==EXIT){
		
			char exit[]="exit";	
			write(my_collection.tabla[position].fd[1],exit,strlen(exit)+1);
				
		
		
	}		
		
}		



