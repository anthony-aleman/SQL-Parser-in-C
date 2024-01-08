#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


// Project: LL(1) SQL Parser
// Author: Anthony Aleman
// 
// Description: This is a simple LL(1) SQL Parser written using C11. 
//            This tool will be used to query simple CSV files.
//            This is a work in progress and is not yet complete 
//            as this only parses a small subset of SQL.
//                                                                  .
//
//  Current SQL subset supported:
//    - SELECT
//    - WHERE
//    - Table names as the csv file names (FROM)
//    - Column names without aliases
//   
// Example Query: SELECT column1, column2 FROM table1 WHERE column1 = 'value';
//
// Writing Grammar:
//  Query -> SELECT ColumnList FROM TableName WhereClause
//  ColumnList -> ColumnName ColumnListTail
//  ColumnListTail -> , ColumnName ColumnListTail | e
//  TableName -> identifier
//  WhereClause -> WHERE Condition | e
//  Condition -> ColumnName = Value
//  ColumnName -> identifier
//  Value -> string_literal,



//Definig tokens
typedef enum {
    SELECT, // SELECT
    FROM,  // FROM
    WHERE, // WHERE
    IDENTIFIER, // 
    STRING_LITERAL,
    COMMA, 
    EQUALS, 
    SEMICOLON,
    EPSILON, 
    EOF_TOKEN, // \0
    INVALID,
    /* Non-Terminals*/
    QUERY, 
    COLUMN_LIST, 
    COLUMN_LIST_TAIL,
    TABLE_NAME, 
    WHERE_CLAUSE, 
    CONDITION,
    COLUMN_NAME, 
    VALUE
} Symbols;

bool isKeyword(const char* word);
Symbols keywordToSymbol(const char* word);


bool isKeyword(const char* word){
    const char* keywords[] = {"SELECT", "FROM", "WHERE"};
    int numKeywords = sizeof(keywords) / sizeof(keywords[0]);

    for (int i = 0; i < numKeywords; i++)
    {
        if (strcmp(word, keywords[i]) == 0)
        {
            return true;
        }
        
    }
    //if (strcmp(word, "SELECT") == 0) return true;
    //if (strcmp(word, "FROM") == 0) return true;
    //if (strcmp(word, "WHERE") == 0) return true;
    return false;
}

Symbols keywordToSymbol(const char* word){
    if (strcmp(word, "SELECT") == 0) return SELECT;
    if (strcmp(word, "FROM") == 0) return FROM;
    if (strcmp(word, "WHERE") == 0) return WHERE;
    return INVALID;
}


// Lexer Function

Symbols* lexer (const char* input){
    //Allocate memory for the tokens based on the length of the input
    Symbols* tokens = malloc(sizeof(Symbols) * strlen(input));
    if(!tokens) return NULL;// Return NULL if memory allocation fails

    int tokenCount = 0;
    int index = 0;
    char word[256];

    // Iterate over each character in the input
    while (input[index] != '\0')
    {
        // Check if character in the input is an alphabet letter
        // 'a'- 'z' || 'A' - 'Z'
        if (isalpha(input[index]))
        {
            int wordIndex = 0;
            // Collect all contigious alphanumeric characters
            while (isalnum(input[index]))
            {
                word[wordIndex++] = input[index++];
            }
            word[wordIndex] = '\0'; // Null terminate the string
            
            // Determine if the word is a keyword or an identifier
            tokens[tokenCount++] = isKeyword(word) ? keywordToSymbol(word) : IDENTIFIER;
        } 
        else if (input[index] == '\'') {  // Handling string literals
            index++; // Skip the opening quote
            int j = 0;
            
            // Collect all contigious characters until the closing quote
            while (input[index] != '\0' && input[index] != '\'') {
                word[j++] = input[index++];
            }

            if (input[index] == '\'') {
                index++; // Skip the closing quote
            }
            word[j] = '\0'; // Null terminate the string
            tokens[tokenCount++] = STRING_LITERAL;
        }   
        else {
            // Sewitch case to handle all single character tokens
            switch (input[index])
            {
            case ' ': case '\t': case '\n': // ignoring whitespace
                index++;
                break;
            case ',': // Add comma token
                tokens[tokenCount++] = COMMA;
                index++;
                break;
            case '=': // Add equals token
                tokens[tokenCount++] = EQUALS;
                index++;
                break;
            case ';': // Add semicolon token
                tokens[tokenCount++] = SEMICOLON;
                index++;
                break;
            default: //Mark anything else as INVALID
                tokens[tokenCount++] = INVALID;
                index++;
                break;
            }
        }
       
        
    }

    // Add EOF token to indicate the end of the token stream
    tokens[tokenCount] = EOF_TOKEN;
    return tokens; // Return the array of tokens
}


void printToken(Symbols token) {
    const char* tokenNames[] = {
        "SELECT", "FROM", "WHERE", "IDENTIFIER", "STRING_LITERAL",
        "COMMA", "EQUALS", "SEMICOLON", "EPSILON", "EOF_TOKEN", "INVALID",
        "QUERY", "COLUMN_LIST", "COLUMN_LIST_TAIL", "TABLE_NAME",
        "WHERE_CLAUSE", "CONDITION", "COLUMN_NAME", "VALUE"
    };
    printf("%s\n", tokenNames[token]);
}




int main(){
    printf("Starting Parser\n");

    char* input = "SELECT ColumnName FROM File WHERE ColumnName = 'value';";

    Symbols* tokens = lexer(input);

    if(!tokens){
        printf("Memory allocation failed\n");
        return 1;
    }


    int i = 0;
    while (tokens[i] != EOF_TOKEN){
        printToken(tokens[i]);

        i++;
    }
    
    free(tokens);
    
    return 0;
}