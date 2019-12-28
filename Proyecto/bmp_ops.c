#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "menu.h"
#pragma pack(push, 1)
typedef struct tagBITMAPFILEHEADER {
  char type[2]; // Should be "BM"
  int bfSize;  // 4 bytes with the size
  char  bfReserved[4]; // 4 bytes reserved
  int bfOffBits; // offset where the pixel array can be found
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct tagBITMAPINFOHEADER {
  int biSize;  // size of the DIB header from this point
  int  biWidth; // width in pixels
  int  biHeight; //height in pixels
  short  biPlanes; // number of planes
  short  biBitCount; // number of bits per pixel 
  int biCompression; // if compression BI_RGB = 0 Ã³ BI_BITFIELDS = 3
  int biSizeImage; // size of the array data including padding
  int biXPelsPerMeter; // hor resolution of the image
  int  biYPelsPerMeter; // ver resoilution of the image
  int biClrUsed; // number of colours in the palette, 0 if it uses all
  int biClrImportant; // number of important colours, 0 if all are imp
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;
#pragma pack(pop)

typedef struct __attribute__((__packed__)){
  unsigned char b;
  unsigned char g;
  unsigned char r;
} PIXELS;


typedef struct image{
  BITMAPFILEHEADER fh;
  BITMAPINFOHEADER ih;
  char *alignment; // characters between header and pixel map
  unsigned int padding; // row padding within the pixel map
  PIXELS *im; // Here you can use either an array or a multidimensional array
  // If you want to use a multidimensional array you should
  // define PIXELS **im; and perform 2 mallocs instead of one
} BMPFILE;





#define SUCCESS_INPUT 0
#define FILE_DOES_NOT_EXIST -1
#define FILE_DOES_NOT_OPEN -2
#define ERR_READING_FILE -3


BMPFILE load_bmp (BMPFILE file, char*filename, int*error){
	
	/*We create a file with the route of the filename and try to open it
	 * on Read Binary mode. If there is any problem, we return an empty
	 * file and an error to be checked*/
	 
	char dir_abs[PATH_MAX];
	FILE * image;
	
	realpath(".", dir_abs);    
  	strcat(dir_abs,"/");
  	strcat(dir_abs, filename);
	
	image = fopen(filename, "rb");
		if(image == NULL){
			printf("Error while opening");
			*error = FILE_DOES_NOT_OPEN;
				fclose(image);
				return file;
		}
		
	/*We read the BMP file in order to obtain its header and information 
	 * and we store it in the BMP File Header and BMP Information Header 
	 * structures*/	
	 
	BITMAPFILEHEADER header;

	size_t num = fread(&header, sizeof(BITMAPFILEHEADER), 1, image);
	
	if(num!=1){
		printf("Error reading the header. Cannot verify it is a BMP file.");
		*error = ERR_READING_FILE;
			fclose(image);
			return file;
	}
	
	BITMAPINFOHEADER information;
	
	num = fread(&information, sizeof(BITMAPINFOHEADER), 1, image);
	
	if(num!=1){
		printf("Error reading the header. Cannot verify it is a BMP file.");
		*error = ERR_READING_FILE;
			fclose(image);
			return file;
	}
	
	/*We assign the file header and info header to our bmp file arguments*/
	
	file.fh = header;
	file.ih = information;
	
	/*Since the argument information.biSizeImage image includes the 
	 * padding and the Width argument doesn't include the padding pixels,
	 * we can obtain the padding argument by performing the following 
	 * operation */
	 
	file.padding = (information.biSizeImage-(information.biHeight*information.biWidth*sizeof(PIXELS)))/(information.biHeight);

	/*If there is any sort of alignment between the header and the data 
	 * array, we store it in the alignment argument of the image*/
	 
	size_t gap = header.bfOffBits-sizeof(BITMAPFILEHEADER)- sizeof(BITMAPINFOHEADER);
	
	if(gap !=0){
		file.alignment = malloc(gap*sizeof(char));
		num = fread(file.alignment, gap, 1, image);
		if(num!=1){
			printf("Error reading the header. Cannot verify it is a BMP file.");
			*error = ERR_READING_FILE;
				fclose(image);
				return file;
		}
	}
	
	/*We prepare the argument of the data array with the proper size of 
	 * it and store it, in this case we also store the padding bytes in 
	 * it. In our case, we've also decided to use an unidimensional array.*/
	 
	file.im = malloc(information.biSizeImage);
	num = fread(file.im, information.biSizeImage, 1, image);
	if(num!=1){
		printf("Error while reading");
		*error = ERR_READING_FILE;
			free(file.im);
			fclose(image);
			return file;
	}
	
	/*At this point, the bmp file is properly stored, we can return a
	 *success error value and return the file*/
	 
	fclose(image);
	*error = SUCCESS_INPUT;
		return file;
}

int save_bmp (char*filename, BMPFILE bmp, int*error){
	FILE* nuevo;
	nuevo = fopen(filename, "w");
	
	size_t num;
	
	num = fwrite(&bmp.fh, sizeof(BITMAPFILEHEADER),1, nuevo);
	
	if(num!= 1){
		printf("ERROR while writing the File Header.");
		return -1;
	}
	
	num = fwrite(&bmp.ih, sizeof(BITMAPINFOHEADER),1, nuevo);
	
	if(num!= 1){
		printf("ERROR while writing the Information header");
		return -1;
	}
	
	if(bmp.fh.bfOffBits-sizeof(BITMAPFILEHEADER)-sizeof(BITMAPINFOHEADER)!= 0){
		//printf("ALLIGNMENT");
			num = fwrite(bmp.alignment, bmp.fh.bfOffBits-sizeof(BITMAPFILEHEADER)-sizeof(BITMAPINFOHEADER),1, nuevo);
			if(num!= 1){
				printf("ERROR while writing the alignment between header and pixel data array");
				return -1;
			}
	}
			
	num = fwrite(bmp.im, bmp.ih.biSizeImage,1, nuevo);
	
	if(num!= 1){
		printf("ERRORO while writing the pixel data array");
		return -1;
	}
	
	fclose (nuevo); 
	return 0;
}	

int zeros_bmp (char* filename, char*optional_1, int* error, char** path_original,char** path_original2){
	//~ *path_original=(char **)malloc(100);
	//~ *path_original2=(char **)malloc(100);
		/*IMPLEMENTATION COMMENT: OPTIONAL 1 AND OPTIONAL MUST BE FILLED ALWAYS, IF
	 * ONLY ONE PIXEL WANTS TO BE ERASED, ADD AS A PARAMETER A - OR WHATEVER BUT
	 * COLOR CODES*/
	 
	/* We define a BMPFILE and call load_bmp() method to load the image. 
	 * If after the call is there any type of error, we revise it and 
	 * return an error for the whole method in that case*/
	 
	BMPFILE bmp;
	
	bmp = load_bmp (bmp, filename, error);
	
	if(*error != 0){
		printf("Error while loading the BMP: %d\n", *error);
		return -1;
	}
	
	/*We make a temporal file in order to store the original bmp file
	 * in case the user wants to undo the operation. The path where this
	 * temporal file is saved will be stored as original_path argument*/
	
	char template[PATH_MAX];
	memset(template, 0x00, sizeof(template));
	strcat (template , getenv("HOME"));
	strcat (template, "/fileXXXXXX");
	
	int fd;
	fd = mkstemp(template);
	
	if(fd == -1){
		printf("Error creating the temporal file.");
		return -1;
	}
	
	char*  non_bmp= strdup(template);
	strcat(template,".bmp");
	
	int rem = remove(non_bmp);
	if( rem != 0){
		printf ("Unable to remove the temporal file.");
		return -1;
	}	
	
	*path_original = strdup(template);
	
	int backup = save_bmp(template, bmp, error);
	if(backup!=0){
		printf("Error while saving backup.");
		return -1;
	}
	

	/*We traverse the whole pixel data array and set to 0 the values of 
	 * the pixel colors stated by the user (Up to 2 colors). WARNING: 
	 * Don't panic, with pictures so defined as the ones uploaded by Iria,
	 * this whole for loop could take up to 2 minutes to be completely
	 * traversed (There are almost 9 million pixels in some pictures) 
	 * Be patient and wait*/
	 
	int i = 0;
	printf ("Changing color of the image. This process could take a while, please wait.\n");
	for (i = 0; i < bmp.ih.biSizeImage/sizeof(PIXELS); i++){
		//printf("%d\n", i);
		if (strcmp (optional_1, "b") == 0 ){
			//printf ("Changing blue pixel.\n");
			bmp.im[i].b = 0;
		}
		if (strcmp (optional_1, "g") == 0){
			//printf ("Changing green pixel.\n");
			bmp.im[i].g = 0;
		}
		if (strcmp (optional_1, "r") == 0){
			//printf ("Changing red pixel.\n");
			bmp.im[i].r = 0;	
		}
			/*printf ("%d\n", bmp.im[i].b);
			printf ("%d\n", bmp.im[i].g);
			printf ("%d\n", bmp.im[i].r);*/

	}
	
	/*We open the image again in order to overwrite it with the operation asked
	 * by the user*/
	 
	 int saving = save_bmp (filename, bmp, error);
	 if( saving !=0){
		 printf("Error while saving changes.here");
		 return -1;
	 }
	 
	 
	 //Guardar bmp temporal de como queda la foto despues de la OP tambien en ./saucem para poder volver a ella en redo
	 
	char template2[PATH_MAX];
	
	memset(template2, 0x00, sizeof(template2));
	
	
	
	strcat (template2 , getenv("HOME"));
	strcat (template2, "/fileXXXXXX");
	
	int fd2;
	fd2= mkstemp(template2);
	
	if(fd2 == -1){
		printf("Error creating the temporal file.");
		return -1;
	}
	
	char*  non_bmp2= strdup(template2);
	strcat(template2,".bmp");
	
	int rem2 = remove(non_bmp2);
	if( rem2 != 0){
		printf ("Unable to remove the temporal file.saucem");
		return -1;
	}	
	
	*path_original2 = strdup(template2);
	
	
	
	int backup2 = save_bmp(template2, bmp, error);
	if(backup2!=0){
		printf("Error while saving backup.");
		return -1;
	}
	 
	return 0;	
}


int crop_bmp(char* filename, POINT top_left, POINT bottom_right, int* error, char** path_original,char** path_original2){
	
	/* We define a BMPFILE and call load_bmp() method to load the image. If after
	 * the call is there any type of error, we revise it and return an error for the
	 * whole method in that case*/
	 
		/*Given the cropping arguments, we calculate the pixels of the original picture where 
	 * our cropped picture begins and ends*/
	 BMPFILE bmp;
	
	bmp = load_bmp (bmp, filename, error);
	
	if(*error != 0){
		printf("Error while loading the BMP: %d\n", *error);
		return -1;
	}
	
	/*We make a temporal file in order to store the original bmp file
	 * in case the user wants to undo the operation. The path where this
	 * temporal file is saved will be stored as original_path argument*/
	
	char template[PATH_MAX];
	
	memset(template, 0x00, sizeof(template));
	
	strcat (template , getenv("HOME"));
	strcat (template, "/fileXXXXXX");
	
	int fd;
	fd = mkstemp(template);
	
	if(fd == -1){
		printf("Error creating the temporal file.");
		return -1;
	}
	
	char*  non_bmp= strdup(template);
	strcat(template,".bmp");
	
	int rem = remove(non_bmp);
	if( rem != 0){
		printf ("Unable to remove the temporal file.");
		return -1;
	}	
	
	*path_original = strdup(template);
	
	
	
	int backup = save_bmp(template, bmp, error);
	if(backup!=0){
		printf("Error while saving backup.");
		return -1;
	}
	


	 
	int y1 = bmp.ih.biHeight-(bmp.ih.biHeight * (bottom_right.y/100.0)); 
	int y2 = bmp.ih.biHeight-(bmp.ih.biHeight * (top_left.y/100.0));
	
	int x1 = bmp.ih.biWidth * (top_left.x/100.0)  ;
	int x2 = bmp.ih.biWidth * (bottom_right.x/100.0) ;

	/*We calculate the new padding of the picture*/
	 
	int new_padding = (4 - ((x2-x1) * 3) % 4) %4;
	
	/*We create a temporal data array to store the cropped image*/
	PIXELS *tmp = malloc ((y2-y1)*(x2-x1+new_padding)*sizeof(PIXELS));
	int size_data = 0;
	int n;
	int i;
	int deviation;
	if (bmp.padding !=0){
		deviation = 4-bmp.padding;
	} else{
		deviation = 0;
	}
	printf("Cropping image. This could take a while.");
	for (n = y1; n< y2; n++){
		for (i =(x1+(n-1)*(bmp.ih.biWidth+deviation)); i< (x2+(n-1)*(bmp.ih.biWidth+deviation)); i++){		
			
			tmp[size_data] = bmp.im[i];
			size_data++;
		}
	}
	
	/*We set the new values of the image*/
	bmp.im = tmp;
	bmp.ih.biWidth = x2-x1;
	bmp.ih.biHeight = y2-y1;
	bmp.padding = new_padding;
	bmp.ih.biSizeImage = (bmp.ih.biWidth*(sizeof(PIXELS))+bmp.padding)*bmp.ih.biHeight;
	bmp.fh.bfSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+(bmp.fh.bfOffBits-sizeof(BITMAPFILEHEADER)-sizeof(BITMAPINFOHEADER))+ bmp.ih.biSizeImage;

	/*We write the file taking into account the new padding*/
	FILE* nuevo;
	nuevo = fopen(filename, "w");
	
	size_t num;
	num = fwrite(&bmp.fh, sizeof(BITMAPFILEHEADER),1, nuevo);
	if(num!= 1){
		printf("ERROR. file header");
	}
	num = fwrite(&bmp.ih, sizeof(BITMAPINFOHEADER),1, nuevo);
	if(num!= 1){
		printf("ERROR. info header");
	}
	
	if(bmp.fh.bfOffBits-sizeof(BITMAPFILEHEADER)-sizeof(BITMAPINFOHEADER)!= 0){
			num = fwrite(bmp.alignment, bmp.fh.bfOffBits-sizeof(BITMAPFILEHEADER)-sizeof(BITMAPINFOHEADER),1, nuevo);
			if(num!= 1){
		printf("ERROR. alignment offset");
		}
	}
	
	char pad = 0;
	int v;
	int u;
	PIXELS* copy = &tmp[0];
	for (v = 0; v<bmp.ih.biHeight; v++){
		num = fwrite(copy, bmp.ih.biWidth*sizeof(PIXELS),1, nuevo);
		copy = &tmp[(v+1)*bmp.ih.biWidth];
		if(num!= 1){
		printf("ERROR. data line");
		}
		for (u = 0; u<bmp.padding; u++){
			num = fwrite(&pad, sizeof(char), 1, nuevo);
			if(num!= 1){
				printf("ERROR. padding");
			}
		}
	}
	
	fclose (nuevo); 
	
	 int saving = save_bmp (filename, bmp, error);
	 if( saving !=0){
		 printf("Error while saving changes.here");
		 return -1;
	 }
	 
	 
	 //Guardar bmp temporal de como queda la foto despues de la OP tambien en ./saucem para poder volver a ella en redo
	 
	char template2[PATH_MAX];
	
	memset(template2, 0x00, sizeof(template2));
	
	
	
	strcat (template2 , getenv("HOME"));
	strcat (template2, "/fileXXXXXX");
	
	int fd2;
	fd2= mkstemp(template2);
	
	if(fd2 == -1){
		printf("Error creating the temporal file.");
		return -1;
	}
	
	char*  non_bmp2= strdup(template2);
	strcat(template2,".bmp");
	
	int rem2 = remove(non_bmp2);
	if( rem2 != 0){
		printf ("Unable to remove the temporal file.saucem");
		return -1;
	}	
	
	*path_original2 = strdup(template2);
	
	
	
	int backup2 = save_bmp(template2, bmp, error);
	if(backup2!=0){
		printf("Error while saving backup.");
		return -1;
	}
	 


	return 0;
}

int hist_bmp (char* filename, int*error){
	
	/* We define a BMPFILE and call load_bmp() method to load the image. If after
	 * the call is there any type of error, we revise it and return an error for the
	 * whole method in that case*/
	 
	BMPFILE bmp;

	bmp = load_bmp (bmp, filename, error);
	printf("ERROR: %d\n", *error);
	if(*error != 0){
		return -1;
	}

	/*We create an array with each intensity value for each color*/
	
	int intensity_map [3][256];

	/*We initialize all the values of the intensity array to zero*/

	int i;
	int j;
	for (j = 0; j<3; j++){
		for (i= 0; i<256; i++){
			intensity_map[j][i] = 0;
		}
	}
	
	/*We traverse the pixels data array and whether each color pixel intensity 
	 * value is stored in our array, add 1 to that value*/
	 
	for (i = 0; i < bmp.ih.biSizeImage/sizeof(PIXELS); i++){
		
			intensity_map[0][bmp.im[i].b] = intensity_map[0][bmp.im[i].b] + 1 ;
			intensity_map[1][bmp.im[i].g] = intensity_map[1][bmp.im[i].g] + 1;
			intensity_map[2][bmp.im[i].r] = intensity_map[2][bmp.im[i].r] + 1;
	}
	
	/*We create a temporal file with mkstemp on the folder .saucem/ */
	
	char template[PATH_MAX];
	memset(template, 0x00, sizeof(template));
	
	strcat (template , getenv("HOME"));
	strcat (template, "/fileXXXXXX");
	
	int fd;
	fd = mkstemp(template);
	
	if(fd == -1){
		printf("Error creating the temporal file.");
		return -1;
	}
	
	/*As we need a .csv file, we strdup the directory of the temporal file,
	 * remove the temporal created and make a new one with the .csv extension*/
	 
	char*  non_csv= strdup(template);
	strcat(template,".csv");
	
	int rem = remove(non_csv);
	if( rem != 0){
		printf ("Unable to remove the temporal file.");
		return -1;
	}	
	
	
	FILE * file;
	 file = fopen(template, "wb");
	 
	/* We print the obtained values in the desired format on the .csv file*/
	 
	fprintf(file, "Intensidad \t Rojo \t Verde \t Azul\n");
	
	for (i= 0; i<256; i++){
		
		fprintf(file, "%d", i);
		fprintf(file, "\t");
		fprintf(file, "%d", intensity_map[2][i]);
		fprintf(file, "\t");
		fprintf(file, "%d", intensity_map[1][i]);
		fprintf(file, "\t");
		fprintf(file, "%d", intensity_map[0][i]);
		fprintf(file, "\n");
	}
	
	fclose(file);
	
	/*We prepare the arguments of the command gnuplot in order to execute it later
	 * (SEE pinto_hist.gp TO KNOW THE STRUCTURE OF THE COMMAND)*/
	 
	char inputfile [PATH_MAX];
	
	strcat(inputfile, "inputfile='");
	strcat(inputfile, template);
	strcat(inputfile, "'");
	
	char outputfile [PATH_MAX];
	memset(outputfile, 0x00, sizeof(outputfile));
	
	strcat(outputfile, "outputfile='histo.png'");
	
	char* gp = "pinto_hist.gp";
	
	char *args[] = { "gnuplot", "-e", inputfile, "-e", outputfile, gp, NULL };
	
	/*We create a child process necessary to execute gnuplot, if there is any
	 * error we detect it, and if it is properly created we make an execution call
	 * and make the parent process wait for its child with waitpid() method
	 * SOURCE: https://stackoverflow.com/questions/37388845/invoking-gnuplot-to-plot-through-a-file-in-a-c-program*/
	 
	int status = 0;
	pid_t pid;
	pid = fork();
		if(pid==-1){
			printf("Error while forking.");
		}else if (pid == 0){
			execvp(*args, args);
		}
    
    waitpid (pid, &status, 0);
			
	/*After creating the .png file in the folder where the code is, we 
	 * delete the .csv temporal file. If there is
	 * any error while removing it we exit the program*/
	 
	rem = remove(template);
	if( rem != 0){
		printf ("Unable to remove the temporal file.");
		return -1;
	}	
	
	/*The histogram was made successfully*/
	
	return 0;
	
	
}

int gray_bmp(char* filename, char*optional_1, int* error, char** path_original,char** path_original2){
	//~ *path_original=(char **)malloc(100);
	//~ *path_original2=(char **)malloc(100);
		/*IMPLEMENTATION COMMENT: OPTIONAL 1 AND OPTIONAL MUST BE FILLED ALWAYS, IF
	 * ONLY ONE PIXEL WANTS TO BE ERASED, ADD AS A PARAMETER A - OR WHATEVER BUT
	 * COLOR CODES*/
	 
	/* We define a BMPFILE and call load_bmp() method to load the image. 
	 * If after the call is there any type of error, we revise it and 
	 * return an error for the whole method in that case*/
	 
	BMPFILE bmp;
	
	bmp = load_bmp (bmp, filename, error);
	
	if(*error != 0){
		printf("Error while loading the BMP: %d\n", *error);
		return -1;
	}
	
	/*We make a temporal file in order to store the original bmp file
	 * in case the user wants to undo the operation. The path where this
	 * temporal file is saved will be stored as original_path argument*/
	
	char template[PATH_MAX];
	memset(template, 0x00, sizeof(template));
	strcat (template , getenv("HOME"));
	strcat (template, "/fileXXXXXX");
	
	int fd;
	fd = mkstemp(template);
	
	if(fd == -1){
		printf("Error creating the temporal file.");
		return -1;
	}
	
	char*  non_bmp= strdup(template);
	strcat(template,".bmp");
	
	int rem = remove(non_bmp);
	if( rem != 0){
		printf ("Unable to remove the temporal file.");
		return -1;
	}	
	
	*path_original = strdup(template);
	
	int backup = save_bmp(template, bmp, error);
	if(backup!=0){
		printf("Error while saving backup.");
		return -1;
	}
	

	/*We traverse the whole pixel data array and set to 0 the values of 
	 * the pixel colors stated by the user (Up to 2 colors). WARNING: 
	 * Don't panic, with pictures so defined as the ones uploaded by Iria,
	 * this whole for loop could take up to 2 minutes to be completely
	 * traversed (There are almost 9 million pixels in some pictures) 
	 * Be patient and wait*/
	 
	int i = 0;
	printf ("Changing gray scale of the image. This process could take a while, please wait.\n");
	for (i = 0; i < bmp.ih.biSizeImage/sizeof(PIXELS); i++){
		//printf("%d\n", i);
		if (strcmp (optional_1, "b") == 0 ){
			//printf ("Changing blue pixel.\n");
			bmp.im[i].r=bmp.im[i].b;
			bmp.im[i].g=bmp.im[i].b;
		}
		if (strcmp (optional_1, "g") == 0){
			//printf ("Changing green pixel.\n");
			bmp.im[i].r = bmp.im[i].g;
			bmp.im[i].b = bmp.im[i].g;
		}
		if (strcmp (optional_1, "r") == 0){
			//printf ("Changing red pixel.\n");
			bmp.im[i].g = bmp.im[i].r;
			bmp.im[i].b = bmp.im[i].r;	
		}
		if (strcmp (optional_1, "y") == 0 ){
			//printf ("Changing blue pixel.\n");
			bmp.im[i].r=0.212656*bmp.im[i].r;
			bmp.im[i].g=0.715158*bmp.im[i].g;
			bmp.im[i].b=0.072186*bmp.im[i].b;;
		}
	}
	
	/*We open the image again in order to overwrite it with the operation asked
	 * by the user*/
	 
	 int saving = save_bmp (filename, bmp, error);
	 if( saving !=0){
		 printf("Error while saving changes.here");
		 return -1;
	 }
	 
	 
	 //Guardar bmp temporal de como queda la foto despues de la OP tambien en ./saucem para poder volver a ella en redo
	 
	char template2[PATH_MAX];
	
	memset(template2, 0x00, sizeof(template2));
	
	
	
	strcat (template2 , getenv("HOME"));
	strcat (template2, "/fileXXXXXX");
	
	int fd2;
	fd2= mkstemp(template2);
	
	if(fd2 == -1){
		printf("Error creating the temporal file.");
		return -1;
	}
	
	char*  non_bmp2= strdup(template2);
	strcat(template2,".bmp");
	
	int rem2 = remove(non_bmp2);
	if( rem2 != 0){
		printf ("Unable to remove the temporal file.saucem");
		return -1;
	}	
	
	*path_original2 = strdup(template2);
	
	
	
	int backup2 = save_bmp(template2, bmp, error);
	if(backup2!=0){
		printf("Error while saving backup.");
		return -1;
	}
	 
	return 0;	
}
int mirror_bmp(char* filename, char* orientation, int* error, char** path_original,char** path_original2){
	
	/* We define a BMPFILE and call load_bmp() method to load the image. If after
	 * the call is there any type of error, we revise it and return an error for the
	 * whole method in that case*/
	 
	BMPFILE bmp;
	
	bmp = load_bmp (bmp, filename, error);
	printf("ERROR: %d\n", *error);
	if(*error != 0){
		return -1;
	}
	
	/*We make a temporal file in order to store the original bmp file
	 * in case the user wants to undo the operation. The path where this
	 * temporal file is saved will be stored as original_path argument*/
	 
	char template[PATH_MAX];
	memset(template, 0x00, sizeof(template));
	strcat (template , getenv("HOME"));
	strcat (template, "/fileXXXXXX");
	
	int fd;
	fd = mkstemp(template);
	
	if(fd == -1){
		printf("Error creating the temporal file.");
		return -1;
	}
	
	char*  non_bmp= strdup(template);
	strcat(template,".bmp");
	
	int rem = remove(non_bmp);
	if( rem != 0){
		printf ("Unable to remove the temporal file.");
		return -1;
	}	
	
	*path_original = strdup(template);
	
	int backup = save_bmp(template, bmp, error);
	if(backup!=0){
		printf("Error while saving backup.");
		return -1;
	}
	
	/*Given the cropping arguments, we calculate the pixels of the original picture where 
	 * our cropped picture begins and ends*/
	 


	PIXELS *tmp = malloc (bmp.ih.biSizeImage);
	int size_data = 0;
	int n;
	int i;
	int deviation;
	if (bmp.padding !=0){
		deviation = 4-bmp.padding;
	} else{
		deviation = 0;
	}
	printf("Cropping image. This could take a while.");
	if(strcmp(orientation, "v") == 0){
		for (n = bmp.ih.biHeight; n > 0 ; n--){
			for (i =((n-1)*(bmp.ih.biWidth+deviation)); i< (bmp.ih.biWidth+(n-1)*(bmp.ih.biWidth+deviation)); i++){		
			
				tmp[size_data] = bmp.im[i];
				size_data++;
			}
		}
	}else if(strcmp(orientation, "h") == 0){
		for (n = 1; n < bmp.ih.biHeight ; n++){
			for (i = (bmp.ih.biWidth+(n-1)*(bmp.ih.biWidth+deviation)); i> ((n-1)*(bmp.ih.biWidth+deviation)); i--){		
			
				tmp[size_data] = bmp.im[i];
				size_data++;
			}
		}
	}
	
	/*We set the new values of the image*/
	bmp.im = tmp;

	/*We write the file taking into account the new padding*/
	FILE* nuevo;
	nuevo = fopen(filename, "w");
	
	size_t num;
	num = fwrite(&bmp.fh, sizeof(BITMAPFILEHEADER),1, nuevo);
	if(num!= 1){
		printf("ERROR. file header");
	}
	num = fwrite(&bmp.ih, sizeof(BITMAPINFOHEADER),1, nuevo);
	if(num!= 1){
		printf("ERROR. info header");
	}
	
	if(bmp.fh.bfOffBits-sizeof(BITMAPFILEHEADER)-sizeof(BITMAPINFOHEADER)!= 0){
			num = fwrite(bmp.alignment, bmp.fh.bfOffBits-sizeof(BITMAPFILEHEADER)-sizeof(BITMAPINFOHEADER),1, nuevo);
			if(num!= 1){
		printf("ERROR. alignment offset");
		}
	}
	
	char pad = 0;
	int v;
	int u;
	PIXELS* copy = &tmp[0];
	for (v = 0; v<bmp.ih.biHeight; v++){
		num = fwrite(copy, bmp.ih.biWidth*sizeof(PIXELS),1, nuevo);
		copy = &tmp[(v+1)*bmp.ih.biWidth];
		if(num!= 1){
		printf("ERROR. data line");
		}
		for (u = 0; u<bmp.padding; u++){
			num = fwrite(&pad, sizeof(char), 1, nuevo);
			if(num!= 1){
				printf("ERROR. padding");
			}
		}
	}
	
	fclose (nuevo); 
	 int saving = save_bmp (filename, bmp, error);
	 if( saving !=0){
		 printf("Error while saving changes.here");
		 return -1;
	 }
	 
	 
	 //Guardar bmp temporal de como queda la foto despues de la OP tambien en ./saucem para poder volver a ella en redo
	 
	char template2[PATH_MAX];
	
	memset(template2, 0x00, sizeof(template2));
	
	
	
	strcat (template2 , getenv("HOME"));
	strcat (template2, "/fileXXXXXX");
	
	int fd2;
	fd2= mkstemp(template2);
	
	if(fd2 == -1){
		printf("Error creating the temporal file.");
		return -1;
	}
	
	char*  non_bmp2= strdup(template2);
	strcat(template2,".bmp");
	
	int rem2 = remove(non_bmp2);
	if( rem2 != 0){
		printf ("Unable to remove the temporal file.saucem");
		return -1;
	}	
	
	*path_original2 = strdup(template2);
	
	
	
	int backup2 = save_bmp(template2, bmp, error);
	if(backup2!=0){
		printf("Error while saving backup.");
		return -1;
	}
	 
	return 0;
}
int rotate_bmp(char* filename, char* orientation, int* error, char** path_original,char** path_original2){
	
	//DISCLAIMER: LA FUNCIÓN ROTATE SOLO FUNCIONA PARA FOTOS SIN PADDING, PARA EL RESTO NO SE GUARDA CORRECTAMENTE
	
	
	/* We define a BMPFILE and call load_bmp() method to load the image. If after
	 * the call is there any type of error, we revise it and return an error for the
	 * whole method in that case*/
	 
	BMPFILE bmp;
	
	bmp = load_bmp (bmp, filename, error);
	printf("ERROR: %d\n", *error);
	if(*error != 0){
		return -1;
	}
	
	/*We make a temporal file in order to store the original bmp file
	 * in case the user wants to undo the operation. The path where this
	 * temporal file is saved will be stored as original_path argument*/
	 
	char template[PATH_MAX];
	memset(template, 0x00, sizeof(template));
	strcat (template , getenv("HOME"));
	strcat (template, "/fileXXXXXX");
	
	int fd;
	fd = mkstemp(template);
	
	if(fd == -1){
		printf("Error creating the temporal file.");
		return -1;
	}
	
	char*  non_bmp= strdup(template);
	strcat(template,".bmp");
	
	int rem = remove(non_bmp);
	if( rem != 0){
		printf ("Unable to remove the temporal file.");
		return -1;
	}	
	
	*path_original = strdup(template);
	
	int backup = save_bmp(template, bmp, error);
	if(backup!=0){
		printf("Error while saving backup.");
		return -1;
	}
	
	/*Given the cropping arguments, we calculate the pixels of the original picture where 
	 * our cropped picture begins and ends*/
	 


	PIXELS *tmp = malloc (bmp.ih.biSizeImage);
	int size_data = 0;
	int n;
	int i;
	int deviation;
	if (bmp.padding !=0){
		deviation = 4-bmp.padding;
	} else{
		deviation = 0;
	}
	printf("Cropping image. This could take a while.");
	if(strcmp(orientation, "l") == 0){
		for (i = 0; i < bmp.ih.biWidth; i++){		
				for (n = bmp.ih.biHeight-1; n >= 0  ; n--){
				tmp[size_data] = bmp.im[(n*(bmp.ih.biWidth+deviation))+i+1];
				size_data++;
			}
		}
	}else if(strcmp(orientation, "r") == 0){
			for (i = 0; i < bmp.ih.biWidth; i++){		
				for (n = 0; n < bmp.ih.biHeight ; n++){
				tmp[size_data] = bmp.im[(n*(bmp.ih.biWidth+deviation))+i+1];
				size_data++;
			}
		}
	}
	
	int new_padding = (4 - (bmp.ih.biHeight)*12 % 4) %4;
	/*We set the new values of the image*/
	bmp.im = tmp;
	int temporal_height = bmp.ih.biHeight;
	
	bmp.padding = new_padding;
	bmp.ih.biHeight = bmp.ih.biWidth;
	bmp.ih.biWidth = temporal_height;
	
	
	/*We write the file taking into account the new padding*/
	FILE* nuevo;
	nuevo = fopen(filename, "w");
	
	size_t num;
	num = fwrite(&bmp.fh, sizeof(BITMAPFILEHEADER),1, nuevo);
	if(num!= 1){
		printf("ERROR. file header");
	}
	num = fwrite(&bmp.ih, sizeof(BITMAPINFOHEADER),1, nuevo);
	if(num!= 1){
		printf("ERROR. info header");
	}
	
	if(bmp.fh.bfOffBits-sizeof(BITMAPFILEHEADER)-sizeof(BITMAPINFOHEADER)!= 0){
			num = fwrite(bmp.alignment, bmp.fh.bfOffBits-sizeof(BITMAPFILEHEADER)-sizeof(BITMAPINFOHEADER),1, nuevo);
			if(num!= 1){
		printf("ERROR. alignment offset");
		}
	}
	
	char pad = 0;
	int v;
	int u;
	PIXELS* copy = &tmp[0];
	for (v = 0; v<bmp.ih.biHeight; v++){
		num = fwrite(copy, bmp.ih.biWidth*sizeof(PIXELS),1, nuevo);
		copy = &tmp[(v+1)*bmp.ih.biWidth];
		if(num!= 1){
		printf("ERROR. data line");
		}
		for (u = 0; u<bmp.padding; u++){
			num = fwrite(&pad, sizeof(char), 1, nuevo);
			if(num!= 1){
				printf("ERROR. padding");
			}
		}
	}
	
	fclose (nuevo); 
	 int saving = save_bmp (filename, bmp, error);
	 if( saving !=0){
		 printf("Error while saving changes.here");
		 return -1;
	 }
	 
	 
	 //Guardar bmp temporal de como queda la foto despues de la OP tambien en ./saucem para poder volver a ella en redo
	 
	char template2[PATH_MAX];
	
	memset(template2, 0x00, sizeof(template2));
	
	
	
	strcat (template2 , getenv("HOME"));
	strcat (template2, "/fileXXXXXX");
	
	int fd2;
	fd2= mkstemp(template2);
	
	if(fd2 == -1){
		printf("Error creating the temporal file.");
		return -1;
	}
	
	char*  non_bmp2= strdup(template2);
	strcat(template2,".bmp");
	
	int rem2 = remove(non_bmp2);
	if( rem2 != 0){
		printf ("Unable to remove the temporal file.saucem");
		return -1;
	}	
	
	*path_original2 = strdup(template2);
	
	
	
	int backup2 = save_bmp(template2, bmp, error);
	if(backup2!=0){
		printf("Error while saving backup.");
		return -1;
	}
	 
	return 0;
}

int byw_bmp (char* filename, int* error, char** path_original,char** path_original2){
	
	
	gray_bmp(filename, "y", error, path_original, path_original2);
	
	// Otsu's Method code obtained from : https://stackoverflow.com/questions/33693423/otsu-thresholding
	
	/* We define a BMPFILE and call load_bmp() method to load the image. If after
	 * the call is there any type of error, we revise it and return an error for the
	 * whole method in that case*/
	 
	BMPFILE bmp;
	
	bmp = load_bmp (bmp, filename, error);
	printf("ERROR: %d\n", *error);
	if(*error != 0){
		return -1;
	}
	
	/*We make a temporal file in order to store the original bmp file
	 * in case the user wants to undo the operation. The path where this
	 * temporal file is saved will be stored as original_path argument*/
	 
	char template[PATH_MAX];
	memset(template, 0x00, sizeof(template));
	strcat (template , getenv("HOME"));
	strcat (template, "/fileXXXXXX");
	
	int fd;
	fd = mkstemp(template);
	
	if(fd == -1){
		printf("Error creating the temporal file.");
		return -1;
	}
	
	char*  non_bmp= strdup(template);
	strcat(template,".bmp");
	
	int rem = remove(non_bmp);
	if( rem != 0){
		printf ("Unable to remove the temporal file.");
		return -1;
	}	
	
	*path_original = strdup(template);
	
	int backup = save_bmp(template, bmp, error);
	if(backup!=0){
		printf("Error while saving backup.");
		return -1;
	}
	
	/*Given the cropping arguments, we calculate the pixels of the original picture where 
	 * our cropped picture begins and ends*/
	 
	 
	float histogram [256];

	/*We initialize all the values of the intensity array to zero*/

	int i;
	
		for (i= 0; i<256; i++){
			histogram[i] = 0;
		}
	
	
	/*We traverse the pixels data array and whether each color pixel intensity 
	 * value is stored in our array, add 1 to that value*/
	 
	for (i = 0; i < bmp.ih.biSizeImage/sizeof(PIXELS); i++){
		
			histogram[bmp.im[i].b] = histogram[bmp.im[i].b] + 1 ;
			
	}
	
	for (i = 0; i < 256; i++){
		
			histogram[i] /= (float)(bmp.ih.biHeight*bmp.ih.biWidth);
	}
	
	float probability[256], mean[256];
    float max_between, between[256];
    int threshold;
	
	for(int i = 0; i < 256; i++) {
        probability[i] = 0.0;
        mean[i] = 0.0;
        between[i] = 0.0;
    }
	
	probability[0] = histogram[0];

    for(int i = 1; i < 256; i++) {
        probability[i] = probability[i - 1] + histogram[i];
        mean[i] = mean[i - 1] + i * histogram[i];
    }

    threshold = 0;
    max_between = 0.0;

    for(int i = 0; i < 255; i++) {
        if(probability[i] != 0.0 && probability[i] != 1.0)
            between[i] = (mean[255] * probability[i] - mean[i])*(mean[255] * probability[i] - mean[i]) / (probability[i] * (1.0 - probability[i]));
    else{
        between[i] = 0.0;
	}
        if(between[i] > max_between) {
            max_between = between[i];
            threshold = i;
        }
    }
	
	for (i = 0; i < bmp.ih.biSizeImage/sizeof(PIXELS); i++){
		//printf("%d\n", i);
		if (bmp.im[i].b < threshold){
			//printf ("Changing blue pixel.\n");
			bmp.im[i].b = 0;
			bmp.im[i].g = 0;
			bmp.im[i].r = 0;
		}else{
			bmp.im[i].b = 255;
			bmp.im[i].g = 255;
			bmp.im[i].r = 255;
		}
	}
	
	//------------------------------------------------------------------------------------------------ 
	 
	 int saving = save_bmp (filename, bmp, error);
	 if( saving !=0){
		 printf("Error while saving changes.here");
		 return -1;
	 }
	 
	 
	 //Guardar bmp temporal de como queda la foto despues de la OP tambien en ./saucem para poder volver a ella en redo
	 
	char template2[PATH_MAX];
	
	memset(template2, 0x00, sizeof(template2));
	
	
	
	strcat (template2 , getenv("HOME"));
	strcat (template2, "/fileXXXXXX");
	
	int fd2;
	fd2= mkstemp(template2);
	
	if(fd2 == -1){
		printf("Error creating the temporal file.");
		return -1;
	}
	
	char*  non_bmp2= strdup(template2);
	strcat(template2,".bmp");
	
	int rem2 = remove(non_bmp2);
	if( rem2 != 0){
		printf ("Unable to remove the temporal file.saucem");
		return -1;
	}	
	
	*path_original2 = strdup(template2);
	
	
	
	int backup2 = save_bmp(template2, bmp, error);
	if(backup2!=0){
		printf("Error while saving backup.");
		return -1;
	}
	 
	return 0;	
}

int level_bmp (char* filename, char*black_level, char*white_level, int* error,char** path_original,char** path_original2){
	
	/*IMPLEMENTATION COMMENT: OPTIONAL 1 AND OPTIONAL MUST BE FILLED ALWAYS, IF
	 * ONLY ONE PIXEL WANTS TO BE ERASED, ADD AS A PARAMETER A - OR WHATEVER BUT
	 * COLOR CODES*/
	 
	/* We define a BMPFILE and call load_bmp() method to load the image. 
	 * If after the call is there any type of error, we revise it and 
	 * return an error for the whole method in that case*/
	 
	BMPFILE bmp;
	
	bmp = load_bmp (bmp, filename, error);
	
	if(*error != 0){
		printf("Error while loading the BMP: %d\n", *error);
		return -1;
	}
	
	/*We make a temporal file in order to store the original bmp file
	 * in case the user wants to undo the operation. The path where this
	 * temporal file is saved will be stored as original_path argument*/
	 
	char template[PATH_MAX];
	memset(template, 0x00, sizeof(template));
	
	strcat (template , getenv("HOME"));
	strcat (template, "/fileXXXXXX");
	
	int fd;
	fd = mkstemp(template);
	
	if(fd == -1){
		printf("Error creating the temporal file.");
		return -1;
	}
	
	char*  non_bmp= strdup(template);
	strcat(template,".bmp");
	
	int rem = remove(non_bmp);
	if( rem != 0){
		printf ("Unable to remove the temporal file.");
		return -1;
	}	
	
	*path_original = template;
	
	int backup = save_bmp(template, bmp, error);
	if(backup!=0){
		printf("Error while saving backup.");
		return -1;
	}
	
	/*We traverse the whole pixel data array and set to 0 the values of 
	 * the pixel colors stated by the user (Up to 2 colors). WARNING: 
	 * Don't panic, with pictures so defined as the ones uploaded by Iria,
	 * this whole for loop could take up to 2 minutes to be completely
	 * traversed (There are almost 9 million pixels in some pictures) 
	 * Be patient and wait*/
	 
	int i = 0;
	int percentage_black = 255* (atoi(black_level) /100.0);
		printf("BLACK %d\n", percentage_black);
	int percentage_white = 255 * (atoi(white_level)/100.0);
		printf("WHITE %d\n", percentage_white);
	printf ("Changing color of the image. This process could take a while, please wait.\n");
	for (i = 0; i < bmp.ih.biSizeImage/sizeof(PIXELS); i++){
		//printf("%d\n", i);
		if (bmp.im[i].b <= percentage_black){
			//printf ("Changing blue pixel.\n");
			bmp.im[i].b = 0;
		}else if (bmp.im[i].b >= percentage_white){
			//printf ("Changing blue pixel.\n");
			bmp.im[i].b = 255;
		}else if(bmp.im[i]. b < percentage_white && bmp.im[i].b > percentage_black) {
			//printf("ENTRÉ AZUL");
			bmp.im[i].b = bmp.im[i].b * (((bmp.im[i].b*100/255.0) - atoi(black_level) )/ (atoi(white_level)- atoi(black_level)));
		}
		
		if (bmp.im[i].g <= percentage_black){
			//printf ("Changing blue pixel.\n");
			bmp.im[i].g = 0;
		}else if (bmp.im[i].g >= percentage_white){
			//printf ("Changing blue pixel.\n");
			bmp.im[i].g = 255;
		}else if (bmp.im[i].g < percentage_white && bmp.im[i].g > percentage_black){
			//printf("ENTRÉ verde");
			bmp.im[i].g = bmp.im[i].g * (((bmp.im[i].g*100/255.0) - atoi(black_level) )/ (atoi(white_level)- atoi(black_level)));
		}
		
		if (bmp.im[i].r <= percentage_black){
			//printf ("Changing blue pixel.\n");
			bmp.im[i].r = 0;
		}else if (bmp.im[i].r >= percentage_white){
			//printf ("Changing blue pixel.\n");
			bmp.im[i].r = 255;
		}else if (bmp.im[i].r < percentage_white && bmp.im[i].r > percentage_black){
			//printf("ENTRÉ rojo");
			bmp.im[i].r = bmp.im[i].r * (((bmp.im[i].r*100/255.0) - atoi(black_level) )/ (atoi(white_level)- atoi(black_level)));
		}
		
			/*printf ("%d\n", bmp.im[i].b);
			printf ("%d\n", bmp.im[i].g);
			printf ("%d\n", bmp.im[i].r);*/

	}
	
	/*We open the image again in order to overwrite it with the operation asked
	 * by the user*/
	
	 int saving = save_bmp (filename, bmp, error);
	 if( saving !=0){
		 printf("Error while saving changes.here");
		 return -1;
	 }
	 
	 
	 //Guardar bmp temporal de como queda la foto despues de la OP tambien en ./saucem para poder volver a ella en redo
	 
	char template2[PATH_MAX];
	
	memset(template2, 0x00, sizeof(template2));
	
	
	
	strcat (template2 , getenv("HOME"));
	strcat (template2, "/fileXXXXXX");
	
	int fd2;
	fd2= mkstemp(template2);
	
	if(fd2 == -1){
		printf("Error creating the temporal file.");
		return -1;
	}
	
	char*  non_bmp2= strdup(template2);
	strcat(template2,".bmp");
	
	int rem2 = remove(non_bmp2);
	if( rem2 != 0){
		printf ("Unable to remove the temporal file.saucem");
		return -1;
	}	
	
	*path_original2 = strdup(template2);
	
	
	
	int backup2 = save_bmp(template2, bmp, error);
	if(backup2!=0){
		printf("Error while saving backup.");
		return -1;
	}
	 

	 
	return 0;	
}




int  checkBMP(char* filename){
	
	FILE *file;
	struct stat buffer;
	if((file = fopen(filename, "rb")) == NULL){
		puts("can not open this file");
		
		return -1;
	}
	
	char BM[3];
	BM[0] =0;
	BM[1] = 0;
	BM[2] = '\0';
	if (fread(BM, 1, 2, file) && (strcmp("BM",BM)==0)) {
	
	
		int size=0;
		fread(&size, 1, sizeof(int), file);

		if (stat(filename, &buffer) == -1) {
			puts("Error while doing the stat");
			fclose(file);
		return -1;
		}
    fclose(file);
	return 0;
	}else if((strcmp("BM",BM)!=0)){
		//printf("Operation not supported. The file is not of the proper type.\n");
	fclose(file);
	return -1;
	}else{
						fclose(file);
	return -1;
	}
}
void freeBMP(BMPFILE bmp){
	if(bmp.alignment!=NULL){
	free(bmp.alignment);
	}
	if (bmp.im!=NULL){
				free(bmp.im);
				
	}
}
