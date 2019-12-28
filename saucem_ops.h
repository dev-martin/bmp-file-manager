
#include  "menu.h"
const char *get_error_msg_saucemops(int error);

struct collection add_op(struct collection my_collection ,int id,char *operation,char *parameter,char *parameter2,POINT p1, POINT p2, int *error);
struct collection undo_op(struct collection my_collection ,int id,int *error);
struct collection redo_op(struct collection my_collection ,int id,int *error);
struct collection clean_op(struct collection my_collection ,int id,int *error);
struct collection save_current_id(struct collection my_collection ,int id,int *error,int *savecurrentiddone);
void print_ops(struct collection my_collection,int id,int *error);
int find_item_id_collection(struct collection my_collection,int id,int *position);		
struct collection add_onlyop( struct collection my_collection, int id , char *operation,char * parameter,char * parameter2, POINT p1, POINT p2 , int *error ) ;
	
