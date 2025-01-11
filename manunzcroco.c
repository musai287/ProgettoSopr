





void createCrocodile(int *pipe, Crocodile *crocodiles, Game *game) {
    Crocodile newCroc;
    srand(time(NULL));

    int randDir = (rand() % 2 == 0) ? 1 : -1;

    
    for (int j = 0; j < LINES - 20; j++) { // Evita righe 0-4 e LINES-5 a LINES-1
        int rowspeed = rand () % 2 + game->crocSpeed; // Velocità casuale
        int rowSpawn = j + 12;
        
        if ((rowSpawn + 4) % 4 == 0) {
            randDir = randDir * (-1);
            for (int i = 0; i < MAX_CROCODILES; i++) {
                // Inizializza i valori prima di fork()
              int validPosition = 0;

                while (!validPosition) {
                    // Genera una posizione casuale
                    newCroc.cords.x = rand() % (COLS - CROCODILE_LENGTH) + 1; // Evita spawn oltre COLS
                    newCroc.cords.y = rowSpawn + 4;
                    newCroc.cords.direction = randDir;
                    newCroc.cords.source = (j * MAX_CROCODILES) + i + 1;
                    newCroc.cords.speed = rowspeed;
                    newCroc.sprite.length = CROCODILE_LENGTH;
                    newCroc.sprite.height = CROCODILE_HEIGHT;

                    // Assume che la posizione sia valida finché non si dimostra il contrario
                    validPosition = 1;

                    // Controlla se la posizione è occupata
                    for (int k = 0; k < MAX_CROCODILES * (LINES - 12); k++) {
                        if (crocodiles[k].cords.y == newCroc.cords.y && 
                            abs(crocodiles[k].cords.x - newCroc.cords.x) < (CROCODILE_LENGTH) + CROCODILE_SHIFT) {
                            validPosition = 0;
                            break;
                        }
                    }
                }
                pid_t pid = fork();

                if (pid < 0) {
                    perror("Fork failed");
                    exit(1);
                } else if (pid == 0) {
                    srand(time(NULL) + getpid());
                    moveCrocodile(pipe, &newCroc);
                    exit(0);
                } else {
                    newCroc.PID = pid;
                    crocodiles[(j * MAX_CROCODILES) + i] = newCroc;
                }
            }
        }
    }
}



