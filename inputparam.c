#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

//#include  "input_param.h"
// Programa que recibe como argumento -f nombre -o salida
// -f nombre se puede poner como --file nombre
// y -o salida como --output salida
//Si está el argumento -h (o --help) presente, presenta la ayuda

#define MAL_ARGS 1
struct argumentos{
  int help;
  int sort1;
  int persist1;
  int log;
  int ini;
  int stored;
  char *sorttype;
  char *logfile;
  char *persist;
  char *inifile;
  char *storedfile;
};

static struct option long_options[] = {
  {"help", no_argument,       0,  'h' },
  {"sort",  required_argument, 0, 's'},
  {"persist",    required_argument, 0,'p'},
  {0,         0,                 0,  0 }
  // {--,         requierearg?,                 0,  - }
};


void limpio_resultado(struct argumentos arg){
  if (arg.persist){
    free(arg.persist);
    arg.persist=NULL;
  }
  if (arg.sorttype){
    free(arg.sorttype);
    arg.sorttype=NULL;
  }
  if (arg.logfile){
    free(arg.logfile);
    arg.logfile=NULL;
  }
  if (arg.inifile){
    free(arg.inifile);
    arg.inifile=NULL;
  }
  if (arg.storedfile){
    free(arg.storedfile);
    arg.storedfile=NULL;
  }

}
struct argumentos proceso_args(int num, char **argumentos,int *error){
 struct argumentos resultado;
  int option_index=0;
  int c=0;
  resultado.help = 0;
  resultado.sort1=0;
  resultado.persist1=0;
  resultado.log=0;
  resultado.ini=0;
  resultado.stored=0;
  resultado.sorttype=NULL;
  resultado.logfile=NULL;
  resultado.inifile=NULL;
  resultado.storedfile=NULL;
  resultado.persist=NULL;
  
  while ((c = getopt_long(num, argumentos,"hs:p:l:i:o:",long_options, &option_index))!=-1){
    switch(c){
	case 'h':{
		resultado.help=1;
	break;
	}
	
	case 's': {		
	if (optarg){
		puts("entras en sort");
		resultado.sorttype=strdup(optarg);   
		resultado.sort1=1;
		}
	break;
	}
	case 'p':{
	if (optarg){
		puts("entras en persistence");
		resultado.persist=strdup(optarg);   
		resultado.persist1=1;
	}
	break;
	}
    case 'l':{
		if (optarg)
		puts("entras en logfile");
      resultado.logfile = strdup(optarg);
      resultado.log=1;
	}
     break; 
    case 'i':{
		if (optarg)
       resultado.inifile = strdup(optarg);
       puts("entras en	inifile");
       resultado.ini=1;
	}
      break;
	
    case 'o':{
		if (optarg)
       resultado.storedfile = strdup(optarg);
       puts("entras en storedfile");
       resultado.stored=1;
    }
      break;
	
    case '?':{
      /*Para si el ultimo es un argumento con param requerido*/
      puts("?");
      *error = MAL_ARGS;
    }
      break;
	
    default:{
      /*Por si hay errores*/
      *error = MAL_ARGS;
      //printf("entra en default on %d",c);
      printf("?? getopt returned character code 0%o ??\n", c);
	}
    }
  }
  
  /*En nuestro caso no hay más argumentos y si los hubiese debería dar 
    un error*/
  if ((optind < num)|| (*error == MAL_ARGS)){
    limpio_resultado(resultado);
    resultado.help = 1;
    *error=MAL_ARGS;
  }
    
  return resultado;
}
