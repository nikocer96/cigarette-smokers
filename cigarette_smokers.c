#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Ingredienti
enum Ingrediente { TABACCO, CARTA, FIAMMIFERI };

// Mutex e condizioni per gli ingredienti sul tavolo
pthread_mutex_t mutex_tavolo = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_ingrediente[3] = { PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER };

// Condizione e mutex per l'agente
pthread_mutex_t mutex_agente = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_agente = PTHREAD_COND_INITIALIZER;

int ingredienti_sul_tavolo[2] = { -1, -1 };

// Funzione eseguita dai thread dei fumatori
void *fumatore(void *arg) {
    int mio_ingrediente = *(int *)arg;

    while (1) {
        // Acquisisci il mutex del tavolo
        pthread_mutex_lock(&mutex_tavolo);

        // Attendi gli ingredienti necessari sul tavolo
        while (!(ingredienti_sul_tavolo[0] != mio_ingrediente && ingredienti_sul_tavolo[1] != mio_ingrediente)) {
            pthread_cond_wait(&cond_ingrediente[mio_ingrediente], &mutex_tavolo);
        }

        // Prendi gli ingredienti, crea e fuma la sigaretta
        printf("Fumatore %d: ho preso gli ingredienti e sto fumando...\n", mio_ingrediente);
        ingredienti_sul_tavolo[0] = ingredienti_sul_tavolo[1] = -1;

        // Rilascia il mutex del tavolo
        pthread_mutex_unlock(&mutex_tavolo);

        // Segnala all'agente che hai finito di fumare
        pthread_mutex_lock(&mutex_agente);
        pthread_cond_signal(&cond_agente);
        pthread_mutex_unlock(&mutex_agente);

        sleep(1);
    }
}

// Funzione eseguita dal thread dell'agente
void *agente(void *arg) {
    while (1) {
        // Acquisisci il mutex dell'agente
        pthread_mutex_lock(&mutex_agente);

        // Aspetta che un fumatore finisca di fumare
        pthread_cond_wait(&cond_agente, &mutex_agente);

        // Scegli casualmente due ingredienti diversi da mettere sul tavolo
        int ingrediente1 = rand() % 3;
        int ingrediente2 = (ingrediente1 + 1 + rand() % 2) % 3;
        printf("Agente: ho messo gli ingredienti %d e %d sul tavolo.\n", ingrediente1, ingrediente2);

        // Rilascia il mutex dell'agente
        pthread_mutex_unlock(&mutex_agente);

        // Acquisisci il mutex del tavolo
        pthread_mutex_lock(&mutex_tavolo);

        // Metti gli ingredienti sul tavolo e segnala ai fumatori
        ingredienti_sul_tavolo[0] = ingrediente1;
        ingredienti_sul_tavolo[1] = ingrediente2;

        // Segnala ai fumatori gli ingredienti sul tavolo
        for (int i = 0; i < 3; i++) {
            pthread_cond_signal(&cond_ingrediente[i]);
        }

        // Rilascia il mutex del tavolo
        pthread_mutex_unlock(&mutex_tavolo);
    }
}

int main() {
    // Crea e avvia i thread dei fumatori
    pthread_t thread_fumatori[3];
    int ingredienti[3] = { TABACCO, CARTA, FIAMMIFERI };
    for (int i = 0; i < 3; i++) {
        pthread_create(&thread_fumatori[i], NULL, fumatore, &ingredienti[i]);
    }

    // Crea e avvia il thread dell'agente
    pthread_t thread_agente;
    pthread_create(&thread_agente, NULL, agente, NULL);

    // Attendi i thread (in realtà, non termineranno mai)
    for (int i = 0; i < 3; i++) {
        pthread_join(thread_fumatori[i], NULL);
    }
    pthread_join(thread_agente, NULL);

    return 0;
}