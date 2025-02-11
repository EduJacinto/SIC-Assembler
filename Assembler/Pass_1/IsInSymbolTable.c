#include "headers.h"
/*
    This function checks if a symbol exists within the symbol table and it returns a result of type int
*/
//@param table - the head of the linked list known as the SymbolList
//@param Test - the symbol. Function checks if Test is already in the linked list
int IsInSymbolTable( SymbolList list, char Test[7]) {

    //temp variable that will be set equal to the current node in the list
    SymbolList temp = list;//set temp equal to the head of the list

    //while end of the list has not been reached, loop
    while( temp != NULL )   
    {
        //if the strings are the same enter condtional, set result to true, and exit the while loop
        if ( strcmp( temp->Name, Test) == 0 ) 
        {
            return 1;
        }

        //set temp equal to the next node in the list
        temp = temp->Next;
    }

    //if this point is reached then the symbol is not in the list so add it to the list
    return 0;
}

