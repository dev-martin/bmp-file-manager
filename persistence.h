char* file_exists( char *filename, int *error);
void save_col( char *nfile,struct collection my_collection, int ops,int *error);//nota este volcar te hace todo los campos del item.
struct collection load_col(struct collection my_collection,char *filename,int ops,int *error,int loadtype );
void checkPersistance(int persistnoops,int persistsave);
