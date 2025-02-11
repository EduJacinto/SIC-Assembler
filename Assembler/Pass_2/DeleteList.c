#include "headers.h"
void DeleteList(SymbolList table) {
    SymbolList current = table;
    while (current != NULL) {
        SymbolList temp = current;
        current = current->Next;
        free(temp);
    }
}