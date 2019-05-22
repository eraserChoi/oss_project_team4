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

struct newTOKEN (struct TOKEN *head, char *string, int number, char array)
{
	struct TOKEN *newToken = (struct TOKEN*)malloc(sizeof(struct TOKEN));
	newToken->number = number;
	newToken->string = string;
	newToken->array = array;
	newToken->next = null;
	head->next = 
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
	struct TOKEN *head;
	for (int i=0;i<size;i++) {
		if (readFile[i] == '{'){
			object++;
			count++;
		} 
		if (readFile[i] == ':' || buffer[i] == ',') continue;
		if (readFile[i] == '"') { // indicate start of string type object
			start = i+1;
			while(readFile[start] != '"')
				buffer[j++] = readFile[start++];
			i=start;
			start=0
			j=0;
			newTOKEN(head, buffer, 0, "0");
		} 
		if (count > 1) { // indicating array type
			start=i;
			while(readFile[start] != '}')
				buffer[j++]=readFile[start++];
			buffer[j+1]=readFile[start+1];
			i=start;
			start=0;
			j=0;
			newTOKEN(head,buffer,0,buffer);
		}
		if (readFile[i] == '}') count--;
		if (readFile[i] == '0'
				|| readFile[i] == '1'
				|| readFile[i] == '2'
				|| readFile[i] == '3'
				|| readFile[i] == '4'
				|| readFile[i] == '5'
				|| readFile[i] == '6'
				|| readFile[i] == '7'
				|| readFile[i] == '8'
				|| readFile[i] == '9'
				|| readFile[i] == '-' ){
			start=i;
			while(readFile[start] != ',')
				buffer[j++]=readFile[start++];
			i=start;
			start=0;
			j=0;
			newTOKEN(head,0

			



	return 0;
}
