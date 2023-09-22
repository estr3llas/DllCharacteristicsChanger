#pragma once

#include <string.h>

#define OPTION_SET_ASLR "-sA"
#define OPTION_REMOVE_ASLR "-rA"
#define OPTION_SET_DEP "-sD"
#define OPTION_REMOVE_DEP "-rD"
#define OPTION_SET_FINTEGRITY "-sI"
#define OPTION_REMOVE_FINTEGRITY "-rI"
#define OPTION_SHOW_HELP "--help"

typedef struct {
	int setASLR;
	int removeASLR;
	int setDEP;
	int removeDEP;
	int setFINTEGRIY;
	int removeFINTEGRITY;
	int showHelp;
} SelectedOptions;

SelectedOptions processOptions(int argc, char* argv[]) {
	SelectedOptions selectedOptions;
	memset(&selectedOptions, 0, sizeof(SelectedOptions));

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], OPTION_SET_ASLR) == 0) {
			selectedOptions.setASLR = 1;
		}
		else if (strcmp(argv[i], OPTION_REMOVE_ASLR) == 0) {
			selectedOptions.removeASLR = 1;
		}
		else if (strcmp(argv[i], OPTION_SET_DEP) == 0) {
			selectedOptions.setDEP = 1;
		}
		else if (strcmp(argv[i], OPTION_REMOVE_DEP) == 0) {
			selectedOptions.removeDEP = 1;
		}
		else if (strcmp(argv[i], OPTION_SET_FINTEGRITY) == 0) {
			selectedOptions.setFINTEGRIY = 1;
		}
		else if (strcmp(argv[i], OPTION_REMOVE_FINTEGRITY) == 0) {
			selectedOptions.removeFINTEGRITY = 1;
		}
		else if (strcmp(argv[i], OPTION_SHOW_HELP) == 0) {
			selectedOptions.showHelp = 1;
		}
	}

	return selectedOptions;
}