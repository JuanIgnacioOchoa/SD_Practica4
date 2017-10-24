#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define NICK_SIZE 20
#define STR_SIZE 40

void *writeS(void *arg);
void *readS(void *arg);

// OH GOD YES
int sockfd;
pthread_t tid[2];

int main()
{
    int len;
    struct sockaddr_in address;

    int result;
    char *ch = 'A';

    /* Crear un socket para el cliente */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /* Nombrar el socket, de acuerdo con el server */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(9734);
    len = sizeof(address);
    result = connect(sockfd, (struct sockaddr *)&address, len);

    if (result == -1)
    {
        perror("Oops: client");
        exit(1);
    }
    char name[NICK_SIZE];
    int name_size = (int)(sizeof(name)*sizeof(char));

    printf("Ingresar nombre de usuario(Max 20 caracteres)\n");

    fgets(name, name_size, stdin);
    printf("Welcome %s_________________\n", name);

    write(sockfd, &name, name_size);

    pthread_create(&tid[0], NULL, writeS, (void *)&ch);
    pthread_create(&tid[1], NULL, readS, (void *)&ch);

    int i;
    for (i = 0; i < 2; i++)
    {
        pthread_join(tid[i], NULL);
    }

    free(ch);

    exit(0);
}
void *writeS(void *arg)
{
    char str[STR_SIZE];
    //printf("start writing thread\n");
    while (1)
    {
        //printf("scanning\n");
        fgets(str, STR_SIZE, stdin);
        //printf("%s\n", str);
        write(sockfd, &str, sizeof(str));
        sleep(1);
    }
    free(str);
    //return;
}
void *readS(void *arg)
{
    //printf("start reading thread\n");
    char str[STR_SIZE];
    int *n = (int *)arg;
    int mynum = *n;
    while (1)
    {
        read(sockfd, &str, sizeof(str));
        printf("%s\n", str);
        sleep(1);
    }
    free(str);
    free(n);
    //return;
}