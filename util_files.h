/*******************************************************************

  Fichero     util_files.h

  Resumen     Módulo para las funciones relativas a información de ficheros
              del sistema operativo

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
#ifndef UTIL_FILES_H
#define UTIL_FILES_H
#include <sys/stat.h>

/*****************************************************************************

  Resumen      Información acerca de un fichero
  Descripción  Dado un directorio y un nombre de fichero, consulta su
               información y su path absoluto.
  Parámetros   char *path: nombre del directorio
               char *filename: nombre del fichero  
               char **abs_filename: dirección de una variable de tipo
                         char * (cadena), donde se guardará el path
                         absoluto del fichero. Para ello, se deberá usar
                         realpath sobre el directorio y concatenar el
                         nombre de fichero
               struct stat *bufStat: dirección de una variable de 
                           tipo struct stat donde devuelve
                           la información relativa a ese fichero. 
                           Se deberá usar la función lstat para consultar
                           la información (ver la diferencia entre stat y
                           lstat en la página de manual)
               int *error: dirección de una variable de tipo int donde devuelve
                           el error (si se produce). En este caso, devuelve 
                           el valor de errno, por lo que el mensaje se 
                           deberá imprimir usando strerror
               Devuelve si se ha producido un error. En este caso, devuelve 
               el valor de errno, por lo que el mensaje se deberá imprimir 
               usando strerror.
  Efec. Colat. Si la llamada ha tenido éxito, se crea en memoria dinámica  
               abs_filename, por lo que se deberá llamar a free cuando
               ya no se necesite.

******************************************************************************/

int info_file(char *path, char *filename, char **abs_filename,
	      struct stat *bufStat);

#endif

