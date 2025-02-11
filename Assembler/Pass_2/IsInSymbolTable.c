#include "headers.h"
// modify for case-insensitive comparisons
int IsInSymbolTable(SymbolList table, char Test[7]) {
    SymbolList current = table;
    while (current != NULL) {
        if (strcasecmp(current->name, Test) == 0) {
            return 1;
        }
        current = current->Next;
    }
    return 0;
}