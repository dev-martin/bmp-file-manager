

struct confArguments{
		char *p;
		char *l;
		char* i;
		char *s;	
		int bydef;
};
struct confArguments readconf(int *error);
void clear_confargs(struct confArguments confargs);
struct confArguments setconfbydfault(struct confArguments argconf, int *error,char *path); 
struct confArguments setconfbydfault(struct confArguments argconf, int *error,char *path);
