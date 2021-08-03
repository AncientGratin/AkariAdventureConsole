#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "akariadv.h"

// Check if there is any overlay position of units
//  Return Value:
//  Summary of all
int check_any_overlay_position(Game game, int id_myself) {
	int i;
	int result = 0;
	Point distance;

	for (i = 0; i < NUMBER_OF_CHARACTERS; i++) {
		
	}
}

// Check if overlay position of 2 units
//  Return Value:
//  1 = TRUE, 0 = FALSE
int check_overlay_position(Point p1, Point p2) {
	if (p1.x == p2.x && p1.y == p2.y)
		return 1;

	return 0;
}

// Display the game information on the screen.
void display(Game game) {
	int i, j;
	int warn_flags = get_warning_flags(game);
	system("cls");

	//printf("\n");
	//printf(" AKARI'S ADVENTURE\n");
	//printf("\n");
	//printf("  0 1 2 3 4 5 6 7\n");
	//printf(" -----------------  Akari [9/5]\n");
	//printf("0| | | | | | | | |  Vampire [9]\n");
	//printf(" -----------------\n");
	//printf("1| | | | | | | | |  Warning!\n");
	//printf(" -----------------  [ ] Adjacent a trap\n");
	//printf("2| | | | | | | | |  [ ] Vampire approach\n");
	//printf(" -----------------  [ ] Met a servant\n");
	//printf("3| | | | | | | | |  [ ] Met a villager\n");
	//printf(" -----------------\n");
	//printf("4| | | | | | | | |  Akari attacked\n");
	//printf(" -----------------  [ ] Vampire\n");
	//printf("5| | | | | | | | |  [ ] a villager\n");
	//printf(" -----------------  [ ] a servant\n");
	//printf("6| | | | | | | | |  [ ] a trap\n");
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
			printf("  Akari [%d/%d]", game.units[ID_AKARI].hp, game.units[ID_AKARI].attack_chance);
			break;
		case 2:
			printf("  [%c] Adjacent trap", GET_FLAG(warn_flags, FLAG_WARNING_TRAP) ? '*' : ' ');
			break;
		case 3:
			printf("  [%c] Met a servant", GET_FLAG(warn_flags, FLAG_WARNING_SERVANT) ? '*' : ' ');
			break;
		case 5:
			printf("  [%c] Vampire", ' ');
			break;
		case 6:
			printf("  [%c] a servant", ' ');
			break;
		}

		printf("\n%d", i);

		for (j = 0; j < 8; j++) {
			printf("|%c",
				game.units[ID_AKARI].position.x == j && game.units[ID_AKARI].position.y == i ? 'A' : ' ');
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
			printf("  [%c] Vampire approach", GET_FLAG(warn_flags, FLAG_WARNING_VAMPIRE) ? '*' : ' ');
			break;
		case 3:
			printf("  [%c] Met a villager", GET_FLAG(warn_flags, FLAG_WARNING_VILLAGER) ? '*' : ' ');
			break;
		case 4:
			printf("  Akari attacked");
			break;
		case 5:
			printf("  [%c] a villager", ' ');
			break;
		case 6:
			printf("  [%c] a trap", ' ');
			break;
		case 7:
			printf("  present by");
			break;
		}

		printf("\n");
	}

	printf(" -----------------  AncientGratin\n\n");
}

// Get distance between 2 points.
int get_distance(Point from, Point to) {
	return abs(to.x - from.x) + abs(to.y - from.y);
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

	// Check if the vampire is near by Akari (distance <= 2)
	if (get_distance(game.units[ID_AKARI].position, game.units[ID_VAMPIRE].position) <= 2) {
		flags += FLAG_WARNING_VAMPIRE;
	}

	// Check if Akari and a servant is overlay 
	if (
		check_overlay_position(game.units[ID_AKARI].position, game.units[ID_SERVANT1].position) ||
		check_overlay_position(game.units[ID_AKARI].position, game.units[ID_SERVANT2].position) ||
		check_overlay_position(game.units[ID_AKARI].position, game.units[ID_SERVANT3].position) ||
		check_overlay_position(game.units[ID_AKARI].position, game.units[ID_SERVANT4].position)
		) {
		flags += FLAG_WARNING_SERVANT;
	}

	// Check if Akari and a villager is overlay 
	if (
		check_overlay_position(game.units[ID_AKARI].position, game.units[ID_PEOPLE1].position) ||
		check_overlay_position(game.units[ID_AKARI].position, game.units[ID_PEOPLE2].position) ||
		check_overlay_position(game.units[ID_AKARI].position, game.units[ID_PEOPLE3].position) ||
		check_overlay_position(game.units[ID_AKARI].position, game.units[ID_PEOPLE4].position)
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
	Point distance;
	
	// Initialize Akari
	g.units[ID_AKARI].id = ID_AKARI;
	//g.units[ID_AKARI].name = (char*)malloc(sizeof(char) * strlen(NAME_AKARI));
	strcpy_s(g.units[ID_AKARI].name, CAPACITY_OF_NAME, NAME_AKARI);
	g.units[ID_AKARI].hp = MAX_HP_OF_AKARI;
	randomize_position(&g.units[ID_AKARI].position);
	g.units[ID_AKARI].attack_chance = ATTACK_CHANCE;

	// Initialize Vampire
	g.units[ID_VAMPIRE].id = ID_VAMPIRE;
	strcpy_s(g.units[ID_VAMPIRE].name, CAPACITY_OF_NAME, NAME_VAMPIRE);
	g.units[ID_VAMPIRE].hp = MAX_HP_OF_VAMPIRE;
	while (1) {
		randomize_position(&g.units[ID_VAMPIRE].position);
		distance = get_vector(g.units[ID_VAMPIRE].position, g.units[ID_AKARI].position);
		
		if (distance.x == 0 && distance.y == 0)
			continue;

		break;
	};
	g.units[ID_VAMPIRE].attack_chance = -1;

	// Initialize Village Peoples
	g.units[ID_PEOPLE1].id = ID_PEOPLE1;
	strcpy_s(g.units[ID_PEOPLE1].name, CAPACITY_OF_NAME, NAME_PEOPLE1);
	g.units[ID_PEOPLE1].hp = MAX_HP_OF_PEOPLE;
	while (1) {
		randomize_position(&g.units[ID_PEOPLE1].position);

		distance = get_vector(g.units[ID_PEOPLE1].position, g.units[ID_AKARI].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		distance = get_vector(g.units[ID_PEOPLE1].position, g.units[ID_VAMPIRE].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		break;
	};
	g.units[ID_PEOPLE1].attack_chance = -1;
	
	g.units[ID_PEOPLE2].id = ID_PEOPLE2;
	strcpy_s(g.units[ID_PEOPLE2].name, CAPACITY_OF_NAME, NAME_PEOPLE2);
	g.units[ID_PEOPLE2].hp = MAX_HP_OF_PEOPLE;
	while (1) {
		randomize_position(&g.units[ID_PEOPLE2].position);

		distance = get_vector(g.units[ID_PEOPLE2].position, g.units[ID_AKARI].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		distance = get_vector(g.units[ID_PEOPLE2].position, g.units[ID_VAMPIRE].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		distance = get_vector(g.units[ID_PEOPLE2].position, g.units[ID_PEOPLE1].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		break;
	};
	g.units[ID_PEOPLE2].attack_chance = -1;

	g.units[ID_PEOPLE3].id = ID_PEOPLE3;
	strcpy_s(g.units[ID_PEOPLE3].name, CAPACITY_OF_NAME, NAME_PEOPLE3);
	g.units[ID_PEOPLE3].hp = MAX_HP_OF_PEOPLE;
	while (1) {
		randomize_position(&g.units[ID_PEOPLE3].position);

		distance = get_vector(g.units[ID_PEOPLE3].position, g.units[ID_AKARI].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		distance = get_vector(g.units[ID_PEOPLE3].position, g.units[ID_VAMPIRE].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		distance = get_vector(g.units[ID_PEOPLE3].position, g.units[ID_PEOPLE1].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		distance = get_vector(g.units[ID_PEOPLE3].position, g.units[ID_PEOPLE2].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		break;
	};
	g.units[ID_PEOPLE3].attack_chance = -1;

	g.units[ID_PEOPLE4].id = ID_PEOPLE4;
	strcpy_s(g.units[ID_PEOPLE4].name, CAPACITY_OF_NAME, NAME_PEOPLE4);
	g.units[ID_PEOPLE4].hp = MAX_HP_OF_PEOPLE;
	while (1) {
		randomize_position(&g.units[ID_PEOPLE4].position);

		distance = get_vector(g.units[ID_PEOPLE4].position, g.units[ID_AKARI].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		distance = get_vector(g.units[ID_PEOPLE4].position, g.units[ID_VAMPIRE].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		distance = get_vector(g.units[ID_PEOPLE4].position, g.units[ID_PEOPLE1].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		distance = get_vector(g.units[ID_PEOPLE4].position, g.units[ID_PEOPLE2].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		distance = get_vector(g.units[ID_PEOPLE4].position, g.units[ID_PEOPLE3].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		break;
	};
	g.units[ID_PEOPLE4].attack_chance = -1;

	// Set traps
	while (1) {
		randomize_position(&g.traps[0]);

		distance = get_vector(g.traps[0], g.units[ID_AKARI].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		distance = get_vector(g.traps[0], g.units[ID_VAMPIRE].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		distance = get_vector(g.traps[0], g.units[ID_PEOPLE1].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		distance = get_vector(g.traps[0], g.units[ID_PEOPLE2].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		distance = get_vector(g.traps[0], g.units[ID_PEOPLE3].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		distance = get_vector(g.traps[0], g.units[ID_PEOPLE4].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		break;
	}

	while (1) {
		randomize_position(&g.traps[1]);

		distance = get_vector(g.traps[1], g.units[ID_AKARI].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		distance = get_vector(g.traps[1], g.units[ID_VAMPIRE].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		distance = get_vector(g.traps[1], g.units[ID_PEOPLE1].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		distance = get_vector(g.traps[1], g.units[ID_PEOPLE2].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		distance = get_vector(g.traps[1], g.units[ID_PEOPLE3].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		distance = get_vector(g.traps[1], g.units[ID_PEOPLE4].position);
		if (distance.x == 0 && distance.y == 0)
			continue;

		distance = get_vector(g.traps[1], g.traps[0]);
		if (distance.x == 0 && distance.y == 0)
			continue;

		break;
	}
	
	g.turn = 0;

	*pgame = g;
}

// Attack command
int input_attack_command() {
	int n = 0;

	printf("U:Up D:Down L:Left R:Right N:Nothing\n");
	printf("Please ender attack command: ");
	scanf_s("%d", &n);

	return n;
}

// Move command
char input_move_command() {
	char command;

	printf("U:Up D:Down L:Left R:Right Q:Quit\n");
	printf("Please ender move command: ");
	scanf_s("%c", &command);

	return command;
}

// People ===> Servant
void morph_servant(Unit* ppeople) {
	Unit new_servant;

	// When the argument is not a people, it does not happened.
	switch (ppeople->id) {
	case ID_PEOPLE1:
		new_servant.id = ID_SERVANT1;
		strcpy_s(new_servant.name, CAPACITY_OF_NAME, NAME_SERVANT1);
		break;
	case ID_PEOPLE2:
		new_servant.id = ID_SERVANT2;
		strcpy_s(new_servant.name, CAPACITY_OF_NAME, NAME_SERVANT2);
		break;
	case ID_PEOPLE3:
		new_servant.id = ID_SERVANT3;
		strcpy_s(new_servant.name, CAPACITY_OF_NAME, NAME_SERVANT3);
		break;
	case ID_PEOPLE4:
		new_servant.id = ID_SERVANT4;
		strcpy_s(new_servant.name, CAPACITY_OF_NAME, NAME_SERVANT4);
		break;
	default:
		return;
	}

	// When a people is turned to a servant, HP is initialized.
	new_servant.hp = MAX_HP_OF_SERVANT;
	new_servant.attack_chance = -1;
	new_servant.position.x = ppeople->position.x;
	new_servant.position.y = ppeople->position.y;

	*ppeople = new_servant;
}

// Move a unit
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

// Quit
void quit() {
	char c;
	printf("Are you sure quit[Y/N]: ");
	scanf_s("%c", &c);
	scanf_s("%c", &c);

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

// Testing
void test() {
	Game game;
	init_game(&game);

	printf("Turn : %d\n\n", game.turn);

	printf("[Akari]\n");
	printf("Name : %s\n", game.units[ID_AKARI].name);
	printf("HP : %d\n", game.units[ID_AKARI].hp);
	printf("attack_chance : %d\n", game.units[ID_AKARI].attack_chance);
	printf("Position : (%d, %d)\n\n", game.units[ID_AKARI].position.x, game.units[ID_AKARI].position.y);
	
	printf("[Vampire]\n");
	printf("Name : %s\n", game.units[ID_VAMPIRE].name);
	printf("HP : %d\n", game.units[ID_VAMPIRE].hp);
	printf("attack_chance : %d\n", game.units[ID_VAMPIRE].attack_chance);
	printf("Position : (%d, %d)\n\n", game.units[ID_VAMPIRE].position.x, game.units[ID_VAMPIRE].position.y);

	printf("[People1]\n");
	printf("Name : %s\n", game.units[ID_PEOPLE1].name);
	printf("HP : %d\n", game.units[ID_PEOPLE1].hp);
	printf("attack_chance : %d\n", game.units[ID_PEOPLE1].attack_chance);
	printf("Position : (%d, %d)\n\n", game.units[ID_PEOPLE1].position.x, game.units[ID_PEOPLE1].position.y);

	printf("[People2]\n");
	printf("Name : %s\n", game.units[ID_PEOPLE2].name);
	printf("HP : %d\n", game.units[ID_PEOPLE2].hp);
	printf("attack_chance : %d\n", game.units[ID_PEOPLE2].attack_chance);
	printf("Position : (%d, %d)\n\n", game.units[ID_PEOPLE2].position.x, game.units[ID_PEOPLE2].position.y);

	printf("[People1]\n");
	printf("Name : %s\n", game.units[ID_PEOPLE3].name);
	printf("HP : %d\n", game.units[ID_PEOPLE3].hp);
	printf("attack_chance : %d\n", game.units[ID_PEOPLE3].attack_chance);
	printf("Position : (%d, %d)\n\n", game.units[ID_PEOPLE3].position.x, game.units[ID_PEOPLE3].position.y);

	printf("[People1]\n");
	printf("Name : %s\n", game.units[ID_PEOPLE4].name);
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
	Point prev_pos, new_pos;

	// Increase the turn number.
	pgame->turn++;

	// Display
	display(*pgame);

	// Player's turn
	new_pos.x = prev_pos.x = pgame->units[ID_AKARI].position.x;
	new_pos.y = prev_pos.y = pgame->units[ID_AKARI].position.y;
	while (1) {
		move(&new_pos, input_move_command());

		if (prev_pos.x != new_pos.x || prev_pos.y != new_pos.y) {
			pgame->units[ID_AKARI].position.x = new_pos.x;
			pgame->units[ID_AKARI].position.y = new_pos.y;

			break;
		}

		display(*pgame);	// Refresh game information
	}

	// People's turn
	//  People move per 5 turns.

	// Vampire's turn
	//  Vampire moves per 2 turns.

	// Servants' turn
	//  Servants move per 5 turns.
}
