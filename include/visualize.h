#ifndef VISUALIZE_H
#define VISUALIZE_H

#include <SDL3/SDL.h>

// Draw the array as vertical bars
void drawArray(SDL_Renderer* renderer, int* arr, int n, int highlight1, int highlight2);

// Pause for visualization
void visualizeDelay(Uint32 ms);

#endif
