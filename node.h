    #include "menu.h"

	struct node* add_node(struct collection *my_collection, int pos,struct node *nw);

	struct node *create_node(char* operation ,char*parameter,char* parameter2,int id,POINT p1,POINT p2,int *error);
	
	struct collection delete_node( struct collection my_collection, int id);
	
	struct collection redo_node(struct collection my_collection, int id);
		
	struct collection clean_node_pos(struct collection my_collection, int id);
	void clean_node(struct node	**n);
	void free_next_nodes(struct node* nodo);
	
