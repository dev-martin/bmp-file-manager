//CONF file 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "configuration_file.h"

struct confArguments readconf(int *error){
	FILE *config;
	struct confArguments argconf;
	argconf.p=NULL;
	argconf.l=NULL;
	argconf.i=NULL;
	argconf.s=NULL;
	argconf.bydef=0;
	char path[PATH_MAX];
	strcpy(path,getenv("HOME"));
	strcat(path, "/.saucem");
	strcat(path, "/conf");
	
	//Path de la caroeta saucem
	char* pathdir = getenv("HOME");
	strcat(pathdir, "/.saucem");
	
    if(access(path, F_OK )!=0){//if the conf file do not exist
				puts("The configuration file do not exist");
				int dir;
				//Permissions of the directory obtained from  http://pubs.opengroup.org/onlinepubs/009695399/functions/mkdir.html
				DIR* folder = opendir(pathdir);
				if (folder == NULL){
					printf("Folder created");
					dir = mkdir( pathdir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
				}
				closedir(folder);
				
				if (dir == -1){
					printf("ERROR creating the folder");
					*error=-1;
					return argconf;				
				}
				
				argconf=setconfbydfault(argconf,error,path);	
				argconf.bydef=1;
			/*	if(config==NULL){
						printf("ERROR while OPENING");
						*error=-1;
					return argconf;
				}
				*/
				
		
		return argconf;

		
	}else{//si existe confile
		
		if(access(path, R_OK )!=0){
			puts("The configuration file can not be read");
			*error=-1;
			 return argconf;   
		}else{
			
			size_t buffer_size = 0;
			char *token;
			char  *delim="=";
			int numgetlines=0;
			config = fopen (path, "r");
					
			//			char *buffer = malloc(buffer_size * sizeof(char));
			char *buffer = NULL;
			char *puntero = NULL;
			while(getline(&puntero, &buffer_size, config)!=-1){
			  buffer = puntero;
				numgetlines++;
				token=strtok_r(buffer,delim,&buffer); 
				
				if(buffer!=NULL){
					buffer[strlen(buffer)-1]=0;	
					if(strcmp(token,"p")==0){
						
						argconf.p=strdup(buffer);
					}
					if(strcmp(token,"l")==0){
						argconf.l=strdup(buffer);
					}
					if(strcmp(token,"i")==0){
						argconf.i=strdup(buffer);
					
					}
					if(strcmp(token,"s")==0){
						argconf.s=strdup(buffer);

					}
				}else{
					puts("wrong configuration file format");	
				}
	 
		}	
		
		if(numgetlines==0 || argconf.p==NULL || argconf.l==NULL){
			argconf=setconfbydfault(argconf,error, path);	
		}

		// make sure we close the file and clean up the buffer when we're finished
		//free(token);
		//free(buffer);
		fclose(config);

		if (puntero)
		  free(puntero);
		
		return argconf;
		
		
		}
	
	
	}
}

struct confArguments setconfbydfault(struct confArguments argconf, int *error,char *path){
				
				FILE *config;
				
				config = fopen (path, "w");
				
				char persist[]="p=persistance\n";
				char log[] = "l=log\n";
				
				int num;
				
				num = fprintf(config, persist);
				
				if (num<0){
					printf("ERROR while writing");
					fclose(config);
					*error=-1;
					return argconf;
				}
				
				argconf.p="persistance";
				
				num = fprintf(config, log);
				if (num<0){
					printf("ERROR while writing");
					fclose(config);
					*error=-1;
					return argconf;
				}
				argconf.l="log";
				fclose(config);
		return argconf;
}		

void clear_confargs(struct confArguments confargs){
	
	if(confargs.bydef==0){ 
		if (confargs.p!=NULL){
			free(confargs.p);
			confargs.p = NULL;
		}
		if (confargs.l!=NULL){
			free(confargs.l);
			confargs.l = NULL;
		}
	}
	if (confargs.s!=NULL){
		free(confargs.s);
		confargs.s = NULL;
	}
	if (confargs.i!=NULL){
		free(confargs.i);
		confargs.i = NULL;
	}
 
}

