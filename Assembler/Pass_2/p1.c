#include "headers.h"

int main(int argc, string argv[]) {
    FILE *fp;
    char line[1024];

    SymbolList SymbolTable = NULL;

    int locationCntr = 0;    // Location Counter
    int startAddress = 0;
    int lineNum = 0;         // Line number counter for error handling
    bool hasError = false;   // Error tracking flag
    bool pass1Only = false;  // Flag to indicate if only Pass 1 should be performed

    // Constants for SIC memory limits
    const int SIC_MEMORY_MAX_ADDR = 0x7FFF;

    // Command-line argument parsing
    string filename = NULL;

    if (argc < 2 || argc > 3) {
        printf("Usage: %s [--pass1only] <filename>\n OR %s <filename> [--pass1only] \n", argv[0], argv[0]);
        return EXIT_FAILURE;
    }

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--pass1only") == 0) {
            pass1Only = true;
        } else {
            filename = argv[i];
        }
    }

    if (filename == NULL) {
        printf("ERROR: No source file specified.\n");
        return EXIT_FAILURE;
    }

    fp = fopen(filename, "r"); // Opening the file in read mode

    if (fp == NULL) {
        printf("ERROR: The file %s could not be opened for reading.\n", filename);
        return EXIT_FAILURE;
    }

    // First pass: Build the symbol table and check for errors
    while (fgets(line, 1024, fp) != NULL) {
        lineNum++; // Increment line number for error reporting

        // Trim leading and trailing whitespace
        char *trimmed_line = trim_whitespace(line);

        // ERROR: blank lines
        if (strlen(trimmed_line) < 1 || trimmed_line[0] == '\n') {
            printf("ASSEMBLY ERROR: ");
            printf("%s ", line);
            printf("Line %d: Blank lines are not allowed ", lineNum);
            hasError = true;
            break;
        }

        // Skip comment lines without printing
        if (trimmed_line[0] == '#') {
            continue;
        }

        // Parse the line into label, opcode, operand
        string label = NULL;
        string opcode = NULL;
        string operand = NULL;

        string token = NULL;
        string rest = trimmed_line;

        // Get the first token
        token = strtok_r(rest, " \t\n", &rest);

        if (token == NULL) {
            continue; // Skip empty lines
        }

        // Check if the first token is an opcode or label
        if (IsOpcode(token) || strcmp(token, "START") == 0 || strcmp(token, "END") == 0 ||
            strcmp(token, "WORD") == 0 || strcmp(token, "RESW") == 0 || strcmp(token, "RESB") == 0 ||
            strcmp(token, "BYTE") == 0) {
            // No label present
            opcode = token;
        } else {
            // Label is present
            label = token;
            opcode = strtok_r(NULL, " \t\n", &rest);
        }

        // Get the operand
        if (opcode != NULL) {
            operand = strtok_r(NULL, "\n", &rest);
            if (operand != NULL) {
                // Trim leading and trailing whitespace from operand
                operand = trim_whitespace(operand);
            }
        }

        // Validate operand if opcode requires a symbol operand
        if (operand != NULL && strlen(operand) > 0 && IsOpcode(opcode)) {
            // Exclude constants (numeric values)
            if (!isNumeric(operand)) {
                char *operandCopy = strdup(operand);
                char *token_op = strtok(operandCopy, ",");
                while (token_op != NULL) {
                    token_op = trim_whitespace(token_op);
                    if (strcmp(token_op, "X") != 0 && strcmp(token_op, "A") != 0 && strcmp(token_op, "L") != 0 &&
                        strcmp(token_op, "B") != 0 && strcmp(token_op, "S") != 0 && strcmp(token_op, "T") != 0 &&
                        strcmp(token_op, "F") != 0) {
                        if (!isValidSymbol(token_op)) {
                            printf("ASSEMBLY ERROR: ");
                            printf("%s ", line);
                            printf("Line %d: Invalid operand symbol '%s'\n", lineNum, token_op);
                            hasError = true;
                            free(operandCopy);
                            break;
                        }
                    }
                    token_op = strtok(NULL, ",");
                }
                free(operandCopy);
                if (hasError) {
                    break;
                }
            }
        }

        // Handle START directive
        if (opcode != NULL && strcmp(opcode, "START") == 0) {
            if (operand != NULL) {
                sscanf(operand, "%x", &startAddress); // Convert operand to hex integer
                if (startAddress > SIC_MEMORY_MAX_ADDR) {
                    printf("ASSEMBLY ERROR: ");
                    printf("%s ", line);
                    printf("Line %d: Starting address %X exceeds SIC memory capacity\n", lineNum, startAddress);
                    hasError = true;
                    break;
                }
                locationCntr = startAddress;
                if (label != NULL) {
                    if (!isValidSymbol(label)) {
                        printf("ASSEMBLY ERROR: ");
                        printf("%s ", line);
                        printf("Line %d: Invalid symbol '%s'\n", lineNum, label);
                        hasError = true;
                        break;
                    }
                    if (IsInSymbolTable(SymbolTable, label)) {
                        printf("ASSEMBLY ERROR: ");
                        printf("%s ", line);
                        printf("Line %d: Duplicate symbol '%s'\n", lineNum, label);
                        hasError = true;
                        break;
                    } else {
                        InsertSymbol(&SymbolTable, label, locationCntr, lineNum);
                    }
                }
            } else {
                printf("ASSEMBLY ERROR: ");
                printf("%s ", line);
                printf("Line %d: No operand for START directive\n", lineNum);
                hasError = true;
                break;
            }
            continue;
        }

        // If there is a label, insert it into the symbol table
        if (label != NULL) {
            if (!isValidSymbol(label)) {
                printf("ASSEMBLY ERROR: ");
                printf("%s ", line);
                printf("Line %d: Invalid symbol '%s'\n", lineNum, label);
                hasError = true;
                break;
            }
            if (IsInSymbolTable(SymbolTable, label)) {
                printf("ASSEMBLY ERROR: ");
                printf("%s ", line);
                printf("Line %d: Duplicate symbol '%s'\n", lineNum, label);
                hasError = true;
                break;
            } else {
                InsertSymbol(&SymbolTable, label, locationCntr, lineNum);
            }
        }

        // Update LOCCTR based on opcode
        if (opcode != NULL) {
            if (strcmp(opcode, "WORD") == 0) {
                if (operand != NULL) {
                    if (isNumeric(operand)) {
                        long long value = strtoll(operand, NULL, 0);
                        if (value < -524288 || value > 524287) {
                            printf("ASSEMBLY ERROR: ");
                            printf("%s ", line);
                            printf("Line %d: WORD constant '%s' exceeds 24-bit limitation\n", lineNum, operand);
                            hasError = true;
                            break;
                        }
                    }
                } else {
                    printf("ASSEMBLY ERROR: ");
                    printf("%s ", line);
                    printf("Line %d: Missing operand for WORD\n", lineNum);
                    hasError = true;
                    break;
                }
                locationCntr += 3;
            } else if (strcmp(opcode, "RESW") == 0) {
                if (operand != NULL) {
                    locationCntr += 3 * atoi(operand);
                } else {
                    printf("ASSEMBLY ERROR: ");
                    printf("%s ", line);
                    printf("Line %d: Missing operand for RESW\n", lineNum);
                    hasError = true;
                    break;
                }
            } else if (strcmp(opcode, "RESB") == 0) {
                if (operand != NULL) {
                    locationCntr += atoi(operand);
                } else {
                    printf("ASSEMBLY ERROR: ");
                    printf("%s ", line);
                    printf("Line %d: Missing operand for RESB\n", lineNum);
                    hasError = true;
                    break;
                }
            } else if (strcmp(opcode, "BYTE") == 0) {
                if (operand != NULL) {
                    if (operand[0] == 'X' && operand[1] == '\'') {
                        char *hexString = strdup(operand + 2);
                        char *endQuote = strchr(hexString, '\'');
                        if (endQuote != NULL) {
                            *endQuote = '\0';
                            bool validHex = true;
                            for (char *ptr = hexString; *ptr != '\0'; ptr++) {
                                if (!isxdigit(*ptr)) {
                                    validHex = false;
                                    break;
                                }
                            }
                            if (!validHex) {
                                printf("ASSEMBLY ERROR:  ");
                                printf("%s  ", line);
                                printf("Line %d: Invalid hexadecimal constant '%s'\n", lineNum, operand);
                                hasError = true;
                                free(hexString);
                                break;
                            }
                            int length = strlen(hexString);
                            locationCntr += (length + 1) / 2;
                            free(hexString);
                        } else {
                            printf("ASSEMBLY ERROR: ");
                            printf("%s ", line);
                            printf("Line %d: Missing closing quote in BYTE operand '%s'\n", lineNum, operand);
                            hasError = true;
                            free(hexString);
                            break;
                        }
                    } else if (operand[0] == 'C' && operand[1] == '\'') {
                        char *charString = strdup(operand + 2);
                        char *endQuote = strchr(charString, '\'');
                        if (endQuote != NULL) {
                            *endQuote = '\0';
                            int length = strlen(charString);
                            locationCntr += length;
                            free(charString);
                        } else {
                            printf("ASSEMBLY ERROR:");
                            printf("%s", line);
                            printf("Line %d: Missing closing quote in BYTE operand '%s'\n", lineNum, operand);
                            hasError = true;
                            free(charString);
                            break;
                        }
                    } else {
                        printf("ASSEMBLY ERROR:");
                        printf(" %s ", line);
                        printf("Line %d: Invalid operand for BYTE\n", lineNum);
                        hasError = true;
                        break;
                    }
                } else {
                    printf("ASSEMBLY ERROR:");
                    printf("%s", line);
                    printf("Line %d: Missing operand for BYTE\n", lineNum);
                    hasError = true;
                    break;
                }
            } else if (strcmp(opcode, "END") == 0) {
                break;
            } else if (IsOpcode(opcode)) {
                locationCntr += 3;
            } else {
                printf("ASSEMBLY ERROR:");
                printf("%s", line);
                printf("Line %d: Invalid opcode '%s'\n", lineNum, opcode);
                hasError = true;
                break;
            }
        } else {
            printf("ASSEMBLY ERROR:");
            printf("%s", line);
            printf("Line %d: Missing opcode\n", lineNum);
            hasError = true;
            break;
        }

        if (locationCntr > SIC_MEMORY_MAX_ADDR) {
            printf("ASSEMBLY ERROR:");
            printf("%s", line);
            printf("Line %d: Program does not fit in SIC memory. Address exceeds 0x7FFF\n", lineNum);
            hasError = true;
            break;
        }
    }

    fclose(fp);

    if (hasError) {
        DeleteList(SymbolTable);
        // Remove object file if created
        char objectFilename[256];
        strcpy(objectFilename, filename);
        strcat(objectFilename, ".obj");
        remove(objectFilename);
        return EXIT_FAILURE;
    }

    // Print the symbol table after Pass One
    /*if(strcmp(argv[2],"--pass1only") == 0){

    }*/

    if (pass1Only) {
        // If '--pass1only' is specified, stop after Pass 1
        PrintSymbolTable(SymbolTable);
        DeleteList(SymbolTable);
        return EXIT_SUCCESS;
    }

    // Pass Two: Generate object code and write to the object file
    // Ensure no output from Pass 2 is printed to the screen during successful assembly

    ProcessFilePass2(filename, SymbolTable);

    // Clean up
    DeleteList(SymbolTable);

    return EXIT_SUCCESS;
}