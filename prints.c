
#include <stdio.h>
void printmenu(){
	puts("Type help to see the user guide");
	//puts("Enter an Option : ");
}

void printhelpS(){
	printf("Valid commands: load_dir, add, delete, sort, info, infoall, help,\n");
	printf("add_op, undo, redo, clean, save_col, load_col, logon, logoff, exit\n\n");
	puts("More Information?(Y/N):");
}

#include <stdio.h>
void printhelpPREV(){
	puts("Program for handling and managing files\n\n");
	printf("Usage: ./saucem_enhanced [--help] [--sort size|name|time|default]");
	printf("[-l logfile] [-i file] [-o file] [--persist clean|no-ops|no-save]\n\n");
	printf("--help \t\t\t shows this text and quits\n");
	printf("--sort size|name|time|inode|default   sets the internal");
	printf("order of the collection:\n");
	printf("\t\t\t\t\t  size    By file size, ascendant\n");
	printf("\t\t\t\t\t  name    By file name\n");
	printf("\t\t\t\t\t  time    By modification time\n");
	printf("\t\t\t\t\t  inode    By inode number\n");
	printf("\t\t\t\t\t  default By node identifier\n");
	printf("-l logfile \t\t sets the path of the log file\n");
	printf("-i file \t\t indicates the file from where the initial\n");
	printf("\t\t\t\t\t collection is read\n");
	printf("-o file \t\t indicates the file where the collection is\n");
	printf("\t\t\t\t\t stored at the end of the program\n");
	printf("--persist clean|no-ops|no-save \t options regarding the persistence:\n");
	printf("\t\t\t\t\t clean   The program begins with an empty collection\n");
	printf("\t\t\t\t\t no-save  The collection is not stored at the end\n");
	printf("\t\t\t\t\t no-ops  At the initial and persistence files,\n");
	printf("\t\t\t\t\t\t he list of ops of each item are not read/stored\n");
	
}

void printhelpL(){
	printf("./saucem_enhanced is a file manager developed by SAUCEM S.L.");
	printf("that offers the following options:\n\n");
	printf("load_dir \t dir \t - adds all the files and dirs within dir to\n");
	printf("\t\t\t    collection is read\n");
	printf("add file_name \t\t - adds the file file_name into the colletion\n");
	printf("delete id \t\t - removes the node with identifier id from the collection\n");
	printf("\t\t\t    without erasing the file from the file system\n");
	printf("sort size|name|time|inode|def - sorts the collection according to a criterion and changes\n");
	printf("\t\t\t         the order in which new elements are stored from now on\n");
	printf("info id \t\t - shows information of the node with identifier id\n");
	printf("infoall \t\t - shows the information of every node in the collection\n");
	printf("help \t\t\t - shows this text\n");
	printf("add_op id [opA|opB|opC|opD] - adds an operation to the list of operations\n");
	printf("\t\t\t       of an id\n");
	printf("undo id \t\t - undoes the last scheduled op for an id.\n");
	printf("redo id \t\t - reverts the last undo .\n");
	printf("clean id \t\t - cleans all the scheduled ops for an id.\n");
	printf("save_col [no-ops] file_name - stores the current collection in file file_name.\n");
	printf("\t\t\t       If no-ops is present the list of operations will\n");
	printf("\t\t\t       not be stored\n");
	printf("load_col [no-ops] file_name - loads the collection stored in file file_name\n");
	printf("\t\t\t       into the program (this operation destroys the\n");
	printf("\t\t\t       current collection).\n");
	printf("\t\t\t       If no-ops is present the list of operations will\n");
	printf("\t\t\t       no be loaded \n");
	printf("logon \t\t\t - option for administrators: prints on screen messages.\n");
	printf("\t\t\t    being stored in the log file\n");
	printf("logoff \t\t\t - option for administrators: stop printing on screen.\n");
	printf("\t\t\t    messages above. A logoff without a previous logon doesn't\n");
	printf("\t\t\t    have any effect \n");
	printf("exit  \t\t\t - quits the program and stores the collection into the \n");
	printf("\t\t\t    persistence file \n");
	printf("If you enter CTRL+D in this menu, the program will quit storing (and merging) the \n");
	printf("current collection into the persistence file. ");
	printf("If you enter CTRL+D in a submenu, the program will return to this menu.\n\n");
	printf("The program captures the following signals:\n\n");	
	printf("SIGINT:  if CTRL+C is pressed, the program quits safely\n");
	printf("SIGTSTP: if CTRL+Z is pressed, the program changes the internal order of the\n");
	printf("\t  collection to by inode number\n");
	printf("SIGUSR1: if SIGUSR1 is captured, the program changes the internal order of the\n");
	printf("\t  collection by filename\n\n\n");
	printf("Enter an option:");
} 

void printRESyes(){
			puts(".....................................................START................................................................\n");
			puts(".....................................................005................................................................\n");
			puts("...This is the statistic research of the Team 005...\n\n");
			puts(".....................................................005................................................................\n");
			puts("... ---DESCRIPTION--- ...\n\n");
			puts(".....................................................005................................................................\n");
			puts("....Here the IT develompent group of this app will take into account the main usage that the user is giving ...\n");
			puts("...  to this app in order to work harder on those concrete aspects ....\n");
			puts(".....................................................005................................................................\n");
			puts("... ---RESEARCH ---...\n\n");
			puts(".....................................................005................................................................\n");
}
