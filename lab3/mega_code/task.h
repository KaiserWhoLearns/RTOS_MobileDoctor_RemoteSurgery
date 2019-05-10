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

struct node {
   TCB data;

   struct node *next;
   struct node *prev;
};

//this link always point to first Link
struct node *front = NULL;

//this link always point to last Link
struct node *back = NULL;

struct node *current = NULL;

bool isEmpty() {
   return front == NULL;
}

void insertLast(int key, int data) {

   //create a link
   struct node *link = (struct node*) malloc(sizeof(struct node));

   link->data = data;

   if(isEmpty()) {
      //make it the last link
      back = link;
   } else {
      //make link a new last link
      back->next = link;

      //mark old last node as prev of new link
      link->prev = back;
   }

   //point last to new last node
   back = link;
}

//delete given item

struct node* deleteT(TCB data) {
	//start from the first link
	   struct node* current = front;
	   struct node* previous = NULL;

	   //if list is empty
	   if(front == NULL) {
	      return NULL;
	   }

	   //navigate through list
	   while(current->data != data) {
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

