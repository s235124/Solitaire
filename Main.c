#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

typedef struct CardStruct {
    int number;
    char suit;
    char displayedChars[2];
    struct CardStruct* next;
    bool facingDir;
} Card;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Quit();

    printf("Hello world");
    return 0;
}