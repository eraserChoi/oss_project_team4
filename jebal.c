#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
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

struct url_data {
    size_t size;
    char* data;
};

void printMenu(){
    int menu;
    printf("\n\n\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    printf("1. Seoul Weather\n");
    printf("2. Japan\n");
    printf("3. China\n");
    printf("4. USA\n");
    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");

    printf("menu: ");
    scanf("%d",&menu);

    if(menu==1){
        printf("seoul weather");
    }else if(menu==2){
        printf("japan weather");
    }else if(menu==3){  
        printf("china weather");
    }else if(menu==4){
        printf("usa weather");
    }else{
        printf("Please give correct number\n");
    }
}

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

size_t write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data) {

    size_t index = data->size;
    size_t n = (size * nmemb);
    char* tmp;

    data->size += (size * nmemb);

#ifdef DEBUG
    fprintf(stderr, "data at %p size=%ld nmemb=%ld\n", ptr, size, nmemb);
#endif
    tmp = realloc(data->data, data->size + 1); 

    if(tmp) {
        data->data = tmp;
    } else {
        if(data->data) {
            free(data->data);
        }
        fprintf(stderr, "Failed to allocate memory.\n");
        return 0;
    }

    memcpy((data->data + index), ptr, n);
    data->data[data->size] = '\0';

    return size * nmemb;

}
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

char *handle_url(char* url) {
    CURL *curl;
    struct url_data data;
    data.size = 0;
    data.data = malloc(4096); 

    if(NULL == data.data) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return NULL;
    }

    data.data[0] = '\0';

    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {

            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }
    return data.data;
}

void freeJSON(JSON *json)
{
	for (int i = 0; i < TOKEN_COUNT; i++)
	{
		if (json->tokens[i].isString == true)
			free(json->tokens[i].string);			
	}
}



int main(int argc, char* argv[]) {
    FILE *file;
    file = fopen("data1.txt","w+");
    char* data1;
    printMenu();
    
    data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Seoul&appid=ec80733cb38bf63779279c65ce1085e6");

    if(data1) {
        //printf("%s\n", data1);
        fprintf(file,"%s",data1);
        free(data1);
    }
    fclose(file);
	int size;
	char *doc = readFile("data1.txt", &size);
//	char *doc = jsonFile;
	if (doc == NULL) return -1;
	JSON json = { 0, };
//	size = strlen(doc);
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
			printf("[%d]: %s } ", json.tokens[j].tokenIndex, json.tokens[j].string);
			printf("( Line Number : %d ~ %d, Object )\n", json.tokens[j].begin, json.tokens[j].end);
			continue;
		}
		else if (json.tokens[j].isBigList == true && json.tokens[j].isArray == true) {
			printf("[%d]: %s ] ", json.tokens[j].tokenIndex, json.tokens[j].string);
			printf("( Line Number : %d ~ %d, Array )\n", json.tokens[j].begin, json.tokens[j].end);
			continue;
		} else break;
	}
	freeJSON(&json);
	free(doc);
    printf("FINISH\n");

    
    return 0;
}