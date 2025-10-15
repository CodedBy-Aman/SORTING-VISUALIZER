#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sorting_algos.h"

TTF_Font *font = NULL;

void drawArray(SDL_Renderer *renderer, SortState *state, int highlight1, int highlight2) {
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);

    int barWidth = state->windowWidth / state->size;
    for (int i=0;i<state->size;i++){
        if (i==highlight1 || i==highlight2) SDL_SetRenderDrawColor(renderer,255,0,0,255);
        else SDL_SetRenderDrawColor(renderer,0,255,0,255);

        int barHeight = state->arr[i]*(state->windowHeight-80)/state->size;
        SDL_Rect rect = {(int)state->pos[i], state->windowHeight-barHeight-20, barWidth-2, barHeight};
        SDL_RenderFillRect(renderer,&rect);
    }

    if (!state->sortingStarted) {
        SDL_Rect btn={state->windowWidth/2-60,10,120,40};
        SDL_SetRenderDrawColor(renderer,255,255,0,255);
        SDL_RenderFillRect(renderer,&btn);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderDrawRect(renderer,&btn);

        SDL_Color black={0,0,0,255};
        SDL_Surface *surf=TTF_RenderText_Solid(font,"START",black);
        SDL_Texture *tex=SDL_CreateTextureFromSurface(renderer,surf);
        int w,h;
        SDL_QueryTexture(tex,NULL,NULL,&w,&h);
        SDL_Rect tRect={btn.x+(btn.w-w)/2,btn.y+(btn.h-h)/2,w,h};
        SDL_RenderCopy(renderer,tex,NULL,&tRect);
        SDL_DestroyTexture(tex);
        SDL_FreeSurface(surf);
    }

    char algoText[50];
    sprintf(algoText,"Algorithm: %s",
        state->algo==BUBBLE?"Bubble Sort":
        state->algo==INSERTION?"Insertion Sort":
        state->algo==SELECTION?"Selection Sort":"Merge Sort");
    SDL_Color white={255,255,255,255};
    SDL_Surface *textSurf=TTF_RenderText_Solid(font,algoText,white);
    SDL_Texture *textTex=SDL_CreateTextureFromSurface(renderer,textSurf);
    SDL_Rect textRect={10,10,textSurf->w,textSurf->h};
    SDL_RenderCopy(renderer,textTex,NULL,&textRect);
    SDL_DestroyTexture(textTex);
    SDL_FreeSurface(textSurf);

    SDL_RenderPresent(renderer);
}

int main(int argc,char *argv[]){
    srand(time(NULL));
    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO)!=0){printf("SDL Error: %s\n",SDL_GetError()); return 1;}
    if (TTF_Init()!=0){printf("TTF Error: %s\n",TTF_GetError()); SDL_Quit(); return 1;}

    SDL_Window *win=SDL_CreateWindow("Sorting Visualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,800,600,SDL_WINDOW_RESIZABLE);
    SDL_Renderer *ren=SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);
    font=TTF_OpenFont("C:/Windows/Fonts/arial.ttf",24);

    SortState state={0};
    state.size=MAX_SIZE;
    state.windowWidth=800;
    state.windowHeight=600;
    state.sortingStarted=0;
    state.algo=BUBBLE;
    for(int i=0;i<state.size;i++){state.arr[i]=rand()%state.size+1; state.pos[i]=i*(state.windowWidth/state.size);}

    int bubble_i=0,bubble_j=0;
    int insertion_i=1,insertion_j=0;
    int selection_i=0,selection_j=1,selection_min=0;
    MergeState merge={0,0,0,0};

    int running=1;
    int delay=50;
    SDL_Event e;

    while(running){
        while(SDL_PollEvent(&e)){
            if(e.type==SDL_QUIT) running=0;
            if(e.type==SDL_MOUSEBUTTONDOWN && !state.sortingStarted){
                int x=e.button.x,y=e.button.y;
                if(x>state.windowWidth/2-60 && x<state.windowWidth/2+60 && y>10 && y<50) state.sortingStarted=1;
            }
            if(e.type==SDL_WINDOWEVENT && e.window.event==SDL_WINDOWEVENT_SIZE_CHANGED){
                state.windowWidth=e.window.data1;
                state.windowHeight=e.window.data2;
                for(int i=0;i<state.size;i++) state.pos[i]=i*(state.windowWidth/state.size);
            }
            if(e.type==SDL_KEYDOWN){
                if(e.key.keysym.sym==SDLK_1) state.algo=BUBBLE;
                if(e.key.keysym.sym==SDLK_2) state.algo=INSERTION;
                if(e.key.keysym.sym==SDLK_3) state.algo=SELECTION;
                if(e.key.keysym.sym==SDLK_4) state.algo=MERGE;
                if(e.key.keysym.sym==SDLK_UP && delay>5) delay-=5;
                if(e.key.keysym.sym==SDLK_DOWN && delay<500) delay+=5;

                for(int i=0;i<state.size;i++){state.arr[i]=rand()%state.size+1; state.pos[i]=i*(state.windowWidth/state.size);}
                bubble_i=bubble_j=0;
                insertion_i=1;insertion_j=0;
                selection_i=0;selection_j=1;selection_min=0;
                merge=(MergeState){0,0,0,0};
                state.sortingStarted=0;
            }
        }

        if(state.sortingStarted){
            if(state.algo==BUBBLE) bubbleStep(&state,&bubble_i,&bubble_j);
            else if(state.algo==INSERTION) insertionStep(&state,&insertion_i,&insertion_j);
            else if(state.algo==SELECTION) selectionStep(&state,&selection_i,&selection_j,&selection_min);
            else if(state.algo==MERGE) mergeStep(&state,&merge);
        }

        int h1 = state.algo==BUBBLE ? bubble_j :
          state.algo==INSERTION ? insertion_j+1 :
          state.algo==SELECTION ? selection_min :
          merge.merging ? merge.mergeLeft : 0;
        int h2 = state.algo==BUBBLE ? bubble_j+1 :
          state.algo==INSERTION ? insertion_j+1 :
          state.algo==SELECTION ? selection_j :
          merge.merging ? merge.mergeLeft + merge.mergeSize - 1 : 0;


        drawArray(ren,&state,h1,h2);
        SDL_Delay(delay);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();
    return 0;
}