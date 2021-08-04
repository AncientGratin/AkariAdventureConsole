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
		return -1;	// Wrong argument!
	}
	
	// On stage only
	if (my_pos.x < 0 || my_pos.x > 7 || my_pos.y < 0 || my_pos.y > 7)
		return 0;

	for (i = 0; i <= max_id; i++) {
		// pass myself
		if (i == id_myself)
			continue;

		if (get_distance(my_pos, game.units[i].position) == 0) {
			flags += (int)pow(2, i);
		}
	}

	if (max_id >= ID_TRAP1 && get_distance(my_pos, game.traps[0]) == 0)
		flags += (int)pow(2, ID_TRAP1);

	if (max_id >= ID_TRAP2 && get_distance(my_pos, game.traps[1]) == 0)
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

	if (here.x < 0 || here.x > 7 || here.y < 0 || here.y > 7) {
		return -1;	// Wrong argument!
	}

	for (i = 0; i <= ID_SERVANT4; i++) {
		there.x = game.units[i].position.x;
		there.y = game.units[i].position.y;

		if (there.x < 0 || there.x > 7 || there.y < 0 || there.y > 7) {
			continue;
		}

		if (get_distance(here, there) == 0) {
			flags += (int)pow(2, i);
		}
	}

	there.x = game.traps[0].x;
	there.y = game.traps[0].y;

	if (there.x > -1 && there.x < 8 && 
		there.y > -1 && there.y < 8 &&
		get_distance(here, there) == 0 )
		flags += (int)pow(2, ID_TRAP1);

	there.x = game.traps[1].x;
	there.y = game.traps[1].y;

	if (there.x > -1 && there.x < 8 &&
		there.y > -1 && there.y < 8 &&
		get_distance(here, there) == 0)
		flags += (int)pow(2, ID_TRAP2);

	return flags;
}

// Display the game information on the screen.
void display(Game game) {
	int i, j;
#ifdef DEBUG
	char dbg_c = ' ';	// For debugging
	int dbg_overlay_flags = 0;
	Point dbg_pos;
#endif
	int warn_flags = get_warning_flags(game);
	system("cls");

	//printf("\n");
	//printf(" AKARI'S ADVENTURE\n");
	//printf("\n");
	//printf("  0 1 2 3 4 5 6 7\n");
	//printf(" -----------------  Akari   [9/5]\n");
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
			printf("  Akari   [%d/%d]", game.units[ID_AKARI].hp, game.units[ID_AKARI].attack_chance);
			break;
		case 2:
			printf("  [%c] Adjacent trap", GET_FLAG(warn_flags, FLAG_WARNING_TRAP) > 0 ? '*' : ' ');
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

			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_AKARI)) {
				dbg_c = 'A';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_VAMPIRE)) {
				dbg_c = dbg_c == ' ' ? 'V' : 'O';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_PEOPLE1)) {
				dbg_c = dbg_c == ' ' ? 'P' : 'O';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_PEOPLE2)) {
				dbg_c = dbg_c == ' ' ? 'P' : 'O';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_PEOPLE3)) {
				dbg_c = dbg_c == ' ' ? 'P' : 'O';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_PEOPLE4)) {
				dbg_c = dbg_c == ' ' ? 'P' : 'O';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_SERVANT1)) {
				dbg_c = dbg_c == ' ' ? 'S' : 'O';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_SERVANT2)) {
				dbg_c = dbg_c == ' ' ? 'S' : 'O';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_SERVANT3)) {
				dbg_c = dbg_c == ' ' ? 'S' : 'O';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_SERVANT4)) {
				dbg_c = dbg_c == ' ' ? 'S' : 'O';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_SERVANT1)) {
				dbg_c = dbg_c == ' ' ? 'S' : 'O';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_TRAP1)) {
				dbg_c = dbg_c == ' ' ? 'T' : 'O';
			}
			if (GET_FLAG(dbg_overlay_flags, CHECK_VALUE_TRAP2)) {
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
		distance = get_distance(g.units[ID_VAMPIRE].position, g.units[ID_AKARI].position);
		
		if (distance == 0)
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

		distance = get_distance(g.units[ID_PEOPLE1].position, g.units[ID_AKARI].position);
		if (distance == 0)
			continue;

		distance = get_distance(g.units[ID_PEOPLE1].position, g.units[ID_VAMPIRE].position);
		if (distance == 0)
			continue;

		break;
	};
	g.units[ID_PEOPLE1].attack_chance = -1;
	
	g.units[ID_PEOPLE2].id = ID_PEOPLE2;
	strcpy_s(g.units[ID_PEOPLE2].name, CAPACITY_OF_NAME, NAME_PEOPLE2);
	g.units[ID_PEOPLE2].hp = MAX_HP_OF_PEOPLE;
	while (1) {
		randomize_position(&g.units[ID_PEOPLE2].position);

		overlay_flags = check_any_overlay_position(g, ID_PEOPLE2, ID_PEOPLE2 - 1);
		if (overlay_flags != 0)
			continue;

		break;
	};
	g.units[ID_PEOPLE2].attack_chance = -1;

	g.units[ID_PEOPLE3].id = ID_PEOPLE3;
	strcpy_s(g.units[ID_PEOPLE3].name, CAPACITY_OF_NAME, NAME_PEOPLE3);
	g.units[ID_PEOPLE3].hp = MAX_HP_OF_PEOPLE;
	while (1) {
		randomize_position(&g.units[ID_PEOPLE3].position);

		overlay_flags = check_any_overlay_position(g, ID_PEOPLE3, ID_PEOPLE3 - 1);
		if (overlay_flags != 0)
			continue;

		break;
	};
	g.units[ID_PEOPLE3].attack_chance = -1;

	g.units[ID_PEOPLE4].id = ID_PEOPLE4;
	strcpy_s(g.units[ID_PEOPLE4].name, CAPACITY_OF_NAME, NAME_PEOPLE4);
	g.units[ID_PEOPLE4].hp = MAX_HP_OF_PEOPLE;
	while (1) {
		randomize_position(&g.units[ID_PEOPLE4].position);

		overlay_flags = check_any_overlay_position(g, ID_PEOPLE4, ID_PEOPLE4 - 1);
		if (overlay_flags != 0)
			continue;

		break;
	};
	g.units[ID_PEOPLE4].attack_chance = -1;

	// Ser servants
	g.units[ID_SERVANT1].attack_chance = -1;
	g.units[ID_SERVANT1].hp = MAX_HP_OF_SERVANT;
	g.units[ID_SERVANT1].id = ID_SERVANT1;
	strcpy_s(g.units[ID_SERVANT1].name, CAPACITY_OF_NAME, NAME_SERVANT1);
	g.units[ID_SERVANT1].position.x = -1;
	g.units[ID_SERVANT1].position.y = -1;

	g.units[ID_SERVANT2].attack_chance = -1;
	g.units[ID_SERVANT2].hp = MAX_HP_OF_SERVANT;
	g.units[ID_SERVANT2].id = ID_SERVANT2;
	strcpy_s(g.units[ID_SERVANT2].name, CAPACITY_OF_NAME, NAME_SERVANT1);
	g.units[ID_SERVANT2].position.x = -1;
	g.units[ID_SERVANT2].position.y = -1;

	g.units[ID_SERVANT3].attack_chance = -1;
	g.units[ID_SERVANT3].hp = MAX_HP_OF_SERVANT;
	g.units[ID_SERVANT3].id = ID_SERVANT3;
	strcpy_s(g.units[ID_SERVANT3].name, CAPACITY_OF_NAME, NAME_SERVANT1);
	g.units[ID_SERVANT3].position.x = -1;
	g.units[ID_SERVANT3].position.y = -1;

	g.units[ID_SERVANT4].attack_chance = -1;
	g.units[ID_SERVANT4].hp = MAX_HP_OF_SERVANT;
	g.units[ID_SERVANT4].id = ID_SERVANT4;
	strcpy_s(g.units[ID_SERVANT4].name, CAPACITY_OF_NAME, NAME_SERVANT1);
	g.units[ID_SERVANT4].position.x = -1;
	g.units[ID_SERVANT4].position.y = -1;

	// Set traps
	while (1) {
		randomize_position(&g.traps[0]);

		overlay_flags = check_any_overlay_position(g, ID_TRAP1, ID_TRAP1 - 1);
		if (overlay_flags != 0)
			continue;

		break;
	}

	while (1) {
		randomize_position(&g.traps[1]);

		overlay_flags = check_any_overlay_position(g, ID_TRAP2, ID_TRAP2 - 1);
		if (overlay_flags != 0)
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
void morph_servant(Unit* ppeople, Unit* pservant) {
	Unit new_servant;

	// In case of wrong argument:
	// Nothing happened.
	if (ppeople->id < ID_PEOPLE1 || ppeople->id > ID_PEOPLE4)
		return;
	if (pservant->id < ID_SERVANT1 || pservant->id > ID_SERVANT4)
		return;

	// When the argument is not a people, it does not happened.
	//switch (ppeople->id) {
	//case ID_PEOPLE1:
	//	new_servant.id = ID_SERVANT1;
	//	strcpy_s(new_servant.name, CAPACITY_OF_NAME, NAME_SERVANT1);
	//	break;
	//case ID_PEOPLE2:
	//	new_servant.id = ID_SERVANT2;
	//	strcpy_s(new_servant.name, CAPACITY_OF_NAME, NAME_SERVANT2);
	//	break;
	//case ID_PEOPLE3:
	//	new_servant.id = ID_SERVANT3;
	//	strcpy_s(new_servant.name, CAPACITY_OF_NAME, NAME_SERVANT3);
	//	break;
	//case ID_PEOPLE4:
	//	new_servant.id = ID_SERVANT4;
	//	strcpy_s(new_servant.name, CAPACITY_OF_NAME, NAME_SERVANT4);
	//	break;
	//default:
	//	return;
	//}

	//// When a people is turned to a servant, HP is initialized.
	//new_servant.hp = MAX_HP_OF_SERVANT;
	//new_servant.attack_chance = -1;
	//new_servant.position.x = ppeople->position.x;
	//new_servant.position.y = ppeople->position.y;

	//*ppeople = new_servant;

	pservant->position.x = ppeople->position.x;
	pservant->position.y = ppeople->position.y;

	ppeople->position.x = -1;
	ppeople->position.y = -1;
	ppeople->hp = 0;
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
	int i = 0;
	int flag_overlay = 0, damage = 0, dimension = 0;

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
	flag_overlay = check_any_overlay_position(*pgame, ID_AKARI, ID_TRAP2);
	if (GET_FLAG(flag_overlay, CHECK_VALUE_VAMPIRE) == 1) {
		// Met vampire
		damage = min(pgame->units[ID_AKARI].hp, pgame->units[ID_VAMPIRE].hp);
		pgame->units[ID_AKARI].hp -= damage;
		pgame->units[ID_VAMPIRE].hp -= damage;

		display(*pgame);

		if (pgame->units[ID_AKARI].hp == 0) {
			if (pgame->units[ID_VAMPIRE].hp != 0) {
				// You lose
				printf("Akari lost!\n");
			}
			else {
				// Draw
				printf("Akari and Vampire killed each other!\n");
			}
			printf("Thank you for playing!");
			exit(0);
		}
		else if (pgame->units[ID_VAMPIRE].hp == 0) {
			// You win
			printf("Akari win!\n");
			printf("Thank you for playing!");
			exit(0);
		}
	}
	if (GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE1) == 1 ||
		GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE2) == 1 ||
		GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE3) == 1 ||
		GET_FLAG(flag_overlay, CHECK_VALUE_PEOPLE4) == 1) {
		// Met a villager
		if (pgame->units[ID_AKARI].hp < MAX_HP_OF_AKARI) {
			pgame->units[ID_AKARI].hp++;
			//display(*pgame);
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
			printf("Akari lost!\n");
			printf("Thank you for playing!");
			exit(0);
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
			printf("Akari lost!\n");
			printf("Thank you for playing!");
			exit(0);
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
			printf("Akari lost!\n");
			printf("Thank you for playing!");
			exit(0);
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
			printf("Akari lost!\n");
			printf("Thank you for playing!");
			exit(0);
		}
	}
	if (GET_FLAG(flag_overlay, CHECK_VALUE_TRAP1) == 1) {
		// Touch a trap
		pgame->traps[0].x = -1;
		pgame->traps[0].y = -1;

		pgame->units[ID_AKARI].hp =
			max(pgame->units[ID_AKARI].hp - 3, 0);

		if (pgame->units[ID_AKARI].hp == 0) {
			display(*pgame);
			printf("Akari lost!\n");
			printf("Thank you for playing!");
			exit(0);
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
			printf("Akari lost!\n");
			printf("Thank you for playing!");
			exit(0);
		}
	}

	if (pgame->turn % 5 == 0) {
		// People's turn
		//  People move per 5 turns.
		for (i = ID_PEOPLE1; i <= ID_PEOPLE4; i++) {
			new_pos.x = prev_pos.x = pgame->units[i].position.x;
			new_pos.y = prev_pos.y = pgame->units[i].position.y;

			// A dead villager doesn't move.
			if (prev_pos.x == -1 || prev_pos.y == -1)
				continue;

			while (1) {
				dimension = rand() % 4;

				switch (dimension) {
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

				if (new_pos.x > -1 && new_pos.x < 8 &&
					new_pos.y > -1 && new_pos.x < 8) {
					break;
				}

				new_pos.x = prev_pos.x;
				new_pos.y = prev_pos.y;
			}

			pgame->units[i].position = new_pos;
			flag_overlay = check_any_overlay_position(*pgame, i, ID_TRAP2);

			// A villager met Akari
			if (GET_FLAG(flag_overlay, CHECK_VALUE_AKARI) == 1) {
				if (pgame->units[ID_AKARI].hp < MAX_HP_OF_AKARI) {
					pgame->units[ID_AKARI].hp++;
				}
			}

			// A villager met Vampire
			if (GET_FLAG(flag_overlay, CHECK_VALUE_VAMPIRE) == 1) {
				morph_servant(&pgame->units[i], &pgame->units[i + 4]);
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
		}

		// Servants' turn
		//  Servants move per 5 turns.
	}

	
	
	// Vampire's turn
	//  Vampire moves per 2 turns.

	
}
