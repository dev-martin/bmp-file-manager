

#include <stdio.h>
#include "collection.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "item.h"
#include "util_files.h"
#include "directorio.h"
#include "bmp_ops.h"
#include "saucem_ops.h"

//ERRORS

#define INI_SIZE 20
#define SUCCESS_INPUT 0
#define ERR_NO_COLLECTION -1
#define ERR_NO_FREE_SPACE_COLLECTION -2
#define ERR_ELEMENT_ALREADY_COLL -3
#define MORE_FILES_THAN_SPACE -4
#define FILE_DOES_NOT_EXIST -5
#define ELEMENT_NOT_FOUND -6
#define NUM_COLLECTION_ERROR_MSGS 7
//VARIABLE
struct collection my_collection;
//ARRAY ERRORS
static const char *error_map_coll[NUM_COLLECTION_ERROR_MSGS] = {
  "Success",
  "No elements within the collection",
  "Total Capacity Reached",
  "Element already in the collection",
  "There were more files than space in the collection",
  "File does not exist",
  "Element not found"
};
//FUNCTION

struct collection initialize_collection(){

  my_collection.number_occupied=0;
  my_collection.total_capacity=INI_SIZE;
			
  return my_collection;
}
//PRINT ERRORS

const char *get_error_msg_collection(int error){
  if (errno==0 && (error>-7 && error<=0)){
    error=-error;
    const char* c;
    c=error_map_coll[error];
    return c;
  }else{
    return  strerror(error);
  }
}

//CLEAN MODULE

void clean_module_collection(int savecurrentid){
  delete_collection(my_collection,savecurrentid);

}       

//DELETE COLLLECTION

struct collection delete_collection(struct collection my_collection,int savecurrentid){
  int i;
  int error=0;
  for (i=0;i<my_collection.number_occupied;i++){
		
    
    my_collection=delete_item_id(my_collection,my_collection.tabla[i].id,&error,savecurrentid);
    free_item(&my_collection.tabla[i]);
    if (error!=0){
      struct collection my_collection2= initialize_collection();
      return my_collection2;
    }
		      
		       
  }

  return my_collection;
}


//FUNCION PROPIA (para simplificar la construccion de funciones posteriores)
int find_item_name_collectioN(struct collection my_collection,char *Absname,int *error){
  int i;
  if (my_collection.number_occupied!=0){
    for(i =0;i<my_collection.number_occupied;i++){
      if ((strcmp(my_collection.tabla[i].data.filename,Absname))==0){//ENCONTRADO

	*error=ERR_ELEMENT_ALREADY_COLL;
				
	return *error;
      }
    }
        
    *error=ELEMENT_NOT_FOUND;//LLEGAMOS AL FINAL SIN ENCONTRARLO
    return *error;  
  }
  *error=ERR_NO_COLLECTION;// NO HAY ELEM
  return *error ;
}

//FUNCTION ADD-FILE


struct collection add_file(struct collection my_collection,char *path, char *nombre, int *error){
	*error=0;
  static int sid=0; 
  struct stat info;
  char *dirAbs=NULL;
  *error=info_file(path,nombre,&dirAbs,&info);
  if(*error!=0){
    if (dirAbs){
      free(dirAbs);
      dirAbs=NULL;
	}
    *error=errno;
    return my_collection;
  }
  if (my_collection.number_occupied==my_collection.total_capacity){
    //si esta lleno devuelves la coleccion tal cual porque no puedes añadir mas
    *error=ERR_NO_FREE_SPACE_COLLECTION;
    if (dirAbs)
      free(dirAbs);
    return my_collection;
  }else{   
    int index;
    index=find_item_name_collectioN(my_collection,nombre,error);
    if(index==ERR_ELEMENT_ALREADY_COLL){// SI ESTA YA NO LO AÑADES
			if (dirAbs)
				free(dirAbs);
			return my_collection;
							
    }else if(index==ERR_NO_COLLECTION || index==ELEMENT_NOT_FOUND){//SI NO LO ENCUENTRAS  O NO HAY ELEM LO AÑADES 
			char originalpath[PATH_MAX];
			memset(originalpath, 0x00, sizeof(originalpath));
		    
			char pathcheck[PATH_MAX];
			memset(pathcheck, 0x00, sizeof(pathcheck));
		    
			realpath(".",originalpath);
		
			    
			
			if(strcmp(path,".")==0){
				realpath(".",pathcheck);
				strcat(pathcheck,"/");  
				strcat(pathcheck,nombre);
				
				path=calloc(sizeof(char),PATH_MAX);
				for(int i =0;i<strlen(originalpath);i++){
				path[i]=originalpath[i];
				}
				
			}else{
				for(int i =0;i<strlen(path);i++){
				pathcheck[i]=path[i];
				}
				strcat(pathcheck,"/");  
				strcat(pathcheck,nombre);
			}
			
			strcat(originalpath,"/");
			strcat(originalpath,nombre);
			*error=checkBMP(pathcheck);
			
			if(*error==0){
				
				 my_collection.tabla[my_collection.number_occupied]=create_item(path,nombre,dirAbs,info,error,originalpath);
			}else{
				my_collection.tabla[my_collection.number_occupied]=create_item(path,nombre,dirAbs,info,error,NULL);
			}

			if(*error==0){// SI NO HAY ERROR AL CREAR ITEM
				if(my_collection.number_occupied==0){
					sid=0;
				}
				my_collection.tabla[my_collection.number_occupied].id=sid;
				my_collection.number_occupied++;
				sid++;
				if (dirAbs)
					free(dirAbs);
				if(path)
					free(path);
				return my_collection;
			}else{//SI HAY ERROR AL CREAR ITEM DEVUELVES LA COLECCION
				puts("ha habido un error en crear item");
				if (dirAbs)
					free(dirAbs);
				if(path)
					free(path);
				return my_collection;
			}
    }else{
		if (dirAbs)
			free(dirAbs);
      
      return my_collection;
	}
}
}

//FUNCTION ADD-ALL-DIR

struct collection add_all_directory(struct collection my_collection,char *path, int *error){
               
  int i;
  *error=0;				
  if (my_collection.number_occupied==INI_SIZE){
    //si esta lleno devuelves la coleccion tal cual porque no puedes añadir mas
    *error=ERR_NO_FREE_SPACE_COLLECTION;
    return my_collection;

  }else {
    int nDir;// Devuelve numero de archivos leido en el directorio
    struct dirent **info_files;
    nDir=info_all_files_dir(path, &info_files, error);	
		
    if (*error==0){
      for (i=0;i<nDir;i++){// MIENTRAS ENTREN AÑADIMOS
	if (my_collection.number_occupied<my_collection.total_capacity){
	  my_collection=add_file(my_collection,path,info_files[i]->d_name,error);	 
							
	}
      }
      free_info_files_array(info_files,nDir);// FREE OF USED VARIABLES
      return my_collection;
			
    }else{
      free_info_files_array(info_files,nDir);
      return my_collection;
    }
  }
}

//FUNCTION SORT

struct collection sort_collection(struct collection my_collection,int (*compare)(const void *, const void *)){
  if (my_collection.number_occupied>0){
    qsort(my_collection.tabla,my_collection.number_occupied,sizeof(struct item),*compare);// LLAMADA A QSORT CON PARAMETRO EL METODO COMPARE 
  }
  return my_collection;   
}


//FUNCTION PRINT ITEM

void print_item_id(struct collection my_collection, int id, int *error){
	
  if (my_collection.number_occupied!=0){
    int n=0;// numero para comparar

    for(int i=0;i<my_collection.number_occupied;i++){
      n=my_collection.tabla[i].id;// LA RECORREMOS
			
      if (n==id){
				
	printf("   Id   Inode           Perms Links Owner Group        Size              Last Modification            Filename\n");
	print_item(my_collection.tabla[id]);// SI LO ENCONTRAMOS
	*error=0;
	break;
      }
      *error=ELEMENT_NOT_FOUND;// SI NO LO ENCONTRAMOS
    }
            
  }else{
    *error=ERR_NO_COLLECTION;
    printf(get_error_msg_collection(*error));
  }

}

// DELETE ITEM

struct collection delete_item_id(struct collection my_collection, int id, int *error,int savecurrentiddone){
		
	
  int isBMP=0;
  if (my_collection.number_occupied!=0){
	int n=0;// numero para comparar
		
	for(int i=0;i<my_collection.number_occupied;i++){
      n=my_collection.tabla[i].id;// guardo el id 
			
      if (n==id){     
				//*error=0;
				struct item tmp;
				//BMPFILE bmp;
				isBMP=checkBMP(my_collection.tabla[i].data.filename);
				
				if(isBMP==0){
						tmp=my_collection.tabla[my_collection.number_occupied-1];
						my_collection.tabla[my_collection.number_occupied-1]=my_collection.tabla[i]	;
						my_collection.tabla[i]=tmp;
						if(savecurrentiddone==0){
							save_current_id(my_collection,n,error,&savecurrentiddone);
						}
						//*error=remove(my_collection.tabla[my_collection.number_occupied-1].original);
						if (*error!=0){
							puts("can not remove");
						
						}		
						free_item(&my_collection.tabla[my_collection.number_occupied-1]);
						
						//free_item(&tmp);
					
						my_collection.number_occupied--;
								
				}else{
					
					//CREAMOS VARIABLE TEMPORAL PARA EL CAMBIO
					tmp=my_collection.tabla[my_collection.number_occupied-1];
					my_collection.tabla[my_collection.number_occupied-1]=my_collection.tabla[i]	;
					my_collection.tabla[i]=tmp;
					
					free_item(&my_collection.tabla[my_collection.number_occupied-1]);
						
					//free_item(&tmp);
							
					my_collection.number_occupied--;
								
					return my_collection;
				}
				
				return my_collection;
			  
		}
	}
      *error=ELEMENT_NOT_FOUND;
    return my_collection;
	
  }else{
    *error=ERR_NO_COLLECTION;
    return my_collection;
  }
  return my_collection;
}

void print_collection(struct collection my_collection){
  int i ;
  if (my_collection.number_occupied!=0){
    printf("   Id   Inode           Perms Links Owner Group        Size           Last Modification        Filename\n");
    for (i =0;i<my_collection.number_occupied;i++){//RECOREMOS COMPLETA IMPRIMIENDO CADA POSICION
      print_item(my_collection.tabla[i]);
    }
  }else{
    puts("numero de ocupados = 0");
					
  }
}



