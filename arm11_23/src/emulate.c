#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "arm_structure.h";


int main(int argc, char **argv) {
	FILE *fin;

	if((fin = fopen(argv[argc - 1], "r")) == NULL) {
		perror("Error opening file.txt!");
		exit(EXIT_FAILURE);
	}

	fseek(fin, 0, SEEK_END);
	int size = ftell(fin);
	rewind(fin);

	fread(ARM.memory, size, 1, fin);
	fclose(fin);

	pipeline();

	return EXIT_SUCCESS;
}
