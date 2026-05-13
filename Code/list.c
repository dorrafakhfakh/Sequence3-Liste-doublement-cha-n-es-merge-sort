/*-----------------------------------------------------------------*/
/*
 Licence Informatique - Structures de données
 Mathias Paulin (Mathias.Paulin@irit.fr)
 
 Implantation du TAD List vu en cours.
 */
/*-----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"

typedef struct s_LinkedElement {
	int value;
	struct s_LinkedElement* previous;
	struct s_LinkedElement* next;
} LinkedElement;

/* Use of a sentinel for implementing the list :
 The sentinel is a LinkedElement* whose next pointer refer always to the head of the list and previous pointer to the tail of the list
 */
struct s_List {
	LinkedElement* sentinel;
	int size;
};

typedef struct SubList{
	LinkedElement  *head;
	LinkedElement  *tail;
}SubList;

/*-----------------------------------------------------------------*/

List* list_create(void) {
	List* l = malloc(sizeof(LinkedElement)+sizeof(List));
	l->sentinel=(LinkedElement*)(l+1);
	l->sentinel->next=l->sentinel;
	l->sentinel->previous=l->sentinel;
	l->size=0;
	return l;
}

/*-----------------------------------------------------------------*/

List* list_push_back(List* l, int v) {
	LinkedElement *new=malloc(sizeof(LinkedElement));
	LinkedElement *last=l->sentinel->previous;
	new->value=v;
	last->next=new;
	new->previous=last;
	new->next=l->sentinel;
	l->sentinel->previous=new;
	l->size++;
	return l;
}

/*-----------------------------------------------------------------*/

void list_delete(ptrList* l) {
	while((*l)->sentinel->next!=(*l)->sentinel){
		LinkedElement* to_free=(*l)->sentinel->next;
		(*l)->sentinel->next=(*l)->sentinel->next->next;
		free(to_free);
	}
	free(*l);
	*l=NULL;
}

/*-----------------------------------------------------------------*/

List* list_push_front(List* l, int v) {
	LinkedElement *new=malloc(sizeof(LinkedElement));
	LinkedElement *first=l->sentinel->next;
	new->value=v;
	new->previous=l->sentinel;
	new->next=first;
	first->previous=new;
	l->sentinel->next=new;
	l->size++;
	return l;
}

/*-----------------------------------------------------------------*/

int list_front(const List* l) {
	return l->sentinel->next->value;
}

/*-----------------------------------------------------------------*/

int list_back(const List* l) {
	return l->sentinel->previous->value;
}

/*-----------------------------------------------------------------*/

List* list_pop_front(List* l) {
	LinkedElement *to_free=l->sentinel->next;
	LinkedElement *new_head=to_free->next;
	l->sentinel->next=new_head;
	new_head->previous=l->sentinel;
	free(to_free);
	l->size--;
	return l;
}

/*-----------------------------------------------------------------*/

List* list_pop_back(List* l){
	LinkedElement *to_free=l->sentinel->previous;
	LinkedElement *new_tail=to_free->previous;
	l->sentinel->previous=new_tail;
	new_tail->next=l->sentinel;
	free(to_free);
	l->size--;
	return l;
}

/*-----------------------------------------------------------------*/

List* list_insert_at(List* l, int p, int v) {
	LinkedElement *old=l->sentinel->next;
	LinkedElement *new=malloc(sizeof(LinkedElement));
	for(int i=0;i<p;i++){
		old=old->next;
	}
	new->value = v;
    new->next = old;
    new->previous = old->previous;
    old->previous->next = new;
    old->previous = new;
	l->size++;
	return l;
}



/*-----------------------------------------------------------------*/

List* list_remove_at(List* l, int p) {
	LinkedElement *to_remove=l->sentinel->next;
	for(int i=0;i<p;i++){
		to_remove=to_remove->next;
	}
	to_remove->previous->next=to_remove->next;
	to_remove->next->previous=to_remove->previous;
	free(to_remove);
	l->size--;
	return l;
}

/*-----------------------------------------------------------------*/

int list_at(const List* l, int p) {
	LinkedElement *current=l->sentinel->next;
	for(int i=0;i<p;i++){
		current=current->next;
	}
	return current->value;
}

/*-----------------------------------------------------------------*/

bool list_is_empty(const List* l) {
	
	return (l->sentinel->next)==(l->sentinel);
}

/*-----------------------------------------------------------------*/

int list_size(const List* l) {
	return l->size;
}

/*-----------------------------------------------------------------*/

List* list_map(List* l, ListFunctor f, void* environment) {
	LinkedElement *current=l->sentinel->next;
	while(current!=l->sentinel){
		current->value=f(current->value,environment);
		current=current->next;
	}
	return l;
}

/*-----------------------------------------------------------------*/

List* list_sort(List* l, OrderFunctor f) {
	(void)f;
	return l;
}

SubList list_split(SubList l){
	LinkedElement *slow=l.head;
	LinkedElement *fast=l.head;

	while(fast->next!=NULL && fast->next->next!=NULL ){
		fast=fast->next->next;
		slow=slow->next;
	}

	LinkedElement *right=slow->next;
	slow->next->previous=NULL;
	slow->next=NULL;
	l.head=slow;
	l.tail=right; //on l'utilise lui parce que fast est uniquement fais pour le parcours 
	return l;
}