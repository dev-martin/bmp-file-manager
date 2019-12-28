#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include "menu.h" 
#include "senhal.h"
#include "item.h"
#include "collection.h"
#include "input_program.h"
#include "util_files.h"
#include "directorio.h"
#include "saucem_ops.h"
#include "node.h"
#include "bmp_ops.h"


#define SUCCESS_SAUCEM 0
#define ELEMENT_NOT_FOUND_SAUCEM -1
#define NOT_VALID_OP -2
#define CAN_NOT_REDO -3
#define CAN_NOT_CLEAN -4
#define ERR_NO_ELEMENTS -5
#define CAN_NOT_UNDO -6
#define NO_LIST -7

#define NUM_SAUCEM_ERROR_MSGS 8

extern int savecurrentiddone;


    
static const char *error_map_saucem[NUM_SAUCEM_ERROR_MSGS] =
  {
    "Success.",
    "Element not found.",
    "Non valid operation.",
    "Cannot perform redo command. There are no discarded ops.",
    "No operations to clean.",
    "No elements within the collection",
    "Can not undo further operations ",
    "No list generated",

    };

const char *get_error_msg_saucemops(int error){
	if (errno==0 && (error>-7 && error<=0)){
		error=-error;
        const char* c;
		c=error_map_saucem[error];
		return c;
    }else{
		return  strerror(error);
	}
}



//FIND ITEM ID 
int find_item_id_collection(struct collection my_collection,int id,int *position){
	int i;
	int found=0;
	*position=0;
	if (my_collection.number_occupied!=0){
		for(i =0;i<my_collection.number_occupied;i++){
			if (my_collection.tabla[i].id==id){//ENCONTRADO
				found=1;
				*position=i;
				return found;
			}
			
		}  
	}
		
	return found;
}	

//////ADD OPERATION METHOD
struct collection add_onlyop( struct collection my_collection, int id , char *operation,char * parameter,char * parameter2, POINT p1, POINT p2 , int *error ) {
	int pos;
	//IF the id entered exists.
	if( find_item_id_collection( my_collection,id,&pos) == 1 ) {
		//IF operation entered is either opA opB or opC, 
		//it creates an node and passes it to add_new in order to 
		//add it to the end of the linked list.		
			struct node *new;
			*error=checkBMP(my_collection.tabla[pos].data.filename);
			if(*error==0){
				new=create_node(operation,parameter,parameter2,id,p1,p2,error);
				if(*error==0){
					my_collection.tabla[pos].first=add_node(&my_collection,pos,new);
					*error=SUCCESS_SAUCEM;
					return my_collection;
				}else{
				
				printf("Operation not supported. The file is not of the proper type.\n");
				return my_collection;
				}
			}
			return my_collection;
	}else{
		*error=ELEMENT_NOT_FOUND_SAUCEM;
		return my_collection;
	}
}
struct collection add_op( struct collection my_collection, int id , char *operation,char * parameter,char * parameter2, POINT p1, POINT p2 , int *error) {
	int pos;
	char **previousop=malloc(sizeof(char*)*100);
	char **currentop=malloc(sizeof(char*)*100);
	//IF the id entered exists.
	if( find_item_id_collection( my_collection,id,&pos) == 1 ) {
		//IF operation entered is either opA opB or opC, 
		//it creates an node and passes it to add_new in order to 
		//add it to the end of the linked list.		
			struct node *new;
			*error=checkBMP(my_collection.tabla[pos].data.filename);
			if(*error==0){
				new=create_node(operation,parameter,parameter2,id,p1,p2,error);
			
				if(*error==0){
					my_collection.tabla[pos].first=add_node(&my_collection,pos,new);
					*error=SUCCESS_SAUCEM;
					if (strcmp(operation,"hist")==0){
						*error=hist_bmp (my_collection.tabla[pos].data.filename,error);
						puts ("llego a hist");
					}else if (strcmp(operation,"zero")==0){
					*error= zeros_bmp (my_collection.tabla[pos].data.filename, parameter,  error,previousop,currentop);
					if(my_collection.tabla[pos].first){
						my_collection.tabla[pos].current->previousop=strdup(my_collection.tabla[pos].original);
					}
					 my_collection.tabla[pos].current->previousop=strdup(*previousop);
					 my_collection.tabla[pos].current->currentop=strdup(*currentop);
						puts ("llego a zeros");
					}else if (strcmp(operation,"gray")==0){
					*error= gray_bmp(my_collection.tabla[pos].data.filename, parameter,  error,previousop,currentop);
					if(my_collection.tabla[pos].first){
						my_collection.tabla[pos].current->previousop=strdup(my_collection.tabla[pos].original);
					}
					 my_collection.tabla[pos].current->previousop=strdup(*previousop);
					 my_collection.tabla[pos].current->currentop=strdup(*currentop);
						puts ("llego a gray");
					}else if (strcmp(operation,"crop")==0){
					*error= crop_bmp (my_collection.tabla[pos].data.filename, p1,  p2 ,error,previousop,currentop);
					if(my_collection.tabla[pos].first){
						my_collection.tabla[pos].current->previousop=strdup(my_collection.tabla[pos].original);
					}
					 my_collection.tabla[pos].current->previousop=strdup(*previousop);
					 my_collection.tabla[pos].current->currentop=strdup(*currentop);
						puts ("llego a crop");
					}else if (strcmp(operation,"mirror")==0){
					*error= mirror_bmp(my_collection.tabla[pos].data.filename,  parameter, error, previousop,currentop);
					if(my_collection.tabla[pos].first){
						my_collection.tabla[pos].current->previousop=strdup(my_collection.tabla[pos].original);
					}
					 my_collection.tabla[pos].current->previousop=strdup(*previousop);
					 my_collection.tabla[pos].current->currentop=strdup(*currentop);
						puts ("llego a mirror");
					}else if (strcmp(operation,"level")==0){
					*error= level_bmp (my_collection.tabla[pos].data.filename,parameter,parameter2,error,previousop,currentop);
						if(my_collection.tabla[pos].first){
						my_collection.tabla[pos].current->previousop=strdup(my_collection.tabla[pos].original);
					}
					 my_collection.tabla[pos].current->previousop=strdup(*previousop);
					 my_collection.tabla[pos].current->currentop=strdup(*currentop);
						puts ("llego a level");
					}else if (strcmp(operation,"blackandwhite")==0){
						*error= byw_bmp (my_collection.tabla[pos].data.filename,error,previousop,currentop);
							if(my_collection.tabla[pos].first){
							my_collection.tabla[pos].current->previousop=strdup(my_collection.tabla[pos].original);
							}
						 my_collection.tabla[pos].current->previousop=strdup(*previousop);
						 my_collection.tabla[pos].current->currentop=strdup(*currentop);
							puts ("llego a black&white");
					}
					else if (strcmp(operation,"rotate")==0){
						*error= rotate_bmp(my_collection.tabla[pos].data.filename,parameter,error,previousop,currentop);
							if(my_collection.tabla[pos].first){
							my_collection.tabla[pos].current->previousop=strdup(my_collection.tabla[pos].original);
							}
						 my_collection.tabla[pos].current->previousop=strdup(*previousop);
						 my_collection.tabla[pos].current->currentop=strdup(*currentop);
							puts ("llego a rotate");
					}
					return my_collection;
				
				}else{
					puts("Imposible to create a new node");
					return my_collection;
				}

			}else{
				printf("Operation not supported. The file is not of the proper type.\n");
				return my_collection;
			}
	}else{
		*error=ELEMENT_NOT_FOUND_SAUCEM;
		return my_collection;
	}
}


//////UNDO

struct collection undo_op( struct collection my_collection, int id, int *error) {
	int pos;
	//IF the id entered exists.
	if( find_item_id_collection( my_collection,id,&pos) == 1 ) {
	//We check if that item has a list of operations or if
		//it is empty
		if(my_collection.tabla[pos].first!=NULL){
			
			if (my_collection.tabla[pos].first->prev == my_collection.tabla[pos].current) { 
				*error=CAN_NOT_UNDO;
				return my_collection;
			}else{	
			//IF there is a list, we need to move the current position to the
			//previous one, so that the last operation "disappears" from the
			//readable list. This is implemented in the delete_op() method.
			    char bmppath[PATH_MAX];
			    realpath(my_collection.tabla[pos].original,bmppath);
			    
			   char * template =my_collection.tabla[pos].current->previousop;
			   BMPFILE bmp;
				bmp = load_bmp (bmp, template, error);
	
				if(*error != 0){
					printf("Error while loading the BMP: %d\n", *error);
					return my_collection;
				}
			    rename(my_collection.tabla[pos].current->previousop,bmppath);
			    int backup = save_bmp(template, bmp, error);
				if(backup!=0){
						printf("Error while saving backup.");
						return my_collection;
					}
	
				my_collection=delete_node( my_collection, pos);
				
				*error=SUCCESS_SAUCEM;
				return my_collection;
			}
		
		}else{
			*error=NO_LIST;
			return my_collection;
		}
	}else{//IF the id entered has no associated item in the collection it prints an error.
		*error=ELEMENT_NOT_FOUND_SAUCEM;
		return my_collection;
		}

}

/////REDO
struct collection redo_op( struct collection my_collection, int id ,int *error) {
	int pos=0;
	  BMPFILE bmp;
	  char*template=NULL;
	  int backup =0;
	char bmppath[PATH_MAX];
	//IF the id entered exists.
	if( find_item_id_collection( my_collection,id,&pos) == 1 ) {
		//We check if that item has a list of operations which
	        //have been undone
	    if(my_collection.tabla[pos].first!=NULL){
			
			if (my_collection.tabla[pos].first->prev == my_collection.tabla[pos].current){
				my_collection.tabla[pos].current=my_collection.tabla[pos].first;
				template=my_collection.tabla[pos].current->currentop;
			 	bmp = load_bmp (bmp, template, error);
			 	
				if(*error != 0){
					printf("Error while loading the BMP: %d\n", *error);
					return my_collection;
				}

				 realpath(my_collection.tabla[pos].original,bmppath);
			  rename(my_collection.tabla[pos].current->currentop,bmppath);
					  backup = save_bmp(template, bmp, error);
				if(backup!=0){
						printf("Error while saving backup.");
						return my_collection;
					}
				*error=SUCCESS_SAUCEM;
				return my_collection;
			}else if (my_collection.tabla[pos].current->next == NULL) { 
				*error=CAN_NOT_REDO;
				
				return my_collection;
			}else{	
			//IF there is a list of undone operations we need to move the current
			//position to the following one, so that there is one element more in the
			//readable list. This is implemented in the redo_op() method
			 	template=my_collection.tabla[pos].current->next->currentop;
			 	bmp = load_bmp (bmp, template, error);
			 	
	
				if(*error != 0){
					printf("Error while loading the BMP: %d\n", *error);
					return my_collection;
				}

			    realpath(my_collection.tabla[pos].original,bmppath);
			    rename(my_collection.tabla[pos].current->next->currentop,bmppath);
			    backup = save_bmp(template, bmp, error);
				if(backup!=0){
						printf("Error while saving backup.");
						return my_collection;
					}
				my_collection=redo_node(my_collection,pos);
				*error=SUCCESS_SAUCEM;
				return my_collection;
			}
		
		}else{//IF the id entered has no associated item in the collection it prints an error.
			*error=NO_LIST;
			return my_collection;
		}
		
	}
		else{
			*error=ELEMENT_NOT_FOUND_SAUCEM;
			return my_collection;
		}

}

////CLEAN
struct collection clean_op( struct collection my_collection, int id ,int *error) {
	int pos;
	char bmppath[PATH_MAX];
	//IF the id entered exists.	
	if( find_item_id_collection( my_collection,id,&pos) == 1 ) {
		
		//We check if there are operations in the list, either readable
	    //or discarded operations.
		
		if (my_collection.tabla[pos].first == NULL) { 
			*error=CAN_NOT_CLEAN;
			return my_collection;
			
		}else{
			//IF there is a list of operations for that id we clean it completely.
			//This is implemented in the clean_op() method.
			realpath(my_collection.tabla[pos].original,bmppath);
			rename(my_collection.tabla[pos].first->previousop,bmppath);
			my_collection=clean_node_pos(my_collection,pos);
			*error=SUCCESS_SAUCEM;
			printf("The list of operations for this id has been cleaned. \n");
			return my_collection;
			}
	//IF the id entered has no associated item in the collection it prints an error.
	}else{
		*error=ELEMENT_NOT_FOUND_SAUCEM;
		return my_collection;
	}

}

void print_ops(struct collection my_collection,int id,int *error){
		int pos;
	    
	
		if(find_item_id_collection(my_collection,id,&pos)==1){
			
			printf("\nId: %d\n",id);
			printf("File: %s\n",my_collection.tabla[pos].data.filename);
			printf("Scheduled operations:\n");
			
			if(my_collection.tabla[pos].first == NULL || my_collection.tabla[pos].current==NULL){
				puts("No scheduled operations for this id\n");
				
			}else{	
				
				struct node *aux=my_collection.tabla[pos].first;
				
				printf("%s",aux->operation);
				
				while(aux!=my_collection.tabla[pos].current){
					aux=aux->next;
					printf("-%s",aux->operation);
				}
				*error=SUCCESS_SAUCEM;
				printf("\n");
			}	
		
		
	}
	else{
		*error=ELEMENT_NOT_FOUND_SAUCEM;
	}
	
}

//~ struct node {
	//~ char *operation;
	//~ char *parameter;
	//~ char *previousop;
	//~ char *currentop;
	//~ struct node *next;
	//~ struct node *prev;
	//~ int id;
//~ };



struct collection save_current_id(struct collection my_collection ,int id,int *error,int *savecurrentiddone){
	int pos=0;
	if(find_item_id_collection(my_collection,id,&pos)==1){
		
		
		
		if(my_collection.tabla[id].tail!=NULL){
			
		
			while(my_collection.tabla[id].tail!=NULL){
			
			
				my_collection.tabla[id].tail->operation=NULL;
				free(my_collection.tabla[id].tail->operation);
				
				my_collection.tabla[id].tail->parameter=NULL;
				free(my_collection.tabla[id].tail->parameter);
				
				*error=remove(my_collection.tabla[id].tail->currentop);
				if(*error!=0){
					printf("Could not save_current_id");
					return my_collection;
				}
				
				my_collection.tabla[id].tail->currentop=NULL;
				free(my_collection.tabla[id].tail->currentop);
				
				*error=remove(my_collection.tabla[id].tail->previousop);
				if(*error!=0){
					printf("Could not save_current_id");
					return my_collection;
				}
				
				my_collection.tabla[id].tail->previousop=NULL;
				free(my_collection.tabla[id].tail->previousop);
				
				
				if(my_collection.tabla[id].tail!=my_collection.tabla[id].first){
					my_collection.tabla[id].tail=my_collection.tabla[id].tail->prev;
					free(my_collection.tabla[id].tail->next);
					my_collection.tabla[id].tail->next=NULL;
					my_collection.tabla[id].current=my_collection.tabla[id].tail->next;
					
				}
				else if(my_collection.tabla[id].tail==my_collection.tabla[id].first){
					my_collection.tabla[id].tail=NULL;
					free(my_collection.tabla[id].tail);
					my_collection.tabla[id].first=my_collection.tabla[id].tail;
				}
					
		}
		*error=SUCCESS_SAUCEM;
		*savecurrentiddone=1;
	}
}
	else{
		*error=ELEMENT_NOT_FOUND_SAUCEM;
		return my_collection;
	}
	return my_collection;
}
