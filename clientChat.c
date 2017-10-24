#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void *writeS(void *arg);
void *readS(void *arg);
int sockfd;
pthread_t tid[2];
int main()
{
    
    int len;
    struct sockaddr_in address;
    int result;
    char* ch = 'A';
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
        perror("oops: client");
        exit(1);
    }
    printf("Ingresar nombre de usuario(Max 20 caracteres)\n");
    scanf("%s", &ch);
    printf("Welcome %c\n", ch);
    write(sockfd, &ch, 1);
    pthread_create(&tid[0], NULL, writeS, (void *)&ch);
    pthread_create(&tid[1], NULL, readS, (void *)&ch);
    while(1){
        sleep(5);
    }
    exit(0);
}
void *writeS(void *arg)
{
    char *ch;
    //printf("start writing thread\n");
    while(1) {
        //printf("scanning\n");
        scanf("%s", &ch);
        //printf("Write %s\n",&ch);
        write(sockfd, &ch, 1);
        sleep(1);
    }
    printf("bye\n");
    return;
}
void *readS(void *arg)
{
    //printf("start reading thread\n");
    char *ch;
    int  *n = (int *)arg;
    int mynum = *n;
    while(1) {
        read(sockfd, &ch,   1);
        printf("reading %s\n", &ch);
        sleep(1);
    }

    return;
}