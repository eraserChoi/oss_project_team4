#define _CRT_SECURE_NO_WARNINGS   
#include <stdio.h>  
#include <stdlib.h>    
#include <stdbool.h>  
#include <string.h>    
typedef enum _TOKEN_TYPE {
    TOKEN_STRING,   
    TOKEN_NUMBER,   
} TOKEN_TYPE;

typedef struct _TOKEN {
    TOKEN_TYPE type;  
    union {            
        char *string;     
        double number;    
    };
    bool isArray; 
} TOKEN;

#define TOKEN_COUNT 20   


typedef struct _JSON {
    TOKEN tokens[TOKEN_COUNT]; 
} JSON;

char *readFile(char *filename, int *readSize)   
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
        return NULL;

    int size;
    char *buffer;

  
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

   
    buffer = malloc(size + 1);
    memset(buffer, 0, size + 1);

    
    if (fread(buffer, size, 1, fp) < 1)
    {
        *readSize = 0;
        free(buffer);
        fclose(fp);        
        return NULL;
    }

  
    *readSize = size;

    fclose(fp);    

    return buffer;
}

void parseJSON(char *doc, int size, JSON *json)   
{
    int tokenIndex = 0;    
    int searchIndex = 0;          

    if (doc[searchIndex] != '{')   
        return;

    searchIndex++;    

    while (searchIndex < size)       
    {
        switch (doc[searchIndex])    
        {
        case '"':            
        {
         
            char *begin = doc + searchIndex + 1;

        
            char *end = strchr(begin, '"');
            if (end == NULL)    
                break;         

            int stringLength = end - begin; 

            
       
            json->tokens[tokenIndex].type = TOKEN_STRING;
         
            json->tokens[tokenIndex].string = malloc(stringLength + 1);
        
            memset(json->tokens[tokenIndex].string, 0, stringLength + 1);

         
            memcpy(json->tokens[tokenIndex].string, begin, stringLength);

            tokenIndex++; 

            searchIndex = searchIndex + stringLength + 1;  
        }
        break;
	
        }

        searchIndex++; 



    }
}

void freeJSON(JSON *json)    
{
    for (int i = 0; i < TOKEN_COUNT; i++)            
    {
        if (json->tokens[i].type == TOKEN_STRING)    
            free(json->tokens[i].string);            
    }
}

int main(int argc,char **argv)
{
    int fileSize;  
    
    
    char *file = readFile(argv[1], &fileSize);
    if (file == NULL)
        return -1;

    JSON json = { 0, };    

    parseJSON(file, fileSize, &json);    

    for(int i=0;i<TOKEN_COUNT-1;i++){
    	printf("%s\n",json.tokens[i].string);
    }
    

    freeJSON(&json);    

    free(file);    

    return 0;
}
