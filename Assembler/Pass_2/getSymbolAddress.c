#include "headers.h"

int getSymbolAddress(SymbolList table, char *label) {
    SymbolList current = table;
    while (current != NULL) {
        if (strcmp(current->name, label) == 0) {
            return current->address;
        }
        current = current->Next;
    }
    return -1; // Undefined symbol
}
