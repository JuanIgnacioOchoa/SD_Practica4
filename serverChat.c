#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENT 10


int client_sockfd[MAX_CLIENT];
void *coneccion_clte(void *arg);
char user[10][20];
int totcltes = 0;
int main()
{
    int server_sockfd;
    int server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int parametro[MAX_CLIENT];
    pthread_t tid[MAX_CLIENT];
    /* Crear un socket sin nombre para el server */
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    /* Nombrar el socket */
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(9734);
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
    listen(server_sockfd, 5);
    printf("Server started\n");
    while (totcltes < MAX_CLIENT)
    {
        printf("clientes\n");
        char ch;
        client_sockfd[totcltes] = accept(server_sockfd,
                               (struct sockaddr *)&client_address, &client_len);
        parametro[totcltes] = totcltes;
        pthread_create(&tid[totcltes], NULL, coneccion_clte, (void *)&parametro[totcltes]);
        totcltes ++;
    }
    printf("Maximo de clientes alcanzado\n");
}
void *coneccion_clte(void *arg)
{
    
    char *ch;
    int  *n = (int *)arg;
    int mynum = *n;
    int i = 0;
    read(client_sockfd[mynum], &ch, 1);
    printf("El usuario %s ha ingresado al chat mynum %d\n",&ch, mynum);
    *user[mynum] = ch;
    while(1 == 1) {
        printf("reading\n");
        read(client_sockfd[mynum], &ch, 1);
        i = 0;
        while(i < totcltes)
        {
            //printf("writing %s to user %s\n",&ch, &user[i]);
           if(i != mynum){
                printf("writing %s to user %s\n",&ch, &user[i]);
                write(client_sockfd[i], &ch, 1);
           }
            i++;
        }
        sleep(1);
    }

    return;
}