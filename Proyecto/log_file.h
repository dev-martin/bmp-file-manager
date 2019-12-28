FILE* init_log (struct confArguments configargs, int* error);
void log_entry(FILE* stream, int logon, char* fmt, ...);
char fmt1 [] = "[%s]  %s  %s line %s: %s\n";
char* LastcharDel(char* name);
