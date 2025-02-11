# SIC Assembler
a repo containing pass one and pass two of a SIC assembler in C

# SIC Assembler - Pass 1
## Overview
This program implements the first pass of an assembler for the Simplified Instructional Computer (SIC) architecture. The program processes a SIC assembly source file, extracts symbols, and builds the symbol table while computing location addresses.
## Features
* Reads and tokenizes each line of a SIC assembly source file
* Supports symbol definitions directives, and instructions
* Computes location addresses based on the instruction or directive
* Builds a symbol table, ensuring no duplicate symbols exist
* Prints the symbol table if no errors are found
* Handles errors by printing a message and stopping execution
* Frees allocated memory before exiting

## Usage

### Compilation

Compile the program using the command:
>
`make`

### Execution

Run the program using the command:
>
`./project1 <filename>`
>
where <filename> is one of the test files provided in the project folder.
### Output
* If no errors are found, the program prints the symbol table and exits.
* If an error is encountered, the program prints an error message and terminates without printing the symbol table,
##Error Handling
The program detects and reports errors such as:
>
* Duplicate symbols
* Invalid symbols
* syntax errors in the source file
* Memory limitations
##Memory Management
* The symbol table is stored as alinked list
* Before exiting, regardless of success or failure, the program frees all allocated memory

# SIC Assembler - Pass 2
## Overview
This project implements the second pass of an assembler for the Simplified Instructional Computer (SIC) architecture. The assembler processes a SIC assembly source file, performs necessary validation, and generates anobject file in the ~.sic.obj file format.
## Features
* Reads a SIC assembly file
* Uses the symbol table generated in the first pass of the assembler to resolve addresses
* Generates SIC object code
* Validates assembly and prints an error message if an invalid instruction is detected
* Produces an object file only if the assembly completes without errors
## Usage

### Compilation
Compile the project using:
>
`make`
>
in the terminal window
###Execution
Run the project by using the command:
>
`./project2 <filename>`
>
where filename is one of the test files provided in the project folder.
###Output
* if assembly completes successfully, an object file is created with .obj appended to the input file name.
>
* if an error is found, the assembler prints an error message and does not generate an object file
## Error Handling
The assembler dtects and reports errors that cannot be validated in Pass one, including but not limited to:
>
* Undefined symbols
* Invalid opcodes
* Addressing errors
* Format violations
>
If any of these error are encountered, the assembler halts ad prints a detailed error message
## License
This project is for education purposes
