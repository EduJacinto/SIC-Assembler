#include "headers.h"
/*
Understand:
    given a SIC assembler program file, you must read in the file and create a symbol table

    no blank lines are allowed
    symbols have a maximum of 6 chars, 7 including the string terminator character
    symbols must be strictly alphabetical 

    if an error is found within the assembly file, stop the program and return an error message, 
    include the line number and description of error encountered

    if the file is valid, the output should be the symbol table followed by a tab and the hexadecimal address of the symbol and \r\n
*/
int main(int argc, char* argv[] )
{

        //declare variables and file pointer
        FILE *fp;
        char line[1024];
        char *token;
		SymbolList list = NULL;

        //check if the  user input the correct number of arguments
        if ( !(argc == 2) )
		{
            printf("USAGE: %s <filename>.  - where filename is a SIC Assembly File\n", argv[0]);
            return 0;
        }
        //open a file stream
        fp = fopen(argv[1], "r");

        //check if file exists fopen returns null if no file. Otherwise returns file pointer.
        if ( fp == NULL )
        {
            printf("ERROR: The File %s could not be opened for reading\n", argv[1] );
            return 0;
        }


        //will keep track of current line and locationAddress
		int lineCounter = 0;
        int locationAddress= 0;

		//if file is valid, and while line does not equal null we have to loop through each 
        //line of the given file to extract data
        while( fgets(line, 1024, fp) != NULL )
        {
            lineCounter++;//update line number
           
            
            

            //check for blank lines and stop if blank lines are read in
            if( strlen(line) <= 2 || line[0] == '\n')
            {
                printf("ASSEMBLY ERROR:\nLine %d ERROR: Blank lines are not allowed\r\n", lineCounter);
                fclose(fp);
                return EXIT_FAILURE;
            }
			//check if the line is a comment, then skip it
            if( line[0] == 35 )
            {
                continue;
            }

			//when reading in a line of valid assemmbly it should contain a maximum of three tokens but at least two 
            //The first of three tokens is a symbol, but if there are less than three tokens, there is no symbol

            /* 
                although we can tokenize the current line, we need to break it down to 
                symbols, directives, and operands. so we need to be able to store each of 
                these components, if they exist, into a structure for use later.
                Additionally, we need to be able to accurately increment the location address.
            */
            char symbolName[7] = {'\0'};
            char *directive = NULL; //this will act as a catch all for directives and instructions
            char *operand = NULL;
            char *saveptr;
            int numTokens = 0;
            //begin tokenizing the line, the first argument must be the line string itself
            //subsequent calls we pass NULL for the first argument
            token = strtok_r(line, " \r\t\n", &saveptr);

            //we are tokenizing, check the first token and verify if it is a symbol
            //if symbol, then save as symbolName
            //otherwise, check if it is a valid directive

            while(token != NULL)
            {
                numTokens++;

                if(numTokens == 1)
                {               
                    if(IsDirective(token) == 1 || IsInstruction(token) == 1 )
		            {
                        directive = token;
                        
                    }
                    else
                    {
                        if( IsValidSymbol(token) == 1 )
                        {
                            strcpy(symbolName, token);
                            
                        }
                        else
                        {
                            printf("ASSEMBLY ERROR:\nLine %d ERROR: Invalid symbol entered \r\n", lineCounter);
                            fclose(fp);
                            return EXIT_FAILURE;
                        }
                    }                    
                }
                else if( numTokens == 2 )
                {
                    if( directive != NULL )
                    {
                        operand = token;
                        
                    }
                    else
                    {
                        directive = token;
                        
                    }
                }
                else if(numTokens == 3)
                {
                    operand = token;
                    
                }
                // printf("\n%s\n", token);
                token = strtok_r(NULL, " \t\n", &saveptr);

                //this is a possible fix statement for the test case 4 error where an invalid symbol is passed
                //this statement cannot go here because it makes every testcase fail
                if(numTokens== 4 && symbolName[0] == '\0')
                {
                    printf("ASSEMBLY ERROR:\nLine %d ERROR: SIC Program Has Symbol Name That Is An Assembler Directive\r\n", lineCounter);
                    fclose(fp);
                    return EXIT_FAILURE;
                }
            }//end line tokenization process
	/**********TOKENIZATION PROCESS IS COMPLETE, CONTINUE TO TOKEN PROCESSING***************/
            // printf("%d\n", numTokens);
            // printf("\n%s\n", directive);
            //there should always be a directive
            if( directive == NULL )//handle null directive error
            {
                
                printf("ASSEMBLY ERROR:\nLine %d ERROR: NULL directive\r\n", lineCounter);
                fclose(fp);
                return EXIT_FAILURE;
            }
            else if( IsDirective(directive) == 0 && IsInstruction(directive) == 0 )//check if an invalid directive is saved to directive
            {
                printf("ASSEMBLY ERROR:\nLine %d ERROR: Invalid directive or instruction\r\n", lineCounter);
                fclose(fp);
                return EXIT_FAILURE;
            }

            //the line has been processed so, begin processing tokens
            
            if( strcmp(directive, "END") == 0 )//this may be deprecated, it is just a prototype of an ending case where the directive is END
            {   
                if( symbolName[0] != '\0' )
                {
                    if( IsInSymbolTable(list, symbolName) == 0 )
                    {
                    
                        InsertSymbol(&list, symbolName, locationAddress, lineCounter);
                   
                    }
                    else
                    {
                        printf("ASSEMBLY ERROR:\nLine %d ERROR: Duplicate symbol exists\r\n", lineCounter);
                        fclose(fp);
                        return EXIT_FAILURE;
                    }

                }
                
                break;
            }

            //I want to insert the START directive at the beginning of each 
            if( strcmp(directive, "START") == 0 )//the case of the start directive
            {
                if(operand == NULL)//START requires an operand usually a memory address
                {
                    printf("ASSEMBLY ERROR\nLine %d ERROR: operand required\r\n", lineCounter);
                    fclose(fp);
                    return EXIT_FAILURE;
                }
                else
                {
                    locationAddress = (int)strtol(operand, NULL, 16);

		            if(locationAddress >= 0x8000)
		            {
			            printf("ASSEMBLY ERROR:\nLine %d ERROR: SIC Program Starts at Hex 8000. No room left in SIC memory.\r\n", lineCounter);
			            fclose(fp);
			            return EXIT_FAILURE;
		            }
                    

                    if( IsInSymbolTable(list, symbolName) == 0 )//possible seg fault here
                    {
                        
                        InsertSymbol(&list, symbolName, locationAddress, lineCounter);
                        
                        continue;
                    }
                    else
                    {
                        printf("ASSEMBLY ERROR:\nLine %d ERROR: Duplicate symbol exists\r\n", lineCounter);
                        fclose(fp);
                        return EXIT_FAILURE;
                    }
                }
            }//end if directive = start

            if( symbolName[0] != '\0' )
            {
                if( IsInSymbolTable(list, symbolName) == 0 )
                {
                    
                    InsertSymbol(&list, symbolName, locationAddress, lineCounter);
                   
                }
                else{
                    printf("ASSEMBLY ERROR:\nLine %d ERROR: Duplicate symbol exists\r\n", lineCounter);
                    fclose(fp);
                    return EXIT_FAILURE;
                }

            }

            
            if( strcmp(directive, "RESW") == 0 )//if requesting to reserve certain amount of space
            {
                if( operand == NULL )//error if null operand
                {
                    printf("ASSEMBLY ERROR:\nLine %d ERROR: NULL operand\r\n", lineCounter);
                    fclose(fp);
                    return EXIT_FAILURE;
                }
                else//will probably need to be able to address invalid operand error
                {
                    int request = (int)strtol(operand, NULL, 10);//parse integer out of operand token
                    locationAddress += ( 3 * request );//increment locAddress by 3 * request indicated

		    if(locationAddress >= 0x8000)
            	    {
                	printf("ASSEMBLY ERROR:\nLine %d ERROR: Program Does not fit in SIC Memory.\r\n", lineCounter);//CHANGE MADE HERE AND AT START IF STATEMENT
                	fclose(fp);
                	return EXIT_FAILURE;
                    }
                }
            }
            else if( strcmp(directive, "RESB") == 0 )//if requesting to reserve certain amount of space
            {
                if( operand == NULL )//error if null operand
                {
                    printf("ASSEMBLY ERROR:\nLine %d ERROR: NULL operand\r\n", lineCounter);
                    fclose(fp);
                    return EXIT_FAILURE;
                }
                else//will probably need to be able to address invalid operand error
                {
                    //resb just reserves the indicated number of bytes in hex the operand will be the number in decimal so if you just strtol in base 16 it should be automatically turned from base  10 to base 16
                    int request = (int)strtol(operand, NULL, 10);//parse the requested num of bytes in decimal
                    locationAddress += request;

                    if(locationAddress >= 0x8000)
            	    {
                    	printf("ASSEMBLY ERROR:\nLine %d ERROR: Program Does not fit in SIC Memory.\r\n", lineCounter);//CHANGE MADE HERE AND AT START IF STATEMENT
                    	fclose(fp);
                    	return EXIT_FAILURE;
            	    }

		        }
            }
            //if word then check if the operand length is greater than 6 digits
            //then throw error and word constant is longer than 24 bits
            else if( strcmp(directive, "WORD") == 0 )
            {
                if(operand == NULL)
                {
                    printf("ASSEMBLY ERROR:\nLine %d ERROR: NULL operand, operand necessary\r\n", lineCounter);
                    fclose(fp);
                    return EXIT_FAILURE;
                }
                else
                {

                    if( strlen(operand) > 7)
                    {
                        printf("ASSEMBLY ERROR:\nLine %d ERROR: WORD Constant Exceeds 24 Bit Limitation.\r\n", lineCounter);//CHANGE MADE HERE AND AT START IF STATEMENT
                	    fclose(fp);
               	        return EXIT_FAILURE;
                    }
                    else
                    {
                        locationAddress += 3;//if the directive matches and there is an operand, it is a valid line. increment by 3.

		                if(locationAddress >= 0x8000)
                        {
                	        printf("ASSEMBLY ERROR:\nLine %d ERROR: Program Does not fit in SIC Memory.\r\n", lineCounter);//CHANGE MADE HERE AND AT START IF STATEMENT
                	        fclose(fp);
               	            return EXIT_FAILURE;
            	        }
                    }
                    // locationAddress += 3;//if the directive matches and there is an operand, it is a valid line. increment by 3.

		            // if(locationAddress >= 0x8000)
                    // {
                	//     printf("ASSEMBLY ERROR:\nLine %d ERROR: Program Does not fit in SIC Memory.\r\n", lineCounter);//CHANGE MADE HERE AND AT START IF STATEMENT
                	//     fclose(fp);
               	    //     return EXIT_FAILURE;
            	    // }
                }
            }
            else if( /*strcmp(directive, "WORD") == 0 || */strcmp(directive, "RESR") == 0 || strcmp(directive, "EXPORTS") == 0 )
            {
                if(operand == NULL)
                {
                    printf("ASSEMBLY ERROR:\nLine %d ERROR: NULL operand, operand necessary\r\n", lineCounter);
                    fclose(fp);
                    return EXIT_FAILURE;
                }
                else
                {
                    locationAddress += 3;//if the directive matches and there is an operand, it is a valid line. increment by 3.

		            if(locationAddress >= 0x8000)
                    {
                	printf("ASSEMBLY ERROR:\nLine %d ERROR: Program Does not fit in SIC Memory.\r\n", lineCounter);//CHANGE MADE HERE AND AT START IF STATEMENT
                	fclose(fp);
               	        return EXIT_FAILURE;
            	    }
                }
            }
            else if( strcmp(directive, "BYTE") == 0 )
            {
                if(operand == NULL)
                {
                    printf("ASSEMBLY ERROR:\nLine %d ERROR: NULL operand, operand necessary\r\n", lineCounter);
                    fclose(fp);
                    return EXIT_FAILURE;
                }
                else
                {
                    if( operand[0] == 'C' && operand[1] == '\'' )
                    {
                        int size = strlen(operand) - 3;
                        locationAddress += size;

			            if(locationAddress >= 0x8000)
            		    {
                	        printf("ASSEMBLY ERROR:\nLine %d ERROR: Program Does not fit in SIC Memory.\r\n", lineCounter);//CHANGE MADE HERE AND AT START IF STATEMENT
                	        fclose(fp);
                	        return EXIT_FAILURE;
            		    }
                    }
                    else if( operand[0] == 'X' && operand[1] == '\'' )//need to also check for inalid hex constants
                    {
                        //extract the constant from the token using strrchr to find the last occurence of the ' char
                        //then extract substr hex const 
                        char* end = strrchr(operand, '\'');
                        int substr_sz = end - operand;
                        char* valid_hex = "AaBbCcDdEeFf0123456789";

                        for(int i = 2; i < substr_sz; i ++)
                        {
                            if( strchr(valid_hex, operand[i]) == NULL )
                            {
                                printf("ASSEMBLY ERROR:\nLine %d ERROR: SIC Program Has Invalid Hex Constant\r\n", lineCounter);
                                fclose(fp);
                                return EXIT_FAILURE;
                            }
                        }

                        int size = strlen(operand) - 3;
                        locationAddress += ( size / 2 );

			            if(locationAddress >= 0x8000)
            		    {
                	        printf("ASSEMBLY ERROR:\nLine %d ERROR: Program Does not fit in SIC Memory.\r\n", lineCounter);//CHANGE MADE HERE AND AT START IF STATEMENT
                	        fclose(fp);
                	        return EXIT_FAILURE;
            		    }
		            }
                    else
                    {
                        printf("ASSEMBLY ERROR:\nLine %d ERROR: Invalid operand\r\n", lineCounter);
                        fclose(fp);
                        return EXIT_FAILURE;
                    }
                }
            }
            else if( IsInstruction(directive) == 1 )
            {
                locationAddress += 3;
            	if(locationAddress >= 0x8000)
            	{
                    printf("ASSEMBLY ERROR:\nLine %d ERROR: Program Does not fit in SIC Memory.\r\n", lineCounter);//CHANGE MADE HERE AND AT START IF STATEMENT
                    fclose(fp);
                    return EXIT_FAILURE;
            	}
	    }
            /* Finished processing all the directives and updating location counter */
        }//end reading in lines

        fclose(fp); //close file stream

        PrintSymbolTable(list);
        DeleteList(list);
        return 0;
}