/*******************************************************************

  Fichero     input_program.h

  Resumen     Módulo para la entrada por teclado por parte del usuario

  Descripción Módulo para la entrada por teclado por parte del usuario. Ofrece
              funciones para leer un número y una cadena por teclado. La cadena
              usada es una variable de módulo. Después de usar el módulo se 
              deberá limpiar la memoria usada llamando a la función 
              correspondiente.

  Autor       Iria Estévez Ayres

  Copyright   [Copyright (c) 2016 Universidad Carlos III de Madrid

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

#ifndef INPUT_PROGRAM_H
#define INPUT_PROGRAM_H

/*---------------------------------------------------------------------------*/
/* Declaración de constantes                                                 */
/*---------------------------------------------------------------------------*/

#define SUCCESS_INPUT 0
#define ERR_NO_NUMBERS_INPUT -1
#define ERR_FURTHER_CHARS_INPUT -2
#define ERR_CTRLD_INPUT -3

/*---------------------------------------------------------------------------*/
/* Declaración de variables                                                  */
/*---------------------------------------------------------------------------*/

/*
 * Esta variable y esta constante deberían ser internas al módulo y no expuesta en el .h 


#define NUM_INPUT_ERROR_MSGS 4

static const char *error_map[NUM_INPUT_ERROR_MSGS +1] =
  {
    "Success",
    "No digits were found",
    "Further characters after number",
    "CTRL+D was pressed (EOF)",
    "Unknown error"
  };

*/

/*---------------------------------------------------------------------------*/
/* Prototipos de las funciones                                               */
/*---------------------------------------------------------------------------*/

/*****************************************************************************

  Resumen      Entrada de una cadena por parte del usuario

  Descripción  Espera la entrada de una cadena por teclado por parte del 
               usuario. Devuelve la cadena introducida por el usuario, a menos
               que el usuario haya introducido CTRL+D (EOF) o CTRL +C 
               (señal SIGINT)

  Parámetros   int *error: dirección de una variable de tipo int donde devuelve
                           el error (si se produce). La cadena asociada al 
                           error se obtiene usando la función 
                           get_error_msg_input
               
  Efec. Colat. Se crea una cadena en el módulo que deberá ser liberada llamando
               a la función clean_module_input_program

  Ver también  get_error_msg_input, clean_module_input_program

******************************************************************************/
char *get_string(int *error);

/*****************************************************************************

  Resumen      Entrada de un número entero en la base seleccionada 
               por parte del usuario

  Descripción  Espera la entrada de un número entero por teclado por parte del 
               usuario en la base indicada. 
               Devuelve el número introducida por el usuario, a menos
               que el usuario haya introducido CTRL+D (EOF condition).

  Parámetros   int base: base en la que se quiere el número 
                         entero (e.g. 2, 10, 16)
               int *error: dirección de una variable de tipo int donde devuelve
                           el error (si se produce). La cadena asociada al 
                           error se obtiene usando la función 
                           get_error_msg_input

******************************************************************************/
long get_number(int base, int *error);

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
  Ver también  get_string, get_number

******************************************************************************/
const char *get_error_msg_input(int error);

/*****************************************************************************

  Resumen      Limpia el módulo

  Descripción  Libera la cadena interna al módulo (variable global en el fichero
               input_program.c) al finalizar de usarlo. Esta función deberá
               ser usada por todas aquellas funciones que pretendan salir 
               totalmente del programa (como aquéllas asociadas al manejo 
               de señales)
  Ver también  get_string

******************************************************************************/
void clean_module_input_program();

/* 
   Os copio también los prototipos de las funciones internas al módulo usadas
   por las otras funciones.
   Al ser internas NO deben ser declaradas en el .h
   Están en este comentario con fin únicamente orientativo (no tenéis que
   implementar las mismas funciones internas).
   
   void free_cadena();
*/


#endif
