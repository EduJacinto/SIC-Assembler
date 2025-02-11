#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define the Symbol structure
typedef struct Symbol {
    char Name[7];          // Symbol name (max 6 characters)
    int Address;           // Address of the symbol
    int LineNumber;        // Line number where the symbol was defined
    struct Symbol *Next;   // Pointer to the next symbol in the list
} SYMBOL;

// Define SymbolList as a pointer to SYMBOL
typedef SYMBOL* SymbolList;

// Function prototypes
void PrintSymbolTable(SymbolList list);
int IsInSymbolTable( SymbolList list, char Test[7]);
void InsertSymbol( SymbolList *list, char Name[7], int Addr, int LineNum );
void DeleteList(SymbolList list);
int IsInstruction(char *candidate);
int IsDirective(char *candidate);
int IsValidSymbol(char *symbol);

#endif // HEADERS_H