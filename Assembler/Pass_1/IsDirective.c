#include "headers.h"

/*need to compare the candidate string to the list of  known directives and ionstructions. If no match,
the candidate string is a symbol and the function returns 1 */
int IsDirective(char *candidate)
{
    const char *directives[] = {
        "START", "END", "BYTE", "WORD", "RESB", "RESW", "RESR", "EXPORTS", NULL
    };

    //this compares the symbol candidate against the list of known directives
    for(int i = 0; directives[i] != NULL ; i++ ){
        if( strcmp(candidate, directives[i]) == 0 )
        {
            return 1;
        }
    }

    return 0;
}