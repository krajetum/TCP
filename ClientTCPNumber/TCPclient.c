#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include "TCPclient.h"


int socket_id; // identificatore del socket
int TCPclient_connect(char ip_address[], unsigned short port_number)
{
     unsigned long binary_address = inet_network(ip_address);
     struct sockaddr_in add; // struttura per indirizzo

     // costruzione struttura indirizzo
     memset(&add, 0, sizeof(add)); // azzeramento struttura
     add.sin_family = AF_INET; // dominio indirizzi IP
     add.sin_addr.s_addr = htonl(binary_address); // indirizzo IP server
     add.sin_port = htons(port_number); // numero di porta server
     // apertura socket TCP
     if ((socket_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
       return -1;
     // connessione al socket del server
     if (connect(socket_id, (struct sockaddr*)&add, sizeof(add)) < 0)
         {
            close(socket_id);
            return -1;
         }
     // impostazione socket non bloccante
     if (fcntl(socket_id, F_SETFL, O_NONBLOCK) < 0)
         {
            close(socket_id);
            return -1;
         }
     return 0;
}

int TCPclient_receive(char buffer[], int size)
{
     int n;

     // ricezione dati (non bloccante)
     if ((n = recv(socket_id, (void*)buffer, (size_t)size, 0)) <= 0)
       return -1;
     return n;
}

int TCPclient_send(char* buffer, int num)
{
     int n;

     // trasmissione dati
     if ((n = send(socket_id, (void*)buffer, (size_t)num, 0)) < 0)
       return -1;
     return n;
}

void TCPclient_disconnect()
{
     // chiusura socket (disconnessione dal server)
     close(socket_id);
     return;
}

