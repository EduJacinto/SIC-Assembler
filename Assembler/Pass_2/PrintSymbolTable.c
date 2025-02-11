#include "headers.h"

// Function to print the symbol table
void PrintSymbolTable(SymbolList list) {
    printf("\nSymbol Table:\n");
    printf("Label\tAddress\n");
    SymbolList current = list;
    while (current != NULL) {
        printf("%s\t%04X\n", current->name, current->address);
        current = current->Next;
    }
}