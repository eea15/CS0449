// Emma Akbari (eea21)
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void get_line(char* input, int size) {
	fgets(input, size, stdin);
	int len = strlen(input);

	// this line of code *might* have a bug in it.
	// think about what value of "len" would cause a problem.
	input[len - 1] = '\0';
}

// args are user input string and its length
// function to convert string to all uppercase
void uppercase(char* userStr, int len) {

	for(int i = 0; i < len; i++)
	{
		userStr[i] = toupper(userStr[i]);
	}
}

int main() {
	//for user input
	char userIn[100];
	//ask user to enter string and convert to uppercase
	printf("Type something in: ");
	get_line(userIn, 100);
	uppercase(userIn, strlen(userIn));
	printf("Now in uppercase: %s\n", userIn);
	return 0;
}