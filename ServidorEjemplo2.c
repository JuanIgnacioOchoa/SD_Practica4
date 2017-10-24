#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdio.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>

        
#define MAX_CLIENT 10

int totcltes = 0;
int client_sockfd[MAX_CLIENT];
void *coneccion_clte(void *arg);

int main()
{
    int server_sockfd;
    int server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int parametro[MAX_CLIENT];
    pthread_t tid[MAX_CLIENT];
    int i;
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(9734);
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
    /* Crear una cola de conexiones */
    listen(server_sockfd, 5);
    while (totcltes < MAX_CLIENT)
    //
    {
        printf("server esperando, clientes atendidos %d\n", totcltes);
        /* Aceptar conexión */
        client_sockfd[totcltes] = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
        /* Crea un hilo que atiende al nuevo cliente */
        parametro[totcltes] = totcltes;
        pthread_create(&tid[totcltes], NULL, coneccion_clte, (void *)&parametro[totcltes]);
        totcltes++;
    }
    for (i = 0; i < MAX_CLIENT; i++)
        pthread_join(tid[i], NULL);
    
    return;
}
void *coneccion_clte(void *arg)
{
    char ch;
    int *n = (int *)arg;
    int mynum = *n;
    read(client_sockfd[mynum], &ch, 1);
    sleep(5);
    ch++;
    write(client_sockfd[mynum], &ch, 1);
    close(client_sockfd[mynum]);
    return;
}