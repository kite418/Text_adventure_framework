#ifndef ROOMS_H
#define ROOMS_H

#include "items.h"

typedef struct room Room;

//Datastructure representing a room in the game
/*
Item * item: linked list of items,
Room * all directions: Rooms that you can navigate to from this room,
char * entry_message: Message printed when you enter a room for atmospheric purposes
char * look_message: Message printed when using the look command, to give the player hints about where to go
_Bool active: Whether the room is hidden or not, used when a room must be revealed via the usage of an item
_Bool locked: Whether a room is accessible or not, used when a room should be visible but not accessible until an item is used,
char * locked_message: Message printed when the player attempts to open a locked room
*/
struct room {
	char * description;
	Item * items;
	Room * north;
	Room * south;
	Room * east;
	Room * west;
	Room * up;
	Room * down;
	char * entry_message;
	char * look_message;
	_Bool active;
	_Bool locked;
	char * locked_message;
};

//Function that creates a new Room * with the given parameters and returns it
Room * room(char * description, Item * items, 
					Room * north, Room * south, 
					Room * east, Room * west, 
					Room * up, Room * down, 
					char * entry_message, char * look_message,
					_Bool active, _Bool locked, char * locked_message);

//Getter for room->description
char * room_description(Room * room);
//Getter for room->entry_message
char * room_entry_message(Room * room);
//Getter for room->north
Room * room_north(Room * room);
//Setter for room->north
void room_north_set(Room * room, Room * north);
//Getter for room->south
Room * room_south(Room * room);
//Setter for room->south
void room_south_set(Room * room, Room * south);
//Getter for room->east
Room * room_east(Room * room);
//Setter for room->east
void room_east_set(Room * room, Room * east);
//Getter for room->west
Room * room_west(Room * room);
//Setter for room->west
void room_west_set(Room * room, Room * west);
//Getter for room->up
Room * room_up(Room * room);
//Setter for room->up
void room_up_set(Room * room, Room * up);
//Getter for room->down
Room * room_down(Room * room);
//Setter for room->down
void room_down_set(Room * room, Room * down);
//Getter for room->active
_Bool room_active(Room * room);
//Setter for room->active
void room_active_set(Room * room, _Bool active);
//Getter for room->locked
_Bool room_locked(Room * room);
//Setter for room->locked
void room_locked_set(Room * room, _Bool locked);
//Getter for room->locked_message
char * room_locked_message(Room * room);
//Getter for room->look_message
char * room_look_message(Room * room);

//Getter for room->items
Item * room_items(Room * room);
//Setter for room->items
void room_items_set(Room * room, Item * items);
//Prints a list of all of the items in the given Room *, and formats the text output
void room_print_items(Room * room);
//Used to exit a current room in a specific direction, links the room that is being entered with the room that is being left
Room * room_exit(Room * current_room, char * direction);
//Prints a formatted list of all of the rooms adjacent to a given room
void print_adjacent_rooms(Room * room);
//Determines if a room direction is valid, that is north, south, east, west, up, down
_Bool room_is_valid_direction(Room * room, char * direction);
//Used to handle entering a given room and printing some text about performing this action
void enter_room(Room * room);
//Lookup Item * and based on what it is perform a certain action on the given room
void room_use_item(Room * current_room, Item * item);

#endif