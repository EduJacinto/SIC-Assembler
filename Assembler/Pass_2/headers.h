#ifndef ASSEMBLER_HEADERS_H
#define ASSEMBLER_HEADERS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define SymbolList struct symbol*
#define string char*

struct symbol {
    int address;
    int linenumber;
    char name[7];
    struct symbol* Next;
};

// Function prototypes
void PrintSymbolTable(SymbolList list);
bool isValidSymbol(string symbol);
void DeleteList(SymbolList table);
void InsertSymbol(SymbolList* table, char Name[7], int Addr, int LineNum);
int  IsInSymbolTable(SymbolList table, char Test[7]);
int  IsOpcode(string token);
bool isNumeric(string str);
string trim_whitespace(char *str);
void ProcessFilePass2(string filename, SymbolList SymbolTable);
int getSymbolAddress(SymbolList table, char *label);
int getOpcodeValue(char *opcode);

#endif //ASSEMBLER_HEADERS_H
