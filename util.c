#include "util.h"

void initBuffer(BufferCircolare *bc) {
    bc->head = 0;
    bc->tail = 0;
    bc->count = 0;
    bc->terminato = 0;  // Flag per terminazione sicura
    pthread_mutex_init(&bc->mutex, NULL);
    pthread_cond_init(&bc->cond_non_vuoto, NULL);
    pthread_cond_init(&bc->cond_non_pieno, NULL);
}

void destroyBuffer(BufferCircolare *bc) {
    pthread_mutex_lock(&bc->mutex);
    bc->terminato = 1;  // Segnala la terminazione ai thread
    pthread_cond_broadcast(&bc->cond_non_vuoto);  // Sblocca eventuali consumatori
    pthread_cond_broadcast(&bc->cond_non_pieno);  // Sblocca eventuali produttori
    pthread_mutex_unlock(&bc->mutex);

    pthread_mutex_destroy(&bc->mutex);
    pthread_cond_destroy(&bc->cond_non_vuoto);
    pthread_cond_destroy(&bc->cond_non_pieno);
}

void produceMessaggio(BufferCircolare *bc, Messaggio msg) {
    pthread_mutex_lock(&bc->mutex);
    
    while (bc->count == BUFFER_SIZE && !bc->terminato)
        pthread_cond_wait(&bc->cond_non_pieno, &bc->mutex);
    
    if (bc->terminato) {
        pthread_mutex_unlock(&bc->mutex);
        return;  // Esci se il buffer è stato chiuso
    }

    bc->buffer[bc->tail] = msg;
    bc->tail = (bc->tail + 1) % BUFFER_SIZE;
    bc->count++;

    pthread_cond_signal(&bc->cond_non_vuoto);
    pthread_mutex_unlock(&bc->mutex);
}

Messaggio consumeMessaggio(BufferCircolare *bc) {
    pthread_mutex_lock(&bc->mutex);
    
    while (bc->count == 0 && !bc->terminato)
        pthread_cond_wait(&bc->cond_non_vuoto, &bc->mutex);
    
    if (bc->terminato && bc->count == 0) {
        pthread_mutex_unlock(&bc->mutex);
        Messaggio msg = { .tipo = -1 };  // Messaggio speciale per indicare chiusura
        return msg;
    }

    Messaggio msg = bc->buffer[bc->head];
    bc->head = (bc->head + 1) % BUFFER_SIZE;
    bc->count--;

    pthread_cond_signal(&bc->cond_non_pieno);
    pthread_mutex_unlock(&bc->mutex);
    
    return msg;
}
