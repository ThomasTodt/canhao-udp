/*
 * Programa servidor do teste de qualidade de rede Canhão UDP.
 * Objetivo: Mandar uma alta quantidade de mensagens UDP e verificar 
 * a quantidade delas que ficam fora de ordem ou são perdidos.
 *
 * Autores: Thomas Bianci Todt e Gustavo Silveira Frehse
 * Disciplina: Redes II
 * Data da última atualização: 22/02/2023
 */

#include "defs.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXHOSTNAME 30

int main (int argc, char *argv[]) {
    int sockdescr;
    struct sockaddr_in sa;
    struct hostent *hp;
    char localhost[MAXHOSTNAME];
    int recebidas = 0, foraDeOrdem = 0, perdeuOrdem = 0;

    if (argc != 2) {
        puts("Uso correto: servidor <porta>");
        exit(1);
    }

    gethostname(localhost, MAXHOSTNAME);

    if ((hp = gethostbyname(localhost)) == NULL) {
        perror("gethostbyname");
        puts("Não consegui meu próprio IP");
        exit(1);
    }

    bcopy((char *) hp->h_addr_list[0], (char *) &sa.sin_addr, hp->h_length);
    sa.sin_family = hp->h_addrtype;
    sa.sin_port = htons(atoi(argv[1]));

    if ((sockdescr = socket(hp->h_addrtype, SOCK_DGRAM, 0)) < 0) {
        perror("socket"); 
        puts("Não consegui abrir o socket.");
        exit(1);
    }
	printf("Consegui abrir o socket.\n");

    // Timeout
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    setsockopt(sockdescr, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)); 

    if (bind(sockdescr, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
        perror("bind");
        puts("Não consegui fazer o bind.");
        exit(1);
    }
	printf("Consegui fazer o bind.\n");

    unsigned int seqEsperado = 0;
    unsigned int msg;
	unsigned int ordena = 0;
    while (1) {
        if (recvfrom(sockdescr, &msg, sizeof(unsigned int), 0, NULL, NULL) < 0) {
            break;
        }

        if (msg < seqEsperado) {
            printf("Perda de ordem, esperava %u recebido %u\n", seqEsperado, msg);
            perdeuOrdem++;
        }

		if (msg < ordena) {
            printf("Mensagem fora de ordem, esperava %u recebido %u\n", seqEsperado, msg);
            foraDeOrdem++;
        }

       seqEsperado = msg + 1;
	   if (ordena <= msg) ordena = msg + 1; 
       recebidas++;

		if (!(recebidas % 100))
		{
			printf("\n--RECEBIDAS: %d\n", recebidas);
			printf("--FORA DE ORDEM: %d\n\n", foraDeOrdem);
			printf("--PERDEU ORDEM: %d\n\n", perdeuOrdem);
		}

        if (msg == NUM_MSGS - 1) {
            break;
        }
    }

    printf("Recebidas: %u / %u = %3.4f%%\n", recebidas, NUM_MSGS, 100.0 * (double) recebidas / (double) NUM_MSGS);
    printf("Fora de Ordem: %u = %3.4f%%\n", foraDeOrdem, 100.0 * (double) foraDeOrdem / (double) NUM_MSGS);
	printf("Perdeu ordem: %u = %3.4f%%\n", perdeuOrdem, 100.0 * (double) perdeuOrdem / (double) NUM_MSGS);

    return 0;
}
