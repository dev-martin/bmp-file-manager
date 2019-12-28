/*******************************************************************

  Fichero     collection.h

  Resumen     Módulo para la gestión, creación y manejo de la colección

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


#ifndef COLLECTION_H
#define COLLECTION_H
/*Esta biblioteca es necesario incluirla aquí para definir los items de la
  colección*/
#include "item.h"

/*---------------------------------------------------------------------------*/
/* Declaración de constantes                                                 */
/*---------------------------------------------------------------------------*/


#define INI_SIZE 20
#define SUCCESS_INPUT 0
#define ERR_NO_COLLECTION -1
#define ERR_NO_FREE_SPACE_COLLECTION -2
#define ERR_ELEMENT_ALREADY_COLL -3
#define MORE_FILES_THAN_SPACE -4
#define FILE_DOES_NOT_EXIST -5
#define ELEMENT_NOT_FOUND -6

#define NUM_COLLECTION_ERROR_MSGS 7

/*---------------------------------------------------------------------------*/
/* Declaración de estructuras                                                */
/*---------------------------------------------------------------------------*/

struct collection{

  int total_capacity;
  int number_occupied;
  struct item tabla[INI_SIZE];
};

/*---------------------------------------------------------------------------*/
/* Declaración de variables                                                  */
/*---------------------------------------------------------------------------*/
/*
 * Esta variable es interna al módulo y no expuesta en el .h 
   EStá definida en el módulo. Os la he copiado aquí con fines educativos
static const char *error_map_coll[NUM_COLLECTION_ERROR_MSGS] =
  {
    "Success",
    "No elements within the collection",
    "Total Capacity Reached",
    "Element already in the collection",
    "There were more files than space in the collection",
    "File does not exist",
    "Element not found"
  };

 */

/* 
   Esta variable está definida aquí sólo para poder ser usada por las
   funciones de manejo de señales. No se debe usar para otro fin
   Está definida en el .c como variable global al módulo.
*/
extern struct collection my_collection;

/*****************************************************************************

  Resumen      Dado un error de este módulo, devuelve la cadena asociada.

  Descripción  Dado un error de este módulo, devuelve la cadena asociada. 
               Se usan errores negativos y positivos en este módulo (esto
               se podría solucionar de otras maneras más elegantes). Los 
               negativos son los definidos en la variable error_map (se devuelve
               la cadena en la posición -error y los 
               positivos los que usan errno (y esta función devolvería
               directamente strerror(error). La función debe controlar cuando 
               error es negativo, que sea menor que el número de 
               errores definidos en el módulo.
  Parámetros   int error: error del que se quiere obtener la cadena de 
               error asociada.
******************************************************************************/
const char *get_error_msg_collection(int error);

/*****************************************************************************

  Resumen      Inicializa la colección a tamaño inicial INI_SIZE y 
               number_occupied a 0
******************************************************************************/
struct collection initialize_collection();

/*****************************************************************************

  Resumen      Libera toda la memoria reservada por este módulo.
  Descripción  Esta función deberá ser llamada al finalizar el programa para
               liberar toda la memoria.
******************************************************************************/

void clean_module_collection(int savecurrentiddone);

/*****************************************************************************

  Resumen      Dado un nombre de fichero lo añade, si es posible, a la 
               colección.

  Descripción  Dado un nombre de fichero lo añade, si es posible, a la 
               colección. Si el fichero no existe o si ya existe en la 
               colección no lo añade. Esta función es la encargada de 
               añadir el identificador único a cada nuevo item (los 
               identificadores no se reutilizan).
  Parámetros   struct collection my_collection: la colección a la que se 
                           va a añadir el nuevo fichero
               char *path: nombre del directorio donde está el fichero
               char *nombre: nombre del fichero a añadir               
               int *error: dirección de una variable de tipo int donde devuelve
                           el error (si se produce). La cadena asociada al 
                           error se obtiene usando la función 
                           get_error_msg_collection

  Efec. Col.   Se guarda el nombre del fichero en memoria dinámica, por lo que 
               se deberá liberar llamando a clean_module_collection

******************************************************************************/
struct collection add_file(struct collection my_collection,
			   char *path, char *nombre, int *error);
/*****************************************************************************

  Resumen      Añade todos los ficheros a la colección

  Descripción  Dado un nombre de directorio añade todos los ficheros que 
               sea posible a la colección. Si el fichero no existe o si 
               ya existe en la colección no lo añade.
               Para comprobar si ya existe comprueba su abs_filename.
  Parámetros   struct collection my_collection: la colección a la que se 
                           van a añadir todos los ficheros que sea posible
                           del directorio path 
               char *path: nombre del directorio donde están los ficheros a 
                           añadir.
               int *error: dirección de una variable de tipo int donde devuelve
                           el error (si se produce). La cadena asociada al 
                           error se obtiene usando la función 
                           get_error_msg_collection

  Efec. Col.   Se guarda el nombre de los ficheros en memoria dinámica, por 
               lo que se deberá liberar llamando a clean_module_collection

******************************************************************************/

struct collection add_all_directory(struct collection my_collection,
				    char *path, int *error);

/*****************************************************************************
  Resumen      Dado un identificador único en la colección lo borra, si es 
               posible, de la colección. 
  Parámetros   struct collection my_collection: la colección de la que se 
                       va a borrar el fichero
               int id: identificador único del fichero a borrar.       
               int *error: dirección de una variable de tipo int donde devuelve
                           el error (si se produce). La cadena asociada al 
                           error se obtiene usando la función 
                           get_error_msg_collection

******************************************************************************/
struct collection delete_item_id(struct collection my_collection,
				 int id, int *errnum,int savecurrentiddone);


/*****************************************************************************
  Resumen      Dada una colección, elimina todos sus elementos. La colección
               que se devuelve está en el mismo estado que una colección
               recién inicializada.
  Parámetros   struct collection my_collection: la colección que se va a borrar.

******************************************************************************/
struct collection delete_collection(struct collection my_collection,int savecurrentid);

/*****************************************************************************
  Resumen      Dado un identificador único en la colección lo borra, si es 
               posible, tanto de la colección como del sistema de ficheros.
  Parámetros   struct collection my_collection: la colección que se va a 
                           ordenar
               compare: función de comparación entre items de la tabla.
******************************************************************************/

struct collection sort_collection(struct collection my_collection,
				  int (*compare)(const void *, const void *));

/*****************************************************************************
  Resumen      Dado un identificador único en la colección, imprime su 
               información.
  Parámetros   struct collection my_collection: la colección 
               int id: identificador único del fichero
               int *errnum:  dirección de una variable de tipo int 
                           donde devuelve el error (si se produce). 
                           La cadena asociada al error se obtiene usando 
                           la función get_error_msg_collection
******************************************************************************/
void print_item_id(struct collection my_collection, int id, int *errnum);

/*****************************************************************************
  Resumen      Dada la colección, la imprime
  Parámetros   struct collection my_collection: la colección 
******************************************************************************/

void print_collection(struct collection my_collection);

/*****************************************************************************
  Resumen      Dada la colección, la guarda en fichero
  Parámetros   struct collection my_collection: la colección 
               char *filename: fichero donde guardar la colección.
                               este fichero se sobreescribirá, por lo que
                               deberá tener permisos de escritura.
               int *errnum:  dirección de una variable de tipo int 
                           donde devuelve el error (si se produce). 
                           La cadena asociada al error se obtiene usando 
                           la función get_error_msg_collection
******************************************************************************/
void save_to_file(struct collection my_collection, char *filename, int *errnum);

/* 
   Os copio también los prototipos de las funciones internas al módulo usadas
   por las otras funciones.
   Al ser internas NO deben ser declaradas en el .h
   Están en este comentario con fin únicamente orientativo (no tenéis que
   implementar las mismas funciones internas).
   
   void print_cabecera();
   int find_item(struct collection my_collection, int id, int *error);
   int find_item_name(struct collection my_collection, char *abs_name, int *error);
*/
#endif
