/*
mi serveno le strutture perché devo dare ad event dei valori in corrispondezza a cosa succede
event 0 = non succede nulla caso base 
event 1 = coccodrillo fuori dallo schermo e ritorna alla posizione iniziale
event 2 = la rana è sopra il coccodrillo e segue il movimento del coccodrillo
event 3 = la rana è nel fiueme e muore
event 4 = la rana arriva alla tana e vince
ho frog.base.event = 0; come valore di default
ho croco.base.event = 0; come valore di default
nel ciclo del gioco, posso inserire una funzione che mi permette di 
controllare le posizioni e attivare gli eventi. 
in funzionamento ho sia rana che coccodrilli.
*/
#include "collisioni.h"

int ranaSuCroco(Frog *frog, Crocodile *croco, int numCroco, int pipeEvent) {
    for (int i = 0; i < numCroco; i++) {
        if (frog->base.x == croco[i].base.x && frog->base.y == croco[i].base.y) {
            // La rana è su un coccodrillo, invia un evento per segnalarlo
            Event evento;
            evento.tipo = 2;  // Tipo di evento per indicare che la rana è su un coccodrillo
             // Puoi memorizzare l'indice del coccodrillo o altre informazioni
            write(pipeEvent, &evento, sizeof(Event));
            return  croco[i].base.x;  // Restituisci l'indice del coccodrillo
        }
    }
    return 0;  // Nessun coccodrillo trovato
}