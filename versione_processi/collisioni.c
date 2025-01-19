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

int ranaSuCrocoCheck(Frog *frog, Crocodile *croco, int numCroco) {
    // Estremi della rana
    int ranaSinistra = frog->base.x;
    int ranaDestra = frog->base.x + frog->base.sprite.larghezza;
    int ranaAlto = frog->base.y;
    int ranaBasso = frog->base.y + frog->base.sprite.lunghezza;

    for (int i = 0; i < numCroco; i++) {
        // Estremi del coccodrillo
        int crocoSinistra = croco[i].base.x;
        int crocoDestra = croco[i].base.x + croco[i].base.sprite.larghezza;
        int crocoAlto = croco[i].base.y;
        int crocoBasso = croco[i].base.y + croco[i].base.sprite.lunghezza;

        // Controlla se la rana è sopra il coccodrillo
        if (ranaDestra > crocoSinistra && ranaSinistra < crocoDestra &&
            ranaBasso > crocoAlto && ranaAlto < crocoBasso) {
            return i;  // La rana è sopra un coccodrillo
        }
    }
    return 0;  // La rana non è sopra alcun coccodrillo
}



int ranaSuCroco(Frog *frog, Crocodile *croco, int numCroco) {
    Event evento;

    if (frog->base.y > 7 && frog->base.y <= 15) {
        for (int i = 0; i < numCroco; i++){
            int sopraCoccodrillo = ranaSuCrocoCheck(frog, croco, numCroco);

            if (sopraCoccodrillo > 0) {
            // La rana è sopra un coccodrillo, invia l'evento per seguirlo
                evento.tipo = 2;
                evento.data = croco[sopraCoccodrillo].base.x;  // Puoi usare la posizione del coccodrillo specifico
                return evento.data;
            } else {
                return 0;  // La rana è nel fiume ma non sopra un coccodrillo
            }
        }
    }
    return 0;  // La rana non è nel fiume
}

int ranaInFiume(Frog *frog, Crocodile *croco, int numCroco) {
    // Verifica se la rana si trova nel fiume
    int SopraCroco = ranaSuCrocoCheck(frog, croco, numCroco);
    if (frog->base.y > 7 && frog->base.y <= 15 && !SopraCroco) {
        return 1;  // La rana è nel fiume
    }
    return 0;  // La rana non è nel fiume
}
int ranaInTanaCheck(Frog *frog, Map *tana) {
    // Estremi della rana
    int ranaSinistra = frog->base.x;
    int ranaDestra = frog->base.x + frog->base.sprite.larghezza;
    int ranaAlto = frog->base.y;
    int ranaBasso = frog->base.y + frog->base.sprite.lunghezza;

    for (int i = 0; i < 5; i++) {
        // Estremi del coccodrillo
        int tanaSx = tana[i].x;
        int tanaDx = tana[i].x + tana[i].sprite.larghezza;
        int tanaUp = tana[i].y;
        int tanaDw = tana[i].y + tana[i].sprite.lunghezza;

        // Controlla se la rana è sopra il coccodrillo
        if (ranaDestra > tanaSx && ranaSinistra < tanaDx &&
            ranaBasso > tanaUp && ranaAlto < tanaDw) {
            return i;  // La rana è sopra un coccodrillo
        }
    }
    return 0;  // La rana non è sopra alcun coccodrillo
}
int ranaInTana(Frog *frog, Map *tana) {
        for (int i = 0; i < 5; i++){
            int RanaBase = ranaInTanaCheck(frog, tana);

            if (RanaBase > 0) {
            // La rana è sopra un coccodrillo, invia l'evento per seguirlo
                  // Puoi usare la posizione del coccodrillo specifico
                return 1;
            } else {
                return 0;  // La rana è nel fiume ma non sopra un coccodrillo
            }
        }
    
    return 0;  // La rana non è nel fiume
}