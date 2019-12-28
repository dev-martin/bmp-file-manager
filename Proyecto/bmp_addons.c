int mirror_bmp(char* filename, char* orientation, int* error, char* path_original){
	
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
	strcat (template , getenv("HOME"));
	strcat (template, "/.saucem/fileXXXXXX");
	
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
	
	path_original = template;
	
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

	return 0;
}

int level_bmp (char* filename, char*black_level, char*white_level, int* error, char* path_original){
	
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
	for(int t=0;t<PATH_MAX;t++){
			template[t]=0;
	}
	
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
	
	path_original = template;
	
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
		 printf("Error while saving changes.");
		 return -1;
	 }
	return 0;	
}


int mirror_bmp(char* filename, char* orientation, int* error, char* path_original){
	
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
	strcat (template , getenv("HOME"));
	strcat (template, "/.saucem/fileXXXXXX");
	
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
	
	path_original = template;
	
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

	return 0;
}
