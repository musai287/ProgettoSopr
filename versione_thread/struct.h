#pragma once
#include <ncurses.h>
#include <pthread.h>  // per i thread e i mutex/cond
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

// Parametri generici
#define DELAY 1000000
#define DELAYCLOSED 2000000

#define MAX_CROCO 24
#define BUFFER_SIZE 10



/* ---------- Sezione Sprite e Map ---------- */
typedef struct Sprite {
    short int larghezza; 
    short int lunghezza; 
    char pixels[10][10];
} Sprite;

typedef struct Map {
    Sprite sprite;
    short int x, y;
} Map;


struct SharedData;  // Dichiari che esiste una struct SharedData

/* ---------- Sezione Entity e sottotipi ---------- */
typedef struct Entity {
    short int x, y;
    Sprite sprite;
    short int id;  // 1 => frog, 2.. => coccodrilli, 60..61 => granate, 30 => proiettile
    void (*entity_move)(struct Entity *self, short int x, short int y);
    struct SharedData *sd;
} Entity;

typedef struct Frog {
    Entity base;
    short int lives;
} Frog;

typedef struct Crocodile {
    Entity base;
    short int direction; // 1 => destra, 2 => sinistra
} Crocodile;

/* ---------- Finestre ncurses ---------- */
typedef struct Fin {
    short int height;
    short int width;
    short int starty;
    short int startx;
} Fin;

extern WINDOW *vita, *gioco;

/* ---------- Funzioni di disegno/spostamento ---------- */
void antiStampaEntity(WINDOW *win, Entity *entity);
void entity_move(Entity *self, short int x, short int y);
void stampaEntity(WINDOW *win, Entity *entity);
void stampaMap(WINDOW *win, Map *map);

/* ---------- Sprite (extern) ---------- */
extern Sprite spriteGranata;
extern Sprite spriteProiettile;
extern Sprite spriteTana;
extern Sprite spriteTanaChiusa;
extern Sprite spriteRana;
extern Sprite spriteCrocodile;

/* ---------- Inizializzazione entità ---------- */
Map initTana();
Frog initFrog();
Entity initGranata();
Entity initProiettile();
Crocodile initCrocodile();

/* ---------- Tipi di messaggio per il buffer circolare ---------- */
typedef enum {
    MSG_MOVIMENTO,
    MSG_POSIZIONE,
    MSG_EVENTO
} TipoMessaggio;

typedef struct {
    TipoMessaggio tipo;
    int id;   // 1 => frog, 2.. => coccodrilli
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
            int info;
        } evento;
    } dati;
} Messaggio;

/* ---------- Buffer circolare ---------- */
typedef struct {
    Messaggio buffer[BUFFER_SIZE];
    int head;
    int tail;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t cond_non_vuoto;
    pthread_cond_t cond_non_pieno;
} BufferCircolare;

/* ---------- Struttura condivisa ---------- */
typedef struct {
    Frog frog;                  // ID=1
    Crocodile croco[MAX_CROCO]; // ID=2..(1+numCroco)
    Entity granata[2];          // ID=60..61
    Entity proiettile;          // ID=30
    int numCroco;
    int punteggio;
    int manche;
    int gameOver;  // Se 1 => i thread e il loop principale terminano

    pthread_mutex_t lock;  // Esempio di mutex su SharedData

    // Il buffer circolare che i thread useranno per inviare messaggi
    BufferCircolare buffer;
} SharedData;

/* Buffer circolare: prototipi delle funzioni */
void produceMessaggio(BufferCircolare *bc, Messaggio msg);
Messaggio consumeMessaggio(BufferCircolare *bc);
