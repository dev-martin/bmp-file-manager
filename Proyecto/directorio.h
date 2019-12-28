/*******************************************************************

  Fichero     directorio.h

  Resumen     Módulo para la lectura de contenidos de directorios.

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
#ifndef DIRECTORIO_H
#define DIRECTORIO_H
/* Se incluye la biblioteca dirent porque contiene la definición 
   de la estructura struct dirent */
#include <dirent.h>

/*****************************************************************************

  Resumen      Dado un nombre de directorio, lee los ficheros 
               que contiene (y devuelve en la variable info_files, que es un 
               array, su información) 
               y devuelve el número de ficheros existentes.
  Descripción  Dado un nombre de directorio, lee los ficheros 
               que contiene (y devuelve en la variable info_files, que es un 
               array, su información) 
               y devuelve el número de ficheros existentes.
  Parámetros   char *path: nombre del directorio (se deberá usar scandir)
               struct dirent ***info_files: dirección de una variable de 
                           tipo struct dirent **
                           (array de struct dirent *) donde devuelve
                           los datos de todos los ficheros 
                           ese directorio.
                           Cuidado porque el campo dt_name de este array 
                           contiene los nombres absolutos de 
                           los ficheros por lo que se deberá usar la función
                           realpath o una equivalente.
               int *error: dirección de una variable de tipo int donde devuelve
                           el error (si se produce). En este caso, devuelve 
                           el valor de errno, por lo que el mensaje se 
                           deberá imprimir usando strerror
               Devuelve el número de ficheros leídos (la longitud del array
               all_files)         
  Efec. Colat. Se crean en memoria dinámica tanto el array info_files como 
               cada uno de sus elementos,
               por lo que se deberá llamar a free_info_files_array una 
               vez se procese el array.

******************************************************************************/
int info_all_files_dir(char *path, struct dirent ***info_files, int *error);

/*****************************************************************************

  Resumen      Dado un array de estructuras de tipo struct dirent y su 
               longitud, libera la memoria pedida.
  Parámetros   char **info_files: array pedido previamente por 
                                  info_all_filenames_dir
               int len: tamaño del array
               
******************************************************************************/
void free_info_files_array( struct dirent **info_files, int n);

/* 
  En este módulo no he definido más funciones 
*/

#endif
