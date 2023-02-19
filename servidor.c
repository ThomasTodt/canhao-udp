#include "defs.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
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
    int recebidas = 0, foraDeOrdem = 0;

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

    if (bind(sockdescr, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
        perror("bind");
        puts("Não consegui fazer o bind.");
        exit(1);
    }

    unsigned int seqEsperado = 0;
    unsigned int msg;
    while (1) {
        recvfrom(sockdescr, &msg, sizeof(unsigned int), 0, NULL, NULL);
        printf("Sou o servidor, recebi a mensagem----> %u\n", msg);

        if (msg != seqEsperado) {
            printf("Mensagem fora de ordem, esperava %u recebido %u\n", seqEsperado, msg);
            foraDeOrdem++;
        }

        seqEsperado++; // (?)
        recebidas++;

        if (msg == NUM_MSGS - 1) {
            break;
        }
    }

    printf("Recebidas: %u / %u = %3.2f%%\n", recebidas, NUM_MSGS, 100.0 * (double) recebidas / (double) NUM_MSGS);
    printf("Fora de Ordem: %u = %3.2f%%\n", foraDeOrdem, 100.0 * (double) foraDeOrdem / (double) NUM_MSGS);

    return 0;
}
