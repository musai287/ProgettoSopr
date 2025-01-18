int ranaSuCroco(Frog *frog, Crocodile *croco, int numCroco) {
    for (int i = 0; i < numCroco; i++) {
        // Verifica se la rana e il coccodrillo si sovrappongono lungo l'asse x e y
        int frog_right = frog->base.x + frog->base.sprite.larghezza;
        int croco_right = croco[i].base.x + croco[i].base.sprite.larghezza;
        
        int frog_bottom = frog->base.y + frog->base.sprite.lunghezza;
        int croco_bottom = croco[i].base.y + croco[i].base.sprite.lunghezza;

        // Controlla se le aree degli oggetti si sovrappongono
        if (frog->base.x < croco_right && frog_right > croco[i].base.x && 
            frog->base.y < croco_bottom && frog_bottom > croco[i].base.y) {
            
            // La rana è sopra un coccodrillo, invia un evento
            Event evento;
            evento.tipo = 2;  // Evento che dice alla rana di seguire il coccodrillo
            if (croco[i].direction == 2) {
                evento.data = 2;
                //write(pipeEvent, &evento, sizeof(Event));
            } else if (croco[i].direction == 1) {
                evento.data = 1;
                //write(pipeEvent, &evento, sizeof(Event));     
            }
            return evento.data;  // Ritorna la direzione del coccodrillo
        }
    }
    return 0;  // Nessuna collisione trovata
}