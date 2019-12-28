/*******************************************************************

  Fichero     item.h

  Resumen     Módulo para la gestión, creación y manejo de los item de 
              la colección

  Autor       Iria Estévez Ayres

  Copyright   [Copyright (c) 2017 Universidad Carlos III de Madrid

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

#ifndef ITEM_H
#define ITEM_H

/*
  Esta biblioteca se incluye aquí y no en el .c 
  sólo porque es la que contiene la definición del tipo de 
  datos struct timespec
*/
#include <sys/stat.h>
#include "menu.h"
#include <sys/types.h>
/*---------------------------------------------------------------------------*/
/* Declaración de estructuras                                                */
/*---------------------------------------------------------------------------*/
struct node {
	char *operation;
	char *parameter;
	char *parameter2;
	char *previousop;
	char *currentop;
	struct node *next;
	struct node *prev;
	int id;
	POINT p1;
	POINT p2;
};

struct info{
  char *path;               /* path to the file */
  char *filename;           /* filename (relative) */
  char *abs_filename;       /* absolute name of the file */
  char *target_filename;     /* only if the file is a symbolic link */
  
  ino_t st_ino;             /* Inode Number */
  off_t     st_size;        /* total size, in bytes */
  nlink_t st_nlink;         /* number of hard links */
  struct timespec st_mtim;  /* time of last modification */
  mode_t st_mode;           /* protection, type of file*/

  uid_t owner;              /* numeric id of the owner */
  gid_t group;              /* numeric id of the group */
  
  /* name of the owner and of the group. They should be NULL if the names 
     are not found using the functions getpwuid and getgrgid */
  char *owner_name; 
  char *group_name;

};

struct item{
  int id;
  pid_t pid;
  int fd[2];
  struct info data;
  struct node *first;
  struct node *tail;
  struct node *current;
  char *original;
};

/*****************************************************************************

  Resumen      Funciones que dadas las direcciones de dos items, los comparan 
               por id, por nombre, por fecha de última modificación, por
               tamaño, por número de inodo. 
               Estas funciones son usadas por la función de ordenación
  Descripción  Funciones que dadas las direcciones de dos items, los comparan 
               por id, por nombre, por fecha de última modificación y por
               tamaño. Estas funciones son usadas por la función de ordenación.
               Todas devuelven 0 si los ítems son iguales, -1 si p < q y 1 si
               p > q
  Parámetros   const void *p Dirección del primer item a comparar
               const void *q Dirección del segundo item a comparar

******************************************************************************/

int compare_id(const void *p, const void *q);
int compare_filename(const void *p, const void *q);
int compare_last_modified(const void *p, const void *q);
int compare_size(const void *p, const void *q);
int compare_inode(const void *pa, const void *pb);

/*****************************************************************************

  Resumen      Dado un nombre de fichero, crea el item correspondiente.
  Parámetros   char *path: nombre del directorio.
               char *filename: nombre del fichero.
               char *abs_filename: nombre del fichero.
               int *error: dirección de una variable de tipo int donde devuelve
                           el error (si se produce). En este caso, devuelve 
                           el valor de errno, por lo que el mensaje se 
                           deberá imprimir usando strerror
               
  Efec. Colat. Se crean (en memoria dinámica) las cadenas 
               path, filename, abs_filename, target_filename, owner_name 
               y group_name (estas tres últimas sólo si es necesario, tal
               y como indica la definición de la estructura). 
               Estas cadenas deberán ser liberadas si se borra el elemento.

******************************************************************************/

struct item create_item(char *path, char *filename, char *abs_filename,
			struct stat info,
			int *error,
			char *originalpath);

/*****************************************************************************
 Resumen      Dado un item, libera la memoria usada y deja sus 
              campos puntero a NULL
******************************************************************************/
void free_item(struct item *item);

/*****************************************************************************

  Resumen      Dado un item lo imprime por pantalla

******************************************************************************/

void print_item(struct item foo);


char letter_filetype(mode_t mode);
/* 
  En este módulo he definido la siguiente función auxiliar:

  char letter_filetype(mode_t mode);
  Que devuelve un char dependiendo del tipo de fichero:
  Si es un fichero regular: '-' 
  Si es un directorio: 'd'
  Si es un fichero de bloque: 'b' 
  Si es un fichero de caracteres: 'c' 
  Si es un pipe: 'p' 
  Si es un enlace simbólico: 'l' 
  Si es un socket: 's' 
  Si es cualquier otra cosa: '?' 

 Usa las macros: S_ISREG(m), S_ISDIR(m), S_ISBLK(m), S_ISCHR(m), S_ISFIFO(m)
                 S_ISLNK(m), S_ISSOCK(m)
 Se puede encontrar información sobre estas macros en la página de manual
 de lstat
*/

#endif
