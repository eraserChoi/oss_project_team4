#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>     
#include <stdlib.h>    
#include <stdbool.h>   
#include <string.h>    

// restrict the number of tokens to 1024
#define TOKEN_COUNT 1024

// struct TOKEN
// contains information about each tokens
// such as types { string, number, array }
typedef struct _TOKEN {
	int begin;
	int end;
	int stringLength;
	int tokenIndex;
	union {
		char *string;
		double number;
	};
	bool isString;
	bool isNumber;
	bool isObjectList;
	bool isArray;
	bool isBigList;
	bool isAll;
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
	while (!feof(fp)) {
		fread(buffer, size, 1, fp);
	}
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
	char *begin = doc + pos;
	json->tokens[tokenIndex].begin = pos;
	int stringLength = size;
	json->tokens[tokenIndex].end = pos + stringLength;
	json->tokens[tokenIndex].stringLength = stringLength;
	json->tokens[tokenIndex].isString = true;
	json->tokens[tokenIndex].isAll = true;
	json->tokens[tokenIndex].string = malloc(stringLength + 1);
	memset(json->tokens[tokenIndex].string, 0, stringLength + 1);
	memcpy(json->tokens[tokenIndex].string, begin, stringLength);
	tokenIndex++;
	pos++;
	// applies to all ( key : value ) objects    
	while (pos < size)
	{
		switch (doc[pos])
		{
			// ignore quotation marks 
		case '"':
		{
			json->tokens[tokenIndex].tokenIndex = tokenIndex;
			char *begin = doc + pos + 1;
			json->tokens[tokenIndex].begin = pos + 1;
			char *end = strchr(begin, '"');
			if (end == NULL)

				break;
			int stringLength = end - begin;
			json->tokens[tokenIndex].end = pos + stringLength;
			json->tokens[tokenIndex].stringLength = stringLength;
			json->tokens[tokenIndex].isString = true;
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
			json->tokens[tokenIndex].tokenIndex = tokenIndex;
			char *begin = doc + pos;
			json->tokens[tokenIndex].begin = pos;
			char *end = strchr(begin, '}');
			if (end == NULL)
				break;
			int stringLength = end - begin;
			json->tokens[tokenIndex].end = pos + stringLength;
			json->tokens[tokenIndex].stringLength = stringLength;
//			json->tokens[tokenIndex].isString = true;
			json->tokens[tokenIndex].isBigList = true;
			json->tokens[tokenIndex].string = malloc(stringLength + 1);
			json->tokens[tokenIndex].isObjectList = true;
			memset(json->tokens[tokenIndex].string, 0, stringLength + 1);
			memcpy(json->tokens[tokenIndex].string, begin, stringLength);
			tokenIndex++;
			pos++;
			while (doc[pos] != '}')
			{
				json->tokens[tokenIndex].tokenIndex = tokenIndex;
				if (doc[pos] == '"')
				{
					char *begin = doc + pos + 1;
					json->tokens[tokenIndex].begin = pos + 1;
					char *end = strchr(begin, '"');
					if (end == NULL)
						break;
					int stringLength = end - begin;
					json->tokens[tokenIndex].end = pos + stringLength;
					json->tokens[tokenIndex].stringLength = stringLength;
					json->tokens[tokenIndex].isString = true;
					json->tokens[tokenIndex].string = malloc(stringLength + 1);
					json->tokens[tokenIndex].isObjectList = true;
					memset(json->tokens[tokenIndex].string, 0, stringLength + 1);
					memcpy(json->tokens[tokenIndex].string, begin, stringLength);
					tokenIndex++;
					pos = pos + stringLength + 1;
				}
				if (doc[pos] == '0' || doc[pos] == '1'
					|| doc[pos] == '2'
					|| doc[pos] == '3'
					|| doc[pos] == '4'
					|| doc[pos] == '5'
					|| doc[pos] == '6'
					|| doc[pos] == '7'
					|| doc[pos] == '8'
					|| doc[pos] == '9'
					|| doc[pos] == '-')
				{
					char *begin = doc + pos;
					json->tokens[tokenIndex].begin = pos;
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
					json->tokens[tokenIndex].end = pos + stringLength;
					json->tokens[tokenIndex].stringLength = stringLength;
					buffer = malloc(stringLength + 1);
					memset(buffer, 0, stringLength + 1);
					memcpy(buffer, begin, stringLength);
					json->tokens[tokenIndex].isNumber = true;
					json->tokens[tokenIndex].isObjectList = true;
					json->tokens[tokenIndex].number = atof(buffer);
					free(buffer);
					tokenIndex++;
					pos = pos + stringLength + 1;
				}
				pos++;
			}
		}
		break;
		// applies to array list
		// ex : [ "item1", "item2", "item3" ].
		case '[':
		{
			json->tokens[tokenIndex].tokenIndex = tokenIndex;
			char *begin = doc + pos;
			json->tokens[tokenIndex].begin = pos;
			char *end = strchr(begin, ']');
			if (end == NULL) break;
			int stringLength = end - begin;
			json->tokens[tokenIndex].end = pos + stringLength;
			json->tokens[tokenIndex].stringLength = stringLength;
//			json->tokens[tokenIndex].isString = true;
			json->tokens[tokenIndex].isBigList = true;
			json->tokens[tokenIndex].string = malloc(stringLength + 1);
			json->tokens[tokenIndex].isArray = true;
			memset(json->tokens[tokenIndex].string, 0, stringLength + 1);
			memcpy(json->tokens[tokenIndex].string, begin, stringLength);
			tokenIndex++;
			pos++;
			while (doc[pos] != ']')
			{
				json->tokens[tokenIndex].tokenIndex = tokenIndex;
				if (doc[pos] == '"')
				{
					char *begin = doc + pos + 1;
					json->tokens[tokenIndex].begin = pos + 1;
					char *end = strchr(begin, '"');
					if (end == NULL) break;
					int stringLength = end - begin;
					json->tokens[tokenIndex].end = pos + stringLength;
					json->tokens[tokenIndex].stringLength = stringLength;
					json->tokens[tokenIndex].isString = true;
					json->tokens[tokenIndex].string = malloc(stringLength + 1);
					json->tokens[tokenIndex].isArray = true;
					memset(json->tokens[tokenIndex].string, 0, stringLength + 1);
					memcpy(json->tokens[tokenIndex].string, begin, stringLength);
					tokenIndex++;
					pos = pos + stringLength + 1;
				}
				if (doc[pos] == '0' || doc[pos] == '1'
					|| doc[pos] == '2'
					|| doc[pos] == '3'
					|| doc[pos] == '4'
					|| doc[pos] == '5'
					|| doc[pos] == '6'
					|| doc[pos] == '7'
					|| doc[pos] == '8'
					|| doc[pos] == '9'
					|| doc[pos] == '-')
				{
					char *begin = doc + pos;
					json->tokens[tokenIndex].begin = pos;
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
					json->tokens[tokenIndex].end = pos + stringLength;
					json->tokens[tokenIndex].stringLength = stringLength;
					buffer = malloc(stringLength + 1);
					memset(buffer, 0, stringLength + 1);
					memcpy(buffer, begin, stringLength);
					json->tokens[tokenIndex].isNumber = true;
					json->tokens[tokenIndex].isArray = true;
					json->tokens[tokenIndex].number = atof(buffer);
					free(buffer);
					tokenIndex++;
					pos = pos + stringLength + 1;
				}
				pos++;
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
			json->tokens[tokenIndex].tokenIndex = tokenIndex;
			char *begin = doc + pos;
			json->tokens[tokenIndex].begin = pos;
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
			json->tokens[tokenIndex].end = pos + stringLength;
			json->tokens[tokenIndex].stringLength = stringLength;
			buffer = malloc(stringLength + 1);
			memset(buffer, 0, stringLength + 1);
			memcpy(buffer, begin, stringLength);
			json->tokens[tokenIndex].isNumber = true;
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
		if (json->tokens[i].isString == true)
			free(json->tokens[i].string);			
	}
}

void printJson(JSON *json)
{
	;
}

#define jsonFile "{ \"studentName\": \"foo\", \"studentID\" : 21500266, \"name\" : \"jsmn\", \"keywords\" : \"json\", \"description\" : \"Minimalistic JSON parser/tokenizer in C. It can be easily integrated into resoource-limited or embedded projects\", \"repository\" : { \"type\": \"git\", \"url\" : \"https://github.com/zserge/jsmn.git \", \"score\" : 123, \"test\" : 321, \"It_Is\" : \"over\" }, \"frameworks\": \"*\", \"platforms\" : \"*\", \"examples\" : [ \"ex1.c\", \"ex2.c\", \"ex3.c\", 1234, 5678, \"It_Is_OVER\" ], \"exclude\": \"test\" }"

// the main function to carry out main tasks
int main(int argc, char **argv)
{
//	if (argv[1] == NULL) printf("please give json file name as argument\n");
	int size;
//	char *doc = readFile(argv[1], &size);
	char *doc = jsonFile;
	if (doc == NULL) return -1;
	// initialize json token list
	JSON json = { 0, };
	// separate buffer into list of strings
	size = strlen(doc);
	parseJSON(doc, size, &json);
	for (int j = 0; j < TOKEN_COUNT; j++) {
		if (json.tokens[j].isAll == true)
			printf("[%d]: %s\n", json.tokens[j].tokenIndex, json.tokens[j].string);
		else if (json.tokens[j].isString == true)
			printf("[%d]: %s ( Line Number : %d ~ %d, String )\n",
				json.tokens[j].tokenIndex,
				json.tokens[j].string,
				json.tokens[j].begin,
				json.tokens[j].end);
		else if (json.tokens[j].isNumber == true)
			printf("[%d]: %d ( Line Number : %d ~ %d, Number )\n",
				json.tokens[j].tokenIndex,
				(int)json.tokens[j].number,
				json.tokens[j].begin,
				json.tokens[j].end);
		else if (json.tokens[j].isBigList == true && json.tokens[j].isObjectList == true) {
			printf("[%d]: %s }\n", json.tokens[j].tokenIndex, json.tokens[j].string);
			printf("( Line Number : %d ~ %d, Object\n", json.tokens[j].begin, json.tokens[j].end);
			continue;
		}
		else if (json.tokens[j].isBigList == true && json.tokens[j].isArray == true) {
			printf("[%d]: %s ]\n", json.tokens[j].tokenIndex, json.tokens[j].string);
			printf("( Line Number : %d ~ %d, Array\n", json.tokens[j].begin, json.tokens[j].end);
			continue;
		} else break;
	}
	freeJSON(&json);
	free(doc);
	printf("FINISH");
	return 0;
}
