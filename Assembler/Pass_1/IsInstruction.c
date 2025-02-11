#include "headers.h"

/*need to compare the candidate string to the list of  known directives and ionstructions. If no match,
the candidate string is a symbol and the function returns 1 */
int IsInstruction(char *candidate)
{
   
    const char *instructions[] = {
        "ADD", "ADDF", "ADDR", "AND", "CLEAR", "COMP", "COMPF", "COMPR", "DIV", "DIVF", "DIVR", "FIX", "FLOAT", "HIO", "J",
        "JEQ", "JGT", "JLT", "JSUB", "LDA", "LDB", "LDCH", "LDF", "LDL", "LDS", "LDT", "LDX", "LPS", "MUL", "MULF", "MULR",
        "NORM", "OR", "RD", "RMO", "RSUB", "SHIFTL", "SHIFTR", "SIO", "SSK", "STA", "STB", "STCH", "STF", "STI", "STL", "STS",
        "STSW", "STT", "STX", "SUB", "SUBF", "SUBR", "SVC", "TD", "TIO", "TIX", "TIXR", "WD", NULL
    };

    //this compares the symbol candidate against the list of known instructions
    for( int i = 0; instructions[i] != NULL ; i++ )
    {
        if( strcmp(candidate, instructions[i]) == 0 )
        {
            return 1;//true it is an instruction
        }
    }

    return 0;//false, not an instruction
}