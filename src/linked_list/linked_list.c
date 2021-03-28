/*
Contains all functions for lists/nodes in a singly linked list.
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "string.h"
#include "../utils.h"
#include "linked_list.h"

/* 
Returns pointer to new list. 
*/
linked_list *create_list() {
    linked_list *new_list = malloc(sizeof(linked_list));
    assert(new_list);
    new_list->next_link = NULL;
    return new_list;
}


/*
Inserts new process at the front of the list.
*/
void insert_process(linked_list **head, process *new_process) {

    struct list_node *temp = malloc(sizeof(struct list_node));
    assert(temp);
    temp->current_process = new_process;
    temp->next_link = (*head)->next_link;
    // point head to the new first node: which is temp
    (*head)->next_link = temp;
}


/*
Return the parent process of the sub-process given
Return null if no match found
*/
process *find_parent(linked_list *head, process *child) {

    // Child id will be a decimal (ie 3.15) (as a string)
    int *int_array = _string_to_int_array(child->pid);
    int child_id_part1 = int_array[0];

    struct list_node *p = head->next_link;
    while (p) {
        
        // Parent id will be an integer (ie 3) (as a string)
        if (atoi(p->current_process->pid) == child_id_part1) {
            free(int_array);
            return p->current_process;
        }
        p = p->next_link;
    }
    free(int_array);
    return NULL;
}


/*
Find the matching process (using pid) and remove its node from the list.
Also free the matching process.
*/
void linked_list_remove_node(linked_list **head, process *p_to_delete) {
    struct list_node *p = (*head)->next_link;

    if (p && strcmp(p->current_process->pid, p_to_delete->pid) == 0) {
        if (p->next_link) {
            // point to link after next.
            (*head)->next_link = p->next_link;
            free(p->current_process);
            free(p);
        } else {
            // p is the only link in the list.
            free(p->current_process);
            free(p);
            (*head)->next_link = NULL;
        }
        
    } else while (p && p->next_link) {

        struct list_node *next_node = p->next_link;

        if (strcmp(next_node->current_process->pid, p_to_delete->pid) == 0) {
            if (next_node->next_link) {
                // Point p to the link after next.
                p->next_link = next_node->next_link;
                free(next_node->current_process);
                free(next_node);
            } else {
                // Next link is the last one in the list.
                free(next_node->current_process);
                free(next_node);
                p->next_link = NULL;
            }
            return;
            
        } else {
            p = p->next_link;
        }
    }
}


/*
Frees all nodes in linked_list
- Doesn't free the processes.
*/
void free_linked_list(linked_list *head) {
    struct list_node *p = head->next_link;
    while (p) {
        struct list_node *temp = p;
        p = p->next_link;
        free(temp);
    }
    free(head);
}


/*
Just for debugging.
*/
void print_list(linked_list *head) {
    struct list_node *p = head->next_link;
    int i = 0;
    while (p) {
        printf("%d:", i);
        print_process(p->current_process);
        p = p->next_link;
        i++;
    }    
}
