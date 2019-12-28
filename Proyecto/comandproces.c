#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "menu.h"
#include "input_program.h"
#include  "bmp_ops.h"
#include "comandproces.h"
#define SUCCESS_INPUT 0
#define TOO_MANY_ARGUMENTS -1
#define WRONG_ARG -2;
#include <errno.h>
#define NUM_COMP_ERROR_MSGS 3

static const char *error_map_comp[NUM_COMP_ERROR_MSGS] = {
    "Success",
    "Too many arguments",
    "Wrong argument"
};

const char *get_error_msg_comp(int error){
	if (errno==0 && (error>-3 && error<=0)){
		error=-error;
        const char* c;
		c=error_map_comp[error];
		return c;
    }else{
		return  strerror(error);
	}
}



char **stringcut(char *order,int *error){
	char **buffer=NULL;
	char *token;
	//char auxorder
    int counter=0;
	buffer=(char**)malloc(5 * sizeof(char*));
	buffer[0]=NULL;
	buffer[1]=NULL;
	buffer[2]=NULL;
	buffer[3]=NULL;
	buffer[4]=NULL;
	
    while ( (token = strtok_r(order, " ", &order)) && counter<5){
       
       
		if(counter<5){
			buffer[counter]=strdup(token);
		}
		counter++;
	}
	
	if(counter>=6){
		*error=TOO_MANY_ARGUMENTS;
	}
	
	//fress
	return buffer;
}
						

comandos_t asigncomand(int *error,char **buffer){
	comandos_t c;		
	c.comando=HELP;
	c.opcional.arg_opt_long=0;
	c.opcional.arg_opt_char=NULL;
	c.parameter=NULL;
	c.parameter2=NULL;
	c.id=0;
	
	if(strcmp(buffer[0],"load_dir")==0 && buffer[1]!=NULL && buffer[2]==NULL&&buffer[3]==NULL){	
	
		c.comando=LOAD_DIR;
		c.opcional.arg_opt_char=strdup(buffer[1]);
		
	}else if(strcmp(buffer[0],"add")==0 && buffer[1]!=NULL && buffer[2]==NULL&&buffer[3]==NULL){
		
		c.comando=ADD_FILE;
		c.opcional.arg_opt_char=strdup(buffer[1]);
		
		
	}else if(strcmp(buffer[0],"save_current_id")==0 && buffer[1]!=NULL && buffer[2]==NULL&&buffer[3]==NULL){
		
		c.comando=SAVE_CURRENT_ID;
		c.opcional.arg_opt_long=getid(10,error,buffer[1]);
		
		
	}else if(strcmp(buffer[0],"delete")==0 && buffer[1]!=NULL && buffer[2]==NULL&&buffer[3]==NULL){
		
		c.comando=DEL_FILE;
		c.opcional.arg_opt_long=getid(10,error,buffer[1]);
	}else if(strcmp(buffer[0],"sort")==0 && buffer[1]!=NULL && buffer[2]==NULL&&buffer[3]==NULL){
		
		c.comando=SORT;
		
		if(strcmp(buffer[1],"name")==0 ){
				c.opcional.arg_opt_sort_t=SORT_NAME;
		}else if(strcmp(buffer[1],"size")==0){
				
				c.opcional.arg_opt_sort_t=SORT_SIZE;
		}else if(strcmp(buffer[1],"time")==0){
				 	
				c.opcional.arg_opt_sort_t=SORT_LAST_MODIFIED;
		}else if(strcmp(buffer[1],"inode")==0){
				 
				c.opcional.arg_opt_sort_t=SORT_INODE;
			
		}else if(strcmp(buffer[1],"def")==0){//id
				 
				c.opcional.arg_opt_sort_t=SORT_ID;
			
		}
		else{
			*error=WRONG_ARG;
		}





	}else if(strcmp(buffer[0],"info")==0 && buffer[1]!=NULL && buffer[2]==NULL&&buffer[3]==NULL){
	
		c.comando=INFO_FILE;
		c.opcional.arg_opt_long=getid(10,error,buffer[1]);
	}else if(strcmp(buffer[0],"infoall")==0 && buffer[1]==NULL && buffer[2]==NULL&&buffer[3]==NULL){
		c.comando=INFO_ALL;
	}else if(strcmp(buffer[0],"help")==0 && buffer[1]==NULL && buffer[2]==NULL&&buffer[3]==NULL){
		c.comando=HELP;
	}else if(strcmp(buffer[0],"logon")==0 && buffer[1]==NULL && buffer[2]==NULL&&buffer[3]==NULL){
		c.comando=LOGON;
	}else if(strcmp(buffer[0],"logoff")==0 && buffer[1]==NULL && buffer[2]==NULL&&buffer[3]==NULL){
		c.comando=LOGOFF;
	}	else if(strcmp(buffer[0],"add_op")==0){
		if(buffer[1]!=NULL&&buffer[2]==NULL){
			puts("Need more parameters");
		}else if(buffer[1]!=NULL&&buffer[2]!=NULL&&buffer[3]==NULL){
			 if(strcmp(buffer[2],"blackandwhite")==0 && buffer[3]==NULL && buffer[4]==NULL){
					c.id=getid(10,error,buffer[1]);
					if(*error==0){
						c.comando=ADD_OP;
						c.opcional.arg_opt_char=strdup(buffer[2]);
						c.parameter="";
						c.parameter2="";
					}
				}else{
					puts("Need more parameters");
					puts("The command introducced do not correspond to any functionallity \n");
					puts ("Checkout the HELP manual");
					c.comando=HELP;
				}
		}else if(buffer[1]!=NULL&&buffer[2]!=NULL&&buffer[3]!=NULL){
				if(strcmp(buffer[2],"hist")==0 && buffer[4]==NULL){
					c.id=getid(10,error,buffer[1]);
					if(*error==0){
						c.comando=ADD_OP;
						c.opcional.arg_opt_char=strdup(buffer[2]);
						c.parameter=strdup(buffer[3]);
						c.parameter2="";
						puts("guarda parametros");
					}
				}else if(strcmp(buffer[2],"zero")==0&&(strcmp(buffer[3],"r")==0||(strcmp(buffer[3],"g")==0||(strcmp(buffer[3],"b"))==0))&& buffer[4]==NULL){
					c.id=getid(10,error,buffer[1]);
					if(*error==0){
						c.comando=ADD_OP;
						c.opcional.arg_opt_char=strdup(buffer[2]);
						c.parameter=strdup(buffer[3]);
						c.parameter2="";
					}
				}else if(strcmp(buffer[2],"rotate")==0&&(strcmp(buffer[3],"r")==0||strcmp(buffer[3],"l")==0)&& buffer[4]==NULL){
					c.id=getid(10,error,buffer[1]);
					if(*error==0){
						c.comando=ADD_OP;
						c.opcional.arg_opt_char=strdup(buffer[2]);
						c.parameter=strdup(buffer[3]);
						c.parameter2="";
					}
				}else if(strcmp(buffer[2],"gray")==0&&(strcmp(buffer[3],"r")==0||(strcmp(buffer[3],"g")==0)||((strcmp(buffer[3],"b"))==0)||(strcmp(buffer[3],"y"))==0)&& buffer[4]==NULL){
					c.id=getid(10,error,buffer[1]);
					if(*error==0){
						c.comando=ADD_OP;
						c.opcional.arg_opt_char=strdup(buffer[2]);
						c.parameter=strdup(buffer[3]);
						c.parameter2="";
					}
				}else if(strcmp(buffer[2],"mirror")==0&&(strcmp(buffer[3],"h")==0||(strcmp(buffer[3],"v")==0))&& buffer[4]==NULL){
					c.id=getid(10,error,buffer[1]);
					if(*error==0){
						c.comando=ADD_OP;
						c.opcional.arg_opt_char=strdup(buffer[2]);
						c.parameter=strdup(buffer[3]);
						c.parameter2="";
					}
				}else if(strcmp(buffer[2],"level")==0&&buffer[3]!=NULL&& buffer[4]!=NULL){
						
					c.comando=ADD_OP;
					if(*error==0){
						c.id=getid(10,error,buffer[1]);
						c.opcional.arg_opt_char=strdup(buffer[2]);
						c.parameter=strdup(buffer[3]);
						c.parameter2=strdup(buffer[4]);
					}
				}else if(strcmp(buffer[2],"crop")==0 &&buffer[3] !=NULL && buffer[4] != NULL){
					c.id=getid(10,error,buffer[1]);
					if(*error==0){
						
						c.comando=ADD_OP;
						c.opcional.arg_opt_char=strdup(buffer[2]);	
						char **buffer1;
						char **buffer2;
						char **buffer3;
						char *token1;
						char *token2;
						char *token3;
						
						int counter1=0;
						buffer1=(char**)malloc(2 * sizeof(char*));
						buffer1[0]=NULL;
						buffer1[1]=NULL;
						while( (token1 = strtok_r(buffer[3], "(", &buffer[3]))  && counter1<2){ 
							if(counter1<2){
								buffer1[counter1]=strdup(token1);
							}
							counter1++;
							}
							
							if(counter1>=3){
							*error=TOO_MANY_ARGUMENTS;
							}
						counter1=0;
						buffer2=(char**)malloc(2 * sizeof(char*));
						buffer2[0]=NULL;
						buffer2[1]=NULL;
						while ( (token2= strtok_r(buffer1[0], ")", &buffer1[0])) && counter1<2){ 
							if(counter1<2){
								buffer2[counter1]=strdup(token2);
							}
							counter1++;
							}		
							if(counter1>=3){
								*error=TOO_MANY_ARGUMENTS;
							}
						counter1=0;
						buffer3=(char**)malloc(2 * sizeof(char*));
						buffer3[0]=NULL;
						buffer3[1]=NULL;
						while ((token3= strtok_r(buffer2[0], ",", &buffer2[0])) && counter1<2){ 
							if(counter1<2){
								buffer3[counter1]=strdup(token3);
							}
							counter1++;
							}		
							if(counter1>=3){
								*error=TOO_MANY_ARGUMENTS;
							}
							c.top_left.x= getid(10,error,buffer3[0]);
							c.top_left.y= getid(10,error,buffer3[1]);
							
						char **buffer4;
						char **buffer5;
						char **buffer6;
						char *token4;
						char *token5;
						char *token6;
						
						 counter1=0;
						buffer4=(char**)malloc(2 * sizeof(char*));
						buffer4[0]=NULL;
						buffer4[1]=NULL;
						while( (token4= strtok_r(buffer[4], "(", &buffer[4]))  && counter1<2){ 
							if(counter1<2){
								buffer4[counter1]=strdup(token4);
							}
							counter1++;
							}
							
							if(counter1>=3){
							*error=TOO_MANY_ARGUMENTS;
							}
						counter1=0;
						buffer5=(char**)malloc(2* sizeof(char*));
						buffer5[0]=NULL;
						buffer5[1]=NULL;
						while ( (token5= strtok_r(buffer4[0], ")", &buffer4[0])) && counter1<2){ 
							if(counter1<2){
								buffer5[counter1]=strdup(token5);
							}
							counter1++;
							}		
							if(counter1>=3){
								*error=TOO_MANY_ARGUMENTS;
							}
						counter1=0;
						buffer6=(char**)malloc(2 * sizeof(char*));
						buffer6[0]=NULL;
						buffer6[1]=NULL;
						while ((token6= strtok_r(buffer5[0], ",", &buffer5[0])) && counter1<2){ 
							if(counter1<2){
								buffer6[counter1]=strdup(token6);
							}
							counter1++;
							}		
							if(counter1>=3){
								*error=TOO_MANY_ARGUMENTS;
							}
							c.bottom_right.x=getid(10,error,buffer6[0]);
							c.bottom_right.y=getid(10,error, buffer6[1]);
							c.parameter="";
							c.parameter2="";
					}
				}else{
					*error=WRONG_ARG;
				}
						
}
		
	}else if(strcmp(buffer[0],"undo")==0 && buffer[1]!=NULL && buffer[2]==NULL){		

		c.comando=UNDO_OP;
		c.opcional.arg_opt_long=getid(10,error,buffer[1]);

	}else if(strcmp(buffer[0],"redo")==0 && buffer[1]!=NULL && buffer[2]==NULL){
		
		c.comando=REDO_OP;
		c.opcional.arg_opt_long=getid(10,error,buffer[1]);

	}else if(strcmp(buffer[0],"clean")==0 && buffer[1]!=NULL && buffer[2]==NULL){
		
		c.comando=DELETE_ALL_OPS;
		c.opcional.arg_opt_long=getid(10,error,buffer[1]);

	
	}else if(strcmp(buffer[0],"save_col")==0 ){
		if(buffer[1]==NULL && buffer[2]==NULL){
			puts("Camand save needs at least a parameter");
		} else if(buffer[1]!=NULL&&buffer[2]==NULL){
			c.comando=SAVE_COL;
			c.arg_opt_ops=OPS;
			c.opcional.arg_opt_char=strdup(buffer[1]);
		} else if(buffer[1]!=NULL &&buffer[2]!=NULL && strcmp(buffer[1],"no-ops")==0){
			c.comando=SAVE_COL;
			c.arg_opt_ops=NO_OPS;
			c.opcional.arg_opt_char=strdup(buffer[2]);; 
		}else if(buffer[1]!=NULL && buffer[2]!=NULL && strcmp(buffer[1],"no-ops")!=0){
			puts("Command save_col accepts only one parameter different from ´no_ops`");
		}		
	}else if((strcmp(buffer[0],"load_col")==0)){
		if(buffer[1]==NULL &&buffer[2]==NULL){
			puts("Camand save needs at least a parameter");
		}else if(buffer[1]!=NULL&&buffer[2]==NULL){
			c.comando=LOAD_COL;
			c.arg_opt_ops=OPS;
			c.opcional.arg_opt_char=strdup(buffer[1]);
		} else if(buffer[1]!=NULL &&buffer[2]!=NULL && strcmp(buffer[1],"no-ops")==0){
			c.comando=LOAD_COL;
			c.arg_opt_ops=NO_OPS;
			c.opcional.arg_opt_char=strdup(buffer[2]); 
		}else if(buffer[1]!=NULL && buffer[2]!=NULL && strcmp(buffer[1],"no-ops")!=0){
			puts("Command load_col accepts only one parameter different from ´no_ops`");
		}
	/*}else if(strcmp(buffer[0],"logon")==0 && buffer[1]!=NULL && buffer[2]==NULL){
		c.comando=LOGON;
	}else if(strcmp(buffer[0],"logoff")==0 && buffer[1]!=NULL && buffer[2]==NULL){
		c.comando=LOGOF;*/
	}else if(strcmp(buffer[0],"exit")==0 && buffer[1]==NULL && buffer[2]==NULL){
		c.comando=EXIT;
	}else{
		*error=WRONG_ARG;
		puts("The command introducced do not correspond to any functionallity \n");
		puts ("Checkout the HELP manual");
		c.comando=HELP;
	}
	for(int i=0;i<5;i++){
	if((buffer[i])!=NULL){
		  free(buffer[i]);
	  }
	  }
	if((buffer)!=NULL){
		  free(buffer);
	  }
	  
return c;
	
	
}


long getid(int base, int *error,char *cadena/*id*/){

	char *cpy; //char* auxiliar para el strtol
	long val=0;
	
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
	
  return val;				
}

	
	















