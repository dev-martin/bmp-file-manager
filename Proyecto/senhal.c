
#include "senhal.h"
#include <stdio.h>
#include "input_program.h"
#include <stdlib.h>
#include "collection.h"
#include "configuration_file.h"
#include "item.h"
#include <signal.h>
	struct confArguments confargs;	  
	int savecurrentiddone;
	void signal_handlers(int senhal){
	
		
	if (senhal == SIGINT){			//Una vez capturada  indicamos funcionalidad de la señal.
		printf("Era SIGINT\n");
		clean_module_collection(savecurrentiddone);
		clean_module_input_program();
		clear_confargs(confargs);
		// salimos del programade forma controlada
		exit(0);
		
	}
	  
	if (senhal == SIGTSTP){		//Una vez capturada  indicamos funcionalidad de la señal.
		printf("Era SIGTSTP\n");
		my_collection=sort_collection(my_collection, compare_inode);// ordenamos por inode
		
	}
	  
	if (senhal == SIGUSR1){		// Una vez capturada  indicamos funcionalidad de la señal.
		printf("Era SIGUSR1\n");
		
		my_collection=sort_collection(my_collection, compare_filename);// ordenamos por filename
	
  
	
	}
	
	}
	void inicialize_signal_handlers(){
	  // mapeo de las señalales utilizando signal (por ese motivo incluimos-->#include <signal.h> )
	  
		signal(SIGINT,signal_handlers);
		signal(SIGTSTP,signal_handlers);
		signal(SIGUSR1,signal_handlers);
		
		}
