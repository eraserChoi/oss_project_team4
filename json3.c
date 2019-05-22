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

int main()
{
	char buffer[MAX] = {0};
	char *readFile;
	FILE *fp = fopen("example.json", "rb");
	if (fp == NULL) return -1;
	int size;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);
	readFile = (char *)calloc(size+1,sizeof(char));
	if (fread(readFile, size, 1, fp) < 1) {
		size = 0;
		fclose(fp);
		return -1;
	}
	fclose(fp);
	if (readFile[0] != '{') {
		printf("This is not json file >>>>> missing '{'\n");
		return -1;
	}
	printf("%s\n",readFile);
	int object=0,count=0,start=0,j=0;
	for (int i=0;i<size;i++) {

	}
}
