#include "headers.h"


void ProcessFilePass2(string filename, SymbolList SymbolTable) {
    FILE *sourceFile;
    FILE *objectFile;
    char line[1024];
    int lineNum = 0;
    bool hasError = false;

    sourceFile = fopen(filename, "r");
    if (sourceFile == NULL) {
        printf("ERROR: Could not open source file %s\n", filename);
        return;
    }

    char objectFilename[256];
    strcpy(objectFilename, filename);
    strcat(objectFilename, ".obj");
    objectFile = fopen(objectFilename, "w");
    if (objectFile == NULL) {
        printf("ERROR: Could not create object file %s\n", objectFilename);
        fclose(sourceFile);
        return;
    }

    int startAddress = 0;
    int programLength = 0;
    int locationCntr = 0;
    int firstExecutableAddress = 0;
    char programName[7] = "      ";
    char textRecord[61] = "";
    int textRecordStart = 0;
    int textRecordLength = 0;
    int lastLoc = 0;

    bool startDirectiveFound = false;
    bool endDirectiveFound = false;

#define MAX_M_RECORDS 1000
    char mRecords[MAX_M_RECORDS][20];
    int mRecordCount = 0;

    char textRecords[1000][80];
    int textRecordCount = 0;

    while (fgets(line, 1024, sourceFile) != NULL) {
        lineNum++;
        line[strcspn(line, "\n")] = '\0';
        char *trimmed_line = trim_whitespace(line);
        if (strlen(trimmed_line) == 0 || trimmed_line[0] == '#') 
        {
            continue;
        }

        string label = NULL;
        string opcode = NULL;
        string operand = NULL;

        string token = NULL;
        string rest = trimmed_line;
        token = strtok_r(rest, " \t\n", &rest);
        if (token == NULL) {
            continue;
        }

        if (IsOpcode(token) || strcmp(token, "START") == 0 || strcmp(token, "END") == 0 ||
            strcmp(token, "WORD") == 0 || strcmp(token, "RESW") == 0 || strcmp(token, "RESB") == 0 ||
            strcmp(token, "BYTE") == 0) {
            opcode = token;
        } else {
            label = token;
            opcode = strtok_r(NULL, " \t\n", &rest);
        }

        if (opcode != NULL) {
            operand = strtok_r(NULL, "\n", &rest);
            if (operand != NULL) {
                operand = trim_whitespace(operand);
            }
        }

        if (opcode != NULL && strcmp(opcode, "START") == 0) {
            startDirectiveFound = true;
            if (operand != NULL) {
                sscanf(operand, "%x", &startAddress);
                locationCntr = startAddress;
                firstExecutableAddress = startAddress;
                if (label != NULL) {
                    strncpy(programName, label, 6);
                    programName[6] = '\0';
                }
            } else {
                printf("ASSEMBLY ERROR:\n");
                printf("%s\n", line);
                printf("Line %d: No operand for START directive\n", lineNum);
                hasError = true;
                break;
            }
            continue;
        }

        if (!startDirectiveFound) {
            printf("ASSEMBLY ERROR:\n");
            printf("Line %d: Missing START directive\n", lineNum);
            hasError = true;
            break;
        }

        if (opcode != NULL && strcmp(opcode, "END") == 0) 
        {
            endDirectiveFound = true;
            if (textRecordLength > 0) {
                sprintf(textRecords[textRecordCount], "T%06X%02X%s", textRecordStart, textRecordLength, textRecord);
                textRecordCount++;
                textRecord[0] = '\0';
                textRecordLength = 0;
            }
            if (operand != NULL && strlen(operand) > 0) 
            {
                operand = trim_whitespace(operand);
                for (int i = 0; operand[i]; i++) 
                {
                    operand[i] = toupper(operand[i]);
                }
                if (!IsInSymbolTable(SymbolTable, operand)) 
                {
                    printf("ASSEMBLY ERROR:\n");
                    printf("%s\n", line);
                    printf("Line %d: Undefined symbol in END directive '%s'\n", lineNum, operand);
                    hasError = true;
                    break;
                }
                firstExecutableAddress = getSymbolAddress(SymbolTable, operand);
            } 
            else 
            {
                firstExecutableAddress = startAddress;
            }
            break;
        }

        int currentAddress = locationCntr;
        char *objCode = NULL;

        if (IsOpcode(opcode)) 
        {
            int opcodeValue = getOpcodeValue(opcode);
            int address = 0;
            int indexed = 0;
            if (operand != NULL && strlen(operand) > 0) {
                char *commaPos = strchr(operand, ',');
                if (commaPos != NULL && strcmp(commaPos, ",X") == 0) {
                    indexed = 1;
                    *commaPos = '\0';
                    operand = trim_whitespace(operand);
                }
                operand = trim_whitespace(operand);
                for (int i = 0; operand[i]; i++) 
                {
                    operand[i] = toupper(operand[i]);
                }
                if (IsInSymbolTable(SymbolTable, operand)) 
                {
                    address = getSymbolAddress(SymbolTable, operand);
                    if (mRecordCount < MAX_M_RECORDS) 
                    {
                        sprintf(mRecords[mRecordCount], "M%06X04+%s", currentAddress + 1, programName);
                        mRecordCount++;
                    } 
                    else 
                    {
                        printf("ERROR: Exceeded maximum number of modification records.\n");
                        hasError = true;
                        break;
                    }
                } else {
                    printf("ASSEMBLY ERROR:\n");
                    printf("%s\n", line);
                    printf("Line %d: Undefined symbol '%s'\n", lineNum, operand);
                    hasError = true;
                    break;
                }
            }
            int finalAddress = address;
            if (indexed) {
                finalAddress += 0x8000;
            }
            objCode = (char *)malloc(7);
            sprintf(objCode, "%02X%04X", opcodeValue, finalAddress & 0xFFFF);
            locationCntr += 3;
        } else if (strcmp(opcode, "BYTE") == 0) {
            if (operand[0] == 'C' && operand[1] == '\'') {
                char *start = strchr(operand, '\'') + 1;
                char *end = strrchr(operand, '\'');
                if (end == NULL || end <= start) {
                    printf("ASSEMBLY ERROR:\n");
                    printf("%s\n", line);
                    printf("Line %d: Invalid BYTE operand '%s'\n", lineNum, operand);
                    hasError = true;
                    break;
                }
                int length = end - start;
                int objCodeSize = (length * 2) + 1;
                objCode = (char *)malloc(objCodeSize);
                for (int i = 0; i < length; i++) {
                    sprintf(objCode + i * 2, "%02X", start[i]);
                }
                objCode[length * 2] = '\0';
                locationCntr += length;
            } else if (operand[0] == 'X' && operand[1] == '\'') {
                char *start = strchr(operand, '\'') + 1;
                char *end = strrchr(operand, '\'');
                if (end == NULL || end <= start) {
                    printf("ASSEMBLY ERROR:\n");
                    printf("%s\n", line);
                    printf("Line %d: Invalid BYTE operand '%s'\n", lineNum, operand);
                    hasError = true;
                    break;
                }
                int length = end - start;
                objCode = (char *)malloc(length + 1);
                strncpy(objCode, start, length);
                objCode[length] = '\0';
                locationCntr += (length + 1) / 2;
            } else {
                printf("ASSEMBLY ERROR:\n");
                printf("%s\n", line);
                printf("Line %d: Invalid BYTE operand\n", lineNum);
                hasError = true;
                break;
            }
        } else if (strcmp(opcode, "WORD") == 0) {
            if (operand == NULL || !isNumeric(operand)) {
                printf("ASSEMBLY ERROR:\n");
                printf("%s\n", line);
                printf("Line %d: Invalid or missing operand for WORD\n", lineNum);
                hasError = true;
                break;
            }
            int value = atoi(operand);
            objCode = (char *)malloc(7);
            sprintf(objCode, "%06X", value & 0xFFFFFF);
            locationCntr += 3;
        } else if (strcmp(opcode, "RESB") == 0) {
            if (operand == NULL || !isNumeric(operand)) {
                printf("ASSEMBLY ERROR:\n");
                printf("%s\n", line);
                printf("Line %d: Invalid or missing operand for RESB\n", lineNum);
                hasError = true;
                break;
            }
            locationCntr += atoi(operand);
            if (textRecordLength > 0) {
                sprintf(textRecords[textRecordCount], "T%06X%02X%s", textRecordStart, textRecordLength, textRecord);
                textRecordCount++;
                textRecord[0] = '\0';
                textRecordLength = 0;
            }
            continue;
        } 
        else if (strcmp(opcode, "RESW") == 0) 
        {
            if (operand == NULL || !isNumeric(operand)) 
            {
                printf("ASSEMBLY ERROR:\n");
                printf("%s\n", line);
                printf("Line %d: Invalid or missing operand for RESW\n", lineNum);
                hasError = true;
                break;
            }
            locationCntr += 3 * atoi(operand);
            if (textRecordLength > 0) 
            {
                sprintf(textRecords[textRecordCount], "T%06X%02X%s", textRecordStart, textRecordLength, textRecord);
                textRecordCount++;
                textRecord[0] = '\0';
                textRecordLength = 0;
            }
            continue;
        } 
        else {
            printf("ASSEMBLY ERROR:\n");
            printf("%s\n", line);
            printf("Line %d: Invalid opcode '%s'\n", lineNum, opcode);
            hasError = true;
            break;
        }

        if (objCode != NULL) {
            int objCodeLength = strlen(objCode);
            int objCodeIndex = 0;

            while (objCodeIndex < objCodeLength) {
                if (textRecordLength == 0) {
                    textRecordStart = currentAddress + (objCodeIndex / 2);
                }

                int remainingBytesInRecord = 30 - textRecordLength;
                int remainingHexCharsInRecord = remainingBytesInRecord * 2;
                int remainingObjCodeLength = objCodeLength - objCodeIndex;
                int copyLength = (remainingObjCodeLength < remainingHexCharsInRecord) ? remainingObjCodeLength : remainingHexCharsInRecord;

                strncat(textRecord, objCode + objCodeIndex, copyLength);
                textRecordLength += copyLength / 2;
                objCodeIndex += copyLength;

                if (textRecordLength == 30) {
                    sprintf(textRecords[textRecordCount], "T%06X%02X%s", textRecordStart, textRecordLength, textRecord);
                    textRecordCount++;
                    textRecord[0] = '\0';
                    textRecordLength = 0;
                }
            }

            free(objCode);
        }

        lastLoc = locationCntr;
    }

    if (!hasError && !endDirectiveFound) {
        printf("ASSEMBLY ERROR:\n");
        printf("Line %d: Missing END directive\n", lineNum);
        hasError = true;
    }

    if (textRecordLength > 0) {
        sprintf(textRecords[textRecordCount], "T%06X%02X%s", textRecordStart, textRecordLength, textRecord);
        textRecordCount++;
    }

    if (!hasError) {
        if (!endDirectiveFound) {
            programLength = lastLoc - startAddress;
        }
        fprintf(objectFile, "H%-6s%06X%06X\n", programName, startAddress, programLength);
        for (int i = 0; i < textRecordCount; i++) {
            fprintf(objectFile, "%s\n", textRecords[i]);
        }
        for (int i = 0; i < mRecordCount; i++) {
            fprintf(objectFile, "%s\n", mRecords[i]);
        }
        fprintf(objectFile, "E%06X\n", firstExecutableAddress);
    }

    fclose(sourceFile);
    fclose(objectFile);

    if (hasError) 
    {
        remove(objectFilename);
    }
}