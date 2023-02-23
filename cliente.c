/*
 * Programa cliente do teste de qualidade de rede Canhão UDP.
 * Objetivo: Receber uma alta quantidade de mensagens UDP e verificar 
 * a quantidade delas que ficam fora de ordem ou são perdidos.
 *
 * Autores: Thomas Bianci Todt e Gustavo Silveira Frehse
 * Disciplina: Redes II
 * Data da última atualização: 22/02/2023
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "defs.h"

int main(int argc, char *argv[]) {  
    int sockdescr;
    struct sockaddr_in sa;
    struct hostent *hp;
    char *host;
    char *port;

    if (argc != 3) {
        puts("Uso correto: cliente <nome-servidor> <porta>");
        exit(1);
    }

    host = argv[1];
    port = argv[2];

    if ((hp = gethostbyname(host)) == NULL) {
        perror("gethostbyname"); 
        puts("Não consegui obter endereco IP do servidor.");
        exit(1);
    }

    bcopy((char *)hp->h_addr_list[0], (char *)&sa.sin_addr, hp->h_length);

    sa.sin_family = hp->h_addrtype;

    sa.sin_port = htons(atoi(port));

    if ((sockdescr=socket(hp->h_addrtype, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        puts("Não consegui abrir o socket.");
        exit(1);
    }

    unsigned int seq = 0;
    for (int i = 0; i < NUM_MSGS; i++) {
        if (sendto(sockdescr, &seq, sizeof(seq), 0, (struct sockaddr *) &sa, sizeof(sa)) != sizeof(seq)) {
            perror("sendto");
            puts("Não consegui mandar os dados."); 
            exit(1);
        }

        seq++;
    }

    close(sockdescr);
    exit(0);
}

