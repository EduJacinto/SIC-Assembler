# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra

# Source Files
SRC = p1.c p2.c IsOpcode.c trim_whitespace.c isValidSymbol.c isNumeric.c InsertSymbol.c PrintSymbolTable.c IsInSymbolTable.c getSymbolAddress.c getOpcodeValue.c DeleteList.c

# Executable Name
EXEC = project2

# Default Target
all: $(EXEC)

# Build the Executable
$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

# Clean up all compiled files and executables
clean:
	rm -f $(EXEC) *.o *.obj *.txt

.PHONY: all clean
