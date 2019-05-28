#include <stdio.h>     
#include <stdlib.h>    
#include <stdbool.h>   
#include <string.h>    

// restrict the number of tokens to 1024
#define TOKEN_COUNT 1024

// enum structure
// contains token type
typedef enum _TOKEN_TYPE {
    STRING, // 0
    NUMBER, // 1
} TOKEN_TYPE;

// struct TOKEN
// contains information about each tokens
// such as types { string, number, array }
typedef struct _TOKEN {	
    TOKEN_TYPE type;
    union {            
        char *string;     
        double number;
    };
    bool isObjectList;
    bool isArray;
} TOKEN;

// struct JSON
// contains token list
typedef struct _JSON {
    TOKEN tokens[TOKEN_COUNT];
} JSON;

// utility function to readfile
// store date into buffer
// then return buffer
char *readFile(char *filename, int *readSize)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) return NULL;
    int size;
    char *buffer;
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    rewind(fp);
    buffer = malloc(size + 1);
    memset(buffer, 0, size + 1);
    while(!feof(fp)){
    	fread(buffer, size, 1, fp);}
    *readSize = size;
    fclose(fp);   
    return buffer;
}

// utility function to parse 
// the contents of buffer into
// each tokens
void parseJSON(char *doc, int size, JSON *json)
{
    // points to the position of index in token list
    int tokenIndex = 0;  
    // points to position of item in buffer 
    int pos = 0;           
    if (doc[pos] != '{')   
        return;
    pos++;
    // applies to all ( key : value ) objects    
    while (pos < size)     
    {
        switch (doc[pos])    
        {
	// ignore quotation marks 
        case '"':            
        {
            char *begin = doc + pos + 1;
            char *end = strchr(begin, '"');
            if (end == NULL) 
                break;       
            int stringLength = end - begin;   
            json->tokens[tokenIndex].type = STRING;
            json->tokens[tokenIndex].string = malloc(stringLength + 1);
            memset(json->tokens[tokenIndex].string, 0, stringLength + 1);
            memcpy(json->tokens[tokenIndex].string, begin, stringLength);
            tokenIndex++;    
            pos = pos + stringLength + 1;   
        }
        break;
	// applies to object list
	// ex : { "ex1":"item1", "ex2":"item2", "ex3":"item3" },
        case '{':            
        {
	    char *start = doc + pos;
	    json->tokens[tokenIndex].type = STRING;
	    json->tokens[tokenIndex].string = malloc(2);
	    memset(json->tokens[tokenIndex].string,0,2);
	    memcpy(json->tokens[tokenIndex++].string,start, 1);
	    pos++;
            while (doc[pos] != '}')    
            {
                if (doc[pos] == '"')   
                {
                    char *begin = doc + pos + 1;
                    char *end = strchr(begin, '"');
                    if (end == NULL)  
                        break;        
                    int stringLength = end - begin;   
                    json->tokens[tokenIndex].type = STRING;
                    json->tokens[tokenIndex].string = malloc(stringLength + 1);
                    json->tokens[tokenIndex].isObjectList = true;
                    memset(json->tokens[tokenIndex].string, 0, stringLength + 1);
                    memcpy(json->tokens[tokenIndex].string, begin, stringLength);
                    tokenIndex++;    
                    pos = pos + stringLength + 1;    
		}
                pos++;   
		if (doc[pos+1] == '}') {
			char *sstart = doc + pos + 1;
			json->tokens[tokenIndex].type = STRING;
			json->tokens[tokenIndex].string = malloc(2);
			memset(json->tokens[tokenIndex].string,0,2);
			memcpy(json->tokens[tokenIndex++].string,sstart,1);
		}
	    }
	}
        break;
	// applies to array list
	// ex : [ "item1", "item2", "item3" ].
	case '[':           
        {
            char *start = doc + pos;
	    json->tokens[tokenIndex].type = STRING;
	    json->tokens[tokenIndex].string = malloc(2);
	    memset(json->tokens[tokenIndex].string,0,2);
	    memcpy(json->tokens[tokenIndex++].string,start, 1);
	    pos++;
            while (doc[pos] != ']')    
            {
                if (doc[pos] == '"')   
                {
                    char *begin = doc + pos + 1;
                    char *end = strchr(begin, '"');
                    if (end == NULL)   
                        break;         
                    int stringLength = end - begin;   
                    json->tokens[tokenIndex].type = STRING;
                    json->tokens[tokenIndex].string = malloc(stringLength + 1);
                    json->tokens[tokenIndex].isArray = true;
                    memset(json->tokens[tokenIndex].string, 0, stringLength + 1);
                    memcpy(json->tokens[tokenIndex].string, begin, stringLength);
                    tokenIndex++;    
                    pos = pos + stringLength + 1;  
                }
                pos++;  
		if (doc[pos+1] == ']') {
			char *sstart = doc + pos + 1;
			json->tokens[tokenIndex].type = STRING;
			json->tokens[tokenIndex].string = malloc(2);
			memset(json->tokens[tokenIndex].string,0,2);
			memcpy(json->tokens[tokenIndex++].string,sstart,1);
		}
            }
        }
        break;
	// read numbers and 
	// store it as string
	// need to use atof()
	// to convert string -> integer 
        case '0': case '1': case '2': case '3': case '4': case '5':    
        case '6': case '7': case '8': case '9': case '-':              
        {
            char *begin = doc + pos;
            char *end;
            char *buffer;
            end = strchr(doc + pos, ',');
            if (end == NULL)
            {
                end = strchr(doc + pos, '}');
                if (end == NULL)   
                    break;          
            }
            int stringLength = end - begin;   
            buffer = malloc(stringLength + 1);
            memset(buffer, 0, stringLength + 1);
            memcpy(buffer, begin, stringLength);
            json->tokens[tokenIndex].type = NUMBER;  
            json->tokens[tokenIndex].number = atof(buffer);
            free(buffer);   
            tokenIndex++;    
            pos = pos + stringLength + 1; 
        }
        break;
        }
        pos++;
    }
}

// utility function to free json memeory
void freeJSON(JSON *json) 
{
    for (int i = 0; i < TOKEN_COUNT; i++) 
    {
        if (json->tokens[i].type == STRING) 
            free(json->tokens[i].string);  
    }
}

// the main function to carry out main tasks
int main(int argc, char **argv)
{
    // if the argv[1] is not specified at the beginning 
    // print out error message
    if (argv[1] == NULL) printf("please give json file name as argument\n");
    int size;
    // store file contents into buffer
    char *doc = readFile(argv[1], &size);
    if (doc == NULL) return -1;
    // initialize json token list
    JSON json = { 0, };    
    // separate buffer into list of strings
    parseJSON(doc, size, &json);
/*
 * This is to know the size	This is to know the size
    printf("size of size : %ld\n", sizeof(size));
    printf("size of argv[1] : %ld\n",sizeof(argv[1]));
    printf("size of json : %ld\n",sizeof(json));
    printf("size of JSON : %ld\n",sizeof(JSON));
    printf("size of TOKEN : %ld\n",sizeof(TOKEN));
    printf("size of doc : %ld\n",sizeof(doc));
 * This is to know the size	This is to know the size
*/
//  printf("%d\n",json.tokens[3].type);
    char buf[256]= { "0" };
    double temp;
/*  for (int j=0;j<8;j++){
//    	if (json.tokens[j].type==0)
//	    printf("string\n");
//    	else 
//	    printf("integer\n");
	if (json.tokens[j].type == 0)
		printf("%s\n",json.tokens[j].string);
	else
		printf("%f\n",json.tokens[j].number);
//		continue;
    }*/
    int i;
//  while (json.tokens[i].string != NULL){
    for (i=0;i<TOKEN_COUNT;i++){ 
	// to stop at the end of token list
	if (json.tokens[i].string == NULL) {
		printf("null point character");
		break;
	}
	// if the beginning of object list is met
	// don't print but continue on to the next token in list
	} else if (json.tokens[i+1].isObjectList == true) { 
	    	printf("%s : \n",json.tokens[i].string);
	    	i+=2;
		// print out tokens after '{'
		while (1) {
			// if the end of token is met
			// don't print and just break loop
			if (strcmp(json.tokens[i].string,"}")==0) break;
			printf("\t%s : %s\n",json.tokens[i].string,json.tokens[i+1].string);
			i+=2;
			}
	// if the beginning of array list is met
	// don't print but continue on to the next token in list
	} else if (json.tokens[i+1].isArray == true) {
		printf("%s : \n",json.tokens[i].string);
		i+=2;
		// print out tokens after '['
		while (1) {
			// if the end of token is met
			// don't print and just break loop
			if (strcmp(json.tokens[i].string,"]")==0) break;
			printf("\t%s\n",json.tokens[i].string);
			i++;
			}
	// if just regular ( key : value ) then print the following line
      	} else if (json.tokens[i+1].type == 1) {
		printf("%s : %d\n",json.tokens[i].string, (int)json.tokens[i+1].number);
		i++;
	} else {
		printf("%s : %s\n",json.tokens[i].string, json.tokens[i+1].string);
		i++;
	}
    }

    freeJSON(&json);
    free(doc);    
    return 0;
}
