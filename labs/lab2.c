// Emma Akbari (eea21)

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

void get_line(char* buffer, int size) {
	fgets(buffer, size, stdin);
	int len = strlen(buffer);
	// this is a little more robust than what we saw in class.
	if(len != 0 && buffer[len - 1] == '\n')
		buffer[len - 1] = '\0';
}

// returns 1 if the two strings are equal, and 0 otherwise.
int streq(const char* a, const char* b) {
	return strcmp(a, b) == 0;
}

// returns 1 if the two strings are equal ignoring case, and 0 otherwise.
// so "earth" and "Earth" and "EARTH" will all be equal.
int streq_nocase(const char* a, const char* b) {
	// hohoho aren't I clever
	for(; *a && *b; a++, b++) if(tolower(*a) != tolower(*b)) return 0;
	return *a == 0 && *b == 0;
}

// planet weight conversion function
// returns what you weigh on that planet or -1 if it's not a planet
float weight_on_planet(const char* planet_name, int user_weight){

	float newWeight = -1; // weight to be returned

	if(streq_nocase(planet_name, "mercury"))
	{
		newWeight = user_weight*(0.38);
	}
	else if(streq_nocase(planet_name, "venus"))
	{
		newWeight = user_weight*(0.91);
	}
	else if(streq_nocase(planet_name, "mars"))
	{
		newWeight = user_weight*(0.38);
	}
	else if(streq_nocase(planet_name, "jupiter"))
	{
		newWeight = user_weight*(2.54);
	}
	else if(streq_nocase(planet_name, "saturn"))
	{
		newWeight = user_weight*(1.08);
	}
	else if(streq_nocase(planet_name, "uranus"))
	{
		newWeight = user_weight*(0.91);
	}
	else if(streq_nocase(planet_name, "neptune"))
	{
		newWeight = user_weight*(1.19);
	}

	return newWeight;
}

int main() {

	char weight[100]; // user input weight
	int weightInt;
	char planet[100]; // user input planet
	float scaledWeight; 

	printf("How much do you weigh?\n");
	get_line(weight, sizeof(weight)); 
	// parse number out of weight and put into weightInt
	sscanf(weight, "%d", &weightInt);
	
	// loop runs unless user types exit
	while(true){
		printf("What planet do you wanna go to ('exit' to exit)?\n");
		get_line(planet, sizeof(planet));

		if(streq(planet, "exit"))
		{
			break;
		}
		else if(streq_nocase(planet, "earth"))
		{
			printf("You're already there lol\n");
		}
		else
		{
			scaledWeight = weight_on_planet(planet, weightInt);
			
			if(scaledWeight < 0)
			{
				printf("That's not a planet!!\n");
			}
			else
			{
				printf("You'd weigh %.2f there\n", scaledWeight);
			}
		}
	} // end while

	return 0;
}