#include "collisioni.h"

/*
 Funzioni di collisione come da tuo codice.
 Attenzione: i coccodrilli hanno coordinate in croco[i].base.x/y
*/
int ranaSuCrocoCheck(Frog *frog, Crocodile *croco, int numCroco) {
    int ranaSinistra = frog->base.x;
    int ranaDestra  = frog->base.x + frog->base.sprite.larghezza;
    int ranaAlto    = frog->base.y;
    int ranaBasso   = frog->base.y + frog->base.sprite.lunghezza;

    for (int i = 0; i < numCroco; i++) {
        int cSinistra = croco[i].base.x;
        int cDestra   = croco[i].base.x + croco[i].base.sprite.larghezza;
        int cAlto     = croco[i].base.y;
        int cBasso    = croco[i].base.y + croco[i].base.sprite.lunghezza;

        if (ranaDestra > cSinistra && ranaSinistra < cDestra &&
            ranaBasso > cAlto     && ranaAlto     < cBasso) {
            return i+1;  // Indice coccodrillo +1
        }
    }
    return 0;
}

int ranaSuCroco(Frog *frog, Crocodile *croco, int numCroco) {
    // Se la rana è in una certa fascia y (fiume), controlliamo
    if (frog->base.y > 7 && frog->base.y <= 15) {
        int idx = ranaSuCrocoCheck(frog, croco, numCroco);
        if (idx > 0) {
            // Indica coccodrillo su cui si trova la rana
            return idx; 
        }
        // Altrimenti 0 => non sopra nessun coccodrillo
    }
    return 0; 
}

int ranaInFiume(Frog *frog, Crocodile *croco, int numCroco) {
    // Se la rana è nella fascia del fiume e non è su un coccodrillo
    if (frog->base.y > 7 && frog->base.y <= 15) {
        int idx = ranaSuCrocoCheck(frog, croco, numCroco);
        if (idx == 0) {
            return 1; // nel fiume
        }
    }
    return 0; 
}

int ranaInTanaCheck(Frog *frog, Map *tana) {
    int ranaSinistra = frog->base.x;
    int ranaDestra  = frog->base.x + frog->base.sprite.larghezza;
    int ranaAlto    = frog->base.y;
    int ranaBasso   = frog->base.y + frog->base.sprite.lunghezza;

    // Tana => 5 elementi
    for (int i = 0; i < 5; i++) {
        int tx = tana[i].x;
        int ty = tana[i].y;
        int tLarg = tana[i].sprite.larghezza;
        int tLung = tana[i].sprite.lunghezza;

        int tanaSx = tx;
        int tanaDx = tx + tLarg;
        int tanaUp = ty;
        int tanaDw = ty + tLung;

        if (ranaDestra  > tanaSx &&
            ranaSinistra < tanaDx &&
            ranaBasso    > tanaUp &&
            ranaAlto     < tanaDw ) {
            tana[i].sprite = spriteTanaChiusa;
            return i+1;
        }
    }
    return 0;
}

int ranaInTana(Frog *frog, Map *tana) {
    // Se > 0 => entra in una tana
    for (int i = 0; i < 5; i++) {
        if (ranaInTanaCheck(frog, tana) > 0) {
            return 1; 
        }
    }
    return 0;
}

int ranaProiettileCheck(Frog *frog, Entity *proiettile) {
    int rSx = frog->base.x;
    int rDx = frog->base.x + frog->base.sprite.larghezza;
    int rAlto = frog->base.y;
    int rBasso = frog->base.y + frog->base.sprite.lunghezza;

    if (rDx >= proiettile->x && rSx <= proiettile->x &&
        rBasso >= proiettile->y && rAlto == proiettile->y) {
        return 1;
    }
    return 0;
}

int ranaProiettile(Frog *frog, Entity *proiettile) {
    if (ranaProiettileCheck(frog, proiettile)) {
        return proiettile->id; // la rana è colpita
    }
    return 0;
}

int proiettileFuori(Entity *proiettile) {
    return (proiettile->x > COLS || proiettile->x <= -7);
}
