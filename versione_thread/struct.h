#pragma once
#include <ncurses.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#define DELAY 1000000
#define DELAYCLOSED 2000000

#define MAX_CROCO 24
#define BUFFER_SIZE 10


typedef struct Sprite {
    short int larghezza; 
    short int lunghezza; 
    char pixels[10][10];
} Sprite;

typedef struct Map {
    Sprite sprite;
    short int x, y;
} Map;

typedef struct Event {
    short int tipo;
    short int data;
} Event;

typedef struct Entity {
    short int x, y;
    Sprite sprite;
    short int id;
    // RIMOSSO pid_t pid;
    void (*entity_move)(struct Entity *self, short int x, short int y);
} Entity;

typedef struct Frog {
    Entity base;
    short int lives;
} Frog;

typedef struct Crocodile {
    Entity base;
    short int direction;
} Crocodile;

typedef struct Fin {
    short int height;
    short int width;
    short int starty;
    short int startx;
} Fin;

// Funzioni di disegno e spostamento
void antiStampaEntity(WINDOW *win, Entity *entity);
void entity_move(Entity *self, short int x, short int y);
void stampaEntity(WINDOW *win, Entity *entity);
void stampaMap(WINDOW *win, Map *map);

// Sprite globali
extern Sprite spriteGranata;
extern Sprite spriteProiettile;
extern Sprite spriteTana;
extern Sprite spriteTanaChiusa;
extern Sprite spriteRana;
extern Sprite spriteCrocodile;

// Funzioni di inizializzazione
Map initTana();
Frog initFrog();
Entity initGranata();
Entity initProiettile();
Crocodile initCrocodile();

// Finestre globali
extern WINDOW *vita, *gioco;

typedef struct {
    // Tutti i dati da condividere tra i thread
    Frog frog;
    Crocodile croco[MAX_CROCO];
    Entity granata[2];
    Entity proiettile;
    int numCroco;
    int punteggio;
    int manche;
    int gameOver;   // se 1, terminano i thread

    // Esempio di mutex (se vorrai proteggere accessi)
    pthread_mutex_t lock;
} SharedData;

typedef enum {
    MSG_MOVIMENTO,
    MSG_POSIZIONE,
    MSG_EVENTO,   // per eventi generici, come collisioni
    // altri tipi di aggiornamento
} TipoMessaggio;

typedef struct {
    TipoMessaggio tipo;
    int id;         // ad es. ID dell'entità che ha prodotto il messaggio
    union {
        struct {
            int dx;
            int dy;
        } movimento;
        struct {
            int x;
            int y;
        } posizione;
        struct {
            int info; // qualsiasi informazione extra
        } evento;
    } dati;
} Messaggio;


typedef struct {
    Messaggio buffer[BUFFER_SIZE];
    int head;
    int tail;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t cond_non_vuoto;
    pthread_cond_t cond_non_pieno;
} BufferCircolare;
