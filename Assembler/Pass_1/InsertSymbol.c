#include "headers.h"

/* 
    This function inserts symbols and their associated memory address location and line number
*/
//@param List - the symbol linked list
//@param Name - the name of the symbol
//@param Addr - the hexadecimal memory address
//@param LineNum - line number of the symbol
void InsertSymbol( SymbolList *list, char Name[7], int Addr, int LineNum ){

    //declare a new Symbol struct
    SymbolList new;
    new = malloc( sizeof( struct Symbol ) ); //allocate the appropriate amount of memory for the symbol
    //memset bc when you malloc a structure this is used to zero out memory from the structure to prevent memory leak
    if(new == NULL) 
    {
        printf("ERROR:\n Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    memset(  new, '\0', sizeof( struct Symbol ) );
    

    

    //adding the properties to the new node in the list 
    new->Address = Addr;
    new->LineNumber = LineNum;

    //strcpy to copy the param name to the new node's name var
    strcpy(  new->Name, Name );

    //make the new node point to the end of the list
    new->Next = NULL;

    
    //check if list is empty
    if( *list == NULL )
    {
        *list = new;
    }
    else
    {
        //make temp == to the header of the list
        SymbolList temp = *list;

        while( temp->Next != NULL )
        {
            temp = temp->Next;
        }
        
        temp->Next = new;//make the end of the list point to the new symbol
    }
    //while the end of the list has not been reached, traverse towards the end
    
        
}
 