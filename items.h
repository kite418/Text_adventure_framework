#ifndef ITEMS_H
#define ITEMS_H


typedef struct item Item;

/*Data structure representing an item
char* name: name of the item,
char* description: item prefix/description,
Item * next: the next item in the linked list,
_Bool multiple: Whether the item is plural or not, for printing readability
*/
struct item {
	char* name;
	char* description;
	Item * next;
	_Bool multiple;
};

//Function that creates a new item from the given parameters and returns it.
Item * item(char* name, char* description, Item * next, _Bool multiple);
//Getter for Item->name
char * item_name(Item * item);
//Getter for Item->description
char * item_description(Item * item);
//Getter for Item->next
Item * item_next(Item * item);
//Removes the item with name = char * name, from the linked list and returns it
Item * item_take(Item * headItem, char * name);
//Getter for Item->multiple
_Bool item_multiple(Item * item);
//Setter for Item->next
void item_next_set(Item * item, Item * next);
//Adds an item to a linked list
void item_add_one(Item * list, Item * item);
//Returns and item from the linked list but does not remove it.
Item * item_find_one(Item * headItem, char * name);

#endif