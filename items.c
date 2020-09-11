#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "items.h"
#include "rooms.h"

#include <stdio.h>

Item * item(char* name, char* description, Item * next, _Bool multiple) {
	
	Item * newItem = (Item *) malloc(sizeof(Item));

	newItem->name = name;
	newItem->description = description;
	newItem->next = next;
	newItem->multiple = multiple;

	return newItem;
}

char * item_name(Item * item) {
	return item->name;
}

char * item_description(Item * item) {
	return item->description;
}

Item * item_next(Item * item) {
	return item->next;
}

void item_next_set(Item * item, Item * next) {
	item->next = next;
}

_Bool item_multiple(Item * item) {
	return item->multiple;
}

Item * item_take(Item * headItem, char * name) {

	Item * currentItem = headItem, *previous;

	while (currentItem != NULL) {

		if (strcasecmp(item_name(currentItem), name) == 0) {

			item_next_set(previous, item_next(currentItem));
			item_next_set(currentItem, NULL);
			return currentItem;
		}

		previous = currentItem;
		currentItem = item_next(currentItem);
	}

	return NULL;

}

Item * item_find_one(Item * headItem, char * name) {
	Item * currentItem = headItem, *previous;

	while (currentItem != NULL) {

		if (strcasecmp(item_name(currentItem), name) == 0) {
			return currentItem;
		}

		previous = currentItem;
		currentItem = item_next(currentItem);
	}

	return NULL;
}

void item_add_one(Item * list, Item * item) {
	Item * current = list;
	Item * previous = NULL;
	while (current != NULL) {
		previous = current;
		current = item_next(current);
	}
	item_next_set(previous, item);
}