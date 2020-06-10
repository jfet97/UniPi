#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELIMETER "0;0"

typedef struct info {
    int matricola;
    int voto1;
    int voto2;
    double media;
    struct info* next;
} Voto;

typedef Voto* Elenco;

void inserisciVotoInElenco(Elenco* elenco, Voto* votoPtr) {
    if (NULL == *elenco) {
        *elenco = votoPtr;
        return;
    }

    int medesimaMatricola = votoPtr->matricola == (*elenco)->matricola;
    if (medesimaMatricola) {
        (*elenco)->voto2 = votoPtr->voto2;
        (*elenco)->media = ((*elenco)->voto1 + (*elenco)->voto2) / (double)2;
        return;
    }

    int comparazione = votoPtr->voto1 > (*elenco)->voto1 || ((votoPtr->voto1 == (*elenco)->voto1) && (votoPtr->matricola < (*elenco)->matricola));

    if (comparazione) {
        votoPtr->next = *elenco;
        *elenco = votoPtr;
        return;
    } else {
        inserisciVotoInElenco(&(*elenco)->next, votoPtr);
        return;
    }
}

int inserisciVoti(Elenco* elenco, int numeroCompitino) {
    char buffer[15];
    fgets(buffer, 15, stdin);
    char* newlinePtr = strchr(buffer, '\n');
    if (newlinePtr) {
        *newlinePtr = '\0';
    }

    while (strcmp(buffer, DELIMETER) != 0) {
        Voto* votoPtr = malloc(sizeof(*votoPtr));
        if (NULL == votoPtr) {
            return -1;
        }

        int voto;

        if (sscanf(buffer, "%d;%d", &votoPtr->matricola, &voto) != 2 ||
            votoPtr->matricola < 10000 ||
            votoPtr->matricola > 99999 ||
            voto < 1 ||
            voto > 31) {
            return -1;
        }
        votoPtr->next = NULL;
        votoPtr->media = 0;

        if (1 == numeroCompitino) {
            votoPtr->voto1 = voto;
            votoPtr->voto2 = -1;
        } else if (2 == numeroCompitino) {
            votoPtr->voto2 = voto;
            votoPtr->voto1 = -1;
        }

        // printf("%d %d\n", votoPtr->matricola, votoPtr->voto);
        inserisciVotoInElenco(elenco, votoPtr);

        fgets(buffer, 15, stdin);

        newlinePtr = strchr(buffer, '\n');
        if (newlinePtr) {
            *newlinePtr = '\0';
        }
    }

    return 0;
}

void stampaElenco(Elenco elenco) {
    if (NULL == elenco) {
        return;
    }

    if (elenco->media > 18) {
        printf("%d\t%6.2lf\t%6.2lf\t%6.2lf\n", elenco->matricola, (double)elenco->voto1, (double)elenco->voto2, (double)elenco->media);
    }

    stampaElenco(elenco->next);
}

int main(void) {
    Elenco compitini = NULL;

    if (inserisciVoti(&compitini, 1) == -1) {
        puts("Errore nell'input.");
        exit(1);
    }

    if (inserisciVoti(&compitini, 2) == -1) {
        puts("Errore nell’input.");
        exit(1);
    }

    stampaElenco(compitini);
}
