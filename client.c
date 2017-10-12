/****************** CLIENT ****************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(){
  int clientSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  /*---- Creer le socket, les 3 arguments sont: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP dans ce cas) */
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);

  /*---- Configurer les parametres de la structure d'adressedu serveur ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Definir le numero de port, en utilisant htons pour utiliser proprement byte order */
  serverAddr.sin_port = htons(7891);
  /* Definir l'@ IP à localhost */
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  /* Tous les bits du padding à 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  /*---- Connecter le socket au server en utilisant la structure d'adresse ----*/
  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

  /*---- Lire le msg provenant du serveur dans le buffer ----*/
  recv(clientSocket, buffer, 1024, 0);

  /*---- Afficher le message reçu ----*/
  printf("Data received: %s\n",buffer);

  return 0;
}
