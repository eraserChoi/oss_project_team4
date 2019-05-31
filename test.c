#include <stdio.h>
#include <string.h>

int main ()
{
	char *string = "hello World";
	for (int i=0; i<strlen(string);i++){
		printf("%c\n",string[i]);
		printf("%s\n",string + i + 1);
	}
	return 0;
}
