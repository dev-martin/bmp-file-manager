
/*****************************************************************************

  Fichero     item.c

  Resumen     Fichero para trabajar con distintos item. Creación del item. 
              Métodos para comparación de parámetros del fichero. Impresión de
              características del fichero. Liberación de los campos del fichero.

  Autor       José Javier Blázquez Campos y Dulce Nombre de María Artalejo
              Sacristán

  Copyright   [Copyright (c) 2012 Universidad Carlos III de Madrid

  Se concede, sin necesidad de acuerdo por escrito y sin ningún tipo de derechos
  de autor, permiso para utilizar, copiar, modificar y distribuir este programa
  y su documentación para cualquier propósito siempre y cuando esta advertencia
  y los siguientes dos párrafos aparezcan en las copias.

  EN NINGÚN CASO SE RECONOCERÁ A LA UNIVERSIDAD CARLOS III DE MADRID RESPONSABLE
  DIRECTA, INDIRECTA, ESPECIAL, O SUBSIDIARIA DE LOS POSIBLES DAÑOS Y PERJUICIOS
  QUE PUEDAN DERIVARSE DEL USO DE ESTE PROGRAMA Y DE SU DOCUMENTACIÓN, INCLUSO
  EN EL CASO DE QUE LA UNIVERSIDAD CARLOS III DE MADRID HAYA SIDO ADVERTIDA DE
  TALES DAÑOS Y PERJUICIOS.

  LA UNIVERSIDAD CARLOS III DE MADRID ESPECÍFICAMENTE SE DESENTIENDE DE TODO
  TIPO DE GARANTÍAS INCLUYENDO, PERO NO LIMITANDOLAS A, LAS GARANTIAS IMPLÍCITAS
  DE LA COMERCIALIZACIÓN Y ADECUACIÓN PARA CUALQUIER PROPÓSITO EN PARTICULAR. EL
  PROGRAMA SE PROVEE TAL CUAL Y LA UNIVERSIDAD CARLOS III DE MADRID NO TIENE
  OBLIGACIÓN ALGUNA DE OFRECER MANTENIMIENTO, SOPORTE, ACTUALIZACIONES, MEJORAS
  O MODIFICACIONES.]

******************************************************************************/

//We include all the necessary libraries

#include "item.h"
#include "input_program.h"
#include "util_files.h"
#include "collection.h"
#include "node.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

//With this method we obtain the type of the file

char letter_filetype(mode_t mode){
  if (S_ISREG(mode)) return'-';
  if (S_ISDIR(mode)) return 'd';
  if (S_ISBLK(mode)) return 'b';
  if (S_ISCHR(mode)) return 'c';
  if (S_ISFIFO(mode)) return 'p';
  if (S_ISLNK(mode)) return 'l';
  if (S_ISSOCK(mode)) return 's';
  return '?';
}

//Here we create an item and if there is an error we take it into account

struct item create_item(char *path, char *filename, char *abs_filename,struct stat info,int *error,char *pathbmp){
  static int id = 0;
  char mode = letter_filetype(info.st_mode);
  struct item result;
  *error = 0; 
  result.id = id;
  id++;
	

  result.data.path = strdup(path);
  result.data.filename = strdup(filename);
  result.data.abs_filename = strdup(abs_filename);
  //READLINK code obtained from https://stackoverflow.com/questions/5525668/how-to-implement-readlink-to-find-the-path
  //We create a file depending on the type of that file.
  
  if(mode == 'l'){
   char buffer [PATH_MAX];
   ssize_t length = readlink(abs_filename, buffer, sizeof(buffer)-1);
    if (length != -1) {
      buffer[length] = '\0'; 
    }
    result.data.target_filename = strdup(buffer);
  }
  result.data.st_ino = info.st_ino;
  result.data.st_size = info.st_size;
  result.data.st_mtim = info.st_mtim;
  result.data.st_mode = info.st_mode;
  result.data.st_nlink = info.st_nlink;
  if(getpwuid(info.st_uid) == NULL)
    result.data.owner_name = NULL;
  else{
    struct passwd *user = getpwuid(info.st_uid);
    result.data.owner_name = strdup(user->pw_name);
  }
  if (getgrgid(info.st_gid) == NULL)
    result.data.group_name = NULL;
  else{
    struct group *grupo = getgrgid(info.st_gid);
    result.data.group_name = strdup(grupo->gr_name);
  }

  result.first = NULL;
  result.tail = NULL;
  result.current = NULL;
 
  if(pathbmp!=NULL){
  result.original=strdup(pathbmp);
  }else{ result.original=NULL;}
  
  return result;
}

//In this method we have deleted the item and we need to free all of it
//We will do this by freeing and equalling to NULL
  
void free_item(struct item *item){
  if (letter_filetype(item->data.st_mode) == 'l'){
    free (item->data.target_filename);
     item->data.target_filename = NULL;
   }
  free (item->data.path);
   item->data.path = NULL;
  
  free (item->data.filename);
   item->data.filename = NULL;
  
  free (item->data.abs_filename);
   item->data.abs_filename = NULL;
   
   if (item->data.owner_name != NULL){
    free(item->data.owner_name);
     item->data.owner_name = NULL;
   }
  
   if (item->data.group_name != NULL){
    free(item->data.group_name);
     item->data.group_name = NULL;
   }
   if (item->original != NULL){
    free(item->original);
     item->original = NULL;
   }
   
   
   
   clean_node(&item->first);
   
 
}

//Here we print the information of the chosen item

void print_item(struct item foo){
  
    printf("[ %d]\t%ld\t", foo.id,(long)foo.data.st_ino);    
    printf("%c", letter_filetype(foo.data.st_mode));

    //Printing of permissions obtained from
    //https://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c
    
    printf( (foo.data.st_mode & S_IRUSR) ? "r" : "-");
    printf( (foo.data.st_mode & S_IWUSR) ? "w" : "-");
    printf( (foo.data.st_mode & S_IXUSR) ? "x" : "-");
    printf( (foo.data.st_mode & S_IRGRP) ? "r" : "-");
    printf( (foo.data.st_mode & S_IWGRP) ? "w" : "-");
    printf( (foo.data.st_mode & S_IXGRP) ? "x" : "-");
    printf( (foo.data.st_mode & S_IROTH) ? "r" : "-");
    printf( (foo.data.st_mode & S_IWOTH) ? "w" : "-");
    printf( (foo.data.st_mode & S_IXOTH) ? "x" : "-");

    //Printing of last time modified from
    //https://www.tutorialspoint.com/c_standard_library/c_function_strftime.html
    
    printf("\t%ld\t",(long)foo.data.st_nlink);
    if (foo.data.owner_name  == NULL) printf("%ld\t",(long) foo.data.owner);
    else printf("%s\t",foo.data.owner_name);
    if (foo.data.group_name == NULL) printf("%ld\t", (long) foo.data.group);
    else printf ("%s\t", foo.data.group_name);
    printf("%d\t",(int)foo.data.st_size);
    
   struct tm *info;
   char buffer[80];
   info = localtime(&foo.data.st_mtim.tv_sec);

   strftime(buffer,80,"%x - %I:%M%p", info);
   printf("%s", buffer );

   printf("\t%s\n",foo.data.filename);
	 }

//We implement function about comparing id

int compare_id(const void *p, const void *q){
   struct  item *f = (struct item*)p;
   struct  item *s = (struct item*)q;

   if((f->id)>(s->id)){
     return 1;
   }
   if(s->id>f->id){
     return -1;
   }
   return 0;
	    
}

//We implement function about comparing filenames

int compare_filename(const void *p, const void *q){
  struct  item *f = (struct item*)p;
   struct  item *s = (struct item*)q;

   if(strcmp(f->data.filename,s->data.filename)>0){
     return 1;}
   if(strcmp(f->data.filename,s->data.filename)<0){
     return -1;
   }
   return 0;
}

//We implement function about comparing last modified

int compare_last_modified(const void *p, const void *q){
   struct  item *f = (struct item*)p;
   struct  item *s = (struct item*)q;

   if((f->data.st_mtim.tv_sec)>(s->data.st_mtim.tv_sec)){
     return 1;}
   if((s->data.st_mtim.tv_sec)>(f->data.st_mtim.tv_sec)){
     return -1;
   }
     return 0;
}

//We implement function about comparing size

int compare_size(const void *p, const void *q){
   struct  item *f = (struct item*)p;
   struct  item *s = (struct item*)q;

   if(f->data.st_size>s->data.st_size){
     return 1;}
   if(s->data.st_size>f->data.st_size){
     return -1;
   }
     return 0;
  
}

//We implement function about comparing inodes

int compare_inode(const void *pa, const void *pb){
   struct  item *f = (struct item*)pa;
   struct  item *s = (struct item*)pb;

   if(f->data.st_ino>s->data.st_ino){
     return 1;}
   if(s->data.st_ino>f->data.st_ino){
     return -1;
   }
     return 0;
  
}
