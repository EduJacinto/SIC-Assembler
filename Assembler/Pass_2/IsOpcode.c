#include "headers.h"

int IsOpcode(string token) {
    const char *opcodes[] = {
            "ADD", "AND", "COMP", "DIV", "J", "JEQ", "JGT", "JLT", "JSUB",
            "LDA", "LDCH", "LDL", "LDX", "MUL", "OR", "RD", "RSUB", "STA",
            "STCH", "STL", "STSW", "STX", "SUB", "TD", "TIX", "WD"
    };
    int num_opcodes = sizeof(opcodes) / sizeof(opcodes[0]);

    for (int i = 0; i < num_opcodes; i++) {
        if (strcmp(token, opcodes[i]) == 0) {
            return 1;
        }
    }
    return 0;
}
