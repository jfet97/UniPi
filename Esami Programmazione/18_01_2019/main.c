#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STOP "xxx;xxx;-1"
#define ERROR "Errore nell’input."

typedef struct s {
    char nome[21];
    char cognome[21];
    int voto;
    struct s* next;
} Studente;
typedef Studente* Elenco;

int leggiVoti(Elenco* elencoPtr);
void eraseNewLine(char* string);
int controllaDati(Studente* studentePtr);
void inserisciOrdinato(Elenco* elencoPtr, Studente* studentePtr);
void stampaElenco(Elenco elenco);
void filtraElencoSoloCompitini(Elenco* compitini, Elenco scritto);

int main(void) {
    Elenco elencoCompitini = NULL;
    if (leggiVoti(&elencoCompitini) == -1) {
        puts(ERROR);
        exit(1);
    }

    Elenco elencoScritto = NULL;
    if (leggiVoti(&elencoScritto) == -1) {
        puts(ERROR);
        exit(1);
    }

    stampaElenco(elencoCompitini);
    puts("------------");
    stampaElenco(elencoScritto);
    puts("------------");

    filtraElencoSoloCompitini(&elencoCompitini, elencoScritto);

    stampaElenco(elencoCompitini);
    puts("------------");

    return 0;
}

int leggiVoti(Elenco* elencoPtr) {
    char buffer[50];
    fgets(buffer, 50, stdin);
    eraseNewLine(buffer);

    while (strcmp(buffer, STOP) != 0) {
        Studente* studentePtr = malloc(sizeof(*studentePtr));
        if (studentePtr == NULL) {
            return -1;
        }

        if (sscanf(buffer, "%20[^;];%20[^;];%d", studentePtr->cognome, studentePtr->nome, &studentePtr->voto) != 3) {
            return -1;
        }

        if (controllaDati(studentePtr) == -1) {
            return -1;
        }

        studentePtr->next = NULL;
        inserisciOrdinato(elencoPtr, studentePtr);

        fgets(buffer, 50, stdin);
        eraseNewLine(buffer);
    }

    return 0;
}

void eraseNewLine(char* string) {
    char* nwLPtr = strchr(string, '\n');
    if (nwLPtr) {
        *nwLPtr = 0;
    }
}

int controllaDati(Studente* studentePtr) {
    size_t lunghezzaCognome = strlen(studentePtr->cognome);
    for (int i = 0; i < lunghezzaCognome; i++) {
        if (!isalpha(studentePtr->cognome[i])) {
            return -1;
        }
    }
    size_t lunghezzaNome = strlen(studentePtr->nome);
    for (int i = 0; i < lunghezzaNome; i++) {
        if (!isalpha(studentePtr->nome[i])) {
            return -1;
        }
    }

    int voto = studentePtr->voto;
    if (voto < 0 || voto > 30) {
        return -1;
    }

    return 0;
}

void inserisciOrdinato(Elenco* elencoPtr, Studente* studentePtr) {
    if (*elencoPtr == NULL) {
        *elencoPtr = studentePtr;
        return;
    } else {
        int comparazioneCognome = strcmp(studentePtr->cognome, (*elencoPtr)->cognome);
        int comparazioneNome = strcmp(studentePtr->nome, (*elencoPtr)->nome);

        int maggiore = comparazioneCognome > 0 || (comparazioneCognome == 0 && comparazioneNome > 0);
        if (maggiore) {
            // va messo più avanti
            inserisciOrdinato(&((*elencoPtr)->next), studentePtr);
        } else {
            // va messo qua
            studentePtr->next = *elencoPtr;
            *elencoPtr = studentePtr;
        }
    }
}

void stampaElenco(Elenco elenco) {
    if (elenco != NULL && elenco->next != NULL) {
        printf("%s %s %d -> ", elenco->nome, elenco->cognome, elenco->voto);
        stampaElenco(elenco->next);
    } else if (elenco != NULL) {
        printf("%s %s %d\n", elenco->nome, elenco->cognome, elenco->voto);
    }
}

void filtraElencoSoloCompitini(Elenco* compitini, Elenco scritto) {
    if (*compitini == NULL || scritto == NULL) {
        return;
    }

    int comparazione = strcmp((*compitini)->cognome, scritto->cognome);
    if (comparazione == 0) {
        comparazione = strcmp((*compitini)->nome, scritto->nome);
    }

    if (comparazione == 0) {
        Studente* temp = (*compitini);
        *compitini = (*compitini)->next;
        free(temp);
        filtraElencoSoloCompitini(compitini, scritto->next);
    } else if (comparazione > 1) {
        filtraElencoSoloCompitini(compitini, scritto->next);
    } else {
        filtraElencoSoloCompitini(&(*compitini)->next, scritto);
    }
}