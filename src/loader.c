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
