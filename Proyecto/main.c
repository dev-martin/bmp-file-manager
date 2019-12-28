#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
#include "inputparam.h"
#include <errno.h>
#include "persistence.h"
#include "prints.h"
#include "configuration_file.h"
#include "bmp_ops.h"
#include "concurrence.h"
#include "log_file.h"

/*char bmppath[PATH_MAX];
 *  realpath(my_collection.tabla[pos].original,bmppath);
 * */

struct confArguments  comparations(struct argumentos processed,struct confArguments confargs,int *error,int *persistops,int *persistsave,int *persistclean);
int main(int argc,char**argv){
	
	//struct confArguments confargs; 
	int salir =0;
	int error=0;
	int persistclean=0;
	int persistsave=0;
	int persistops=1;	
	int media=0;
	int nopedone=0; 
	int files=0;
	int savecurrentiddone=0;
    comandos_t c;
    c.opcional.arg_opt_long=0;
	c.opcional.arg_opt_char=NULL;
	c.parameter=NULL;
	c.parameter2=NULL;
	c.id=0;
	
	FILE *stream;      
    time_t curtime;
    time(&curtime);  
    int logon = 0;
    my_collection= initialize_collection();
	inicialize_signal_handlers();
	
	
    struct argumentos processed = proceso_args(argc,argv,&error);
	 if (error){
		printf("Un error ha ocurrido\n");
		salir=1;

	}
	confargs=readconf(&error);
	
	if(error!=0){
		puts("Error while checking Configuration File");
		clear_confargs(confargs);
		exit(0);
	}
	confargs=comparations(processed,confargs,&error,&persistops,&persistsave,&persistclean);
	
	if (((persistclean!=0)&&(processed.ini!=0))||((processed.stored!=0)&&(persistclean!=0))){
		puts("logical collision");
		salir=1;
	}
	if (processed.help==1){
		salir=1;
	}
	
	stream = init_log(confargs, &error); 
	
	if (persistclean!=0){
	 my_collection= initialize_collection();
	}
    //leemos el configuration file
    
	
	char bmppath[PATH_MAX];//obtener el real path de .
	 realpath(".",bmppath);
	if(confargs.i==NULL || strcmp(confargs.i, confargs.p)==0){
		my_collection=load_col(my_collection,confargs.p,1,&error,0);//saca err
	}else{
		strcat(bmppath,confargs.i);
		my_collection=load_col(my_collection,bmppath,1,&error,1);
		memset(bmppath, 0x00, sizeof(bmppath));
		realpath(".",bmppath);
	}
	
	if(confargs.s!=NULL){
		

			if(strcmp(confargs.s,"size")==0){
				my_collection =sort_collection(my_collection, compare_size);
				}
			
			if(strcmp(confargs.s,"name")==0){
				my_collection =sort_collection(my_collection, compare_filename);
				}	
			if(strcmp(confargs.s,"time")==0){
					my_collection =sort_collection(my_collection, compare_last_modified);
				}		
			if(strcmp(confargs.s,"inode")==0){
					my_collection =sort_collection(my_collection, compare_inode);
				}
			if(strcmp(confargs.s,"id")==0){
					my_collection =sort_collection(my_collection,compare_id );
				}			
					
		
	}
	
	if(error!=0){
		clear_confargs(confargs);
		exit(0);
	}
	
	  
	printmenu();
	
	
	while(salir==0){
	
	    
		c = ask_command(); 
		
	switch(c.comando){
		case EXIT:{
			log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "136", "Exit command entered");  
			salir=-1;
			clear_comando(c);
			if(persistsave==0&&processed.stored!=1){
			save_col(confargs.p, my_collection,persistops,&error);
			}
			if(processed.stored==1){
			printf("Atencion guardas la colecion UNICAMENTE en %s",processed.storedfile);
			strcat(bmppath,processed.storedfile);
			save_col(bmppath, my_collection,persistops,&error);
			
			}fclose(stream);
			clear_confargs(confargs);
			clean_module_input_program();
			clean_module_collection(savecurrentiddone);
			//checkPersistance(peristnoops,persistsave);
			developmentresearch(media ,files ,nopedone);
			
			
		break;
		}
		case LOAD_DIR:{
			log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "158", "Load Directory command entered");  
			my_collection=add_all_directory(my_collection,c.opcional.arg_opt_char,&error);
			if(error){
			log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "161", "Error in Load Directory");  
			printf("Error: %s\n",get_error_msg_collection(error));
			}
			nopedone++;
			files++;
			clear_comando(c);
			break;
		}
		case SAVE_CURRENT_ID : {
			log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "170", "Save Current ID command entered");  
			my_collection = save_current_id(my_collection,c.opcional.arg_opt_long,&error,&savecurrentiddone);
						if(error){
			log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "173", "Error in Save Current ID");  
			printf("Error: %s\n",get_error_msg_collection(error));
			}
			nopedone++;
			media++;
			clear_comando(c);
			break;
		}
		case ADD_FILE:{
			log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "182", "Add File command entered");  
			my_collection=add_file(my_collection,".",c.opcional.arg_opt_char,&error);
			if(error){
			log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "185", "Error in Add File");  
			printf("Error: %s\n",get_error_msg_collection(error));
			}
			nopedone++;
			files++;
			clear_comando(c);
			break;
		}
		case DEL_FILE:{
			log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "194", "Delete item command entered");  
			my_collection=delete_item_id(my_collection, c.opcional.arg_opt_long, &error,savecurrentiddone);
			nopedone++;
			files++;
			if(error){
			log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "199", "Error in Delete Item");  
			printf("Error: %s\n",get_error_msg_collection(error));
			}
			//clear_comando(c);
			break;
		}
		case SORT:{
		 int selection=c.opcional.arg_opt_sort_t;
			nopedone++;
			files++;
			switch(selection){
				case SORT_ID:{
					log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "211", "Sort by ID command entered");  
					puts ("You chose SORT_ID\n");
					my_collection =sort_collection(my_collection, compare_id);
					break;
					}
				case SORT_NAME:{
					log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "217", "Sort by Name command entered");  
					puts ("You chose SORT_NAME\n");
					my_collection =sort_collection(my_collection, compare_filename);
					break;
					}
				case SORT_LAST_MODIFIED:{
					log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "223", "Sort by Last Modified command entered");  
					puts ("You chose SORT_LAST_MODIFIED\n");
					my_collection =sort_collection(my_collection, compare_last_modified);
					break;
					}
				case SORT_SIZE:{
					log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "229", "Sort by Size command entered");  
					puts ("You chose SORT_SIZE\n");
					my_collection =sort_collection(my_collection, compare_size);
					break;
					}
				case SORT_INODE:{
					log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "235", "Sort by Inode command entered");  
					puts ("You chose SORT_INODE\n");
					my_collection =sort_collection(my_collection, compare_inode);		
					break;
					}	
				default:{
					puts ("No sorting criteria\n");
					break;
					}
					
				}
			
			clear_comando(c);
			break;
		}
		case INFO_FILE:{
			log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "251", "Info file command entered");  
			//print_item_id(my_collection,c.opcional.arg_opt_long,&error);	
			print_ops(my_collection,c.opcional.arg_opt_long,&error);
			nopedone++;
			files++;
			if(error){
				log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "257", "Error in Info File");  
				printf("Error: %s\n",get_error_msg_saucemops(error));
					
			}
			clear_comando(c);
			break;
		}
		case INFO_ALL:{
			log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "265", "Info All command entered");  
			nopedone++;
			files++;
			print_collection(my_collection);
			clear_comando(c);
		break;
		}
		case HELP:{
			log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "273", "Help command entered");  
			nopedone++;
			print_help();
			clear_comando(c);
		break;
		}
		case ADD_OP:{
					log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "280", "Add Operation command entered");  
					my_collection=add_op( my_collection,c.id,c.opcional.arg_opt_char,c.parameter,c.parameter2,c.top_left , c.bottom_right ,&error);
					if(error){
						log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "283", "Error in Add Operation");  
						printf("Error: %s\n",get_error_msg_saucemops(error));
					}
			media++;
			nopedone++;
			//~ padrerun(my_collection,c.comando,c.opcional.arg_opt_char,c.parameter,c.parameter2,c.top_left,c.bottom_right,c.id);
								
			
		clear_comando(c);
		//free(cadena);			
		break;
		}
		case UNDO_OP:{
				log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "295", "Undo operation command entered");  
				my_collection=undo_op(my_collection ,c.opcional.arg_opt_long/*id*/,&error);
				nopedone++;
				if(error){
					log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "300", "Error in Undo Operation");  
					printf("Error: %s\n",get_error_msg_saucemops(error));
				}
		clear_comando(c);			
		break;
		}
		case REDO_OP:{
				log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "305", "Redo operation command entered");  
				my_collection=redo_op(my_collection ,c.opcional.arg_opt_long/*id*/,&error);
				nopedone++;
				if(error){
					log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "311", "Error in Redo Operation");  
					printf("Error: %s\n",get_error_msg_saucemops(error));
				}
		clear_comando(c);		
		break;
		}
		case DELETE_ALL_OPS:{
				log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "315", "Delete all Operations command entered");  
				nopedone++;
				my_collection=clean_op(my_collection ,c.opcional.arg_opt_long/*id*/,&error);
				
				if(error){
					log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "323", "Error in Delete all operations");  
					printf("Error: %s\n",get_error_msg_saucemops(error));
				}
		clear_comando(c);
		break;
		}
		case SAVE_COL:{
			log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "330", "Save Collection command entered");  
			nopedone++;
			save_col(c.opcional.arg_opt_char, my_collection,c.arg_opt_ops,&error);
		clear_comando(c);
		break;
		}
		case LOAD_COL:{
			log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "136", "Load Collection command entered");  
			nopedone++;
			my_collection=load_col ( my_collection, c.opcional.arg_opt_char,c.arg_opt_ops,&error,1);
			clear_comando(c);
		break;
		}
		case LOGON:{
			log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "344", "Enabling logs on screen");  
			nopedone++;
			logon = 1;
			clear_comando(c);
		break;
		}	
		case LOGOFF:{
			log_entry(stream, logon, fmt1, LastcharDel(ctime(&curtime)), "main.c", "main", "351", "Disabling logs on screen");  
			nopedone++;
			logon = 0;
			clear_comando(c);
		break;
		}	
	}
	//	clear_comando(c);
	//	clean_module_input_program(); // esto es importante para eliminar de la memori	
	}
	return 0;
}

struct confArguments  comparations (struct argumentos processed,struct confArguments confargs,int *error,int *persistops,int *persistsave,int *persistclean){
	
		if(processed.help==1){
			printhelpPREV();
			//clean_module_collection();
			}
		else if(processed.log==1){
			puts("logfile");
			confargs.l=strdup(processed.logfile);
			}	
		else if(processed.ini==1){
			puts("initialfile");
			confargs.i=strdup(processed.inifile);
			}	
		else if(processed.stored==1){
			puts("stored");
		//filename ---> processed.storedfile
			}
		else if(processed.persist1==1){
			if(strcmp(processed.persist,"clean")==0){
				*persistclean=1;
				puts("persist clean");
				}
			else if(strcmp(processed.persist,"no-ops")==0){
				puts("persist no-ops");
				*persistops=1;
				}
			else if(strcmp(processed.persist,"no-save")==0){
				puts("persist no-save");
				*persistsave=0;
				}
			else {
				puts("wrong persist type");
				}			
			}
		else if(processed.sort1==1){
			if(strcmp(processed.sorttype,"size")==0){
				puts ("You chose SORT_SIZE\n");
				my_collection =sort_collection(my_collection, compare_size);				
				}
			else if(strcmp(processed.sorttype,"name")==0){
				puts ("You chose SORT_NAME\n");
				my_collection =sort_collection(my_collection, compare_filename);
				}
			else if(strcmp(processed.sorttype,"time")==0){
				puts ("You chose SORT_LAST_MODIFIED\n");
				my_collection =sort_collection(my_collection, compare_last_modified);
				}		
			else if(strcmp(processed.sorttype,"inode")==0){
				puts ("You chose SORT_INODE\n");
				my_collection =sort_collection(my_collection, compare_inode);		
				}			
			else if(strcmp(processed.sorttype,"default")==0){
				puts ("You chose SORT_ID\n");
				my_collection =sort_collection(my_collection, compare_id);
				}
			else {
				puts("wrong sort type");
				}					
			}
			else {
				puts("NO PARAMETER INTRODUCCED -->>  (you have executed your program without parameters while you call it ) ");
				}	
				
				limpio_resultado(processed);	
			return confargs;			
			
	}

