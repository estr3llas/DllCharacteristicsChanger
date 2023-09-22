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

SelectedOptions processOptions(int argc, char* argv[]);