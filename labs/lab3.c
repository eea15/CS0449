// Emma Akbari (eea21)
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
	char name[16];
	double price;
	int stock;
} Food;


bool streq(const char* a, const char* b) {
	return strcmp(a, b) == 0;
}

void print_food(Food f) {
	printf("%s ($%.2f each, %d in stock)\n", f.name, f.price, f.stock);
}

FILE* open_database() {
	// open and return file if not NULL
	FILE* f = fopen("food.db", "rb+");

	if(f == NULL) {
		printf("Error: could not open file\n");
		exit(1);
	} 

	return f; 
}

int count_foods(FILE* db) {
	// len = byte length of file
	fseek(db, 0, SEEK_END);
	int len = ftell(db);
	fseek(db, 0, SEEK_SET);

	// check if len is a multiple of sizeof(Food)
	if(len%(sizeof(Food)) != 0) {
		printf("This doesn't look like a valid food database.\n");
		exit(1);
	}

	return len/=(sizeof(Food)); // return num foods
}

Food read_food(FILE* db, int i) {
	// calculate pos of i'th Food, then read & return
	int pos = i*32;
	fseek(db, pos, SEEK_SET);
	Food food;
	fread(&food, sizeof(food), 1, db);

	return food;
}

void add_food(FILE* db, Food food) {
	// add new food to end of file
	fseek(db, 0, SEEK_END);
	fwrite(&food, sizeof(food), 1, db);
}

int main(int argc, char** argv) {
	FILE* db = open_database();
	int num_foods = count_foods(db);
	Food next_food; // food to be read
	
	if(argc == 2 && streq(argv[1], "print")) {
		printf("There are %d foods in the database.\n", num_foods);
		// print all foods in db
		for(int i = 0; i < num_foods; i++) {
			next_food = read_food(db, i);
			printf("Food #%d: ", i);
			print_food(next_food);
		}
	} else if(argc == 3 && streq(argv[1], "print")) {
		// parse index out of argv[2]
		int food_index;
		sscanf(argv[2], "%d", &food_index);

		// error if index out of range
		if(food_index < 0 || food_index > (num_foods-1)) {
			printf("Invalid index!\n");
		} else {
			// print food at user input index
			next_food = read_food(db, food_index);
			printf("Food #%d: ", food_index);
			print_food(next_food);
		}
	} else if(argc == 5 && streq(argv[1], "add")) {
		Food newFood;

		sscanf(argv[3], "%lf", &newFood.price); // parse double out of argv[3]
		sscanf(argv[4], "%d", &newFood.stock); // parse int out of argv[4]

		if((int)strlen(argv[2]) > 16) {
			printf("Name is too long!\n"); // checks for valid food length
		} else if(newFood.price <= 0) {
			printf("Price must be at least $0.01!\n"); // checks for valid price range
		} else if(newFood.stock < 0) {
			printf("Stock must be at least 0!\n"); // checks for valid stock range
		} else {
			strcpy(newFood.name, argv[2]); // copy new name into food
			add_food(db, newFood); // add food to end of file
		}
	} else {
		printf("You can run this three ways:\n");
		printf("  ./lab3 print\n");
		printf("      prints all foods in food.db.\n");
		printf("  ./lab3 print n\n");
		printf("      prints food at index n from food.db.\n");
		printf("  ./lab3 add name price stock\n");
		printf("      adds a new food to food.db.\n");
	}

	if(db != NULL)
		fclose(db);
    return 0;
}