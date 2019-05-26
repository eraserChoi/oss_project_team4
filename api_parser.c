

#include <stdio.h>

#include <string.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <stdlib.h>

#include <unistd.h>


#define MYPORT 49612

#define BUF_SIZE 10000


char file[10000];


void sendIndex(int client) {

    send(client, "HTTP/1.1 200 OK\n", 16, 0);

    send(client, "Content-Type: text/html\n\n", 25, 0);

    FILE *fp = fopen("index.html", "r");

    while (fgets(file, 1000, fp) != NULL) {

        send(client, file, strlen(file), 0);

    }

    fclose(fp);

}


void sendQuery(int client) {

    send(client, "HTTP/1.1 200 OK\n", 16, 0);

    send(client, "Content-Type: text/html\n\n", 25, 0);

    FILE *fp = fopen("query.html", "r");

    while (fgets(file, 1000, fp) != NULL) {

        send(client, file, strlen(file), 0);

    }

    fclose(fp);

}

int main() {

    int sockfd, clientfd, str_len, i, req_len;

    struct sockaddr_in my_addr, client_addr;

    char *buf = (char *) malloc(BUF_SIZE * (sizeof(char)));

    char *req_info[50];

    char *strptr;

    socklen_t addr_len;


    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {

        printf("socket error\n");

        return 0;

    }


    memset(&my_addr, 0, sizeof(my_addr));

    my_addr.sin_family = AF_INET;   //IP v4

    my_addr.sin_port = htons(MYPORT);   //assign port NO

    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);    //IP addr


    if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(my_addr)) < 0) {

        printf("bind error\n");

        return -1;

    }

    addr_len = sizeof(client_addr);


    while (1) {


        if (listen(sockfd, 10) < 0) {

            printf("listen failed\n");

            return -1;

        }

        clientfd = accept(sockfd, (struct sockaddr *) &client_addr, &addr_len);


        if (clientfd < 0) {

            printf("accept error\n");

            return -1;

        }


        str_len = recv(clientfd, buf, BUF_SIZE - 1, 0);

        buf[str_len] = 0;
                strptr = strtok(buf, " ");

        req_info[0] = strptr;

        for (i = 1; strptr != NULL; ++i) {

            strptr = strtok(NULL, " ");

            req_info[i] = strptr;

        }

        req_len = i - 1;


        strptr = strtok(req_info[req_len - 1], "\n");

        for (i = 0; strptr != NULL; i++) {

            strptr = strtok(NULL, "\n");

            if (i == 1)

                req_info[req_len - 1] = strptr;

        }


        i = 0;


        printf("%s %s\n", req_info[0], req_info[1]);


        if (strcmp(req_info[0], "GET") == 0) {

            if (strcmp(req_info[1], "/") == 0 || strcmp(req_info[1], "/index.html") == 0) {

                sendIndex(clientfd);

            } else if (strcmp(req_info[1], "/query.html") == 0) {

                sendQuery(clientfd);

            } else {

                send(clientfd, "HTTP/1.1 404 NOT FOUND\n", 24, 0);

            }

        } else if (strcmp(req_info[0], "POST") == 0) {

            send(clientfd, "HTTP/1.1 200 OK\n", 16, 0);

            send(clientfd, "Content-Type: text/html\n\n", 25, 0);

            char text[100] = "<h2>";

            strcat(text, req_info[req_len - 1]);

            strcat(text, "</h2>");

            send(clientfd, text, strlen(text), 0);
                  } else {

            send(clientfd, "HTTP/1.1 404 NOT FOUND\n", 24, 0);

        }


        close(clientfd);

        shutdown(clientfd, 0);

    }


    close(sockfd);

    return 0;

}