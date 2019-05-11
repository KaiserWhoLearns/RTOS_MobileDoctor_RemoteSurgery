/*
 * task.h
 *
 *  Created on: May 9, 2019
 *      Author: xinyu
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tcb.h"



// struct node {
//    TCB data;

//    struct node *next;
//    struct node *prev;
// };



bool isEmpty() {
   return front == NULL;
}

void insertLast(TCB* node) {

   //create a link
   //TCB* link = (TCB*) malloc(sizeof(TCB));

   //link->data = data;

   if(isEmpty()) {
      //make it the last link
		front = node;
      back = node;
   } else {
      //make link a new last link
      back->next = node;

      //mark old last node as prev of new link
      node->prev = back;
		back = node;
   }

   return;
}

//delete given item

TCB* deleteNode(TCB* node) {
	//start from the first link
	   TCB* current = front;
	   TCB* previous = NULL;

	   //if list is empty
	   if(front == NULL) {
	      return NULL;
	   }

	   //navigate through list
	   while(current != node) {
	      //if it is last node

	      if(current->next == NULL) {
	         return NULL;
	      } else {
	         //store reference to current link
	         previous = current;

	         //move to next link
	         current = current->next;
	      }
	   }

	   //found a match, update the link
	   if(current == front) {
	      //change first to point to next link
	      front = front->next;
	   } else {
	      //bypass the current link
	      current->prev->next = current->next;
	   }

	   if(current == back) {
	      //change last to point to prev link
	      back = current->prev;
	   } else {
	      current->next->prev = current->prev;
	   }

	   return current;
}

