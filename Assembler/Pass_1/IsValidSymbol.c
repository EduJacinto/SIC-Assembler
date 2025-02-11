#include "headers.h"

int IsValidSymbol(char *symbol){

    //if the symbol does not start with alpha character return false
    if( !isalpha(symbol[0]) )
    {
        return 0;
    }

    //if the symbol is longer than 6 characters return false
    if( strlen(symbol) > 6 )
    {
        return 0;
    }

    //check if the symbol candidate contains any of the unacceptable characters
    for( int i = 0; i < strlen(symbol); i++ )
    {
        if( strchr(" $!=+-()@", symbol[i]) != NULL )
        {
            return 0;
        }
    }

    return 1;//returns true, this means the symbol is valid

}