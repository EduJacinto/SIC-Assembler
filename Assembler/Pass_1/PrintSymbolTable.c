#include "headers.h"

/*
    this function prints the contents of the symbol list.
*/

//@param list - head of the linked list containing the symbols
void PrintSymbolTable( SymbolList list) {

    //temp variable that will hold the current node data
    SymbolList node = list;

   
    //while not at the end of list
    while( node != NULL )
    {
        //print the list
        printf("%s\t%04x\n", node->Name, node->Address );

        //move on the next node
        node = node->Next;
        
    }
    

}