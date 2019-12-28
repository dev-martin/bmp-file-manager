//PERSISTENCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "collection.h"
#include "item.h"
#include <string.h>
#include <errno.h>
#include "node.h"
#include "saucem_ops.h"
#include "input_program.h"
#include <linux/limits.h>
#include "comandproces.h"
#include "bmp_ops.h"
#include "menu.h"
#include <dirent.h>

	
//-----------------------------------------------------------------
	
	
void save_col(char *nfile,struct collection my_collection, int ops,int *error){//nota este volcar te hace todo los campos del item.
  FILE *fp;
  size_t num;
  char *fn=NULL;
  char savingpath[PATH_MAX];
  memset(savingpath, 0x00, sizeof(savingpath));
  char savingpathaux[PATH_MAX];
  memset(savingpathaux, 0x00, sizeof(savingpathaux));

  int backup=0;
  BMPFILE bmp;
  if(strcmp(nfile,"persistance")==0){
	  
    fn = getenv("HOME");
    strcat(fn, "/");
    for(int i = 0 ; i < strlen(fn) ; i++){
      savingpath[i] =fn[i];
    }
    strcat(fn,"persistance");
    fp = fopen (fn, "w");
    if (fp == NULL){	
      printf("ERROR while OPENING");
    }
    strcat(savingpath,"PERSISTANCE"); 
    int dir=0;
    //Permissions of the directory obtained from  http://pubs.opengroup.org/onlinepubs/009695399/functions/mkdir.html
    DIR* folder = opendir(savingpath);
    if (folder == NULL){
      printf("Folder created\n");
      dir = mkdir( savingpath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
    closedir(folder);
				
    if (dir == -1){
      printf("ERROR creating the folder");
      *error=-1;			
    }
				
				
  }else{
    char savingfile[PATH_MAX];
      memset(savingfile, 0x00, sizeof(savingfile));
    int dir=0;
    //Permissions of the directory obtained from  http://pubs.opengroup.org/onlinepubs/009695399/functions/mkdir.html
    DIR* folder = opendir(nfile);
    if (folder == NULL){
      printf("Folder created");
      dir = mkdir(nfile, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
    closedir(folder);
				
    if (dir == -1){
      printf("ERROR creating the folder");
      *error=-1;				
    }
				
				
	realpath(".",savingpath);			
    
    for(int i = 0 ; i < strlen(savingpath) ; i++){
      savingfile[i] =savingpath[i];
    }				
				

    strcat(savingfile,"/");
    strcat(savingfile,nfile);
    strcat(savingfile,"/");
    strcat(savingfile,nfile);
					
    fp = fopen (savingfile, "w");
    if (fp == NULL){	
      printf("ERROR while OPENING");
    }
  }	
  
  ////////////
  
  
  
  /////////
  
  if(ops==1||ops==0){
		
    for(int i=0;i<my_collection.number_occupied;i++){
			  
			  
      num = fprintf(fp,"%s ",my_collection.tabla[i].data.filename);
      if (num<0){
	fclose(fp);
	*error= -1;
	break;	
      }			

      if(checkBMP(my_collection.tabla[i].data.filename)==0){
				
					
	int currentpos=0;
				
				
	num = fprintf(fp,"%s ",savingpath);
	if (num<0){
	  fclose(fp);
	  *error= -1;
	  break;	
	}	
				
	 memset(savingpathaux, 0x00, sizeof(savingpathaux));			
	for(int j = 0 ; j < strlen(savingpath) ; j++){
	  savingpathaux[j] =savingpath[j];
	}
			
	strcat(savingpathaux,"/");
		
	strcat(savingpathaux,my_collection.tabla[i].data.filename);
				
	bmp=load_bmp(bmp,my_collection.tabla[i].original,error);
				
	rename(my_collection.tabla[i].original,savingpathaux);
				
	backup=save_bmp(my_collection.tabla[i].data.filename,bmp,error);
	freeBMP(bmp);
	if(backup!=0){
	  printf("Error duplicando");
	  break;
	}
				
	if(ops==1){
	  struct node *aux;	
	  if(my_collection.tabla[i].first==NULL){
	    //no ops on this item
	  }else {
	    aux=my_collection.tabla[i].first;	
	    while(aux!=NULL){
						
	      if((aux!=my_collection.tabla[i].current)){
		currentpos++;
	      }
										
	      if(aux->next!=NULL){
		if(strcmp(aux->operation,"zero")==0){
		  num = fprintf(fp,"%s|%s-",aux->operation,aux->parameter);
		  if (num<0){
		    fclose(fp);
		    *error= -1;
		    break;	
		  }
		}
		if(strcmp(aux->operation,"crop")==0){
		  num = fprintf(fp,"%s|%d|%d|%d|%d-",aux->operation,aux->p1.x,aux->p1.y,aux->p2.x,aux->p2.y);
		  if (num<0){
		    fclose(fp);
		    *error= -1;
		    break;	
		  }							
													
		}
		if(strcmp(aux->operation,"level")==0){
		  num = fprintf(fp,"%s|%s|%s-",aux->operation,aux->parameter,aux->parameter2);
		  if (num<0){
		    fclose(fp);
		    *error= -1;
		    break;	
		  }							
													
		}
		if(strcmp(aux->operation,"blackandwhite")==0){
		  num = fprintf(fp,"%s-",aux->operation);
		  if (num<0){
		    fclose(fp);
		    *error= -1;
		    break;	
		  }							
													
		}						
		if(strcmp(aux->operation,"rotate")==0){
		  num = fprintf(fp,"%s|%s-",aux->operation,aux->parameter);
		  if (num<0){
		    fclose(fp);
		    *error= -1;
		    break;	
		  }							
													
		}					
		if(strcmp(aux->operation,"hist")==0){
		  num = fprintf(fp,"%s|%s-",aux->operation,aux->parameter);
		  if (num<0){
		    fclose(fp);
		    *error= -1;
		    break;	
		  }							
													
		}
		if(strcmp(aux->operation,"mirror")==0){
		  num = fprintf(fp,"%s|%s-",aux->operation,aux->parameter);
		  if (num<0){
		    fclose(fp);
		    *error= -1;
		    break;	
		  }
		}
		if(strcmp(aux->operation,"gray")==0){
		  num = fprintf(fp,"%s|%s-",aux->operation,aux->parameter);
		  if (num<0){
		    fclose(fp);
		    *error= -1;
		    break;	
		  }
		}
												
	      }else{
												
		if(strcmp(aux->operation,"mirror")==0){
		  num = fprintf(fp,"%s|%s ",aux->operation,aux->parameter);
		  if (num<0){
		    fclose(fp);
		    *error= -1;
		    break;	
		  }
		}
		if(strcmp(aux->operation,"rotate")==0){
		  num = fprintf(fp,"%s|%s ",aux->operation,aux->parameter);
		  if (num<0){
		    fclose(fp);
		    *error= -1;
		    break;	
		  }							
													
		}
		if(strcmp(aux->operation,"blackandwhite")==0){
		  num = fprintf(fp,"%s ",aux->operation);
		  if (num<0){
		    fclose(fp);
		    *error= -1;
		    break;	
		  }							
													
		}
		if(strcmp(aux->operation,"level")==0){
		  num = fprintf(fp,"%s|%s|%s ",aux->operation,aux->parameter,aux->parameter2);
		  if (num<0){
		    fclose(fp);
		    *error= -1;
		    break;	
		  }							
													
		}	
		if(strcmp(aux->operation,"gray")==0){
		  num = fprintf(fp,"%s|%s ",aux->operation,aux->parameter);
		  if (num<0){
		    fclose(fp);
		    *error= -1;
		    break;	
		  }
		}
		if(strcmp(aux->operation,"zero")==0){
		  num = fprintf(fp,"%s|%s ",aux->operation,aux->parameter);
		  if (num<0){
		    fclose(fp);
		    *error= -1;
		    break;	
		  }
		}
		if(strcmp(aux->operation,"crop")==0){
		  num = fprintf(fp,"%s|%d|%d|%d|%d ",aux->operation,aux->p1.x,aux->p1.y,aux->p2.x,aux->p2.y);
		  if (num<0){
		    fclose(fp);
		    *error= -1;
		    break;	
		  }							
													
		}
											
		if(strcmp(aux->operation,"hist")==0){
		  num = fprintf(fp,"%s|%s ",aux->operation,aux->parameter);
		  if (num<0){
		    fclose(fp);
		    *error= -1;
		    break;	
		  }							
														
		}
	      }//last op
	      aux=aux->next;
	    }		//while							
							
	    num = fprintf(fp,"%d",currentpos);
	    if (num<0){
	      fclose(fp);
	      *error= -1;
	      break;	
	    }
	  }//if there are ops
	}//if ops	
	num = fprintf(fp,"%s\n","");
	if (num<0){
	  fclose(fp);
	  *error= -1;
	  break;	
	}
				
					
      }else{
			
	num = fprintf(fp,"%s ",my_collection.tabla[i].data.path);
	if (num<0){
	  fclose(fp);
	  *error= -1;
	  break;	
	}
	num = fprintf(fp,"%s\n","");
	if (num<0){
	  fclose(fp);
	  *error= -1;
	  break;	
	}	
      }
			
    }//for
	
  }//if ops or no ops
		
  //~ if(fn!=NULL){
    //~ free(fn);
    //~ fn=NULL;
  //~ }
  if (*error==0){
    fclose(fp);
  }
		
		
		
	
}	



//-------------------------------------------------------------------------------------



/*
  void checkPersistance(int persistnoops,int persistsave){
  int error;
  if (persistnoops==1){
  save_col(NULL,my_collection,0,&error);
  }else if (persistsave==1){
  return;
  }else{
  save_col(NULL,my_collection,1,&error);
  }
		
		
  }*/
//PERSISTENCE

struct collection load_col(struct collection my_collection,char *filename,int ops,int *error, int loadtype){
  FILE *file;
  int correct=1;
  char path[PATH_MAX];
  memset(path, 0x00, sizeof(path));
 

  size_t buffer_size = 0;
			
  if(loadtype==0){
	
    strcpy(path,getenv("HOME"));
    strcat(path, "/");
    strcat(path, filename);
    if(access(path, F_OK )!=0){
      puts("The file do not exist, we only store the collection");
      return my_collection;   
    }else{
      if(access(path, R_OK )){
	puts("The configuration file can not be read");
	*error=-1;
	return my_collection;   
      }
    }
		
    file = fopen (path, "r");
	
  }else if(loadtype==1){
	realpath(".",path);
	strcat(path,"/");
	strcat(path,filename);
    strcat(path,"/");//carpoeta
    strcat(path,filename);//archivo
   // filename[strlen(filename)-1]=0;
	
    if(access(path, F_OK )!=0 || access(path, R_OK )!=0){
      puts("The file do not exist.");
      return my_collection;   
    }
		
    if(my_collection.number_occupied!=0){
      int error2=0;
	  char *decission=NULL;		
      printf("Do you want to overwrite the collection with the items inside the indicated file?(Y/N):\n");
      while(correct !=0){
					
		
		error2 = getline(&decission, &buffer_size, stdin);
							
		if(error2!=-1){
		  if(strcmp(decission,"Y\n")==0){
			correct=0;
									
		  }else if (strcmp(decission,"N\n" )==0){
			return my_collection;
		  }else{
			printf("Wrong option, please introduce (Y/N):\n");
			correct = 1;
		  }
		}else{
		  correct=1;
		}
		
		if(decission){
		free(decission);
		decission=NULL;
		}
      

	}
		delete_collection(my_collection,1);
		my_collection = initialize_collection(my_collection);	
    }
    
    
    file = fopen (path, "r");
	
  }
		
	
  char* itemname=NULL;
 
  
  char *itempath=NULL;
  
				
  char* itemops=NULL;
  char* itemredos=NULL;
  char *token=NULL;
  char *token2=NULL;
  char *token3=NULL;
  char *token4=NULL;
  char *token5=NULL;
  char *token6=NULL;
  char *token7=NULL;
  char *token8=NULL;
  char *token9=NULL;
  char *token10=NULL;
  POINT p1 ;
  POINT p2;
  int redos=0;
  int counter=0;
  int position=0;
  int counterline=0;
  char *buffer = NULL;
  char *puntero=NULL;
  while(-1 != getline(&buffer, &buffer_size, file)){
    puntero = buffer;
    counter=0;
    counterline++;
			
    buffer[strlen(buffer)-1]=0;
					
    while((token=strtok_r(puntero," ",&puntero))!=NULL){
				
      if(puntero!=NULL){
					
	counter++;	
	if (counter==1){
	  itemname=strdup(token);
	}else if (counter==2){
	  itempath=strdup(token);		
	}else if(counter==3){
	  itemops=strdup(token);
	}else if(counter==4){
	  itemredos=strdup(token);		
	}else{
								
	}				
							
      }//ifopsops
    }//ifopsops
		
    if(itemname!=NULL && itempath!=NULL){
				
      my_collection=add_file(my_collection,itempath,itemname,error);
      //~ my_collection.tabla[0].id=0;
				
      if(ops==1 && itemops!=NULL && itemredos!=NULL ){
					 
	while((token2=strtok_r(itemops,"-",&itemops))!=NULL){
	  token3=strtok_r(token2,"|",&token4);
										
	  if(strcmp(token3,"zero")==0){
	    my_collection=add_onlyop(my_collection,my_collection.tabla[position].id,token3,token4,"",p1,p2,error);
	  }
	  if(strcmp(token3,"rotate")==0){
	    my_collection=add_onlyop(my_collection,my_collection.tabla[position].id,token3,token4,"",p1,p2,error);
	  }
					  
	  if(strcmp(token3,"blackandwhite")==0){
	    my_collection=add_onlyop(my_collection,my_collection.tabla[position].id,token3,"","",p1,p2,error);
	  }
	  if(strcmp(token3,"hist")==0){
										
	    my_collection=add_onlyop(my_collection,my_collection.tabla[position].id,token3,token4,"",p1,p2,error);

	  }
	  if(strcmp(token3,"gray")==0){
										 
	    my_collection=add_onlyop(my_collection,my_collection.tabla[position].id,token3,token4,"",p1,p2,error);
	  }
	  if(strcmp(token3,"mirror")==0){
										 
	    my_collection=add_onlyop(my_collection,my_collection.tabla[position].id,token3,token4,"",p1,p2,error);
	  }
										 
	  if(strcmp(token3,"crop")==0){
	    token5=strtok_r(token4,"|",&token4);//x1
	    token6=strtok_r(token4,"|",&token4);//y1
										 
										 
	    token7=strtok_r(token4,"|",&token4);//x2
	    token8=strtok_r(token4,"|",&token4);//y2
										 
	    p1.x=getid(10,error,token5);
	    p1.y=getid(10,error,token6);
										 
	    p2.x=getid(10,error,token7);
	    p2.y=getid(10,error,token8);
										 
	    my_collection=add_onlyop(my_collection,
				     my_collection.tabla[position].id,
				     token3,token4,"",p1,p2,error);

	  }
	  if(strcmp(token3,"level")==0){
	    token9=strtok_r(token4,"|",&token4);//param1
	    token10=strtok_r(token4,"|",&token4);//param2
	    my_collection=add_onlyop(my_collection,my_collection.tabla[position].id,token3,token9,token10,p1,p2,error);
	  }
									
	}
												
	struct node* aux;
								
	if(my_collection.tabla[position].first!=NULL){
	  aux=my_collection.tabla[position].first;
	  redos=getid(10,error,itemredos);
	  for(int i=0;i<redos;i++){
	    aux=aux->next;
	  }
	  my_collection.tabla[position].current=aux;
	}
      }
    }
    position++;
//        buffer=NULL;
   if (buffer){ 
   free(buffer);
    buffer=NULL; 
	} 
	  
  if (itemname){
    free(itemname);
    itemname=NULL;
  }
  }

  fclose(file);
  
  if (buffer){
    free(buffer);
    buffer=NULL;
  }
  
   if(puntero==buffer&&puntero!=NULL){ 
   free(puntero); 
   } 
  
  
  //~ if (itempath){
  //~ free(itempath);
  //~ itempath=NULL;
  //~ }
    
  //~ if (itemops){
  //~ free(itemops);
  //~ itemops=NULL;
  //~ }
    
  //~ if (itemredos){
  //~ free(itemredos);
  //~ itemredos=NULL;
  //~ }
  return my_collection;
}//while getline
	



		
