#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "rooms.h"
#include "items.h"

typedef struct avatar Avatar;

struct avatar {
	Room * current_room;
	Item * inventory;
};

Avatar * avatar(Room * current_room, Item * inventory) {
	Avatar * new_avatar = (Avatar *) malloc(sizeof(Avatar));
	
	new_avatar->current_room = current_room;
	new_avatar->inventory = inventory;

	return new_avatar;
}

Room * avatar_current_room(Avatar * avatar) {
	return avatar->current_room;
}

void avatar_current_room_set(Avatar * avatar, Room * current_room) {
	avatar->current_room = current_room;
}

Item * avatar_inventory(Avatar * avatar) {
	return avatar->inventory;
}

void avatar_print_inventory(Avatar * avatar) {
	
	Item * current = avatar_inventory(avatar);
	printf("Inventory:");
	while (current != NULL) {
		if (strcmp(item_name(current), "DUMMY") != 0) {
			printf(" %s", item_name(current));

			if (item_next(current) != NULL) {
				printf(",");
			}
		}

		current = item_next(current);
	}

	printf("\n");
}

void flush(void) {
	int c;
    do {
    	c = getchar();
    } while (c != '\n' && c != EOF);
}

char * get_command_input(_Bool first_run) {

	char * command_input = malloc(256);
	printf("What's next?\n");

	if (first_run) {
		flush();
	}

	fgets(command_input, 256, stdin);
	return command_input;
}

_Bool interpret_command_input(Avatar * player_avatar, char * command_input) {
	
	//Remove trailing newline character
	char *newline = strchr(command_input, '\n');
	if ( newline )
  		*newline = 0;

	char * command_token = strtok(command_input, " ,.-");
	char * parsed_commands[2] = {"EMPTY", "EMPTY"};
	
	int index = 0;
	while (command_token != NULL) {

		if (index >= 2) {
			printf("Invalid action.\n");
			return 0;
		}

		parsed_commands[index] = command_token;
		command_token = strtok(NULL, " ,.-");
		index++;
	}

	if (strcasecmp(parsed_commands[0], "look") == 0) {
		
		Room * current_room = avatar_current_room(player_avatar);
		printf("You take a look around.\n");
		printf("You are standing in a %s\n", room_description(current_room));
		printf("%s\n", room_look_message(current_room));
		room_print_items(current_room);
		print_adjacent_rooms(current_room);

	} else if (strcasecmp(parsed_commands[0], "take") == 0) {

		char * name = parsed_commands[1];
		if (strcmp(name, "EMPTY") == 0) {
			printf("Invalid action.\n");
			return 0;
		}

		Room * current_room = avatar_current_room(player_avatar);
		Item * found_item = item_take(room_items(current_room), name);

		if (found_item != NULL) {
			printf("You take the %s.\n", item_name(found_item));
			item_add_one(avatar_inventory(player_avatar), found_item);
		} else {
			printf("There is no %s to take!\n", name);
		}

	} else if (strcasecmp(parsed_commands[0], "go") == 0) {

		char * exit_direction = parsed_commands[1];
		if (strcmp(exit_direction, "EMPTY") == 0) {
			printf("Invalid direction.\n");
			return 0;
		}

		Room * current_room = avatar_current_room(player_avatar);
		if (!(room_is_valid_direction(current_room, exit_direction))) {
			printf("Invalid direction.\n");
			return 0;
		}
		
		Room * temp = room_exit(current_room, exit_direction);
		if (temp == NULL || !room_active(temp)) {
			printf("I can't go that way.\n");
			return 0;
		} else if (room_locked(temp)) {
			printf("%s\n", room_locked_message(temp));
			return 0;
		}

		current_room = temp; 
		avatar_current_room_set(player_avatar, current_room);
		enter_room(current_room);

		if (strcmp(room_description(current_room), "Treasure room") == 0) {
			printf("CONGRATULATIONS YOU FOUND THE TREASURE!\n");
			printf("GAME OVER, THANKS FOR PLAYING\n");
			return 1;
		}

		room_print_items(current_room);
		print_adjacent_rooms(current_room);

	} else if (strcasecmp(parsed_commands[0], "inventory") == 0) {
		avatar_print_inventory(player_avatar);
	} else if (strcasecmp(parsed_commands[0], "drop") == 0) {

		char * name = parsed_commands[1];
		if (strcmp(name, "EMPTY") == 0 || strcasecmp(name, "DUMMY") == 0) {
			printf("Invalid action.");
			return 0;
		}

		Item * inventory_item = item_take(avatar_inventory(player_avatar), name);

		if (inventory_item == NULL) {
			printf("I don't have that item!\n");
			return 0;
		}

		Room * current_room = avatar_current_room(player_avatar);

		char * test = item_name(inventory_item);
		printf("You drop the %s\n", test);
		item_add_one(room_items(current_room), inventory_item);

	} else if (strcasecmp(parsed_commands[0], "use") == 0) {
		
		char * name = parsed_commands[1];
		if (strcmp(name, "EMPTY") == 0 || strcasecmp(name, "DUMMY") == 0) {
			printf("Invalid action");
			return 0;
		}

		Item * inventory_item = item_find_one(avatar_inventory(player_avatar), name);

		if (inventory_item == NULL) {
			printf("I don't have that item!\n");
			return 0;
		}

		room_use_item(avatar_current_room(player_avatar), inventory_item);

	} else {
		printf("Invalid action.\n");
		return 0;
	}

	return 0;

}

void new_game(Room * starting_room, Item * starting_items) {

	Room * current_room = starting_room;

	printf("Welcome to: ancient dungeon adventure\n");
	printf("Commands: look, go 'direction', use 'item name', drop 'item name', take 'item name'\n");
	printf("Start your adventure? y/n\n");

	char user_input_letter;
	scanf(" %c", &user_input_letter);

	if (tolower(user_input_letter) == 'y') {
		
		Avatar * player_avatar = avatar(current_room, starting_items);

		//Give room description
		//Give list of items in room
		//Ask for user input
		//Interpret input
		//Execute command, or explain why we cannot

		printf("You are standing in a %s\n", room_description(current_room));
		room_print_items(current_room);
		print_adjacent_rooms(current_room);

		_Bool is_running = 1;
		_Bool first_run = 1;
		while (is_running) {
			
			char * command_input;
			if (first_run) {
				command_input = get_command_input(1);
				first_run = 0;
			} else {
				command_input = get_command_input(0);
			}
			
			_Bool winner = interpret_command_input(player_avatar, command_input);

			if (winner) {
				exit(0);
			}

			free(command_input);
			command_input = NULL;
		}

	} else {
		exit(0);
	}
	
}

int main() {

	Item * inventory = item("DUMMY", NULL, NULL, 0);

	Item * shovel = item("Shovel", "rusty old", NULL, 0);
	Item * forest_items = item("DUMMY", NULL, shovel, 0);
	Room * forest = room("dark forest", forest_items, NULL, NULL, NULL, NULL, NULL, NULL, NULL, "Below your feet is a soft soil.", 1, 0, NULL);

	Item * pit_items = item("DUMMY", NULL, NULL, 0);
	Item * tunnel_items = item("DUMMY", NULL, NULL, 0);
	
	Room * tunnel = room("dark tunnel", tunnel_items, NULL, NULL, NULL, NULL, NULL, NULL, "A light shimmers at the end of the tunnel", "You can't see much, except for a glowing light at the end of the tunnel", 1, 0, NULL);
	Room * pit = room("dungeon entrance", pit_items, tunnel, NULL, NULL, NULL, NULL, NULL, "An ominious feeling washes over you", "You're in a room surrounded by yellow, seemingly ancient stone bricks", 0, 0, NULL);

	room_down_set(forest, pit);

	Item * chamber_items = item("DUMMY", NULL, NULL, 0);
	Item * north_east_items = item("DUMMY", NULL, NULL, 0);
	Item * north_items = item("DUMMY", NULL, NULL, 0);

	Room * north_east_passage = room("dark passageway", north_east_items, NULL, NULL, NULL, NULL, NULL, NULL, "You feel lost", NULL, 1, 0, NULL);
	Room * northern_passage = room("dark passageway", north_items, NULL, NULL, north_east_passage, NULL, NULL, NULL, "The sound of your footsteps echos around you.", NULL, 1, 0, NULL);

	Room * chamber = room("ominious chamber", chamber_items, northern_passage, NULL, NULL, NULL, NULL, NULL, "A small torch illuminates the corner of the room.", "You see passages spanning around you, one of which is guarded by a heavy iron door to the east.", 1, 0, NULL);

	Item * blocked_passage_items = item("DUMMY", NULL, NULL, 0);
	Room * door_blocked_passage = room("Treasure room", blocked_passage_items, NULL, NULL, NULL, NULL, NULL, NULL, "Inside the room lies thousands of gold coins and other riches!", NULL, 1, 1, "The iron door is locked.");

	Item * west_passage_items = item("DUMMY", NULL, NULL, 0);
	Room * west_passage = room("dark passageway", west_passage_items, NULL, NULL, NULL, NULL, NULL, NULL, "The pathway appears to lead somewhere.", NULL, 1, 0, NULL);
	
	Item * dusty_key = item("key", "dusty", NULL, 0);
	Item * west_north_passage_items = item("DUMMY", NULL, dusty_key, 0);
	Room * west_north_passage = room("dark passageway", west_north_passage_items, NULL, NULL, NULL, NULL, NULL, NULL, "The pathway appears to lead somewhere.", NULL, 1, 0, NULL);

	Item * west_west_passage_items = item("DUMMY", NULL, NULL, 0);
	Room * west_west_passage = room("dark passageway", west_passage_items, NULL, NULL, NULL, NULL, NULL, NULL, "You feel lost", "You're surrounded by nothing but stone walls.", 1, 0, NULL);

	room_west_set(chamber, west_passage);
	room_north_set(west_passage, west_north_passage);
	room_west_set(west_passage, west_west_passage);
		
	room_east_set(chamber, door_blocked_passage);
	room_north_set(tunnel, chamber);

	new_game(forest, inventory);

	return 0;
}