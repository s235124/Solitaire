#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <ddraw.h>
#include <SDL_image.h>

// Subroutines in the program
void sdlExample();

typedef struct CardStruct {
    int number;
    char suit;
    char displayedChars[2];
    struct CardStruct* next;
    bool facingDir;
} Card;

int main(int argc, char* argv[]) {

    sdlExample();

    return 0;
}

void sdlExample () {
    Card c;
    c.number = 5;
    c.suit = 'D';
    c.facingDir = true;
    c.displayedChars[0] = '5';
    c.displayedChars[1] = 'D';

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
    SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_Log("Error renderer creation: %s", SDL_GetError());
        return;
    }

    SDL_Surface* lettuce_sur = IMG_Load("C:\\Users\\ziabe\\CLionProjects\\Solitaire\\BarackOsama.png");
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

//    // Get the window's surface
//    SDL_Surface *surface = SDL_GetWindowSurface(window);
//
//    // Fill the surface with blue color
//    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 255));
//
//    // Update the surface
//    SDL_UpdateWindowSurface(window);
//
//    // Wait for 3 seconds before quitting
//    SDL_Delay(3000);

    // Cleanup
    SDL_DestroyTexture(lettuce_tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}