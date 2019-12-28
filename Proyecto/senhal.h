/*******************************************************************

  Fichero     senhal.h

  Resumen     Módulo para el manejo de señales

  Descripción Módulo para el manejo de senhales. Incluye la función que 
              inicializa las señales y los distintos manejadores de señales.

  Ver también 

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

#ifndef SENHAL_H
#define SENHAL_H

extern struct confArguments confargs;	
/*****************************************************************************

  Resumen      Inicializa el módulo.
  Descripción  Vincula las señales que se van a capturar por el programa
               con su correspondiente manejador de señales.
  Ver también  signal_handlers

******************************************************************************/
void inicialize_signal_handlers();

/* 
   Os copio también los prototipos de las funciones internas al módulo usadas
   por las otras funciones.
   Al ser internas NO deben ser declaradas en el .h
   Están en este comentario con fin únicamente orientativo (no tenéis que
   implementar las mismas funciones internas).
   En este caso también os indico qué hace para cada señal.

   Resumen      Manejador de señales
   Descripción  Manejador de las señales SIGINT y SIGTSTP. 
               La señal SIGINT: limpia la memoria usada y termina el programa.
               La señal SIGTSTP: ordena la colección interna por número de inodo
               La señal SIGUSR1: ordena la colección interna por nombre de fichero
   Nota: para enviar la señal SIGUSR1 deberás averiguar en otro terminal el identificador
         de proceso del programa y con el comando kill mandar la señal
   void signal_handlers(int senhal);
*/



#endif
