//bmp_ops.h

#include  "menu.h"


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
  int biCompression; // if compression BI_RGB = 0 ó BI_BITFIELDS = 3
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
#define FILE_DOES_NOT_E -1
#define FILE_DOES_NOT_OPEN -2
#define ERR_READING_FILE -3


//Method to load a bmp file
BMPFILE load_bmp (BMPFILE file, char*filename, int*error);

//Method to save a bmp file
int save_bmp (char*filename, BMPFILE bmp, int*error);

//Method to set one or two channels of a bmp file to 0
int zeros_bmp (char* filename, char*optional_1, int* error,char**p,char **a);
				
//Method to crop a bmp file
int crop_bmp(char* filename, POINT top_left, POINT bottom_right, 
				int* error,char**p,char **a);
				
//Method to obtain the histogram of the pixels of a bmp file
int hist_bmp (char* filename, int*error);
int mirror_bmp(char* filename, char* orientation, int* error, char** path_original,char** path_original2);
int level_bmp (char* filename, char*black_level, char*white_level, int* error,char** path_original,char** path_original2);
int gray_bmp(char* filename, char*optional_1, int* error, char** path_original,char** path_original2);
int  checkBMP(char* filename);
void freeBMP(BMPFILE bmp); 
int rotate_bmp(char* filename, char* orientation, int* error, char** path_original,char** path_original2);
int byw_bmp (char* filename, int* error, char** path_original,char** path_original2);
