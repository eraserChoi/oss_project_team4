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
		if (doc[pos] == '"')
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
		pos = pos + stringLength + 2;
		}
		else if (doc[pos] == '{')
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
		json->tokens[tokenIndex].isBigList = true;
		json->tokens[tokenIndex].isObjectList = true;
		json->tokens[tokenIndex].string = malloc(stringLength + 1);
		memset(json->tokens[tokenIndex].string, 0, stringLength + 1);
		memcpy(json->tokens[tokenIndex].string, begin, stringLength);
		tokenIndex++;
		pos++;
	 }
		else if (doc[pos] == '[')
		{
		json->tokens[tokenIndex].tokenIndex = tokenIndex;
		char *begin = doc + pos;
		json->tokens[tokenIndex].begin = pos;
		char *end = strchr(begin, ']');
		if (end == NULL) break;
		int stringLength = end - begin;
		json->tokens[tokenIndex].end = pos + stringLength;
		json->tokens[tokenIndex].stringLength = stringLength;
		json->tokens[tokenIndex].isBigList = true;
		json->tokens[tokenIndex].isArray = true;
		json->tokens[tokenIndex].string = malloc(stringLength + 1);
		memset(json->tokens[tokenIndex].string, 0, stringLength + 1);
		memcpy(json->tokens[tokenIndex].string, begin, stringLength);
		tokenIndex++;
		pos++;
	 }
		else if (doc[pos] == '0' 
		|| doc[pos] == '1'
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
	//	pos++;
		}
		else
		{
		pos++;
		}
		}
		}
	
	void freeJSON(JSON *json)
	{
		for (int i = 0; i < TOKEN_COUNT; i++)
		{
		if (json->tokens[i].isString == true)
		free(json->tokens[i].string);
		}
	}
	
	
	struct url_data {
	 size_t size;
	 char* data;
	};
		void printTotal(){
	
	 printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	 printf("1. Asia\n");
	 printf("2. Africa\n");
	 printf("3. EU\n");
	 printf("4. North America\n");
	 printf("5. South America\n");
	 printf("6. EXIT\n");
	 printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	
	 printf("menu: ");
	 
	}
	
	void printAsiaMenu(){
	
	 printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	 printf("1. Seou-Korea\n");
	 printf("2. Tokyo-Japan\n");
	 printf("3. Beijing-China\n");
	 printf("4. Manila-Philippines\n");
	 printf("5. Tbilisi-Gruziya\n");
	 printf("6. Kathmandu-Nepal\n");
	 printf("7. Dili-East Timor\n");
	 printf("8. Vientiane-Laos\n");
	 printf("9. Beirut-Lebanon\n");
	 printf("10. Kuala Lumpur-Malaysia\n");
	 printf("11. Male-Maldives\n");
	 printf("12. EXIT\n");
	 printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	
	 printf("menu: ");
	 
	}

	void printAfricaMenu(){
	 printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	 printf("1. Accra-Ghana\n");
	 printf("2. Abuja-Nigeria\n");
	 printf("3. Tlypolry-Libya\n");
	 printf("4. Nouakchott-Mauritania\n");
	 printf("5. Mdadane-Swaziland\n");

	 printf("6. EXIT\n");
	 printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	
	 printf("menu: ");
	}

	void printEuMenu(){
	 printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	 printf("1. Warsaw-Poland\n");
	 printf("2. Berlin-Germany\n");
	 printf("3. Riga-Latvia\n");
	 printf("4. Paris-France\n");
	 printf("5. Rome-Italy\n");
	 

	 
	 printf("6. EXIT\n");
	 printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	
	 printf("menu: ");
	}

	void printNorthMenu(){
	 printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	 printf("1. Washington-America\n");
	 printf("2. Ottawa-Canada\n");
	 printf("3. MexicoCity-Mexico\n");
	 printf("4. Kingston-Jamaica\n");
	 printf("5. Managua-Nicaragua\n");

	 printf("6. EXIT\n");
	 printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	
	 printf("menu: ");
	}
	
	void printSouthMenu(){
	 printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	 printf("1. Georgetown-Guyana\n");
	 printf("2. Brasilia-Brazil\n");
	 printf("3. Caracas-Venezuela\n");
	 printf("4. Lima-Peru\n");
	 printf("5. Santiago-Chile\n");

	 printf("6. EXIT\n");
	 printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	
	 printf("menu: ");
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
	
	
	
	int main(int argc, char* argv[]) {
	while(1){

	
	 FILE *file;
	 file = fopen("data1.txt","w+");
	 char* data1;
	
	int total;
	printTotal();
	scanf("%d",&total);
	int menu1;
	if(total ==1){
	printAsiaMenu();
	scanf("%d",&menu1);

	if(menu1==1){
		printf("seoul weather\n");
		data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Seoul&appid=ec80733cb38bf63779279c65ce1085e6");
	}else if(menu1==2){
		printf("japan weather\n");
		data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Tokyo&appid=ec80733cb38bf63779279c65ce1085e6");
	}else if(menu1==3){ 
		printf("china weather\n");
		data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Beijing&appid=ec80733cb38bf63779279c65ce1085e6");
	}else if(menu1==4){
		printf("Philippines weather\n");
		data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Manila&appid=ec80733cb38bf63779279c65ce1085e6");
	}else if(menu1 ==5){
		printf("Gruziya weather\n");//tbilisi
		data1=handle_url("https://api.openweathermap.org/data/2.5/weather?q=Tbilisi&appid=ec80733cb38bf63779279c65ce1085e6");
	}else if(menu1 == 6){
		printf("Nepal weather\n");
		data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Kathmandu&appid=ec80733cb38bf63779279c65ce1085e6");
	}else if(menu1 ==7){
		printf("East Timor weather\n");
		data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Dili&appid=ec80733cb38bf63779279c65ce1085e6");
	}else if(menu1 ==8){
		printf("Laos weather\n");
		data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Vientiane&appid=ec80733cb38bf63779279c65ce1085e6");
	}else if(menu1 == 9){
		printf("Lebanon weather\n");
		data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Beirut&appid=ec80733cb38bf63779279c65ce1085e6");
	}else if(menu1 ==10){
		printf("Malaysia weather\n");
		data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Kuala%20Lumpur&appid=ec80733cb38bf63779279c65ce1085e6");
	}else if(menu1 ==11){
		printf("Maldives weather\n");
		data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Male&appid=ec80733cb38bf63779279c65ce1085e6");
	}else if(menu1 == 12){
		return 0;
	
	}else{
		printf("Please give correct number\n");
		return 0;
	}
	
	}else if(total ==2){
		printAfricaMenu();
		scanf("%d",&menu1);
		if(menu1==1){
			printf("Ghana weather\n");
			data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Accra&appid=ec80733cb38bf63779279c65ce1085e6");
		}else if(menu1 ==2){
			printf("Nigeria weather\n");
			data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Abuja&appid=ec80733cb38bf63779279c65ce1085e6");
		}else if(menu1==3){
			printf("Zimbabwe weather\n");
			data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Harare&appid=ec80733cb38bf63779279c65ce1085e6");
		}else if(menu1 ==4){
			printf("Mauritania weather\n");
			data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Nouakchott&appid=ec80733cb38bf63779279c65ce1085e6");
		}else if(menu1 ==5){
			printf("Egypt weather\n");
			data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Cairo&appid=ec80733cb38bf63779279c65ce1085e6");
		}else if(menu1==6){
			return 0;
		}else{
		printf("please write correct number\n");
		return 0;
	}
	}else if(total ==3){
		printEuMenu();
		scanf("%d",&menu1);
		if(menu1==1){
			printf("Poland weather\n");
			data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Warsaw&appid=ec80733cb38bf63779279c65ce1085e6");
		}else if(menu1==2){
			printf("Germany weather\n");
			data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Berlin&appid=ec80733cb38bf63779279c65ce1085e6");
		}else if(menu1==3){
			printf("Latvia weather\n");
			data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Riga&appid=ec80733cb38bf63779279c65ce1085e6");
		}else if(menu1==4){
			printf("France weather\n");
			data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Paris&appid=ec80733cb38bf63779279c65ce1085e6");
		}else if(menu1==5){
			printf("Italy weather\n");
			data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Rome&appid=ec80733cb38bf63779279c65ce1085e6");
		}else if(menu1==6){
			return 0;
		}else{
		printf("please write correct number\n");
		return 0;
	}
	}else if(total == 4){
		printNorthMenu();
		scanf("%d",&menu1);
		if(menu1==1){
			printf("America weather\n");
			data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Washington&appid=ec80733cb38bf63779279c65ce1085e6");
		}else if(menu1==2){
			printf("Canada weather\n");
			data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Ottawa&appid=ec80733cb38bf63779279c65ce1085e6");
		}else if(menu1==3){
			printf("Mexico weather\n");
			data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Mexico&appid=ec80733cb38bf63779279c65ce1085e6");
		}else if(menu1==4){
			printf("weather\n");
			data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Kingston&appid=ec80733cb38bf63779279c65ce1085e6");
		}else if(menu1==5){
			printf("weather\n");
			data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Managua&appid=ec80733cb38bf63779279c65ce1085e6");
		}else if(menu1==6){
			return 0;
		}else{
		printf("please write correct number\n");
		return 0;
	}
	}else if(total == 5){
		printSouthMenu();
		scanf("%d",&menu1);
		if(menu1==1){
			printf("Guyana weather\n");
			data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Georgetown&appid=ec80733cb38bf63779279c65ce1085e6");
		}else if(menu1==2){
			printf("Brazil weather\n");
			data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Brasilia&appid=ec80733cb38bf63779279c65ce1085e6");
		}else if(menu1==3){
			printf("Venezuela weather\n");
			data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Caracas&appid=ec80733cb38bf63779279c65ce1085e6");
		}else if(menu1==4){
			printf("Peru weather\n");
			data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Lima&appid=ec80733cb38bf63779279c65ce1085e6");
		}else if(menu1==5){
			printf("Chile weather\n");
			data1 = handle_url("https://api.openweathermap.org/data/2.5/weather?q=Santiago&appid=ec80733cb38bf63779279c65ce1085e6");
		}else if(menu1==6){
			return 0;
		}else{
		printf("please write correct number\n");
		return 0;
	}
	}else if(total ==6){
		return 0;
	}else{
		printf("please write correct number\n");
		return 0;
	}


	 if(data1) {
	 //printf("%s\n", data1);
	 fprintf(file,"%s",data1);
	 free(data1);
	 }
		fclose(file);
		//	if (argv[1] == NULL) printf("please give json file name as argument\n");
		int size;
		//	char *doc = readFile(argv[1], &size);
		char *doc = readFile("data1.txt",&size);
		if (doc == NULL) return -1;
		JSON json = { 0, };
		size = strlen(doc);
		parseJSON(doc, size, &json);
		// for (int j = 0; j < TOKEN_COUNT; j++) {

		// if (json.tokens[j].isAll == true)
		// printf("[%d]: %s\n", json.tokens[j].tokenIndex, json.tokens[j].string);
		// else if (json.tokens[j].isString == true)
		// printf("[%d]: %s ( %d ~ %d, String )\n",
		// json.tokens[j].tokenIndex,
		// json.tokens[j].string,
		// json.tokens[j].begin,
		// json.tokens[j].end);
		// else if (json.tokens[j].isNumber == true)
		// printf("[%d]: %d ( %d ~ %d, Number )\n",
		// json.tokens[j].tokenIndex,
		// (int)json.tokens[j].number,
		// json.tokens[j].begin,
		// json.tokens[j].end);
		// else if (json.tokens[j].isBigList == true && json.tokens[j].isObjectList == true) {
		// printf("[%d]: %s } ", json.tokens[j].tokenIndex, json.tokens[j].string);
		// printf("( %d ~ %d, Object )\n", json.tokens[j].begin, json.tokens[j].end);
		// }
		// else if (json.tokens[j].isBigList == true && json.tokens[j].isArray == true) {
		// printf("[%d]: %s } ", json.tokens[j].tokenIndex, json.tokens[j].string);
		// printf("( %d ~ %d, Array )\n", json.tokens[j].begin, json.tokens[j].end);
		// }
		// else break;
		// }
		
			printf("country name : %s\n",json.tokens[55].string);
			printf("city name : %s\n",json.tokens[65].string);
			printf("weather : %s\n",json.tokens[13].string);
			printf("weather details: %s\n",json.tokens[15].string);
			printf("temp : %f\n",json.tokens[23].number-273.15);
			printf("humidity : %f\n",json.tokens[27].number);
			
			printf("wind speed : %f\n",json.tokens[37].number);
			printf("clouds : %f %%\n",json.tokens[43].number);
		
		freeJSON(&json);
		free(doc);
		
		
	
	
	}
	 return 0;
	}