#include "../include/loader.h"
#include <string.h>

GameDefinition *choose_patience(char *folder) {
	GameDefinition *r;
	DIR *d = opendir(folder);
	if (d == NULL) {
		printf("%s folder not found", folder);
		GameDefinition *empty = malloc(sizeof(*empty));
		empty->game_name[0] = '\0';
		return empty;
	}
	char files[50][256];
	int count = list_options(d, files);
	int option = input_patience(count);
	r = load_option(folder, files, option);
	return r;
}

GameDefinition *load_option(char *folder, char files[][256], int option) {
	char path[256];
	sprintf(path, "%s/%s", folder, files[option - 1]);
	GameDefinition *g = load_patience(path);
	printf("loading %s for you <3", path);
	strcpy(g->game_name, files[option - 1]);
	return g;
}

int list_options(DIR *d, char (*files)[256]) {
	struct dirent *entry;
	int count = 0;
	while ((entry = readdir(d)) != NULL) {
		if (entry->d_name[0] != '.') {
			strcpy(files[count], entry->d_name);
			printf("%d. %s\n", count + 1, entry->d_name);
			count++;
		}
	}
	return count;
}

int input_patience(int count) {
	int option = 0;
	char buffer[128];
	int bflag = 0;
	printf("What game do you wanna play?\n Choose a number\n");
	while (!bflag) {
		if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
			if (sscanf(buffer, "%d", &option) == 1 && option >= 1 &&
				option <= count)
				bflag = 1;
			else
				printf("Not a valid option. Please try again but this time a "
					   "number between 1 and %d\n",
					   count);
		}
	}
	return option;
}
