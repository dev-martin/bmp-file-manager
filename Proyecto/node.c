#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "menu.h" 
#include "senhal.h"
#include "item.h"
#include "collection.h"
#include "input_program.h"
#include "util_files.h"
#include "directorio.h"
#include "node.h"

// struct node 
 /*struct node {
	int id ;
	char *data ;
 	struct node *next;
	struct node *prev;

};*/



// method of add
	void free_next_nodes(struct node* aux){
		if(aux->operation!=NULL){
				free(aux->operation);
				aux->operation=NULL;
			}
			if(aux->parameter!=NULL){
				free(aux->parameter);
				aux->parameter=NULL;
			}
			if(aux->parameter2!=NULL){
				free(aux->parameter2);
				aux->parameter2=NULL;
			}
			if(aux->previousop!=NULL){
				free(aux->previousop);
				aux->previousop=NULL;
			}
			if(aux->currentop!=NULL){
				free(aux->currentop);
				aux->currentop=NULL;
			}
		if(aux->next!=NULL){
			free_next_nodes(aux->next);
			aux->next=NULL;
		}
		free(aux);
		
	}
	
	struct node* add_node(struct collection *my_collection, int pos,struct node *new){

		
		if(my_collection->tabla[pos].first==NULL){
			my_collection->tabla[pos].first=new;
			my_collection->tabla[pos].current=my_collection->tabla[pos].first;
			my_collection->tabla[pos].tail=my_collection->tabla[pos].first;
		return my_collection->tabla[pos].first;
	   }
		
		
		if(my_collection->tabla[pos].first->prev!=my_collection->tabla[pos].current){
		
			if(my_collection->tabla[pos].current->next!=NULL){
				free_next_nodes(my_collection->tabla[pos].current->next);
				my_collection->tabla[pos].tail=my_collection->tabla[pos].current;
			}
			
			my_collection->tabla[pos].current->next=new;
			new->prev = my_collection->tabla[pos].current;
			my_collection->tabla[pos].current=new;
			my_collection->tabla[pos].tail=new;
		}else{
			free_next_nodes(my_collection->tabla[pos].first);
			
			
			my_collection->tabla[pos].first=new;
			my_collection->tabla[pos].current=my_collection->tabla[pos].first;
			my_collection->tabla[pos].tail=my_collection->tabla[pos].first;
				
			
		}
		
		return my_collection->tabla[pos].first;
	}
	
// create node 	
// caution !! we only can use this method when we have already checked  the operation char*
	struct node *create_node(char* operation,char* parameter,char* parameter2,int id,POINT p1, POINT p2, int *error){
		errno=0;
		*error=0;
		struct node *n;
		n=(struct node*)malloc(sizeof (struct node));
		if (n==NULL){
			*error=errno;
			return NULL;
		}
		n->id=id;
		n->next=NULL;
		n->prev=NULL;
		n->operation=strdup(operation);
		n->parameter=strdup(parameter);
		n->parameter2=strdup(parameter2);
		n->previousop=NULL;
		n->currentop=NULL;
		
		n->p1=p1;
		n->p2=p2;
		
		if (n->operation==NULL){
			*error=errno;
			free(n);
			return NULL;
			}
		
		return n;
	}
	
	
	struct collection delete_node( struct collection my_collection, int pos){
		my_collection.tabla[pos].current=my_collection.tabla[pos].current->prev;
	return my_collection;
	}
	
	struct collection redo_node(struct collection my_collection, int pos){
	
			my_collection.tabla[pos].current=my_collection.tabla[pos].current->next;
		
	
		return my_collection;
	}
		
		
	struct collection clean_node_pos(struct collection my_collection, int pos){
		struct node *aux;
			
																															
		while(my_collection.tabla[pos].first!=NULL){	
			aux=my_collection.tabla[pos].first;																														
			my_collection.tabla[pos].first=my_collection.tabla[pos].first->next;
			
			
			if(aux->operation!=NULL){
				free(aux->operation);
				aux->operation=NULL;
			}
			if(aux->parameter!=NULL){
				free(aux->parameter);
				aux->parameter=NULL;
			}
			if(aux->parameter2!=NULL){
				free(aux->parameter2);
				aux->parameter2=NULL;
			}
			if(aux->previousop!=NULL){
				free(aux->previousop);
				aux->previousop=NULL;
			}
			if(aux->currentop!=NULL){
				free(aux->currentop);
				aux->currentop=NULL;
			}
			free(aux);
			
			}
			
			return my_collection;
	}

	
	void clean_node(struct node **n){
		struct node *aux;
		while(*n!=NULL){
			aux=*n;
			*n= (*n)->next;
			
			if(aux->operation!=NULL){
				free(aux->operation);
				aux->operation=NULL;
			}
			if(aux->parameter!=NULL){
				free(aux->parameter);
				aux->parameter=NULL;
			}
			if(aux->parameter2!=NULL){
				free(aux->parameter2);
				aux->parameter2=NULL;
			}
			if(aux->previousop!=NULL){
				free(aux->previousop);
				aux->previousop=NULL;
			}
			if(aux->currentop!=NULL){
				free(aux->currentop);
				aux->currentop=NULL;
			}
			free(aux);
		}
	}
