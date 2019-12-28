#include "util_files.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

//Funcion  ->info_file

int info_file(char *path, char *filename, char **abs_filename, struct stat* bufStat){
		
		 *abs_filename=NULL;
		char r_path[PATH_MAX]; // definimos variable con tamaño PATH_MAX
		int e;				  // creamos vafr para almacenar error
		realpath(path, r_path);	//Expamde el nombre del directorio introducido como un "." al real y lo almacenamos en r_path
		
		strcat(r_path, "/");	//concatenacion
		strcat(r_path, filename);//concatenacion
		
		*abs_filename=strdup(r_path); // guardamos  r_path en el abs_filename
	
		e=lstat(r_path, bufStat);//
		return e;
		
		
}

	//realpath()
	
	/*realpath() expands all symbolic links and resolves references to /./,
       /../ and extra '/' characters in the null-terminated string named by
       path to produce a canonicalized absolute pathname.  The resulting
       pathname is stored as a null-terminated string, up to a maximum of
       PATH_MAX bytes, in the buffer pointed to by resolved_path.  The
       resulting path will have no symbolic link, /./ or /../ components.

       If resolved_path is specified as NULL, then realpath() uses malloc(3)
       to allocate a buffer of up to PATH_MAX bytes to hold the resolved
       pathname, and returns a pointer to this buffer.  The caller should
       deallocate this buffer using free(3).*/
       
       
	//stat()
	//These functions return information about a file
	
	//	lstat()
	
	/*The lstat() function shall be equivalent to stat(), except when path refers to a symbolic link. 
	 * In that case lstat() shall return information about the link, 
	 * while stat() shall return information about the file the link references.*/
