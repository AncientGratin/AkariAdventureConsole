#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "akariadv.h"

#define DEBUG 0

// Check if there is any overlay position of units
//  Return Value:
//  Summary of all
int check_any_overlay_position(Game game, int id_myself, int max_id) {
	int i = 0;
	int flags = 0;
	Point my_pos;

	if (id_myself == ID_TRAP1 || id_myself == ID_TRAP2) {
		my_pos.x = game.traps[id_myself - 10].x;
		my_pos.y = game.traps[id_myself - 10].y;
	}
	else if (id_myself > -1 && id_myself < ID_TRAP1) {
		my_pos.x = game.units[id_myself].position.x;
		my_pos.y = game.units[id_myself].position.y;
	}
	else {
		return ERROR;	// Wrong argument!
	}
	
	if (check_on_stage(my_pos) == FALSE)
		return 0;

	for (i = 0; i <= min(max_id, 9); i++) {
		// pass myself
		if (i == id_myself)
			continue;

		if (!check_on_stage(game.units[i].position) == TRUE)
			continue;

		if (get_distance(my_pos, game.units[i].position) == 0) {
			flags += (int)pow(2, i);
		}
	}

	if (max_id >= ID_TRAP1 && id_myself != ID_TRAP1 && get_distance(my_pos, game.traps[0]) == 0)
		flags += (int)pow(2, ID_TRAP1);

	if (max_id >= ID_TRAP2 && id_myself != ID_TRAP2 && get_distance(my_pos, game.traps[1]) == 0)
		flags += (int)pow(2, ID_TRAP2);

	return flags;
}

// Check if there is any overlay position of units by position
//  Return Value:
//  Summary of all
int check_any_overlay_position_by_point(Game game, Point here) {
	int i = 0;
	int flags = 0;
	Point there;

	if (!check_on_stage(here) == TRUE) {
		return ERROR;	// Wrong argument!
	}

	for (i = 0; i <= ID_SERVANT4; i++) {
		there.x = game.units[i].position.x;
		there.y = game.units[i].position.y;

		/*if (there.x < 0 || there.x > 7 || there.y < 0 || there.y > 7) {
			continue;
		}*/
		if (!check_on_stage(there) == TRUE) {
			continue;
		}

		if (get_distance(here, there) == 0) {
			flags += (int)pow(2, i);
		}
	}

	there.x = game.traps[0].x;
	there.y = game.traps[0].y;

	if (check_on_stage(there) == TRUE && get_distance(here, there) == 0)
		flags += (int)pow(2, ID_TRAP1);

	there.x = game.traps[1].x;
	there.y = game.traps[1].y;

	if (check_on_stage(there) == TRUE && get_distance(here, there) == 0)
		flags += (int)pow(2, ID_TRAP2);

	return flags;
}

// Check if a point is on the stage.
int check_on_stage(Point pos) {
	return
		(pos.x > -1 && pos.x < FIELD_WIDTH &&
			pos.y > -1 && pos.y < FIELD_HEIGHT) ? TRUE : FALSE;
}

// If all people are dead, game over.
int check_villager_extinct(Game game) {
	int hp_sum = 0;
	int i;

	for (i = 0; i < 4; i++) {
		hp_sum += game.units[ID_PEOPLE1 + i].hp;
	}

	return hp_sum > 0 ? TRUE : hp_sum == 0 ? FALSE : ERROR;
}

// Display the game information on the screen.
void display(Game game) {
	int i, j;
#ifdef DEBUG
	char dbg_c = ' ';	// For debugging
	int dbg_overlay_flags = 0;
	Point dbg_pos;
#endif
	system("cls");

	//printf("\n");
	//printf(" AKARI'S ADVENTURE\n");
	//printf("\n");
	//printf("  0 1 2 3 4 5 6 7\n");
	//printf(" -----------------  Akari   [9/5]\n");
	//printf("0| | | | | | | | |  Vampire [9]\n");
	//printf(" -----------------\n");
	//printf("1| | | | | | | | |  Warning!\n");
	//printf(" -----------------  [ ] Vampire approach\n");
	//printf("2| | | | | | | | |  [ ] Adjacent a trap\n");
	//printf(" -----------------  [ ] Met a servant\n");
	//printf("3| | | | | | | | |  [ ] Met a villager\n");
	//printf(" -----------------\n");
	//printf("4| | | | | | | | |  Akari attacked\n");
	//printf(" -----------------  [ ] Vampire\n");
	//printf("5| | | | | | | | |  [ ] a trap\n");
	//printf(" -----------------  [ ] a servant\n");
	//printf("6| | | | | | | | |  [ ] a villager\n");
	//printf(" -----------------\n");
	//printf("7| | | | | | | | |  present by\n");
	//printf(" -----------------  AncientGratin\n");

	printf("\n");
	printf(" AKARI'S ADVENTURE\n");
	printf("\n");
	printf("  0 1 2 3 4 5 6 7\n");

	for (i = 0; i < 8; i++) {
		printf(" -----------------");

		switch (i) {
		case 0:
			printf("  Akari   [%d/%d]", game.units[ID_AKARI].hp, game.units[ID_AKARI].attack_chance);
			break;
		case 2:
			printf("  [%c] Vampire approach", game.lights_warn[LIGHT_WARN_VAMPIRE] ? '*' : ' ');
			break;
		case 3:
			printf("  [%c] Met a servant", game.lights_warn[LIGHT_WARN_SERVANT] ? '*' : ' ');
			break;
		case 5:
			printf("  [%c] Vampire", game.lights_attack[LIGHT_ATTACK_VAMPIRE] ? '*' : ' ');
			break;
		case 6:
			printf("  [%c] a servant", game.lights_attack[LIGHT_ATTACK_SERVANT] ? '*' : ' ');
			break;
		}

		printf("\n%d", i);

		for (j = 0; j < 8; j++) {
			// Akari : A
			// Vampire : V
			// People : P
			// Servant : S
			// Trap : T
			// Overlay : O

#if DEBUG
			dbg_pos.x = j;
			dbg_pos.y = i;

			dbg_overlay_flags = check_any_overlay_position_by_point(game, dbg_pos);

			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_AKARI) == 1) {
				dbg_c = 'A';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_VAMPIRE) == 1) {
				dbg_c = dbg_c == ' ' ? 'V' : 'O';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_PEOPLE1) == 1) {
				dbg_c = dbg_c == ' ' ? 'P' : 'O';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_PEOPLE2) == 1) {
				dbg_c = dbg_c == ' ' ? 'P' : 'O';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_PEOPLE3) == 1) {
				dbg_c = dbg_c == ' ' ? 'P' : 'O';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_PEOPLE4) == 1) {
				dbg_c = dbg_c == ' ' ? 'P' : 'O';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_SERVANT1) == 1) {
				dbg_c = dbg_c == ' ' ? 'S' : 'O';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_SERVANT2) == 1) {
				dbg_c = dbg_c == ' ' ? 'S' : 'O';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_SERVANT3) == 1) {
				dbg_c = dbg_c == ' ' ? 'S' : 'O';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_SERVANT4) == 1) {
				dbg_c = dbg_c == ' ' ? 'S' : 'O';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_TRAP1) == 1) {
				dbg_c = dbg_c == ' ' ? 'T' : 'O';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_TRAP2) == 1) {
				dbg_c = dbg_c == ' ' ? 'T' : 'O';
			}
			
			printf("|%c", dbg_c);

			dbg_c = ' ';
#else
			printf("|%c",
				game.units[ID_AKARI].position.x == j && game.units[ID_AKARI].position.y == i ? 'A' : ' ');
#endif
		}

		printf("|");

		switch (i) {
		case 0:
			printf("  Vampire [%d]", game.units[ID_VAMPIRE].hp);
			break;
		case 1:
			printf("  Warning!");
			break;
		case 2:
			printf("  [%c] Adjacent a trap", game.lights_warn[LIGHT_WARN_TRAP] ? '*' : ' ');
			break;
		case 3:
			printf("  [%c] Met a villager", game.lights_warn[LIGHT_WARN_VILLAGER] ? '*' : ' ');
			break;
		case 4:
			printf("  Akari attacked");
			break;
		case 5:
			printf("  [%c] a trap", game.lights_attack[LIGHT_ATTACK_TRAP] ? '*' : ' ');
			break;
		case 6:
			printf("  [%c] a villager", game.lights_attack[LIGHT_ATTACK_VILLAGER] ? '*' : ' ');
			break;
		case 7:
			printf("  present by");
			break;
		}

		printf("\n");
	}

	printf(" -----------------  AncientGratin\n\n");
}

// Show game over message and exit
void game_over(int code) {
	switch (code) {
	case CODE_GAMEOVER_AKARIWON:
		printf("Akari won!\n");
		printf("Congratulations!\n");
		exit(0);
	case CODE_GAMEOVER_DRAW:
		printf("Akari and Vampire killed each other!\n");
		printf("People will remember your sacrifice...\n");
		exit(0);
	case CODE_GAMEOVER_AKARIDEAD:
		printf("Akari lost!\n");
		printf("Game Over\n");
		exit(0);
	case CODE_GAMEOVER_PEOPLEDEAD:
		printf("All people are dead!\n");
		printf("Game Over\n");
		exit(0);
	}
}

// Get the direction from routine.
int get_direction(Point routine) {
	if (routine.x == 0) {
		if (routine.y == 0)
			return STAY;

		return routine.y > 0 ? SOUTH : NORTH;
	}
	else if (routine.y == 0) {
		return routine.x > 0 ? EAST : WEST;
	}
	else {
		if (routine.x > 0) {
			if (routine.y > 0) {
				return routine.x == routine.y ? SOUTHEAST : routine.x > routine.y ? EAST : SOUTH;
			}
			else {
				return routine.x == routine.y ? NORTHEAST : routine.x > routine.y ? EAST : NORTH;
			}
		}
		else {
			if (routine.y > 0) {
				return routine.x == routine.y ? SOUTHWEST : routine.x > routine.y ? WEST : SOUTH;
			}
			else {
				return routine.x == routine.y ? NORTHWEST : routine.x > routine.y ? WEST : NORTH;
			}
		}
	}
}

// Get distance between 2 points.
int get_distance(Point from, Point to) {
	return abs(to.x - from.x) + abs(to.y - from.y);
}

// Decide the direction that Vampire or a servant tracks Akari or a villager.
int get_track_direction(Unit* punits, int my_id) {
	int arr_distance[5] = { 15, };
	int arr_distance_from_akari[4] = { 0, };
	int i = 0, j = 0, min_distance = 0, count = 0;
	int min_id = 0, direction = STAY;
	int max_id_from_akari = 0, count_max_from_akari = 0;
	int max_distance_from_akari = 0;
	int indexes_both[4] = { 0, }, count_both = 0;
	int shuffled_indexes[4];
	Point routine;

	// Wrong argument
	if (my_id != ID_VAMPIRE) {
		if (my_id < ID_SERVANT1 || my_id > ID_SERVANT4)
			return OTHER;
	}

	// Tracks the nearest unit.
	// When Akari and another villager is the nearest, tracks a villager.
	// When 2 or more villagers is the nearest, tracks one the most far from Akari.
	arr_distance[0] = min_distance = get_distance(punits[my_id].position, punits[ID_AKARI].position);
	count++;

	for (i = 0; i < 4; i++) {
		if (punits[ID_PEOPLE1 + i].position.x < 0 ||
			punits[ID_PEOPLE1 + i].position.y < 0)
			continue;

		arr_distance[i + 1] = get_distance(punits[my_id].position, punits[ID_PEOPLE1 + i].position);

		if (arr_distance[i + 1] == min_distance) {
			count++;
		}	

		if (arr_distance[i + 1] < min_distance) {
			min_distance = arr_distance[i + 1];
			min_id = i + ID_PEOPLE1;
		}
	}

	if (count == 1) {
		direction = get_direction(get_vector(punits[my_id].position, punits[min_id].position));
	}
	else {
		// Counting Condition:
		//  1. The nearest to Vampire
		//  2. The most far from Akari
		for (i = 0; i < 4; i++) {
			arr_distance_from_akari[i] = get_distance(punits[ID_PEOPLE1 + i].position, punits[ID_AKARI].position);

			if (punits[i + ID_PEOPLE1].position.x == -1 ||
				punits[i + ID_PEOPLE1].position.y == -1)
				continue;

			if (arr_distance_from_akari[i] > min_distance)
				continue;

			if (i == 0) {
				max_id_from_akari = ID_PEOPLE1;
				count_max_from_akari++;
			}
			else {
				if (arr_distance_from_akari[i - 1] < arr_distance_from_akari[i]) {
					max_id_from_akari = i + ID_PEOPLE1;
					indexes_both[count_max_from_akari] = i;
				}
				if (arr_distance_from_akari[i - 1] == arr_distance_from_akari[i]) {
					count_max_from_akari++;
					indexes_both[count_max_from_akari] = i;
				}
			}
		}

		if (count_max_from_akari > 1) {
			max_id_from_akari = indexes_both[rand() % count_max_from_akari];
		}

		direction = get_direction(get_vector(punits[my_id].position, punits[max_id_from_akari].position));
	}

	switch (direction) {
	case WEST:
	case NORTH:
	case SOUTH:
	case EAST:
		return direction;
	case NORTHWEST:
		return rand() % 2 ? NORTH : WEST;
	case NORTHEAST:
		return rand() % 2 ? NORTH : EAST;
	case SOUTHWEST:
		return rand() % 2 ? SOUTH : WEST;
	case SOUTHEAST:
		return rand() % 2 ? SOUTH : EAST;
	default:
		return OTHER;
	}
}

// Get vector between 2 points.
Point get_vector(Point from, Point to) {
	Point v;

	v.x = to.x - from.x;
	v.y = to.y - from.y;

	return v;
}

// Get flags of warning
int get_warning_flags(Game game) {
	int flags = 0;

	// Check if Akari is adjacent to a trap
	if (get_distance(game.units[ID_AKARI].position, game.traps[0]) == 1 ||
		get_distance(game.units[ID_AKARI].position, game.traps[1]) == 1) {
		flags += FLAG_WARNING_TRAP;
	}

	// Check if the vampire is near by Akari (distance = 1 or 2)
	if (get_distance(game.units[ID_AKARI].position, game.units[ID_VAMPIRE].position) == 1 ||
		get_distance(game.units[ID_AKARI].position, game.units[ID_VAMPIRE].position) == 2) {
		flags += FLAG_WARNING_VAMPIRE;
	}

	// Check if Akari and a servant is overlay 
	if (
		get_distance(game.units[ID_AKARI].position, game.units[ID_SERVANT1].position) == 0 ||
		get_distance(game.units[ID_AKARI].position, game.units[ID_SERVANT2].position) == 0 ||
		get_distance(game.units[ID_AKARI].position, game.units[ID_SERVANT3].position) == 0 ||
		get_distance(game.units[ID_AKARI].position, game.units[ID_SERVANT4].position) == 0
		) {
		flags += FLAG_WARNING_SERVANT;
	}

	// Check if Akari and a villager is overlay 
	if (
		get_distance(game.units[ID_AKARI].position, game.units[ID_PEOPLE1].position) == 0 ||
		get_distance(game.units[ID_AKARI].position, game.units[ID_PEOPLE2].position) == 0 ||
		get_distance(game.units[ID_AKARI].position, game.units[ID_PEOPLE3].position) == 0 ||
		get_distance(game.units[ID_AKARI].position, game.units[ID_PEOPLE4].position) == 0
		) {
		flags += FLAG_WARNING_VILLAGER;
	}

	return flags;
}

// Initialize the game
void init_game(Game* pgame) {
	// Initialize status
	int i;
	Game g;
	int distance = 0, overlay_flags = 0;
	
	// Initialize Akari
	g.units[ID_AKARI].id = ID_AKARI;
	//strcpy_s(g.units[ID_AKARI].name, CAPACITY_OF_NAME, NAME_AKARI);
	g.units[ID_AKARI].hp = MAX_HP_OF_AKARI;
	randomize_position(&g.units[ID_AKARI].position);
	g.units[ID_AKARI].attack_chance = ATTACK_CHANCE;

	// Initialize Vampire
	g.units[ID_VAMPIRE].id = ID_VAMPIRE;
	//strcpy_s(g.units[ID_VAMPIRE].name, CAPACITY_OF_NAME, NAME_VAMPIRE);
	g.units[ID_VAMPIRE].hp = MAX_HP_OF_VAMPIRE;
	while (1) {
		randomize_position(&g.units[ID_VAMPIRE].position);
		distance = get_distance(g.units[ID_VAMPIRE].position, g.units[ID_AKARI].position);
		
		if (distance == 0)
			continue;

		break;
	};
	g.units[ID_VAMPIRE].attack_chance = -1;

	// Initialize Village Peoples
	for (i = ID_PEOPLE1; i <= ID_PEOPLE4; i++) {
		g.units[i].id = i;
		//strcpy_s(g.units[i].name, CAPACITY_OF_NAME, NAME_PEOPLE1);
		g.units[i].hp = MAX_HP_OF_PEOPLE;
		while (1) {
			randomize_position(&g.units[i].position);

			overlay_flags = check_any_overlay_position(g, i, i - 1);
			if (overlay_flags == TRUE)
				continue;

			break;
		};
		g.units[i].attack_chance = -1;
	}
	
	// Set servants
	for (i = ID_SERVANT1; i <= ID_SERVANT4; i++) {
		g.units[i].attack_chance = -1;
		g.units[i].hp = MAX_HP_OF_SERVANT;
		g.units[i].id = i;
		//strcpy_s(g.units[i].name, CAPACITY_OF_NAME, NAME_SERVANT1);
		g.units[i].position.x = -1;
		g.units[i].position.y = -1;
	}
	
	// Set traps
	while (1) {
		randomize_position(&g.traps[0]);

		overlay_flags = check_any_overlay_position(g, ID_TRAP1, ID_TRAP1 - 1);
		if (overlay_flags == TRUE)
			continue;

		break;
	}

	while (1) {
		randomize_position(&g.traps[1]);

		overlay_flags = check_any_overlay_position(g, ID_TRAP2, ID_TRAP2 - 1);
		if (overlay_flags == TRUE)
			continue;

		break;
	}
	
	g.turn = 0;

	// Initialize lights
	for (i = 0; i < 4; i++) {
		g.lights_warn[i] = 0;
		g.lights_attack[i] = 0;
	}

	*pgame = g;
}

// Attack command
char input_attack_command() {
	char command = '\0';

	printf("U:Up D:Down L:Left R:Right S:Skip\n");
	printf("Please ender attack command: ");
	scanf_s("%c%c", &command);

	return command;
}

// Move command
char input_move_command() {
	char command = '\0';

	printf("U:Up D:Down L:Left R:Right Q:Quit\n");
	printf("Please ender move command: ");
	scanf_s("%c%c", &command);

	return command;
}

// People ===> Servant
void morph_servant(Unit* ppeople, Unit* pservant) {
	Unit new_servant;

	// In case of wrong argument:
	// Nothing happened.
	if (ppeople->id < ID_PEOPLE1 || ppeople->id > ID_PEOPLE4)
		return;
	if (pservant->id < ID_SERVANT1 || pservant->id > ID_SERVANT4)
		return;

	pservant->position.x = ppeople->position.x;
	pservant->position.y = ppeople->position.y;

	ppeople->position.x = -1;
	ppeople->position.y = -1;
	ppeople->hp = 0;
}

// Move Akari
void move(Point* ppos, char command) {
	Point new_pos;
	new_pos.x = ppos->x;
	new_pos.y = ppos->y;

	switch (command) {
	case 'U':
	case 'u':
		new_pos.y = max(0, new_pos.y - 1);
		break;
	case 'D':
	case 'd':
		new_pos.y = min(7, new_pos.y + 1);
		break;
	case 'R':
	case 'r':
		new_pos.x = min(7, new_pos.x + 1);
		break;
	case 'L':
	case 'l':
		new_pos.x = max(0, new_pos.x - 1);
		break;
	case 'Q':
	case 'q':
		quit();
		break;
	}

	*ppos = new_pos;
}

// Play the game
void play() {
	Game game;
	init_game(&game);

	while (1) {
		turn(&game);
	}
}

// Quit by player
void quit() {
	char c;
	printf("Are you sure quit[Y/N]: ");
	scanf_s("%c%c", &c);

	if (c == 'Y' || c == 'y')
		exit(0);
}

// Randomize the position
void randomize_position(Point* point) {
	Point p;
	p.x = rand() % FIELD_WIDTH;
	p.y = rand() % FIELD_HEIGHT;
	*point = p;
}

// Set attack lights
void set_attack_lights(int* lights, int flags) {
	lights[LIGHT_ATTACK_VAMPIRE] = GET_FLAG(flags, CHECK_VALUE_VAMPIRE);
	
	lights[LIGHT_ATTACK_TRAP] =
		GET_FLAG(flags, CHECK_VALUE_TRAP1) == 1 ||
		GET_FLAG(flags, CHECK_VALUE_TRAP2) == 1;
	
	lights[LIGHT_ATTACK_SERVANT] =
		GET_FLAG(flags, CHECK_VALUE_SERVANT1) == 1 ||
		GET_FLAG(flags, CHECK_VALUE_SERVANT2) == 1 ||
		GET_FLAG(flags, CHECK_VALUE_SERVANT3) == 1 ||
		GET_FLAG(flags, CHECK_VALUE_SERVANT4) == 1;

	lights[LIGHT_ATTACK_VILLAGER] =
		GET_FLAG(flags, CHECK_VALUE_PEOPLE1) == 1 ||
		GET_FLAG(flags, CHECK_VALUE_PEOPLE2) == 1 ||
		GET_FLAG(flags, CHECK_VALUE_PEOPLE3) == 1 ||
		GET_FLAG(flags, CHECK_VALUE_PEOPLE4) == 1;
}

// Set warning lights
void set_warn_lights(int* lights, int flags) {
	lights[LIGHT_WARN_VAMPIRE] = GET_FLAG(flags, FLAG_WARNING_VAMPIRE) == 1;
	lights[LIGHT_WARN_TRAP] = GET_FLAG(flags, FLAG_WARNING_TRAP) == 1;
	lights[LIGHT_WARN_SERVANT] = GET_FLAG(flags, FLAG_WARNING_SERVANT) == 1;
	lights[LIGHT_WARN_VILLAGER] = GET_FLAG(flags, FLAG_WARNING_VILLAGER) == 1;
}

// Testing
void test() {
	Game game;
	init_game(&game);

	printf("Turn : %d\n\n", game.turn);

	printf("[Akari]\n");
	//printf("Name : %s\n", game.units[ID_AKARI].name);
	printf("HP : %d\n", game.units[ID_AKARI].hp);
	printf("attack_chance : %d\n", game.units[ID_AKARI].attack_chance);
	printf("Position : (%d, %d)\n\n", game.units[ID_AKARI].position.x, game.units[ID_AKARI].position.y);
	
	printf("[Vampire]\n");
	//printf("Name : %s\n", game.units[ID_VAMPIRE].name);
	printf("HP : %d\n", game.units[ID_VAMPIRE].hp);
	printf("attack_chance : %d\n", game.units[ID_VAMPIRE].attack_chance);
	printf("Position : (%d, %d)\n\n", game.units[ID_VAMPIRE].position.x, game.units[ID_VAMPIRE].position.y);

	printf("[People1]\n");
	//printf("Name : %s\n", game.units[ID_PEOPLE1].name);
	printf("HP : %d\n", game.units[ID_PEOPLE1].hp);
	printf("attack_chance : %d\n", game.units[ID_PEOPLE1].attack_chance);
	printf("Position : (%d, %d)\n\n", game.units[ID_PEOPLE1].position.x, game.units[ID_PEOPLE1].position.y);

	printf("[People2]\n");
	//printf("Name : %s\n", game.units[ID_PEOPLE2].name);
	printf("HP : %d\n", game.units[ID_PEOPLE2].hp);
	printf("attack_chance : %d\n", game.units[ID_PEOPLE2].attack_chance);
	printf("Position : (%d, %d)\n\n", game.units[ID_PEOPLE2].position.x, game.units[ID_PEOPLE2].position.y);

	printf("[People1]\n");
	//printf("Name : %s\n", game.units[ID_PEOPLE3].name);
	printf("HP : %d\n", game.units[ID_PEOPLE3].hp);
	printf("attack_chance : %d\n", game.units[ID_PEOPLE3].attack_chance);
	printf("Position : (%d, %d)\n\n", game.units[ID_PEOPLE3].position.x, game.units[ID_PEOPLE3].position.y);

	printf("[People1]\n");
	//printf("Name : %s\n", game.units[ID_PEOPLE4].name);
	printf("HP : %d\n", game.units[ID_PEOPLE4].hp);
	printf("attack_chance : %d\n", game.units[ID_PEOPLE4].attack_chance);
	printf("Position : (%d, %d)\n\n", game.units[ID_PEOPLE4].position.x, game.units[ID_PEOPLE4].position.y);

	printf("[Trap1]\n");
	printf("Position : (%d, %d)\n\n", game.traps[0].x, game.traps[0].y);

	printf("[Trap2]\n");
	printf("Position : (%d, %d)\n\n", game.traps[1].x, game.traps[1].y);
}

// Execute 1 turn.
void turn(Game* pgame) {
	Point prev_pos, new_pos, atk_pos;
	int i = 0;
	int flag_overlay = 0, flag_warn = 0;
	int damage = 0, direction = 0;

	prev_pos.x = -1;
	prev_pos.y = -1;
	new_pos.x = -1;
	new_pos.y = -1;
	atk_pos.x = -1;
	atk_pos.y = -1;

	// Increase the turn number.
	pgame->turn++;

	// set warning lights
	
	flag_warn = get_warning_flags(*pgame);
	set_warn_lights(pgame->lights_warn, flag_warn);

	// Display
	display(*pgame);

	// Initialize attack lights
	for (i = 0; i < 4; i++) {
		pgame->lights_attack[i] = 0;
	}

	// Player's turn
	turn_akari(pgame);
	
	if (pgame->turn % 5 == 0) {
		// People's turn
		//  People move per 5 turns.
		turn_people(pgame);

		// Servants' turn
		//  Servants move per 5 turns.
		turn_servant(pgame);
	}

	// Vampire's turn
	//  Vampire moves per 2 turns.
	if (pgame->turn % 2 == 0) {
		turn_vampire(pgame);
	}
}

// Akari's turn
void turn_akari(Game* pgame) {
	Point new_pos, prev_pos, atk_pos;
	int flag_overlay = 0, flag_warn = 0, damage = 0;

	new_pos.x = prev_pos.x = pgame->units[ID_AKARI].position.x;
	new_pos.y = prev_pos.y = pgame->units[ID_AKARI].position.y;
	while (1) {
		move(&new_pos, input_move_command());

		if (prev_pos.x != new_pos.x || prev_pos.y != new_pos.y) {
			pgame->units[ID_AKARI].position.x = new_pos.x;
			pgame->units[ID_AKARI].position.y = new_pos.y;

			break;
		}
	}

	flag_overlay = check_any_overlay_position(*pgame, ID_AKARI, ID_TRAP2);
	flag_warn = get_warning_flags(*pgame);
	set_warn_lights(pgame->lights_warn, flag_warn);
	display(*pgame);	// Refresh game information

	if (GET_FLAG(flag_overlay, CHECK_VALUE_TRAP1) == 1) {
		// Touch a trap
		pgame->traps[0].x = -1;
		pgame->traps[0].y = -1;

		pgame->units[ID_AKARI].hp =
			max(pgame->units[ID_AKARI].hp - 3, 0);

		if (pgame->units[ID_AKARI].hp == 0) {

			display(*pgame);
			game_over(CODE_GAMEOVER_AKARIDEAD);
		}
	}
	if (GET_FLAG(flag_overlay, CHECK_VALUE_TRAP2) == 1) {
		// Touch a trap
		pgame->traps[1].x = -1;
		pgame->traps[1].y = -1;

		pgame->units[ID_AKARI].hp =
			max(pgame->units[ID_AKARI].hp - 3, 0);

		if (pgame->units[ID_AKARI].hp == 0) {
			display(*pgame);
			game_over(CODE_GAMEOVER_AKARIDEAD);
		}
	}
	if (GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE1) == 1 ||
		GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE2) == 1 ||
		GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE3) == 1 ||
		GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE4) == 1) {
		// Met a villager
		if (pgame->units[ID_AKARI].hp < MAX_HP_OF_AKARI) {
			pgame->units[ID_AKARI].hp++;
			display(*pgame);
		}
	}
	if (GET_FLAG(flag_overlay, CHECK_VALUE_SERVANT1) == 1) {
		// Met a servant
		pgame->units[ID_SERVANT1].hp = 0;
		pgame->units[ID_SERVANT1].position.x = -1;
		pgame->units[ID_SERVANT1].position.y = -1;
		pgame->units[ID_AKARI].hp--;

		if (pgame->units[ID_AKARI].hp == 0) {
			display(*pgame);
			game_over(CODE_GAMEOVER_AKARIDEAD);
		}
	}
	if (GET_FLAG(flag_overlay, CHECK_VALUE_SERVANT2) == 1) {
		// Met a servant
		pgame->units[ID_SERVANT2].hp = 0;
		pgame->units[ID_SERVANT2].position.x = -1;
		pgame->units[ID_SERVANT2].position.y = -1;
		pgame->units[ID_AKARI].hp--;

		if (pgame->units[ID_AKARI].hp == 0) {
			display(*pgame);
			game_over(CODE_GAMEOVER_AKARIDEAD);
		}
	}
	if (GET_FLAG(flag_overlay, CHECK_VALUE_SERVANT3) == 1) {
		// Met a servant
		pgame->units[ID_SERVANT3].hp = 0;
		pgame->units[ID_SERVANT3].position.x = -1;
		pgame->units[ID_SERVANT3].position.y = -1;
		pgame->units[ID_AKARI].hp--;

		if (pgame->units[ID_AKARI].hp == 0) {
			display(*pgame);
			game_over(CODE_GAMEOVER_AKARIDEAD);
		}
	}
	if (GET_FLAG(flag_overlay, CHECK_VALUE_SERVANT4) == 1) {
		// Met a servant
		pgame->units[ID_SERVANT4].hp = 0;
		pgame->units[ID_SERVANT4].position.x = -1;
		pgame->units[ID_SERVANT4].position.y = -1;
		pgame->units[ID_AKARI].hp--;

		if (pgame->units[ID_AKARI].hp == 0) {
			display(*pgame);
			game_over(CODE_GAMEOVER_AKARIDEAD);
		}
	}
	if (GET_FLAG(flag_overlay, CHECK_VALUE_VAMPIRE) == 1) {
		// Met vampire
		damage = min(pgame->units[ID_AKARI].hp, pgame->units[ID_VAMPIRE].hp);
		pgame->units[ID_AKARI].hp -= damage;
		pgame->units[ID_VAMPIRE].hp -= damage;

		display(*pgame);

		if (pgame->units[ID_AKARI].hp == 0) {
			if (pgame->units[ID_VAMPIRE].hp != 0) {
				// You lose
				game_over(CODE_GAMEOVER_AKARIDEAD);
			}
			else {
				// Draw
				game_over(CODE_GAMEOVER_DRAW);
			}
		}
		else if (pgame->units[ID_VAMPIRE].hp == 0) {
			// You win
			game_over(CODE_GAMEOVER_AKARIWON);
		}
	}
	display(*pgame);


	// Akari attacks
	// If any weapon doesn't remain, Akari doesn't attack.
	if (pgame->units[ID_AKARI].attack_chance > 0) {
		while (1) {
			switch (input_attack_command()) {
			case 'R':
			case 'r':
				atk_pos.x = pgame->units[ID_AKARI].position.x + 1;
				atk_pos.y = pgame->units[ID_AKARI].position.y;
				break;
			case 'L':
			case 'l':
				atk_pos.x = pgame->units[ID_AKARI].position.x - 1;
				atk_pos.y = pgame->units[ID_AKARI].position.y;
				break;
			case 'D':
			case 'd':
				atk_pos.x = pgame->units[ID_AKARI].position.x;
				atk_pos.y = pgame->units[ID_AKARI].position.y + 1;
				break;
			case 'U':
			case 'u':
				atk_pos.x = pgame->units[ID_AKARI].position.x;
				atk_pos.y = pgame->units[ID_AKARI].position.y - 1;
				break;
			case 'S':
			case 's':
				atk_pos.x = pgame->units[ID_AKARI].position.x;
				atk_pos.y = pgame->units[ID_AKARI].position.y;
				break;
			default:
				atk_pos.x = -1;
				atk_pos.y = -1;
			}

			if (check_on_stage(atk_pos))
				break;
		}
		if (atk_pos.x != pgame->units[ID_AKARI].position.x ||
			atk_pos.y != pgame->units[ID_AKARI].position.y) {

			flag_overlay = check_any_overlay_position_by_point(*pgame, atk_pos);
			set_attack_lights(&pgame->lights_attack, flag_overlay);

			// Akari attacked Vampire
			if (GET_FLAG(flag_overlay, CHECK_VALUE_VAMPIRE) == 1) {
				pgame->units[ID_VAMPIRE].hp--;

				if (pgame->units[ID_VAMPIRE].hp == 0) {
					// You win
					display(*pgame);
					game_over(CODE_GAMEOVER_AKARIWON);
				}
			}

			// Akari attacked a trap
			if (GET_FLAG(flag_overlay, CHECK_VALUE_TRAP1) == 1) {
				pgame->traps[0].x = -1;
				pgame->traps[0].y = -1;
			}
			if (GET_FLAG(flag_overlay, CHECK_VALUE_TRAP2) == 1) {
				pgame->traps[1].x = -1;
				pgame->traps[1].y = -1;
			}

			// Akari attacked a villager
			if (GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE1) == 1) {
				pgame->units[ID_PEOPLE1].hp = 0;
				pgame->units[ID_PEOPLE1].position.x = -1;
				pgame->units[ID_PEOPLE1].position.y = -1;
			}
			if (GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE2) == 1) {
				pgame->units[ID_PEOPLE2].hp = 0;
				pgame->units[ID_PEOPLE2].position.x = -1;
				pgame->units[ID_PEOPLE2].position.y = -1;
			}
			if (GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE3) == 1) {
				pgame->units[ID_PEOPLE3].hp = 0;
				pgame->units[ID_PEOPLE3].position.x = -1;
				pgame->units[ID_PEOPLE3].position.y = -1;
			}
			if (GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE4) == 1) {
				pgame->units[ID_PEOPLE4].hp = 0;
				pgame->units[ID_PEOPLE4].position.x = -1;
				pgame->units[ID_PEOPLE4].position.y = -1;
			}
			if (!GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE1) == 1 &&
				!GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE2) == 1 &&
				!GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE3) == 1 &&
				!GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE4) == 1) {

			}
			if (!check_villager_extinct(*pgame)) {
				display(*pgame);
				game_over(CODE_GAMEOVER_PEOPLEDEAD);
			}

			// Akari attacked a servant
			if (GET_FLAG(flag_overlay, CHECK_VALUE_SERVANT1) == 1) {
				pgame->units[ID_SERVANT1].hp = 0;
				pgame->units[ID_SERVANT1].position.x = -1;
				pgame->units[ID_SERVANT1].position.y = -1;
			}
			if (GET_FLAG(flag_overlay, CHECK_VALUE_SERVANT2) == 1) {
				pgame->units[ID_SERVANT2].hp = 0;
				pgame->units[ID_SERVANT2].position.x = -1;
				pgame->units[ID_SERVANT2].position.y = -1;
			}
			if (GET_FLAG(flag_overlay, CHECK_VALUE_SERVANT3) == 1) {
				pgame->units[ID_SERVANT3].hp = 0;
				pgame->units[ID_SERVANT3].position.x = -1;
				pgame->units[ID_SERVANT3].position.y = -1;
			}
			if (GET_FLAG(flag_overlay, CHECK_VALUE_SERVANT4) == 1) {
				pgame->units[ID_SERVANT4].hp = 0;
				pgame->units[ID_SERVANT4].position.x = -1;
				pgame->units[ID_SERVANT4].position.y = -1;
			}
			if (!GET_FLAG(flag_overlay, CHECK_VALUE_SERVANT1) == 1 &&
				!GET_FLAG(flag_overlay, CHECK_VALUE_SERVANT2) == 1 &&
				!GET_FLAG(flag_overlay, CHECK_VALUE_SERVANT3) == 1 &&
				!GET_FLAG(flag_overlay, CHECK_VALUE_SERVANT4) == 1) {
			}

			// After attack
			pgame->units[ID_AKARI].attack_chance = max(pgame->units[ID_AKARI].attack_chance - 1, 0);
		}
	}
}

// People's turn
void turn_people(Game* pgame) {
	int i = 0, direction = 0, flag_overlay = 0;
	Point new_pos, prev_pos;

	for (i = ID_PEOPLE1; i <= ID_PEOPLE4; i++) {
		new_pos.x = prev_pos.x = pgame->units[i].position.x;
		new_pos.y = prev_pos.y = pgame->units[i].position.y;

		// A dead villager doesn't move.
		if (prev_pos.x == -1 || prev_pos.y == -1)
			continue;

		while (1) {
			direction = rand() % 4;

			switch (direction) {
			case WEST:
				new_pos.x--;
				break;
			case NORTH:
				new_pos.y--;
				break;
			case SOUTH:
				new_pos.y++;
				break;
			case EAST:
				new_pos.x++;
				break;
			}

			if (check_on_stage(new_pos)) {
				break;
			}

			new_pos.x = prev_pos.x;
			new_pos.y = prev_pos.y;
		}

		pgame->units[i].position = new_pos;
		flag_overlay = check_any_overlay_position(*pgame, i, ID_TRAP2);

		// A villager touched a trap
		if (GET_FLAG(flag_overlay, CHECK_VALUE_TRAP1) == 1) {
			pgame->units[i].hp = 0;
			pgame->units[i].position.x = -1;
			pgame->units[i].position.y = -1;

			pgame->traps[0].x = -1;
			pgame->traps[0].y = -1;
		}
		if (GET_FLAG(flag_overlay, CHECK_VALUE_TRAP2) == 1) {
			pgame->units[i].hp = 0;
			pgame->units[i].position.x = -1;
			pgame->units[i].position.y = -1;

			pgame->traps[1].x = -1;
			pgame->traps[1].y = -1;
		}

		// A villager met Akari
		if (GET_FLAG(flag_overlay, CHECK_VALUE_AKARI) == 1) {
			if (pgame->units[ID_AKARI].hp < MAX_HP_OF_AKARI) {
				pgame->units[ID_AKARI].hp++;
			}
		}

		// A villager met a servant
		if (GET_FLAG(flag_overlay, CHECK_VALUE_SERVANT1) == 1 ||
			GET_FLAG(flag_overlay, CHECK_VALUE_SERVANT2) == 1 ||
			GET_FLAG(flag_overlay, CHECK_VALUE_SERVANT3) == 1 ||
			GET_FLAG(flag_overlay, CHECK_VALUE_SERVANT4) == 1) {

			pgame->units[i].hp = 0;
			pgame->units[i].position.x = -1;
			pgame->units[i].position.y = -1;
		}

		// A villager met Vampire
		if (GET_FLAG(flag_overlay, CHECK_VALUE_VAMPIRE) == 1) {
			morph_servant(&pgame->units[i], &pgame->units[i + 4]);
		}

		if (!check_villager_extinct(*pgame)) {
			display(*pgame);
			game_over(CODE_GAMEOVER_PEOPLEDEAD);
		}
	}
}

// Servants' turn
void turn_servant(Game* pgame) {
	int i = 0, direction = 0, flag_overlay = 0;
	Point new_pos, prev_pos;

	for (i = ID_SERVANT1; i <= ID_SERVANT4; i++) {

		new_pos.x = prev_pos.x = pgame->units[i].position.x;
		new_pos.y = prev_pos.y = pgame->units[i].position.y;

		// An inactive servant doesn't move.
		if (prev_pos.x == -1 || prev_pos.y == -1)
			continue;

		while (1) {
			direction = get_track_direction(pgame->units, i);

			switch (direction) {
			case WEST:
				new_pos.x--;
				break;
			case NORTH:
				new_pos.y--;
				break;
			case SOUTH:
				new_pos.y++;
				break;
			case EAST:
				new_pos.x++;
				break;
			}

			if (check_on_stage(new_pos)) {
				break;
			}

			new_pos.x = prev_pos.x;
			new_pos.y = prev_pos.y;
		}

		pgame->units[i].position = new_pos;
		flag_overlay = check_any_overlay_position(*pgame, i, ID_TRAP2);

		// A servant touched a trap
		if (GET_FLAG(flag_overlay, CHECK_VALUE_TRAP1) == 1) {
			pgame->units[i].hp = 0;
			pgame->units[i].position.x = -1;
			pgame->units[i].position.y = -1;

			pgame->traps[0].x = -1;
			pgame->traps[0].y = -1;
		}
		if (GET_FLAG(flag_overlay, CHECK_VALUE_TRAP2) == 1) {
			pgame->units[i].hp = 0;
			pgame->units[i].position.x = -1;
			pgame->units[i].position.y = -1;

			pgame->traps[1].x = -1;
			pgame->traps[1].y = -1;
		}

		// A servant met Vampire
		// Nothing happened.

		// A servant met a villager
		if (GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE1) == 1) {
			pgame->units[ID_PEOPLE1].hp = 0;
			pgame->units[ID_PEOPLE1].position.x = -1;
			pgame->units[ID_PEOPLE1].position.y = -1;
		}
		if (GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE2) == 1) {
			pgame->units[ID_PEOPLE2].hp = 0;
			pgame->units[ID_PEOPLE2].position.x = -1;
			pgame->units[ID_PEOPLE2].position.y = -1;
		}
		if (GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE3) == 1) {
			pgame->units[ID_PEOPLE3].hp = 0;
			pgame->units[ID_PEOPLE3].position.x = -1;
			pgame->units[ID_PEOPLE3].position.y = -1;
		}
		if (GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE4) == 1) {
			pgame->units[ID_PEOPLE4].hp = 0;
			pgame->units[ID_PEOPLE4].position.x = -1;
			pgame->units[ID_PEOPLE4].position.y = -1;
		}
		if (!check_villager_extinct(*pgame)) {
			display(*pgame);
			game_over(CODE_GAMEOVER_PEOPLEDEAD);
		}

		// A servant met Akari
		if (GET_FLAG(flag_overlay, CHECK_VALUE_AKARI) == 1) {
			pgame->units[i].hp = 0;
			pgame->units[i].position.x = -1;
			pgame->units[i].position.y = -1;

			pgame->units[ID_AKARI].hp--;

			if (pgame->units[ID_AKARI].hp == 0) {
				display(*pgame);
				game_over(CODE_GAMEOVER_AKARIDEAD);
			}
		}
	}
}

// Vampire's turn
void turn_vampire(Game* pgame) {
	int i = 0, direction = 0, flag_overlay = 0, damage = 0;
	Point new_pos, prev_pos;

	new_pos.x = prev_pos.x = pgame->units[ID_VAMPIRE].position.x;
	new_pos.y = prev_pos.y = pgame->units[ID_VAMPIRE].position.y;

	while (1) {
		direction = get_track_direction(pgame->units, ID_VAMPIRE);

		switch (direction) {
		case WEST:
			new_pos.x--;
			break;
		case NORTH:
			new_pos.y--;
			break;
		case SOUTH:
			new_pos.y++;
			break;
		case EAST:
			new_pos.x++;
			break;
		}

		if (check_on_stage(new_pos)) {
			break;
		}

		new_pos.x = prev_pos.x;
		new_pos.y = prev_pos.y;
	}

	pgame->units[ID_VAMPIRE].position = new_pos;
	flag_overlay = check_any_overlay_position(*pgame, ID_VAMPIRE, ID_TRAP2);

	// Vampire touched a trap
	if (GET_FLAG(flag_overlay, CHECK_VALUE_TRAP1) == 1) {
		pgame->traps[0].x = -1;
		pgame->traps[0].y = -1;

		pgame->units[ID_VAMPIRE].hp =
			max(pgame->units[ID_VAMPIRE].hp - 3, 0);

		if (pgame->units[ID_VAMPIRE].hp == 0) {
			display(*pgame);
			game_over(CODE_GAMEOVER_AKARIWON);
		}
	}
	if (GET_FLAG(flag_overlay, CHECK_VALUE_TRAP1) == 1) {
		pgame->traps[1].x = -1;
		pgame->traps[1].y = -1;

		pgame->units[ID_VAMPIRE].hp =
			max(pgame->units[ID_VAMPIRE].hp - 3, 0);

		if (pgame->units[ID_VAMPIRE].hp == 0) {
			display(*pgame);
			game_over(CODE_GAMEOVER_AKARIWON);
		}
	}

	// Vampire met a servant
	// Nothing happened.

	// Vampire met a villager
	if (GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE1) == 1) {
		morph_servant(&pgame->units[ID_PEOPLE1], &pgame->units[ID_SERVANT1]);
	}
	if (GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE2) == 1) {
		morph_servant(&pgame->units[ID_PEOPLE2], &pgame->units[ID_SERVANT2]);
	}
	if (GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE3) == 1) {
		morph_servant(&pgame->units[ID_PEOPLE3], &pgame->units[ID_SERVANT3]);
	}
	if (GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE4) == 1) {
		morph_servant(&pgame->units[ID_PEOPLE4], &pgame->units[ID_SERVANT4]);
	}

	if (!check_villager_extinct(*pgame)) {
		display(*pgame);
		game_over(CODE_GAMEOVER_PEOPLEDEAD);
	}

	// Vampire met Akari
	if (GET_FLAG(flag_overlay, CHECK_VALUE_AKARI) == 1) {
		damage = min(pgame->units[ID_AKARI].hp, pgame->units[ID_VAMPIRE].hp);
		pgame->units[ID_AKARI].hp -= damage;
		pgame->units[ID_VAMPIRE].hp -= damage;

		display(*pgame);

		if (pgame->units[ID_AKARI].hp == 0) {
			if (pgame->units[ID_VAMPIRE].hp != 0) {
				// You lose
				game_over(CODE_GAMEOVER_AKARIDEAD);
			}
			else {
				// Draw
				game_over(CODE_GAMEOVER_DRAW);
			}
		}
		else if (pgame->units[ID_VAMPIRE].hp == 0) {
			// You win
			game_over(CODE_GAMEOVER_AKARIWON);
		}
	}
}
