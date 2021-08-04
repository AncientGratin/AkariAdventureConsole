#define __AKARIADV_H__
#ifdef __AKARIADV_H__

// Constants of the Dimension
#define WEST 1
#define NORTH 2
#define SOUTH 3
#define EAST 4
#define STAY 0
#define OTHER -1
#define NORTHWEST 21
#define NORTHEAST 24
#define SOUTHWEST 31
#define SOUTHEAST 34

// The capacity of name
#define CAPACITY_OF_NAME 9

// The maximum number of village people in the game
#define MAX_NUMBER_OF_PEOPLE 4

// The maximum number of village people in the game
#define MAX_NUMBER_OF_SERVANTS 4

#define MAX_NUMBER_OF_TRAPS 2

// Size of the field
#define FIELD_WIDTH 8
#define FIELD_HEIGHT 8

#define MAX_HP_OF_AKARI 9
#define MAX_HP_OF_VAMPIRE 9
#define MAX_HP_OF_PEOPLE 1
#define MAX_HP_OF_SERVANT 1
#define ATTACK_CHANCE 5

// Check values of characters:
//  Following numbers are used to check,
//  Such as is there a character or some characters
//  on a particular place.
//  A binary status(TRUE/FALSE) of each character
//  is indicated by a number.
// ex)
//  AKARI		: 0B000000000001
//  VAMPIRE		: 0B000000000010
//  PEOPLE1		: 0B000000000100
//  PEOPLE2		: 0B000000001000
//  PEOPLE3		: 0B000000010000
//  PEOPLE4		: 0B000000100000
//  SERVANT1	: 0B000001000000
//  SERVANT2	: 0B000010000000
//  SERVANT3	: 0B000100000000
//  SERVANT4	: 0B001000000000
//  TRAP1		: 0B010000000000
//  TRAP2		: 0B100000000000
#define CHECK_VALUE_AKARI		0B1
#define CHECK_VALUE_VAMPIRE		0B10
#define CHECK_VALUE_PEOPLE1		0B100
#define CHECK_VALUE_PEOPLE2		0B1000
#define CHECK_VALUE_PEOPLE3		0B10000
#define CHECK_VALUE_PEOPLE4		0B100000
#define CHECK_VALUE_SERVANT1	0B1000000
#define CHECK_VALUE_SERVANT2	0B10000000
#define CHECK_VALUE_SERVANT3	0B100000000
#define CHECK_VALUE_SERVANT4	0B1000000000
#define CHECK_VALUE_TRAP1		0B10000000000
#define CHECK_VALUE_TRAP2		0B100000000000

// ID is pair with check value.
// 2 ^ (ID) = (CHECK_VALUE)
#define ID_AKARI	0
#define ID_VAMPIRE	1
#define ID_PEOPLE1	2
#define ID_PEOPLE2	3
#define ID_PEOPLE3	4
#define ID_PEOPLE4	5
#define ID_SERVANT1	6
#define ID_SERVANT2	7
#define ID_SERVANT3	8
#define ID_SERVANT4	9
#define ID_TRAP1	10
#define ID_TRAP2	11

// Name
#define NAME_AKARI "Akari"
#define NAME_VAMPIRE "Vampire"
#define NAME_PEOPLE1 "People1"
#define NAME_PEOPLE2 "People2"
#define NAME_PEOPLE3 "People3"
#define NAME_PEOPLE4 "People4"
#define NAME_SERVANT1 "Servant1"
#define NAME_SERVANT2 "Servant2"
#define NAME_SERVANT3 "Servant3"
#define NAME_SERVANT4 "Servant4"

// How many are characters in this game?
#define NUMBER_OF_CHARACTERS 10

// Flags of warning
//
// Adjacent a trap	: 0B0001
// Vampire approach	: 0B0010
// Met a servant	: 0B0100
// Met a villager	: 0B1000
#define FLAG_WARNING_TRAP		0B1
#define FLAG_WARNING_VAMPIRE	0B10
#define FLAG_WARNING_SERVANT	0B100
#define FLAG_WARNING_VILLAGER	0B1000

// Get a flag
#define GET_FLAG(flags, flag) flags / flag % 2

typedef struct _Point {
	int x, y;
} Point;

// Unit
typedef struct _Unit {
	int id;
	char name[CAPACITY_OF_NAME];
	int hp;
	Point position;
	int attack_chance;
} Unit;

// The struct for this game
typedef struct _Game {
	Unit units[NUMBER_OF_CHARACTERS];
	Point traps[MAX_NUMBER_OF_TRAPS];	// Position of traps

	// Turn of this game
	int turn;
} Game;

// Declaration of functions
int check_any_overlay_position(Game, int, int);			// Check if there is any overlay position of units by some unit
int check_any_overlay_position_by_point(Game, Point);	// Check if there is any overlay position of units by position
void display(Game);		// Display the game information on the screen.
int get_direction(Point);	// Get the direction from routine.
int get_distance(Point, Point);	// Get distance between 2 points.
int get_track_direction(Unit*, int);	// Decide the direction that Vampire or a servant tracks Akari or a villager.
Point get_vector(Point, Point);	// Get vector between 2 points.
int get_warning_flags(Game);		// Get flags of warning
void init_game(Game*);		// Initialize the game
int input_attack_command();	// Attack command
char input_move_command();	// Move command
void morph_servant(Unit*, Unit*);	// People ===> Servant
void move(Point*, char);	// Move a unit
void play();	// Play the game
void quit();	// Quit
void randomize_position(Point*);	// Randomize the position
void test();
void turn(Game*);	// Execute 1 turn.

#endif