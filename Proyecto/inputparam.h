struct argumentos{
  int help;
  int sort1;
  int persist1;
  int log;
  int ini;
  int stored;
  char *sorttype;
  char *logfile;
  char *persist;
  char *inifile;
  char *storedfile;
};

void limpio_resultado(struct argumentos arg);

struct argumentos proceso_args(int num, char **argumentos,int *error);
