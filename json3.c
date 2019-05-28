#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024
struct TOKEN
{
	int number;
	char *string;
	char array;
	struct TOKEN *next;
};

struct newTOKEN (char *string, int number, char array)
{
	struct TOKEN *newToken = (struct TOKEN*)malloc(sizeof(struct TOKEN));
	newToken->number = number;
	newToken->string = string;
	newToken->array = array;
	newToken->next = null; 
	return newToken;
}

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

void parse(char *doc, int size, JSON *json)
{
	int tokenIndex=0;
	int pos=0;
	if (doc[pos] != '{') return;
	pos++;
	while (pos<size) {
			
}

int main(int argc, char **argv)
{
    	int size;
    	char *doc = readFile(argv[1], &size);
    	if (doc == NULL)
        	return -1;
	JSON json = { 0, };
	parse(doc, size, &json);
	return 0;
}
