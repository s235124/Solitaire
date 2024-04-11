#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
//#include <SDL.h>
//#include <ddraw.h>
//#include <SDL_image.h>

#define realpath(N,R) _fullpath((R),(N),_MAX_PATH)

typedef struct CardStruct {
    int number;
    char suit;
    char displayedChars[2];
    struct CardStruct* next;
    bool facingDown;
} Card;

// Subroutines in the program
//void sdlExample();
void show();
void load();
void printFileLines(const char *filename, Card* cards);

int main(int argc, char* argv[]) {
    //sdlExample();
    Card cards[52];

    char* relativePath = "../file.txt";
    char absolutePath[1024];

    if (realpath(relativePath, absolutePath) == NULL) {
        return 1;
    }

    printFileLines(absolutePath, &cards[0]);

    return 0;
}

void load () {

}

void printFileLines(const char *filename, Card* cards) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        return;
    }

    char line[256];
    int i = 0;

    while (fgets(line, sizeof(line), file)) {
        if (i == 0) {
            cards[i].displayedChars[0] = line[0];
            cards[i].displayedChars[1] = line[1];

            cards[i].next = 0;
        }
        else {
            cards[i].displayedChars[0] = line[0];
            cards[i].displayedChars[1] = line[1];

            cards[i].next = &(cards[i-1]);
        }

        printf("%c  %c\n", cards[i].displayedChars[0], cards[i].displayedChars[1]);

        i++;
    }

    fclose(file);
}
void columns (Card* finalCard) {

}

void show () {

}
/*
void sdlExample () {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        SDL_Log("Unable to initialize SDL image: %s", SDL_GetError());
        return;
    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 512, 512, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_Log("Error renderer creation: %s", SDL_GetError());
        return;
    }

    SDL_Surface* lettuce_sur = IMG_Load("/Users/mohammedismail/CLionProjects/Solitaire/BarackOsama.png");
    if (lettuce_sur == NULL) {
        SDL_Log("Error loading image: ", IMG_GetError());
        return;
    }

    SDL_Texture* lettuce_tex = SDL_CreateTextureFromSurface(renderer, lettuce_sur);
    if (lettuce_tex == NULL) {
        SDL_Log("Error creating texture");
        return;
    }

    SDL_FreeSurface(lettuce_sur);

    while (true) {
        SDL_Event e;
        if (SDL_WaitEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, lettuce_tex, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyTexture(lettuce_tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}*/