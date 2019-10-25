// Emma Akbari (eea21) 

#include <stdlib.h>
#include <stdio.h>

// Node struct
typedef struct Node {
	int value;
	struct Node* next;
} Node;

// create a Node, set its value, and return it
Node* create_node(int value) {
	Node* newNode = malloc(sizeof(Node));
	newNode->value = value;
	newNode->next = NULL;

	return newNode;
}

// print values of all items in list
void list_print(Node* head){

	// iterate through LL and print node values
	for(Node* n = head; n != NULL; n = n->next){
		printf("%d", n->value);
		// only print arrow if not last node
		if(n->next != NULL) {
			printf(" -> ");
		}
	}

	printf("\n");
}

// add node to end of LL and return it
Node* list_append(Node* head, int value) {

	Node* newNode;

	// iterate until last node in list
	for(Node* n = head; n != NULL; n = n->next) {

		// if at the last node
		if(n->next == NULL) {
			// append new node at end
			newNode = create_node(value);
			n->next = newNode;
			break;
		}
	}

	return newNode;
}

// create new head node and return it
Node* list_prepend(Node* head, int value) {

	Node* newNode = create_node(value);
	newNode->next = head;
	return newNode;
}

// free all nodes if head isn't NULL
void list_free(Node* head) {

	if(head != NULL) {

		Node* prev = head;

		for(Node* n = head; n != NULL; n = n->next) {
			
			if(n != head) {
				// free the previous node (1 before n)
				free(prev);
				prev = n;
			}
		}

		// free the last node
		free(prev);
	}
}

// remove node from list and free it, return the head of the list
Node* list_remove(Node* head, int value) {

	Node* newHead; // new head to return

	if(head->value == value) {
		// special case 1: remove head
		newHead = head->next;
		free(head);
	} else {
		
		newHead = head;
		
		// iterate until value is found in the next node
		for(Node* curr = head; curr != NULL; curr = curr->next) {

			// if the value is found
			if(curr->next != NULL && curr->next->value == value) {

				if(curr->next->next == NULL) {
					// special case 2: remove last node
					Node* last = curr->next;
					curr->next = NULL;
					free(last);
					break;
				} else {
					// regular case: remove node from LL and free
					Node* remove = curr->next;
					curr->next = curr->next->next;
					free(remove);
					break;
				}
			}
		}
	}

	return newHead;
} 

// driver
int main() {
	// The comments at the ends of the lines show what list_print should output.
	Node* head = create_node(1);
	list_print(head);                  // 1
	Node* end = list_append(head, 2);
	list_print(head);                  // 1 -> 2
	end->next = create_node(3);
	list_print(head);                  // 1 -> 2 -> 3
	head = list_prepend(head, 0);
	list_print(head);                  // 0 -> 1 -> 2 -> 3
	list_append(head, 4);
	list_print(head);                  // 0 -> 1 -> 2 -> 3 -> 4
	list_append(head, 5);
	list_print(head);                  // 0 -> 1 -> 2 -> 3 -> 4 -> 5

	head = list_remove(head, 5);
	list_print(head);                  // 0 -> 1 -> 2 -> 3 -> 4
	head = list_remove(head, 3);
	list_print(head);                  // 0 -> 1 -> 2 -> 4
	head = list_remove(head, 0);
	list_print(head);                  // 1 -> 2 -> 4
	
	list_free(head);
	
	return 0;
}