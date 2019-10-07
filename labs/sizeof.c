#include <stdio.h>

int main() {

	//64-bit, 32-bit
	int x = 10;
	printf("sizeof(x) = %d\n", (int)sizeof(x)); //4
	printf("sizeof(int) = %d\n", (int)sizeof(int)); //4
	printf("sizeof(char) = %d\n", (int)sizeof(char)); //1
	printf("sizeof(short) = %d\n", (int)sizeof(short)); //2
	printf("sizeof(unsigned int) = %d\n", (int)sizeof(unsigned int)); //4
	printf("sizeof(long) = %d\n", (int)sizeof(long)); //8, 4
	printf("sizeof(long long) = %d\n", (int)sizeof(long long)); //8
	printf("sizeof(float) = %d\n", (int)sizeof(float)); //4
	printf("sizeof(double) = %d\n", (int)sizeof(double)); //8
	printf("sizeof(long double) = %d\n", (int)sizeof(long double)); //16, 12
	printf("sizeof(int*) = %d\n", (int)sizeof(int*)); //8, 4
	printf("sizeof(&x) = %d\n", (int)sizeof(&x)); //8, 4
	printf("sizeof(int**) = %d\n", (int)sizeof(int**)); //8, 4
	printf("sizeof(double*) = %d\n", (int)sizeof(double*)); //8, 4
	printf("sizeof(char*) = %d\n", (int)sizeof(char*)); //8, 4

	//the unsigned does not change the size
	//pointers are all the same size 8B (memory addresses are 8B)
	//pointer type does NOT change the size

	char a[10];
	int b[10];
	int* c = b;

	printf("sizeof(a) = %d\n", (int)sizeof(a)); //10
	printf("sizeof(b) = %d\n", (int)sizeof(b)); //40
	printf("sizeof(c) = %d\n", (int)sizeof(c)); //8, 4
	printf("sizeof(&a) = %d\n", (int)sizeof(&a)); //8, 4
	printf("sizeof(&b) = %d\n", (int)sizeof(&b)); //8, 4

	//long, long double, pointers, and addresses change with CPU
}