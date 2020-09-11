#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rooms.h"

Room * room(char * description, Item * items, Room * north, Room * south, 
				   Room * east, Room * west, Room * up, Room * down, char * entry_message, char * look_message, _Bool active, _Bool locked, char * locked_message) 
{

		Room * newRoom = (Room *) malloc(sizeof(Room));

		newRoom->description = description;
		newRoom->items = items;
		newRoom->north = north;
		newRoom->south = south;
		newRoom->east = east;
		newRoom->west = west;
		newRoom->up = up;
		newRoom->down = down;
		newRoom->entry_message = entry_message;
		newRoom->look_message = look_message;
		newRoom->active = active;
		newRoom->locked = locked;
		newRoom->locked_message = locked_message;

		return newRoom;

}

char * room_description(Room * room) {
	return room->description;
}

char * room_entry_message(Room * room) {
	return room->entry_message;
}

Room * room_north(Room * room) {
	return room->north;
}

void room_north_set(Room * room, Room * north) {
	room->north = north;
}

Room * room_south(Room * room) {
	return room->south;
}

void room_south_set(Room * room, Room * south) {
	room->south = south;
}

Room * room_east(Room * room) {
	return room->east;
}

void room_east_set(Room * room, Room * east) {
	room->east = east;
}

Room * room_west(Room * room) {
	return room->west;
}

void room_west_set(Room * room, Room * west) {
	room->west = west;
}

Room * room_up(Room * room) {
	return room->up;
}

void room_up_set(Room * room, Room * up) {
	room->up = up;
}

Room * room_down(Room * room) {
	return room->down;
}

void room_down_set(Room * room, Room * down) {
	room->down = down;
}

Item * room_items(Room * room) {
	return room->items;
}

void room_items_set(Room * room, Item * items) {
	room->items = items;
}

char * room_look_message(Room * room) {
	return room->look_message;
}

_Bool room_active(Room * room) {
	return room->active;
}

void room_active_set(Room * room, _Bool active) {
	room->active = active;
}

_Bool room_locked(Room * room) {
	return room->locked;
}

void room_locked_set(Room * room, _Bool locked) {
	room->locked = locked;
}

char * room_locked_message(Room * room) {
	return room->locked_message;
}

Room * room_exit(Room * current_room, char * direction) {
	
	if (strcasecmp(direction, "North") == 0) {
		Room * north_room = room_north(current_room);
		if (north_room != NULL && !room_locked(north_room) && room_active(north_room)) {
			room_south_set(north_room, current_room);
		}
		return north_room;
	} else if (strcasecmp(direction, "South") == 0) {
		Room * south_room = room_south(current_room);
		if (south_room != NULL && !room_locked(south_room) && room_active(south_room)) {
			room_north_set(south_room, current_room);
		}
		return south_room;
	} else if (strcasecmp(direction, "East") == 0) {
		Room * east_room = room_east(current_room);
		if (east_room != NULL && !room_locked(east_room) && room_active(east_room)) {
			room_west_set(east_room, current_room);
		}
		return east_room;
	} else if (strcasecmp(direction, "West") == 0) {
		Room * west_room = room_west(current_room);
		if (west_room != NULL && !room_locked(west_room) && room_active(west_room)) {
			room_east_set(west_room, current_room);
		}
		return west_room;
	} else if (strcasecmp(direction, "Up") == 0) {
		Room * up_room = room_up(current_room);
		if (up_room != NULL && !room_locked(up_room) && room_active(up_room)) {
			room_down_set(up_room, current_room);
		}
		return up_room;
	} else if (strcasecmp(direction, "Down") == 0) {
		Room * down_room = room_down(current_room);
		if (down_room != NULL && !room_locked(down_room) && room_active(down_room)) {
			room_up_set(down_room, current_room);
		}
		return down_room;
	}

	return NULL;
}

void enter_room(Room * room) {
	printf("You step into the %s\n", room_description(room));
	char * entry_message = room_entry_message(room);
	if (entry_message != NULL) {
		printf("%s\n", entry_message);
	}
}

void room_print_items(Room * room) {

	Item * current = room->items;
	while (current != NULL) {
		if (strcmp(item_name(current), "DUMMY") != 0) {
			if (item_multiple(current)) {
				printf("There are some %s %s, here\n", item_description(current), item_name(current));
			} else {
				printf("There is a %s %s, here\n", item_description(current), item_name(current));
			}
		}

		current = item_next(current);
	}
	
}

_Bool room_is_valid_direction(Room * room, char * direction) {
	if (strcasecmp(direction, "North") == 0) {
		return 1;
	} else if (strcasecmp(direction, "South") == 0) {
		return 1;
	} else if (strcasecmp(direction, "East") == 0) {
		return 1;
	} else if (strcasecmp(direction, "West") == 0) {
		return 1;
	} else if (strcasecmp(direction, "Up") == 0) {
		return 1;
	} else if (strcasecmp(direction, "Down") == 0) {
		return 1;
	} else {
		return 0;
	}
}

void print_adjacent_rooms(Room * room) {
	
	Room * north = room_north(room);
	Room * south = room_south(room);
	Room * east = room_east(room);
	Room * west = room_west(room);
	Room * up = room_up(room);
	Room * down = room_down(room);

	Room * rooms[6] = {north, south, east, west, up, down};

	for (int i = 0; i < 6; i++) {
		Room * current = rooms[i];
		if (current != NULL && room_active(current)) {
			printf("There is a %s ", room_description(current));
			if (i <= 3) {
				printf("in the ");
			}
				
			switch (i) {
				case 0:
				printf("north");
					break;
				case 1:
					printf("south");
					break;
				case 2:
					printf("east");
					break;
				case 3:
					printf("west");
					break;
				case 4:
					printf("above");
					break;
				case 5:
					printf("below");
					break;
				};

			printf("\n");

		}
	}

}

void room_use_item(Room * current_room, Item * item) {
	printf("You use the %s\n", item_name(item));

	if (strcmp(item_name(item), "Shovel") == 0) {

		if (strcmp(room_description(current_room), "dark forest") == 0) {
			printf("You use the shovel to dig a hole. The soil collpases inward revealing a large pit before you leading to a dungeon.\n");
			
			room_active_set(room_down(current_room), 1);
		}

	} else if (strcmp(item_name(item), "key") == 0) {

		if (strcmp(room_description(current_room), "ominious chamber") == 0) {
			printf("You unlock the iron door with the key.\n");

			room_locked_set(room_east(current_room), 0);
		}

	}

}

/*
void room_exit_north(struct Room * current, struct Room * other) {
	current->north = other;
}*/



