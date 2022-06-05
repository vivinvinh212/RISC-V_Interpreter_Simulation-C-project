#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

struct linkedlist
{
    struct linkedlist_node *first;
    // TODO: define linked list metadata
    
    int size;
	int capacity
};

struct linkedlist_node
{
    // TODO: define the linked list node
	int node_key;
	int node_value;
	struct linkedlist_node* node_next;
};

typedef struct linkedlist_node linkedlist_node_t;

linkedlist_t *ll_init()
{
    // TODO: create a new linked list

    // We have done this TODO for you as an example of how to use malloc().
    // You might want to read more about malloc() from Linux Manual page.
    // Usually free() should be used together with malloc(). For example, 
    // the linkedlist you are currently implementing usually have free() in the
    // ll_delete() function. Since we are not asking you to implement
    // the ll_delete() function, you will not be using free() in this case.

    // First, you use C's sizeof function to determine
    // the size of the linkedlist_t data type in bytes.
    // Then, you use malloc to set aside that amount of space in memory.
    // malloc returns a void pointer to the memory you just allocated, but
    // we can assign this pointer to the specific type we know we created
    linkedlist_t *list = malloc(sizeof(linkedlist_t));

    // TODO: set metadata for your new list and return the new list
    
    list->size = 0;
    list->capacity = 0;
    return list;
}

void ll_add(linkedlist_t *list, int key, int value)
{
    // TODO: create a new node and add to the front of the linked list if a
    // node with the key does not already exist.
    // Otherwise, replace the existing value with the new value.
    if (list->size == 0) {
	list->first = (linkedlist_node_t *)malloc(sizeof(linkedlist_node_t));
        list->first->node_key = key;
        list->first->node_value = value;
        list->size ++;

    }

	else {

	int i = 0;
	int check_key;
	linkedlist_node_t *next_node_check = list->first;
	while ((i == 0) && (next_node_check != NULL)) {
		check_key = next_node_check->node_key;
		if (check_key == key) {
	    		i = 1;
		}

		else {
	    		next_node_check = next_node_check->node_next;
		}
	}

		if (i == 1) {
			next_node_check->node_value = value;
		}

		else {
			linkedlist_node_t *temp = list->first;
			list->first = (linkedlist_node_t *)malloc(sizeof(linkedlist_node_t));
			list->first->node_key = key;
			list->first->node_value = value;
			list->first->node_next = temp;
			list->size ++;
		}
	
	}
}

int ll_get(linkedlist_t *list, int key)
{
    // TODO: go through each node in the linked list and return the value of
    // the node with a matching key.
    // If it does not exist, return 0.
    	
    	int i = 0;
        int check_key;
		linkedlist_node_t *next_node_check = list->first;
        while ((i == 0) && (next_node_check != NULL))	{
                check_key = next_node_check->node_key;
		if (check_key == key) {
                        i = 1;
                }

                else {
                        next_node_check = next_node_check->node_next;
                }
        }

		if (i == 1) {
			return next_node_check->node_value;
		}
	
		else {
			return 0;
		}
}

int ll_size(linkedlist_t *list)
{
    // TODO: return the number of nodes in this linked list
    
    	return list->size;
}
