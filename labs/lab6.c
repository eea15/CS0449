// Emma Akbari (eea21)
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef bool (*PREDICATE)(const void*);

// ------------------------------------------------------

int filter(void* output, const void* input, int length, int item_size, PREDICATE pred) {
	// tracks how many items in output
	int counter = 0;

	// iterate through input array
	for(int i = 0; i < length; i++) {
		// if pred is true, copy item
		if(pred(input)) {
			memcpy(output, input, item_size);
			counter++; 
			//increment output pointer only if pred true
			output+=item_size; 
		}

		// increment input pointer every time
		input+=item_size;
	}

	return counter;
}

// interpret param as float pointer & return true if < 50
bool less_than_50_f(const void* p) {
	// cast pointer then dereference
	float num = *(const float*)p;
	// return true if less than 50
	return num < 50;
}

// interpret param as float pointer & return true if whole number is even
bool even_whole_number(const void* p) {
	// cast pointer then dereference
	float num = *(const float*)p;
	int num_int = (int)num;
	// return true if even
	return num_int%2 == 0;
}

// interpret param as char pointer & return true if < 50
bool less_than_50_b(const void* p) {
	// cast pointer then dereference
	char num = *(const char*)p;
	// return true if < 50
	return num < 50;
}

// ------------------------------------------------------
// you shouldn't change the stuff below here.
// you can for testing, but please put it back the way it was before you submit.

#define NUM_VALUES 10

float float_values[NUM_VALUES] = {
	31.94, 61.50, 36.10,  1.00,  6.35,
	20.76, 69.30, 19.60, 79.74, 51.29,
};

char byte_values[NUM_VALUES] = {
	31, 61, 36,  1,  6,
	20, 69, 19, 79, 51,
};

int main() {
	// test it on floats...
	float filtered[NUM_VALUES];
	int filtered_len = filter(filtered, float_values, NUM_VALUES, sizeof(float), &less_than_50_f);

	printf("there are %d floats less than 50:\n", filtered_len);

	for(int i = 0; i < filtered_len; i++)
		printf("\t%.2f\n", filtered[i]);

	// test it on the same float array, but with a different predicate
	filtered_len = filter(filtered, float_values, NUM_VALUES, sizeof(float), &even_whole_number);

	printf("there are %d floats that have an even whole part:\n", filtered_len);

	for(int i = 0; i < filtered_len; i++)
		printf("\t%.2f\n", filtered[i]);

	// and then test it on a char array.
	char filtered_b[NUM_VALUES];
	filtered_len = filter(filtered_b, byte_values, NUM_VALUES, sizeof(char), &less_than_50_b);

	printf("there are %d bytes less than 50:\n", filtered_len);

	for(int i = 0; i < filtered_len; i++)
		printf("\t%d\n", filtered_b[i]);

	return 0;
}