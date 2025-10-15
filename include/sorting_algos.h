#ifndef SORTING_ALGOS_H
#define SORTING_ALGOS_H

#define MAX_SIZE 50

typedef enum { BUBBLE, INSERTION, SELECTION, MERGE } Algorithm;

typedef struct {
    int arr[MAX_SIZE];
    float pos[MAX_SIZE];
    int size;
    int comparisons;
    int swaps;
    int sortingStarted;
    int windowWidth;
    int windowHeight;
    Algorithm algo;
} SortState;

typedef struct {
    int sorted;
    int merging;
    int mergeSize;
    int mergeLeft;
} MergeState;

// Function declarations
void swap(SortState *state, int i, int j);
int bubbleStep(SortState *state, int *i, int *j);
int insertionStep(SortState *state, int *i, int *j);
int selectionStep(SortState *state, int *i, int *j, int *min_idx);
int mergeStep(SortState *state, MergeState *m);

#endif
