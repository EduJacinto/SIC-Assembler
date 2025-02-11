#include "headers.h"

/*
    This function deletes the contents of the list in order to free the memory allocated for the list.
*/

void DeleteList( SymbolList list )
{
    if( list == NULL )
    {
        return ;
    }
    
     
    //recursive call to this method, loads the stack until the end of the list
    //then frees memory from the tail until the stack is done being emptied
    DeleteList(list->Next);
    free(list);

    return;

}