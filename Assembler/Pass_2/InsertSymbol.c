#include "headers.h"

void InsertSymbol(SymbolList* table, char Name[7], int Addr, int LineNum) {
    for (int i = 0; Name[i]; i++) {
        Name[i] = toupper(Name[i]);
    }

    SymbolList newSymbol = (SymbolList)malloc(sizeof(struct symbol));
    strcpy(newSymbol->name, Name);
    newSymbol->address = Addr;
    newSymbol->linenumber = LineNum;
    newSymbol->Next = NULL; // Since this will be the last node

    if (*table == NULL) {
        // If the list is empty, set newSymbol as the head
        *table = newSymbol;
    } else {
        // Traverse to the end of the list
        SymbolList current = *table;
        while (current->Next != NULL) {
            current = current->Next;
        }
        // Append the new symbol at the end
        current->Next = newSymbol;
    }
}