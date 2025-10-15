#include "visualize.h"

void drawArray(SDL_Renderer* renderer, int* arr, int n, int highlight1, int highlight2) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    float width = 800.0f / n;
    for (int i = 0; i < n; i++) {
        if (i == highlight1 || i == highlight2)
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        else
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        SDL_FRect rect = { i * width, 600 - arr[i], width, arr[i] };
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_RenderPresent(renderer);
}

void visualizeDelay(Uint32 ms) {
    SDL_Delay(ms);
}
