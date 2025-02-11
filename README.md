# SIC-Assembler
a repo containing pass one and pass two of a SIC assembler in C

# Assembler Pass 1
 This program is supposed to simulate an assembler's first pass. The program takes two command line arguments and one 
is suposed to be the name of the .sic file that should be opened and read by the program. The program will read each line of
the file and tokenize each line. Each line may have 1 to 3 tokens. If there are three tokens, the first should be the 
symbol, the second is the directive or instruction, and the third is the operand. After each line is successfully tokenized,
the program will determine how much to increment the location address depending on the directive or instruction found.
If a symbol is found, the program should determine if the symbol is already in the symbol list. If not, then the symbol
should be inserted into the list. The symbol name and it's location address are part of each symbol struct. If the file 
contains no errors, then the symbol list is printed. Once printed, the symbol list is deleted by calling free() on each 
node, and the program exits. 
If an error is located at any point, the program prints the error and which at line it was found in the file.
