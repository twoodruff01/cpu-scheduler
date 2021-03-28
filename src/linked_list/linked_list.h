#ifndef LINKED_LIST
#define LINKED_LIST

#include "../utils.h"

struct list_node {
    process *current_process;
    struct list_node *next_link;
};

typedef struct {
    struct list_node *next_link;
} linked_list;


linked_list *create_list();
void insert_process(linked_list **head, process *new_process);
process *find_parent(linked_list *head, process *child);
void linked_list_remove_node(linked_list **head, process *p_to_delete);
void free_linked_list(linked_list *head);
void print_list(linked_list *head);

#endif
